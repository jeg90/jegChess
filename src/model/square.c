#include <stdlib.h>
#include "chess.h"
#include "square.h"
#include "piece.h"

/*
 *   square_init_blank
 * This function creates a blank square_t struct.  Though
 * most of the values given to the struct by default arent
 * satisfactory for normal use of the struct, this still
 * provides a convenient way to allocate and ensure init
 * of all fields.
 */
square_t* square_init_blank()
{
  /* Allocate struct resources */
  square_t *squarePtr = malloc(sizeof(square_t) );
  if (!squarePtr) return NULL;

  /* Init struct fields */
  squarePtr->rank = -1;
  squarePtr->file = -1;
  squarePtr->piece = NULL;
  squarePtr->num_threats[WHITE]=0;
  squarePtr->num_threats[BLACK]=0;
  squarePtr->turns_held[WHITE]=0;
  squarePtr->turns_held[BLACK]=0;
  squarePtr->num_adjacent_pieces[WHITE]=0;
  squarePtr->num_adjacent_pieces[BLACK]=0;

  return squarePtr;
}


/*
 *   square_init
 * This function creates a square_t struct used to represent
 * a square on a chess board.  It is responsible for allocating
 * resources for the struct, and initializing its values
 *   @param rank the row on the chess board.
 *   @param file the file on the chess board.
 *   @return a * to the new square_t
 */
square_t* square_init(int rank, int file)
{
  if (rank < 0 || rank >= BOARD_SIZE) return NULL;
  if (file < 0 || file >= BOARD_SIZE) return NULL;
  /* Allocate memory and return on failure */
  square_t *newSquare = malloc(sizeof(square_t) );
  if (!newSquare)
    return NULL;


  /* Set member variables to func arguments*/
  newSquare->rank=rank;
  newSquare->file=file;

  /* Zero out all other arguments */
  newSquare->piece=NULL;
  newSquare->num_threats[WHITE]=0;
  newSquare->num_threats[BLACK]=0;
  newSquare->turns_held[WHITE]=0;
  newSquare->turns_held[BLACK]=0;
  newSquare->num_adjacent_pieces[WHITE]=0;
  newSquare->num_adjacent_pieces[BLACK]=0;
  return newSquare;
}

/*
 *   square_destroy
 * This function is responsible for cleaning up all resources
 * involved in a square_t struct.  These include the memory
 * allocated for the struct itself, as well as deleting any
 * piece resident in this square.
 *   @param squareToDel A pointer to the square to destroy
 */
void square_destroy(square_t *squareToDel)
{
  /* If theres a piece on the square, delete it */
    //This behavior might need to change once I
    //flesh out how coupled piece/square binding is
  if (squareToDel->piece)
    piece_destroy(squareToDel->piece);

  free(squareToDel);
}

/*
 *   square_equal
 * Checks that two square_t structs are equal.  Calls
 * piece_equal to compare any pieces, as well as comparing
 * stats.
 *   @param square_one the first square_t* for the comparison
 *   @param square_two the second square_t* for the comparison
 *   @return true if the squares are equal, false otherwise
 */
bool square_equal(square_t *square_one, square_t *square_two)
{
  /* If both square_t*'s are null, then they match */
  if (!square_one && !square_two) return true;

  /* If only one square_t* is null, then they don't match */
  if (square_one == NULL || square_two == NULL) return false;

  /* Check core square fields: position and pieces */
  /* 'no_match' flag is OR'd against all conditions checking for equality */
  bool no_match = false;
  no_match |= square_one->rank != square_two->rank;
  no_match |= square_one->file != square_two->file;
  no_match |= ! piece_equal(square_one->piece, square_two->piece);

  /* Check statistic fields */
  no_match |= square_one->num_threats[WHITE] != square_two->num_threats[WHITE];
  no_match |= square_one->num_threats[BLACK] != square_two->num_threats[BLACK];
  no_match |= square_one->turns_held[WHITE] != square_two->turns_held[WHITE];
  no_match |= square_one->turns_held[BLACK] != square_two->turns_held[BLACK];
  no_match |= square_one->num_adjacent_pieces[WHITE] != square_two->num_adjacent_pieces[WHITE];
  no_match |= square_one->num_adjacent_pieces[BLACK] != square_two->num_adjacent_pieces[BLACK];

  return !no_match;
}

/*
 *   square_copy_deep
 * Makes a deep copy of a square_t object, creating a
 * completely independent clone from the original.  This
 * means that the piece_t on the square (if there is one)
 * is also deep copied.
 *   @param orig the square_t to copy
 *   @return a * to the square_t to clone
 */
