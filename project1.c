// Zinovii Kava
// 19.03.2026
// lab8

/*
====================================================
  BIT-MAZE TREASURE HUNT
  B.Eng. Software & Electronic Engineering - Year 2
  C/C++ Programming for Electronics
  Group Mini Project I

  Topics covered:
  - 2D arrays       : grid[][] and state[][]
  - Bitwise ops     : reading bits from treasure values
  - Strings         : player names, password
  - Pointers        : passing score to functions
  - rand()          : random treasure generation
  - Multiplayer     : two players take turns
  - Score / Win     : first to 50 wins
  - Play Again      : loop to replay
  - Total Wins      : remembered across games
====================================================
*/

/* Visual Studio safe string functions */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* On GCC/Linux scanf_s and strcpy_s do not exist.
   These lines make the code compile on both. */
#ifndef _MSC_VER
#define scanf_s(fmt, ...)        scanf(fmt, __VA_ARGS__)
#define strcpy_s(dst, sz, src)   strcpy(dst, src)
#endif

   /* ====================================================
      CONSTANTS
      ==================================================== */
#define ROWS       6        /* grid has 6 rows          */
#define COLS       6        /* grid has 6 columns       */
#define WIN_SCORE  20       /* first to 20 points wins  */
#define NAME_LEN   32       /* max player name length   */
#define PASSWORD   "bit42"  /* game password (5 chars)  */

      /* ====================================================
         BITWISE FLAGS - used in state[][] to track each cell
         We use OR  ( |= ) to SET  a flag
         We use AND ( &  ) to CHECK a flag
         ==================================================== */
#define VISITED   0x01   /* binary: 0000 0001  cell was picked  */
#define WAS_TRAP  0x02   /* binary: 0000 0010  it was a trap    */
#define HAD_GOLD  0x04   /* binary: 0000 0100  it had gold      */

         /* ====================================================
            TREASURE BIT MASKS
            Each cell holds a random number 0-255.
            We read specific bits to find out what is inside:
              bit 0      = 1 means TRAP
              bit 1      = 1 means GOLD
              bits 4-7   = amount of gold (value 0 to 15)
            ==================================================== */
#define BIT_TRAP   0x01   /* 0000 0001 */
#define BIT_GOLD   0x02   /* 0000 0010 */
#define GOLD_MASK  0xF0   /* 1111 0000  - mask to keep only bits 4-7 */

            /* ====================================================
               PLAYER  - stores name, current score, and total wins
               ==================================================== */
typedef struct
{
    char name[NAME_LEN];
    int  score;       /* points in this game           */
    int  totalWins;   /* wins across all games played  */
} Player;

/* ====================================================
   GLOBAL GRIDS
   grid[][]  - the treasure values (random 0-255)
   state[][] - what has happened to each cell (flags)
   ==================================================== */
int grid[ROWS][COLS];
int state[ROWS][COLS];


/* ====================================================
   FUNCTION: checkPassword
   Asks the user to type the password.
   Allows 3 attempts.
   Returns 1 if correct, 0 if all attempts failed.
   ==================================================== */
int checkPassword(void)
{
    char input[20];   /* string to store what user types */
    int  tries = 3;

    printf("\n");
    printf("  +----------------------------------+\n");
    printf("  |  GAME IS PASSWORD PROTECTED      |\n");
    printf("  |  You have 3 attempts             |\n");
    printf("  +----------------------------------+\n");

    while (tries > 0)
    {
        printf("\n  Enter password (%d attempt(s) left): ", tries);
        scanf_s("%19s", input, (unsigned)sizeof(input));

        /* strcmp compares two strings.
           It returns 0 when they are equal. */
        if (strcmp(input, PASSWORD) == 0)
        {
            printf("  Correct! Welcome!\n");
            return 1;
        }

        tries = tries - 1;
        printf("  Wrong password!\n");
    }

    printf("\n  Too many wrong attempts. Game closed.\n");
    return 0;
}


/* ====================================================
   FUNCTION: setupPlayers
   Asks for both player names.
   p1 and p2 are POINTERS - changes go into the original struct.
   totalWins is NOT reset here so it survives across games.
   ==================================================== */
void setupPlayers(Player* p1, Player* p2)
{
    printf("\n  Enter Player 1 name: ");
    scanf_s("%31s", p1->name, NAME_LEN);

    printf("  Enter Player 2 name: ");
    scanf_s("%31s", p2->name, NAME_LEN);
}


