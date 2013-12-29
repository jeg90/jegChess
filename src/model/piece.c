#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "piece.h"

/* This file contains impl of functions affecting piece struct
 * objs. This includes init and destroy, as well as mutators
 */

/*
 *   piece_init
 * This function allocates and initializes all the values
 * in the piece_t struct.  For most use cases, these initial
 * values will need to be overwritten, however, this func
 * does provide a convenient way to init a struct without
 * providing initial values for all fields
 *   @return piece_t* to the new piece_t struct
 */
static piece_t*
piece_init_blank(void)
{
  /* Allocate space for piece struct */
  piece_t *piecePtr = malloc(sizeof(piece_t) );
  if (!piecePtr) return NULL;

  piecePtr->color=WHITE;
  piecePtr->type=PAWN;
  piecePtr->health=DEAD;
  piecePtr->rank=-1;
  piecePtr->file=-1;

  return piecePtr;
}


/*
 *   piece_init
 * This func takes init vals for each member in the struct
 * and allocates an initialized piece_t struct on the heap
 *   @param color an enum specifying the piece is white or black
 *   @param type ROOK, BISHOP, KNIGHT, etc.
 *   @param health  an enum specifying if the piece is alive
 *   @return a piece_t* to the created piece_t struct
 */
piece_t*
piece_init(color_t color, piece_type_t type,
  piece_health_t health, int rank, int file
)
{
  piece_t *piecePtr;

  /* Allocate space for piece struct */
  piecePtr= (piece_t*) malloc(sizeof(piece_t) );
  if (piecePtr==NULL)
    return NULL;

  /* Init member values */
  piecePtr->color=color;
  piecePtr->type=type;
  piecePtr->health=health;
  piecePtr->rank = rank;
  piecePtr->file = file;

  /* Return initialized struct pointer */
  return piecePtr;
}




/*
 *   piece_init_alive
 * This function takes init values for each member in the piece_t struct
 * and allocates an initialized chess piece_t struct on the heap.  The
 * piece is assumed to be alive on creation.
 *   @param color an enum specifying whether the piece is white or black
 *   @param type an enum specifying which type of chess piece will be created
 *   @return a piece_t* to the created piece_t object
 */
piece_t*
piece_init_alive(color_t color, piece_type_t type, int rank, int file)
{
  piece_t *piecePtr;

  /* Allocate space for new piece */
  piecePtr= (piece_t*) malloc(sizeof(piece_t) );
  if (piecePtr==NULL)
    return NULL;

  /* Init member values */
  piecePtr->color = color;
  piecePtr->type = type;
  piecePtr->health = ALIVE;
  piecePtr->rank = rank;
  piecePtr->file = file;

  /* Return initialized struct pointer */
  return piecePtr;
}



/*
 *   piece_destroy:
 * Since piece_t contains no references to heap data, all it takes
 * to 'destroy' the struct is a call to free.
 *   @param ptr a piece_t* to the piece to destroy/cleanup
 */
void piece_destroy(piece_t *ptr)
{
  free(ptr);
}

/*
 *   piece_equal
 * Checks that two piece_t structs are equal.  NULL pointers
 * are considered equal.
 *   @param piece_one the first piece_t* for the comparison
 *   @param piece_two the second piece_t* for the comparison
 *   @return true if the pieces are equal, false otherwise
 */
bool piece_equal(piece_t *piece_one, piece_t *piece_two)
{
  /* If both piece_t*'s are null, then they match */
  if (! piece_one && ! piece_two) return true;

  /* If only one of the piece_t*'s is null, then they dont match */
  if (! piece_one || ! piece_two) return false;

  /* Since we know both that both our piece_t*'s are valid, check each field */
  if (piece_one->type != piece_two->type) return false;
  if (piece_one->color != piece_two->color) return false;
  if (piece_one->health != piece_two->health) return false;
  if (piece_one->rank != piece_two->rank) return false;
  if (piece_one->file != piece_two->file) return false;

  return true;
}

/*
 *   piece_copy_deep
 * Makes a deep copy of a piece_t struct
 *   @param orig The original piece_t struct to copy
 *   @return a * to the new piece_t
 */
piece_t* piece_copy_deep(piece_t *orig)
{
  if (!orig) return NULL;

  /* Allocate new piece_t object */
  piece_t *new_piece=piece_init_blank();
  if (!new_piece) return NULL;

  /* Copy over fields from orig */
  new_piece->type=orig->type;
  new_piece->color=orig->color;
  new_piece->health=orig->health;
  new_piece->rank=orig->rank;
  new_piece->file=orig->file;

  return new_piece;
}


/*
 *   piece_kill
 * Marks a piece as killed by setting the internal member var.
 * May also be responsible for removing piece from board, and
 * adding piece to dead-queue
 *   @param piece the piece_t struct to mark as killed
 *   @return -1 if the piece was already dead, 0 if the piece was alive
 */
int piece_kill(piece_t *piece)
{
  int alreadyDead=0;
  if (piece->health==DEAD) alreadyDead = -1;

  piece->health=DEAD;
  return alreadyDead;
}


/*
 *   piece_load_from_file
 * Loads a piece from a file.
 *   @param handle an open FILE * handle pointing to the starting location
 *    of a piece_t struct in a file.
 *   @return a piece_t* referencing the newly loaded piece_t struct
 */
piece_t * piece_load_from_file(FILE *handle)
{
  /* Allocate memory for piece */
  piece_t *loaded = piece_init_blank();
  if (!loaded) return NULL;

  /* Read the piece type from file */
  fread(&(loaded->type),sizeof(piece_type_t),1,handle);
  /* Read the piece color from file */
  fread(&(loaded->color),sizeof(color_t),1,handle);
  /* Read the piece health from file */
  fread(&(loaded->health),sizeof(piece_health_t),1,handle);
  /* Read the rank and file */
  fread(&(loaded->rank),sizeof(loaded->rank),1,handle);
  fread(&(loaded->file),sizeof(loaded->file),1,handle);

  return loaded;
}


/*
 *   piece_save_to_file
 * Saves a piece to the file via the provided FILE *
 *   @param handle a FILE * handle opened and seek'd to the location that
 *    a piece_t struct should be written.
 *   @param pieceToSave a ptr to the piece_t struct written to the file
 *   @return 0 on success, non-zero for error cases.
 */
int piece_save_to_file(FILE *handle, piece_t *pieceToSave)
{
  /* Write the piece type to the file */
  fwrite(&(pieceToSave->type),sizeof(piece_type_t),1,handle);
  /* Write the piece color to the file */
  fwrite(&(pieceToSave->color),sizeof(color_t),1,handle);
  /* Write the piece health/state to the file */
  fwrite(&(pieceToSave->health),sizeof(piece_health_t),1,handle);
  /*Write the current rank and file positions to the file */
  fwrite(&(pieceToSave->rank),sizeof(pieceToSave->rank),1,handle);
  fwrite(&(pieceToSave->file),sizeof(pieceToSave->file),1,handle);

  /* Return 0 for success: in the future this should check that
   * all the fwrite's were successful, but that can be added later
   */
  return 0;
}
