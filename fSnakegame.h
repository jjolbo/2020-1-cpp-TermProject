/*
This is the game logic (no user interaction).
The game is a very simple version of the classic snake game.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <time.h>
#include <climits>

using namespace std;

#ifndef FSNAKEGAME_H
#define FSNAKEGAME_H

// Unreal standards
//using int = int;

struct CharPosition
{
	int x, y;
	CharPosition(int col, int row);
	CharPosition();
};

class fSnakeGame
{
private:
	time_t start, end;
	int map_arr[30][21];
	int score, del;
	double result;
	int snake_length, fruit_cnt, poison_cnt, gate_cnt, max_snake, level;
	char direction, partchar, edgechar, fruitchar, poisonchar, gatechar;
	// partchar is the character representing the snake's body
	// edgechar is the character representing the edge of the game window
	// fruitchar is the character representing the fruit
	// del stands for delay
	bool bEatsFruit, bEatsPoison, bEatsGate;
	CharPosition fruit; // need to clarify this combination
	CharPosition poison;
	vector<CharPosition> Gate; // represent the snake's body
	vector<CharPosition> snake; // represent the snake's body

	void InitGameWindow();
	void DrawWindow();
	void DrawSnake();
	void PrintScore();
	void PrintBorder();
	void PositionFruit();
	void PositionPoison();
	void PositionGate();
	bool FatalCollision();
	void MoveSnake();
	bool GetsFruit();
	bool GetsPoison();
	bool GetsGate();
	void getStrMap();
	void PassGate(int, int);

public:
	fSnakeGame(int _level);
	~fSnakeGame(); // destructor for cleanup and memory deallocation
	void PlayGame();
};

#endif
