#include <stdlib.h>
#include <stdio.h>

#include "../model/board.h"
#include "window-params.h"
#include "cursor.h"
#include "display-data.h"

/*
 *   display_data_init
 * Create a display_data_t struct for holding board and preference info
 *   @param board the canonical board to be displayed to the user
 *   @param prefs the window preferences to use on the display window
 *   @return a * to the display_data_t struct
 */
display_data_t*
display_data_init(board_t *board, window_params_t *prefs)
{
  display_data_t *new_display_data = malloc(sizeof(display_data_t) );
  if (! new_display_data) return NULL;

  new_display_data->board_on_screen = board;
  new_display_data->display_prefs = prefs;
  new_display_data->cursor = cursor_init();
  new_display_data->selected_square = NULL;
  return new_display_data;
}

/*
 *   display_data_init_default
 * Create a display_data_t struct using default window parameters
 *   @param board the canonical board to be displayed to the user
 *   @return a * to the display_data_t struct created
 */
display_data_t*
display_data_init_default_window_params(board_t *board)
{
  display_data_t *new_display_data = malloc(sizeof(display_data_t) );
  if (! new_display_data) return NULL;

  /* Init struct members */
  new_display_data->board_on_screen = board;
  new_display_data->display_prefs = window_params_init_defaults();
  new_display_data->cursor = cursor_init();
  new_display_data->selected_square = NULL;

  return new_display_data;
}

/*
 *   display_data_destroy
 * Destroy resources associated with a display_data_t struct
 *   @param data the display_data_t to cleanup
 */
void display_data_destroy(display_data_t *data)
{
  /* Destroy strongly-owned member variables */
  window_params_destroy(data->display_prefs);
  cursor_destroy(data->cursor);

  free(data);
}
