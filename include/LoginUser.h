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

    bool Login();
    void ChangePassword();
    void ForgotPassword();
private:
    std::vector<UserData> all_user_data;

    std::string OkPassword();
};


#endif //DATA_STRUCTURE_HOMEWORK_LOGINUSER_H
