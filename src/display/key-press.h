#ifndef _KEY_PRESSED_H
#define _KEY_PRESSED_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <SDL/SDL.h>

#include <stdlib.h>
#include <stdbool.h>

/* Process the release of normal (ASCII) keyboard keys */
void
key_press_normal_released(unsigned char key, int x, int y);

/* Process the release of non-ASCII keys.  The only
 * keys supported at this time are arrow keys
 */
void
key_press_special_released(int key, int x, int y);

#endif
