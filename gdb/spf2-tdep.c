/* Target-dependent code for the SPF2.

   Copyright (C) 2015-2016 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */


#include "defs.h"
#include "arch-utils.h"
#include "dis-asm.h"
#include "floatformat.h"
#include "frame.h"
#include "frame-base.h"
#include "frame-unwind.h"
#include "gdbcore.h"
#include "gdbtypes.h"
#include "osabi.h"
#include "regcache.h"
#include "regset.h"
#include "trad-frame.h"
#include "dwarf2-frame.h"
#include "value.h"
#include "spf2-tdep.h"
#include "symtab.h"
#include "features/spf2.c"
#include "target.h"
#include "objfiles.h"

extern "C" {
#include "elf/spf2.h"
}

/* Return the name of register REGNUM.  */
static char *spf2_register_names[] =
{
	"r0", "r1", "r2", "r3", "r4",		/* 0 1 2 3 4 */
	"r5", "r6", "r7", "r8", "r9",		/* 5 6 7 8 9 */
	"r10", "r11", "r12", "r13", "r14", /* 10 11 12 13 14 */
	"r15", "r16", "r17", "r18", "r19", /* 15 16 17 18 19 */	
	"r20", "r21", "r22", "r23", "r24", /* 20 21 22 23 24 */
	"r25", "r26", "r27", "r28", "r29", /* 25 26 27 28 29 */
	"r30", "r31", "r32", "r33", "r34", /* 30 31 32 33 34 */
	"r35", "r36", "r37", "r38", "r39", /* 35 36 37 38 39 */	
	"r40", "r41", "r42", "r43", "r44", /* 40 41 42 43 44 */
	"r45", "r46", "r47", "r48", "r49", /* 45 46 47 48 49 */	
	"r50", "r51", "r52", "r53", "r54", /* 50 51 52 53 54 */
	"r55", "r56", "r57", "r58", "r59", /* 55 56 57 58 59 */
	"r60", "r61", "r62", "r63", "ocm_padd1", /* 60 61 62 63 64 */
	"ocm_padd2", "ocm_pcount1", "ocm_pcount2", "ceva_a_pc", "ocm_dadd_low", /* 65 66 67 68 69 */
	"ocm_dadd_high", "ocm_acount", "ocm_dcount", "ocm_dvm", "ocm_control",  /* 70 71 72 73 74 */
	"ocm_sa_bp_en", "mem_config", "mss_config", "ocm_status", "ocm_sa_bp_st", /* 75 76 77 78 79 */
	"m_config", "s_config", "debug_config", "core_version", "core_id",		 /* 80 81 82 83 84 */
	"core_config", "ocore_drd3", "ocore_drd2", "ocore_drd1", "ocore_drd",  /* 85 86 87 88 89 */
	"retreg", "retreg_p1", "fba", "btbdata", "alc",	/* 90 91 92 93 94 */
	"version", "pemr", "pc", "retregb", "retregi",  /* 95 96 97 98 99 */
	"retregn", "csr", "gsr", "ivbr", "ntcsr", 		/* 100 101 102 103 104 */
	"cecsr", "dcsr", "epc", "gcr", "sasr",  		/* 105 106 107 108 109 */
	"prdr", "iopr", "sfpsr", "samr", "iba", 	/* 110 111 112 113 114 */
	"vasr", "sp", "modvfp" 						/* 115 115 117 */
};


static const char *
spf2_register_name (struct gdbarch *gdbarch, int regnum)
{
  if (regnum >= 0 && regnum < ARRAY_SIZE (spf2_register_names))
    return spf2_register_names[regnum];

  return NULL;
}


/* Map a DWARF register REGNUM onto the appropriate GDB register
   number.  */

static int
spf2_dwarf_reg_to_regnum (struct gdbarch *gdbarch, int reg)
{
  /* General purpose, address and sp registers */
	  if ((reg >= 0) && (reg <= 118))
	  {
		  if (reg == 64)
			  return 116;
		  return reg;
	  }
return -1;
}

/* Return the GDB type object for the "standard" data type of data in
   register REGNUM.  */

