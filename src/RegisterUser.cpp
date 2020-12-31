//
// Created by shrimp on 2020/12/27.
//

#include "RegisterUser.h"
#include <string>
#include <regex>


RegisterUser::RegisterUser() {
    DB& db = DB::getInstance();

    all_user_data = db.select_all_user_data();
}

RegisterUser::~RegisterUser() {
//    DB& db = DB::getInstance();

//    db.open();
//    db.insert_user_data(user);
//    db.close();
}


int RegisterUser::Register(const std::string& in_account,
                            const std::string& in_password,
                            const std::string& in_confirm_password,
                            const std::string& in_user_name,
                            const std::string& in_real_name,
                            const std::string& in_phone_number,
                            const std::string& in_email,
                            const std::string& in_id_number,
                            const std::string& in_pay_password,
                            const std::string& in_pay_confirm_password) {

    if (!register_account(in_account) || !register_user_name(in_user_name)) return 1;
    if (!register_password(in_password, in_confirm_password)) return 2;
    if (!register_phone_number(in_phone_number) || !register_email(in_email) || !register_id_number(in_id_number)) return 3;
    if (!register_pay_password(in_pay_password, in_pay_confirm_password)) return 4;

    DB& db = DB::getInstance();

    db.insert_user_data(user);

    return 0;
}

// if the account already exists, return false.
bool RegisterUser::register_account(const std::string& account) {

    for (const auto& it : all_user_data) {
        if (it.account == account)
            return false;
    }

    user.account = account;
    return true;
}

//
bool RegisterUser::register_password(const std::string& password, const std::string& confirm_password) {
    // add some password rule
    // password length must be greater than 8 bits and less than 16 bits.
    if (password != confirm_password) return false;
    if (password.size() < 8 || password.size() > 16) return false;

    user.password = password;
    return true;
}

//
bool RegisterUser::register_user_name(const std::string& user_name) {
   //
   for (const auto& it : all_user_data) {
       if (it.user_name == user_name)
           return false;
   }

   user.user_name = user_name;
   return true;
}

bool RegisterUser::register_real_name(const std::string& real_name) {

    user.real_name = real_name;
    return true;
}

bool RegisterUser::register_phone_number(const std::string& phone_number) {
    // 对于手机号的正则表达式
    std::regex phone_regex("^1(3\\d|47|5([0-3]|[5-9])|8(0|2|[5-9]))\\d{8}$");
    // std::regex phone_regex("(.*)");

    // 未匹配成功
    if (!std::regex_match(phone_number, phone_regex)) return false;

    user.phone_number = phone_number;
    return true;
}

bool RegisterUser::register_email(const std::string& email) {
    // only support qq email
    std::regex email_regex("(.*)@qq.com");

    // 未匹配成功
    if (!std::regex_match(email, email_regex)) return false;

    user.email = email;
    return true;
}

bool RegisterUser::register_id_number(const std::string& id_number) {
    std::regex id_number_regex(R"(^([1-9]\d{5}[12]\d{3}(0[1-9]|1[012])(0[1-9]|[12][0-9]|3[01])\d{3}[0-9xX])$)");

    // 未匹配成功
    if (!std::regex_match(id_number, id_number_regex)) return false;

    user.id_number = id_number;
    return true;
}

bool RegisterUser::register_pay_password(const std::string& pay_password, const std::string& confirm_pay_password) {

    // add some pay password rule.
    // only support number password, and password length must be 6.
    std::regex regex_pay_password("\\d*");

    if (pay_password.size() != 6) return false;
    if (pay_password != confirm_pay_password) return false;
    if (!std::regex_match(pay_password, regex_pay_password)) return false;

    user.wallet.money = 0;
    user.wallet.password = pay_password;
    return true;
}