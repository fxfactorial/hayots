// -*- objc -*-

#pragma once

#import <Foundation/Foundation.h>

#import "hayots_ide.h"
#import "hayots_app_dispatcher.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (strong, nonatomic) Dispatcher *ml_dispatch;
@property (strong, nonatomic) NSWindow *main_window;
@property (strong, nonatomic) Hayots_IDE *app_logic;

@end
