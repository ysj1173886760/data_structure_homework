//
// Created by shrimp on 2020/12/27.
//

#include "RegisterUser.h"
#include "RegisterSeller.h"
#include "RegisterManager.h"
#include "LoginUser.h"
#include "LoginSeller.h"
#include "LoginManager.h"
#include "DB.h"
#include <vector>

int main() {
    DB& db = DB::getInstance();
    db.open();

    std::string account;
    std::string password;
    std::string confirm_password;
    std::string user_name;
    std::string real_name;
    std::string phone_number;
    std::string email;
    std::string id_number;
    std::string pay_password;
    std::string pay_confirm_password;

    std::cin >> account;
    std::cin >> password;
    std::cin >> confirm_password;
    std::cin >> user_name;
    std::cin >> real_name;
    std::cin >> phone_number;
    std::cin >> email;
    std::cin >> id_number;
    std::cin >> pay_password;
    std::cin >> pay_confirm_password;

    RegisterUser ru;
    if (ru.Register(account,
                password,
                confirm_password,
                user_name,
                real_name,
                phone_number,
                email,
                id_number,
                pay_password,
                pay_confirm_password)) {
        std::cout << "ok" << std::endl;
    }
    else {
        std::cout << "no ok" << std::endl;
    }

    db.close();
}