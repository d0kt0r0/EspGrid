//
//  EspGrid.h
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

#import <Foundation/Foundation.h>
#import "EspInternalProtocol.h"
#import "EspBridge.h"
#import "EspOsc.h"
#import "EspClock.h"
#import "EspBeat.h"
#import "EspPeerList.h"
#import "EspChat.h"
#import "EspKeyValueController.h"
#import "EspCodeShare.h"
#import "EspMessage.h"
#import "EspQueue.h"

@interface EspGrid: NSObject <EspHandleOsc>
{
  EspPeerList* peerList;
  EspKeyValueController* kvc;
  EspInternalProtocol* internal;
  EspBridge* bridge;
  EspOsc* osc;
  EspClock* clock;
  EspBeat* beat;
  EspChat* chat;
  EspCodeShare* codeShare;
  EspMessage* message;
  EspQueue* queue;
  NSString* versionString;
  NSString* title;
  BOOL highVolumePosts;
}
@property (assign) EspPeerList* peerList;
@property (assign) EspKeyValueController* kvc;
@property (assign) EspInternalProtocol* internal;
@property (assign) EspBridge* bridge;
@property (assign) EspOsc* osc;
@property (assign) EspClock* clock;
@property (assign) EspBeat* beat;
@property (assign) EspChat* chat;
@property (assign) EspCodeShare* codeShare;
@property (assign) EspMessage* message;
@property (assign) EspQueue* queue;
@property (assign) NSString* versionString;
@property (assign) NSString* title;
@property (assign) BOOL highVolumePosts;

@end