static struct type *
spf2_register_type (struct gdbarch *gdbarch, int regnum)
{
  return builtin_type (gdbarch)->builtin_int;
}

static const gdb_byte *
spf2_breakpoint_from_pc (struct gdbarch *gdbarch, CORE_ADDR *pc, int *len)
{
  //to-do: return non static value
// static CORE_ADDR instr = 2;
// return (gdb_byte*)&instr;
	return NULL;
}

/* Implement the sw_breakpoint_from_kind gdbarch method.  */

static const gdb_byte *
spf2_sw_breakpoint_from_kind (struct gdbarch *gdbarch, int kind, int *size)
{
	return NULL;
}

static CORE_ADDR
spf2_adjust_dwarf2_data_addr (CORE_ADDR data_addr)
{
  if (data_addr & 0xFFFFFFFF00000000ULL)
    return data_addr;
  else
    if (data_addr < 0x3FFFFFULL)
      return spf2_elf_put_prefix_of_address (bfd_mach_spf2, data_addr, SPF2_RAWMEMSPACE_INT_DRAM);
      else
        return spf2_elf_put_prefix_of_address (bfd_mach_spf2, data_addr, SPF2_RAWMEMSPACE_EXT_RAM);
}

static struct frame_id
spf2_dummy_id (struct gdbarch *gdbarch, struct frame_info *this_frame)
{
  return frame_id_build (get_frame_register_unsigned (this_frame,
			 SPF2_SP_REGNUM), get_frame_pc (this_frame));
}

/* SPF2 cache structure.  */
struct spf2_unwind_cache
{
  /* The frame's base, optionally used by the high-level debug info.  */
  CORE_ADDR base;

  /* The previous frame's inner most stack address.  Used as this
     frame ID's stack_ baddr.  */
  CORE_ADDR cfa;

  /* The address of the first instruction in this function */
  CORE_ADDR pc;

  /* The offset of register saved on stack.  If register is not saved, the
     corresponding element is -1.  */
  CORE_ADDR reg_saved[SPF2_NUM_REGS];
};

static void
spf2_setup_default (struct spf2_unwind_cache *cache)
{
  int i;

  for (i = 0; i < SPF2_NUM_REGS; i++)
    cache->reg_saved[i] = -1;
}


/*The SPF2 hardware used hw callstack with ras and ras_depth reisters instead of regular lr register.
 * Also it has feature thar ras_depth register updated not when the command been executed but when pc points on it.
 * To support regular call stack unwinding mechanizm, we should analize our code and if we stay on jsr resgister or on it delay slots, 
 * remove first ras register from usage and reduce number of available returns*/ 
static int
spf2_skip_frame(struct frame_info *this_frame){
    int ret = false;
    
    CORE_ADDR current_pc,func_pc;
    LONGEST insn;
    LONGEST mask = 0xEFC00000; //jsr opcode mask
    int i = 3;
    struct gdbarch *gdbarch = get_frame_arch (this_frame);
    enum bfd_endian byte_order = gdbarch_byte_order (gdbarch);
    
    
    //move till frame level 0
    while(this_frame != NULL && frame_relative_level(this_frame) != 0){
        this_frame = get_next_frame(this_frame);
    }
    
    func_pc = get_frame_func (this_frame);
    current_pc = get_frame_pc (this_frame)&~1;
    
    while(i>0){
        insn = read_memory_unsigned_integer (current_pc, 8/*64bit*/, byte_order);
        insn = (insn >>32) & mask;
        if (insn == 0x42000000 || insn == 0x42800000 ||
            insn == 0x43000000 || insn == 0x43800000)
        {//jsr instruction opcodes
            ret = 1;
            break;
        }
        
        current_pc -= 2;
        
        if (current_pc < func_pc){
            break;
        }
        i = i - 1;
    }
    
    return ret;
}

/*During stack unwinding we may have situation when we have inline frames in call stack, in sucj case we must skip it from the calculation of the ras_depth register*/
static int
spf2_num_inline_frames(struct frame_info *this_frame){
    int ret = 0;

    struct frame_info* next = get_next_frame(this_frame);
    while(next !=NULL && get_frame_type(next) != SENTINEL_FRAME){
        if (get_frame_type(next) == INLINE_FRAME){
            ret++;
        }
        next = get_next_frame(next);
    }

    return ret;
}


