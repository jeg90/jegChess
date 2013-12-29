#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "board.h"
#include "square.h"
#include "piece.h"
#include "file-utils.h"

void setUp(void) {}
void tearDown(void) {}

void test_board_init_returns_non_null()
{
  board_t *board = board_init();
  TEST_ASSERT_MESSAGE(
    board != NULL,
    "Expected board_init to return board_t, but found NULL"
  );
  board_destroy(board);
}

void test_board_init_moves_white_first()
{
  board_t *board = board_init();
  TEST_ASSERT_MESSAGE(
    board->moves_next == WHITE,
    "Expected board_init to let WHITE move first, but found BLACK"
  );
  board_destroy(board);
}

void test_board_init_makes_square_array()
{
  board_t *board = board_init();
  TEST_ASSERT_MESSAGE(
    board->spaces != NULL,
    "Expected board_init to create initialize an array of squares, but found NULL"
  );
  board_destroy(board);
}

void test_board_init_creates_each_square()
{
  board_t *board = board_init();
  int rank, file;
  for (rank = 0; rank < BOARD_SIZE; rank++) {
    for (file = 0; file < BOARD_SIZE; file++) {
      TEST_ASSERT_MESSAGE(
        board->spaces[rank][file] != NULL,
        "Expected square to be square_t, but found NULL"
      );
    }
  }
  board_destroy(board);
}

void test_board_init_sets_no_pieces()
{
  board_t *board = board_init();
  int rank, file;
  for (rank = 0; rank < BOARD_SIZE; rank++) {
    for (file = 0; file < BOARD_SIZE; file++) {
      TEST_ASSERT_MESSAGE(
        board->spaces[rank][file]->piece == NULL,
        "Expected all squares to be empty, found non-null piece_t*"
      );
    }
  }
  board_destroy(board);
}

void test_board_init_start_returns_non_null()
{
  board_t *board = board_init_start();
  TEST_ASSERT_MESSAGE(
    board != NULL,
    "Expected board_init_start to return board_t, but found NULL"
  );
  board_destroy(board);
}

void test_board_init_start_moves_white_first()
{
  board_t *board = board_init_start();
  TEST_ASSERT_MESSAGE(
    board->moves_next == WHITE,
    "Expected board_init to let WHITE move first, but found BLACK"
  );
  board_destroy(board);
}

void test_board_init_start_makes_square_array()
{
  board_t *board = board_init_start();
  TEST_ASSERT_MESSAGE(
    board != NULL,
    "Expected board_init_start to set square array, found NULL pointer"
  );
  board_destroy(board);
}

void test_board_init_start_creates_each_square()
{
  board_t *board = board_init_start();
  int rank, file;
  for (rank = 0; rank < BOARD_SIZE; rank++) {
    for (file = 0; file < BOARD_SIZE; file++) {
      TEST_ASSERT_MESSAGE(
        board->spaces[rank][file] != NULL,
        "Expected valid square_t*, found NULL pointer"
      );
    }
  }
  board_destroy(board);
}

void test_board_init_start_sets_pieces()
{
  int rank, file;
  board_t *board = board_init_start();
  /* Check for white's pieces (ranks: 0-1) */
  for (rank = 0; rank <= 1; rank++) {
    for (file = 0; file < BOARD_SIZE; file++) {
      TEST_ASSERT_MESSAGE(
        board->spaces[rank][file] != NULL,
        "Expected square_t* to be non-NULL but found NULL"
      );
      TEST_ASSERT_MESSAGE(
        board->spaces[rank][file]->piece != NULL,
        "Expected valid piece_t*, but found NULL"
      );
    }
  }

  /* Check for black's pieces (ranks: 6-7) */
  for (rank = 6; rank < BOARD_SIZE; rank++) {
    for (file = 0; file < BOARD_SIZE; file++) {
      TEST_ASSERT_MESSAGE(
        board->spaces[rank][file] != NULL,
        "Expected square_t* to be non-NULL, but found NULL"
      );
      TEST_ASSERT_MESSAGE(
        board->spaces[rank][file]->piece != NULL,
        "Expected valid black piece_t* found NULL"
      );
    }
  }

  board_destroy(board);
}

void test_board_copy_deep_with_bad_args_returns_null()
{
  board_t *copy = board_copy_deep(NULL);
  TEST_ASSERT_MESSAGE(
    copy == NULL,
    "Expected copy to return NULL when given NULL argument"
  );
}

void test_board_copy_deep_returns_board_at_new_address()
{
  board_t *original = board_init_start();
  board_t *copy = board_copy_deep(original);
  TEST_ASSERT_MESSAGE(
    original != copy,
    "Expected original and copy board_t* to point to different places"
  );

  board_destroy(original);
  board_destroy(copy);
}

void test_board_copy_deep_moves_next_is_preserved()
{
  board_t *original = board_init_start();
  board_t *copy = board_copy_deep(original);
  TEST_ASSERT_MESSAGE(
    original->moves_next == copy->moves_next,
    "Expected original and copy boards to have same 'next-movers'"
  );

  board_destroy(original);
  board_destroy(copy);
}