square_t* square_copy_deep(square_t *orig)
{
  if (!orig) return NULL;

  /* Init the new clone */
  square_t *new_sq = square_init_blank();
  if (!new_sq) return NULL;

  /* Copy over fields */
  new_sq->rank=orig->rank;
  new_sq->file=orig->file;
  new_sq->num_threats[WHITE]=orig->num_threats[WHITE];
  new_sq->turns_held[WHITE]=orig->num_threats[WHITE];
  new_sq->num_adjacent_pieces[WHITE]=orig->num_adjacent_pieces[WHITE];
  new_sq->num_threats[BLACK]=orig->num_threats[BLACK];
  new_sq->turns_held[BLACK]=orig->num_threats[BLACK];
  new_sq->num_adjacent_pieces[BLACK]=orig->num_adjacent_pieces[BLACK];

  /* Copy over piece if there is one */
  if (orig->piece) {
    new_sq->piece = piece_copy_deep(orig->piece);
  }

  return new_sq;
}

/*
 *   square_load_from_file
 * Reads a square_t struct from the point in a file stream
 * referenced by the provided FILE * handle.
 *   @param handle a FILE* seek'd to the location to read the square_t.
 *   @return a * to the loaded square_t, or NULL on an error
 */
square_t* square_load_from_file(FILE *handle)
{
  if (!handle) return NULL;

  /* Allocate square_t resources */
  square_t *load = square_init_blank();
  if (!load) return NULL;

  /* Read struct fields to blank struct */
  fread(&(load->rank),sizeof(load->rank),1,handle);
  fread(&(load->file),sizeof(load->file),1,handle);
  fread(&(load->num_threats[WHITE]),sizeof(load->num_threats[WHITE]),1,handle);
  fread(&(load->num_threats[BLACK]),sizeof(load->num_threats[BLACK]),1,handle);
  fread(&(load->turns_held[WHITE]),sizeof(load->turns_held[WHITE]),1,handle);
  fread(&(load->turns_held[BLACK]),sizeof(load->turns_held[BLACK]),1,handle);
  fread(&(load->num_adjacent_pieces[WHITE]),sizeof(load->num_adjacent_pieces[WHITE]),1,handle);
  fread(&(load->num_adjacent_pieces[BLACK]),sizeof(load->num_adjacent_pieces[BLACK]),1,handle);

  /* Read the piece ptr from the file.  If the val is
   * non-NULL, trigger the piece's load from the file and
   * overwrite the ptr with the piece's new location
   */
  fread(&(load->piece), sizeof(load->piece), 1, handle);
  if (load->piece)
  {
    load->piece=piece_load_from_file(handle);

    /* If load failed, free everything and return */
    if (!load->piece)
    {
      square_destroy(load);
      return NULL;
    }
  }

  /* Everything is loaded, return successfully loaded obj */
  return load;
}

/*
 *   square_save_to_file
 * Writes a square_t struct to the FILE stream provided.
 *   @param handle the file stream to write the square_t to.
 *   @param squareToSave the square_t object to save.  The
 *    square_t is not destroyed by this method, just saved.
 *   @return 0 on success, non-0 on error
 */
int square_save_to_file(FILE *handle, square_t *squareToSave)
{
  if (!handle || !squareToSave) return -1;
  /* Write standard square_t fields to file */
  fwrite(&(squareToSave->rank),sizeof(squareToSave->rank),1,handle);
  fwrite(&(squareToSave->file),sizeof(squareToSave->file),1,handle);
  fwrite(&(squareToSave->num_threats[WHITE]),sizeof(squareToSave->num_threats[WHITE]),1,handle);
  fwrite(&(squareToSave->num_threats[BLACK]),sizeof(squareToSave->num_threats[BLACK]),1,handle);
  fwrite(&(squareToSave->turns_held[WHITE]),sizeof(squareToSave->turns_held[WHITE]),1,handle);
  fwrite(&(squareToSave->turns_held[BLACK]),sizeof(squareToSave->turns_held[BLACK]),1,handle);
  fwrite(&(squareToSave->num_adjacent_pieces[WHITE]),sizeof(squareToSave->num_adjacent_pieces[WHITE]),1,handle);
  fwrite(&(squareToSave->num_adjacent_pieces[BLACK]),sizeof(squareToSave->num_adjacent_pieces[BLACK]),1,handle);

  /* Write piece_t* to file.  The ptr isn't really important here,
   * but it serves as a flag to say whether the piece exists
   */
  fwrite(&(squareToSave->piece), sizeof(piece_t*), 1, handle);
  if (squareToSave->piece)
    piece_save_to_file(handle, squareToSave->piece);

  /* This shouldn't be just a ret 0..I should check the ret vals
   * of each fwrite
   */
  return 0;
}

/*
 *   square_add_piece
 * Adds a piece struct to a square struct, updating each to
 * represent the change
 *   @param location the square a piece is being added to
 *   @param added the piece to add to 'location'
 *   @return 0 on success, -1 on invalid args, -2 on piece already present
 */
int
square_add_piece(square_t *location, piece_t *added)
{
  if (!location || !added) return -1;
  if (location->piece) return -2;

  location->piece = added;
  added->rank = location->rank;
  added->file = location->file;
  return 0;
}
