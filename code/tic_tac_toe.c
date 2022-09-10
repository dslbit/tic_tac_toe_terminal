// TODOs:
// compile with -W4 -WX and clean for cl, gcc and clang
// --
// Do compiler checks (Symbolic constants, search for erros and warning)
// Figure out a better way to wrap the "system("cls");" instruction to work both on Windows and Linux
// Should I replace (stdio.h / stdlib.h / math.h) printf, sscanf, strlen, gets, getchar, etc. ?
// Remove CRuntime
// should I make this c89-cpp compatible?

// DESIGN IDEAS:
// get a sense of time to animate the board of the winner with its plays traced (least important)
// can we do colors? (least important)

#define GLOBAL static

#define TRUE (1)
#define FALSE (0)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define GAME_BOARD_MAX_ROWS (3)
#define GAME_BOARD_MAX_COLUMNS (3)
#define MAX_AVAILABLE_GAME_BOARD_POSITIONS (GAME_BOARD_MAX_ROWS * GAME_BOARD_MAX_COLUMNS + 1)

GLOBAL int global_running = TRUE;

int main(void)
{
	int i, j;
	int end_of_game, ended_in_a_draw;
	char game_board[GAME_BOARD_MAX_ROWS][GAME_BOARD_MAX_COLUMNS];
	int game_board_moves;
	char winner;
	char terminal_input[256];
	int row_input, column_input;

	// variables default value
	end_of_game = ended_in_a_draw = FALSE;
	winner = 0;
	game_board_moves = 0;
	row_input = column_input = 0;

	// setting up the random number seed based on the local user time
	time_t current_local_time = time(0);
	current_local_time *= 60 * 60;
	srand(current_local_time);

	// setup game_board default values
	{
		int count = 1;

		for(i = 0; i < GAME_BOARD_MAX_ROWS; ++i)
		{
			for(j = 0; j < GAME_BOARD_MAX_COLUMNS; ++j)
			{
				game_board[i][j] = '0' + count;
				++count;
			}
		}
	}

	// program presentation "menu"
	{
		printf("------------------\n");
		printf("   TIC TAC TOE    \n");
		printf("  By: Douglas L.  \n");
		printf("------------------\n\n");
		printf("NOTE: The game board is a grid 3x3, so the first position\n is 1,1 and the last one is 3,3. You start playing as X.\n\nHave fun!\n\n");
		printf("> Press ENTER to start playing.");

		// NOTE(Douglas): here, this loop is necessary, otherwise stdin will fuck up the
		//  input down there, the logic won't fail but it will show error messages about
		//  the syntax to the user, and that shouldn't happen.
		while( getchar() != '\n' );

		system("cls");
	}

	// game main loop
	do
	{
		system("cls");

		// prints out the game board to the terminal 
		{
			printf("\n");
			for(i = 0; i < GAME_BOARD_MAX_ROWS; ++i)
			{
				for(j = 0; j < GAME_BOARD_MAX_COLUMNS; ++j)
				{
					if(game_board[i][j] == 'X' || game_board[i][j] == 'O')
					{
						printf("| %c ", game_board[i][j]);
					}
					else
					{
						printf("| %c ", ' ');
					}
				}
				printf("|\n\n");
			}
		}

		// check for a winner (useful here because the way the code is structured)
		if(end_of_game)
		{
			global_running = FALSE;

			if(ended_in_a_draw)
			{
				printf("\nThe game ended in a draw!\nGame Over!\n\n");
			}
			else
			{
				printf("\nWinner: %c\n\n", winner);
			}

			printf("> Thanks for playing the game!\nPress ENTER to close the program.");
			getchar();

			break;
		}

		// process the user input
		{
			int valid_input, input_result, input_length;
		
			valid_input = FALSE;

			while(valid_input == FALSE)
			{
				input_result = EOF;
				row_input = column_input = EOF;
				input_length = 0;

				// NOTE(Douglas): if "terminal_input" is too small for the user input, then things could get
				// ugly, because "gets(...)" doesn't consider "terminal_input" length. It can easly write
				// into another memory address space.
				printf("\nPlease choose a position on the board (syntax: row,column): ");
				gets(&terminal_input);
				input_length = strlen(terminal_input);
				input_result = sscanf(terminal_input, "%d,%d", &row_input, &column_input);
				
				if(input_result == 0 || input_result == EOF) // checking for "sscanf" errors
				{
					printf("> wrong input syntax, please try again as informed: row,column\n");
				}
				else if( (row_input <= 0 || row_input > GAME_BOARD_MAX_ROWS) ||
				         (column_input <= 0 || column_input > GAME_BOARD_MAX_COLUMNS) ) // checking game board indices
				{
					printf("> wrong index syntax, invalid game board position\n");
				}
				else if(input_length != 3) // checking the game board syntax: x,x
				{
					printf("> wrong input syntax, please try again as informed: row,column\n");
					continue;
				}
				else
				{
					valid_input = TRUE;

					row_input--;
					column_input--;

					if(game_board[row_input][column_input] != 'X' && game_board[row_input][column_input] != 'O')
					{
						game_board[row_input][column_input] = 'X';
						++game_board_moves;
					}
					else
					{
						valid_input = FALSE;
						printf("> position is already occupied, try another one.\n");
					}
				}
			}
		}

		// generate the computer move
		if(game_board_moves < 9)
		{
			char game_board_pos;
			int random_board_position, got_available_position;

			got_available_position = FALSE;

			do
			{
				random_board_position = (int) (((float)rand() / (float)RAND_MAX) * 8.0f + 0.5f);
				game_board_pos = *((char *)game_board + random_board_position);

				if(game_board_pos != 'X' && game_board_pos != 'O') // if it is an available position
				{
					*((char *)game_board + random_board_position) = 'O';
					++game_board_moves;
					got_available_position = TRUE;
				}
			} while(!got_available_position);
		}

		// check for a winner
		{
			// rows check
			if( ((winner = game_board[0][0]) == game_board[1][0] && game_board[1][0] == game_board[2][0]) ||
			    ((winner = game_board[0][1]) == game_board[1][1] && game_board[1][1] == game_board[2][1]) ||
			    ((winner = game_board[0][2]) == game_board[1][2] && game_board[1][2] == game_board[2][2]) )
			{
				end_of_game = TRUE;
			}
			// columns check
			else if( ((winner = game_board[0][0]) == game_board[0][1] && game_board[0][1] == game_board[0][2]) ||
			         ((winner = game_board[1][0]) == game_board[1][1] && game_board[1][1] == game_board[1][2]) ||
			         ((winner = game_board[2][0]) == game_board[2][1] && game_board[2][1] == game_board[2][2]) )
			{
				end_of_game = TRUE;
			}
			// diagonal
			else if( (winner = game_board[0][0]) == game_board[1][1] && game_board[1][1] == game_board[2][2] )
			{
				end_of_game = TRUE;
			}
			// ended in a draw?
			else if(game_board_moves == 9 && !end_of_game)
			{
				end_of_game = TRUE;
				ended_in_a_draw = TRUE;
			}
		}

	} while(global_running);

	return 0;
}
