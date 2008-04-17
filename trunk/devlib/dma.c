#include <stdlib.h>
#include <string.h>
#include "ucvm.h"
#include "prototypes.h"
#include "hdd.h"
#include "clown/config.h"

enum {DMA_IDLE, DMA_DSC2MEM, DMA_MEM2DSC, DMA_PREP1, DMA_PREP2};
struct Clown_IODevice dma_device;
static struct UCVM dma_ucvm;

#define TRACK_REG     0
#define SECTOR_REG    1
#define MEM_BASE_REG  2
#define OPERATION_REG 3
#define TRACK     (dma_ucvm.r[TRACK_REG    ])
#define SECTOR    (dma_ucvm.r[SECTOR_REG   ])
#define MEM_BASE  (dma_ucvm.r[MEM_BASE_REG ])
#define OPERATION (dma_ucvm.r[OPERATION_REG])
#define R4            4
#define R5            5

static const short ucvm_program[] = {
    /*       write_to_port (IOBASE_HDD + 0, 3); --  reset disk buffer */
             UCVM_A (UCVM_xOUTI, IOBASE_HDD + 0, 0), 
    /* 01 */         0, 
    /* 02 */         3,
    /*       write_to_port (IOBASE_HDD + 0, 0); -- seek track */
    /* 03 */ UCVM_A (UCVM_xOUTI, IOBASE_HDD + 0, 0), 
    /* 04 */         0, 
    /* 05 */         0,
    /*       write_to_port (IOBASE_HDD + 2, track); -- seek track */
    /* 06 */ UCVM_A (UCVM_OUT,   IOBASE_HDD + 2, TRACK_REG),
    /*       while (!read_from_port (IOBASE_HDD + 1)); -- wait */
    /* 07 */ UCVM_A (UCVM_IN,    IOBASE_HDD + 1, R4),
    /* 08 */ UCVM_A (UCVM_xCMPI,              0, R4), 
    /* 09 */         0, 
    /* 10 */         0,
    /* 11 */ UCVM_B (UCVM_JEQ,               7),
    /*       if (operation != DMA_DSC2MEM) {*/
    /* 12 */ UCVM_A (UCVM_xCMPI,              0, OPERATION_REG),
    /* 13 */         0,
    /* 14 */         DMA_DSC2MEM,
    /* 15 */ UCVM_B (UCVM_JEQ,              42),
    /*       for (i = 0; i < DISC_WORDS_PER_SECTOR; i++) {*/
    /* 16 */ UCVM_A (UCVM_xMOVI,              0, R4),
    /* 17 */         0,
    /* 18 */         0,
    /* 19 */ UCVM_A (UCVM_xCMPI,              0, R4), 
    /* 20 */         0, 
    /* 21 */         DISC_WORDS_PER_SECTOR,
    /* 22 */ UCVM_B (UCVM_JEQ,              32),
    /*       Dword w; */
    /*       -- write the next work */
    /*       clown_read_cache (mem_base + i, &w); */
    /* 23 */ UCVM_A (UCVM_LD,      MEM_BASE_REG, R5),
    /* 	     write_to_port (IOBASE_HDD + 2, w); */
    /* 24 */ UCVM_A (UCVM_OUT,   IOBASE_HDD + 2, R5),
    /* 25 */ UCVM_A (UCVM_xADDI,              0, R4),
    /* 26 */         0, 
    /* 27 */         1,
    /* 28 */ UCVM_A (UCVM_xADDI,              0, MEM_BASE_REG),
    /* 29 */         0, 
    /* 30 */         1,
    /* 31 */ UCVM_B (UCVM_JMP,              19),
    /*       write_to_port (IOBASE_HDD + 0, DMA_MEM2DSC); -- write */
    /* 32 */ UCVM_A (UCVM_xOUTI, IOBASE_HDD + 0, 0), 
    /* 33 */         0, 
    /* 34 */         DMA_MEM2DSC,
    /*       write_to_port (IOBASE_HDD + 2, sector); -- where to write? */
    /* 35 */ UCVM_A (UCVM_OUT,   IOBASE_HDD + 2, SECTOR_REG), 
    /*       while (read_from_port (IOBASE_HDD + 1) != 1); -- wait */
    /* 36 */ UCVM_A (UCVM_IN,    IOBASE_HDD + 1, R5),
    /* 37 */ UCVM_A (UCVM_xCMPI,              0, R5), 
    /* 38 */         0, 
    /* 39 */         1,
    /* 40 */ UCVM_B (UCVM_JEQ,               68),
    /* 41 */ UCVM_B (UCVM_JMP,               36),


    /*       } else {*/
    /*       write_to_port (IOBASE_HDD + 0, DMA_DSC2MEM); -- read */
    /* 42 */ UCVM_A (UCVM_xOUTI, IOBASE_HDD + 0, 0), 
    /* 43 */         0, 
    /* 44 */         DMA_DSC2MEM,
    /*       write_to_port (IOBASE_HDD + 2, sector); -- what to read? */
    /* 45 */ UCVM_A (UCVM_OUT,   IOBASE_HDD + 2, SECTOR_REG), 
    /*       for (i = 0; i < DISC_WORDS_PER_SECTOR; i++) {*/
    /* 46 */ UCVM_A (UCVM_xMOVI,              0, R4),
    /* 47 */         0,
    /* 48 */         0,
    /* 49 */ UCVM_A (UCVM_xCMPI,              0, R4), 
    /* 50 */         0, 
    /* 51 */         DISC_WORDS_PER_SECTOR,
    /* 52 */ UCVM_B (UCVM_JEQ,              68),
    /*       while (read_from_port (IOBASE_HDD + 1) != 1); -- wait */
    /* 53 */ UCVM_A (UCVM_IN,    IOBASE_HDD + 1, R5),
    /* 54 */ UCVM_A (UCVM_xCMPI,              0, R5), 
    /* 55 */         0, 
    /* 56 */         1,
    /* 57 */ UCVM_B (UCVM_JEQ,               59),
    /* 58 */ UCVM_B (UCVM_JMP,               53),
    /*       read the next word */
    /*       clown_write_cache (mem_base + i, read_from_port (IOBASE_HDD + 2)); */
    /* 59 */ UCVM_A (UCVM_IN,    IOBASE_HDD + 2, R5),
    /* 60 */ UCVM_A (UCVM_ST,      MEM_BASE_REG, R5),
    /* 61 */ UCVM_A (UCVM_xADDI,              0, R4),
    /* 62 */         0, 
    /* 63 */         1,
    /* 64 */ UCVM_A (UCVM_xADDI,              0, MEM_BASE_REG),
    /* 65 */         0, 
    /* 66 */         1,
    /* 67 */ UCVM_B (UCVM_JMP,              49),
    /*       }} */
    /*       sector++; */
    /* 68 */ UCVM_A (UCVM_xADDI,              0, SECTOR_REG),
    /* 69 */         0,
    /* 70 */         1,
    /*       mem_base += DISC_WORDS_PER_SECTOR; */
    /* 71 */ UCVM_A (UCVM_END,                0, 0),
};

