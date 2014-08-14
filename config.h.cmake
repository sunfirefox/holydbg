#ifndef __HOLYDBG_CONFIG_H__
#define __HOLYDBG_CONFIG_H__

#define HOLYDBG_VERSION @holydbg_VERSION@

#cmakedefine HOLYDBG_OS_WINDOWS
#cmakedefine HOLYDBG_OS_LINUX
#cmakedefine HOLYDBG_OS_OSX

#cmakedefine HOLYDBG_ARCH_X86_64
#cmakedefine HOLYDBG_ARCH_X86

#cmakedefine HOLYDBG_ENDIAN_BE

#cmakedefine HOLYDBG_CXX_MSVC
#cmakedefine HOLYDBG_CXX_GNU
#cmakedefine HOLYDBG_CXX_CLANG
#cmakedefine HOLYDBG_CXX_UNKNOWN

#cmakedefine LIBHDBG_STATIC
#cmakedefine hdbg_EXPORTS

#if defined( HOLYDBG_CXX_MSVC )
#  define HOLYDBG_DLLEXPORT __declspec(dllexport)
#  define HOLYDBG_DLLIMPORT __declspec(dllimport)
#elif defined( HOLYDBG_CXX_GNU ) || defined( HOLYDBG_CXX_CLANG )
#  define HOLYDBG_DLLEXPORT __attribute__((visibility("default")))
#  define HOLYDBG_DLLIMPORT __attribute__((visibility("default")))
#else // HOLYDBG_CXX_UNKNOWN
#  define HOLYDBG_DLLEXPORT
#  define HOLYDBG_DLLIMPORT
#endif

#ifdef LIBHDBG_STATIC
#  define HDBG_EXPORT
#else // LIBHDBG_STATIC
#  if defined( hdbg_EXPORTS )
#    define HDBG_EXPORT HOLYDBG_DLLEXPORT
#  else
#    define HDBG_EXPORT HOLYDBG_DLLIMPORT
#  endif
#endif // LIBHDBG_STATIC

#endif // __HOLYDBG_CONFIG_H__
