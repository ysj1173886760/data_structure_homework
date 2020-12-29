//
// Created by shrimp on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_LOGINMANAGER_H
#define DATA_STRUCTURE_HOMEWORK_LOGINMANAGER_H


#include "DB.h"
#include "base.h"
#include <vector>

class LoginManager {
public:
    LoginManager();

    // 成功登入返回 true.
    bool Login(const std::string& account, const std::string& password);

    // ok change, return 0
    // account or password wrong, return -1.
    // illegal password, return -2.
    int ChangePassword(const std::string& account,
                       const std::string& password,
                       const std::string& new_password,
                       const std::string& new_confirm_password);

    // only support root manager give you new password.
    // ok change, return 0.
    // you are not root manager, return -1.
    // no this account, return -2.
    // illegal password, return -3.
    int ForgotPassword(const std::string& account,
                       const std::string& password,
                       const std::string& _account,
                       const std::string& new_password,
                       const std::string& new_confirm_password);

private:
    std::vector<ManagerData> all_manager_data;

    bool ok_password(const std::string& password, const std::string& confirm_password);
};

#endif //DATA_STRUCTURE_HOMEWORK_LOGINMANAGER_H