/* Do a full analysis of the prologue at START_PC and update CACHE accordingly.
   Bail out early if CURRENT_PC is reached.  Returns the address of the first
   instruction after the prologue.  */

static CORE_ADDR
spf2_analyze_prologue (struct gdbarch *gdbarch,
		       CORE_ADDR start_pc, CORE_ADDR current_pc,
		       struct spf2_unwind_cache *cache,
		       struct frame_info *this_frame)
{
  // TODO: it is not implemented yet!
  CORE_ADDR pc = start_pc;
  CORE_ADDR return_pc = start_pc;
  int frame_base_offset_to_r8 = 0;

  if (start_pc >= current_pc)
    return_pc = current_pc;

  if (cache)
  {
    cache->base = 0;

    if (this_frame)
    {
	 cache->base = spf2_adjust_dwarf2_data_addr(get_frame_register_unsigned (this_frame, SPF2_R8_REGNUM));
	 cache->cfa = cache->base + frame_base_offset_to_r8;
    }
  }

  return return_pc;
}

/* Implement the "skip_prologue" gdbarch method.  */

static CORE_ADDR
spf2_skip_prologue (struct gdbarch *gdbarch, CORE_ADDR pc)
{
  unsigned long inst;
  CORE_ADDR skip_pc;
  CORE_ADDR func_addr, limit_pc;
  struct symtab_and_line sal;

  /* See if we can determine the end of the prologue via the symbol
     table.  If so, then return either PC, or the PC after the
     prologue, whichever is greater.  */
  if (find_pc_partial_function (pc, NULL, &func_addr, NULL))
    {
      CORE_ADDR post_prologue_pc =
	skip_prologue_using_sal (gdbarch, func_addr);

      if (post_prologue_pc != 0)
	return max (pc, post_prologue_pc);
    }

   return pc;
}


/* Frame base handling.  */

static struct spf2_unwind_cache *
spf2_frame_unwind_cache (struct frame_info *this_frame, void **this_cache)
{
  struct gdbarch *gdbarch = get_frame_arch (this_frame);
  struct spf2_unwind_cache *cache;
  CORE_ADDR current_pc;

  if (*this_cache != NULL)
    return (struct spf2_unwind_cache *) *this_cache;

  cache = FRAME_OBSTACK_ZALLOC (struct spf2_unwind_cache);
  (*this_cache) = cache;

  spf2_setup_default (cache);

  cache->pc = get_frame_func (this_frame);
  current_pc = get_frame_pc (this_frame);

  /* Prologue analysis does the rest...  */
  if ((cache->pc & 0xFFFFFFFFF) != 0)
    spf2_analyze_prologue (gdbarch, cache->pc, current_pc, cache, this_frame);

  return cache;
}

/* Implement the "stop_reason" frame_unwind method.  */

static enum unwind_stop_reason
spf2_frame_unwind_stop_reason (struct frame_info *this_frame,
					   void **this_cache)
{
  struct spf2_unwind_cache *cache
    = spf2_frame_unwind_cache (this_frame, this_cache);

  /* We've hit a wall, stop.  */
  if (cache->base == 0)
    return UNWIND_OUTERMOST;

  return UNWIND_NO_REASON;
}

static void
spf2_frame_this_id (struct frame_info *this_frame,
			  void **this_cache, struct frame_id *this_id)
{
  struct spf2_unwind_cache *cache =
     spf2_frame_unwind_cache (this_frame, this_cache);

  /* This marks the outermost frame.  */
  if (cache->base == 0)
    return;

  (*this_id) = frame_id_build (cache->cfa, cache->pc);
}

