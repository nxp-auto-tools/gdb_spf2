#ifndef _SPF2_DIS_H_
#define _SPF2_DIS_H_


#if defined(__cplusplus)
extern "C"
{
#endif

#define INST_STR_SIZE 1000

struct disasmLabelInfo
{
	char*         labelName;
	int           labelByte;
	int           labelBit;
	unsigned long labelValue;
	int           isSecName;
	int           isCoff;
	int           isSizeOf;
	int           isIncode;
};

struct OpcodeInfo
{
	unsigned long  instOpcodeArr[8];
	int			   is16BitInst[8];
	int			   numOfInst;
	unsigned long long  cwOpcodeArr[8];
	int		       is16BitCw[8];
	int		       is8BitCw[8];
	int		       is64BitCw[8];
	int			   numOfCw;
	unsigned short header;
	int		       isHeader;

};

typedef struct DecoderInfo_type
{
	int isBkrep;
	int numOfBkrepDelaySlots;
	struct disasmLabelInfo labelInfoArr[8];
	int labelInfoSize;
	int bkrepBlockSize;
	int bkrepNumOfInstInBlock;
	struct OpcodeInfo opcodeInfo;
} DecoderInfo;

typedef int (*T_DisasmIp) (unsigned char*d, int c, int* e, char* asm_inst, int sizeOfStr,
   DecoderInfo* dInfo, unsigned long bundleAddress, unsigned long *targetAddress, char* sectionName);
typedef int(*T_LoadDb) (char* a, int b,char* toolDir);


#if defined(__cplusplus)
};
#endif

#endif  // _SPF2_DIS_H_
