#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "unity.h"
#include "piece.h"

void setUp(void) {}
void tearDown(void) {}

void test_piece_init_initializes_all_fields(void)
{
  piece_t *new_piece = piece_init(WHITE, ROOK, ALIVE, 0, 0);
  TEST_ASSERT_MESSAGE(
    new_piece->color == WHITE,
    "Expected piece_init to set color to match argument"
  );
  TEST_ASSERT_MESSAGE(
    new_piece->color == WHITE,
    "Expected piece_init to set color to match argument"
  );
  TEST_ASSERT_MESSAGE(
    new_piece->type == ROOK,
    "Expected piece_init to set type to match argument"
  );
  TEST_ASSERT_MESSAGE(
    new_piece->health == ALIVE,
    "Expected piece_init to set health to match argument"
  );
  TEST_ASSERT_MESSAGE(
    new_piece->rank == 0,
    "Expected piece_init to set rank to match argument"
  );
  TEST_ASSERT_MESSAGE(
    new_piece->file == 0,
    "Expected piece_init to set file to match argument"
  );

  piece_destroy(new_piece);
}

void test_piece_init_alive_initializes_all_fields()
{

  piece_t *new_piece = piece_init_alive(WHITE, ROOK, 0, 0);

  TEST_ASSERT_MESSAGE(
    new_piece->color == WHITE,
    "Expected piece_init to set color to match argument"
  );
  TEST_ASSERT_MESSAGE(
    new_piece->type == ROOK,
    "Expected piece_init_alive to set type to match argument"
  );
  TEST_ASSERT_MESSAGE(
    new_piece->rank == 0,
    "Expected piece_init_alive to set rank to match argument"
  );
  TEST_ASSERT_MESSAGE(
    new_piece->file == 0,
    "Expected piece_init_alive to set file to match argument"
  );

  piece_destroy(new_piece);
}

void test_piece_init_alive_defaults_health_to_alive()
{
  piece_t *live_piece = piece_init_alive(WHITE, ROOK, 0, 0);

  TEST_ASSERT_MESSAGE(
    live_piece->health == ALIVE,
    "Expected piece_init_alive to default health to 'alive'"
  );

  piece_destroy(live_piece);
}

void test_piece_copy_deep_wont_copy_a_bad_piece()
{
  piece_t *bad_piece = NULL;
  piece_t *bad_copy = NULL;
  bad_copy = piece_copy_deep(bad_piece);
  TEST_ASSERT_MESSAGE(
    bad_copy == NULL,
    "Expected piece_copy_deep to fail when given a bad piece"
  );
}

void test_piece_copy_deep_copies_all_fields()
{
  piece_t *original_piece = piece_init(WHITE, ROOK, ALIVE, 0, 0);
  piece_t *copy;

  copy = piece_copy_deep(original_piece);
  TEST_ASSERT_MESSAGE(
    piece_equal(original_piece, copy),
    "Expected copy to match original piece"
  );

  piece_destroy(original_piece);
  piece_destroy(copy);
}

void test_piece_kill_cant_kill_a_dead_piece()
{
  int result;
  piece_t *dead_piece = piece_init(WHITE, ROOK, DEAD, 0, 0);

  result = piece_kill(dead_piece);
  TEST_ASSERT_MESSAGE(
    result == -1,
    "Expected piece_kill to return a warning when killing a dead piece"
  );

  piece_destroy(dead_piece);
}

void test_piece_kill_kills_living_pieces()
{
  int result;
  piece_t *alive_piece = piece_init(WHITE, ROOK, ALIVE, 0, 0);

  result = piece_kill(alive_piece);
  TEST_ASSERT_MESSAGE(
    result ==0,
    "Expected piece_kill to report success when killing a living piece"
  );

  TEST_ASSERT_MESSAGE(
    alive_piece->health == DEAD,
    "Expected recently killed piece to be dead"
  );
}

void test_piece_equal_returns_true_when_all_fields_match()
{
  int rank = 0;
  int file = 0;
  piece_t *piece_one = piece_init_alive(WHITE, ROOK, rank, file);
  piece_t *piece_two = piece_init_alive(WHITE, ROOK, rank, file);
  bool equal = piece_equal(piece_one, piece_two);
  TEST_ASSERT_MESSAGE(
    equal,
    "Expected piece_equal to return true, when pieces match"
  );

  piece_destroy(piece_one);
  piece_destroy(piece_two);
}

void test_piece_equal_returns_false_when_colors_dont_match()
{
  int rank = 0;
  int file = 0;
  piece_t *piece_one = piece_init_alive(WHITE, ROOK, rank, file);
  piece_t *piece_two = piece_init_alive(BLACK, ROOK, rank, file);
  bool equal = piece_equal(piece_one, piece_two);
  TEST_ASSERT_MESSAGE(
    !equal,
    "Expected piece_equal to return false, when colors conflict"
  );
}

void test_piece_equal_returns_false_when_types_dont_match()
{
  int rank = 0;
  int file = 0;
  piece_t *piece_one = piece_init_alive(WHITE, QUEEN, rank, file);
  piece_t *piece_two = piece_init_alive(WHITE, KING, rank, file);
  bool equal = piece_equal(piece_one, piece_two);
  TEST_ASSERT_MESSAGE(
    !equal,
    "Expected piece_equal to return false when types conflict"
  );
}

void test_piece_equal_returns_false_when_health_doesnt_match()
{
  int rank = 0;
  int file = 0;
  piece_t *piece_one = piece_init(WHITE, ROOK, ALIVE, rank, file);
  piece_t *piece_two = piece_init(WHITE, ROOK, DEAD, rank, file);
  bool equal = piece_equal(piece_one, piece_two);
  TEST_ASSERT_MESSAGE(!equal,
    "Expected piece_equal to return false when health conflicts"
  );
}
