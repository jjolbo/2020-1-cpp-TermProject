#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>

struct snakepart
{
  int x, y;
  snakepart(int col, int row);
  snakepart();
};

class snakeclass
{
  int points, del, maxwidth, maxheight;
  char direction, partchar, oldalchar, foo;
  bool get; // get the foo
  snakepart food;
  std::vector<snakepart> snake;

  void putfood();
  bool collision();
  void movesnake();

public:
  snakeclass();
  ~snakeclass();
  void start(); // 명시적으로 클래스 외부에 있으므로 전체 뱀 게임 저장
};

#endif