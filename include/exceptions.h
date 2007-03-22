#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H 1

#define MAX_FAULT 8
#define MAX_TRAP 16
#define TRAP_BITS 5
enum Clown_Exception {
    /* Faults */
    INVALIDOPC_EX = 0,
     PAGEFAULT_EX = 1,
    DIVIDEZERO_EX = 2,
     STACKOVER_EX = 3,
      SEGFAULT_EX = 4,
    PROTECTION_EX = 5,
      BUSERROR_EX = 6,
         RESERVED = 7,

    /* Traps */
          TRAP_EX = MAX_TRAP - 1,
    /* hardware interrupts live here, too */
	 DUMMY_EX = MAX_TRAP
};

cycle_t raise_exception (enum Clown_Exception ex);
cycle_t handle_exception (void);
#endif /* EXCEPTIONS_H */
