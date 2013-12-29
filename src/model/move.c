#include <stdlib.h>
#include <stdio.h>

#include "move.h"
#include "square.h"

move_t*
move_init(square_t *from_square, square_t *to_square)
{
  move_t *new_move = calloc(1, sizeof(move_t));
  if (! new_move) return NULL;

  new_move->from_square = from_square;
  new_move->to_square = to_square;
  new_move->score = 0;
  return new_move;
}

void move_destroy(move_t *to_destroy)
{
  free(to_destroy);
  return;
}

void
move_set_score(move_t *move, unsigned int score)
{
  move->score = score;
}
