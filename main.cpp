#include "fSnakegame.h"
#include <ncurses.h>

void PlayGame();
int IsUserReady();
int AskUserToPlayAgain();
void ClearCentre();
int UserInput();
int IsUserLevel();

int main()
{
	WINDOW *win_start;

	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_WHITE);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);

	char inputs[4] = {'a', 'b', 'c', 'd'};
	int n_input;
	if (IsUserReady() == 'y')
		do
		{
			for (int i = 0; i < 4; i++)
			{
				if (IsUserLevel() == inputs[i])
				{
					fSnakeGame NewSnake(i + 1);
					NewSnake.PlayGame();
					break;
				}
			}

		} while (AskUserToPlayAgain() == 'y');
	return 0;
}

void ClearCentre(float x, float y)
{
	clear();
	initscr();
	noecho();
	move((21 / y), (21 / x));
}

int UserInput()
{
	int UserInput = getch();
	refresh();
	endwin();
	clear();

	return UserInput;
}

int IsUserReady()
{
	attron(A_BOLD);
	attron(COLOR_PAIR(1));
	refresh();
	ClearCentre(3, 2.5);
	printw("Welcome to the Snake Game.");
	printw("Are you ready? (y/n)");
	return UserInput();
}

int IsUserLevel()
{
	attron(A_BOLD);
	attron(COLOR_PAIR(2));
	refresh();
	mvprintw(10, 10, "[Game level]");
	mvprintw(11, 10, "Level a: No gate");
	mvprintw(12, 10, "Level b: Gate in the wall");
	mvprintw(13, 10, "Level c: Middle wall in the map");
	mvprintw(14, 10, "Level d: Magic gate");
	mvprintw(16, 10, "Select level:");

	return UserInput();
}

int AskUserToPlayAgain()
{
	init_pair(1, COLOR_RED, COLOR_WHITE);
	attron(COLOR_PAIR(1));
	refresh();
	ClearCentre(2.5, 2.5);
	printw("Do you want to play again? (y/n)");
	return UserInput();
}
