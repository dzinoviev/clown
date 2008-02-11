#ifndef PROTOTYPES_H
#define PROTOTYPES_H 1

#include "clown_types.h"

cycle_t     clown_decode_execute (Dword i, Dword op3);
cycle_t     clown_write_linear (Dword addr, Dword data);
cycle_t     clown_read_linear (Dword addr, Dword *data);
cycle_t     clown_load_seg (Selector seg, int reg, Dword *new_pc);
void        clown_init (void);
void        clown_reset (void);
int         clown_init_cache (unsigned nbits);
void        clown_clear_cache (void);
void        clown_invalidate_cache (Dword phys_addr);
int         clown_init_TLB (int size);
void        clown_clear_TLB (void);
Bit         lookup_TLB (Dword lin_addr, Dword *phys_addr, Bit update);
void        insert_TLB (Dword lin_addr, Dword *phys_addr, Bit update);
cycle_t       do_pop (Dword *datum, Dword offset);
cycle_t       do_push (Dword datum, Dword offset);
cycle_t       do_call (Selector new_seg, Dword new_pc, Dword save_pc);
int         load_memory (char *fname, Dword offset);
void        load_debug_info (char *fname, Dword offset, Dword size);
int         load_device (struct Clown_IODevice *device, Bit first_port,Bit IRQ);
cycle_t       clown_read_cache (Dword phys_addr, Dword *datum);
cycle_t       clown_write_cache (Dword phys_addr, Dword datum);
void        clown_read_mem (Dword phys_addr, Dword *datum);
void        clown_write_mem (Dword phys_addr, Dword datum);
void        write_to_port (Bit port_no, Dword datum);
Dword       read_from_port (Bit port_no);
void        print_debug_info (Dword address);
const char *exception_code (enum Clown_Exception exc_no);
void do_step (void);
void do_quit (void);
void do_run (int count);
void do_reset (void);
void show_mem (Dword nmb, int format);
void set_mem (Dword nmb, Dword value);
void show_regs (Dword nmb, int format);
void set_regs (Dword nmb, Dword value);
void show_nmb (Dword nmb, int format);
void show_refregs (Dword nmb, int format);
void set_refregs (Dword nmb, Dword value);
void show_flags (int format);
void show_ir (int format);
void show_pc (int format);
void set_pc (Dword value);

int attach_device (struct Clown_IODevice *device, Bit first_port, Bit IRQ);
int detach_device (struct Clown_IODevice *device);

#endif
