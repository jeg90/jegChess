#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <SDL/SDL.h>
#include "key-press.h"
#include "cursor.h"
#include "chess-screen.h"
#include "display-data.h"
#include <stdio.h>


/* State information about how to handle key presses */
enum display_mode {
  /* Computer's turn, accept no keypresses */
  CPU_TURN,
  /* Player turn: arrows, undo, redo, select */
  PC_NO_PIECE_SELECTED,
  /* Player's turn, piece selected: arrows, cancel, move */
  PC_PIECE_SELECTED
};
typedef enum display_mode display_mode_t;
static display_mode_t key_press_mode = PC_NO_PIECE_SELECTED;
/* End data section */

/* Forward Declarations */
static void
do_handle_normal_released_nps(unsigned char key, int x, int y);
static void
do_handle_normal_released_ps(unsigned char key, int x, int y);
static void
do_handle_arrow_key(int key, int x, int y);
/* End forward declarations */

/*
 *   key_press_normal_released
 * This function is passed as a callback to OpenGL/GLUT to
 * handle the release of ASCII keys (i.e. 'a' or '[' but
 * not LEFT_ARROW.  For the purposes of controlling
 * a chess game, 'key-released' is generally more meaningful
 * than 'key-pressed', so more work happens here than in
 * key_press_pressed
 *   @param key the character code of the pressed key
 *   @param x the x position of the mouse at key-release-time
 *   @param y the y position of the mouse at key-release-time
 */
void
key_press_normal_released(unsigned char key, int x, int y)
{
  /* Key handling depends on the current control mode
   * (i.e. no key presses accepted during Computer's turn)
   */
  switch(key_press_mode)
  {
    case PC_NO_PIECE_SELECTED:
      do_handle_normal_released_nps(key, x, y);
      break;
    case PC_PIECE_SELECTED:
      do_handle_normal_released_ps(key, x, y);
      break;
    case CPU_TURN:
      /* No-op */
      break;
    default:
      break;
  }
  return;
}

/*
 *   do_handle_normal_released_nps
 * Handles ASCII keys pressed by the user when in NO_PIECE_SELECTED mode
 * This code must handle the following keys:
 *    'u': Undo the previous move
 *    'r': Redo a single undone move
 *    'p': Pause the current game
 *    's': Save the current game
 *    'l': Load a new game
 *    'ENTER': select the piece on the given square
 *   @param key the character code for the pressed key
 *   @param x the x value of the mouse when the key was pressed
 *   @param y the y value of the mouse when the key was pressed
 */
static void
do_handle_normal_released_nps(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 'u':
    case 'U':
      /* Unimplemented */
      break;
    case 'r':
    case 'R':
      /* Unimplemented */
      break;
    case 'p':
    case 'P':
      /* Unimplemented */
      break;
    case 's':
    case 'S':
      /* Unimplemented */
      break;
    case 'l':
    case 'L':
      /* Unimplemented */
      break;
    default:
      break;
  }

  glutPostRedisplay();
}

/*
 *   do_handle_normal_released_ps
 * Handles ASCII keys pressed by the user when in PIECE_SELECTED mode
 * This function must handle the following keys:
 *    'c': cancel the currently selected piece
 *    'ENTER': Move the selected piece to the current square
 *   @param key the ID of the key pressed
 *   @param x the x value of the mouse when the key was pressed
 *   @param y the y value of the mouse when the key was pressed
 */
static void
do_handle_normal_released_ps(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 'c':
    case 'C':
      /* Unimplemented */
      break;
    case '\n':
      /* Unimplemented */
      break;
    default:
      break;
  }

  glutPostRedisplay();
}

/*
 *   key_press_special_released
 * This function is passed as a callback to OpenGL/GLUT to
 * handle the release of 'special' (non_ASCII) keys such as the
 * ARROW keys.  For the purposes of controlling a
 * chess game, 'key-released' is generally more meaningful than
 * 'key-pressed', so more work happens here than in the
 * 'pressed' version of this function.
 *   @param key an indentifier for the special key
 *   @param x the x position of the mouse at key-release-time
 *   @param y the y position of the mouse at key-release-time
 */
void
key_press_special_released(int key, int x, int y)
{
  /* Key handling depends on the current control mode
   * (i.e. you can't accept presses during Computer's turn)
   */
  switch(key_press_mode)
  {
    case PC_NO_PIECE_SELECTED:
    case PC_PIECE_SELECTED:
      do_handle_arrow_key(key, x, y);
      break;
    case CPU_TURN:
      /* No-op */
      break;
    default:
      break;
  }

  glutPostRedisplay();
}

/*
 *   do_handle_arrow_key
 * Handle the press of an arrow key in either NO_PIECE_SELECTED
 * or PIECE_SELECTED mode.
 *   @param key the ID of the key pressed
 *   @param x the x value of the mouse at key-press-time
 *   @param y the y value of the mouse at key-press-time
 */
static void
do_handle_arrow_key(int key, int x, int y)
{
  switch(key)
  {
    case GLUT_KEY_LEFT:
      cursor_move(disp_data->cursor, LEFT);
      break;
    case GLUT_KEY_RIGHT:
      cursor_move(disp_data->cursor, RIGHT);
      break;
    case GLUT_KEY_UP:
      cursor_move(disp_data->cursor, UP);
      break;
    case GLUT_KEY_DOWN:
      cursor_move(disp_data->cursor, DOWN);
      break;
    default:
      break;
  }

  glutPostRedisplay();
}
