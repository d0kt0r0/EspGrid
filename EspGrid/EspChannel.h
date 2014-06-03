//
//  EspChannel.h
//  EspGrid
//
//  Created by David Ogborn on 2014-03-30.
//
//

#import <Foundation/Foundation.h>
#import "EspSocket.h"

@class EspChannel;

@protocol EspChannelDelegate
-(void) packetReceived:(NSDictionary*)packet fromChannel:(EspChannel*)channel;
@end

@interface EspChannel : NSObject <EspSocketDelegate>
{
    int port;
    NSString* host;
    EspSocket* socket;
    NSLock* lock;
    id delegate;
}
@property (nonatomic,assign) int port;
@property (nonatomic,copy) NSString* host;
@property (nonatomic,assign) id delegate;

-(void) sendDictionaryWithTimes:(NSDictionary*)d;
-(void) afterDataReceived:(NSDictionary*)plist;

@end
