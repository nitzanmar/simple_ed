#include "output.h"
#include "terminal.h"
#include <unistd.h>

void _clear_screen(void)
{
    write(STDOUT_FILENO, "\x1b[2j", 4);
}

void _reset_cursor(void)
{
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void ed_clear_screen(void)
{
    _clear_screen();
    _reset_cursor();
}

void ed_draw_rows(int num_rows)
{
    for (int i = 0; i < num_rows; ++i) {
        write(STDOUT_FILENO, "~", 1);

        if (i < num_rows - 1) {
            write(STDOUT_FILENO, "\r\n", 2);
        }
    }
}

void ed_refresh_screen(struct EditorConfig const * const config)
{
    ed_clear_screen();
    ed_draw_rows(config->screen_rows);
    _reset_cursor();
}

