//
// Created by shrimp on 2020/12/27.
//

#include "ManagerRegister.h"
#include <string>
#include <regex>

ManagerRegister::ManagerRegister() {
    DB& db = DB::getInstance();

    db.open();
    all_manager_data = db.select_all_manager_data();
    db.close();
}

void ManagerRegister::Register() {

}

void ManagerRegister::register_account() {
    std::string account;

    while (true) {
        std::cout << "input your account: " << std::endl;

        std::cin >> account;

        bool check = false;
        for (auto it : all_manager_data) {
//            std::cout << "in for" << std::endl;
            if (it.account == account) {
                std::cout << "the account already exists!" << std::endl;
                check = true;
                break;
            }
        }
        if (check)
            continue;

        break;
    }

    manager.account = account;
}

void ManagerRegister::register_password() {
    std::string password;
    std::string confirm_password;
    while (true) {
        std::cout << "input your password: ";

        std::cin >> password;

        // password rule
        if (password.size() <= 6) {
            std::cout << "password is too simple" << std::endl;
            continue;
        }

        std::cout << "confirm your password: ";

        std::cin >> confirm_password;

        if (password != confirm_password) {
            std::cout << "password != confirm_password" << std::endl;
            continue;
        }

        break;
    }

    manager.password = password
}