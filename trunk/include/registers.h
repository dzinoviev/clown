#ifndef REGISTERS_H
#define REGISTERS_H 1

#include "isa.h"
#include "generate.h"

/* Linear address is a DWORD */
/* [31..30 29..20 19..10 9..0]*/
/* [PAD    DIR    PAGE   OFFS]*/
#define L_OFFSET(x)  ((x) &  CLOWN_FRAME_MASK)
#define L_PAGE(x)   (((x) & (CLOWN_FRAME_MASK <<      CLOWN_FRAME_BITS))\
		     >>      CLOWN_FRAME_BITS)
#define L_DIR(x)    (((x) & (CLOWN_FRAME_MASK << (2 * CLOWN_FRAME_BITS)))\
		     >> (2 * CLOWN_FRAME_BITS))

/* Page descriptor is a DWORD */
/* [31..26 25..6 5 4 3 2  1 0]*/
/* [PAD    FRAME D U W SU V P]*/
#define CLOWN_FRAME_CNT_MASK (~(~(Uword)0 << (2 * CLOWN_FRAME_BITS)))
#define PD_FRAME(x)    (((x) &  (CLOWN_FRAME_CNT_MASK << 6)) >> 6)
#define PD_DIRTY(x)     ((x) &  0x20)
#define PD_DIRTY_SET(x) ((x) |= 0x20)
#define PD_USED(x)      ((x) &  0x10)
#define PD_USED_SET(x)  ((x) |= 0x10)
#define PD_WRITE(x)     ((x) &  0x08)
#define PD_SUPER(x)     ((x) &  0x04)
#define PD_VALID(x)     ((x) &  0x02)
#define PD_PRESENT(x)   ((x) &  0x01)
 
#define CLOWN_NGPR  (1<<CLOWN_GPR_BITS)  /* data (signed) registers */
#define CLOWN_NSEGR (1<<CLOWN_SEGR_BITS) /* special-purpose registers */
#define CLOWN_NSEGS (1<<CLOWN_SEG_BITS)

#define CLOWN_NPORTS (1<<CLOWN_AUX_BITS)

struct Clown_ActivePeriphery {
    struct Clown_IODevice *device;
};

extern struct Clown_IOPort clown_port[CLOWN_NPORTS];
extern Bit    Clown_IRQ[MAX_TRAP];
extern struct Clown_ActivePeriphery clown_periphery[];
extern Bit clown_active_count;

/* Segment selector      */
/* [15..4 3..2 1   0  ]  */
/*  ID    PROT TAB REAL  */
/* Tables: _GDT, _LDT    */

#define SET_NEWID(x,nid) ((x) = ((x)&0x000F) | ((nid) << 4))
#define SEL_ID(x)   (((x)&0xFFF0)>>4)
#define SEL_RPL(x)  (((x)&0x000C)>>2) /* requestor's privilege level */
#define SEL_TABL(x) (((x)&0x0002)>>1)
#define SEL_REAL(x) ((x)&0x0001)
#define MK_SELECTOR(id,rpl,tabl) (((id)<<4)|((rpl)<<2)|((tabl)<<1))

/* [31..24 23..20 19..16 15..0] */
/*  OPC    OP1    OP2    PAD    */

/* [31..24 23..20 19..12 11..0] */
/*  OPC    OP1    DISPLA PAD    */

/* [31..24 23..20 19.. 4  3..0] */
/*  OPC    OP1    SELECT PAD    */

enum {
    CAN_READ  = 0x1, 
    CAN_WRITE = 0x2, 
    CAN_EXEC  = 0x4
};
#define READABLE(x)   (SF_PERM(x) & CAN_READ )
#define WRITABLE(x)   (SF_PERM(x) & CAN_WRITE)
#define EXECUTABLE(x) (SF_PERM(x) & CAN_EXEC )

