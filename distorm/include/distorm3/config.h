/*
config.h

diStorm3 - Powerful disassembler for X86/AMD64
http://ragestorm.net/distorm/
distorm at gmail dot com
Copyright (C) 2003-2012 Gil Dabah

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef DISTORM_CONFIG_H
#define DISTORM_CONFIG_H

/* diStorm version number. */
#define DISTORM_VERSION 0x030300

#include <string.h> /* memset, memcpy - can be easily self implemented for libc independency. */

/*
 * If you compile diStorm as a dynamic library (.dll or .so) file, make sure you uncomment the next line.
 * So the interface functions will be exported, otherwise they are useable only for static library.
 * For example, this macro is being set for compiling diStorm as a .dll for Python with CTypes.
 */
/* #undef DISTORM_STATIC */

/*
 * 64 bit offsets support:
 * This macro should be defined from compiler command line flags, e.g: -DSUPPORT_64BIT_OFFSET
 * Note: make sure that the caller (library user) defines it too!
 */
#define DISTORM_SUPPORT_64BIT_OFFSET

/*
 * If DISTORM_DISABLE_FORMAT_CAPABILITIES is defined, everything involved in
 * formatting the instructions as text will be excluded from compilation.
 * distorm_decode(..) and distorm_format(..) will not be available.
 * This will decrease the size of the executable and leave you with decomposition functionality only.
 */
/* #undef DISTORM_DISABLE_FORMAT_CAPABILITIES */

/*
 * If DISTORM_DISABLE_BACKWARD_COMPATIBILITY is defined, all workarounds to
 * maintain backward compatibility will be disabled. Use it if you don't need
 * compatibility with legacy versions of distorm.
 */
/* #undef DISTORM_DISABLE_BACKWARD_COMPATIBILITY */

/* If DISTORM_LIGHT is defined, all the options above will take place */
/* #undef DISTORM_LIGHT */
#ifdef DISTORM_LIGHT
  #define DISTORM_DISABLE_BACKWARD_COMPATIBILITY
  #define DISTORM_DISABLE_FORMAT_CAPABILITIES
#endif

/* #undef DISTORM_CC_MSVC */
/* #undef DISTORM_CC_GNU */
#define DISTORM_CC_CLANG
/* #undef DISTORM_CC_INTEL */
/* #undef DISTORM_CC_TINYCC */
/* #undef DISTORM_CC_WATCOM */
/* #undef DISTORM_CC_UNKNOWN */

/*
 * diStorm now supports little/big endian CPU's.
 */
/* #undef DISTORM_ENDIAN_BE */

#define DISTORM_HAVE_STDINT_H
#ifdef DISTORM_HAVE_STDINT_H
  #include <stdint.h>
#endif

/* These macros are used in order to make the code portable. */
#if defined( DISTORM_CC_GNU ) || defined( DISTORM_CC_CLANG )
  
  #define DISTORM_DLLEXPORT __attribute__((visibility("default")))
  #define DISTORM_DLLIMPORT __attribute__((visibility("default")))
  
  #define DISTORM_FASTCALL /* __attribute__((fastcall)) */
  #define DISTORM_INLINE inline
  
#elif defined( DISTORM_CC_MSVC )
  
  #define DISTORM_DLLEXPORT __declspec(dllexport)
  #define DISTORM_DLLIMPOR  __declspec(dllimport)
  #define DISTORM_FASTCALL  __fastcall
  #define DISTORM_INLINE    __inline
  
  #ifndef DISTORM_HAVE_STDINT_H
    typedef signed    __int64 int64_t;
    typedef unsigned  __int64 uint64_t;
    typedef signed    __int32 int32_t;
    typedef unsigned  __int32 uint32_t;
    typedef signed    __int16 int16_t;
    typedef unsigned  __int16 uint16_t;
    typedef signed    __int8  int8_t;
    typedef unsigned  __int8  uint8_t;
  #endif /* DISTORM_HAVE_STDINT_H */
  
#elif defined( DISTORM_CC_WATCOM )
  
  #define DISTORM_DLLEXPORT
  #define DISTORM_DLLIMPORT
  #define DISTORM_FASTCALL
  #define DISTORM_INLINE __inline
  
#elif defined( DISTORM_CC_TINYCC )
  
  #define DISTORM_DLLEXPORT
  #define DISTORM_DLLIMPORT
  #define DISTORM_FASTCALL
  #define DISTORM_INLINE
  
  /* TINYC has a problem with some 64bits library functions, so ignore 64 bit offsets. */
  #undef DISTORM_SUPPORT_64BIT_OFFSET
  
#elif defined( __DMC__ )
  
  #define DISTORM_DLLEXPORT
  #define DISTORM_DLLIMPORT
  #define DISTORM_FASTCALL
  #define DISTORM_INLINE __inline
  
#else
  
  #define DISTORM_DLLEXPORT
  #define DISTORM_DLLIMPORT
  #define DISTORM_FASTCALL
  #define DISTORM_INLINE
  
#endif

#ifdef DISTORM_SUPPORT_64BIT_OFFSET
  typedef uint64_t OFFSET_INTEGER;
#else /* 32 bit offsets are used. */
  typedef unsigned long OFFSET_INTEGER;
#endif

/* If the library isn't compiled as a dynamic library don't export any functions. */
#ifdef DISTORM_STATIC
  #define DISTORM_EXPORT
#else
  #ifdef distorm3_EXPORTS
    #define DISTORM_EXPORT DISTORM_DLLEXPORT
  #else
    #define DISTORM_EXPORT DISTORM_DLLIMPORT
  #endif
#endif

#define DISTORM_FALSE 0
#define DISTORM_TRUE  1

#endif /* DISTORM_CONFIG_H */
