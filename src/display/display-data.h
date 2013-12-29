#ifndef _DISPLAY_DATA_H
#define _DISPLAY_DATA_H

#include "../model/board.h"
#include "../model/chess.h"
#include "cursor.h"

/**
 * The display_data_t struct encapsulates all information
 * needed by the display thread to provide a UI for the user.
 *
 * At a high level this includes two types of info.
 *
 * The first is a handle on the chess model to display.  This is handled
 * via the board_t struct which holds the canonical board that the players
 * see and the computer bases computations on.
 *
 * The second is a handle on the display/window preferences such as the
 * size of the window and the color scheme used.
 */
struct display_data {
  board_t *board_on_screen;       /* (weak) */
  window_params_t *display_prefs; /* (strong) */
  cursor_t *cursor;               /* (strong) */
  square_t *selected_square;      /* (weak) */
};
typedef struct display_data display_data_t;


/* Create a display_data wrapper struct */
display_data_t*
display_data_init(board_t *board, window_params_t *prefs);

/* Create a display_data wrapper using default window_params */
display_data_t*
display_data_init_default_window_params(board_t *board);

void display_data_destroy(display_data_t *data);

#endif
