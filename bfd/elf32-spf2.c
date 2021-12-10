/* CEVA SPF2-specific support for 32-bit ELF.

   Copyright (C) 2003-2015 Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street - Fifth Floor,
   Boston, MA 02110-1301, USA.  */

#include "sysdep.h"
#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/spf2.h"

const bfd_vma kUNKNOWN_RAWMEMSPACE              = (bfd_vma)-1;
const bfd_vma SPF2_RAWMEMSPACE_INT_PRAM         = (bfd_vma)0;
const bfd_vma SPF2_RAWMEMSPACE_EXT_RAM          = (bfd_vma)1;
const bfd_vma SPF2_RAWMEMSPACE_INT_DRAM         = (bfd_vma)2;
const bfd_vma SPF2_RAWMEMSPACE_CPM              = (bfd_vma)3;
const bfd_vma SPF2_RAWMEMSPACE_NON_CPM          = (bfd_vma)4;
const bfd_vma SPF2_RAWMEMSPACE_INT_PRAM_GDB     = (bfd_vma)5;

const bfd_vma SPF2_RAWMEMSPACE_INT_PRAM_HIGH_ADDR    = 0x7FFF;
const bfd_vma SPF2_RAWMEMSPACE_INT_DRAM_HIGH_ADDR    = 0x3FFFFF;

static reloc_howto_type spf2_elf_howto_table[] = {
  /* This reloc does nothing.  */
  HOWTO (R_SPF2_NONE,		/* type */
	 0,			/* rightshift */
	 3,			/* size (0 = byte, 1 = short, 2 = long) */
	 0,			/* bitsize */
	 FALSE,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont,	/* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_SPF2_NONE",	/* name */
	 FALSE,			/* partial_inplace */
	 0,			/* src_mask */
	 0,			/* dst_mask */
	 FALSE),		/* pcrel_offset */
};

static reloc_howto_type *
spf2_reloc_type_lookup (bfd *abfd ATTRIBUTE_UNUSED,
			  bfd_reloc_code_real_type code)
{
  /* Note that the spf2_elf_howto_table is indexed by the R_
     constants. Thus, the order that the howto records appear in the
     table *must* match the order of the relocation types defined in
     include/elf/spf2.h.  */
  return &spf2_elf_howto_table[(int) R_SPF2_NONE];
}

static reloc_howto_type *
spf2_reloc_name_lookup (bfd *abfd ATTRIBUTE_UNUSED, const char *r_name)
{
  return NULL;
}

bfd_vma
spf2_convert_ext_pram_to_raw(bfd_vma addr)
{
  return  addr & 0xFFFFFFFFULL;
}

bfd_vma
spf2_convert_int_dram_to_raw(bfd_vma addr)
{
  return  addr & 0xFFFFFFFFULL;
}

bfd_vma
spf2_convert_cpm_to_raw(bfd_vma addr)
{
  return  addr & 0xFFFFFFFFULL;
}

bfd_vma
spf2_make_ext_ram_addr(bfd_vma addr)
{
  SPF2_VMA_PUT_PREFIX(addr, SPF2_RAWMEMSPACE_EXT_RAM);
  return  addr;
}

bfd_vma
spf2_make_int_dram_addr(bfd_vma addr)
{
  SPF2_VMA_PUT_PREFIX(addr, SPF2_RAWMEMSPACE_INT_DRAM);
  return  addr;
}

bfd_vma
spf2_make_cpm_addr(bfd_vma addr)
{
  SPF2_VMA_PUT_PREFIX(addr, SPF2_RAWMEMSPACE_CPM);
  return  addr;
}

int
spf2_int_dram(bfd_vma addr)
{
  bfd_vma addr2 = addr & 0xFFFFFFFFULL;
  //ORG to be tested against mem space boundaries
  if ((addr2 >= 0x0)
       && (addr2 <= 0x3FFFFFULL))
  /*if ((addr2 >= rawMemSpaceInfo->lowAddressOfRawDataMemSpace)
     && (addr2 <= rawMemSpaceInfo->highAddressOfRawDataMemSpace))
     */
    return 1;
  else
    return 0;
}

bfd_vma
spf2_elf_put_prefix_of_address(bfd_vma spf2_arch_type, bfd_vma addr, bfd_vma memspace_id)
{
  switch (memspace_id)
  {
    case 0:
      //SPF2_RAWMEMSPACE_INT_PRAM
      memspace_id = SPF2_RAWMEMSPACE_INT_PRAM_GDB;
      break;
    case 1:
      //SPF2_RAWMEMSPACE_INT_DRAM
      break;
    case 2:
      //SPF2_RAWMEMSPACE_EXT_RAM
      break;
    case 3:
      //SPF2_RAWMEMSPACE_CPM
      break;
    case 4:
      //SPF2_RAWMEMSPACE_NON_CPM
      break;
    default:
      BFD_ASSERT(FALSE);
      break;
  }
  SPF2_VMA_PUT_PREFIX(addr, memspace_id);
  return addr;
}

