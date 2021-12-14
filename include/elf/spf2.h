/* CEVA SPF2 ELF support for BFD.

   Copyright (C) 2015-2021 Free Software Foundation, Inc.

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
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifndef _ELF_SPF2_H
#define _ELF_SPF2_H

#include "elf/reloc-macros.h"

START_RELOC_NUMBERS (elf_spf2_reloc_type)
  RELOC_NUMBER (R_SPF2_NONE, 0)
END_RELOC_NUMBERS(R_SPF2_max)

extern const bfd_vma kUNKNOWN_RAWMEMSPACE;
extern const bfd_vma SPF2_RAWMEMSPACE_INT_PRAM;
extern const bfd_vma SPF2_RAWMEMSPACE_INT_DRAM;
extern const bfd_vma SPF2_RAWMEMSPACE_EXT_RAM;
extern const bfd_vma SPF2_RAWMEMSPACE_CPM;
extern const bfd_vma SPF2_RAWMEMSPACE_NON_CPM;
extern const bfd_vma SPF2_RAWMEMSPACE_EXT_PRAM_GDB;
#define SPF2_VMA_PUT_PREFIX(x, m) x |= m << 32

extern bfd_vma spf2_elf_put_prefix_of_address(bfd_vma arch_type, bfd_vma addr, bfd_vma memspace_id);
extern bfd_vma spf2_make_ext_ram_addr(bfd_vma addr);
extern bfd_vma spf2_make_int_dram_addr(bfd_vma addr);
extern bfd_vma spf2_make_cpm_addr(bfd_vma addr);
extern int spf2_int_dram(bfd_vma addr);

#define ELF_EF_SPF2_CORE(f)       ( (f) & 0xffUL )


#endif /* _ELF_SPF2_H */
