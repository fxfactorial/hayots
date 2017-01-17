// -*- objc -*-

#import <Cocoa/Cocoa.h>

#import <iostream>

#include "hayots_ide.h"

@implementation Hayots_IDE : NSView

-(instancetype)initWithFrame:(NSRect)f
{
  if ((self = [super initWithFrame:f])) {
    [self setup_menus];
    [self setup_main_interface];
  }
  return self;
}

- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar
     itemForItemIdentifier:(NSString *)itemIdentifier
 willBeInsertedIntoToolbar:(BOOL)flag
{
  // If You want to add a new toolbar item, make it here and add the
  // identifier to the other two methods related to the toolbar
  return nil;
}

- (NSArray*)toolbarAllowedItemIdentifiers:(NSToolbar *)toolbar
{
  return @[NSToolbarShowColorsItemIdentifier, NSToolbarShowFontsItemIdentifier];
}

- (NSArray*)toolbarDefaultItemIdentifiers:(NSToolbar *)toolbar
{
  return @[NSToolbarShowColorsItemIdentifier, NSToolbarShowFontsItemIdentifier];
}

-(void)setup_menus
{
  NSMenu *menu_bar = [NSMenu new];
  NSMenu *app_menu = [NSMenu new];

  NSMenuItem *app_item = [NSMenuItem new];

  NSMenuItem *quit_item =
    [[NSMenuItem alloc] initWithTitle:@"Quit Hayots"
			       action:@selector(terminate:)
			keyEquivalent:@"q"];
  NSArray *add_these = @[quit_item, [NSMenuItem separatorItem]];

  for (id iter in add_these) [app_menu addItem:iter];

  [menu_bar addItem:app_item];
  [app_item setSubmenu:app_menu];
  [NSApp setMainMenu:menu_bar];
}

-(void)setup_main_interface
{
  NSTextView *v = [[NSTextView alloc] init];
  [v setTextColor:[NSColor greenColor]];
  [self addSubview:v];
}

-(void)speak_test
{
  std::cout << "Called in IDE\n";
}

@end
