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

    void Login();
    void ChangePassword();
    void ForgotPassword();

private:
    std::vector<ManagerData> all_manager_data;
};

#endif //DATA_STRUCTURE_HOMEWORK_LOGINMANAGER_H
