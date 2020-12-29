//
// Created by shrimp on 2020/12/29.
//

#include "Email.h"
#include <iostream>

int main() {
    Email email;
    int a = email.send_email("1@qq.com", "--title--", "--content--");
    if (a == 0) std::cout << "ok" << std::endl;
    if (a == 1) std::cout << "account" << std::endl;
    if (a == 2) std::cout << "fail" << std::endl;
}