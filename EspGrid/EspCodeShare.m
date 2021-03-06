//
//  EspCodeShare.m
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

#import "EspCodeShare.h"
#import "EspGridDefs.h"

@implementation  EspCodeShare
@synthesize items;

+(EspCodeShare*) codeShare
{
    static EspCodeShare* sharedObject = nil;
    if(!sharedObject)sharedObject = [[EspCodeShare alloc] init];
    return sharedObject;
}

-(id) init
{
    self = [super init];
    items = [[NSMutableArray alloc] init];
    network = [EspNetwork network];
    osc = [EspOsc osc];
    clock = [EspClock clock];
    return self;
}

-(NSUInteger) countOfShares
{
    return [items count];
}

-(void) shareCode:(NSString*)code withTitle:(NSString*)title
{
    EspCodeShareItem* item = [EspCodeShareItem createWithLocalContent:code title:title timeStamp:monotonicTime()];
    // *** NOTE: we have stamped codeshare items with local monotonic time
    // but we have not reworked codeshare system to adjust local times based on measured differences
    [self willChangeValueForKey:@"items"];
    [items addObject:item];
    [self didChangeValueForKey:@"items"];
    [item announceOnUdp:network];
}

-(NSString*) getOrRequestItem:(EspCodeShareItem*)item
{
    return [item getOrRequestContentOnUdp:network];
}

-(void) handleAnnounceShare:(NSDictionary*)d
{
    // 1. validate info in received opcode - if anything is missing or awry, abort with a warning
    NSString* name = [d objectForKey:@"sourceName"];
    if(name == nil) { postWarning(@"received ANNOUNCE_SHARE with no name",self); return; }
    if(![name isKindOfClass:[NSString class]]) { postWarning(@"received ANNOUNCE_SHARE with name not NSString",self); return; }
    if([name length]==0){ postWarning(@"received ANNOUNCE_SHARE with zero length name",self); return; }

    NSNumber* timeStamp = [d objectForKey:@"timeStamp"];
    if(timeStamp == nil) { postWarning(@"received ANNOUNCE_SHARE with no timeStamp",self); return; }
    if(![timeStamp isKindOfClass:[NSNumber class]]) { postWarning(@"received ANNOUNCE_SHARE with timeStamp not NSNumber",self); return; }

    NSString* title = [d objectForKey:@"title"];
    if(title == nil) { postWarning(@"received ANNOUNCE_SHARE with no title",self); return; }
    if(![title isKindOfClass:[NSString class]]) { postWarning(@"received ANNOUNCE_SHARE with title not NSString",self); return; }
    if([title length]==0){ postWarning(@"received ANNOUNCE_SHARE with zero length title",self); return; }

    NSNumber* length = [d objectForKey:@"length"];
    if(length == nil) { postWarning(@"received ANNOUNCE_SHARE with no length",self); return; }
    if(![length isKindOfClass:[NSNumber class]]) { postWarning(@"received ANNOUNCE_SHARE with length not NSNumber",self); return; }
    if([length longValue]<=0){ postWarning(@"received ANNOUNCE_SHARE with length <= 0",self); return; }

    // 2. see if this item is already in local array - if it is, ignore...
    EspCodeShareItem* item = nil;
    for(EspCodeShareItem* x in items)
    {
        if([x isEqualToName:name timeStamp:timeStamp])
        {
            item = x;
            break;
        }
    }

    // 3. if item is not already present, then create a new EspCodeShareItem and add it to local array
    if(item == nil)
    {
        item = [EspCodeShareItem createWithGridSource:name
                                                title:title
                                            timeStamp:[timeStamp doubleValue]
                                               length:[length longValue]];
        [self willChangeValueForKey:@"items"];
        [items addObject:item];
        [self didChangeValueForKey:@"items"];
        NSString *log = [NSString stringWithFormat:@"received ANNOUNCE_SHARE from %@ for timeStamp %@",name,timeStamp];
        postLog(log, self);
    }
}


-(void) handleRequestShare:(NSDictionary*)d
{
    NSString* sourceName = [d objectForKey:@"sourceName"];
    EspTimeType timeStamp = [[d objectForKey:@"timeStamp"] doubleValue];
    for(EspCodeShareItem* x in items)
    {
        if([[x sourceName] isEqualToString:sourceName] &&
           [x timeStamp] == timeStamp)
        {
            [x deliverAllOnUdp:network];
            return;
        }
    }
}


-(void) handleDeliverShare:(NSDictionary*)d
{
    NSString* sourceName = [d objectForKey:@"sourceName"];
    EspTimeType timeStamp = [[d objectForKey:@"timeStamp"] doubleValue];

    EspCodeShareItem* item;
    for(EspCodeShareItem* x in items)
    {
        if([[x sourceName] isEqualToString:sourceName] &&
           [x timeStamp] == timeStamp)
        {
            item = x;
            break;
        }
    }
    if(item == nil) return; // ? for now... later, receiving delivery of unknown items should start an entry

    NSString* fragment = [d objectForKey:@"fragment"];
    unsigned long index = [[d objectForKey:@"index"] longValue];
    [item addFragment:fragment index:index];
    // [self copyShareToClipboardIfRequested:item]; // factoring this out for cross-platform dvpmt
    NSLog(@"receiving DELIVER_SHARE for %@ with timeStamp %lld (%ld of %ld)",
          sourceName,timeStamp,index+1,[item nFragments]);
}

-(void) handleOpcode:(EspOpcode *)opcode
{
    NSAssert(false,@"empty new opcode handler called");
}

-(void) handleOldOpcode:(NSDictionary*)d
{
    int opcode = [[d objectForKey:@"opcode"] intValue];

    if(opcode == ESP_OPCODE_ANNOUNCESHARE) // receiving ANNOUNCE_SHARE
    {
        [self handleAnnounceShare:d];
    }
    else if(opcode == ESP_OPCODE_REQUESTSHARE) // receiving REQUEST_SHARE
    {
        NSString* l = [NSString stringWithFormat:@"receiving REQUEST_SHARE for %@ on %@",
                       [d valueForKey:@"timeStamp"],[d valueForKey:@"sourceName"]];
        postLog(l,self);
        // we should change this so that any grid instance can respond to a request if it has the goods...
        NSString* ourName = [[NSUserDefaults standardUserDefaults] stringForKey:@"person"];
        if([[d valueForKey:@"sourceName"] isEqual:ourName])
        {
          [self handleRequestShare:d];
        }
    }
    else if(opcode == ESP_OPCODE_DELIVERSHARE) // receiving DELIVER_SHARE
    {
        [self handleDeliverShare:d];
    }
}

-(BOOL) handleOsc:(NSString*)address withParameters:(NSArray*)d fromHost:(NSString*)h port:(int)p
{
    if([address isEqualToString:@"/esp/codeShare/post"])
    {
        if([d count]!=2){postProblem(@"received /esp/codeShare/post with wrong number of parameters",self); return NO;}
        [self shareCode:[d objectAtIndex:1] withTitle:[d objectAtIndex:0]];
        return YES;
    }
    return NO;
}

@end
