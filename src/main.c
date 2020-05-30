#include "terminal.h"
#include "input.h"
#include "output.h"
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

struct EditorConfig global_config;

void reset_terminal(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &global_config.termios_attrs);
}

int main(void)
{
    init_editor(&global_config);
    atexit(reset_terminal);

    while (1) {
        ed_refresh_screen(&global_config);
        ed_process_keypress();
    }

    return EXIT_SUCCESS;
}

