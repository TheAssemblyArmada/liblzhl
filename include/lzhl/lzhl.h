/*
 *  LZH-Light algorithm implementation v 1.0
 *  Copyright (C) Sergey Ignatchenko 1998
 *  This  software  is  provided  "as is"  without  express  or implied 
 *  warranty.
 *
 *  Permission to use, copy, modify, distribute and sell this  software 
 *  for any purpose is hereby  granted  without  fee,  subject  to  the 
 *  following restrictions:
 *  1. this notice may not be removed or altered;
 *  2. altered source versions must be plainly marked as such, and must 
 *     not be misrepresented as being the original software.
 */

#ifndef __lzhl_h
#define __lzhl_h

#include <stddef.h>

typedef struct { int _; }* LZHL_CHANDLE;
typedef struct { int _; }* LZHL_DHANDLE;
#define LZHL_CHANDLE_NULL ((LZHL_CHANDLE)0)
#define LZHL_DHANDLE_NULL ((LZHL_DHANDLE)0)

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#if !defined _MSC_VER && !defined __WATCOMC__
#if __has_attribute(cdecl) && defined __i386__
#define LZHL_ABI __attribute__((cdecl))
#else
#define LZHL_ABI
#endif
#else
#define LZHL_ABI __cdecl
#endif

#ifdef LZHL_SHARED
#define LZHL_EXPORT __attribute__(visibility("default"))
#else
#define LZHL_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

LZHL_CHANDLE LZHL_ABI LZHL_EXPORT LZHLCreateCompressor( void );
size_t LZHL_ABI LZHL_EXPORT LZHLCompressorCalcMaxBuf( size_t );
size_t LZHL_ABI LZHL_EXPORT LZHLCompress( LZHL_CHANDLE, void* dst, const void* src, size_t srcSz );
void LZHL_ABI LZHL_EXPORT LZHLDestroyCompressor( LZHL_CHANDLE );

LZHL_DHANDLE LZHL_ABI LZHL_EXPORT LZHLCreateDecompressor( void );
int  LZHL_ABI LZHL_EXPORT LZHLDecompress( LZHL_DHANDLE, void* dst, size_t* dstSz, void* src, size_t* srcSz );
void LZHL_ABI LZHL_EXPORT LZHLDecompressReset( LZHL_DHANDLE );
void LZHL_ABI LZHL_EXPORT LZHLDestroyDecompressor( LZHL_DHANDLE );

#ifdef __cplusplus
}
#endif

#endif
