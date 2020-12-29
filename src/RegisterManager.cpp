//
// Created by shrimp on 2020/12/27.
//

#include "RegisterManager.h"
#include <string>
#include <regex>

RegisterManager::RegisterManager() {
    DB& db = DB::getInstance();

    all_manager_data = db.select_all_manager_data();
}


int RegisterManager::Register(const std::string& account,
                               const std::string& password,
                               const std::string& confirm_password) {
    //
    if (!register_account(account)) return 1;
    if (!register_password(password, confirm_password)) return 2;

    DB& db = DB::getInstance();
    db.insert_manager_data(manager);

    return 0;
}

//
bool RegisterManager::register_account(const std::string& account) {

    // the account already exists
    for (const auto& it : all_manager_data) {
        if (it.account == account) {
            return false;
        }
    }

    manager.account = account;
    return true;
}

//
bool RegisterManager::register_password(const std::string& password, const std::string& confirm_password) {

    // password rule
    if (password != confirm_password) {
        return false;
    }

    manager.password = password;
    return true;
}