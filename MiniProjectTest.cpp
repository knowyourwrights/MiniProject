// Jesse
// 16/03/2026
// MiniProject
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

int main()
{
	int reset = 0, playerCountIGNORE = 0;
	char password[20] = "12345", userinput[20], Player1Name[20], Player2Name[20];

	printf("Welcome to the game!!\nPlease enter the password: ");
	gets_s(userinput, 20);

	if (strcmp(password, userinput) == 0)
	{
		printf("Correct! Welcome in.");
		printf("\n------------------------------------------------------------------------\n");

		printf("Enter Player 1 name (20 char MAX): ");
		gets_s(Player1Name, 20);

		printf("Enter Player 2 name (20 char MAX): ");
		gets_s(Player2Name, 20);

		// start of game below vv

		while (reset == 0)
		{
			srand((int)time(0));

			int row, col, x, y, userGuess, goldCount = 0, goldCount2 = 0, bombCount = 0, bombCount2 = 0;

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
				int currentPlayer = playerCountIGNORE % 2;
				// these are commented out for gameplay reasons
				/*
				//setting item type & printing
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
				}*/

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

				printf("--------------------------------\n");

				if (currentPlayer == 0)
				{
					printf("Total gold found: %i\n", goldCount);
					printf("Total bombs found: %i\n", bombCount);
				}
				else
				{
					printf("Total gold found: %i\n", goldCount2);
					printf("Total bombs found: %i\n", bombCount);
				}
				
				
				printf("--------------------------------\n");

				if (currentPlayer == 0)
				{
					if (Player1Name[strlen(Player1Name) - 1] == 's')
					{
						printf("%s' turn!\n", Player1Name);
						printf("----------------\n");
					}
					else
					{
						printf("%s's turn!\n", Player1Name);
						printf("----------------\n");

					}
					playerCountIGNORE++;
				}
				else
				{
					if (Player2Name[strlen(Player2Name) - 1] == 's')
					{
						printf("%s' turn!\n", Player2Name);
					}
					else
					{
						printf("%s's turn!\n", Player2Name);
					}
					playerCountIGNORE++;
				}
				int err = 0;
				//check if x or y is out of range
				while (err == 0)
				{
					do
					{
						printf("\nChoose a position 0-5 (x, ROW): ");
						scanf_s("%d", &x);

						printf("\nChoose a position 0-5 (y, COL): ");
						scanf_s("%d", &y);

						if (x > 5 || x < 0 || y > 5 || y < 0)
						{
							printf("Out of range, try again\n");
						}
					} while ((x < 0 || x > 5) || (y < 0 || y > 5));

					err = board[x][y];

					if (err != 0)
					{
						printf("Space already chosen.\n");
						err = 0;
					}
					else
					{
						err++;
					}
				} 


				userGuess = realBoard[x][y];


				//checking 8bit value of guess
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
					if (currentPlayer == 0)
					{
						printf("Bomb found.\n\n");
						bombCount++;

						board[x][y] = 1;
						if (goldCount > 0)
						{
							goldCount -= 1;
						}
					}
					else
					{
						printf("Bomb found.\n\n");
						bombCount2++;

						board[x][y] = 1;
						if (goldCount2 > 0)
						{
							goldCount2 -= 1;
						}
					}
				}

				else if ((((userGuess >> 1) & 1) == 1) && (((userGuess >> 0) & 1) == 1))
				{
					if (currentPlayer == 0)
					{
						printf("Gold found.\n\n");
						board[x][y] = 2;

						gold = (realBoard[x][y] >> 4);

						printf("%i\n\n", gold);
						goldCount += gold;
					}
					else
					{
						printf("Gold found.\n\n");
						board[x][y] = 2;

						gold = (realBoard[x][y] >> 4);

						printf("%i\n\n", gold);
						goldCount2 += gold;
					}
				}

				else
				{
					printf("There was nothing inside.\n\n");
					board[x][y] = 3;
				}

				if (goldCount == 50)
				{
					printf("WOOOOOOOOOOOOOOOOOO player 1 win ^_^\nDo you want to play again? (Y/N): ");
					scanf_s("%c ", &gameActive);

					if (gameActive == 'y' || gameActive == 'Y')
					{
						reset = 0;
					}
					else
					{
						reset = 1;
					}
				}
				else if (goldCount2 == 50)
				{
					printf("WOOOOOOOOOOOOOOOOOO player 2 win ^_^\nDo you want to play again? (Y/N): ");
					scanf_s("%c ", &gameActive);

					if (gameActive == 'y' || gameActive == 'Y')
					{
						reset = 0;
					}
					else
					{
						reset = 1;
					}
				}

			}
			if (i == 36)
			{
				printf("Nobody won! Would you like to try again? (Y/N): ");
				scanf_s("%c ", &gameActive);

				if (gameActive == 'y' || gameActive == 'Y')
				{
					reset = 0;
				}
				else
				{
					reset = 1;
			}

			//printf("%i", userGuess);
		}
	}
	else
	{
		printf("NOaUEWHGJD WRONG,. wrongnggg try again (one attempt): ");
		gets_s(userinput, 20);
	}
	return(0);
}