/* THIS FILE IS GENERATED.  -*- buffer-read-only: t -*- vi:set ro:
  Original: vspa2.xml */

#include "defs.h"
#include "osabi.h"
#include "target-descriptions.h"

struct target_desc *tdesc_spf2;
static void
initialize_tdesc_spf2 (void)
{
  struct target_desc *result = allocate_target_description ();
  struct tdesc_feature *feature;

  set_tdesc_architecture (result, bfd_scan_arch ("spf2"));

  feature = tdesc_create_feature (result, "spf2-general-regs");

  tdesc_create_reg (feature, "r0", 0, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r1", 1, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r2", 2, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r3", 3, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r4", 4, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r5", 5, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r6", 6, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r7", 7, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r8", 8, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r9", 9, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r10", 10, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r11", 11, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r12", 12, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r13", 13, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r14", 14, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r15", 15, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r16", 16, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r17", 17, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r18", 18, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r19", 19, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r20", 20, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r21", 21, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r22", 22, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r23", 23, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r24", 24, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r25", 25, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r26", 26, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r27", 27, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r28", 28, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r29", 29, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r30", 30, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r31", 31, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r32", 32, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r33", 33, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r34", 34, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r35", 35, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r36", 36, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r37", 37, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r38", 38, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r39", 39, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r40", 40, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r41", 41, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r42", 42, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r43", 43, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r44", 44, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r45", 45, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r46", 46, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r47", 47, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r48", 48, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r49", 49, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r50", 50, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r51", 51, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r52", 52, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r53", 53, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r54", 54, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r55", 55, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r56", 56, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r57", 57, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r58", 58, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r59", 59, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r60", 60, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r61", 61, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r62", 62, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "r63", 63, 1, NULL, 32, "uint32");

  feature = tdesc_create_feature (result, "spf2-ocem-regs");

  tdesc_create_reg (feature, "ocm_padd1", 64, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocm_padd2", 65, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocm_pcount1", 66, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocm_pcount2", 67, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ceva_a_pc", 68, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocm_dadd_low", 69, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocm_dadd_high", 70, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocm_acount", 71, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocm_dcount", 72, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocm_dvm", 73, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocm_control", 74, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocm_sa_bp_en", 75, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "mem_config", 76, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "mss_config", 77, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocm_status", 78, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocm_sa_bp_st", 79, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "m_config", 80, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "s_config", 81, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "debug_config", 82, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "core_version", 83, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "core_id", 84, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "core_config", 85, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocore_drd3", 86, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocore_drd2", 87, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocore_drd1", 88, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ocore_drd", 89, 1, NULL, 32, "uint32");

  feature = tdesc_create_feature (result, "spf2-system-regs");

  tdesc_create_reg (feature, "retreg", 90, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "retreg_p1", 91, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "fba", 92, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "btbdata", 93, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "alc", 94, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "version", 95, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "pemr", 96, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "pc", 97, 1, NULL, 32, "code_ptr");
  tdesc_create_reg (feature, "retregb", 98, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "retregi", 99, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "retregn", 100, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "csr", 101, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "gsr", 102, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ivbr", 103, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "ntcsr", 104, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "cecsr", 105, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "dcsr", 106, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "epc", 107, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "gcr", 108, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "sasr", 109, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "prdr", 110, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "iopr", 111, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "sfpsr", 112, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "samr", 113, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "iba", 114, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "vasr", 115, 1, NULL, 32, "uint32");
  tdesc_create_reg (feature, "sp", 116, 1, NULL, 32, "data_ptr");
  tdesc_create_reg (feature, "modvfp", 117, 1, NULL, 32, "uint32");

/*
  tdesc_create_reg (feature, "pc", 97, 1, NULL, 32, "code_ptr");
  tdesc_create_reg (feature, "sp", 116, 1, NULL, 32, "data_ptr");
*/

  tdesc_spf2 = result;
}
