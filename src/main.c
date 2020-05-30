#include "terminal.h"
#include "input.h"
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/* reseting the terminal back to original state on exit.
 * should move into terminal.c
 */
struct termios orig_termios;

void disable_raw_mode(void)
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
        die("tcsetattr");
    }
}

int main(void)
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    enable_raw_mode();
    atexit(disable_raw_mode);

    while (1) {
        ed_process_keypress();
    }

    return EXIT_SUCCESS;
}

