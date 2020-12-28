//
// Created by shrimp on 2020/12/27.
//

#include "LoginSeller.h"
#include <string>

LoginSeller::LoginSeller() {
    DB& db = DB::getInstance();
    db.open();
    all_seller_data = db.select_all_seller_data();
    db.close();
}

bool LoginSeller::Login(const std::string &account, const std::string &password) {

}