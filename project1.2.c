// Zinovii Kava
// 25.03.2026
// Project


#include <stdio.h>    // printf, scanf

#include <stdlib.h>   // rand(), srand()
#include <time.h>     // time(NULL)  - to seed srand

#include <string.h>   // strcmp(), strcpy()
#include <stdio.h>    // scanf(), printf()



//SPACE(grid storage) — 2D arrays

int grid[ROWS][COLS];    // stores treasure values
int state[ROWS][COLS];   // stores visit flags

//MULTIPLAYER — turn switching in playOneGame()

if (turn == 0)
turn = 1;
else
turn = 0;


//BITWISE — reading treasure bits

int isTrap(int treasureValue)
{
    return (treasureValue & BIT_TRAP);   // check bit 0
}

int hasGold(int treasureValue)
{
    return (treasureValue & BIT_GOLD);   // check bit 1
}

int getGoldAmount(int treasureValue)
{
    int masked = treasureValue & GOLD_MASK;  // keep bits 4-7
    int shifted = masked >> 4;                // shift right
    return shifted;
}
//And setting cell state flags:
state[row][col] = state[row][col] | VISITED;   // set visited
state[row][col] = state[row][col] | WAS_TRAP;  // set trap flag
state[row][col] = state[row][col] | HAD_GOLD;  // set gold flag


//SCORE — pointer modifying score directly

int* scorePtr = &players[turn].score;  // pointer to score

*scorePtr = *scorePtr - 1;   // trap: lose 1
*scorePtr = *scorePtr + goldAmount;  // gold: add points


//WIN — score check after every move

if (*scorePtr >= WIN_SCORE)
{
    return 1;   // player wins
}
//And the board-full check:if i updated that 
if (countFreeCells() == 0)
{
    if (players[0].score > players[1].score)
        winnerIdx = 0;
    else if (players[1].score > players[0].score)
        winnerIdx = 1;
    else
        winnerIdx = -1;   // draw
}


//RAND — building treasure values in initGrid()

roll = rand() % 100;          // 0-99 to decide cell type
goldAmt = (rand() % 8) + 1;     // gold amount 1-8
