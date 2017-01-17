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

@interface Dispatcher : NSObject <NSStreamDelegate>

@property (nonatomic, strong) NSApplication *app;
@property (nonatomic, strong) NSInputStream *reader;
@property (nonatomic, strong) NSOutputStream *writer;

-(instancetype)initWithReader:(NSInputStream*)r writer:(NSOutputStream*)w;
-(void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode;

@end

@implementation Dispatcher

-(instancetype)initWithReader:(NSInputStream*)r writer:(NSOutputStream*)w
{
  if (self = [super init]) {
    self.reader = r;
    self.writer = w;
    self.reader.delegate = self;
    self.writer.delegate = self;
    [self.reader scheduleInRunLoop:[NSRunLoop mainRunLoop]
			   forMode:NSDefaultRunLoopMode];
    [self.reader open];
    [self.writer open];
  }
  return self;
}

-(NSDictionary*)read_command
{
  NSError *error = nil;
  uint8_t read_length[4];
  [self.reader read:read_length maxLength:4];
  int size = (int)(*read_length);
  uint8_t buffer[size];
  [self.reader read:buffer maxLength:size];
  NSDictionary *json_command =
    [NSJSONSerialization
          JSONObjectWithData:[NSData dataWithBytes:buffer length:size]
                     options:0
                       error:&error];
  return json_command;
}

-(void)send_reply
{
  NSError *error = nil;
  NSData *d =
    [NSJSONSerialization
      dataWithJSONObject:@{@"foo": @"bar"} options:0 error:&error];
  uint8_t buffer[4];
  *buffer = (int)[d length];
  [self.writer write:buffer maxLength:4];
  [self.writer write:(const uint8_t * _Nonnull)[d bytes] maxLength:[d length]];
}

-(void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode;
{

  switch (eventCode) {
  case NSStreamEventHasBytesAvailable: {
    NSLog(@"As a dict: %@", [self read_command]);
    [self send_reply];
    break;
  }
  case NSStreamEventEndEncountered:
    NSLog(@"Client disconnected");
    break;
  case NSStreamEventNone:
    NSLog(@"What is this none event");
    break;
  case NSStreamEventOpenCompleted:
    break;
  case NSStreamEventErrorOccurred:
    NSLog(@"Event error");
    break;
  case NSStreamEventHasSpaceAvailable:
    NSLog(@"Event has space available");
    break;
  }
}

@end

extern "C" {

  void
  hayots_ml_init_gui(value sock_for_child)
  {
    CAMLparam1(sock_for_child);

    @autoreleasepool {

      NSApplication *app = [NSApplication sharedApplication];
      [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
      app.delegate = [AppDelegate new];
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
      dispatcher.app = app;
      [dispatcher.app run];
      CAMLnoreturn;
    }
  }

}

