#ifndef _CURSOR_H
#define _CURSOR_H

#include <stdbool.h>
enum direction {UP, DOWN, LEFT, RIGHT};
typedef enum direction direction_t;

/**
 * The cursor_t struct encapsulates the location and state
 * of the on-screen 'cursor' used to select pieces.  This state
 * information is generally simple, but benefits from the encapsulation
 * anyway.
 *
 * Though it makes conceptual sense to make the cursor struct/class
 * responsible for drawing itself (via a cursor_draw method, for example)
 * for the Proof-Of-Concept display it was decided to keep all display
 * information in the chess-screen.c file.  Future revisions of this struct
 * may that functionality though.
 */
struct cursor {
  int rank;
  int file;
  bool visible;
};
typedef struct cursor cursor_t;

#define CURSOR_RED    0.8235f
#define CURSOR_GREEN  0.0f
#define CURSOR_BLUE   0.4f

/* Create a cursor_t struct */
cursor_t* cursor_init(void);


/* Cleanup the resources for a cursor_t struct */
void cursor_destroy(cursor_t *to_destroy);

/* Move a cursor UP, DOWN, RIGHT, or LEFT. */
int cursor_move(cursor_t *cursor, direction_t move_towards);

#endif
