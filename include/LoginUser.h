//
// Created by shrimp on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_LOGINUSER_H
#define DATA_STRUCTURE_HOMEWORK_LOGINUSER_H

#include "DB.h"
#include "base.h"
#include <vector>

class LoginUser {
public:
    LoginUser();

    // return true if you login is ok.
    bool Login(const std::string& account, const std::string& password);

    // ok change 0
    // account or password wrong -1
    // email is wrong -2
    // illegal password -3
    // maybe we can add a email verification code.
    int ChangePassword(const std::string& account,
                       const std::string& password,
                       const std::string& email,
                       const std::string& new_password,
                       const std::string& new_confirm_password);


    // ok change  0
    // account or password wrong -1
    // verification information error -2
    // illegal pay password -3
    // maybe we can add a email verification code.
    int ChangePayPassword(const std::string& account,
                          const std::string& password,
                          const std::string& email,
                          const std::string& real_name,
                          const std::string& id_number,
                          const std::string& new_pay_password,
                          const std::string& new_confirm_pay_password);

    // maybe we can add a email verification code
    // return 0, change successfully.
    // return 1, account is wrong
    // return 2, verification information is wrong
    // return 3, email is wrong
    // return 4, password is illegal
    int ForgotPassword(const std::string& account,
                       const std::string& real_name,
                       const std::string& id_number,
                       const std::string& email,
                       const std::string& new_password,
                       const std::string& new_confirm_password);

    // maybe we can add a email verification code
    // return 0, if you get your pay password successfully.
    // return 1, account or password wrong
    // return 2, verification information is wrong
    // return 3, email is wrong
    // return 4, pay password is illegal
    int ForgotPayPassword(const std::string& account,
                          const std::string& password,
                          const std::string& real_name,
                          const std::string& id_number,
                          const std::string& email,
                          const std::string& new_pay_password,
                          const std::string& new_confirm_pay_password);
private:
    std::vector<UserData> all_user_data;

    static bool OkPassword(const std::string& password, const std::string& confirm_password);
    static bool OkPayPassword(const std::string& pay_password, const std::string& confirm_pay_password);
};


#endif //DATA_STRUCTURE_HOMEWORK_LOGINUSER_H
