#ifndef _MOVE_LIST_H
#define _MOVE_LIST_H

#include "move.h"

/*
 * An ordered representation of all possible moves available to a player.
 * For a player, this may be used to visually highlight the options for a
 * selected piece.  For a computer player, this is used to rank and select a
 * move.
 */

struct move_list {
  int num_moves;
  int capacity;
  move_t *moves
};
typedef struct move_list move_list_t;

move_list_t*
move_list_new();

void
move_list_destroy(move_list_t *moves);

void
move_list_add_move(move_t *added);

move_t*
move_list_get_move(int index);

int
move_list_length();

#endif
