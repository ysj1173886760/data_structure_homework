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

int LoginManager::ChangePassword(const std::string &account,
                                 const std::string &password,
                                 const std::string &new_password,
                                 const std::string &new_confirm_password) {
    for (auto it : all_manager_data) {
        if (it.account == account && it.password == password) {
            if (ok_password(new_password, new_confirm_password)) {
                it.password = new_password;

                DB& db = DB::getInstance();
                db.modify_manager_data(it.id, it);

                return 0;
            }
            else {
                return -2; // illegal password.
            }
        }
        if (it.account == account && it.password != password) {
            return -1; // account or password wrong
        }
    }

    return -1; // account or password wrong
}


int LoginManager::ForgotPassword(const std::string &account,
                                 const std::string &password,
                                 const std::string &_account,
                                 const std::string &new_password,
                                 const std::string &new_confirm_password) {
    if (account == "account" && password == "password") {
        for (auto it : all_manager_data) {
            if (it.account == _account) {
                if (ok_password(new_password, new_confirm_password)) {
                    it.password = new_password;

                    DB& db = DB::getInstance();
                    db.modify_manager_data(it.id, it);

                    return 0;
                }
                else {
                    return -3;
                } // illegal password
            }
        }
        return -2; // no account
    }
    else {
        return -1;
    } // no root
}

bool LoginManager::ok_password(const std::string &password, const std::string &confirm_password) {
    if (password != confirm_password) return false;
    if (password.size() < 8 || password.size() > 16) return false;

    return true;
}