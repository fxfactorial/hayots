// -*- objc -*-

#import <Cocoa/Cocoa.h>

#import <iostream>

#import "hayots_ide.h"
#import "MGSFragaria.h"

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
  NSTextField *f = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 50, 50)];
  f.stringValue = @"Hello world";
  [self addSubview:f];

  MGSFragaria *fragaria = [[MGSFragaria alloc] init];
  [fragaria setObject:self forKey:MGSFODelegate];
  [fragaria setObject:@"Objective Caml" forKey:MGSFOSyntaxDefinitionName];
  [fragaria embedInView:self];
  [fragaria setString:@"(* Hayots OCaml IDE *)\nlet () = print_endline \"Hello World\""];
}

-(void)speak_test
{
  std::cout << "Called in IDE\n";
}

@end
