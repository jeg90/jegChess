#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <SDL/SDL.h>
#include <string.h>
#include "window-params.h"
#include "display-data.h"
#include "cursor.h"
#include "key-press.h"
#include "../misc.h"

/* A pointer to the canonical board-data to display */
display_data_t* disp_data;

/* Static display resources */
static GLuint piece_textures[24];

/*
 *   gl_load_texture
 * This function adds an OpenGL texture to the static piece_textures
 * array.  The code was mostly copied from a tutorial found on
 * nehe.gamedev.net.  My understanding of OpenGL is pretty limited
 * so this code is provided AS-IS at the moment.
 *   @param filename the name of the bmp file to load
 *   @param index the index to load the texture into
 *   @return TRUE if the texture load was successful, FALSE otherwise
 */
static int
gl_load_texture(const char *filename,int index)
{
  int status = FALSE;

  SDL_Surface *texture_image[1];

  if (texture_image[0] = SDL_LoadBMP(filename)) {
    status = TRUE;

    glGenTextures(1, &piece_textures[index]);

    glBindTexture(GL_TEXTURE_2D, piece_textures[index]);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture_image[0]->w,
                 texture_image[0]->h, 0, GL_BGR,
                 GL_UNSIGNED_BYTE, texture_image[0]->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  }

  if (texture_image[0]) SDL_FreeSurface(texture_image[0]);
  return status;
}

/*
 *   load_all_textures
 * Loads all the textures (OpenGL primitives akin to 'pictures') into
 * an global array ('piece_textures')
 */
static void
load_all_textures()
{
  gl_load_texture("/usr/share/jegChess/img/black_rook_on_black.bmp",0);
  gl_load_texture("/usr/share/jegChess/img/black_rook_on_white.bmp",1);
  gl_load_texture("/usr/share/jegChess/img/black_knight_on_black.bmp",2);
  gl_load_texture("/usr/share/jegChess/img/black_knight_on_white.bmp",3);
  gl_load_texture("/usr/share/jegChess/img/black_bishop_on_black.bmp",4);
  gl_load_texture("/usr/share/jegChess/img/black_bishop_on_white.bmp",5);
  gl_load_texture("/usr/share/jegChess/img/black_king_on_black.bmp",6);
  gl_load_texture("/usr/share/jegChess/img/black_king_on_white.bmp",7);
  gl_load_texture("/usr/share/jegChess/img/black_queen_on_black.bmp",8);
  gl_load_texture("/usr/share/jegChess/img/black_queen_on_white.bmp",9);
  gl_load_texture("/usr/share/jegChess/img/black_pawn_on_black.bmp",10);
  gl_load_texture("/usr/share/jegChess/img/black_pawn_on_white.bmp",11);

  gl_load_texture("/usr/share/jegChess/img/white_rook_on_black.bmp",12);
  gl_load_texture("/usr/share/jegChess/img/white_rook_on_white.bmp",13);
  gl_load_texture("/usr/share/jegChess/img/white_knight_on_black.bmp",14);
  gl_load_texture("/usr/share/jegChess/img/white_knight_on_white.bmp",15);
  gl_load_texture("/usr/share/jegChess/img/white_bishop_on_black.bmp",16);
  gl_load_texture("/usr/share/jegChess/img/white_bishop_on_white.bmp",17);
  gl_load_texture("/usr/share/jegChess/img/white_king_on_black.bmp",18);
  gl_load_texture("/usr/share/jegChess/img/white_king_on_white.bmp",19);
  gl_load_texture("/usr/share/jegChess/img/white_queen_on_black.bmp",20);
  gl_load_texture("/usr/share/jegChess/img/white_queen_on_white.bmp",21);
  gl_load_texture("/usr/share/jegChess/img/white_pawn_on_black.bmp",22);
  gl_load_texture("/usr/share/jegChess/img/white_pawn_on_white.bmp",23);
}

/*
 *   lookup_texture_index
 * Translates a piece_t struct into the index of the texture
 * to display the piece as on-screen
 *   @param to_draw the piece_t struct to find a texture for
 *   @return the index in the piece_textures array
 */
