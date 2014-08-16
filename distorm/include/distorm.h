/*
distorm.h

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

#ifndef DISTORM_H
#define DISTORM_H

#include <distorm3/distorm.h>

/* We don't want to break the interface */

#define __DISTORMV__ DISTORM_VERSION

typedef D3_DecodeType   _DecodeType;
typedef D3_OffsetType   _OffsetType;
typedef D3_CodeInfo     _CodeInfo;
typedef D3_OperandType  _OperandType;
typedef D3_Value        _Value;
typedef D3_Operand      _Operand;
typedef D3_DInst        _DInst;
typedef D3_DecodeResult _DecodeResult;

#ifndef DISTORM_REMOVE_FORMAT_CAPABILITIES
  typedef D3_WString      _WString;
  typedef D3_DecodedInst  _DecodedInst;
#endif /* DISTORM_REMOVE_FORMAT_CAPABILITIES */

#endif /* DISTORM_H */
