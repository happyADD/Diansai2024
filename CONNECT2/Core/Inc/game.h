#ifndef _GAME_H_
#define _GAME_H_


#include "stdlib.h"
#include "stdint.h"
extern int mode;
uint8_t checkWin1(uint8_t map[9]) ;
void game(uint8_t map1[9],int *best_position);

#endif // !_GAME_H_