/* ====================================================
   FUNCTION: initGrid
   Fills the 6x6 grid with controlled treasure values.
   Also resets state[][] so all cells are unvisited.

   Problem with pure rand() % 256:
     bit 0 is set in ~50% of all numbers -> too many traps!

   Solution: we build the treasure value manually:
     - Only 15% chance of a trap  (bit 0)
     - 50% chance of gold         (bit 1)
     - Gold amount in bits 4-7    (1 to 15)
     - Rest are empty cells
   ==================================================== */
void initGrid(void)
{
    int r, c;
    int roll;        /* random number to decide cell type */
    int goldAmt;     /* gold amount to put in bits 4-7   */
    int value;       /* treasure value we build manually */

    for (r = 0; r < ROWS; r++)
    {
        for (c = 0; c < COLS; c++)
        {
            state[r][c] = 0;       /* reset: not visited */
            value = 0;       /* start with 0       */

            roll = rand() % 100;   /* random 0 to 99     */

            if (roll < 15)
            {
                /* 15% chance: TRAP - set bit 0 */
                value = value | 0x01;
            }
            else if (roll < 65)
            {
                /* 50% chance: GOLD - set bit 1, put amount in bits 4-7 */
                goldAmt = (rand() % 8) + 1;    /* gold amount: 1 to 8  */
                value = value | 0x02;           /* set bit 1 (gold)     */
                value = value | (goldAmt << 4); /* put amount in bits 4-7 */
            }
            /* remaining 35%: empty cell, value stays 0 */

            grid[r][c] = value;
        }
    }
}


/* ====================================================
   FUNCTION: printGrid
   Draws the board on screen.
   p1 and p2 are POINTERS to the Player structs.
   currentPlayer is 0 for Player 1, 1 for Player 2.

   What each symbol means on screen:
     .  = not picked yet (unknown)
     G  = had gold (already found)
     T  = was a trap (already found)
     X  = visited but empty
   ==================================================== */
void printGrid(Player* p1, Player* p2, int currentPlayer)
{
    int r, c;

    printf("\n");
    printf("  +================================================+\n");
    printf("  |          BIT-MAZE TREASURE HUNT                |\n");
    printf("  +================================================+\n");

    /* Show both player scores using pointer -> to access struct fields */
    printf("  |  %-12s : %3d pts    %-12s : %3d pts  |\n",
        p1->name, p1->score,
        p2->name, p2->score);

    printf("  |  Wins: %-3d                    Wins: %-3d        |\n",
        p1->totalWins, p2->totalWins);

    printf("  +------------------------------------------------+\n");

    /* Show whose turn it is */
    if (currentPlayer == 0)
        printf("  |  Turn: %-38s|\n", p1->name);
    else
        printf("  |  Turn: %-38s|\n", p2->name);

    printf("  +------------------------------------------------+\n");

    /* Column header row */
    printf("  |        ");
    for (c = 0; c < COLS; c++)
    {
        printf(" C%d ", c);
    }
    printf("       |\n");

    /* Each row of the grid */
    for (r = 0; r < ROWS; r++)
    {
        printf("  |  R%d    ", r);

        for (c = 0; c < COLS; c++)
        {
            /* Check if cell was visited using bitwise AND */
            if (state[r][c] & VISITED)
            {
                if (state[r][c] & HAD_GOLD)
                    printf(" G  ");        /* was gold */
                else if (state[r][c] & WAS_TRAP)
                    printf(" T  ");        /* was a trap */
                else
                    printf(" X  ");        /* was empty */
            }
            else
            {
                printf(" .  ");            /* not visited yet */
            }
        }

        printf("    |\n");
    }

    printf("  +------------------------------------------------+\n");
    printf("  |  Legend:  .=unknown  G=gold  T=trap  X=empty  |\n");
    printf("  +================================================+\n\n");
}


/* ====================================================
   FUNCTION: isTrap
   Checks bit 0 of the treasure value.
   Returns 1 if it is a trap, 0 if not.
   ==================================================== */
int isTrap(int treasureValue)
{
    return (treasureValue & BIT_TRAP);
}


/* ====================================================
   FUNCTION: hasGold
   Checks bit 1 of the treasure value.
   Returns 1 if there is gold, 0 if not.
   ==================================================== */
int hasGold(int treasureValue)
{
    return (treasureValue & BIT_GOLD);
}


