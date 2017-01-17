// -*- objc -*-

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

#import <iostream>

#include "hayots_app_delegate.h"
#include "hayots_ide.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)a_notification
{
  int flags =
    NSWindowStyleMaskTitled
    | NSWindowStyleMaskResizable
    | NSWindowStyleMaskUnifiedTitleAndToolbar
    | NSWindowStyleMaskClosable
    | NSWindowStyleMaskMiniaturizable;

  auto screen_frame = [[NSScreen mainScreen] frame];
  self.main_window =
    [[NSWindow alloc]
      initWithContentRect:NSMakeRect(100, 100,
				     screen_frame.size.width - 200,
				     screen_frame.size.height - 200)
		styleMask:flags
		  backing:NSBackingStoreBuffered
		    defer:NO];

  NSToolbar *bar = [[NSToolbar alloc] initWithIdentifier:@"main_window_toolbar"];
  bar.displayMode = NSToolbarDisplayModeIconAndLabel;
  bar.sizeMode = NSToolbarSizeModeSmall;

  self.app_logic =
    [[Hayots_IDE alloc]
      initWithFrame:NSMakeRect(0, 0,
			       self.main_window.frame.size.width - 3,
			       self.main_window.frame.size.height - 20)];

  self.main_window.contentView = self.app_logic;

  [bar setDelegate:self.app_logic];
  self.main_window.toolbar = bar;

  [self.main_window setTitle:@"Hayots -- OCaml IDE"];
  [self.main_window makeKeyAndOrderFront:NSApp];
  // Make sure we're ahead of anything else.
  [self.main_window setLevel:NSNormalWindowLevel + 1];

}

- (void)applicationWillTerminate:(NSApplication *)application
{
  kill(self.ml_dispatch.ocaml_child_pid, 5);
}

-(void)applicationWillResignActive:(id)whatever
{
  // Be nice and go away
  [self.main_window setLevel:NSNormalWindowLevel];
}

@end
