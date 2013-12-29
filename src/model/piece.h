
#ifndef _PIECE_H
#define _PIECE_H

#include <stdio.h>
#include <stdbool.h>

#include "chess.h"


/* Enum defined for piece type */
enum piece_type {ROOK,KNIGHT,BISHOP,KING,QUEEN,PAWN};
typedef enum piece_type piece_type_t;

/* Enum defined for piece state (alive, dead) */
enum piece_health {ALIVE,DEAD};
typedef enum piece_health piece_health_t;

/*
 * The piece struct holds info about a piece on the board,
 * such as its current location, type and color.
 */

/* Declare piece struct */
struct piece {
  piece_type_t type;  /* type of chess piece (ROOK, BISHOP, etc) */
  color_t color;      /* Piece color (WHITE or BLACK) */
  piece_health_t health;/* Is piece ALIVE or DEAD */
  int rank;
  int file;
};
typedef struct piece piece_t;

piece_t*
piece_init(color_t color, piece_type_t type,
  piece_health_t health, int rank, int file
);

piece_t*
piece_init_alive(color_t color, piece_type_t type, int rank, int file);

/* Func handles mem de-alloc for heap-based structs */
/* All heap piece_t structs should be destroyed here */
void piece_destroy(piece_t *ptr);

/* This function checks whether two piece_t* structs
 * are 'value'-equal.  Two pieces are considered equal
 * if they're on they have all the same state information.
 */
bool piece_equal(piece_t *piece_one, piece_t *piece_two);

/* This function copies struct fields for a piece_t
 * struct and returns the result as a piece_t
 */
piece_t* piece_copy_deep(piece_t *orig);

/* Function kills a piece by setting internal enum
 * In the future, this function will also be responsible
 * for potentially removing the piece from the board, or
 * adding it to the dead-queue (displayed next to board)
 * (SECONDARY FUNCTIONALITY UNIMPLEMENTED..just marks struct field)
 */
int piece_kill(piece_t *ptr);

/*  This function loads a piece struct from a file, and
 *  returns the collected object as a piece_t struct.  The
 *  FILE * stream is assumed to be at the starting point
 *  of the piece.
 */
piece_t* piece_load_from_file(FILE *handle);

/*  This function saves a piece struct to a file, and
 *  returns an integer representing the status of the operation
 *  The function is not responsible for destroying or free-ing
 *  the piece.
 */
int piece_save_to_file(FILE *handle, piece_t *pieceToSave);
#endif