/* ====================================================
   FUNCTION: getGoldAmount
   Reads bits 4 to 7 of the treasure value.
   These bits hold the gold amount (0 to 15).
   We use AND to keep only those bits, then shift
   right by 4 positions to get the actual number.
   ==================================================== */
int getGoldAmount(int treasureValue)
{
    int masked = treasureValue & GOLD_MASK;   /* keep only bits 4-7   */
    int shifted = masked >> 4;                 /* shift right 4 places */
    return shifted;
}


/* ====================================================
   FUNCTION: countFreeCells
   Counts how many cells have NOT been visited yet.
   Used to detect when the board is completely full.
   ==================================================== */
int countFreeCells(void)
{
    int r, c;
    int count = 0;

    for (r = 0; r < ROWS; r++)
    {
        for (c = 0; c < COLS; c++)
        {
            /* if VISITED bit is NOT set, cell is free */
            if (!(state[r][c] & VISITED))
            {
                count = count + 1;
            }
        }
    }

    return count;
}


/* ====================================================
   FUNCTION: doPlayerMove
   Asks the current player to pick a row and column.
   scorePtr is a POINTER to the player's score.
   We use *scorePtr to read or change the score directly.
   Returns 1 if the player has won (reached WIN_SCORE).
   Returns 0 if the game continues.
   ==================================================== */
int doPlayerMove(int* scorePtr)
{
    int row;
    int col;
    int treasureValue;
    int goldAmount;

    /* Ask for row */
    printf("  Pick a row    (0 to %d): ", ROWS - 1);
    scanf_s("%d", &row);

    /* Ask for column */
    printf("  Pick a column (0 to %d): ", COLS - 1);
    scanf_s("%d", &col);

    /* Check the numbers are inside the grid */
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
    {
        printf("\n  That is outside the grid! You lose your turn.\n");
        printf("  Press ENTER to continue..."); getchar(); getchar();
        return 0;
    }

    /* Check the cell has not been picked before */
    if (state[row][col] & VISITED)
    {
        printf("\n  That cell was already picked! You lose your turn.\n");
        printf("  Press ENTER to continue..."); getchar(); getchar();
        return 0;
    }

    /* Mark the cell as visited using bitwise OR */
    state[row][col] = state[row][col] | VISITED;

    /* Get the treasure value stored in this cell */
    treasureValue = grid[row][col];

    /* ---- Decode the treasure using bitwise operators ---- */

    if (isTrap(treasureValue))
    {
        /* Bit 0 is set -> TRAP! Lose 1 point */
        state[row][col] = state[row][col] | WAS_TRAP;
        *scorePtr = *scorePtr - 1;
        printf("\n  TRAP!  Cell [%d][%d] was a trap.  -1 point.\n", row, col);
        printf("  Your score is now: %d\n", *scorePtr);
    }
    else if (hasGold(treasureValue))
    {
        /* Bit 1 is set -> GOLD! Add the gold amount from bits 4-7 */
        goldAmount = getGoldAmount(treasureValue);
        state[row][col] = state[row][col] | HAD_GOLD;
        *scorePtr = *scorePtr + goldAmount;
        printf("\n  GOLD!  Cell [%d][%d] had %d gold.  +%d points.\n",
            row, col, goldAmount, goldAmount);
        printf("  Your score is now: %d\n", *scorePtr);
    }
    else
    {
        /* No trap, no gold -> empty cell */
        printf("\n  Empty.  Cell [%d][%d] had nothing.\n", row, col);
        printf("  Your score is still: %d\n", *scorePtr);
    }

    /* Show the bit breakdown so you can see how bitwise ops work */
    printf("  [Treasure value: 0x%02X  |  bit0(trap)=%d  bit1(gold)=%d  bits4-7(gold amount)=%d]\n",
        treasureValue,
        isTrap(treasureValue) ? 1 : 0,
        hasGold(treasureValue) ? 1 : 0,
        getGoldAmount(treasureValue));

    /* Check if this player has reached the winning score */
    if (*scorePtr >= WIN_SCORE)
    {
        return 1;   /* this player wins! */
    }

    printf("\n  Press ENTER to continue..."); getchar(); getchar();
    return 0;   /* game continues */
}


/* ====================================================
   FUNCTION: playOneGame
   Runs one complete game between two players.
   Players take turns until someone reaches WIN_SCORE
   or the board is completely full.
   Returns: 0 if Player 1 wins
            1 if Player 2 wins
           -1 if it is a draw
   ==================================================== */
