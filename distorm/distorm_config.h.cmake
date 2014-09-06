/* diStorm3 3.3 */

/*
distorm_config.h

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

#ifndef DISTORM3_DISTORM_CONFIG_H
#define DISTORM3_DISTORM_CONFIG_H

/* WARNING: This file contains compile-time settings, which should be configured via cmake.
 * These settings affect the behaviour and/or the interface of the library. Do not edit!
 */

/*
 * If you compile diStorm as a dynamic library (.dll or .so) file, make sure you uncomment the next line.
 * So the interface functions will be exported, otherwise they are useable only for static library.
 * For example, this macro is being set for compiling diStorm as a .dll for Python with CTypes.
 */
#cmakedefine DISTORM_STATIC

/*
 * 64 bit offsets support:
 * This macro should be defined from compiler command line flags, e.g: -DSUPPORT_64BIT_OFFSET
 * Note: make sure that the caller (library user) defines it too!
 */
#cmakedefine DISTORM_SUPPORT_64BIT_OFFSET

/*
 * If DISTORM_DISABLE_FORMAT_CAPABILITIES is defined, everything involved in
 * formatting the instructions as text will be excluded from compilation.
 * distorm_decode(..) and distorm_format(..) will not be available.
 * This will decrease the size of the executable and leave you with decomposition functionality only.
 */
#cmakedefine DISTORM_DISABLE_FORMAT_CAPABILITIES

/*
 * If DISTORM_DISABLE_BACKWARD_COMPATIBILITY is defined, all workarounds to
 * maintain backward compatibility will be disabled. Use it if you don't need
 * compatibility with legacy versions of distorm.
 */
#cmakedefine DISTORM_DISABLE_BACKWARD_COMPATIBILITY

/* If DISTORM_LIGHT is defined, all the options above will take place */
#cmakedefine DISTORM_LIGHT
#ifdef DISTORM_LIGHT
  #define DISTORM_DISABLE_BACKWARD_COMPATIBILITY
  #define DISTORM_DISABLE_FORMAT_CAPABILITIES
#endif

#endif /* DISTORM3_DISTORM_CONFIG_H */
