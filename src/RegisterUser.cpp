//
// Created by shrimp on 2020/12/27.
//

#include "RegisterUser.h"
#include <string>
#include <regex>


RegisterUser::RegisterUser() {
    DB& db = DB::getInstance();

    db.open();
    all_user_data = db.select_all_user_data();
    db.close();
}

RegisterUser::~RegisterUser() {
//    DB& db = DB::getInstance();

//    db.open();
//    db.insert_user_data(user);
//    db.close();
}

// if you register successfully, will return true.
// 传入参数
// 账号，密码，密码确认，用户名，注册人姓名，手机号码
// 电子邮箱（qq邮箱）,注册人身份证，支付密码，支付密码确认
bool RegisterUser::Register(const std::string& in_account,
                            const std::string& in_password,
                            const std::string& in_confirm_password,
                            const std::string& in_user_name,
                            const std::string& in_real_name,
                            const std::string& in_phone_number,
                            const std::string& in_email,
                            const std::string& in_id_number,
                            const std::string& in_pay_password,
                            const std::string& in_pay_confirm_password) {
    if (!(register_account(in_account) && register_password(in_password, in_confirm_password) &&
        register_pay_password(in_pay_password, in_pay_confirm_password) && register_user_name(in_user_name) &&
        register_real_name(in_real_name) && register_phone_number(in_phone_number) &&
        register_email(in_email) && register_id_number(in_id_number))) return false;

    return true;
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
    if (password != confirm_password) return false;

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
    // std::regex phone_regex("^1(3\\d|47|5([0-3]|[5-9])|8(0|2|[5-9]))\\d{8}$");
    std::regex phone_regex("(.*)");

    // 未匹配成功
    if (!std::regex_match(phone_number, phone_regex)) return false;

    user.phone_number = phone_number;
    return true;
}

bool RegisterUser::register_email(const std::string& email) {
    // only support qq email and google email
    std::regex email_regex("(.*)(@qq.com|@gmail.com)");

    // 未匹配成功
    if (!std::regex_match(email, email_regex)) return false;

    user.email = email;
    return true;
}

bool RegisterUser::register_id_number(const std::string& id_number) {
//    std::regex id_number_regex("^([1-9]\\d{5}[12]\\d{3}(0[1-9]|1[012])(0[1-9]|[12][0-9]|3[01])\\d{3}[0-9xX])$");
    std::regex id_number_regex("(.*)");

    // 未匹配成功
    if (!std::regex_match(id_number, id_number_regex)) return false;

    user.id_number = id_number;
    return true;
}

bool RegisterUser::register_pay_password(const std::string& pay_password, const std::string& confirm_pay_password) {

    // add some pay password rule.
    if (pay_password != confirm_pay_password) return false;

    user.wallet.money = 0;
    user.wallet.password = pay_password;
    return true;
}