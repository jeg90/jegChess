#ifndef _BOARD_H
#define _BOARD_H

#include <stdbool.h>
#include "chess.h"
#include "square.h"
/**
  The board_t is a wrapper around a variety of other structs, most notably
  a 2D array of square structs.  More importantly it provides
  a unifying interface for initiating operations on the chess model (as in
  MVC 'model').

  It is important to note that the board_t struct is NOT responsible for
  game info, such as the moves leading up to the current position.
*/

/* Declare board struct */

struct board {
  square_t *spaces[8][8];/* An arr rep-ing each square of the board */
  color_t moves_next;/* The color of the player moving next */

};
typedef struct board board_t;

/* This function initializes a game board by allocating memory for
 * all resources and setting initial values.  It doesn't place
 * pieces on the board by default.
 */
board_t* board_init();

/* This function initializes a game board in the starting position
 * for a game.  This includes putting the pieces in the default
 * starting position.
 */
board_t* board_init_start();

/* This function creates a deep copy of a game board from a provided
 * sample board (passed by pointer).  This allows new boards to be
 * created for positions other than the game start position.
 */
board_t* board_copy_deep(board_t *boardToCopy);

/* This function destroys all resources associated with a board,
 * including the squares and pieces associated the board
 */
void board_destroy(board_t *boardToDel);

/* Checks whether the 'leaf' values of two boart_t structs match.
 * This means that each square and piece have the same stats and
 * state information.
 */
bool board_equal(board_t *board_one, board_t *board_two);

/* This function loads a board and all associated resources
 * from a file, and returns the collected object as a board_t
 * struct.
 */
board_t* board_load_from_file(const char *fileIn);


/* This function saves a board struct to a file, and returns
 * an integer flag denoting success, or failure.  The save
 * function is not responsible for deleting or destroying the
 * board.
 */
int
board_save_to_file(const char *file_out, board_t *to_save, bool overwrite);



#endif