Dword status_dma (void)
{
    return OPERATION == DMA_IDLE;
}

void write_dma (Dword datum)
{
    switch (OPERATION) {
    case DMA_IDLE:
	TRACK = datum;
	OPERATION = DMA_PREP1;
	break;
    case DMA_PREP1:
	SECTOR = datum;
	OPERATION = DMA_PREP2;
	break;
    case DMA_PREP2:
	MEM_BASE = datum;
	OPERATION = DMA_IDLE;
	break;
    default:
	break;
    }
}

void reset_dma (void)
{
    new_ucvm (&dma_ucvm, ucvm_program);

    TRACK = 0;
    SECTOR = 0;
    MEM_BASE = 0;
    OPERATION = DMA_IDLE;
}

void control_dma (Dword datum)
{
    switch (datum) {
    case 0:			/* reset the controller */
	reset_dma ();
	return;
    case 1:			/* start reading */
	OPERATION = DMA_DSC2MEM;
	break;
    case 2:			/* start writing */
	OPERATION = DMA_MEM2DSC;
	break;
    default:			/* ignore */
	return;
    }
}

Dword id_dma ()
{
    Dword d;
    char id[] = " dma";
    id[0] = DEVICE_ID;
    memcpy (&d, id, sizeof (Dword));
    return d;
}

Bit init_dma (void)
{
    reset_dma ();
    return 1;			/* it never fails */
}

void execute_dma (Bit bus_slot)
{
    if (OPERATION != DMA_IDLE) {
	if (!ucvm_dostep (&dma_ucvm, !bus_slot)) {
	    OPERATION = DMA_IDLE;
	    raise_exception (dma_device.IRQ);
	}
    }
}


static struct Clown_IOPort ports[] =  {
{id_dma, control_dma}, 
{status_dma, NULL}, 
{NULL, write_dma}, 
};
struct Clown_IODevice dma_device = {3, ports, 
execute_dma, 
init_dma, 
reset_dma
};
