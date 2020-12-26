//
// Created by shrimp on 2020/12/26.
//

#ifndef DATA_STRUCTURE_HOMEWORK_LOGIN_H
#define DATA_STRUCTURE_HOMEWORK_LOGIN_H

#include "DB.h"
#include "base.h"
#include <vector>

class LoginSystem {
public:
    LoginSystem();
    void Login();
    void ChangePassword();
    void RecoverPassword();

private:
    std::vector<UserData> user_data;
    std::string SetPassword();
private:
};


#endif //DATA_STRUCTURE_HOMEWORK_LOGIN_H
