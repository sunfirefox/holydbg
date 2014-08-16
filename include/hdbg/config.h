#ifndef __HDBG_CONFIG_H__
#define __HDBG_CONFIG_H__

#define HOLYDBG_MAKE_VERSION(major, minor, patch) ((major) << 16 | \
                                                   (minor) <<  8 | \
                                                   (patch) <<  0 )

#define HDBG_VERSION    HOLYDBG_MAKE_VERSION(0,2,0)
#define HOLYDBG_VERSION HOLYDBG_MAKE_VERSION(0,0,0)

/* #undef HOLYDBG_OS_WINDOWS */
#define HOLYDBG_OS_LINUX
/* #undef HOLYDBG_OS_OSX */

#define HOLYDBG_ARCH_X86_64
/* #undef HOLYDBG_ARCH_X86 */

/* #undef HOLYDBG_ENDIAN_BE */

/* #undef HOLYDBG_CXX_MSVC */
/* #undef HOLYDBG_CXX_GNU */
#define HOLYDBG_CXX_CLANG
/* #undef HOLYDBG_CXX_UNKNOWN */

#define LIBHDBG_STATIC
/* #undef hdbg_EXPORTS */

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

#endif // __HDBG_CONFIG_H__
