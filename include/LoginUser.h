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

    // 输入账号，密码
    // 如果账号密码正确 return true;
    bool Login(const std::string& account, const std::string& password);

    // 账号，密码，邮箱，新密码，确认新密码
    // 成功修改返回 0
    // 账号密码错误返回 -1
    // 验证信息错误返回 -2
    // 非法密码返回 -3
    int ChangePassword(const std::string& account,
                        const std::string& password,
                        const std::string& email,
                        const std::string& new_password,
                        const std::string& new_confirm_password);


    // 账号，密码，邮箱，真实姓名，身份证，新的支付密码，确认新的支付密码
    // 成功修改返回 0
    // 账号密码错误返回 -1
    // 验证信息错误返回 -2
    // 非法支付密码返回 -3
    int ChangePayPassword(const std::string& account,
                          const std::string& password,
                          const std::string& email,
                          const std::string& real_name,
                          const std::string& id_number,
                          const std::string& new_pay_password,
                          const std::string& new_confirm_pay_password);

    // 可以的话可以添加邮箱认证，通过邮箱获取验证码
    // 账号，邮箱，新密码，确认新密码
    // 成功修改返回 0
    // 无此账号返回 -1
    // 邮箱错误返回 -2（该账号不是使用这个邮箱注册的)
    // 非法密码返回 -3（密码不符合密码规则，密码与确认密码不相同）
    // 验证码错误返回 -4（如果添加了邮箱注册，才有这种情况）
    int ForgotPassword(const std::string& account,
                        const std::string& email,
                        const std::string& new_password,
                        const std::string& new_confirm_password);
private:
    std::vector<UserData> all_user_data;

    bool OkPassword(const std::string& password, const std::string& confirm_password);
    bool OkPayPassword(const std::string& pay_password, const std::string& confirm_pay_password);
};


#endif //DATA_STRUCTURE_HOMEWORK_LOGINUSER_H
