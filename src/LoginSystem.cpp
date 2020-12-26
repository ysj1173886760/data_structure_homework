//
// Created by shrimp on 2020/12/26.
//

#include "LoginSystem.h"

#include <string>

LoginSystem::LoginSystem() {
    DB& db = DB::getInstance();

    db.open();
    user_data = db.select_all_user_data();
    db.close();

    std::cout << "user_data.size: " << user_data.size() << std::endl;
}

//
void LoginSystem::Login() {
    std::string account;
    std::string password;

    while (true) {
        std::cout << "Account: ";
        std::cin >> account;

        std::cout << "Password: ";
        std::cin >> password;

        bool check = false;
        for (auto it : user_data) {
            if (it.account == account && it.password == password) {
                check = true;
                std::cout << "successful login" << std::endl;
                break;
            }
        }

        if (check) break;

        std::cout << "please check your account or password!" << std::endl;
    }
}

//
void LoginSystem::ChangePassword() {

}

// TODO: recover user's password through email
void LoginSystem::RecoverPassword() {
    std::string account;
    std::string email;
    std::string password;

    while (true) {
        std::cout << "please input your account" << std::endl;
        std::cin >> account;

        bool check = false;
        for (auto it : user_data) {
            if (it.account == account) {
                std::cout << "please input your email" << std::endl;
                std::cin >> email;

                if (it.email == email) {
                    it.password = SetPassword();
                    check = true;
                    break;
                }
            }
        }

        if (check) break;

        std::cout << "check your account or email" << std::endl;
    }

}

// for set password and confirm password
std::string LoginSystem::SetPassword() {
    std::string password;
    std::string confirm_password;

    while (true) {
        std::cout << "password: " << std::endl;
        std::cin >> password;

        std::cout << "confirm password: " << std::endl;
        std::cin >> confirm_password;

        if (password == confirm_password)
            break;

        std::cout << "password != confirm password!" << std::endl;
    }
    return password;
}