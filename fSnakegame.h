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
	bool bEatsFruit, bEatsPoison, bEatsGate;
	CharPosition fruit;
	CharPosition poison;
	vector<CharPosition> Gate;
	vector<CharPosition> snake;

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
	~fSnakeGame();
	void PlayGame();
};

#endif
