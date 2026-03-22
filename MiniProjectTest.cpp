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
	srand((int)time(0));

	int row, col, x, y, userGuess, goldCount;

	char mask = 0x1;

	uint8_t r;

	int board[6][6], itemStatus[6][6], realBoard[6][6]; // board is for printing, itemStatus is for what type of spot it is, realBoard holds values behind each spot

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

	//setting item type & printing
	/**/
	for (row = 0; row <= 5; row++)
	{
		for (col = 0; col <= 5; col++)
		{
			if (mask & realBoard[row][col])
			{
				itemStatus[row][col] = 1; // gold
			}
			else
			{
				itemStatus[row][col] = 0;
			}

			mask <<= 1;
			if (mask & realBoard[row][col])
			{
				itemStatus[row][col] = 2; // bomb
			}
			else
			{
				itemStatus[row][col] = 0;
			}

			realBoard[row][col] >>= 4;

			mask = 0x1;
		}
	}

	//printing itemStatus
	for (row = 0; row <= 5; row++)
	{
		for (col = 0; col <= 5; col++)
		{
			printf("%d ", itemStatus[row][col]);
			if (col == 5)
			{
				printf("\n");
			}
		}
	}

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

				if (col == 5 && row == 3)
				{
					printf("y");
			
				}
			}
			
		}
		printf("\n");
	}
	printf("     x");



	printf("\nChoose a position (x): ");
	scanf_s("%d", &x);

	printf("\nChoose a position (y): ");
	scanf_s("%d", &y);

	userGuess = realBoard[x][y];

	//printf("%i", userGuess);

	mask = 0x1;

	if (mask & userGuess)
	{
		printf("\ng");
	}

	else
	{
		printf("\nb");
	}


	return(0);
}

