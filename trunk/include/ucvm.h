#ifndef UCVM_H
#define UCVM_H 1

#include "clown_types.h"

/* uCVM instructions: math and control */
#define UCVM_NOP    0		/* single word */
#define UCVM_JEQ    1
#define UCVM_JMP    2
#define UCVM_END    3

#define UCVM_xMOVI  4		/* double word */
#define UCVM_xADDI  5
#define UCVM_xCMPI  6
/* available: 7 */

/* uCVM instructions: I/O and memory */
#define UCVM_OUT    8		/* single word */
#define UCVM_IN     9
#define UCVM_ST    10
#define UCVM_LD    11

#define UCVM_xOUTI 12		/* double word */
/* available: 13, 14, 15 */

/* 
 * Type A : OPC     (PORT/[MEM])? REG?
 * Type B : OPC     OFF 
 */

#define OPC(x)  (((x) & 0xF000) >> 12)
#define PORT(x) (((x) & 0x0FF0) >>  4)
#define MEM(x)  (((x) & 0x0070) >>  4)
#define REG(x)  (((x) & 0x0007) >>  0)
#define OFF(x)  (((x) & 0x0FFF) >>  0)
/*#define OFF(x)  (((x)     << 4) >>  4)*/

#define UCVM_A(opc,port,reg) (((opc) << 12) | ((port) << 4) | (reg))
#define UCVM_B(opc,off)      (((opc) << 12) | (off)              )

#define UCVM_NREGISTERS 8
struct UCVM {
    Bit equal;			/* equal flag */
    short pc;			/* program counter */
    Dword r[UCVM_NREGISTERS];	/* registers */
    const unsigned short *memory;	/* memory */
};

void new_ucvm (struct UCVM *ucvm, const unsigned short *memory);
int ucvm_dostep (struct UCVM *ucvm, Bit iorestr);

#endif
