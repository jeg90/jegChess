#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "unity.h"

#include "square.h"
#include "piece.h"

#define TEMP_SQUARE_FILE "temp_square.chs"

void setUp(void) {}
void tearDown(void) {}


void test_square_init_blank_returns_valid_square()
{
  square_t *blank_square = square_init_blank();
  TEST_ASSERT_MESSAGE(
    blank_square != NULL, "Expected square_init_blank to return valid square"
  );
}

void test_square_init_returns_NULL_when_given_bad_rank()
{
  square_t *bad_rank_square = square_init(-1, 4);
  TEST_ASSERT_MESSAGE(
    bad_rank_square == NULL,
    "Expected square_init to return NULL when given bad rank"
  );
}

void test_square_init_returns_NULL_when_given_bad_file()
{
  square_t *bad_file_square = square_init(4, -1);
  TEST_ASSERT_MESSAGE(
    bad_file_square == NULL,
    "Expected square_init to return NULL when given bad file"
  );
}

void test_square_init_returns_valid_square_when_given_good_args()
{
  square_t *good_square = square_init(3, 3);
  TEST_ASSERT_MESSAGE(
    good_square != NULL,
    "Expected square_init to return non-NULL when given good rank, file"
  );
  square_destroy(good_square);
}

void test_square_equal_returns_true_when_both_args_are_NULL()
{
  bool equal_result = square_equal(NULL, NULL);
  TEST_ASSERT_MESSAGE(
    equal_result == true,
    "Expected square_equal to return true when both args are NULL"
  );
}

void test_square_equal_returns_false_when_only_one_arg_is_NULL()
{
  square_t *valid_square = square_init(0, 0);
  bool result = square_equal(valid_square, NULL);
  TEST_ASSERT_MESSAGE(
    result == false,
    "Expected square_equal to return false when comparing valid square vs NULL"
  );
  square_destroy(valid_square);
}

void test_square_equal_returns_false_when_ranks_conflict()
{
  square_t *square_one = square_init(0, 0);
  square_t *square_two = square_init(1, 0);
  bool result = square_equal(square_one, square_two);

  TEST_ASSERT_MESSAGE(
    result == false,
    "Expected return of false when comparing squares w/ diff ranks"
  );

  square_destroy(square_one);
  square_destroy(square_two);
}

void test_square_equal_returns_false_when_files_conflict()
{
  square_t *square_one = square_init(0, 0);
  square_t *square_two = square_init(0, 1);
  bool result = square_equal(square_one, square_two);

  TEST_ASSERT_MESSAGE(
    result == false,
    "Expected return of false when comparing squares w/ diff files"
  );

  square_destroy(square_one);
  square_destroy(square_two);
}

void test_square_equal_returns_false_when_stats_conflict()
{
  bool result;
  square_t *square_one = square_init(0, 0);
  square_t *square_two = square_init(0, 0);

  /* Change one of square_two's stats */
  square_two->num_threats[WHITE]++;
  result = square_equal(square_one, square_two);
  TEST_ASSERT_MESSAGE(
    result == false,
    "Expected return of false when comparing squares w/ diff stats"
  );

  square_destroy(square_one);
  square_destroy(square_two);
}

void test_square_Equal_returns_true_when_squares_identical()
{
  bool result;
  square_t *square_one = square_init(0, 0);
  square_t *square_two = square_copy_deep(square_one);

  result = square_equal(square_one, square_two);
  TEST_ASSERT_MESSAGE(
    result == true,
    "Expected return of true when comparing identical squares"
  );
}

void test_square_copy_deep_with_bad_args_returns_NULL()
{
  square_t *copy;
  copy = square_copy_deep(NULL);

  TEST_ASSERT_MESSAGE(
    copy == NULL,
    "Expected square_copy_deep to return NULL when given NULL as argument"
  );
}

void test_square_copy_deep_returns_square_at_new_address()
{
  square_t *original, *copy;
  original = square_init(0, 0);
  copy = square_copy_deep(original);

  TEST_ASSERT_MESSAGE(
    original != copy,
    "Expected square_copy_deep to return a copy at a different address"
  );

  square_destroy(original);
  square_destroy(copy);
}

