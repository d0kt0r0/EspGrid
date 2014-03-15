//
//  EspGridDefs.h
//
//  This file is part of EspGrid.  EspGrid is (c) 2012,2013 by David Ogborn.
//
//  EspGrid is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  EspGrid is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with EspGrid.  If not, see <http://www.gnu.org/licenses/>.

#ifndef EspGrid_EspGridDefs_h
#define EspGrid_EspGridDefs_h

#define ESPGRID_MAJORVERSION 0
#define ESPGRID_MINORVERSION 50 // changes to external/internal protocol MUST increment MINORVERSION
#define ESPGRID_SUBVERSION 3

#define ESP_NUMBER_OF_OPCODES 10
#define ESP_OPCODE_BEACON 0
#define ESP_OPCODE_ACK 1
#define ESP_OPCODE_CHATSEND 2
#define ESP_OPCODE_KVC 3
#define ESP_OPCODE_ANNOUNCESHARE 5
#define ESP_OPCODE_REQUESTSHARE 6
#define ESP_OPCODE_DELIVERSHARE 7
#define ESP_OPCODE_OSCNOW 8
#define ESP_OPCODE_OSCFUTURE 9

#define ESP_POST_CHAT 1
#define ESP_POST_LOG 2

#define VALIDATE_OPCODE_NSSTRING(vx) \
 do { \
    if(vx == nil) { postWarning(@"opcode with no " #vx,self); return NO; } \
    if(![vx isKindOfClass:[NSString class]]) { postWarning(@"opcode with " #vx " not NSString",self); return NO; } \
    if([vx length]==0){ postWarning(@"opcode with zero length " #vx,self); return NO; } \
 } while(0)

#define VALIDATE_OPCODE_NSNUMBER(vx) \
 do { \
    if(vx == nil) { postWarning(@"opcode with no " #vx,self); return NO; } \
    if(![vx isKindOfClass:[NSNumber class]]) { postWarning(@"opcode with " #vx " not NSString",self); return NO; } \
 } while(0)


void postChat(NSString* s);
void postWarning(NSString* s,id sender);
void postProblem(NSString* s,id sender);
void postLog(NSString* s,id sender);
void postLogHighVolume(NSString* s,id sender);

// uncomment the next line to compile on Windows (MINGW+GNUstep)
// #define MINGW 1

#import <sys/time.h>
#import <mach/mach_time.h>

typedef SInt64 EspTimeType;

inline static EspTimeType systemTime(void) {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec*1000000000) + (t.tv_usec*1000);
}

inline static EspTimeType monotonicTime(void) {
    return mach_absolute_time();
}

#ifdef GNUSTEP

#ifdef MINGW
// GNUSTEP/MINGW (Windows)
#include <stdlib.h>
typedef uint32_t UInt32;
typedef float Float32;
typedef double Float64;
inline static UInt32 EspSwapInt32(UInt32 x) { return htonl(x); }
inline static Float32 EspSwapFloat32(Float32 x) { return htonl(x); }
inline static Float64 EspSwapFloat64(double x) { return __builtin_bswap64(x); }
#endif

#ifndef MINGW
// GNUSTEP/Linux
#include <endian.h>
typedef uint32_t UInt32;
typedef float Float32;
typedef double Float64;
inline static UInt32 EspSwapInt32(UInt32 x) { return htobe32(x); }
inline static Float32 EspSwapFloat32(Float32 x) { return htobe32(x); }
inline static Float64 EspSwapFloat64(double x) { return htobe64(x); }
#endif

#endif

#ifndef GNUSTEP
// Cocoa/OSX
inline static UInt32 EspSwapInt32(UInt32 x) {return CFSwapInt32(x); }
inline static Float32 EspSwapFloat32(Float32 x) {
    CFSwappedFloat32 y = CFConvertFloatHostToSwapped(x);
    return *((Float32*)(&y));
}
    // return CFSwapInt32(*((UInt32*)&x));
inline static Float64 EspSwapFloat64(double x) { return CFSwapInt64(*((UInt64*)&x)); }
#endif

#endif
