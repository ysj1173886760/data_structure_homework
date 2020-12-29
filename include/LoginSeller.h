//
// Created by shrimp on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_LOGINSELLER_H
#define DATA_STRUCTURE_HOMEWORK_LOGINSELLER_H

#include "DB.h"
#include "base.h"
#include <vector>

class LoginSeller {
public:
    LoginSeller();

    // return true, if you login successfully.
    bool Login(const std::string& account, const std::string& password);

    // ok change, return 0
    // account or password wrong, return -1
    // verification wrong, return -2
    // illegal password, return -3
    int ChangePassword(const std::string& account,
                       const std::string& password,
                       const std::string& shop_owner_name,
                       const std::string& shop_owner_phone_number,
                       const std::string& new_password,
                       const std::string& new_confirm_password);

    // return 0, if you change successfully
    // return 1, the account is wrong
    // return 2, verification error
    // return 3, password is illegal
    int ForgotPassword(const std::string& account,
                       const std::string& shop_name,
                       const std::string& shop_owner_name,
                       const std::string& shop_owner_phone_number,
                       const std::string& shop_owner_id_number,
                       const std::string& new_password,
                       const std::string& new_confirm_password);

private:
    std::vector<SellerData> all_seller_data;

    static bool ok_password(const std::string& password, const std::string& confirm_password);
    bool ok_pay_password(const std::string& pay_password, const std::string& confirm_pay_password);
};


#endif //DATA_STRUCTURE_HOMEWORK_LOGINSELLER_H
