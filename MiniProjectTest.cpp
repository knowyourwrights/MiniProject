// Jesse
// 16/03/2026
// MiniProject
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{
	int reset = 0;

	while (reset == 0)
	{


		srand((int)time(0));

		int row, col, x, y, userGuess, goldCount = 0;

		char mask = 0x1, gameActive;

		uint8_t r, gold;

		int board[6][6], realBoard[6][6]; // board is for printing, itemStatus is for what type of spot it is, realBoard holds values behind each spot
		int i;

		// itemstatus = 0 not checked, 1 gold, 2 bomb,  3 nothing

		for (row = 0; row <= 5; row++)
		{
			for (col = 0; col <= 5; col++)
			{
				board[row][col] = 0;
			}
		}

		//actual values behind each box
		for (row = 0; row <= 5; row++)
		{
			for (col = 0; col <= 5; col++)
			{
				r = rand() % 255;
				realBoard[row][col] = r;
			}
		}

		for (i = 0; i <= 36; i++)
		{
			//setting item type & printing
			/**/
			for (row = 0; row <= 5; row++)
			{
				for (col = 0; col <= 5; col++)
				{
					printf("%i ", board[row][col]);
				}
				printf("\n");
			}

			printf("\n\n");
			// printing actual values
			for (row = 0; row <= 5; row++)
			{
				for (col = 0; col <= 5; col++)
				{
					printf("%d ", realBoard[row][col]);
					if (col == 5)
					{
						printf("\n");
					}
				}
			}

			printf("\n\n");


			// printing initial board
			for (row = 0; row <= 5; row++)
			{
				for (col = 0; col <= 5; col++)
				{
					if (board[row][col] == 0)
					{
						printf("# ");
					}

					else if (board[row][col] == 1)
					{
						printf("B ");
					}

					else if (board[row][col] == 2)
					{
						printf("G ");
					}

					else if (board[row][col] == 3)
					{
						printf(". ");
					}


				}
				if (col == 5 && row == 3)
				{
					printf(" y");

				}
				printf("\n");
			}
			printf("     x\n\n");

			printf("Total gold found: %i", goldCount);

			do
			{
				do
				{
					printf("\nChoose a position 0-5 (x): ");
					scanf_s("%d", &x);

					printf("\nChoose a position 0-5 (y): ");
					scanf_s("%d", &y);

					if (x > 5 || x < 0 || y > 5 || y < 0)
					{
						printf("Out of range, try again\n");
					}
				} while ((x < 0 || x > 5) || (y < 0 || y > 5));
				printf("Space already chosen.\n");
			} while (board[x][y] != 0);

			userGuess = realBoard[x][y];


			printf("\n");
			for (int j = 7; j >= 0; j--)
			{
				if (userGuess & (1 << j))
				{
					printf("1");
				}
				else
				{
					printf("0");
				}
			}
			printf("\n");

			if (((userGuess >> 0) & 1) == 0)
			{
				printf("Bomb found.\n\n");
				board[x][y] = 1;
				if (goldCount > 0)
				{
					goldCount -= 1;
				}

			}

			else if ((((userGuess >> 1) & 1) == 1) && (((userGuess >> 0) & 1) == 1))
			{
				printf("Gold found.\n\n");
				board[x][y] = 2;
				gold = (realBoard[x][y] >> 4);
				printf("%i\n\n", gold);
				goldCount += gold;
			}

			else
			{
				printf("nothign,,, soray\n\n");
				board[x][y] = 3;
			}

			if (goldCount == 50)
			{
				printf("WOOOOOOOOOOOOOOOOOO do you want to play again? (Y/N): ");
				scanf_s(" %c" &gameActive);

				if (gameActive == 'y' || gameActive == 'Y')
				{
					reset == 0;
				}
				else
				{
					reset == 1;
				}
			}

		}
		//printf("%i", userGuess);
	}
	return(0);
}