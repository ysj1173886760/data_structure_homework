//
// Created by shrimp on 2020/12/25.
//

#include <string>
#include <regex>
#include "Register.h"

Register::Register() {
    DB& db = DB::getInstance();
    db.open();
    user_data = db.select_all_user_data();
    std::cout << "Register()" << std::endl;
    std::cout << user_data.size() << std::endl;
    db.close();
}

void Register::register_account(UserData &a) {
    std::string account;

    while (true) {
        std::cout << "input your account: " << std::endl;

        std::cin >> account;

        bool check = false;
        for (auto it : user_data) {
//            std::cout << "in for" << std::endl;
            if (it.account == account) {
                std::cout << "the account already exists!" << std::endl;
                check = true;
                break;
            }
        }
        if (check)
            continue;

        a.account = account;
        break;
    }
}

void Register::register_password(UserData &p) {
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

        p.password = password;
        break;
    }
}

//
void Register::register_phone_number(UserData &phone) {
    std::string phone_number;
    std::regex phone_regex("^1(3\\d|47|5([0-3]|[5-9])|8(0|2|[5-9]))\\d{8}$");
    while (true) {
        std::cout << "input your phone number: ";

        std::cin >> phone_number;

        if (!std::regex_match(phone_number, phone_regex)) {
            std::cout << "phone number is illegal!" << std::endl;
            continue;
         }

        phone.phone_number = phone_number;
        break;
    }
}

//
void Register::register_id_number(UserData &ID) {
    std::regex id_number_regex("^([1-9]\\d{5}[12]\\d{3}(0[1-9]|1[012])(0[1-9]|[12][0-9]|3[01])\\d{3}[0-9xX])$");
    std::string id_number;

    while (true) {
        std::cout << "input your idCard number: ";

        std::cin >> id_number;

        if (!std::regex_match(id_number, id_number_regex)) {
            std::cout << "idCard is illegal!" << std::endl;
            continue;
        }

        ID.id_number = id_number;
        break;
    }
}

void Register::register_email(UserData &E) {
    std::string email;
    // TODO: 邮箱的正则表达式
    std::regex email_regex("");

}

void Register_User::Register_Account() {

    register_account(data);
    register_password(data);
    register_phone_number(data);
    register_id_number(data);

}