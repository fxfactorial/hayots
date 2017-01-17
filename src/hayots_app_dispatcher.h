// -*- objc -*-

#pragma once

#import <Appkit/Appkit.h>

@interface Dispatcher : NSObject <NSStreamDelegate>

@property (nonatomic, strong) NSApplication *app;
@property (nonatomic, strong) NSInputStream *reader;
@property (nonatomic, strong) NSOutputStream *writer;
@property int ocaml_child_pid;

-(instancetype)initWithReader:(NSInputStream*)r writer:(NSOutputStream*)w;
-(void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode;

-(NSDictionary*)read_command;
-(void)send_reply:(NSDictionary*)reply_dict;

@end


