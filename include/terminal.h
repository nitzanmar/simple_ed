#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>

struct EditorConfig {
    int screen_rows;
    int screen_cols;
    struct termios termios_attrs;
};

void init_editor(struct EditorConfig *config);
void reset_terminal(void);
void die(const char *msg);
void enable_raw_mode(void);
char ed_read_key(void);
int get_window_size(int *rows, int *cols);

#endif
