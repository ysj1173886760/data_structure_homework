//
// Created by shrimp on 2020/12/24.
//

#ifndef DATA_STRUCTURE_HOMEWORK_REGISTER_H
#define DATA_STRUCTURE_HOMEWORK_REGISTER_H

#include <iostream>
#include "DB.h"
#include "base.h"


class Register {
    // user, seller, manager
public:
    Register();
//    ~Register();

    virtual void Register_Account() {
        std::cout << "virtual Register_Account" << std::endl;
    }

    void register_account(UserData&);
    void register_password(UserData&);
    void register_id_number(UserData&);
//    void register_real_name(UserData&);
//    void register_user_name(UserData&);
    void register_email(UserData&);
    void register_phone_number(UserData&);

private:
    // a vector of userdata
    std::vector<UserData> user_data;
};

class Register_User: public Register {
public:
    void Register_Account();
    void print() {
        data.print();
    }
private:
    UserData data;
};
class Register_Seller: public Register {
public:

};

class Register_Manager: public Register {
public:

};


#endif //DATA_STRUCTURE_HOMEWORK_REGISTER_H
