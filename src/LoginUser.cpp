//
// Created by shrimp on 2020/12/27.
//

#include "LoginUser.h"
#include <string>

LoginUser::LoginUser() {
    DB& db = DB::getInstance();

    all_user_data = db.select_all_user_data();
}

// if you login successfully, this function will return true;
bool LoginUser::Login(const std::string& account, const std::string& password) {

    //
    for (const auto& it : all_user_data) {
        if (it.account == account && it.password == password) {
            std::cout << "login successfully!" << std::endl;
            return true;
        }
    }

    std::cout << "login error!" << std::endl;
    return false;
}

int LoginUser::ChangePassword(const std::string &account,
                              const std::string &password,
                              const std::string &email,
                              const std::string &new_password,
                              const std::string &new_confirm_password) {

    for (auto it : all_user_data) {
        if (it.account == account && it.password == password) {
            if (it.email == email) {
                if (OkPassword(new_password, new_confirm_password)) {
                    it.password = password;
                    return 0;
                }
                else {
                    return -3;
                } // password is illegal
            }
            else {
                return -2;
            } // email is wrong
        }
        else {
            return -1;
        } // account or password wrong
    }
}

//
int LoginUser::ChangePayPassword(const std::string &account,
                                 const std::string &password,
                                 const std::string &email,
                                 const std::string &real_name,
                                 const std::string &id_number,
                                 const std::string &new_pay_password,
                                 const std::string &new_confirm_pay_password) {
    //
    for (auto it : all_user_data) {
        if (it.account == account && it.password == password) {
            if (it.email == email && it.real_name == real_name && it.id_number == id_number) {
                if (OkPayPassword(new_pay_password, new_confirm_pay_password)) {
                    it.wallet.password = new_pay_password;
                    return 0;
                }
                else {
                    return -3;
                } // pay password is illegal
            }
            else {
                return -2;
            } // authentication is wrong
        }
        else {
            return -1;
        } // account or password is wrong
    }
}


int LoginUser::ForgotPassword(const std::string &account,
                               const std::string &email,
                               const std::string &new_password,
                               const std::string &new_confirm_password) {

    //
    for (auto it : all_user_data) {
        if (it.account == account) {
            if (it.email == email) {
                if (OkPassword(new_password, new_confirm_password)) {
                    it.password = new_password;
                    return 0;
                }
                else {
                    return -3;
                }
            }
            else {
                return -2;
            } // it.email != email
        } // it.account == account
    }

    return -1;
}

// password rule
bool LoginUser::OkPassword(const std::string& password, const std::string& confirm_password) {

    // password rule
    if (password != confirm_password) {
        return false;
    }

    return true;
}

//
bool LoginUser::OkPayPassword(const std::string &pay_password, const std::string &confirm_pay_password) {

    // pay password rule
    if (pay_password != confirm_pay_password) {
        return false;
    }

    return true;
}