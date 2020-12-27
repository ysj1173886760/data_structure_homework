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

    void Register();
    void print() {
        seller.print();
    }
private:
    void register_account();
    void register_password();
    void register_shop_name();
    void register_shop_address();
    void register_shop_owner_name();
    void register_shop_owner_phone_number();
    void register_shop_owner_id_number();

    void copy_to_register_requests_data();

    SellerData seller;
    RegisterRequestData rrd;
    std::vector<SellerData> all_seller_data;
};

#endif //DATA_STRUCTURE_HOMEWORK_REGISTERSELLER_H
