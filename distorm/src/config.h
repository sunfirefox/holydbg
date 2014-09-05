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

#ifndef DISTORM_SRC_CONFIG_H
#define DISTORM_SRC_CONFIG_H

/* WARNING: This file is for internal use only. It contains internal compile-time settings
 * that are needed to ensure cross-platform compatibility of distorm.
 */

/*
 * diStorm now supports little/big endian CPU's.
 */
/* #undef DISTORM_BE_SYSTEM */

#define HAVE_STDINT_H

#define DISTORM_FALSE 0
#define DISTORM_TRUE  1

#endif /* DISTORM_SRC_CONFIG_H */