/* Segment descriptor flags: */
/* P - is it present in memory? */
/* PERM - permissions */	     
/* PRIV - priviledge level */	     

/* [15..8 7 6..4 3..2 1..0] */
/*  PAD   P PERM PRIV PAD   */

#define SF_PRESENT(x) (((x).flags) & 0x0080)
#define SF_SET_PRESENT(x) (((x).flags) |= 0x0080)
#define SF_PERM(x)   ((((x).flags) & 0x0070) >> 4) 
#define SF_SET_PERM(x,perm) (((x).flags) |= (((perm)&0x0007)<<4))
#define SF_DPL(x)    ((((x).flags) & 0x000C) >> 2) /* decsriptor priv. level */

struct Clown_Segment_Descriptor {
    SegFlags flags;		/* what kind of segment is this?  */
    Uword    base;		/* where is the segment? */
    Uword    limit;		/* how big is the segment? */
};

#define LGDT_ENTRY_SIZE (sizeof (struct Clown_Segment_Descriptor) / sizeof (Dword))

struct Clown_Segment_Register  {
    /* The heap of the iceberg */
    Selector selector;
    /* The iceberg: not visible to the programmer */
    struct Clown_Segment_Descriptor descriptor;
};

struct Clown_Registers {
    Dword gpr[CLOWN_NGPR ];	/* general-purpose registers */
    struct Clown_Segment_Register
	 segr[CLOWN_NSEGR];	/* segment registers */
    union Clown_Flags {
	Dword raw;
	struct {
	    Bit traplevel : TRAP_BITS; /* reserved */
	    Bit cpl    : 2;	/* current privilege level */
	    Bit iopl   : 2;	/* IO privilege level */
	    Bit inter  : 1;	/* interrupts enabled */
	    Bit over   : 1;	/* overflow */
	    Bit sign   : 1;	/* negative result */
	    Bit zero   : 1;	/* zero result */
	    Bit carry  : 1;	/* carry bit */
	} bitwise;
    } flags;			/* flags */

    /* PROTECTED */
    Uword ir;			/* current instruction */
    Uword op3;			/* current instruction extension */
    Uword pc;			/* program counter */
    Uword old_pc;		/* old program counter - needed to handle 
				 faults */
};

/* Pointer registers by name */
enum {
    _SP   = CLOWN_NGPR - 3, 
    _PAGE = CLOWN_NGPR - 2, 
    _FAR  = CLOWN_NGPR - 1
};
#define SP   gpr[_SP]	/* stack pointer */
#define PAGE gpr[_PAGE]	/* page directory */
#define FAR  gpr[_FAR]	/* fault address */

/* Segment registers by name */
enum {
    _GDT   = 0,
    _LDT   = 1,
    _ISR   = 2,
    _CODE  = 3,
    _STACK = 4,
    _DATA  = 5,
    _ES    = 6,
    _FS    = 7,
};

#define ISR   segr[_ISR].descriptor	/* interrupt table pointer */
#define GDT   segr[_GDT].descriptor	/* global segment table pointer */
#define LDT   segr[_LDT].descriptor	/* local segment table pointer */

#define CODE  segr[_CODE].descriptor	/* code segment */
#define STACK segr[_STACK].descriptor	/* stack segment */
#define DATA  segr[_DATA].descriptor	/* default data segment */
#define ES    segr[_ES].descriptor	/* data segment */
#define FS    segr[_FS].descriptor	/* data segment */

extern DDword clown_time;	/* the number of CPU cycles */
extern struct Clown_Registers clown;
extern Dword CLOWN_MEMORY[];

#include <setjmp.h>
extern jmp_buf begin_or_abort;
extern Bit fetch;
extern Bit silent;
extern Bit initial_cpl;
extern Uword exception_status;
extern Uword pending_exception;
extern char dev_path[];
extern struct Clown_Segment_Descriptor init_ldt_descr;
extern int firstcode;

#endif /* REGISTERS_H */
