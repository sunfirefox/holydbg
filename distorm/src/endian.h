/*
endian.h

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

#ifndef ENDIAN_H
#define ENDIAN_H

#include "config.h"

#ifdef DISTORM_BE_SYSTEM

static DISTORM_INLINE int16_t RSHORT(const uint8_t * s)
{
	return s[0] | (s[1] << 8);
}

static DISTORM_INLINE uint16_t RUSHORT(const uint8_t * s)
{
	return s[0] | (s[1] << 8);
}

static DISTORM_INLINE int32_t RLONG(const uint8_t * s)
{
	return s[0] | (s[1] << 8) | (s[2] << 16) | (s[3] << 24);
}

static DISTORM_INLINE uint32_t RULONG(const uint8_t * s)
{
	return s[0] | (s[1] << 8) | (s[2] << 16) | (s[3] << 24);
}

static DISTORM_INLINE int64_t RLLONG(const uint8_t * s)
{
	return s[0] | (s[1] << 8) | (s[2] << 16) | (s[3] << 24) |
					((uint64_t)s[4] << 32) | ((uint64_t)s[5] << 40) |
					((uint64_t)s[6] << 48) | ((uint64_t)s[7] << 56) ;
}

static DISTORM_INLINE uint64_t RULLONG(const uint8_t * s)
{
	return s[0] | (s[1] << 8) | (s[2] << 16) | (s[3] << 24) |
					((uint64_t)s[4] << 32) | ((uint64_t)s[5] << 40) |
					((uint64_t)s[6] << 48) | ((uint64_t)s[7] << 56) ;
}

#else

	#define RSHORT(x)  *(int16_t  *)x
	#define RUSHORT(x) *(uint16_t *)x
	#define RLONG(x)   *(int32_t  *)x
	#define RULONG(x)  *(uint32_t *)x
	#define RLLONG(x)  *(int64_t  *)x
	#define RULLONG(x) *(uint64_t *)x

#endif

#endif /* ENDIAN_H */
