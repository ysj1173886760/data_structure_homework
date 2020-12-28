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


// 传入参数
// 账号，密码，密码确认
bool RegisterManager::Register(const std::string& account,
                               const std::string& password,
                               const std::string& confirm_password) {
    //
    if (!(register_account(account) && register_password(password, confirm_password))) {
        return false;
    }

    DB& db = DB::getInstance();
    db.open();
    db.insert_manager_data(manager);
    db.close();

    return true;
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