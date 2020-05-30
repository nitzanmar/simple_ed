#include "input.h"
#include "terminal.h"
#include "output.h"
#include <stdlib.h>

#define CTRL_KEY(k) ((k) & 0x1f)

void ed_process_keypress()
{
    char c = ed_read_key();

    switch (c) {
        case CTRL_KEY('q'):
            ed_clear_screen();
            exit(0);
            break;
    }
}