static int
lookup_texture_index(piece_t *to_draw)
{
  if (!to_draw) return -1;

  /* The texture for a piece is affected by its side (WHITE or BLACK), its
   * type (ROOK, KNIGHT, etc), and the background square color.
   */
  int black_square = (to_draw->rank + to_draw->file) % 2 == 0;
  int white_square = !black_square;

  switch(to_draw->color) {
    case BLACK:
      switch(to_draw->type) {
        case ROOK:
          return black_square ? 0 : 1;
        case KNIGHT:
          return black_square ? 2 : 3;
        case BISHOP:
          return black_square ? 4 : 5;
        case KING:
          return black_square ? 6 : 7;
        case QUEEN:
          return black_square ? 8 : 9;
        case PAWN:
          return black_square ? 10 : 11;
        default:
          /* Should prob report an error */
          return -1;
      }
    case WHITE:
      switch(to_draw->type) {
        case ROOK:
          return black_square ? 12 : 13;
        case KNIGHT:
          return black_square ? 14 : 15;
        case BISHOP:
          return black_square ? 16 : 17;
        case KING:
          return black_square ? 18 : 19;
        case QUEEN:
          return black_square ? 20 : 21;
        case PAWN:
          return black_square ? 22 : 23;
        default:
          /* Should prob return an error */
          return -1;
      }
    default:
      /* Should prob report an error */
      return -1;
  }
}

/*
 *   draw_board_square
 * Draw the square of the board located at the given rank and file
 *   @param rank the rank of the square
 *   @param file the file of the square
 */
static void
draw_board_square(int rank, int file)
{
  /* Figure out square's X,Y position */
  float base_x = DEFAULT_BORDER_COEFF;
  float base_y = DEFAULT_BORDER_COEFF;
  float sq_x = base_x + (float)file*SQUARE_WIDTH;
  float sq_y = base_y + (float)rank*SQUARE_HEIGHT;
  glPushMatrix();
    glTranslatef(sq_x,sq_y,0.0);

    /* Set the square color */
    if ( (rank+file)%2==1) glColor3f(1.0,1.0,1.0);
    else glColor3f(0.0,0.0,0.0);

    /* Draw the actual square */
    glBegin(GL_POLYGON);
      glVertex3f(0.0f,0.0f,0.0f);
      glVertex3f(SQUARE_WIDTH,0.0f,0.0f);
      glVertex3f(SQUARE_WIDTH,SQUARE_HEIGHT,0.0f);
      glVertex3f(0.0f,SQUARE_HEIGHT,0.0f);
    glEnd();
  glPopMatrix();
}

/*
 *   draw_board_squares
 * Iterate through all squares on the chess board, drawing each
 */
static void
draw_board_squares()
{
  int rank=0, file=0;
  /* Iterate over all squares */
  for (rank = 0; rank < BOARD_SIZE; rank++) {
    for (file = 0; file < BOARD_SIZE; file++) {
      draw_board_square(rank,file);
    }
  }
}

/*
 *   draw_board_piece
 * Draws the provided piece_t piece on the chess-screen.
 *   @param to_draw the piece_t struct to be drawn on the chess window
 */
static void
draw_board_piece(piece_t *to_draw)
{

  /* Figure out square's X,Y position */
  float base_x = DEFAULT_BORDER_COEFF;
  float base_y = DEFAULT_BORDER_COEFF;
  float sq_x = base_x + (float)to_draw->file * SQUARE_WIDTH;
  float sq_y = base_y + (float)to_draw->rank * SQUARE_HEIGHT;

  /* Find texture matching piece */
  int tex_index;
  tex_index = lookup_texture_index(to_draw);
  if (tex_index < 0) return; /* If texture couldnt be found, give up */

  glBindTexture( GL_TEXTURE_2D, piece_textures[tex_index] );
  /* Do actual drawing */
  glPushMatrix();
    glTranslatef(sq_x,sq_y,0.0);
    glBegin(GL_QUADS);
      glColor3f(1.0f,1.0f,1.0f);
      /* Botton left of texture and quad */
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f(0.0f,0.0f,0.001f);
      /* Bottom right of texture and quad */
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f(SQUARE_WIDTH,0.0f,0.001f);
      /* Top right of texture and quad */
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f(SQUARE_WIDTH,SQUARE_HEIGHT,0.001f);
      /* Top Left of texture and quad */
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f(0.0f,SQUARE_HEIGHT,0.001f);
    glEnd();
  glPopMatrix();
}

/*
 *   draw_cursor
 * Places a small cursor on the appropriate square of the board
 */
