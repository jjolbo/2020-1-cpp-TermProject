#include <ncurses.h>
#include <iostream>
int main()
{
  initscr();
  start_color();
  // 새로운 윈도우 생성
  // WINDOW *win;
  // win = newwin(10, 10, 10, 10);
  // wrefresh(win);
  // delwin(win);
  keypad(stdscr, true);
  // nodelay(stdscr, true); // 무언가를 누르면 읽을 수는 있는데 기다리지 않음.
  usleep(100000); // 자고 싶다면... 그리고 자동 종료됨.
  // refresh();
  // getch();
  endwin();
  return 0;
}

// int x, y;
//   getmaxyx(stdscr, y, x);
//   move(y / 2, x / 2);
//   // highlight: A_REVERSE, bold: A_BOLD
//   attron(A_BOLD);
//   printw("a");
//   attron(A_BOLD);
//   refresh();
//   getch(); // The getch, wgetch, mvgetch and mvwgetch, routines read a character from the window.