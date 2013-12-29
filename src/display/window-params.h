#ifndef _WINDOW_PARAMS_H
#define _WINDOW_PARAMS_H

#include "../model/chess.h"
#include "rgb.h"

#define DEFAULT_WINDOW_HEIGHT 800
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_X 100
#define DEFAULT_WINDOW_Y 100

/* Each border should take up 15% of the height/width */
#define DEFAULT_BORDER_COEFF .15
#define SQUARE_WIDTH         (1.0-2.0*DEFAULT_BORDER_COEFF)/8.0
#define SQUARE_HEIGHT        SQUARE_WIDTH

/* The cursor should leave 20% border on each side */
#define CURS_COEFF           0.2
#define CURS_WIDTH           (1.0-2*CURS_COEFF)*SQUARE_WIDTH
#define CURS_HEIGHT          (1.0-2*CURS_COEFF)*SQUARE_HEIGHT
#define CURS_WIDTH_OFFSET    CURS_COEFF * SQUARE_WIDTH
#define CURS_HEIGHT_OFFSET   CURS_COEFF * SQUARE_HEIGHT


/**
 * The window_params_t struct is a container for a bunch of options
 * related to the window_params of the chess game.  This includes
 * color settings, audio settings, and more general properties
 * like window size.
 */
struct window_params {
  rgb_t *fg_color1;
  rgb_t *fg_color2;
  rgb_t *bg_color;
  int win_height;
  int win_width;
  int win_posx;
  int win_posy;
};
typedef struct window_params window_params_t;

/* Create window_params_t struct based on default settings */
window_params_t* window_params_init_defaults();

/* Create window_params_t from color and size parameters */
window_params_t*
window_params_init(
  rgb_t *fg_color1, rgb_t *fg_color2, rgb_t *bg_color,
  int height, int width, int posx, int posy
);

/* Cleanup window_params resources */
void window_params_destroy(window_params_t* disp);

#endif
