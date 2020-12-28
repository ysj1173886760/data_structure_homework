//
// Created by shrimp on 2020/12/28.
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
    DB &db = DB::getInstance();
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
    std::string shop_name;
    std::string shop_address;
    std::string shop_owner_name;
    std::string shop_owner_phone_number;
    std::string shop_owner_id_number;


//    std::cout << "account: ";
//    std::cin >> account;
//    std::cout << "password: ";
//    std::cin >> password;
//    std::cout << "confirm_password: ";
//    std::cin >> confirm_password;
//    std::cout << "user_name: ";
//    std::cin >> user_name;
//    std::cout << "real_name: ";
//    std::cin >> real_name;
//    std::cout << "phone_number: ";
//    std::cin >> phone_number;
//    std::cout << "email: ";
//    std::cin >> email;
//    std::cout << "id_number: ";
//    std::cin >> id_number;
//    std::cout << "pay_password: ";
//    std::cin >> pay_password;
//    std::cout << "pay_confirm_password: ";
//    std::cin >> pay_confirm_password;
//
//    std::cout << "shop_name:";
//    std::cin >> shop_name;
//    std::cout << "shop_address: ";
//    std:cin >> shop_address;
//    std::cout << "shop_owner_name: ";
//    std::cin >> shop_owner_name;
//    std::cout << "shop_owner_phone_number: ";
//    std::cin >> shop_owner_phone_number;
//    std::cout << "shop_owner_id_number: ";
//    std::cin >> shop_owner_id_number;
//    std::cout << "pay_password: ";
//    std::cin >> pay_password;
//    std::cout << "pay_confirm_password: ";
//    std::cin >> pay_confirm_password;

//    RegisterUser ru;
//    if (ru.Register(account,
//                    password,
//                    confirm_password,
//                    user_name,
//                    real_name,
//                    phone_number,
//                    email,
//                    id_number,
//                    pay_password,
//                    pay_confirm_password)) {
//        std::cout << "ok" << std::endl;
//    } else {
//        std::cout << "no ok" << std::endl;
//    }

//    LoginUser lu;
//
//    std::cin >> account;
//    std::cin >> password;
//
//    if (lu.Login(account, password)) {
//        std::cout << "login ok" << std::endl;
//    }
//    else {
//        std::cout << "login no ok" << std::endl;
//    }

//    RegisterSeller rs;
//    if (rs.Register(account,
//                password,
//                confirm_password,
//                shop_name,
//                shop_address,
//                shop_owner_name,
//                shop_owner_phone_number,
//                shop_owner_id_number,
//                pay_password,
//                pay_confirm_password)) {
//        std::cout << "register ok!" << std::endl;
//    }
//    else {
//        std::cout << "no register!" << std::endl;
//    }

//    RegisterManager r;
//    if (r.Register(account, password, confirm_password)) {
//        std::cout << "ok" << std::endl;
//    }
//    else {
//        std::cout << "no ok " << std::endl;
//    }

//    LoginManager l;
//    if (l.Login(account, password)) {
//        std::cout << "ok" << std::endl;
//    }
//    else {
//        std::cout << "no ok" << std::endl;
//    }
    db.close();
}