void test_square_copy_deep_square_values_match()
{
  square_t *original, *copy;
  original = square_init(0, 0);
  copy = square_copy_deep(original);

  TEST_ASSERT_MESSAGE(
    square_equal(original, copy),
    "Expected squares to be equal after deep copy"
  );
}

void test_square_save_to_file_returns_neg_one_for_invalid_args()
{
  FILE *bad_handle = NULL, *good_handle = NULL;
  square_t *bad_square = NULL, *good_square = NULL;
  int result;

  good_handle = fopen(TEMP_SQUARE_FILE, "w");
  good_square = square_init(0, 0);

  /* Give square_save_to_file a bad FILE handle */
  result = square_save_to_file(bad_handle, good_square);
  TEST_ASSERT_MESSAGE(
    result == -1,
    "Expected square_save_to_file to return -1 when given bad FILE*"
  );

  /* Give square_save_to_file a bad square_t* */
  result = square_save_to_file(good_handle, bad_square);
  TEST_ASSERT_MESSAGE(
    result == -1,
    "Expected square_save_to_file to return -1 when given bad square_t*"
  );

  fclose(good_handle);
  square_destroy(good_square);
}

void test_square_save_to_file_returns_zero_on_success()
{
  FILE *save_file;
  square_t *square_to_save;
  int result;

  save_file = fopen(TEMP_SQUARE_FILE, "w");
  square_to_save = square_init(0, 0);

  result = square_save_to_file(save_file, square_to_save);
  TEST_ASSERT_MESSAGE(
    result == 0,
    "Expected square_save_to_file to return 0 when given good args"
  );

  fclose(save_file);
  square_destroy(square_to_save);
}

void test_square_load_from_file_returns_NULL_for_invalid_args()
{
  FILE *bad_handle = NULL;
  square_t *loaded_square;

  loaded_square = square_load_from_file(bad_handle);
  TEST_ASSERT_MESSAGE(
    loaded_square == NULL,
    "Expected square_load_from_file to return NULL when given bad FILE*"
  );
}

static void
utility_save_square(square_t *square_to_save)
{
  FILE *handle;

  handle = fopen(TEMP_SQUARE_FILE, "w");
  square_save_to_file(handle, square_to_save);
  fclose(handle);
}

void test_square_load_from_file_loaded_board_matches_original()
{
  FILE *handle;
  square_t *original;
  square_t *loaded;
  bool result;

  original = square_init(0, 0);
  utility_save_square(original);
  handle = fopen(TEMP_SQUARE_FILE, "r");
  loaded = square_load_from_file(handle);
  fclose(handle);

  result = square_equal(original, loaded);
  TEST_ASSERT_MESSAGE(
    result == true,
    "Expected re-loaded square to match original saved version"
  );
}

void test_square_add_piece_returns_neg_one_for_NULL_args()
{
  int result;
  square_t *good_square = square_init(0, 0);
  square_t *bad_square = NULL;

  piece_t *good_piece = piece_init_alive(WHITE, ROOK, 0, 0);
  piece_t *bad_piece = NULL;

  result = square_add_piece(bad_square, good_piece);
  TEST_ASSERT_MESSAGE(
    result == -1,
    "Expected add_piece to fail when using invalid square"
  );

  result = square_add_piece(good_square, bad_piece);
  TEST_ASSERT_MESSAGE(
    result == -1,
    "Expected add_piece to fail when using invalid piece"
  );

  square_destroy(good_square);
  piece_destroy(good_piece);
}

void test_square_add_piece_returns_neg_two_if_square_has_piece_already()
{
  int result;
  square_t *good_square = square_init(0, 0);
  piece_t *good_piece = piece_init_alive(WHITE, ROOK, 0, 0);
  piece_t *second_piece = piece_init_alive(WHITE, BISHOP, 0, 0);

  square_add_piece(good_square, good_piece);

  result = square_add_piece(good_square, second_piece);
  TEST_ASSERT_MESSAGE(result == -2, "Expected duplicate piece add to return error");

  square_destroy(good_square);
  piece_destroy(second_piece);
}

void test_square_add_piece_returns_zero_on_success()
{
  int result;
  square_t *good_square = square_init(0, 0);
  piece_t *good_piece = piece_init_alive(WHITE, ROOK, 0, 0);

  result = square_add_piece(good_square, good_piece);
  TEST_ASSERT_MESSAGE(result == 0, "Expected successful piece-add");

  square_destroy(good_square);
}