/* Implement the previous PC register. */
static struct value *
spf2_prev_pc_register(struct frame_info *this_frame, void **this_cache)
{
	int frame_level;
	CORE_ADDR retreg_val;
	CORE_ADDR prev_pc;
	struct gdbarch *gdbarch = get_current_arch ();
	struct type *data_ptr_type = builtin_type (gdbarch)->builtin_data_ptr;
	frame_level = frame_relative_level(this_frame);

	if (frame_level){
		struct frame_info *next_frame = get_next_frame(this_frame);
		prev_pc = get_frame_base_address(next_frame)-4;
		return frame_unwind_got_memory (this_frame, SPF2_PC_REGNUM, spf2_adjust_dwarf2_data_addr(prev_pc));
	}

	retreg_val = frame_unwind_register_unsigned (this_frame, SPF2_retreg_REGNUM);
	return frame_unwind_got_constant (this_frame, SPF2_retreg_REGNUM, retreg_val);

}


/* Implement the "prev_register" frame_unwind method.  */

static struct value *
spf2_frame_prev_register (struct frame_info *this_frame,
			  void **this_cache, int regnum)
{
  struct spf2_unwind_cache *cache =
		spf2_frame_unwind_cache (this_frame, this_cache);

  /* If we are asked to unwind the PC, then we need to return the RETREG or value from stack
     instead. */
  if (regnum == SPF2_PC_REGNUM)
      return spf2_prev_pc_register(this_frame, this_cache);

  if (regnum == SPF2_SP_REGNUM && cache->cfa)
    return frame_unwind_got_constant (this_frame, regnum, cache->cfa);

  /* If we've worked out where a register is stored then load it from
     there.  */
  if (regnum < SPF2_NUM_REGS && cache->reg_saved[regnum] != -1)
    return frame_unwind_got_memory (this_frame, regnum,
				    cache->reg_saved[regnum]);

  return frame_unwind_got_register (this_frame, regnum, regnum);

}

static CORE_ADDR
spf2_frame_base_address (struct frame_info *this_frame, void **this_cache)
{
  struct spf2_unwind_cache *info
	= spf2_frame_unwind_cache (this_frame, this_cache);
  return info->base;
}

/* SPF2 prologue unwinder.  */
static const struct frame_unwind spf2_frame_unwind =
{
  NORMAL_FRAME,
  spf2_frame_unwind_stop_reason,
  spf2_frame_this_id,
  spf2_frame_prev_register,
  NULL,
  default_frame_sniffer
};

static enum unwind_stop_reason
spf2_lastframe_unwind_stop_reason (struct frame_info *this_frame,
					   void **this_cache)
{
  /* We've hit a wall, stop.  */
  return UNWIND_OUTERMOST;
}

static void
spf2_lastframe_this_id (struct frame_info *this_frame,
			  void **this_cache, struct frame_id *this_id)
{
  /* This marks the outermost frame.  */
  return;
}

/* Return the value of the REGNUM register in the previous frame of
   *THIS_FRAME.  */

static struct value *
spf2_dwarf2_prev_register (struct frame_info *this_frame,
			   void **this_cache, int regnum)
{
  switch (regnum)
    {
    case SPF2_PC_REGNUM:
      return spf2_prev_pc_register(this_frame, this_cache);

    default:
      internal_error (__FILE__, __LINE__,
		      _("Unexpected register %d"), regnum);
    }
}

/* Implement the "init_reg" dwarf2_frame_ops method.  */
static void
spf2_dwarf2_frame_init_reg (struct gdbarch *gdbarch, int regnum,
			    struct dwarf2_frame_state_reg *reg,
			    struct frame_info *this_frame)
{

  switch (regnum)
    {
    case SPF2_PC_REGNUM:
      reg->how = DWARF2_FRAME_REG_FN;
      reg->loc.fn = spf2_dwarf2_prev_register;
      break;

    case SPF2_R8_REGNUM:
      reg->how = DWARF2_FRAME_REG_SAVED_EXP;
      break;
    }
}

static const struct frame_base spf2_frame_base =
{
  &spf2_frame_unwind,
  spf2_frame_base_address,
  spf2_frame_base_address,
  spf2_frame_base_address
};

static CORE_ADDR
spf2_read_pc (struct regcache *regcache)
{
  ULONGEST pc_value;

  regcache_cooked_read_unsigned (regcache, SPF2_PC_REGNUM, &pc_value);
  //regcache->cooked_read ( SPF2_PC_REGNUM, &pc_value);
  SPF2_VMA_PUT_PREFIX(pc_value, SPF2_RAWMEMSPACE_EXT_RAM);
  return pc_value;
}

