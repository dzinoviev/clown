#ifndef CLOWN_TYPES_H
#define CLOWN_TYPES_H 1

typedef signed char cycle_t;
#define EFAIL ((cycle_t) - 1)

#include "exceptions.h"

typedef long unsigned       Uword;
typedef   long signed       Dword;
typedef long long signed    DDword;
typedef         Uword       Selector;
typedef         Uword       SegFlags;
typedef      unsigned       Bit;

struct Clown_IODevice {
    Bit n_registers;
    struct Clown_IOPort *registers;
    void (*execute)(Bit bus_slot);
    Bit  (*init)(void);
    void (*reset)(void);
    Bit  first_port;
    Bit  IRQ;
    Bit  attached;
};

struct Clown_IOPort {
    Dword (*read_hook)(void);
    void (*write_hook)(Dword);
    Bit used;
};

#endif
