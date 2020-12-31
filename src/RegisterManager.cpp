//
// Created by shrimp on 2020/12/27.
//

#include "RegisterManager.h"
#include <string>
#include <regex>


int RegisterManager::Register(const std::string& account,
                               const std::string& password,
                               const std::string& confirm_password) {
    DB& db = DB::getInstance();

    all_manager_data = db.select_all_manager_data();

    //
    if (!register_account(account)) return 1;
    if (!register_password(password, confirm_password)) return 2;

    db.insert_manager_data(manager);

    return 0;
}

//
bool RegisterManager::register_account(const std::string& account) {

    DB& db = DB::getInstance();

    all_manager_data = db.select_all_manager_data();

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

    // add some password rule
    // password length must be greater than 8 bits and less than 16 bits.
    if (password != confirm_password) return false;
    if (password.size() < 8 || password.size() > 16) return false;

    manager.password = password;
    return true;
}