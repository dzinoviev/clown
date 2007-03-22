#ifndef HDD_H
#define HDD_H

#include "clown_types.h"
#include "generate.h"

/* The name of the environment variable that points to the path to the 
   disk image file */
#define DISC_HOME      "CLOWN_DISC"
/* The name of the disk image file */
#define DISC_IMAGE     "clown.dsk"
/* The size of a sector */
#define DISC_WORDS_PER_SECTOR CLOWN_FRAME_SIZE
#define DISC_WORDS_PER_GAP    16

struct Clown_Hdd {
    Uword n_tracks;
    Uword n_sectors;
    Uword t2t_seek;
    Uword max_seek;
    Uword rot_speed;
};

Bit hdd_init (char disc_path[], struct Clown_Hdd *params, Uword *track_len);
Bit hdd_read_sector (Dword track, Dword sector, Dword *hidden_buffer);
Bit hdd_write_sector (Dword track, Dword sector, Dword *hidden_buffer);
Bit hdd_close ();
#endif
