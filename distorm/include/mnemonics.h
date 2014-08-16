/*
mnemonics.h

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

#ifndef MNEMONICS_H
#define MNEMONICS_H

#include <distorm3/mnemonics.h>

/* We don't want to break the interface */
typedef D3_RegisterType _RegisterType;

#ifndef DISTORM_REMOVE_FORMAT_FUNCTIONALITY
  typedef D3_WMnemonic  _WMnemonic;
  typedef D3_WRegister  _WRegister;
#endif /* DISTORM_REMOVE_FORMAT_FUNCTIONALITY */

#endif /* MNEMONICS_H */