static void
spf2_write_pc (struct regcache *regcache, CORE_ADDR new_pc)
{
  new_pc &= 0xfffffffffULL;

  regcache_cooked_write_unsigned (regcache, SPF2_PC_REGNUM, new_pc);
}

static CORE_ADDR
spf2_unwind_pc (struct gdbarch *gdbarch, struct frame_info *next_frame)
{
  CORE_ADDR pc_value =  frame_unwind_register_unsigned (next_frame, SPF2_PC_REGNUM);
  SPF2_VMA_PUT_PREFIX(pc_value, SPF2_RAWMEMSPACE_EXT_RAM);
  return pc_value;
}

static CORE_ADDR
spf2_adjust_dwarf2_data_uoffset (CORE_ADDR uoffset)
{
  uoffset /= 1;
  return uoffset;
}

static int64_t
spf2_adjust_dwarf2_data_offset (int64_t offset)
{
  offset /= 1;
  return offset;
}



static CORE_ADDR
spf2_adjust_dwarf2_addr (CORE_ADDR pc)
{    
  if (pc & 0xFFFFFFFF00000000ULL)
	{
	  gdb_assert ((pc & 0xFFFFFFFF00000000ULL) == (SPF2_RAWMEMSPACE_EXT_RAM<<32));
	  return pc;
	}
  else
	{
	  return spf2_elf_put_prefix_of_address (bfd_mach_spf2, pc, SPF2_RAWMEMSPACE_EXT_RAM);
	}
  
}

static CORE_ADDR
spf2_adjust_dwarf2_line (CORE_ADDR addr, int rel)
{
    if (rel == 0)
    {
        if (addr & 0xFFFFFFFF00000000ULL)
        {
	        gdb_assert(FALSE);
	        return addr;
        }
        else
	         return spf2_elf_put_prefix_of_address (bfd_mach_spf2, addr, SPF2_RAWMEMSPACE_EXT_RAM);
    }
    else
    {
        CORE_ADDR rel_value = spf2_elf_put_prefix_of_address (bfd_mach_spf2, addr, SPF2_RAWMEMSPACE_EXT_RAM);
        rel_value &= 0xFFFFFFFFFULL;
       return rel_value;
    }
  
}

static int
spf2_dwarf2_frame_adjust_return_address_reg (struct gdbarch *gdbarch,
					     int regnum, int eh_frame_p)
{
  return SPF2_PC_REGNUM;
}

static LONGEST
spf2_dwarf2_frame_adjust_offset (struct gdbarch *gdbarch, LONGEST offset)
{
  LONGEST offset_value = spf2_elf_put_prefix_of_address (bfd_mach_spf2, offset, SPF2_RAWMEMSPACE_INT_DRAM);
  offset_value &= 0xFFFFFFFFFULL;
  return offset_value;
}


static CORE_ADDR
spf2_dwarf2_frame_adjust_line (struct gdbarch *gdbarch, CORE_ADDR addr, int rel)
{
  return spf2_adjust_dwarf2_line(addr, rel);
}


/* Convert from address to pointer and vice-versa.  */

static void
spf2_address_to_pointer (struct gdbarch *gdbarch,
			struct type *type, gdb_byte *buf, CORE_ADDR addr)
{
  enum bfd_endian byte_order = gdbarch_byte_order (gdbarch);

  /* Is it a code address?  */
  if (TYPE_CODE (TYPE_TARGET_TYPE (type)) == TYPE_CODE_FUNC
	   || TYPE_CODE (TYPE_TARGET_TYPE (type)) == TYPE_CODE_METHOD)
    {
      store_unsigned_integer (buf, TYPE_LENGTH (type), byte_order, addr);
    }
  else
    {     
	  store_unsigned_integer (buf, TYPE_LENGTH (type), byte_order, addr);	
    }
}

