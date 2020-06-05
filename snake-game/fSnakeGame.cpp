#include "fSnakeGame.h"
#include <unistd.h>
#include <ncurses.h>
#include <fstream>
#include <iostream>

using namespace std;

// Unreal coding standards
using int32 = int;

CharPosition::CharPosition(int32 col, int32 row)
{
	x = col;
	y = row;
}

CharPosition::CharPosition()
{
	x = 0;
	y = 0;
}

fSnakeGame::fSnakeGame()
{
	// variables initialisation:
	partchar = 'x';				// character to represent the snake
	edgechar = (char)219; // full rectangle on the key table
	fruitchar = '*';
	poisonchar = '-';
	fruit.x = 0;
	fruit.y = 0;
	poison.x = 0;
	poison.y = 0;
	score = 0;
	del = 110000;
	snake_length = 3;
	fruit_cnt = 0;
	poison_cnt = 0;
	gate_cnt = 0;
	max_snake = 3;
	bool bEatsFruit = 0;
	bool bEatsPoison = 0;
	direction = 'l';
	srand(time(NULL));

	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLACK); // snake color
	init_pair(2, COLOR_GREEN, COLOR_BLACK);	 // fruit color
	init_pair(3, COLOR_RED, COLOR_BLACK);		 // poison color
	init_pair(4, COLOR_WHITE, COLOR_RED);		 //
	init_pair(5, COLOR_RED, COLOR_WHITE);		 // score board border color
	init_pair(6, COLOR_WHITE, COLOR_BLACK);	 // score board contents color

	InitGameWindow();
	PositionFruit();
	PositionPoison();
	DrawWindow();
	DrawSnake();
	PrintScore();
	refresh();
}

fSnakeGame::~fSnakeGame()
{
	nodelay(stdscr, false);
	getch();
	endwin();
}

// initialise the game window
void fSnakeGame::InitGameWindow()
{
	initscr(); // initialise the screen
	nodelay(stdscr, TRUE);
	keypad(stdscr, true); // initialise the keyboard: we can use arrows for directions
	noecho();							// user input is not displayed on the screen
	curs_set(0);					// cursor symbol is not not displayed on the screen (Linux)
	return;
}

void fSnakeGame::getStrMap()
{

	ifstream inStream;
	inStream.open("./map.txt");
	if (inStream.fail())
	{
		cerr << "Input file opening failed.\n";
		exit(1);
	}

	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			inStream >> map_arr[i][j];
		}
	}
	return;
}

// draw the game window
void fSnakeGame::DrawWindow()
{
	// game_map = newwin(21, 21, 0, 0);
	getStrMap();
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			if (map_arr[i][j] == 1 || map_arr[i][j] == 2)
			{
				move(j, i + 4);
				addch(edgechar);
			}
		}
	}
	return;
}

// draw snake's body
void fSnakeGame::DrawSnake()
{
	for (int32 i = 0; i < 3; i++)
	{
		snake.push_back(CharPosition(21 / 2 + i, 21 / 2));
	}

	for (int32 i = 0; i < snake.size(); i++)
	{
		attron(COLOR_PAIR(1));
		move(snake[i].y, snake[i].x);
		addch(partchar);
	}
	return;
}

// score board
void fSnakeGame::PrintBorder()
{
	// Draw Border
	// top
	attron(COLOR_PAIR(5));

	for (int i = 0; i < 26; i++)
	{
		move(0, 30 + i);
		addch('#');
	}
	//bottom
	for (int i = 0; i < 26; i++)
	{
		move(15, 30 + i);
		addch('#');
	}
	// left
	for (int i = 0; i < 17 - 1; i++)
	{
		move(i, 30);
		addch('#');
	}
	// right
	for (int i = 0; i < 17 - 1; i++)
	{
		move(i, 55);
		addch('#');
	}
}

// print score at bottom of window
void fSnakeGame::PrintScore()
{
	PrintBorder();
	attron(COLOR_PAIR(6));
	move(1, 33);
	printw("<Score board>");

	move(3, 33);
	printw("B: %d", snake_length / max_snake);

	move(5, 33);
	printw("+: %d", fruit_cnt);

	move(7, 33);
	printw("-: %d", poison_cnt);

	move(9, 33);
	printw("G: %d", gate_cnt);

	move(11, 33);
	printw("time: ");

	return;
}

// position a new fruit in the game window
void fSnakeGame::PositionFruit()
{
	while (1)
	{
		int32 tmpx = rand() % 21 + 6; // +1 to avoid the 0
		int32 tmpy = rand() % 21 + 6;

		// check that the fruit is not positioned on the snake
		for (int32 i = 0; i < snake.size(); i++)
		{
			if (snake[i].x == tmpx && snake[i].y == tmpy)
			{
				continue; // if true, ignore the following and go back to the beginning of function
			}
		}

		// check that the fruit is positioned within the game window
		if (tmpx >= 21 - 2 || tmpy >= 21 - 3)
		{
			continue; // if true, ignore the following and go back to the beginning of function
		}

		// if the coordinates are valid, add fruit in the window
		fruit.x = tmpx;
		fruit.y = tmpy;
		break;
	}

	attron(COLOR_PAIR(2));
	move(fruit.y, fruit.x);
	addch(fruitchar);
	refresh();
}

