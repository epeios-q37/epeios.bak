/*
	Copyright (C) 2014-2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

    This file is part of 'xdhcefq'.

    'xdhcefq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhcefq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhcefq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// For OS X only !

#import <Cocoa/Cocoa.h>

#include "include/cef_application_mac.h"

@interface SimpleAppDelegate
: NSObject<NSApplicationDelegate>
- (void)createApplication:(id)object;
- (void)tryToTerminateApplication:(NSApplication*)app;
@end

@interface SimpleApplication
: NSApplication<CefAppProtocol>
{
@private
  BOOL handlingSendEvent_;
}
@end

@implementation SimpleApplication
- (BOOL)isHandlingSendEvent
{
  return handlingSendEvent_;
}

- (void)setHandlingSendEvent:(BOOL)handlingSendEvent
{
  handlingSendEvent_ = handlingSendEvent;
}

- (void)sendEvent:(NSEvent*)event
{
  CefScopedSendingEvent sendingEventScoper;
  [super sendEvent:event];
}

- (void)terminate:(id)sender
{
  SimpleAppDelegate* delegate = static_cast<SimpleAppDelegate*>([NSApp delegate]);
  
  [delegate tryToTerminateApplication:self];
}
@end

@implementation SimpleAppDelegate
- (void)createApplication:(id)object
{
  [NSApplication sharedApplication];

  [NSBundle loadNibNamed:@"MainMenu" owner:NSApp];

  [[NSApplication sharedApplication] setDelegate:self];
}

- (void)tryToTerminateApplication:(NSApplication*)app
{}

- (NSApplicationTerminateReply)applicationShouldTerminate
: (NSApplication *)sender
{
  return NSTerminateNow;
}
@end

NSAutoreleasePool* autopool = NULL;

void Pre( void )
{
  autopool = [[NSAutoreleasePool alloc] init];

  [SimpleApplication sharedApplication];   
}

NSObject* delegate = NULL;

void Main( void )
{
  delegate = [[SimpleAppDelegate alloc] init];
  
  [delegate performSelectorOnMainThread:@selector(createApplication:)
                             withObject:nil
                          waitUntilDone:NO]; 
}

void Post( void )
{
  [delegate release];

  [autopool release];
 }