static CORE_ADDR
spf2_pointer_to_address (struct gdbarch *gdbarch,
			 struct type *type, const gdb_byte *buf)
{
  enum bfd_endian byte_order = gdbarch_byte_order (gdbarch);
  CORE_ADDR addr
    = extract_unsigned_integer (buf, TYPE_LENGTH (type), byte_order);

  addr &= 0xFFFFFFFFULL;

  /* Is it a code address?  */
  if (TYPE_CODE (TYPE_TARGET_TYPE (type)) == TYPE_CODE_FUNC
	|| TYPE_CODE (TYPE_TARGET_TYPE (type)) == TYPE_CODE_METHOD
	|| TYPE_CODE_SPACE (TYPE_TARGET_TYPE (type)))
    {
	  return spf2_make_ext_ram_addr (addr);
    }
  else
    {
	  /* Is it a data address in internal dram?  */
	        if (spf2_int_dram(addr))
	  	{
	  	  return spf2_make_int_dram_addr (addr);
	  	}
	        else
	  	{
	  	   /* It is a data address in ext dram? */
	  	   return spf2_make_ext_ram_addr (addr);
	  	}
    }
}

static CORE_ADDR
spf2_integer_to_address (struct gdbarch *gdbarch,
			 struct type *type, const gdb_byte *buf)
{
  ULONGEST addr = unpack_long (type, buf);
#if 0
  /* Is it a code address?  */
  if (TYPE_CODE (TYPE_TARGET_TYPE (type)) == TYPE_CODE_FUNC
	|| TYPE_CODE (TYPE_TARGET_TYPE (type)) == TYPE_CODE_METHOD
	|| TYPE_CODE_SPACE (TYPE_TARGET_TYPE (type)))
    {
      return addr;
    }
  else
    {
	  return  (addr);	
    }
#endif
  return addr;
}

static CORE_ADDR
spf2_convert_from_func_ptr_addr (struct gdbarch *gdbarch, CORE_ADDR addr,
		   struct target_ops *targ)
{
	return spf2_make_ext_ram_addr (addr);
}

/* Implementation of `address_class_type_flags' gdbarch method.

   This method maps DW_AT_address_class attributes to a
   type_instance_flag_value.  */

static int
spf2_address_class_type_flags (int byte_size, int dwarf2_addr_class)
{
  return 0;
}

/* Implementation of `address_class_type_flags_to_name' gdbarch method.

   Convert a type_instance_flag_value to an address space qualifier.  */

static const char*
spf2_address_class_type_flags_to_name (struct gdbarch *gdbarch, int type_flags)
{
   return NULL;
}

/* Implementation of `address_class_name_to_type_flags' gdbarch method.

   Convert an address space qualifier to a type_instance_flag_value.  */

static int
spf2_address_class_name_to_type_flags (struct gdbarch *gdbarch,
				       const char* name,
				       int *type_flags_ptr)
{
  return 0;
}

/* Default method for gdbarch_addressable_memory_unit_size.  By default, a memory byte has
   a size of 1 octet.  */

static int
spf2_addressable_memory_unit_size (struct gdbarch *gdbarch)
{
  return 1;
}

static int
spf2_adjust_addressable_memory_unit_size (struct gdbarch *gdbarch, CORE_ADDR addr, int memory_unit_size)
{
  if ((addr & (7ULL<<32)) == (1ULL<<32)) // VCPU_DRAM
    return 1;
  else
    if ((addr & (7ULL<<32)) == (2ULL<<32)) // IPPU_PRAM
      return 4;
  else
    if ((addr & (7ULL<<32)) == (3ULL<<32)) // IPPU_DRAM
      return 1;
  else
    if ((addr & (7ULL<<32)) == (4ULL<<32)) // OCRAM_DATA
      return 1;
  else    if ((addr & (7ULL<<32)) == (6ULL<<32)) // VCPU_PRAM
      return 4;

  if (memory_unit_size == 1)
	  return 1;
  else
	  return 2;
}


