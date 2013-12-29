#ifndef _SQUARE_H
#define _SQUARE_H

#include <stdio.h>

#include "chess.h"
#include "piece.h"

/*
 * A single square on the chess board.  Included information includes the rank
 * and file of the square, any pieces using the square, and any pieces
 * threatening the square
 */

/* Declare piece struct */

struct square {
  /* For mental images, assume white moves up from the bottom of the board */
  int rank;
    /* The row on the chess board: 0 is white's back row */
  int file;
    /* The col on the chess board: 0 is white's left rook */
  piece_t *piece;
    /* The chess piece currently on the square */

  /* Stat arrays: index with enum color {WHITE, BLACK} */
  int num_threats[2];/* Number of pieces on each side threatening a square */
  int turns_held[2];/* Number of turns a side has possesed a piece */
  int num_adjacent_pieces[2];/* Number of pieces each side has adjacent */
};
typedef struct square square_t;

/* Allocates and initializes a blank square_t object.
 * In most cases, some of the values in this function will
 * need overridden, however this still provides a convenient
 * way to ensure all fields are given a value
 */
square_t* square_init_blank();
/* Responsible for creating/init-ing individual squares */
square_t * square_init(int rank, int file);

/* Responsible for cleaning up square resources on deletion
 * ...not sure how necessary this is as squares on a board tend
 * to stick around..but it might be useful when our BFS of move
 * possibilities is passing around many boards
 */
void square_destroy(square_t *squareToDel);

/* Check that two squares are equal in all literal values.  This
 * includes checking the statistics about square usage, as well
 * as comparing any pieces on the squares.
 */
bool square_equal(square_t *square_one, square_t *square_two);

/* Makes a deep copy of the square_t object.  This means that
 * the piece on the square is created deeply as well.
 */
square_t* square_copy_deep(square_t *orig);

/* This function loads a square struct from a file,
 * and returns the collected object as a square_t struct
 */
square_t* square_load_from_file(FILE *handle);

/*  This function saves a square to a file,
 *  and returns an integer status flag.  The function is not
 *  responsible for destroying or freeing the square
 */
int square_save_to_file(FILE *handle,square_t* squareToSave);

/* This function adds a piece to the indicated square.  The
 * current position is updated on the piece struct
 */
int square_add_piece(square_t *location, piece_t *added);
#endif
