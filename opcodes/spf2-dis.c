/* Single instruction disassembler for the CEVA SPF2.

   Copyright (C) 2015-2022 Free Software Foundation, Inc.

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
#include "dis-asm.h"
#include "opcode/spf2.h"
#include "spf2-dis.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

#if defined(_WIN32)
	#include "windows.h"
#else
	#include <dlfcn.h>
#endif

#if defined(_WIN32)
	#define K_CevaxcInlineAssembler "cevaxasmsrv.dll"
	HINSTANCE hDll;
#else
	#define K_CevaxcInlineAssembler "libcevaxasmsrv.so"
	void* hDll;
#endif

static T_LoadDb loadDbPtr = NULL;
static T_DisasmIp disasmIpPtr = NULL;

/* Print the CEVA SPF2 instruction at address addr in debugged memory,
   on info->stream. Return length of the instruction, in bytes.  */
int
print_insn_spf2 (bfd_vma addr, disassemble_info *info)
{
	static int dissapi_loaded = 0;
	bfd_byte instrbytes[32];
	int  status;
	int returnedSize = 0;
	char inst_str[INST_STR_SIZE];
	DecoderInfo decoder_info;
	int  sizeOfPacket = 0;
	unsigned long brr_target = 0;

	decoder_info.labelInfoSize = 0;
	decoder_info.isBkrep = 0;
	decoder_info.numOfBkrepDelaySlots = 0;
	decoder_info.opcodeInfo.numOfCw = 0;
	decoder_info.opcodeInfo.numOfInst = 0;

	status = (*info->read_memory_func) (addr , instrbytes, sizeof(instrbytes), info);
    if (status != 0)
	{
	  (*info->memory_error_func) (status, addr, info);
	  return -1;
	}
    if (!dissapi_loaded)
    {
#if defined(_WIN32)
    	hDll = LoadLibrary(K_CevaxcInlineAssembler);
#else
        hDll = dlopen(K_CevaxcInlineAssembler, RTLD_NOW);
#endif

      if (!hDll){

#if defined(_WIN32)
    	  fprintf(stderr, "\nCould not load disassembly library. Error code: %d", GetLastError());
    	  fprintf(stderr, "\nCheck if path to %s is included in %%PATH%%", K_CevaxcInlineAssembler);
#else
          fprintf(stderr, "%s\n", dlerror());
          fprintf(stderr, "Check if path to %s is included in $LD_LIBRARY_PATH\n", K_CevaxcInlineAssembler);
#endif
    	  return -1;
      }

#if defined(_WIN32)
      disasmIpPtr = (T_DisasmIp *) GetProcAddress (hDll, "disasmIp_objdump");
#else
      disasmIpPtr = (T_DisasmIp)dlsym(hDll, "disasmIp_objdump");
#endif

      if(!disasmIpPtr)
        return -1;

#if defined(_WIN32)
      loadDbPtr = (T_LoadDb) GetProcAddress(hDll, "loadDb");
#else
      loadDbPtr = (T_LoadDb)dlsym(hDll, "loadDb");
#endif
	  if ((loadDbPtr == NULL))
		return -1;

	  loadDbPtr("xm8", 0,NULL);
	  dissapi_loaded = 1;
    }

    status = disasmIpPtr(instrbytes, sizeof(instrbytes), &returnedSize, inst_str, INST_STR_SIZE, &decoder_info,
    		addr, &brr_target, NULL);

    if (status)
    {
      (*info->fprintf_func) (info->stream, "%s", inst_str);
      if (brr_target)
      {
    	  info->insn_type = dis_jsr;
    	  info->target = brr_target;
      }
    }

    else
      (*info->fprintf_func) (info->stream, ".word\t0x%08lx", instrbytes);

    return returnedSize;
}
