
#ifndef _MAIN_H_
#define _MAIN_H_

/* Responsible for initializing global structures
 * representing the chess rules and pieces involved in the
 * game being played.  Start point of the whole Chess Model
 */
int main_init_model();

/* Responsible for initializing the UI data structures and
 * threads responsible for displaying game progress visually
 * (via OpenGL), and handling user input (maybe)
 */
int main_init_display();

#endif
