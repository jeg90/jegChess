#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "main.h"

/* Import Objects from Chess model*/
#include "model/chess.h"
#include "model/board.h"
#include "model/square.h"
#include "model/piece.h"

/* Import Objects from Display code */
#include "display/chess-screen.h"
#include "display/window-params.h"
#include "display/display-data.h"
/* A global board representing the current state of the game */
board_t *game_board;

int main()
{
  int error=0;

  /* Initialize the Chess Model (Quit on Failure) */
  error=main_init_model();
  if (error)
  {
    printf("Error initializing model\n");
    exit(0);
  }

  /* Initialize UI component of app (Quit on Failure */
  error=main_init_display();
  if (error) {
    printf("Error initializing display\n");
    exit(0);
  }
  pthread_exit(NULL);
}



/*
 *    main_init_model
 *  This function initializes the Chess Model..in simpler terms it
 *  is responsible for setting up the global board, and any other
 *  structs that will be used for emulating Chess Rules.
 *
 *  Ret: 0 on success, 1 on failure
 */
int main_init_model()
{
  /* Set-up the game board */
  game_board=board_init_start();

  /* If gameBoard is still NULL, our init failed */
  if (!game_board) return 1;

  /* ...otherwise we're fine! */
  return 0;
}


/*
 *    main_init_display()
 *  This function is responsible for initializing the display
 *  component of the application.  This involves initializing the
 *  display_t struct, creating a thread to execute OpenGL code
 *  handling the UI aspect of the game.
 *
 *  Ret: a status integer: 0 on success, 1 on a failure
 */
int main_init_display()
{
  pthread_t disp_thread;
  display_data_t *disp = display_data_init_default_window_params(game_board);

  pthread_create(&disp_thread,NULL,chess_screen_main, (void *)disp);
  return 0;
}
