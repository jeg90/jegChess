#ifndef _MOVE_H
#define _MOVE_H

#include "square.h"
/*
 * The 'move' struct encapsulates information concerning a potential chess move.
 * This struct's uses are varied: move enumeration, move ranking, game 'history'
 * move searching etc.  For this reason the struct contains a variety of
 * optional pieces of information used only under select circumstances.
 */

struct move {
  square_t *from_square;  /* Weak ownership */
  square_t *to_square;    /* Weak ownership */
  unsigned int score;
};
typedef struct move move_t;

/* Create a new move struct.  score is initialized to 0 */
move_t*
move_init(square_t *from_square, square_t *to_square);

/* Clean up resources associated with a move_t struct */
void
move_destroy(move_t *to_destroy);

/* Set the score for a given move */
void
move_set_score(move_t *move, unsigned int score);
#endif
