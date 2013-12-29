#include <stdlib.h>

#include "../model/chess.h"
#include "window-params.h"

/*
 *   window_params_init_defaults
 * Create a window_params struct based on the game's standard defaults
 *   @return a window_params_t* set up with standard game display parameters
 */
window_params_t* window_params_init_defaults()
{
  window_params_t *disp = malloc(sizeof(window_params_t) );
  if (!disp) return NULL;

  /* Initialize colors */
  disp->fg_color1 = rgb_init_color(WHITE);
  disp->fg_color2 = rgb_init_color(BLACK);
  disp->bg_color = rgb_init_color(ORANGE);

  /* Set default sizes */
  disp->win_height = DEFAULT_WINDOW_HEIGHT;
  disp->win_width = DEFAULT_WINDOW_WIDTH;
  disp->win_posx = DEFAULT_WINDOW_X;
  disp->win_posy = DEFAULT_WINDOW_Y;

  return disp;
}

/*
 *   window_params_init
 * Create a window_params_t based on provided color and size parameters
 *   @param fg_color1 the rgb color to use for the 'white' tiles
 *   @param fg_color2 the rgb color to use for the 'black' tiles
 *   @param bg_color the rgb color to use for the window border
 *   @param height the number of vertical pixels to use
 *   @param width the number of horizontal pixels to use
 *   @param posx the X pos to set the top left corner of the window
 *   @param posy the Y pos to set the top left corner of the window
 *   @return the window_params_t* struct created from the params
 */
window_params_t*
window_params_init(rgb_t *fg_color1, rgb_t *fg_color2,
             rgb_t *bg_color, int height, int width, int posx, int posy)
{
  window_params_t *disp = malloc(sizeof(window_params_t) );
  if (!disp) return NULL;

  /* Initialize colors */
  disp->fg_color1 = fg_color1;
  disp->fg_color2 = fg_color2;
  disp->bg_color = bg_color;

  /* Set Window dimensions */
  disp->win_height = height;
  disp->win_width = width;
  disp->win_posx = posx;
  disp->win_posy = posy;
  return disp;
}

/*
 *   window_params_destroy
 * Clean up resources associated with the window_params struct
 *   @param disp a pointer to the window_params struct to destroy
 */
void window_params_destroy(window_params_t* disp)
{
  /* Free color structs */
  rgb_destroy(disp->fg_color1);
  rgb_destroy(disp->fg_color2);
  rgb_destroy(disp->bg_color);

  /* Free window_params itself */
  free(disp);
}
