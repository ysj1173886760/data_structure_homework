//
// Created by shrimp on 2020/12/28.
//

#include "ManagerOperation.h"

ManagerOperation::ManagerOperation() {

}

std::vector<RegisterRequestData> ManagerOperation::view_application_list() {
    DB& db = DB::getInstance();
    return db.select_all_register_request_data();
}

// return true if you accept successfully.
bool ManagerOperation::accept_shop_application(const std::string &account, const std::string &shop_name) {
    std::vector<RegisterRequestData> all_register_request_data;
    std::vector<SellerData> all_seller_data;

    DB& db = DB::getInstance();
    all_register_request_data = db.select_all_register_request_data();
    all_seller_data = db.select_all_seller_data();

    for (const auto& it : all_register_request_data) {
        if (it.account == account && it.shop_name == shop_name) {
            // if the account or the shop_name already exists, return false;
            for (const auto& i : all_seller_data) {
                if (i.account == account || i.shop_name == shop_name) {
                    return false;
                }
            }
            db.insert_seller_data(copy_request_to_seller(it));
            db.delete_register_request_data(it.id);
            return true;
        }
    }

    // maybe the account or shop_name is wrong
    return false;
}

// return true if you delete successfully
bool ManagerOperation::reject_shop_application(const std::string &account, const std::string &shop_name) {
    //
    std::vector<RegisterRequestData> all_register_request_data;

    DB& db = DB::getInstance();
    all_register_request_data = db.select_all_register_request_data();

    for (const auto& it : all_register_request_data) {
        if (it.account == account && it.shop_name == shop_name) {
            db.delete_register_request_data(it.id);
            return true;
        }
    }

    // no this account or shop_name
    return false;
}

// only support root manager to register other managers
bool ManagerOperation::register_manager(const std::string &account, const std::string &password) {
    if (account == "account" && password == "password") {

    }
}

SellerData ManagerOperation::copy_request_to_seller(const RegisterRequestData& rqd) {
    SellerData seller;
    seller.account = rqd.account;
    seller.password = rqd.password;
    seller.shop_name = rqd.shop_name;
    seller.shop_address = rqd.shop_address;
    seller.shop_owner_name = rqd.shop_owner_name;
    seller.shop_owner_phone_number = rqd.shop_owner_phone_number;
    seller.shop_owner_id_number = rqd.shop_owner_id_number;
    return seller;
}