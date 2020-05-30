#include "input.h"
#include "terminal.h"
#include <stdlib.h>

#define CTRL_KEY(k) ((k) & 0x1f)

void ed_process_keypress()
{
    char c = ed_ready_key();

    switch (c) {
        case CTRL_KEY('q'):
            exit(0);
            break;
    }
}