bfd_vma
spf2_elf_convert_word_address(bfd_vma spf2_arch_type, bfd_vma addr, bfd_vma memspace_id)
{
  addr &= 0xFFFFFFFFULL;
  switch (memspace_id)
  {
    case 0:
      //SPF2_RAWMEMSPACE_EXT_PRAM
      memspace_id = SPF2_RAWMEMSPACE_INT_PRAM_GDB;
      break;
    case 1:
      //SPF2_RAWMEMSPACE_INT_DRAM
      break;
    case 2:
      //SPF2_RAWMEMSPACE_EXT_RAM
      break;
    case 3:
      //SPF2_RAWMEMSPACE_CPM
      break;
    case 4:
      //SPF2_RAWMEMSPACE_NON_CPM
      break;
    default:
      BFD_ASSERT(FALSE);
      break;
  }

  return addr;
}

static bfd_boolean
spf2_elf_object_p (bfd * abfd)
{
  Elf_Internal_Phdr *i_phdr;
  Elf_Internal_Shdr **i_shdrp;
  unsigned int i;
  bfd_vma memspace_id;

  elf_elfheader (abfd)->e_entry = spf2_elf_put_prefix_of_address(ELF_EF_SPF2_CORE (elf_elfheader (abfd)->e_flags) , elf_elfheader (abfd)->e_entry, SPF2_RAWMEMSPACE_EXT_RAM);
  // update the address of section with memory space as prefix
  i_shdrp = elf_elfsections (abfd);
  for (i = 0; i < elf_elfheader (abfd)->e_shnum; i++)
   {
     if (i_shdrp[i]->sh_flags & SHF_ALLOC)
     {
    	 if (i_shdrp[i]->sh_flags & SHF_EXECINSTR){
    		 if (i_shdrp[i]->sh_addr <  SPF2_RAWMEMSPACE_INT_PRAM_HIGH_ADDR )
    			 memspace_id = SPF2_RAWMEMSPACE_INT_PRAM;
    		 else
    			 memspace_id = SPF2_RAWMEMSPACE_EXT_RAM;
    	 }
    	 else {
    		 if (i_shdrp[i]->sh_addr <  SPF2_RAWMEMSPACE_INT_DRAM_HIGH_ADDR)
				 memspace_id = SPF2_RAWMEMSPACE_INT_DRAM;
			 else
				 memspace_id = SPF2_RAWMEMSPACE_CPM;
    	 }
    	 i_shdrp[i]->sh_addr = spf2_elf_put_prefix_of_address(ELF_EF_SPF2_CORE (elf_elfheader (abfd)->e_flags), i_shdrp[i]->sh_addr, memspace_id);
    	 bfd_set_section_vma(i_shdrp[i]->bfd_section, i_shdrp[i]->sh_addr);
     }
   }
  memspace_id = kUNKNOWN_RAWMEMSPACE;
  // update the address of segment with memory space as prefix
  i_phdr = elf_tdata (abfd)->phdr;
  for (i = 0; i < elf_elfheader (abfd)->e_phnum; i++)
   {
	  if (i_phdr[i].p_type == PT_LOAD){
		  if (i_phdr[i].p_flags & PF_X){
			  if (i_phdr[i].p_paddr <  SPF2_RAWMEMSPACE_INT_PRAM_HIGH_ADDR )
				 memspace_id = SPF2_RAWMEMSPACE_INT_PRAM;
			  else
				 memspace_id = SPF2_RAWMEMSPACE_EXT_RAM;
		  }
		  else {
			 if (i_phdr[i].p_paddr <  SPF2_RAWMEMSPACE_INT_DRAM_HIGH_ADDR)
				 memspace_id = SPF2_RAWMEMSPACE_INT_DRAM;
			 else
				 memspace_id = SPF2_RAWMEMSPACE_EXT_RAM;
		 }
	  }
     i_phdr[i].p_vaddr = spf2_elf_put_prefix_of_address(ELF_EF_SPF2_CORE (elf_elfheader (abfd)->e_flags), i_phdr[i].p_vaddr, memspace_id);
     i_phdr[i].p_paddr = spf2_elf_put_prefix_of_address(ELF_EF_SPF2_CORE (elf_elfheader (abfd)->e_flags), i_phdr[i].p_paddr, memspace_id);
   }
  return bfd_default_set_arch_mach (abfd, bfd_arch_spf2, ELF_EF_SPF2_CORE (elf_elfheader (abfd)->e_flags));
}

#define ELF_ARCH		bfd_arch_spf2
#define ELF_MACHINE_CODE	EM_NONE
#define ELF_MAXPAGESIZE		1

#define TARGET_LITTLE_SYM       spf2_elf32_vec
#define TARGET_LITTLE_NAME      "elf32-littlespf2"

#define elf_backend_object_p			    spf2_elf_object_p
#define bfd_elf32_bfd_reloc_type_lookup		spf2_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup		spf2_reloc_name_lookup
#include "elf32-target.h"
