#ifndef _CHESS_SCREEN_H
#define _CHESS_SCREEN_H

#include "window-params.h"
#include "display-data.h"

/* Make the display data (cursor, canonical board, etc) available
 * to includers of this header file (mainly key-press.c)
 */
extern display_data_t* disp_data;

/*
 *   chess_screen_main
 *  Thread main for the display window.  This code is the ultimate
 *  entrance point for all code in chess-screen
 */
void* chess_screen_main(void *display_data_as_void);

#endif
