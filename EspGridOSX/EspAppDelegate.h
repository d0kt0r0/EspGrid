//
//  EspAppDelegate.h
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

#import <Cocoa/Cocoa.h>
#import "EspGrid.h"
#import "EspDetailedPeerListController.h"

@interface EspAppDelegate : NSObject <NSApplicationDelegate>
{
    IBOutlet EspGrid* esp;
    
    IBOutlet NSTabView* tabView;
    
    // Main tab
    IBOutlet NSTextField* espClockAdjustment;
    IBOutlet NSTextField* espClockFlux;
    IBOutlet NSButton* beatOn;
    IBOutlet NSTextField* beatTempo;
    IBOutlet NSTextField* beatCycleLength;
    IBOutlet NSTextField* espChatMsg;
    IBOutlet NSTextView* espChatOutput;
    
    // Peers tab
    
    // Shared Code tab
    IBOutlet NSArrayController* codeShareController;
    
    // Log tab
    IBOutlet NSTextView* espLogOutput;
    IBOutlet NSButton* espLogOSC;
    
    // Bridge tab
    IBOutlet NSTextField* espBridgeLocalGroup;
    IBOutlet NSTextField* espBridgeLocalAddress;
    IBOutlet NSTextField* espBridgeLocalPort;
    IBOutlet NSTextField* espBridgeRemoteAddress;
    IBOutlet NSTextField* espBridgeRemotePort;
    IBOutlet NSTextField* espBridgeRemoteClaimedAddress;
    IBOutlet NSTextField* espBridgeRemoteClaimedPort;
    IBOutlet NSTextField* espBridgeRemoteGroup;
    IBOutlet NSTextField* espBridgeRemotePackets;
    
    // other (not a part of tabbed interface)
    IBOutlet NSUserDefaultsController* preferencesController;
    NSWindowController* preferencesPanel;
    EspDetailedPeerListController* detailedPeerList;
    
    IBOutlet NSMenuItem* tickOnBeats;

}
@property (assign) IBOutlet NSWindow *window;

// Main tab
-(IBAction)beatOn:(id)sender;
-(IBAction)beatTempo:(id)sender;
-(IBAction)beatCycleLength:(id)sender;

// Peers tab
-(IBAction)showDetailedPeerList:(id)sender;

// Shared Code tab
-(IBAction)grabShare:(id)sender;
-(IBAction)shareClipboard:(id)sender;

// Log tab
-(IBAction)logOSCChanged:(id)sender;

// Bridge tab
-(IBAction)bridgeLocalGroup:(id)sender;
-(IBAction)bridgeLocalAddress:(id)sender;
-(IBAction)bridgeLocalPort:(id)sender;
-(IBAction)bridgeRemoteAdddress:(id)sender;
-(IBAction)bridgeRemotePort:(id)sender;

-(IBAction)sendChatMessage:(id)sender;
-(IBAction)showPreferences:(id)sender;
-(IBAction)copyLogToClipboard:(id)sender;

// Help buttons
-(IBAction)helpPreferences:(id)sender;
-(IBAction)helpPeerList:(id)sender;
-(IBAction)helpMain:(id)sender;
-(IBAction)helpCode:(id)sender;

// other (not a part of tabbed interface)
-(void)postChatNotification:(NSNotification*)n;
-(void)postLogNotification:(NSNotification*)n;

-(IBAction)tickOnBeatsChanged:(id)sender;

@end
