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

    // 输入账号密码，成功登入就返回 true.
    bool Login(const std::string& account, const std::string& password);

    // 输入：账号，密码，店主姓名，店主手机号，新密码，确认新密码
    // 成功修改返回 0
    // 账号密码错误 -1
    // 验证信息错误 -2
    // 非法密码 -3
    int ChangePassword(const std::string& account,
                       const std::string& password,
                       const std::string& shop_owner_name,
                       const std::string& shop_owner_phone_number,
                       const std::string& new_password,
                       const std::string& new_confirm_password);

    // 输入：账号，店名，店主姓名，店主手机号，店主身份证，新密码，确认新密码
    // 成功修改返回 0
    // 验证信息错误 -1
    // 非法密码 -2
    int ForgotPassword(const std::string& account,
                       const std::string& shop_name,
                       const std::string& shop_owner_name,
                       const std::string& shop_owner_phone_number,
                       const std::string& shop_owner_id_number,
                       const std::string& new_password,
                       const std::string& new_confirm_password);

private:
    std::vector<SellerData> all_seller_data;
};


#endif //DATA_STRUCTURE_HOMEWORK_LOGINSELLER_H
