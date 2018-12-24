/* SSEM opcode list.
   Copyright (C) 2018 Free Software Foundation, Inc.

   This file is part of GDB and GAS.

   GDB and GAS are free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GDB and GAS are distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GDB or GAS; see the file COPYING3.  If not, write to
   the Free Software Foundation, 51 Franklin Street - Fifth Floor,
   Boston, MA 02110-1301, USA.  */

#include "bfd.h"
#include "bfd_stdint.h"

/* The original instruction format featured a 3-bit function code and a
   5-bit immediate field used to address a 32-line store.

    31........16 15..13 12..5 4...0
   +------------+------+-----+-----+
   |      0     | code |  0  | imm |
   +------------+------+-----+-----+

   To extend the address space, historically unused bits have now been
   allocated to form a larger immediate.

    31........16 15..13 12........0
   +------------+------+-----------+
   | imm[28:13] | code | imm[12:0] |
   +------------+------+-----------+
 */
#define SSEM_INSN_SIZE  (sizeof (uint32_t))
#define SSEM_CODE_BITS  3
#define SSEM_CODE_SHIFT 13
#define SSEM_CODE_NUM   (1U << SSEM_CODE_BITS)
#define SSEM_IMM_BITS   (32 - SSEM_CODE_BITS)

#define MASK(x,n)       ((uint32_t)(x) & ((1U << (n)) - 1))
#define ENCODE(x,m,n,o) (MASK(((uint32_t)(x) >> (n)), ((m) - (n))) << ((n) + (o)))
#define DECODE(x,m,n,o) (MASK(((uint32_t)(x) >> ((n) + (o))), ((m) - (n))) << (n))

#define SSEM_CODE_ENCODE(x) ENCODE(x, SSEM_CODE_BITS, 0, SSEM_CODE_SHIFT)
#define SSEM_CODE_DECODE(x) DECODE(x, SSEM_CODE_BITS, 0, SSEM_CODE_SHIFT)

#if (SSEM_IMM_BITS) > (SSEM_CODE_SHIFT)
/* Split immediate field across opcode.  */

#define SSEM_IMM_ENCODE(x) \
  (MASK(x, SSEM_CODE_SHIFT) | ENCODE(x, SSEM_IMM_BITS, SSEM_CODE_SHIFT, SSEM_CODE_BITS))

#define SSEM_IMM_DECODE(x) \
  (MASK(x, SSEM_CODE_SHIFT) | DECODE(x, SSEM_IMM_BITS, SSEM_CODE_SHIFT, SSEM_CODE_BITS))

#else
#define SSEM_IMM_ENCODE(x) MASK(x, SSEM_IMM_BITS)
#define SSEM_IMM_DECODE(x) MASK(x, SSEM_IMM_BITS)
#endif

struct ssem_opcode
{
  const char *name;
  bfd_boolean imm;
};

extern const struct ssem_opcode ssem_opcodes[SSEM_CODE_NUM];
