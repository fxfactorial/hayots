// -*- objc -*-

#define CAML_NAME_SPACE

#import <Foundation/Foundation.h>
#import <Appkit/Appkit.h>

#import <caml/mlvalues.h>
#import <caml/alloc.h>
#import <caml/callback.h>
#import <caml/memory.h>
#import <caml/custom.h>
#import <caml/fail.h>
#import <caml/signals.h>

#import <string>
#import <iostream>

#import "hayots_app_delegate.h"
#import "hayots_app_dispatcher.h"

extern "C" {

  void
  hayots_ml_init_gui(value sock_for_child)
  {
    CAMLparam1(sock_for_child);

    @autoreleasepool {

      NSApplication *app = [NSApplication sharedApplication];
      [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
      CFSocketNativeHandle socket_handle =
	(CFSocketNativeHandle)Int_val(sock_for_child);
      CFReadStreamRef r = NULL;
      CFWriteStreamRef w = NULL;
      CFStreamCreatePairWithSocket(kCFAllocatorDefault,
				   socket_handle,
				   &r,
				   &w);

      Dispatcher *dispatcher = [[Dispatcher alloc]
				 initWithReader:(__bridge NSInputStream*)r
					 writer:(__bridge NSOutputStream*)w];
      AppDelegate *app_delegate = [AppDelegate new];
      app.delegate = app_delegate;

      app_delegate.ml_dispatch = dispatcher;
      dispatcher.app = app;
      [dispatcher.app run];
      CAMLnoreturn;
    }
  }

}

