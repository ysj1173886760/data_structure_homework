//
// Created by sheep on 2020/12/29.
//

#include <ncurses.h>
#include <string>

namespace UI {
    void mvwaddstring(WINDOW *win, int x, int y, const std::string& str) {
        mvwaddnstr(win, x, y, str.c_str(), str.size());
    }

    void mvwaddint(WINDOW *win, int x, int y, int value) {
        char *buffer = new char[80];
        sprintf(buffer, "%d", value);
        mvwaddstr(win, x, y, buffer);
        delete[] buffer;
    }

}
