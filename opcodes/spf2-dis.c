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
#if (defined(__MINGW32__) ||defined(_MSC_VER))
#include <minwindef.h>
#include <windows.h>


static T_LoadDb loadDbPtr = NULL;
static T_DisasmIp disasmIpPtr = NULL;

/* Print the CEVA SPF2 instruction at address addr in debugged memory,
   on info->stream. Return length of the instruction, in bytes.  */
int
print_insn_spf2 (bfd_vma addr, disassemble_info *info)
{

	static int dissapi_loaded = 0;
	static HMODULE dll = NULL;
	bfd_byte instrbytes[20];
	int  status;
	int returnedSize = 0;
	char inst_str[INST_STR_SIZE];

	status = (*info->read_memory_func) (addr , instrbytes, sizeof(instrbytes), info);
    if (status != 0)
	{
	  (*info->memory_error_func) (status, addr, info);
	  return -1;
	}

    if (!dissapi_loaded)
    {
      dll = LoadLibrary("cevaxasmsrv.dll");
      if (!dll)
        return -1;

      disasmIpPtr = (T_DisasmIp *) GetProcAddress (dll, "disasmIp");
        if(!disasmIpPtr)
          return -1;

      loadDbPtr = (T_LoadDb) GetProcAddress(dll, "loadDb");

	  if ((loadDbPtr == NULL))
		return -1;

	  loadDbPtr("xm8", 0,NULL);
	  dissapi_loaded = 1;
    }
    status = disasmIpPtr(instrbytes, 32, &returnedSize, inst_str, INST_STR_SIZE, NULL);

    if (status)
      (*info->fprintf_func) (info->stream, "%s", inst_str);
    else
      (*info->fprintf_func) (info->stream, ".word\t0x%08lx", instrbytes);

    return returnedSize;
}
#else
int
print_insn_spf2 (bfd_vma addr, disassemble_info *info){
    return 0;
}
#endif
