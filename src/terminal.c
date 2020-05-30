#include "terminal.h"
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

void die(const char *msg)
{
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

char ed_ready_key(void)
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