static void
draw_cursor()
{
  /* Draw nothing if the cursor is disabled */
  if (! disp_data->cursor->visible) return;

  /* Figure out square's X,Y position */
  float base_x = DEFAULT_BORDER_COEFF;
  float base_y = DEFAULT_BORDER_COEFF;
  float sq_x = base_x + (float)disp_data->cursor->file * SQUARE_WIDTH;
  float sq_y = base_y + (float)disp_data->cursor->rank * SQUARE_HEIGHT;
  float curs_x = sq_x + CURS_WIDTH_OFFSET;
  float curs_y = sq_y + CURS_HEIGHT_OFFSET;

  /* Draw the cursor */
  glPushMatrix();
    glTranslatef(curs_x, curs_y, 0.0f);
    glColor3f(CURSOR_RED, CURSOR_GREEN, CURSOR_BLUE);
    glBegin(GL_QUADS);

      glVertex3f(0.0f,0.0f,0.0f);
      glVertex3f(CURS_WIDTH,0.0f,0.0f);
      glVertex3f(CURS_WIDTH,CURS_HEIGHT,0.0f);
      glVertex3f(0.0f,CURS_HEIGHT,0.0f);
    glEnd();
  glPopMatrix();
}

/*
 *   draw_board_pieces
 * Iterates through all chess-pieces and draws them on the board
 */
static void
draw_board_pieces()
{
  /* Turn on texture mapping */
  glEnable(GL_TEXTURE_2D);

  /* Iterate over all squares */
  int rank = 0, file = 0;
  for (rank = 0; rank < BOARD_SIZE; rank++) {
    for (file = 0; file < BOARD_SIZE; file++) {
      piece_t *piece_to_draw =
          disp_data->board_on_screen->spaces[rank][file]->piece;
      if (piece_to_draw) draw_board_piece(piece_to_draw);
    }
  }

  /* Turn off texture-mapping..if this isn't done, attempting to draw
   * regular squares creates black dead-space
   */
  glDisable(GL_TEXTURE_2D);
}

/*
 *   display
 * The entry point for all actual OpenGL code.  Passed as a function
 * pointer to the OpenGL callback loop.
 */
static void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  draw_board_squares();
  draw_board_pieces();
  draw_cursor();

  glFlush();
}

/*
 *   glut_init_window
 * Init glut library state variables for window creation.  The GLUT lib
 * is responsible for OS interactions involving Window setup/creation
 *   @param disp the Window display preferences (size, location, etc)
 */
static void
glut_init_window(window_params_t* disp)
{
  /* Set up dummy args for glutInit */
  int argc=1;
  char *argv[argc];
  argv[0] = strdup("jegChess");

  /* Run glut calls */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(disp->win_height, disp->win_width);
  glutInitWindowPosition(200,200);
  glutCreateWindow("jegChess");
}

/*
 *   gl_init_state
 * Init the OpenGL state machine
 *   @param disp the display parameters used to draw (bg color, etc)
 */
static void
gl_init_state(window_params_t *disp)
{
  /* Set bg color */
  glClearColor(disp->bg_color->red_color,disp->bg_color->green_color,
               disp->bg_color->blue_color, 0.0);

  /* Init OpenGL mode values */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
  load_all_textures();
}

/*
 *   gl_enter_main_loop
 * Set up callbacks for display loop, and enter
 * Relatedly: "Speak friend, and enter"
 */
static void
gl_enter_main_loop()
{
  /* Set 'display' function callback */
  glutDisplayFunc(display);

  /* Set key-release callbacks for ASCII and non-ASCII keys */
  glutKeyboardUpFunc(key_press_normal_released);
  glutSpecialUpFunc(key_press_special_released);

  glutMainLoop();
}

/*
 *   chess_screen_main
 * The entry point for the display thread.  Sets up UI
 * callbacks and spins in display look
 *   @param display_data_as_void a void* holding a window_params_t with display
 *    preferences
 *   @return supposed to return a void *, always NULL
 */
void*
chess_screen_main(void *display_data_as_void)
{
  /* Place display data in static global. */
  disp_data=(display_data_t *)display_data_as_void;

  /* Init graphics libraries and begin display */
  glut_init_window(disp_data->display_prefs);
  gl_init_state(disp_data->display_prefs);
  gl_enter_main_loop();
}
