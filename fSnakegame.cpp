#include "fSnakegame.h"
#include <unistd.h>
#include <ncurses.h>
#include <fstream>
#include <iostream>
#include <time.h>

using namespace std;

// Unreal coding standards
//using int = int;

CharPosition::CharPosition(int col, int row)
{
	x = col;
	y = row;
}

CharPosition::CharPosition()
{
	x = 0;
	y = 0;
}

fSnakeGame::fSnakeGame(int _level)
{
	// variables initialisation:
	start = time(NULL);
	partchar = 'x';				// character to represent the snake
	edgechar = (char)219; // full rectangle on the key table
	fruitchar = '*';
	poisonchar = '-';
	gatechar = '^';
	fruit.x = 0;
	fruit.y = 0;
	poison.x = 0;
	poison.y = 0;
	score = 0;
	del = 150000;
	snake_length = 3;
	fruit_cnt = 0;
	poison_cnt = 0;
	gate_cnt = 0;
	max_snake = 3;
	bool bEatsFruit = 0;
	bool bEatsPoison = 0;
	bool bEatsGate = 0;
	direction = 'l';
	level = _level;
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

string format_string(const std::string fmt, ...)
{
	int size = ((int)fmt.size()) * 2;
	std::string buffer;
	va_list ap;
	while (1)
	{
		buffer.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)buffer.data(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size)
		{
			buffer.resize(n);
			return buffer;
		}
		if (n > -1)
			size = n + 1;
		else
			size *= 2;
	}
	return buffer;
}

void fSnakeGame::getStrMap()
{

	ifstream inStream;
	string line;
	string filename = format_string("./map_%d.txt", level);
	inStream.open(filename.c_str());
	if (inStream.fail())
	{
		cerr << "Input file opening failed.\n";
		exit(1);
	}

	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			inStream >> map_arr[j][i];
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
			if (map_arr[i][j] == 7)
			{
				move(j, i + 4);
				addch(gatechar);
				CharPosition tmpgate(i + 4, j);
				Gate.push_back(tmpgate);
			}
		}
	}
	return;
}

// draw snake's body
void fSnakeGame::DrawSnake()
{
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			if (map_arr[i][j] == 3 || map_arr[i][j] == 4)
			{
				snake.push_back(CharPosition(i, j));
			}
		}
	}

	for (int i = 0; i < snake_length - 1; i++)
	{
		attron(COLOR_PAIR(1));
		move(snake[i].y, snake[i].x);
		printw("%c", partchar);
		// addch(partchar);
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
	printw("Level %c", level + 96);

	move(5, 33);
	printw("B: %d", snake_length);

	move(7, 33);
	printw("+: %d", fruit_cnt);

	move(9, 33);
	printw("-: %d", poison_cnt);

	move(11, 33);
	printw("G: %d", gate_cnt);

	// move(11, 33);
	// printw("time: ", result);

	return;
}

