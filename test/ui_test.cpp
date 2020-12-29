//
// Created by sheep on 2020/12/28.
//

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <cstring>

using namespace std;

int main() {
    // make sure your terminal size is (24,80)
    char *buff = (char*)malloc(sizeof(char) * 60);
    initscr();
    keypad(stdscr, TRUE);
    WINDOW *input = newwin(3, 80, 21, 0);
    WINDOW *display = newwin(21, 80, 0, 0);
    int input_row = 0, display_row = 1;
    box(input, 0, 0);
    wrefresh(input);
    wrefresh(display);
    while (1) {
        wclear(input);
        box(input, 0, 0);
        memset(buff, 0, sizeof(char) * 60);
        mvwgetnstr(input, 1, 3, buff, 60);
        mvwaddstr(display, display_row, 3, buff);
        display_row++;
        if (display_row > 21) {
            display_row = 21;
            wmove(display, 1, 0);
            wdeleteln(display);
        }
        if (strcmp(buff, "quit") == 0) break;
        if (strcmp(buff, "clear") == 0) {
            wclear(display);
            display_row = 1;
        }
        wrefresh(display);
        wrefresh(input);
    }
    delwin(input);
    delwin(display);
    endwin();
    return 0;
}