/* Check that pieces are in the same place on original and copy boards */
void test_board_copy_deep_piece_values_match()
{
  board_t *original = board_init_start();
  board_t *copy = board_copy_deep(original);
  int rank, file;

  for (rank = 0; rank < BOARD_SIZE; rank++) {
    for (file = 0; file < BOARD_SIZE; file++) {
      /* Check that all original pieces can be found on the new board */
      piece_t *original_piece = original->spaces[rank][file]->piece;
      if (original_piece) {
        /* Check that the copied piece matches the original */
        piece_t *copied_piece = copy->spaces[rank][file]->piece;
        bool equal = piece_equal(original_piece, copied_piece);
        TEST_ASSERT_MESSAGE(
          equal,
          "Original and copied pieces dont match"
        );
      }
    }
  }

  board_destroy(original);
  board_destroy(copy);
}

void test_board_copy_deep_squares_pointers_are_different()
{
  board_t *original = board_init_start();
  board_t *copy = board_copy_deep(original);
  int rank, file;
  for (rank = 0; rank < BOARD_SIZE; rank++) {
    for (file = 0; file < BOARD_SIZE; file++) {
      TEST_ASSERT_MESSAGE(
        original->spaces[rank][file] != copy->spaces[rank][file],
        "Expected square_t ptrs to be different after deep copy"
      );
    }
  }

  board_destroy(original);
  board_destroy(copy);
}

void test_board_copy_deep_piece_pointers_are_different()
{
  board_t *original = board_init_start();
  board_t *copy = board_copy_deep(original);
  int rank, file;

  for (rank = 0; rank < BOARD_SIZE; rank++) {
    for (file = 0; file < BOARD_SIZE; file++) {
      if (original->spaces[rank][file]->piece) {
        TEST_ASSERT_MESSAGE(
          original->spaces[rank][file]->piece !=
          copy->spaces[rank][file]->piece,
          "Expected piece_t*'s to be different after deep copying board"
        );
      }
    }
  }

  board_destroy(original);
  board_destroy(copy);
}

void test_board_save_to_file_returns_neg_one_for_invalid_args()
{
  const char *filename = "saved1.chs";
  board_t *to_save = board_init_start();
  bool overwrite = true;
  int result;

  /* Return error code when given bad filename */
  result = board_save_to_file(NULL, to_save, overwrite);
  TEST_ASSERT_MESSAGE(
    result == -1,
    "Expected to return -1 when given bad filename"
  );

  /* Return error code when given bad board */
  result = board_save_to_file(filename, NULL, overwrite);
  TEST_ASSERT_MESSAGE(
    result == -1,
    "Expected to return -1 when given bad board_t"
  );

  board_destroy(to_save);
}

void test_board_save_to_file_returns_neg_two_if_file_exists()
{
  const char *filename = "existing_board.chs";
  board_t *to_save = board_init_start();
  bool overwrite = false;
  int result;

  /* Return error code if the file exists and we dont want to overwrite */
  result = board_save_to_file(filename, to_save, overwrite);
  TEST_ASSERT_MESSAGE(
    result == -2,
    "Expected to return error when not overwriting existing file"
  );

  board_destroy(to_save);
}

void test_board_save_to_file_saved_board_matches_original_when_loaded()
{
  const char *filename = "test_board.chs";
  board_t *to_save = board_init_start();
  bool overwrite = true;
  int result;

  /* Save the file */
  result = board_save_to_file(filename, to_save, overwrite);
  TEST_ASSERT_MESSAGE(
    result == 0,
    "Expected overwriting save of valid board to succeed"
  );

  /* Reload the file into a new board_t* */
  board_t *loaded_board;
  loaded_board = board_load_from_file(filename);
  TEST_ASSERT_MESSAGE(
    loaded_board != NULL,
    "Expected board load from valid file to succeed"
  );

  /* Check that the old and new board match */
  bool boards_match;
  boards_match = board_equal(to_save, loaded_board);
  TEST_ASSERT_MESSAGE(
    boards_match,
    "Expected the loaded board to match the saved one"
  );

  board_destroy(loaded_board);
  board_destroy(to_save);
}

void test_board_load_from_file_returns_null_for_invalid_args()
{
  board_t *loaded_board;
  loaded_board = board_load_from_file(NULL);
  TEST_ASSERT_MESSAGE(
    loaded_board == NULL,
    "Expected board_load_from_file to return NULL when given NULL filename"
  );
}

void test_board_load_from_file_returns_null_if_filename_doesnt_exist()
{
  const char *filename = "bad_board_filename.chs";
  board_t *loaded_board = board_load_from_file(filename);
  TEST_ASSERT_MESSAGE(
    loaded_board == NULL,
    "Expected board_load_from_file to return NULL when given non-existent file"
  );
}
