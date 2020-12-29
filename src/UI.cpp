//
// Created by sheep on 2020/12/29.
//

#include <ncurses.h>
#include <string>

namespace UI {
    void mvwaddstring(WINDOW *win, int x, int y, std::string str) {
        mvwaddstr(win, x, y, str.c_str());
    }
    void mvwaddint(WINDOW *win, int x, int y, int value) {
        char *buffer = new char[80];
        sprintf(buffer, "%d", value);
        mvwaddstr(win, x, y, buffer);
        delete[] buffer;
    }
}
