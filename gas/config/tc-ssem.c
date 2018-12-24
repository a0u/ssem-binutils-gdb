/* tc-ssem.c -- SSEM assembler.
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

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING3. If not,
   see <http://www.gnu.org/licenses/>.  */

#include "as.h"
#include "safe-ctype.h"
#include "libiberty.h"
#include "opcode/ssem.h"

/* This array holds the chars that always start a comment.  If the
   pre-processor is disabled, these aren't very useful.  */
const char comment_chars[] = "#;";

/* This array holds the chars that only start a comment at the beginning of
   a line.  If the line seems to have the form '# 123 filename'
   .line and .file directives will appear in the pre-processed output.  */
/* Note that input_file.c hand checks for '#' at the beginning of the
   first line of the input file.  This is because the compiler outputs
   #NO_APP at the beginning of its output.  */
/* Also note that C style comments are always supported.  */
const char line_comment_chars[] = "#";

/* This array holds machine specific line separator characters.  */
const char line_separator_chars[] = ";";

/* Chars that can be used to separate mant from exp in floating point nums.  */
const char EXP_CHARS[] = "eE";

/* Chars that mean this number is a floating point constant.
   As in 0f12.456
   or	 0d1.2345e12  */
const char FLT_CHARS[] = "rRsSfFdDxXpP";

/* Machine-dependent assembler directives.  */
const pseudo_typeS md_pseudo_table[] =
{
  {NULL, NULL, 0}
};

const char *md_shortopts = "";

const char *
md_atof (int type, char *litP, int *sizeP)
{
  return ieee_md_atof (type, litP, sizeP, FALSE);
}

/* Write a value out to the object file, using the appropriate endianness.  */

void
md_number_to_chars (char *buf, valueT val, int n)
{
  number_to_chars_littleendian (buf, val, n);
}

struct option md_longopts[] =
{
  {NULL, no_argument, NULL, 0}
};

size_t md_longopts_size = sizeof (md_longopts);

int
md_parse_option (int c, const char *arg ATTRIBUTE_UNUSED)
{
  return 0;
}

void
md_show_usage (FILE *stream ATTRIBUTE_UNUSED)
{
}

void
md_begin (void)
{
}

static char *
get_expression (char *str, expressionS *exp)
{
  char *save;
  save = input_line_pointer;
  input_line_pointer = str;
  expression (exp);
  str = input_line_pointer;
  input_line_pointer = save;
  return str;
}

void
md_assemble (char *str)
{
  const struct ssem_opcode *opcode;
  char *end;
  char *p;
  char c;
  unsigned int fn;

  end = str;
  while (c = *end, c != '\0' && !ISSPACE (c))
    {
      end++;
    }
  *end = '\0';

  for (fn = 0; fn < ARRAY_SIZE (ssem_opcodes); fn++)
    {
      opcode = &ssem_opcodes[fn];
      if ((opcode->name != NULL) && (strcmp(str, opcode->name) == 0))
        goto insn;
    }
  as_bad (_("unrecognized opcode"));

insn:
  *end = c;
  str = end;
  p = frag_more (SSEM_INSN_SIZE);

  if (opcode->imm)
  {
    expressionS exp;
    while (ISSPACE (*str))
      {
        str++;
      }

    get_expression (str, &exp);
    fix_new_exp (frag_now, p - frag_now->fr_literal, SSEM_INSN_SIZE, &exp, FALSE, BFD_RELOC_32);
  }

  md_number_to_chars (p, SSEM_CODE_ENCODE (fn), SSEM_INSN_SIZE);
  dwarf2_emit_insn (SSEM_INSN_SIZE);
}

symbolS *
md_undefined_symbol (char *name ATTRIBUTE_UNUSED)
{
  return NULL;
}

/* Parse an operand that is machine-specific.  */

void
md_operand (expressionS *expressionP ATTRIBUTE_UNUSED)
{
}

/* Round up a section size to the appropriate boundary.  */

valueT
md_section_align (segT segment ATTRIBUTE_UNUSED, valueT size)
{
  return size;
}

long
md_pcrel_from (fixS *fixP)
{
  return fixP->fx_where + fixP->fx_frag->fr_address;
}

/* Apply a fixup to the object file.  */

void
md_apply_fix (fixS *fixP, valueT *valP, segT seg ATTRIBUTE_UNUSED)
{
  bfd_byte *buf;

  buf = (bfd_byte *) (fixP->fx_where + fixP->fx_frag->fr_literal);
  bfd_putl32 (bfd_getl32 (buf) | SSEM_IMM_ENCODE (*valP), buf);

  if (fixP->fx_addsy == NULL && fixP->fx_pcrel == 0)
    fixP->fx_done = TRUE;
}

/* Generate a machine-dependent relocation.  */

arelent *
tc_gen_reloc (asection *section ATTRIBUTE_UNUSED, fixS *fixP)
{
  arelent *relP;

  relP = XNEW (arelent);
  relP->sym_ptr_ptr = XNEW (asymbol *);
  *relP->sym_ptr_ptr = symbol_get_bfdsym (fixP->fx_addsy);
  relP->address = (fixP->fx_frag->fr_address + fixP->fx_where) >> OCTETS_PER_BYTE_POWER;
  relP->addend = fixP->fx_offset;

  relP->howto = bfd_reloc_type_lookup (stdoutput, fixP->fx_r_type);
  if (relP->howto == NULL)
    {
      as_bad_where (fixP->fx_file, fixP->fx_line,
		    _("cannot represent %s relocation in object file"),
		    bfd_get_reloc_code_name (fixP->fx_r_type));
      return NULL;
    }

  return relP;
}