void fSnakeGame::PositionPoison()
{
	while (1)
	{
		int32 tmpx = rand() % 21 + 4; // +1 to avoid the 0
		int32 tmpy = rand() % 21 + 4;

		// check that the fruit is not positioned on the snake
		for (int32 i = 0; i < snake.size(); i++)
		{
			if (snake[i].x == tmpx && snake[i].y == tmpy)
			{
				continue; // if true, ignore the following and go back to the beginning of function
			}
		}

		// check that the fruit is positioned within the game window
		if (tmpx >= 21 - 2 || tmpy >= 21 - 3)
		{
			continue; // if true, ignore the following and go back to the beginning of function
		}

		// if the coordinates are valid, add fruit in the window
		poison.x = tmpx;
		poison.y = tmpy;
		break;
	}
	attron(COLOR_PAIR(3));
	move(poison.y, poison.x);
	addch(poisonchar);
	refresh();
}

// set game over situations
bool fSnakeGame::FatalCollision()
{
	// if the snake hits the edge of the window
	if (snake[0].x == 4 || snake[0].x == 21 + 3 || snake[0].y == 0 || snake[0].y == 21 - 1)
	{
		return true;
	}

	// if the snake collides into himself
	for (int32 i = 2; i < snake.size(); i++)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
		{
			return true;
		}
	}

	return false;
}

// define behaviour when snake eats the fruit
bool fSnakeGame::GetsFruit()
{
	if (snake[0].x == fruit.x && snake[0].y == fruit.y)
	{
		PositionFruit();
		score += 1;
		fruit_cnt++;
		max_snake++;
		PrintScore();

		// if score is a multiple of 100, increase snake speed
		if ((score % 100) == 0)
		{
			del -= 1000;
		}
		snake_length++;
		return bEatsFruit = true;
	}
	else
	{
		return bEatsFruit = false;
	}
	return bEatsFruit;
}

bool fSnakeGame::GetsPoison()
{
	if (snake[0].x == poison.x && snake[0].y == poison.y)
	{
		PositionPoison();
		score -= 1;
		poison_cnt++;

		PrintScore();

		// if score is a multiple of 100, increase snake speed
		if ((score % 100) == 0)
		{
			del += 1000;
		}
		snake_length--;
		return bEatsPoison = true;
	}
	else
	{
		return bEatsPoison = false;
	}
	return bEatsPoison;
}

// define snake's movements
void fSnakeGame::MoveSnake()
{
	int32 KeyPressed = getch();
	switch (KeyPressed)
	{
	case KEY_LEFT:
		if (direction != 'r')
		{
			direction = 'l';
		}
		else
			snake_length = -1;
		break;
	case KEY_RIGHT:
		if (direction != 'l')
		{
			direction = 'r';
		}
		else
			snake_length = -1;
		break;
	case KEY_UP:
		if (direction != 'd')
		{
			direction = 'u';
		}
		else
			snake_length = -1;
		break;
	case KEY_DOWN:
		if (direction != 'u')
		{
			direction = 'd';
		}
		else
			snake_length = -1;
		break;
	case KEY_BACKSPACE:
		direction = 'q'; // key to quit the game
		break;
	}

	// the snake doesn't eat fruit, remains same size
	if (!bEatsFruit)
	{
		attron(COLOR_PAIR(1));
		move(snake[snake.size() - 1].y, snake[snake.size() - 1].x); // moves at the end of the tail
		printw(" ");																								// add empty ch to remove last character
		refresh();
		snake.pop_back(); // removes the last element in the vector, reducing the container size by one
	}

	// the snake moves and we add an extra character at the beginning of the vector
	// add a head and initialise new coordinates for CharPosition according to the direction input
	if (direction == 'l')
	{
		snake.insert(snake.begin(), CharPosition(snake[0].x - 1, snake[0].y));
	}
	else if (direction == 'r')
	{
		snake.insert(snake.begin(), CharPosition(snake[0].x + 1, snake[0].y));
	}
	else if (direction == 'u')
	{
		snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y - 1));
	}
	else if (direction == 'd')
	{
		snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y + 1));
	}

	// move to the new CharPosition coordinates
	move(snake[0].y, snake[0].x);

	if (bEatsPoison)
	{
		// TODO: 뱀 꼬리 하나 줄여야되는데 이 부분에 구현해야함.
		addch(' ');
		refresh();
		return;
	}

	addch(partchar); // add a new head
	refresh();
	return;
}

void fSnakeGame::PlayGame()
{
	while (1)
	{
		if (FatalCollision() || snake_length < 3)
		{
			move((21 - 2) / 2, (21 - 5) / 2);
			attron(COLOR_PAIR(4));
			refresh();
			printw("!!!GAME OVER!!!");
			break;
		}

		GetsFruit();
		GetsPoison();
		MoveSnake();

		if (direction == 'q') //exit
		{
			break;
		}

		usleep(del); // delay
	}
}
