#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cursor.h"
#include "../model/chess.h"

/*
 *   cursor_init
 * Create a cursor_t struct.
 *   @return a * to the cursor_t struct
 */
cursor_t* cursor_init(void)
{
  cursor_t *new_cursor = malloc(sizeof(cursor_t));
  if (!new_cursor) return NULL;

  new_cursor->rank = 0;
  new_cursor->file = 0;
  new_cursor->visible = true;
  return new_cursor;
}

/*
 *   cursor_destroy
 * Cleanup the resources associated with a cursor_t struct
 *   @param to_destroy a pointer to the cursor_t to destroy
 */
void cursor_destroy(cursor_t *to_destroy)
{
  free(to_destroy);
}

/*
 *   cursor_move
 * Move the provided cursor in the given direction.  Check that
 * the given move is legal (it doesnt move the cursor out of bounds.
 *   @param cursor the cursor object to operate on
 *   @param move_towards the direction to move in (LEFT, RIGHT, etc)
 *   @return 0 on success, 1 for a move disallowed by board boundaries,
 *     or 2 for other bad arguments
 */
int cursor_move(cursor_t *cursor, direction_t move_towards)
{
  if (!cursor) return 2;

  /* Determine the potential new rank and file */
  int new_rank, new_file;
  new_rank = cursor->rank;
  new_file = cursor->file;
  switch(move_towards)
  {
    case UP:
      new_rank++;
      break;
    case DOWN:
      new_rank--;
      break;
    case RIGHT:
      new_file++;
      break;
    case LEFT:
      new_file--;
      break;
  }

  /* Check that the new rank and file are within board bounds */
  if (new_rank < 0 || new_rank >= BOARD_SIZE) return 1;
  if (new_file < 0 || new_file >= BOARD_SIZE) return 1;

  /* If our bounds were legal, update the actual bounds */
  cursor->rank = new_rank;
  cursor->file = new_file;
  return 0;
}
