/* tc-ssem.h -- Header file for tc-ssem.c.

   Copyright (C) 2018 Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with GAS; see the file COPYING.  If not, write to the Free Software
   Foundation, 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#define TARGET_ARCH             bfd_arch_ssem
#define TARGET_FORMAT           "elf32-ssem"
#define TARGET_BYTES_BIG_ENDIAN 0

#define WORKING_DOT_WORD

#define OCTETS_PER_BYTE_POWER 2

#define md_estimate_size_before_relax(A, B) (as_fatal (_("estimate size\n")), 0)
#define md_convert_frag(B, S, F)            as_fatal (_("convert_frag\n"))