int
gdb_print_insn_spf2 (bfd_vma addr, disassemble_info *info)
{
    int ret = 0;

    // set decoded instruction to none (and reset also the subroutine call address)
    info->insn_type = dis_noninsn;
    info->target = 0;
    // call decoder
    //ret = print_insn_spf2(addr, info); TO DO: add decoder funtion print_insn_spf2
    // if the insn is jsr, print also the function name
    if (ret > 0)
    {
        // append here the function name for jsr calls
        if ((info->insn_type == dis_jsr) ||
                (info->insn_type == dis_branch))
        {
            CORE_ADDR target_addr = 0x600000000ull | info->target;
            struct bound_minimal_symbol msym = lookup_minimal_symbol_by_pc (target_addr);
            if (msym.minsym != NULL)
             {
                CORE_ADDR start_pc = BMSYMBOL_VALUE_ADDRESS (msym);
                if (((target_addr  - start_pc) & 0xFFFFFFFFFull) > 0ull)
                    (*info->fprintf_func)(info->stream, "\t< %s + 0x%x >",  MSYMBOL_PRINT_NAME (msym.minsym), (target_addr  - start_pc));
                else
                    (*info->fprintf_func)(info->stream, "\t< %s >", MSYMBOL_PRINT_NAME (msym.minsym));
             }
        }
    }
    return ret;

}

enum return_value_convention
spf2_return_value (struct gdbarch *gdbarch, struct value *function, struct type *valtype, struct regcache *regcache, gdb_byte *readbuf, const gdb_byte *writebuf)
{
	// TODO: this should return the value return by the function
	// return this now, so that it doesn't crash at finish
	return RETURN_VALUE_STRUCT_CONVENTION;
}

/* Initialize the current architecture based on INFO.  If possible, re-use an
   architecture from ARCHES, which is a list of architectures already created
   during this debugging session.

   Called e.g. at program startup, when reading a core file, and when reading
   a binary file.  */

