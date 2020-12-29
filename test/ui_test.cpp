//
// Created by sheep on 2020/12/28.
//

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <cstring>
#include <ctime>
#include <thread>
#include "UI.hpp"
#include "DB.h"

char *buffer;

WINDOW *input;
WINDOW *display;
WINDOW *bar;

int display_row;

bool exit_flag = false;

std::string curr_user_name;

std::thread bar_thread;

std::string GetTime() {
    time_t tt = time(NULL);
    return std::string(ctime(&tt));
}

void exitApp() {
    DB &db = DB::getInstance();
    db.close();

    free(buffer);

    exit_flag = true;
    bar_thread.join();
    delwin(bar);
    delwin(input);
    delwin(display);
    endwin();
}

void get_input() {
    mvwgetnstr(input, 1, 3, buffer, 60);
}

void clear_scr() {
    wclear(input);
    wclear(display);
}

void draw_bar() {
    UI::mvwaddstring(bar, 1, 3, GetTime());
    UI::mvwaddstring(bar, 1, 50, curr_user_name);
    box(bar, 0, 0);
}

void bar_stuff() {
    while(1) {
        wclear(bar);
        draw_bar();
        wrefresh(bar);
        sleep(1);
        if (exit_flag) break;
    }
}

void draw_input() {
    box(input, 0, 0);
}

void draw() {
    draw_input();
}

void refresh_scr() {
    wrefresh(display);
    wrefresh(input);
}

void init() {
    DB &db = DB::getInstance();
    db.open();

    buffer = (char*) malloc(sizeof(char) * 60);
    display_row = 1;
    curr_user_name = "not login";

    initscr();
    keypad(stdscr, TRUE);
    halfdelay(10);
    input = newwin(3, 80, 21, 0);
    display = newwin(18, 80, 3, 0);
    bar = newwin(3, 80, 0, 0);

    curs_set(0);

    bar_thread = std::thread(bar_stuff);
    clear_scr();
    draw();
    refresh_scr();
}

void scanning_line(int y) {
    for (int i = 0; i < 24; i++) {
        mvaddch(i, y, '#');
    }
}
void init_animation() {
    char arr[24][80];
    memset(arr, 0, sizeof(arr));
    strcpy(arr[7],  "             #    #    ######    #      #       ######                     ");
    strcpy(arr[8],  "             #    #    #         #      #      #      #                    ");
    strcpy(arr[9],  "             #    #    #         #      #      #      #                    ");
    strcpy(arr[10], "             #    #    #         #      #      #      #                    ");
    strcpy(arr[11], "             ######    ######    #      #      #      #                    ");
    strcpy(arr[12], "             #    #    #         #      #      #      #                    ");
    strcpy(arr[13], "             #    #    #         #      #      #      #                    ");
    strcpy(arr[14], "             #    #    #         #      #      #      #                    ");
    strcpy(arr[15], "             #    #    #         #      #      #      #                    ");
    strcpy(arr[16], "             #    #    #######   ####   ####    ######                     ");
    initscr();
    for (int i = 0; i < 80; i++) {
        scanning_line(i);
        refresh();
        for (int j = 0; j < 24; j++) {
            if (arr[j][i] != '#') mvdelch(j, i);
        }
        usleep(30000);
    }
    sleep(2);
    clear();
    endwin();
}

int main() {
    // make sure your terminal size is (24,80)
    init_animation();
    init();
    while (1) {
        clear_scr();
        draw();
        get_input();
        if (strcmp(buffer, "quit") == 0) break;

        refresh_scr();
    }

    exitApp();
    return 0;
}