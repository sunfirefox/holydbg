#ifndef __HDBG_HDBG_DLL_HPP__
#define __HDBG_HDBG_DLL_HPP__

#if defined(__GNUC__)
#  define HOLYDBG_EXPORT __attribute__((visibility("default")))
#  define HOLYDBG_IMPORT __attribute__((visibility("default")))
#elif defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
#  define HOLYDBG_EXPORT __declspec(dllexport)
#  define HOLYDBG_IMPORT __declspec(dllimport)
#endif

#ifdef LIBHDBG_STATIC
#  define HDBG_API
#else
#  ifdef hdbg_EXPORTS
#    define HDBG_API HOLYDBG_EXPORT
#  else
#    define HDBG_API HOLYDBG_IMPORT
#  endif
#endif

#endif // __HDBG_HDBG_DLL_HPP__
