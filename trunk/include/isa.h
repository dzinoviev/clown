#ifndef ISA_H
#define ISA_H 1

#include <limits.h>
#include "prototypes.h"

/* 32 bits - 5 bits needed to count */
#define DWORD_BITMASK     ((sizeof (Dword) * CHAR_BIT) - 1)
#define CLOWN_TLB_SIZE 16
#define CLOWN_CACHE_BITS 5

#define DWORD_MAX         LONG_MAX
#define DWORD_MIN         LONG_MIN

#define CLOWN_GPR_BITS    4	/* bits for GPRs */
#define CLOWN_SEGR_BITS   3	/* bits for SPRs */
#define CLOWN_AUX_BITS    8	/* bits for the AUX field */
#define CLOWN_OPCODE_BITS 8
#define CLOWN_SEG_BITS    (sizeof(Selector) * CHAR_BIT)

#define MAX_OFFSET 0x7F /* 7 bits + sign */
#define I_OPC(x)  ((((Uword)x) & 0xFF000000) >> 24)
#define I_OP1(x)  ((((Uword)x) & 0x00F00000) >> 20)
#define I_OP2(x)  ((((Uword)x) & 0x000F0000) >> 16)
/*#define I_DSPL( ((((Dword)x) & 0x0000FF00) >>  8)*/
#define I_DSPL(x) ((((Dword)x) << 16) >> 24)
#define I_SEG(x)  ((((Uword)x) & 0x000FFFF0) >>  4)

#define PUSHPOP (-1)

#define BUILD_INSTRUCTION_A(opc,op1,op2)  ((opc)<<24|(op1)<<20|(op2)<<16)
#define BUILD_INSTRUCTION_B(opc,op1,dspl) \
((opc)<<24|\
 (op1)<<20|\
 (0xFF00 & ((dspl)<<24) >> 16))
#define BUILD_INSTRUCTION_D(opc,op1,op2,dspl) \
((opc)<<24|\
 (op1)<<20|\
 (op2)<<16|\
 (0xFF00 & ((dspl)<<24) >> 16))
#define BUILD_INSTRUCTION_C(opc,op1,seg) ((opc)<<24|(op1)<<20|((seg)&0xFFFF)<<4)
#define UPDATE_SEGMENT(instr,newseg) ((instr) = ((instr) & 0xFFF0000F) \
				      | (((newseg)&0xFFFF)<<4))
#define EXTENSION_BIT (1 << (CLOWN_OPCODE_BITS - 1))
#define NEEDS_EXTENSION(opc) ((opc & EXTENSION_BIT) != 0)

typedef enum {
    ROR  = 0x20, 
    ROL  = 0x21, 
    SAR  = 0x22, 
    SAL  = 0x23, 
    GETB = 0x24,
    SETB = 0x25,
    CLRB = 0x26,
    xLDI  = 0x27 | EXTENSION_BIT, 
    xLD   = 0x28 | EXTENSION_BIT,
    xLDS  = 0x29 | EXTENSION_BIT, 
    LDX  = 0x2A,
    xST   = 0x2B | EXTENSION_BIT, 
    xSTS  = 0x2C | EXTENSION_BIT,
    STX  = 0x2D, 
    MOV  = 0x2E, 
    xXCHG = 0x2F | EXTENSION_BIT, 
    POP  = 0x30, 
    PUSH = 0x31,
    MOVSI= 0x32, 
    MOVTS= 0x33, 
    MOVFS= 0x34, 
    ADD  = 0x35, 
    SUB  = 0x36, 
    MUL  = 0x37, 
    DIV  = 0x38,
    CMP  = 0x39, 
    NEG  = 0x3A, 
    INC  = 0x3B, 
    DEC  = 0x3C,
    AND  = 0x3D, 
    OR   = 0x3E, 
    XOR  = 0x3F, 
    TST  = 0x40,
    NOT  = 0x41,
    CLI  = 0x42, 
    STI  = 0x43, 
    CLC  = 0x44, 
    STC  = 0x45,
    PUSHF =0x46, 
    POPF = 0x47,
    CHIO = 0x48, 
    xNCALL =0x49 | EXTENSION_BIT, 
    xFCALL =0x4A | EXTENSION_BIT,
    NCALLX=0x4B, 
    NRET = 0x4C, 
    FRET = 0x4D, 
    IFRET = 0x4E,
    TRAP = 0x4F,
    JC   = 0x50, 
    JNC  = 0x51, 
    JO   = 0x52, 
    JNO  = 0x53,
    JZ   = 0x54, 
    JNZ  = 0x55, 
    JS   = 0x56, 
    JNS  = 0X57,
    xNJMP = 0x58 | EXTENSION_BIT, 
    xFJMP = 0x59 | EXTENSION_BIT,
    IN   = 0x5A, 
    OUT  = 0x5B, 
    HLT  = 0x5C, 
    NOP  = 0x5D,
    STOP = 0x5E,
    GETBY= 0x5F,
    SETBY= 0x60,
    xADDI= 0x61 | EXTENSION_BIT,
    xANDI= 0x62 | EXTENSION_BIT,
    CLRBI= 0x62,
    xCMPI= 0x63 | EXTENSION_BIT,
    xDIVI= 0x64 | EXTENSION_BIT,
    xDIVJ= 0x65 | EXTENSION_BIT,
    GETBI= 0x66,
    xMULI= 0x67 | EXTENSION_BIT,
    xORI = 0x68 | EXTENSION_BIT,
    xOUTI= 0x69 | EXTENSION_BIT,
    xPUSHI= 0x6A | EXTENSION_BIT,
    ROLI = 0x6B,
    RORI = 0x6C,
    SARI = 0x6D,
    SALI = 0x6E,
    SETBI= 0x6F,
    xSUBI= 0x70 | EXTENSION_BIT,
    xTSTI= 0x71 | EXTENSION_BIT,
    xXORI= 0x72 | EXTENSION_BIT,
    PEEK = 0x73,
    POKE = 0x74,
    INRET = 0x75,
    REM = 0x76,
    xREMI = 0x77 | EXTENSION_BIT,
    xREMJ = 0x78 | EXTENSION_BIT,
} Clown_Opcode;

#endif /* ISA_H */
