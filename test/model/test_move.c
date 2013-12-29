#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "move.h"
#include "square.h"
#include "piece.h"

void setUp(void) {}
void tearDown(void) {}

static move_t* utility_create_simple_move()
{
  move_t *move;
  square_t *to, *from;
  piece_t *mover;

  mover = piece_init_alive(WHITE, ROOK, 0, 0);
  from = square_init(0, 0);
  square_add_piece(from, mover);
  to = square_init(0, 1);
  move = move_init(from, to);
  return move;
}

void test_score_is_initialized_to_zero()
{
  move_t *move = utility_create_simple_move();

  TEST_ASSERT_MESSAGE(
    move->score == 0,
    "Expected move->score to be initialized to 0"
  )

  move_destroy(move);
}

void test_score_can_be_changed()
{
  const int expected_score = 42;

  move_t *move = utility_create_simple_move();
  move_set_score(move, expected_score);

  TEST_ASSERT_MESSAGE(
    move->score == expected_score,
    "Calling move_set_score didn't update the score"
  );
}
