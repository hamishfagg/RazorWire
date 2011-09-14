#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

bool debug;
bool joystick;
bool xbox;
int joy_id;
unsigned long seed;
int state;
/* STATES:
        0 - Startup -> Startup, graphics setup.
        1 - Menu    -> Player is in the main menu.
        2 - Preview -> A level preview is in progress.
        3 - Play    -> A game is currently in progress (game loop is running).
        4 - Paused  -> Current game is paused.
*/

#endif // MAIN_H_INCLUDED
