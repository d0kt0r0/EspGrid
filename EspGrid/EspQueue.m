//
//  EspQueue.m
//
//  This file is part of EspGrid.  EspGrid is (c) 2012,2013 by David Ogborn.
//
//  EspGrid is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  EspGrid is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with EspGrid.  If not, see <http://www.gnu.org/licenses/>.

#import "EspQueue.h"
#import "EspGridDefs.h"

@implementation EspQueue
@synthesize delegate;


-(id) init
{
    self = [super init];
    items = [[NSMutableArray alloc] init];
    queueThread = [[NSThread alloc] initWithTarget:self selector:@selector(queueThreadMainMethod) object:nil];
    [queueThread start];
    return self;
}

-(void) queueThreadMainMethod
{
    [NSThread setThreadPriority: 0.99];
    [NSTimer scheduledTimerWithTimeInterval:0.010
                                     target:self
                                   selector:@selector(queueThreadLoop:)
                                   userInfo:nil
                                    repeats:NO];
    [[NSRunLoop currentRunLoop] run];
}


-(void) queueThreadLoop:(NSTimer*)timer
{
    if(![items count])
    {
        [NSTimer scheduledTimerWithTimeInterval:0.010
                                         target:self
                                       selector:@selector(queueThreadLoop:)
                                       userInfo:nil
                                        repeats:NO];
        return;
    }
    EspTimeType now = monotonicTime();
    for(NSArray* a in items)
    {
        EspTimeType t = [[a objectAtIndex:0] longLongValue];
        if(t<=now)
        {
            [delegate performSelectorOnMainThread:@selector(respondToQueuedItem:)
                                       withObject:[a objectAtIndex:1]
                                    waitUntilDone:YES];
            [items removeObject:a];
        }
    }
            
    if([items count])
    {
        [NSTimer scheduledTimerWithTimeInterval:0.001
                                         target:self
                                       selector:@selector(queueThreadLoop:)
                                       userInfo:nil
                                        repeats:NO];
    }
    else
    {
        [NSTimer scheduledTimerWithTimeInterval:0.010
                                         target:self
                                       selector:@selector(queueThreadLoop:)
                                       userInfo:nil
                                        repeats:NO];
    }
}


-(void) addItem:(id)item atTime:(EspTimeType)t
{
    // right now we are doing this with brute force
    // in the future, we should sort as we insert objects so that queueThreadLoop doesn't have to traverse the whole array!
    NSArray* a = [NSArray arrayWithObjects:[NSNumber numberWithDouble:t],item,nil];
    [items performSelector:@selector(addObject:) onThread:queueThread withObject:a waitUntilDone:NO];
}

@end
