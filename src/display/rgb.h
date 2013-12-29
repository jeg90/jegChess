#ifndef _RGB_H
#define _RGB_H

#include "../model/chess.h"
/**
 *
 * The rgb_t struct encapsulates the numbers used for
 * a OpenGL color
 */
struct rgb {
  float red_color;
  float green_color;
  float blue_color;
};
typedef struct rgb rgb_t;

/* Create a color based on color_t enum value */
rgb_t* rgb_init_color(color_t col);

/* Create rgb based on red, green, blue OpenGL floats */
rgb_t* rgb_init(float red, float green, float blue);

/* Cleanup rgb-related resources */
void rgb_destroy(rgb_t* rgb);

#endif
