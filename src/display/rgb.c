#include <stdlib.h>

#include "rgb.h"
#include "../model/chess.h"

/*
 *   rgb_init_color
 * Create a rgb struct based on a predefined color enum
 *   @param col the color (from an enum) to create
 *   @return a rgb_t struct created from the picked enum val
 */
rgb_t* rgb_init_color(color_t color)
{
  rgb_t* rgb = malloc(sizeof(rgb_t) );
  if(!rgb) return NULL;

  /* Set the color differently based on the option chosen */
  switch(color)
  {
    case WHITE:
      rgb->red_color = 1.0f;
      rgb->green_color = 1.0f;
      rgb->blue_color = 1.0f;
      break;
    case BLACK:
      rgb->red_color = 0.0f;
      rgb->green_color = 0.0f;
      rgb->blue_color = 0.0f;
      break;
    case RED:
      rgb->red_color = 1.0f;
      rgb->green_color = 0.0f;
      rgb->blue_color = 0.0f;
      break;
    case GREEN:
      rgb->red_color = 0.0f;
      rgb->green_color = 1.0f;
      rgb->blue_color = 0.0f;
      break;
    case BLUE:
      rgb->red_color = 0.0f;
      rgb->green_color = 0.0f;
      rgb->blue_color = 1.0f;
      break;
    case ORANGE:
      rgb->red_color = 0.98823f;
      rgb->green_color = 0.88235f;
      rgb->blue_color = 0.07058f;
      break;
    case PURPLE:
      rgb->red_color = 0.56471f;
      rgb->green_color = 0.10588f;
      rgb->blue_color = 0.58039f;
      break;
    case YELLOW:
      rgb->red_color = 0.9647f;
      rgb->green_color = 1.0f;
      rgb->blue_color = 0.0f;
      break;
    case GREY:
      rgb->red_color = 0.74509f;
      rgb->green_color = 0.74901f;
      rgb->blue_color = 0.63137f;
      break;
    default:
      rgb->red_color = 1.0f;
      rgb->green_color = 1.0f;
      rgb->blue_color = 1.0f;
      break;
  }
  return rgb;
}

/*
 *   rgb_init
 * Create a rgb struct given the r, g, and b values
 *   @param red the red color value (b/w 0.0 and 1.0)
 *   @param green the green color value (b/w 0.0 and 1.0)
 *   @param blue the blue color value (b/w 0.0 and 1.0)
 *   @return a rgb_t struct created from the provided floats
 */
rgb_t* rgb_init(float red, float green, float blue)
{
  rgb_t *rgb = malloc(sizeof(rgb_t) );
  if(!rgb) return NULL;

  if(red<0.0f || red>1.0f) return NULL;
  if(green<0.0f || green>1.0f) return NULL;
  if(blue<0.0f || blue>1.0f) return NULL;

  rgb->red_color=red;
  rgb->green_color=green;
  rgb->blue_color=blue;

  return rgb;
}

/*
 *   rgb_destroy
 * Cleanup a rgb_t struct.
 *   @param rgb the rgb_t struct to free
 */
void rgb_destroy(rgb_t* rgb)
{
  free(rgb);
}
