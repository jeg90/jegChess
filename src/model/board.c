#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "chess.h"
#include "square.h"
#include "piece.h"
#include "file-utils.h"

/* Declare static helper functions implemented below */
static int valid_board_init(board_t *boardToCheck);
static int add_opening_pieces(board_t* empty_board);
/*
 *   board_init
 * This function creates a board in the standard
 * starting position for a chess match. It is responsible
 * for initializing and allocating all resources needed on
 * the board.
 *   @return * to initialized board_t struct
 */
board_t* board_init()
{
  /* Allocate memory for new board; return on error */
  board_t *newBoard=malloc(sizeof(board_t) );
  if (!newBoard)
    return NULL;

  /* It is white's turn to move by default */
  newBoard->moves_next = WHITE;

  /* Initialize all squares on the board */
  int rank=0;
  int file=0;
  for (rank=0;rank<BOARD_SIZE;rank++)
    for (file=0;file<BOARD_SIZE;file++)
      newBoard->spaces[rank][file] = square_init(rank,file);


  /* Check that square_init didn't fail; if so return NULL */
  if (!valid_board_init(newBoard) )
  {
    board_destroy(newBoard);
    return NULL;
  }

  return newBoard;
}


/*
 *   board_init_start
 * This function creates a board and places pieces in the
 * default starting location for a chess game (back two rows
 * on each side.
 *   @return * to the board_t struct initialized in the starting position.
 */
board_t* board_init_start()
{
  /* Create and init new board */
  board_t* new_board = board_init();
  if (! new_board) return NULL;

  /* Place starting pieces on board */
  add_opening_pieces(new_board);
  return new_board;
}


/*
 *   board_copy_deep
 * This function creates a deep copy of a board already in
 * existence.  This means that this board obtains its own
 * completely independent set of squares and pieces.  This
 * should only be done on rare occasions when these additional
 * copies are truly needed, as the memory overhead for total
 * duplication may be non-negligible.
 *   @param boardToCopy * to the board_t struct to copy
 *   @return * to the deep-copied board_t
 */
board_t* board_copy_deep(board_t *orig)
{
  if (!orig) return NULL;

  /* Alloc mem for new board obj */
  board_t *deep = malloc(sizeof(board_t) );

  deep->moves_next = orig->moves_next;

  /* Copy each square_t object*/
  int rank=0, file=0;
  for (rank=0;rank<BOARD_SIZE;rank++)
    for (file=0;file<BOARD_SIZE;file++)
      deep->spaces[rank][file] = square_copy_deep(orig->spaces[rank][file]);

  if (!valid_board_init(deep) ) return NULL;

  /* Return success */
  return deep;
}


/*
 *   board_destroy
 * This function is responsible for cleaning up all resources
 * associated with a board.  This responsibility extends to
 * cleaning up and deallocating all squares and pieces on the
 * board.
 *   @param board_to_delete * to the board to destroy
 */
void board_destroy(board_t *board_to_delete)
{
  /* Free each square on the board */
  int rank = 0, file = 0;
  for (rank = 0; rank < BOARD_SIZE; rank++) {
    for ( file = 0; file < BOARD_SIZE; file++) {
      if (board_to_delete->spaces[rank][file]) {
        square_destroy(board_to_delete->spaces[rank][file] );
        board_to_delete->spaces[rank][file] = NULL;
      }
    }
  }
}

/*
 *   board_equal
 * Checks that two board_t structs are equal.  Recursively
 * calls square_equal and piece_equal
 *   @param board_one the first board_t* for the comparison
 *   @param board_two the second board_t* for the comparison
 *   @return true if the boards are equal, false otherwise
 */
bool board_equal(board_t *board_one, board_t *board_two)
{
  if (board_one == NULL || board_two == NULL) return false;

  if (board_one->moves_next != board_two->moves_next) return false;

  /* Check that all squares on the board match each other */
  int rank, file;
  bool squares_match;
  for (rank = 0; rank < BOARD_SIZE; rank++) {
    for (file = 0; file < BOARD_SIZE; file++) {
      squares_match = square_equal(
                        board_one->spaces[rank][file],
                        board_two->spaces[rank][file]
                      );
      if (!squares_match) return false;
    }
  }

  return true;
}

/*
 *   board_load_from_file
 * This function loads a board and all associated resources
 * from a file, and returns the collected object as a board_t
 * struct.  For information about the file format used to store
 * board information, see documentation for board_save_to_file.
 *   @param fileIn The file read the board information from.
 *   @return board_t* with the loaded file, or NULL if error occurred
 */
board_t* board_load_from_file(const char *fileIn)
{
  if (!fileIn) return NULL;

  /* Initialize a board struct */
  board_t *loaded= malloc(sizeof(board_t) );
  if (!loaded) return NULL;

  /* Open file and check for errors */
  FILE *loadFile = fopen(fileIn,"r");
  if (!loadFile) return NULL;

  /* First read in the 'moves_next' */
  fread(&(loaded->moves_next),sizeof(loaded->moves_next),1,loadFile);

  int rank=0, file=0,pieceFlag=0;

  /* Load each square on board (pieces are recursively loaded too) */
  for (rank=0;rank<BOARD_SIZE;rank++)
    for (file=0;file<BOARD_SIZE;file++)
      loaded->spaces[rank][file] = square_load_from_file(loadFile);

  fclose(loadFile);

  /* Check for errors in board init */
  if (!valid_board_init(loaded) )
  {
    board_destroy(loaded);
    return NULL;
  }

  /* Return valid board! */
  return loaded;
}