static struct gdbarch *
spf2_gdbarch_init (struct gdbarch_info info, struct gdbarch_list *arches)
{
  struct gdbarch *gdbarch;
  struct tdesc_arch_data *tdesc_data = NULL;
  const struct target_desc *tdesc = info.target_desc;
  const struct tdesc_feature *feature;
  int valid_p, i, mach, has_feature;
  /* If there is already a candidate, use it.  */
  arches = gdbarch_list_lookup_by_info (arches, &info);
  if (arches != NULL)
    return arches->gdbarch;

   /* Ensure we always have a target descriptor.  */
  if (!tdesc_has_registers (tdesc))
    tdesc = tdesc_spf2;

   gdb_assert (tdesc);

   // check any target description for validity
  tdesc_data = tdesc_data_alloc ();
  valid_p = 1;
  has_feature = 0;
  if (tdesc_has_registers (tdesc)) 
  {
    feature = tdesc_find_feature (tdesc, "spf2-general-regs");
    if (feature != NULL) 
    {
        has_feature = 1;
        for (i = 0; i < SPF2_ocm_padd1_REGNUM; i++)
            valid_p &= tdesc_numbered_register (feature, tdesc_data, i,
                spf2_register_names[i]);
    }   

    if(!has_feature)
       return NULL;

    feature = tdesc_find_feature (tdesc, "spf2-ocem-regs");
     if (feature != NULL)
     {
         has_feature = 1;
         for (i = SPF2_ocm_padd1_REGNUM; i < SPF2_retreg_REGNUM; i++)
             valid_p &= tdesc_numbered_register (feature, tdesc_data, i,
                 spf2_register_names[i]);
     }

     if(!has_feature)
        return NULL;

     feature = tdesc_find_feature (tdesc, "spf2-system-regs");
         if (feature != NULL)
         {
             has_feature = 1;
             for (i = SPF2_retreg_REGNUM; i < SPF2_modvfp_REGNUM; i++)
                 valid_p &= tdesc_numbered_register (feature, tdesc_data, i,
                     spf2_register_names[i]);
         }

         if(!has_feature)
            return NULL;
  }
  
  if (!valid_p) 
  {
    tdesc_data_cleanup (tdesc_data);
    return NULL;
  }

  gdbarch = gdbarch_alloc (&info, NULL);

  set_gdbarch_adjust_dwarf2_addr (gdbarch, spf2_adjust_dwarf2_addr);
  set_gdbarch_adjust_dwarf2_line (gdbarch, spf2_adjust_dwarf2_line);
 
  set_gdbarch_addr_bit (gdbarch, 40);
  set_gdbarch_ptr_bit (gdbarch, 40);
  set_gdbarch_dwarf2_addr_size (gdbarch, 4);

  /* Register info */
  set_gdbarch_num_regs (gdbarch, SPF2_NUM_REGS);
  set_gdbarch_register_name (gdbarch, spf2_register_name);
  set_tdesc_pseudo_register_name (gdbarch, spf2_register_name);
  set_gdbarch_register_type (gdbarch, spf2_register_type);
  tdesc_use_registers (gdbarch, tdesc, tdesc_data);

  /* Internal <-> external register number maps.  */
  set_gdbarch_dwarf2_reg_to_regnum (gdbarch, spf2_dwarf_reg_to_regnum);
  set_gdbarch_sp_regnum (gdbarch, SPF2_SP_REGNUM);
  set_gdbarch_pc_regnum (gdbarch, SPF2_PC_REGNUM);

  /* Frame and stack info */
  set_gdbarch_skip_prologue (gdbarch, spf2_skip_prologue);

  /* Stack grows upward - stack address is decreasing.  */
  set_gdbarch_inner_than (gdbarch, core_addr_lessthan);

  /* Return value info */
  set_gdbarch_return_value(gdbarch, spf2_return_value);

  /* Call dummy code.  */
  /*This target does not support function calls*/
  set_gdbarch_push_dummy_call (gdbarch, NULL /*spf2_push_dummy_call*/);
  set_gdbarch_dummy_id (gdbarch, spf2_dummy_id);

  /* Breakpoint info */
  set_gdbarch_breakpoint_from_pc (gdbarch, spf2_breakpoint_from_pc);

  set_gdbarch_read_pc (gdbarch, spf2_read_pc);
  set_gdbarch_write_pc (gdbarch, spf2_write_pc);
  set_gdbarch_unwind_pc (gdbarch, spf2_unwind_pc);

  // Disassembly
  set_gdbarch_print_insn (gdbarch, gdb_print_insn_spf2); 
 
  /* Unwinding.  */
  dwarf2_append_unwinders (gdbarch);
  frame_unwind_append_unwinder (gdbarch, &spf2_frame_unwind);
  frame_base_set_default (gdbarch, &spf2_frame_base);
  dwarf2_frame_set_init_reg (gdbarch, spf2_dwarf2_frame_init_reg);
  dwarf2_frame_set_adjust_offset (gdbarch, spf2_dwarf2_frame_adjust_offset);

  /* Address handling.  */
  set_gdbarch_adjust_dwarf2_data_addr (gdbarch, spf2_adjust_dwarf2_data_addr);
  set_gdbarch_address_to_pointer (gdbarch, spf2_address_to_pointer);
  set_gdbarch_pointer_to_address (gdbarch, spf2_pointer_to_address);
  set_gdbarch_integer_to_address (gdbarch, spf2_integer_to_address);
  set_gdbarch_address_class_type_flags (gdbarch, spf2_address_class_type_flags);
  set_gdbarch_address_class_type_flags_to_name
    (gdbarch, spf2_address_class_type_flags_to_name);
  set_gdbarch_address_class_name_to_type_flags
    (gdbarch, spf2_address_class_name_to_type_flags);
  set_gdbarch_convert_from_func_ptr_addr
          (gdbarch, spf2_convert_from_func_ptr_addr);

  set_gdbarch_addressable_memory_unit_size (gdbarch, spf2_addressable_memory_unit_size);  
 
  /* Hook in ABI-specific overrides, if they have been registered.  */
  gdbarch_init_osabi (info, gdbarch);

  set_gdbarch_short_bit (gdbarch, 16);

  return (gdbarch);
}

/* Provide a prototype to silence -Wmissing-prototypes.  */
void _initialize_spf2_tdep (void);


void
_initialize_spf2_tdep (void)
{
  gdbarch_register (bfd_arch_spf2, spf2_gdbarch_init, NULL);
  initialize_tdesc_spf2 ();  
}
