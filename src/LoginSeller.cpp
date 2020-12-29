//
// Created by shrimp on 2020/12/27.
//

#include "LoginSeller.h"
#include <string>

LoginSeller::LoginSeller() {
    DB& db = DB::getInstance();

    all_seller_data = db.select_all_seller_data();
}

bool LoginSeller::Login(const std::string &account, const std::string &password) {

    for (const auto& it : all_seller_data) {
        if (it.account == account && it.password == password) {
            return true;
        }
    }

    return false;
}