#ifndef _HISTORY_STACK_H
#define _HISTORY_STACK_H
/*
 * The history_stack struct is a data structure used to record chess moves
 * comprising the game's 'history'.  It can be mentally pictured as a CS stack
 * of Move objects.  Each acted-out move pushes an element onto the stack.
 *
 * This model also provides a framework to allow users to 'UNDO' their most
 * recent move by popping the element from the stack.
*/

/* Declare piece struct */

struct history_stack {

};
typedef struct history_stack history_stack_t;

#endif
