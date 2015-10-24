//
//  EspOscSubscribers.h
//
//  This file is part of EspGrid.  EspGrid is (c) 2012-2015 by David Ogborn.
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

#import <Foundation/Foundation.h>
#import "EspSocket.h"
#import "EspHandleOsc.h"

@interface EspOscSubscribers : NSObject <EspHandleOsc>
{
    NSMutableArray* hosts;
    NSMutableArray* ports;
    EspSocket* socket;
    NSMutableArray* subscribers;
}
@property (nonatomic,assign) EspSocket* socket;

-(void) subscribeHost:(NSString*)host port:(int)port;
-(void) unsubscribeHost:(NSString*)host port:(int)port;
-(int) count;
-(void) sendData:(NSData*)data; // send data to all subscribers

@end
