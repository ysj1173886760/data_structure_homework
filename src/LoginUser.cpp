//
// Created by shrimp on 2020/12/27.
//

#include "LoginUser.h"
#include <string>

LoginUser::LoginUser() {
    DB& db = DB::getInstance();

    db.open();
    all_user_data = db.select_all_user_data();
    db.close();
}

// if you login successfully, this function will return true;
bool LoginUser::Login() {
    std::string account;
    std::string password;

    std::cout << "account" << std::endl;
    std::cin >> account;
    std::cout << "password" << std::endl;
    std::cin >> password;

    for (auto it : all_user_data) {
        if (it.account == account && it.password == password) {
            return true;
        }
    }
    return false;
}

//
void LoginUser::ChangePassword() {
    std::string password;

    if (Login()) {
        password = OkPassword();
    }

}

void LoginUser::ForgotPassword() {

}

// return a true password.
std::string LoginUser::OkPassword() {
    std::string password;
    std::string confirm_password;

    while (true) {
        std::cout << "password" << std::endl;
        std::cin >> password;

        // password rule should be added.

        std::cout << "confirm_password" << std::endl;
        std::cin >> confirm_password;

        if (password == confirm_password) {
            break;
        }
    }

    return password;
}