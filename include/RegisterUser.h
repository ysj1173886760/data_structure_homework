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

    // if you register successfully, will return 0.
    // the account or user_name already exists, return 1.
    // the password and confirm password are different, or password is illegal, return 2.
    // phone, email, id_number is illegal, return 3.
    // pay password and confirm pay password are different or pay password is illegal, return 4.
    int Register(const std::string& in_account,
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
