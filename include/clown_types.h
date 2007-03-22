#ifndef CLOWN_TYPES_H
#define CLOWN_TYPES_H 1

typedef signed char cycle_t;
#define EFAIL ((cycle_t) - 1)

#include "exceptions.h"

typedef unsigned long       Uword;
typedef   signed long       Dword;
typedef         Uword       Selector;
typedef         Uword       SegFlags;
typedef   signed long long DDword;
typedef unsigned char      Bit;

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
