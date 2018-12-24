/* SSEM-specific support for 32-bit ELF.
   Copyright (C) 2018 Free Software Foundation, Inc.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "libiberty.h"
#include "elf-bfd.h"
#include "elf/ssem.h"
#include "opcode/ssem.h"

/* The relocation table used for SHT_RELA sections.  */

static reloc_howto_type ssem_elf_howto_table[] =
{
  /* No relocation.  */
  HOWTO (R_SSEM_NONE,			/* type */
	 0,				/* rightshift */
	 0,				/* size (0 = byte, 1 = short, 2 = long) */
	 0,				/* bitsize */
	 FALSE,				/* pc_relative */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
	 bfd_elf_generic_reloc,		/* special_function */
	 "R_SSEM_NONE",			/* name */
	 FALSE,				/* partial_inplace */
	 0,				/* src_mask */
	 0,				/* dst_mask */
	 FALSE),			/* pcrel_offset */

  HOWTO (R_SSEM_32,			/* type */
	 0,				/* rightshift */
	 0,				/* size (0 = byte, 1 = short, 2 = long) */
	 SSEM_IMM_BITS,			/* bitsize */
	 FALSE,				/* pc_relative */
	 0,				/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
	 bfd_elf_generic_reloc,		/* special_function */
	 "R_SSEM_32",			/* name */
	 FALSE,				/* partial_inplace */
	 0,				/* src_mask */
	 SSEM_IMM_ENCODE(-1),		/* dst_mask */
	 FALSE),			/* pcrel_offset */
};

/* A mapping from BFD reloc types to SSEM ELF reloc types.  */
struct elf_reloc_map
{
  bfd_reloc_code_real_type bfd_reloc_val;
  enum elf_ssem_reloc_type elf_reloc_val;
};

static const struct elf_reloc_map ssem_reloc_map[] =
{
  { BFD_RELOC_NONE, R_SSEM_NONE },
  { BFD_RELOC_32, R_SSEM_32 },
};

static reloc_howto_type *
ssem_reloc_type_lookup (bfd *abfd ATTRIBUTE_UNUSED,
			bfd_reloc_code_real_type code)
{
  unsigned int i;

  for (i = 0; i < ARRAY_SIZE (ssem_reloc_map); i++)
    if (ssem_reloc_map[i].bfd_reloc_val == code)
      return &ssem_elf_howto_table[ssem_reloc_map[i].elf_reloc_val];

  return NULL;
}

static reloc_howto_type *
ssem_reloc_name_lookup (bfd *abfd ATTRIBUTE_UNUSED,
			const char *r_name)
{
  unsigned int i;

  for (i = 0; i < ARRAY_SIZE (ssem_elf_howto_table); i++)
    if (ssem_elf_howto_table[i].name != NULL
	&& strcasecmp (ssem_elf_howto_table[i].name, r_name) == 0)
      return &ssem_elf_howto_table[i];

  return NULL;
}

static bfd_boolean
ssem_info_to_howto_rela (bfd *abfd,
			 arelent *cache_ptr,
			 Elf_Internal_Rela *dst)
{
  unsigned int r_type;

  r_type = ELF32_R_TYPE (dst->r_info);
  if (r_type >= ARRAY_SIZE (ssem_elf_howto_table))
    {
      _bfd_error_handler (_("%pB: unsupported relocation type %#x"),
			  abfd, r_type);
      bfd_set_error (bfd_error_bad_value);
      return FALSE;
    }
  cache_ptr->howto = &ssem_elf_howto_table[r_type];
  return TRUE;
}

#define ELF_ARCH		bfd_arch_ssem
#define ELF_MACHINE_CODE	EM_SSEM
#define ELF_MAXPAGESIZE		0x1 /* FIXME */

#define TARGET_LITTLE_SYM	ssem_elf32_vec
#define TARGET_LITTLE_NAME	"elf32-ssem"

#define elf_info_to_howto_rel			NULL
#define elf_info_to_howto			ssem_info_to_howto_rela

#define bfd_elf32_bfd_reloc_type_lookup		ssem_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup		ssem_reloc_name_lookup

#include "elf32-target.h"
