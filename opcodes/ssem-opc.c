/* SSEM opcode list.
   Copyright (C) 2018 Free Software Foundation, Inc.

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING3. If not,
   see <http://www.gnu.org/licenses/>.  */

#include "sysdep.h"
#include "opcode/ssem.h"

/* Modern assembly mnemonics adopted from:
   Simon H. Lavington, A History of Manchester Computers. 1975.
   <https://archive.org/details/HistoryOfManchesterComputers>  */
const struct ssem_opcode ssem_opcodes[SSEM_CODE_NUM] =
{
  /* name, imm.  */
  { "jmp", TRUE },	/* 0: Absolute indirect unconditional jump.  */
  { "jrp", TRUE },	/* 1: Relative indirect unconditional jump.  */
  { "ldn", TRUE },	/* 2: Load negative.  */
  { "sto", TRUE },	/* 3: Store.  */
  { "sub", TRUE },	/* 4: Subtract.  */
  { NULL,  FALSE },	/* 5: Unused; decoded as sub.  */
  { "cmp", FALSE },	/* 6: Skip if accumulator is negative.  */
  { "stp", FALSE },	/* 7: Stop.  */
};