/*
 *   board_save_to_file
 * Write a chess board_t struct to a file at the given filename.
 *   @param file_out the filename to save the board_t to
 *   @param to_save the board_t* to save
 *   @param overwrite a flag deciding whether we can overwrite existing file
 *   @return -1 for NULL args, -2 if we can't overwrite, 0 on success
 */
int
board_save_to_file(const char *file_out, board_t *to_save, bool overwrite)
{
  if (file_out == NULL || to_save == NULL) return -1;

  /* Check to see if file exists */
  bool exists_already;
  exists_already = file_utils_exists(file_out);

  /* Return if it already exists, but we cant want to overwrite */
  if (exists_already && !overwrite)
    return -2;

  /* Open file for writing..return w/ error on failure */
  FILE *saveHandle=fopen(file_out,"w+");
  if (!saveHandle)
    return -3;

  int write_res;
  /* Write the moves_next enum */
  write_res = fwrite(&(to_save->moves_next), sizeof(color_t), 1, saveHandle);

  /* Iterate through the squares on the board */
  int rank=0, file =0;
  for (rank=0;rank<BOARD_SIZE;rank++)
    for (file=0;file<BOARD_SIZE;file++)
      write_res = square_save_to_file(saveHandle, to_save->spaces[rank][file]);

  /* Close file and return */
  fclose(saveHandle);
  return 0;
}


/*
 *   valid_board_init
 * This helper function checks that all squares on the
 * board have been properly initialized (are not NULL).
 *   @param boardToCheck a ptr to the board to status-check.
 *   @return 0 if board is invalid, 1 otherwise.
 */
static
int valid_board_init(board_t *boardToCheck)
{
  int rank=0, file=0;

  /* If any squares on the board are NULL, return false */
  for (rank=0;rank<BOARD_SIZE;rank++)
    for (file=0;file<BOARD_SIZE;file++)
      if ( !(boardToCheck->spaces[rank][file]) )
        return 0;

  /* ...Otherwise, return true */
  return 1;
}

/*
 *   add_opening_pieces
 * This helper function takes an initially empty board
 * and adds the pieces to it that are needed to start a
 * game in the default opening position.
 *   @param empty the board to add pieces to
 *   @return 0 on success, 1 on piece addition error
 */
static int add_opening_pieces(board_t *empty)
{
  /* Set up WHITE's back row */
  square_add_piece(empty->spaces[0][0], piece_init_alive(WHITE,ROOK,0,0));
  square_add_piece(empty->spaces[0][1], piece_init_alive(WHITE,KNIGHT,0,1));
  square_add_piece(empty->spaces[0][2], piece_init_alive(WHITE,BISHOP,0,2));
  square_add_piece(empty->spaces[0][3], piece_init_alive(WHITE,QUEEN,0,3));
  square_add_piece(empty->spaces[0][4], piece_init_alive(WHITE,KING,0,4));
  square_add_piece(empty->spaces[0][5], piece_init_alive(WHITE,BISHOP,0,5));
  square_add_piece(empty->spaces[0][6], piece_init_alive(WHITE,KNIGHT,0,6));
  square_add_piece(empty->spaces[0][7], piece_init_alive(WHITE,ROOK,0,7));

  /* Set up WHITE's pawn row */
  int file=0;
  for (file=0;file<BOARD_SIZE;file++)
    square_add_piece(empty->spaces[1][file], piece_init_alive(WHITE,PAWN,1,file));

  /* Set up BLACK's back row */
  square_add_piece(empty->spaces[7][0], piece_init_alive(BLACK,ROOK,7,0));
  square_add_piece(empty->spaces[7][1], piece_init_alive(BLACK,KNIGHT,7,1));
  square_add_piece(empty->spaces[7][2], piece_init_alive(BLACK,BISHOP,7,2));
  square_add_piece(empty->spaces[7][3], piece_init_alive(BLACK,QUEEN,7,3));
  square_add_piece(empty->spaces[7][4], piece_init_alive(BLACK,KING,7,4));
  square_add_piece(empty->spaces[7][5], piece_init_alive(BLACK,BISHOP,7,5));
  square_add_piece(empty->spaces[7][6], piece_init_alive(BLACK,KNIGHT,7,6));
  square_add_piece(empty->spaces[7][7], piece_init_alive(BLACK,ROOK,7,7));

  /* Set up BLACK's pawn row */
  for (file=0;file<BOARD_SIZE;file++) {
    square_add_piece(
      empty->spaces[6][file],
      piece_init_alive(BLACK, PAWN, 6, file)
    );
  }

  return 0;
}
