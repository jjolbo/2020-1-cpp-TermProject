#include <ncurses.h>
#include <iostream>

enum MODE {
    USER = 0
} _t_mode;

MODE init_game(void) {
    WINDOW *win_start;

    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, COLOR_RED, COLOR_WHITE);

    win_start = newwin(21, 58, 0, 0);
    wattron(win_start, COLOR_PAIR(2));
    wborder(win_start, '#', '#', '#', '#', '#', '#', '#', '#');
    wattroff(win_start, COLOR_PAIR(2));
    wattron(win_start, COLOR_PAIR(2));
    mvwprintw(win_start, 4, 23, "SNAKE GAME");
    mvwprintw(win_start, 11, 22, "PRESS \"s\" KEY");
    mvwprintw(win_start, 13, 21, " TO START GAME! ");
    wattroff(win_start, COLOR_PAIR(2));
    refresh();
    wrefresh(win_start);

    while(1) {
        int inputKey = getch();
        if (inputKey == 's') return USER;
    }
    delwin(win_start);
}
