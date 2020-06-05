/*
This is the console executable, that makes use of the fSnakeGame class.
This is handling all user interaction. For game logic, please see fSnakeGame.h.
*/

#include "fSnakeGame.h"
#include <ncurses.h>
// Unreal standards
using int32 = int;

// int32 maxheight, maxwidth;
void PlayGame();
int32 IsUserReady();
int32 AskUserToPlayAgain();
void ClearCentre();
int32 UserInput();

int32 main()
{
	WINDOW *win_start;

	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_WHITE);
	if (IsUserReady() == 'y') // wait for confirmation of the user
		do
		{
			{
				fSnakeGame NewSnake;
				NewSnake.PlayGame();
			}
		} while (AskUserToPlayAgain() == 'y');
	return 0;
}

// clear the screen and centre the cursor
void ClearCentre(float x, float y)
{
	clear(); // clear the screen if the game is played for the 2nd time
	initscr();
	noecho();
	// getmaxyx(stdscr, maxheight, maxwidth);
	move((21 / y), (21 / x));
}

// receive user confirmation
int32 UserInput()
{
	int32 UserInput = getch();
	refresh();
	endwin();
	clear();

	return UserInput;
}

// print start menu
int32 IsUserReady()
{
	attron(A_BOLD);
	attron(COLOR_PAIR(1));
	refresh();
	ClearCentre(3, 2.5);
	printw("Welcome to the Snake Game.");
	printw("Are you ready? (y/n)");
	return UserInput();
}

// print end of the game menu and ask user to play again
int32 AskUserToPlayAgain()
{
	init_pair(1, COLOR_RED, COLOR_WHITE);
	attron(COLOR_PAIR(1));
	refresh();
	ClearCentre(2.5, 2.5);
	printw("Do you want to play again? (y/n)");
	return UserInput();
}