int playOneGame(Player* players)
{
    int turn = 0;    /* 0 = Player 1 goes first, 1 = Player 2 */
    int gameOver = 0;
    int winnerIdx = -1;

    /* Reset scores to 0 at the start of each game */
    players[0].score = 0;
    players[1].score = 0;

    /* Fill the grid with new random treasure values */
    initGrid();

    while (gameOver == 0)
    {
        /* Before each turn: check if the board is completely full */
        if (countFreeCells() == 0)
        {
            printf("\n  The board is full! No more moves available.\n");

            /* The player with the higher score wins */
            if (players[0].score > players[1].score)
                winnerIdx = 0;
            else if (players[1].score > players[0].score)
                winnerIdx = 1;
            else
                winnerIdx = -1;   /* equal scores = draw */

            gameOver = 1;
            break;
        }

        /* Print blank lines then the grid */
        printf("\n\n\n");
        printGrid(&players[0], &players[1], turn);

        /* Tell the current player it is their turn */
        printf("  --- %s, it is your turn ---\n\n", players[turn].name);

        /* Pass a POINTER to the current player's score */
        int* scorePtr = &players[turn].score;

        /* Do the move. If it returns 1 this player just won. */
        gameOver = doPlayerMove(scorePtr);

        if (gameOver == 1)
        {
            winnerIdx = turn;
        }

        /* Switch to the other player */
        if (turn == 0)
            turn = 1;
        else
            turn = 0;
    }

    return winnerIdx;
}


/* ====================================================
   MAIN
   Entry point of the program.
   ==================================================== */
int main(void)
{
    /* Seed the random number generator with the current time
       so numbers are different every run */
    srand((unsigned int)time(NULL));

    /* Create an array of 2 players */
    Player players[2];

    /* Set default names using strcpy_s (safe version of strcpy) */
    strcpy_s(players[0].name, sizeof(players[0].name), "Player1");
    strcpy_s(players[1].name, sizeof(players[1].name), "Player2");

    /* Set all scores and wins to zero */
    players[0].score = 0;
    players[0].totalWins = 0;
    players[1].score = 0;
    players[1].totalWins = 0;

    /* Show title screen */
    printf("\n");
    printf("  ==========================================\n");
    printf("   BIT-MAZE TREASURE HUNT\n");
    printf("   First player to reach %d points wins!\n", WIN_SCORE);
    printf("  ==========================================\n");

    /* Check password before anything else */
    if (checkPassword() == 0)
    {
        return 1;   /* exit the program if password wrong */
    }

    /* Ask for player names - only once, names stay for all games */
    setupPlayers(&players[0], &players[1]);

    /* ---- PLAY AGAIN LOOP ---- */
    char again[4];

    do
    {
        /* Play one full game and get the result */
        int winner = playOneGame(players);

        /* Add a win to the winner's total (not for a draw) */
        if (winner >= 0)
        {
            players[winner].totalWins = players[winner].totalWins + 1;
        }

        /* Show the final board */
        printf("\n\n\n");
        printGrid(&players[0], &players[1], 0);

        /* Announce the result */
        printf("  ==========================================\n");

        if (winner == -1)
        {
            printf("   IT IS A DRAW!\n");
            printf("   %s: %d pts   vs   %s: %d pts\n",
                players[0].name, players[0].score,
                players[1].name, players[1].score);
        }
        else
        {
            printf("   WINNER: %s  with %d points!\n",
                players[winner].name, players[winner].score);
        }

        printf("  ==========================================\n");

        /* Show scoreboard */
        printf("\n  --- SCOREBOARD ---\n");
        printf("  %-15s   score = %-4d   total wins = %d\n",
            players[0].name, players[0].score, players[0].totalWins);
        printf("  %-15s   score = %-4d   total wins = %d\n",
            players[1].name, players[1].score, players[1].totalWins);

        /* Ask if they want to play again */
        printf("\n  Play again? (y/n): ");
        scanf_s("%3s", again, (unsigned)sizeof(again));

    } while (again[0] == 'y' || again[0] == 'Y');

    /* Final goodbye message */
    printf("\n  Thanks for playing!\n");
    printf("  Final result:  %s: %d win(s)   |   %s: %d win(s)\n\n",
        players[0].name, players[0].totalWins,
        players[1].name, players[1].totalWins);

    return 0;
}
