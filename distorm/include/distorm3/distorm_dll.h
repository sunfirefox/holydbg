/* diStorm3 3.3 */

/*
distorm_dll.h

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

#ifndef DISTORM3_DISTORM_DLL_H
#define DISTORM3_DISTORM_DLL_H

/* These macros are used in order to make the code portable. */
#if defined( __GNUC__ ) || defined( __clang__ )
  
  #define DISTORM_EXPORT    __attribute__((visibility("default")))
  #define DISTORM_IMPORT    __attribute__((visibility("default")))
  #define DISTORM_FASTCALL  /* __attribute__((fastcall)) */
  #define DISTORM_INLINE    static inline
  
#elif defined( _MSC_VER ) || defined( __MINGW32__ ) || defined( __MINGW64__ )
  
  #define DISTORM_EXPORT    __declspec(dllexport)
  #define DISTORM_IMPOR     __declspec(dllimport)
  #define DISTORM_FASTCALL  __fastcall
  #define DISTORM_INLINE    __inline
  
#elif defined( __WATCOMC__ )
  
  #define DISTORM_EXPORT
  #define DISTORM_IMPORT
  #define DISTORM_FASTCALL
  #define DISTORM_INLINE __inline
  
#elif defined( __TINYC__ )
  
  #define DISTORM_EXPORT
  #define DISTORM_IMPORT
  #define DISTORM_FASTCALL
  #define DISTORM_INLINE
  
  /* TINYC has a problem with some 64bits library functions, so ignore 64 bit offsets. */
  #undef DISTORM_SUPPORT_64BIT_OFFSET
  
#elif defined( __DMC__ )
  
  #define DISTORM_EXPORT
  #define DISTORM_IMPORT
  #define DISTORM_FASTCALL
  #define DISTORM_INLINE __inline
  
#else
  
  #define DISTORM_EXPORT
  #define DISTORM_IMPORT
  #define DISTORM_FASTCALL
  #define DISTORM_INLINE
  
#endif

/* If the library isn't compiled as a dynamic library don't export any functions. */
#ifdef DISTORM_STATIC
#  define DISTORM_API
#else
#  ifdef distorm_EXPORTS
#    define DISTORM_API DISTORM_EXPORT
#  else
#    define DISTORM_API DISTORM_IMPORT
#  endif
#endif

#endif /* DISTORM3_DISTORM_DLL_H */
