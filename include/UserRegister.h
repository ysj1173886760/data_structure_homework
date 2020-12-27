//
// Created by shrimp on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_USERREGISTER_H
#define DATA_STRUCTURE_HOMEWORK_USERREGISTER_H

#include "DB.h"
#include "base.h"
#include <vector>

class UserRegister {
public:
    UserRegister();

    void Register();

private:
    void register_account();
    void register_password();
    void register_user_name();
    void register_real_name();
    void register_phone_number();
    void register_email();
    void register_id_number();

    UserData user;
    std::vector<UserData> all_user_data;
};


#endif //DATA_STRUCTURE_HOMEWORK_USERREGISTER_H
