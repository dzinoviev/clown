#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include "registers.h"
#include "exceptions.h"

#define COMBINE(base,offset) (((base)<<CLOWN_FRAME_BITS)+(offset))
enum {DO_NOT_UPDATE = 0, DO_UPDATE = 1};

static cycle_t translate_validate (Dword lin_addr, Dword *phys_addr, 
				 Bit update)
{
    cycle_t cycles = 0;
    Dword page_desc, dir_addr, page_addr, datum_addr;
    
    /* get the page directory */
    dir_addr = COMBINE (PD_FRAME (clown.PAGE), L_DIR (lin_addr));
    if (dir_addr >= CLOWN_MEMORY_SIZE) {
	raise_exception (BUSERROR_EX);
	return EFAIL;
    }
    cycles += clown_read_cache (dir_addr, &page_desc);
    
    /* get the page */
    page_addr = COMBINE (PD_FRAME (page_desc), L_PAGE (lin_addr));
    if (page_addr >= CLOWN_MEMORY_SIZE) {
	raise_exception (BUSERROR_EX);
	return EFAIL;
    }

    if (!PD_PRESENT (page_desc) || !PD_VALID (page_desc)) {
	raise_exception (PAGEFAULT_EX);
	clown.FAR = page_addr;
	return EFAIL;
    }

    PD_USED_SET (page_desc);
    cycles += clown_write_cache (dir_addr, page_desc);
    cycles += clown_read_cache (page_addr, &page_desc);
    
    /* get the value */
    datum_addr = COMBINE (PD_FRAME (page_desc), L_OFFSET (lin_addr));
    if (datum_addr >= CLOWN_MEMORY_SIZE) {
	raise_exception (BUSERROR_EX); 
	return EFAIL;
    }

    /* do we have enought authority to use this page? */
    if (   (clown.flags.bitwise.cpl && !PD_SUPER (page_desc))
	|| (update && !PD_WRITE (page_desc))) {
	bark (CPLVIOL);
	raise_exception (PROTECTION_EX); 
	return EFAIL;
    }

    if (!PD_PRESENT (page_desc) || !PD_VALID (page_desc)) {
	raise_exception (PAGEFAULT_EX); 
	clown.FAR = lin_addr;
	return EFAIL;
    }
    
    PD_USED_SET (page_desc);
    if (update)
	PD_DIRTY_SET (page_desc);
    cycles += clown_write_cache (page_addr, page_desc);
    
    *phys_addr = datum_addr;

    return cycles;
}

static cycle_t translate_validate_general (Dword lin_addr, 
					 Dword *phys_addr,
					 Bit update)
{
    cycle_t cycles = 0;
    if (PD_PRESENT (clown.PAGE)) { /* paging on */
	if (!lookup_TLB (lin_addr, phys_addr, update)) {
	    if ((cycles = translate_validate (lin_addr, phys_addr,
					      update)) == EFAIL)
		return EFAIL;
	    else
		insert_TLB (lin_addr, phys_addr, update);
	}
    } else {			/* paging off */
	if ((*phys_addr = lin_addr) >= CLOWN_MEMORY_SIZE) {
	    raise_exception (BUSERROR_EX); 
	    return EFAIL;
	}
    }

    return cycles;
}

cycle_t clown_read_linear (Dword lin_addr, Dword *datum)
{
    Dword phys_addr;
    cycle_t cycles;
    cycles = translate_validate_general (lin_addr, &phys_addr, DO_NOT_UPDATE);
    if (cycles != EFAIL)
	cycles += clown_read_cache (phys_addr, datum);
    return cycles;
}

cycle_t clown_write_linear (Dword lin_addr, Dword datum)
{
    Dword phys_addr;
    cycle_t cycles;
    cycles = translate_validate_general (lin_addr, &phys_addr, DO_UPDATE);
    if (cycles != EFAIL)
	cycles += clown_write_cache (phys_addr, datum);
    return cycles;
}

