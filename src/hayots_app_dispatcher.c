// -*- objc -*-

#import <Appkit/Appkit.h>

#import <iostream>

#import "MessagePack.h"

#import "hayots_app_dispatcher.h"

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

    auto queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);

    dispatch_async(queue, ^{
	auto *pipe = [NSPipe pipe];
	NSTask *t = [[NSTask alloc] init];
	[t setLaunchPath: @"/bin/bash"];
	[t setStandardOutput:pipe];
	[t setArguments:@[@"-c", @"nvim --api-info"]];
	[t launch];
	auto *file = [pipe fileHandleForReading];
	NSData *data = [file readDataToEndOfFile];
	NSDictionary *d = [data messagePackParse];
	NSLog(@"TEST: %@", d);

    });

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

-(void)send_reply:(NSDictionary*)reply_dict
{
  // Error is limited to 255 characters
  NSError *error = nil;
  NSData *d =
    [NSJSONSerialization
      dataWithJSONObject:reply_dict options:0 error:&error];

  uint8_t buffer[sizeof(int)];
  *buffer = (int)[d length];

  [self.writer write:buffer maxLength:1];
  [self.writer write:(const uint8_t * _Nonnull)[d bytes]
	   maxLength:[d length]];
}

-(void)handle_commands
{
  NSDictionary *dict = [self read_command];
  NSLog(@"Command dict: %@", dict);
  NSNumber *n = [dict objectForKey:@"child-pid"];
  if (n) self.ocaml_child_pid = [n intValue];

}

-(void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode;
{
  switch (eventCode) {
  case NSStreamEventHasBytesAvailable: {
    [self handle_commands];
    // NSLog(@"As a dict: %@", [self read_command]);
    [self send_reply:@{@"Foo":@"bar"}];
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
