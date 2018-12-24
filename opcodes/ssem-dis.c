/* SSEM disassembler.
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
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include "disassemble.h"
#include "opcode/ssem.h"

int
print_insn_ssem (bfd_vma memaddr, struct disassemble_info* info)
{
  bfd_byte buffer[SSEM_INSN_SIZE];
  uint32_t insn;
  const struct ssem_opcode *opcode;
  const char *name;
  int status;

  status = (*info->read_memory_func) (memaddr, buffer, SSEM_INSN_SIZE, info);
  if (status)
    {
      (*info->memory_error_func) (status, memaddr, info);
      return -1;
    }
  insn = bfd_getl32 (&buffer);

  info->bytes_per_chunk = SSEM_INSN_SIZE;

  opcode = &ssem_opcodes[SSEM_CODE_DECODE (insn)];
  name = (opcode->name != NULL) ? opcode->name : "unknown";

  if (opcode->imm)
    {
      (*info->fprintf_func) (info->stream, "%s\t", name);
      (*info->print_address_func) (SSEM_IMM_DECODE (insn), info);
    }
  else
    {
      (*info->fprintf_func) (info->stream, "%s", name);
    }

  return SSEM_INSN_SIZE;
}
