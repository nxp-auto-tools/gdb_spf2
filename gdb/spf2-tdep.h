/* Target-dependent code for theSPF2.

   Copyright (C) 2021 Free Software Foundation, Inc.

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

#ifndef SPF2_TDEP_H
#define SPF2_TDEP_H

extern struct target_desc *tdesc_spf2;

/* Register numbers of various important registers.  */

enum SPF2_Regnum
{

	SPF2_R0_REGNUM = 0,
	SPF2_R1_REGNUM = 1,
	SPF2_R2_REGNUM = 2,
	SPF2_R3_REGNUM = 3,
	SPF2_R4_REGNUM = 4,
	SPF2_R5_REGNUM = 5,
	SPF2_R6_REGNUM = 6,
	SPF2_R7_REGNUM = 7,
	SPF2_R8_REGNUM = 8,
	SPF2_R9_REGNUM = 9,
	SPF2_R10_REGNUM = 10,
	SPF2_R11_REGNUM = 11,
	SPF2_R12_REGNUM = 12,
	SPF2_R13_REGNUM = 13,
	SPF2_R14_REGNUM = 14,
	SPF2_R15_REGNUM = 15,
	SPF2_R16_REGNUM = 16,
	SPF2_R17_REGNUM = 17,
	SPF2_R18_REGNUM = 18,
	SPF2_R19_REGNUM = 29,
	SPF2_R20_REGNUM = 20,
	SPF2_R21_REGNUM = 21,
	SPF2_R22_REGNUM = 22,
	SPF2_R23_REGNUM = 23,
	SPF2_R24_REGNUM = 24,
	SPF2_R25_REGNUM = 25,
	SPF2_R26_REGNUM = 26,
	SPF2_R27_REGNUM = 27,
	SPF2_R28_REGNUM = 28,
	SPF2_R29_REGNUM = 29,
	SPF2_R30_REGNUM = 30,
	SPF2_R31_REGNUM = 31,
	SPF2_R32_REGNUM = 32,
	SPF2_R33_REGNUM = 33,
	SPF2_R34_REGNUM = 34,
	SPF2_R35_REGNUM = 35,
	SPF2_R36_REGNUM = 36,
	SPF2_R37_REGNUM = 37,
	SPF2_R38_REGNUM = 38,
	SPF2_R39_REGNUM = 39,
	SPF2_R40_REGNUM = 40,
	SPF2_R41_REGNUM = 41,
	SPF2_R42_REGNUM = 42,
	SPF2_R43_REGNUM = 43,
	SPF2_R44_REGNUM = 44,
	SPF2_R45_REGNUM = 45,
	SPF2_R46_REGNUM = 46,
	SPF2_R47_REGNUM = 47,
	SPF2_R48_REGNUM = 48,
	SPF2_R49_REGNUM = 49,
	SPF2_R50_REGNUM = 50,
	SPF2_R51_REGNUM = 51,
	SPF2_R52_REGNUM = 52,
	SPF2_R53_REGNUM = 53,
	SPF2_R54_REGNUM = 54,
	SPF2_R55_REGNUM = 55,
	SPF2_R56_REGNUM = 56,
	SPF2_R57_REGNUM = 57,
	SPF2_R58_REGNUM = 58,
	SPF2_R59_REGNUM = 59,
	SPF2_R60_REGNUM = 60,
	SPF2_R61_REGNUM = 61,
	SPF2_R62_REGNUM = 62,
	SPF2_R63_REGNUM = 63,
	SPF2_ocm_padd1_REGNUM = 64,
	SPF2_ocm_padd2_REGNUM = 65,
	SPF2_ocm_pcount1_REGNUM = 66,
	SPF2_ocm_pcount2_REGNUM = 67,
	SPF2_ceva_a_pc_REGNUM = 68,
	SPF2_ocm_dadd_low_REGNUM = 69,
	SPF2_ocm_dadd_high_REGNUM = 70,
	SPF2_ocm_acount_REGNUM = 71,
	SPF2_ocm_dcount_REGNUM = 72,
	SPF2_ocm_dvm_REGNUM = 73,
	SPF2_ocm_control_REGNUM = 74,
	SPF2_ocm_sa_bp_en_REGNUM = 75,
	SPF2_mem_config_REGNUM = 76,
	SPF2_mss_config_REGNUM = 77,
	SPF2_ocm_status_REGNUM = 78,
	SPF2_ocm_sa_bp_st_REGNUM = 79,
	SPF2_m_config_REGNUM = 80,
	SPF2_s_config_REGNUM = 81,
	SPF2_debug_config_REGNUM = 82,
	SPF2_core_version_REGNUM = 83,
	SPF2_core_id_REGNUM = 84,
	SPF2_core_config_REGNUM = 85,
	SPF2_ocore_drd3_REGNUM = 86,
	SPF2_ocore_drd2_REGNUM = 87,
	SPF2_ocore_drd1_REGNUM = 88,
	SPF2_ocore_drd_REGNUM = 89,
	SPF2_retreg_REGNUM = 90,
	SPF2_retreg_p1_REGNUM = 91,
	SPF2_fba_REGNUM = 92,
	SPF2_btbdata_REGNUM = 93,
	SPF2_alc_REGNUM = 94,
	SPF2_version_REGNUM = 95,
	SPF2_pemr_REGNUM = 96,
	SPF2_PC_REGNUM = 97,
	SPF2_retregb_REGNUM = 98,
	SPF2_retregi_REGNUM = 99,
	SPF2_retregn_REGNUM = 100,
	SPF2_csr_REGNUM = 101,
	SPF2_gsr_REGNUM = 102,
	SPF2_ivbr_REGNUM = 103,
	SPF2_ntcsr_REGNUM = 104,
	SPF2_cecsr_REGNUM = 105,
	SPF2_dcsr_REGNUM = 106,
	SPF2_epc_REGNUM = 107,
	SPF2_gcr_REGNUM = 108,
	SPF2_sasr_REGNUM = 109,
	SPF2_prdr_REGNUM = 110,
	SPF2_iopr_REGNUM = 111,
	SPF2_sfpsr_REGNUM = 112,
	SPF2_samr_REGNUM = 113,
	SPF2_iba_REGNUM = 114,
	SPF2_vasr_REGNUM = 115,
	SPF2_SP_REGNUM = 116,
	SPF2_modvfp_REGNUM = 117,
	SPF2_NUM_REGS   = 118

};

#endif /* spf2-tdep.h */
