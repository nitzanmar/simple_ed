#ifndef OUTPUT_H
#define OUTPUT_H

#include "terminal.h"

void ed_clear_screen(void);
void ed_draw_rows(int num_rows);
void ed_refresh_screen(struct EditorConfig const * const config);

#endif 
