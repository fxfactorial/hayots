// -*- objc -*-

#pragma once

#import <Foundation/Foundation.h>

#import "hayots_ide.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (strong, nonatomic) NSWindow *main_window;
@property (strong, nonatomic) Hayots_IDE *app_logic;

@end
