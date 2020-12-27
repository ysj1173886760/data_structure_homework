//
// Created by shrimp on 2020/12/27.
//

#include "LoginUser.h"

int main() {
    LoginUser u;
    while (!u.Login()) {
        std::cout << "x";
    }
}