//
// Created by shrimp on 2020/12/27.
//

#include "UserRegister.h"
#include <string>
#include <regex>

UserRegister::UserRegister() {
    DB& db = DB::getInstance();

    db.open();
    all_user_data = db.select_all_user_data();
    db.close();
}

void UserRegister::Register() {
    register_account();
    register_password();

    register_user_name();
    register_real_name();

    register_phone_number();
    register_email();
    register_id_number();
}

void UserRegister::register_account() {
    std::string account;

    while (true) {
        std::cout << "input your account: " << std::endl;

        std::cin >> account;

        bool check = false;
        for (auto it : all_user_data) {
            if (it.account == account) {
                check = true;
                break;
            }
        }
        if (check) continue;
        break;
    }

    user.account = account;
}

void UserRegister::register_password() {
    std::string password;
    std::string confirm_password;

    while (true) {
        std::cout << "input your password: ";

        std::cin >> password;

        // password rule
        if (password.size() <= 6) {
            continue;
        }

        std::cout << "confirm your password: ";

        std::cin >> confirm_password;

        if (password != confirm_password) {
            continue;
        }

        break;
    }

    user.password = password;
}

void UserRegister::register_user_name() {
    std::string user_name;

    while (true) {
        std::cout << "user_name" << std::endl;
        std::cin >> user_name;

        for (auto it : all_user_data) {
            if (it.user_name == user_name) {
                std::cout << "check user_name" << std::endl;
                continue;
            }
        }

        break;
    }

    user.user_name = user_name;
}

void UserRegister::register_real_name() {
    std::string real_name;

    std::cout << "real_name" << std::endl;
    std::cin >> real_name;

    user.real_name = real_name;
}

void UserRegister::register_phone_number() {
    std::string phone_number;
//    std::regex phone_regex("^1(3\\d|47|5([0-3]|[5-9])|8(0|2|[5-9]))\\d{8}$");
    std::regex phone_regex("(.*)");
    while (true) {
        std::cout << "input your phone number: ";

        std::cin >> phone_number;

        if (!std::regex_match(phone_number, phone_regex)) {
            std::cout << "phone number is illegal!" << std::endl;
            continue;
        }

        break;
    }

    user.phone_number = phone_number;
}

void UserRegister::register_email() {
    std::string email;
    // only support qq email and google email
    std::regex email_regex("(.*)(@qq.com|@gmail.com)");

    while (true) {
        std::cout << "email" << std::endl;
        std::cin >> email;

        if (std::regex_match(email, email_regex))
            break;
        else
            std::cout << "check email" << std::endl;
    }

    user.email = email;
}

void UserRegister::register_id_number() {
//    std::regex id_number_regex("^([1-9]\\d{5}[12]\\d{3}(0[1-9]|1[012])(0[1-9]|[12][0-9]|3[01])\\d{3}[0-9xX])$");
    std::regex id_number_regex("(.*)");
    std::string id_number;

    while (true) {
        std::cout << "input your idCard number: ";

        std::cin >> id_number;

        if (!std::regex_match(id_number, id_number_regex)) {
            std::cout << "idCard is illegal!" << std::endl;
            continue;
        }

        break;
    }

    user.id_number = id_number;
}