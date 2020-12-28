//
// Created by shrimp on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_REGISTERUSER_H
#define DATA_STRUCTURE_HOMEWORK_REGISTERUSER_H

#include "DB.h"
#include "base.h"
#include <vector>
#include <string>

class RegisterUser {
public:
    RegisterUser();
    ~RegisterUser();

    // if you register successfully, will return true.
    // 传入参数
    // 账号，密码，密码确认，用户名，注册人姓名，手机号码
    // 电子邮箱（qq邮箱）,注册人身份证，支付密码，支付密码确认
    bool Register(const std::string& in_account,
                  const std::string& in_password,
                  const std::string& in_confirm_password,
                  const std::string& in_user_name,
                  const std::string& in_real_name,
                  const std::string& in_phone_number,
                  const std::string& in_email,
                  const std::string& in_id_number,
                  const std::string& in_pay_password,
                  const std::string& in_pay_confirm_password);
    void print() {
        user.print();
    }
private:
    bool register_account(const std::string& in_account);
    bool register_password(const std::string& in_password, const std::string& in_confirm_password);
    bool register_user_name(const std::string& in_user_name);
    bool register_real_name(const std::string& in_real_name);
    bool register_phone_number(const std::string& in_phone_number);
    bool register_email(const std::string& in_email);
    bool register_id_number(const std::string& in_id_number);
    bool register_pay_password(const std::string& in_pay_password, const std::string& in_pay_confirm_password);
    UserData user;
    std::vector<UserData> all_user_data;
};


#endif //DATA_STRUCTURE_HOMEWORK_REGISTERUSER_H