// position a new fruit in the game window
void fSnakeGame::PositionFruit()
{
	while (1)
	{
		int tmpx = rand() % 21 + 6; // +1 to avoid the 0
		int tmpy = rand() % 21 + 6;

		// check that the fruit is not positioned on the snake
		for (int i = 0; i < snake.size(); i++)
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
		int tmpx = rand() % 21 + 6; // +1 to avoid the 0
		int tmpy = rand() % 21 + 6;

		// check that the fruit is not positioned on the snake
		for (int i = 0; i < snake.size(); i++)
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
	if (map_arr[snake[0].x - 4][snake[0].y] == 1)
	{
		return true;
	}

	// if the snake collides into himself
	for (int i = 2; i < snake.size(); i++)
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
		if ((score % 10) == 0)
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
		if ((score % 10) == 0)
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

bool fSnakeGame::GetsGate()
{
	int tmpx = snake[0].x;
	int tmpy = snake[0].y;

	if (map_arr[tmpx - 4][tmpy] == 7)
	{
		gate_cnt++;

		PrintScore();

		// if score is a multiple of 100, increase snake speed
		if ((score % 100) == 0)
		{
			del += 1000;
		}
		return bEatsGate = true;
	}
	else
	{
		return bEatsGate = false;
	}
	return bEatsGate;
}

void fSnakeGame::PassGate(int gate_x, int gate_y)
{
	// The Gate is on the side wall
	if (gate_x == 24)
	{
		direction = 'l';
		snake.insert(snake.begin(), CharPosition(gate_x - 1, gate_y));
	}
	else if (gate_x == 4)
	{
		direction = 'r';
		snake.insert(snake.begin(), CharPosition(gate_x + 1, gate_y));
	}
	else if (gate_y == 0)
	{
		direction = 'd';
		snake.insert(snake.begin(), CharPosition(gate_x, gate_y + 1));
	}
	else if (gate_y == 20)
	{
		direction = 'u';
		snake.insert(snake.begin(), CharPosition(gate_x, gate_y - 1));
	}
	// The Gate is on the inside wall, going foward direction is left-right case.
	else if (map_arr[gate_x - 4][gate_y - 1] == 1 || map_arr[gate_x - 4][gate_y + 1] == 1)
	{
		if (direction == 'l')
		{
			direction = 'l';
			snake.insert(snake.begin(), CharPosition(gate_x - 1, gate_y));
		}
		else if (direction == 'r')
		{
			direction = 'r';
			snake.insert(snake.begin(), CharPosition(gate_x + 1, gate_y));
		}
		else if (direction == 'u')
		{
			direction = 'r';
			snake.insert(snake.begin(), CharPosition(gate_x + 1, gate_y));
		}
		else if (direction == 'd')
		{
			direction = 'l';
			snake.insert(snake.begin(), CharPosition(gate_x - 1, gate_y));
		}
	}
	// The Gate is on the inside wall, going foward direction is up-down case.
	else if (map_arr[gate_x + 1 - 4][gate_y] == 1 || map_arr[gate_x - 1 - 4][gate_y] == 1)
	{
		if (direction == 'l')
		{
			direction = 'd';
			snake.insert(snake.begin(), CharPosition(gate_x, gate_y + 1));
		}
		else if (direction == 'r')
		{
			direction = 'u';
			snake.insert(snake.begin(), CharPosition(gate_x, gate_y - 1));
		}
		else if (direction == 'u')
		{
			direction = 'u';
			snake.insert(snake.begin(), CharPosition(gate_x, gate_y - 1));
		}
		else if (direction == 'd')
		{
			direction = 'd';
			snake.insert(snake.begin(), CharPosition(gate_x, gate_y + 1));
		}
	}
	// The Gate is on the inside wall, going foward direction is any direction case.
	else
	{
		if (direction == 'l')
		{
			snake.insert(snake.begin(), CharPosition(gate_x - 1, gate_y));
		}
		else if (direction == 'r')
		{
			snake.insert(snake.begin(), CharPosition(gate_x + 1, gate_y));
		}
		else if (direction == 'u')
		{
			snake.insert(snake.begin(), CharPosition(gate_x, gate_y - 1));
		}
		else if (direction == 'd')
		{
			snake.insert(snake.begin(), CharPosition(gate_x, gate_y + 1));
		}
	}
}

// define snake's movements
void fSnakeGame::MoveSnake()
{
	int KeyPressed = getch();
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
	if (bEatsGate)
	{
		if (snake[0].x == Gate[0].x && snake[0].y == Gate[0].y)
		{
			PassGate(Gate[1].x, Gate[1].y);
		}
		else
		{
			PassGate(Gate[0].x, Gate[0].y);
		}
	}
	else if (direction == 'l')
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

	if (bEatsPoison)
	{
		// TODO: 뱀 꼬리 하나 줄여야되는데 이 부분에 구현해야함.

		mvprintw(snake[snake.size() - 1].y, snake[snake.size() - 1].x, " ");
		snake.pop_back();

		for (int i = 0; i < snake_length; i++)
		{
			move(snake[i].y, snake[i].x);
			printw("%c", partchar); // add a new head
		}
		refresh();

		return;
	}

	move(snake[0].y, snake[0].x);
	printw("%c", partchar); // add a new head
	refresh();
	return;
}

void fSnakeGame::PlayGame()
{
	while (1)
	{
		if (FatalCollision() || snake_length < 3 || snake_length == 20)
		{
			end = time(NULL);
			result = double(end - start);
			move((21 - 2) / 2, (21 - 5) / 2);
			attron(COLOR_PAIR(4));
			refresh();
			printw("!!!GAME OVER!!!");
			move((21 - 2) / 2 + 2, (21 - 5) / 2 + 2);
			printw("time: %.2f", result);
			break;
		}

		GetsFruit();
		GetsPoison();
		GetsGate();
		MoveSnake();
		DrawWindow();

		if (direction == 'q') //exit
		{
			break;
		}

		usleep(del); // delay
	}
}