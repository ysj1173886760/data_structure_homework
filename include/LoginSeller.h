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

    void Login();
    void ChangePassword();
    void ForgotPassword();

private:
    std::vector<SellerData> all_seller_data;
};


#endif //DATA_STRUCTURE_HOMEWORK_LOGINSELLER_H
