#include "terminal.h"
#include "output.h"
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/ioctl.h>

#define BUF_SIZE 32

void init_editor(struct EditorConfig *config)
{
    if (tcgetattr(STDIN_FILENO, &config->termios_attrs) == -1) {
        die("tcgetattr");
    }

    if (get_window_size(&config->screen_rows, &config->screen_cols) == -1) {
        die("get_window_size");
    }

    enable_raw_mode();
}

void die(const char *msg)
{
    ed_clear_screen();
    perror(msg);
    exit(EXIT_FAILURE);
}

void enable_raw_mode(void)
{
    struct termios raw;
    if (tcgetattr(STDIN_FILENO, &raw) == -1) {
        die("tcgetattr");
    }

    raw.c_oflag &= ~((tcflag_t) OPOST); // unset output flags
    raw.c_iflag &= ~((tcflag_t) (IXON | ICRNL | BRKINT | INPCK | ISTRIP));  //unset input flags
    raw.c_cflag |= ((tcflag_t) CS8); // set char to 8 bit byte
    raw.c_lflag &= ~((tcflag_t) (ECHO | ICANON | ISIG | IEXTEN)); // unset local flags
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        die("tcsetattr");
    }
}

char ed_read_key(void)
{
    int num_read;
    char ch;

    while ((num_read = read(STDIN_FILENO, &ch, 1)) !=1) {
        if (num_read == -1 && errno != EAGAIN) {
            die("read");
        }
    }

    return ch;
}

int _get_window_size_by_cursor(int *rows, int *cols)
{
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
        return -1;
    }

    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) {
        return -1;
    }

    char buf[BUF_SIZE];
    size_t i = 0;

    while (i < BUF_SIZE -1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) {
            break;
        }

        if (buf[i] == 'R') {
            break;
        }

        ++i;
    } 

    buf[i] = '\0';

    if (buf[0] != '\x1b' || buf[1] != '[') {
        return -1;
    }

    if (sscanf(&buf[2], " %d;%d",  rows, cols) != 2) {
        return -1;
    }

    return 0;
}

int _get_window_size_by_ioctl(int *rows, int *cols)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    }

    *rows = ws.ws_row;
    *cols = ws.ws_col;

    return 0;
}

int get_window_size(int *rows, int *cols)
{
    int ret = _get_window_size_by_ioctl(rows, cols);

    if (ret != 0) {
        ret = _get_window_size_by_cursor(rows, cols);
    }

    return ret;
}

