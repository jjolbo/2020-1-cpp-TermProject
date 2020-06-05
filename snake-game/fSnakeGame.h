/*
This is the game logic (no user interaction).
The game is a very simple version of the classic snake game.
*/

#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

#ifndef FSNAKEGAME_H
#define FSNAKEGAME_H

// Unreal standards
using int32 = int;

struct CharPosition
{
	int32 x, y;
	CharPosition(int32 col, int32 row);
	CharPosition();
};

class fSnakeGame
{
private:
	WINDOW *win_score;
	WINDOW *game_map;
	int map_arr[30][21];
	int32 score, del;
	int snake_length, fruit_cnt, poison_cnt, gate_cnt;
	char direction, partchar, edgechar, fruitchar, poisonchar;
	// partchar is the character representing the snake's body
	// edgechar is the character representing the edge of the game window
	// fruitchar is the character representing the fruit
	// del stands for delay
	bool bEatsFruit, bEatsPoison;
	CharPosition fruit; // need to clarify this combination
	CharPosition poison;
	vector<CharPosition> snake; // represent the snake's body

	void InitGameWindow();
	void DrawWindow();
	void DrawSnake();
	void PrintScore();
	void PrintBorder();
	void PositionFruit();
	void PositionPoison();
	bool FatalCollision();
	void MoveSnake();
	bool GetsFruit();
	bool GetsPoison();
	void getStrMap();

public:
	fSnakeGame();
	~fSnakeGame(); // destructor for cleanup and memory deallocation
	void PlayGame();
};

#endif