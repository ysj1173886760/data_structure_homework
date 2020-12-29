//
// Created by shrimp on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_REGISTERSELLER_H
#define DATA_STRUCTURE_HOMEWORK_REGISTERSELLER_H


#include "DB.h"
#include "base.h"
#include <vector>

// seller register need to apply to manager
class RegisterSeller {
public:
    RegisterSeller();
    ~RegisterSeller();

    // return 0, if you successfully apply to manager
    // return 1, if account or shop_name already exists
    // return 2, if password is illegal
    // return 3, if address, owner name, phone or id number wrong
    // return 4, if pay_password is illegal
    int Register(const std::string& account,
                  const std::string& password,
                  const std::string& confirm_password,
                  const std::string& shop_name,
                  const std::string& shop_address,
                  const std::string& shop_owner_name,
                  const std::string& shop_owner_phone_number,
                  const std::string& shop_owner_id_number,
                  const std::string& pay_password,
                  const std::string& confirm_pay_password);
    void print() {
        seller.print();
    }
private:
    bool register_account(const std::string&);
    bool register_password(const std::string&, const std::string&);
    bool register_shop_name(const std::string&);
    bool register_shop_address(const std::string&);
    bool register_shop_owner_name(const std::string&);
    bool register_shop_owner_phone_number(const std::string&);
    bool register_shop_owner_id_number(const std::string&);
    bool register_pay_password(const std::string&, const std::string&);

    void copy_to_register_requests_data();

    SellerData seller;
    RegisterRequestData rrd;
    std::vector<SellerData> all_seller_data;
};

#endif //DATA_STRUCTURE_HOMEWORK_REGISTERSELLER_H
