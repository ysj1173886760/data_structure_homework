//
// Created by shrimp on 2020/12/27.
//

#include "LoginManager.h"

LoginManager::LoginManager() {
    DB& db = DB::getInstance();

    all_manager_data = db.select_all_manager_data();
}

bool LoginManager::Login(const std::string &account, const std::string &password) {

    //
    for (const auto& it : all_manager_data) {
        if (it.account == account && it.password == password) {
            return true;
        }
    }

    return false;
}