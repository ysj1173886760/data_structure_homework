//
// Created by shrimp on 2020/12/27.
//

#include "RegisterSeller.h"
#include <string>
#include <regex>

RegisterSeller::RegisterSeller() {
    DB& db = DB::getInstance();

    all_seller_data = db.select_all_seller_data();
}

RegisterSeller::~RegisterSeller() {

}


// return 0, if you successfully apply to manager
// return 1, if account or shop_name already exists
// return 2, if password is illegal
// return 3, if address, owner name, phone or id number wrong
// return 4, if pay_password is illegal
int RegisterSeller::Register(const std::string& account,
                              const std::string& password,
                              const std::string& confirm_password,
                              const std::string& shop_name,
                              const std::string& shop_address,
                              const std::string& shop_owner_name,
                              const std::string& shop_owner_phone_number,
                              const std::string& shop_owner_id_number,
                              const std::string& pay_password,
                              const std::string& confirm_pay_password) {
    //
    if (!register_account(account) || !register_shop_name(shop_name)) return 1;
    if (!register_password(password, confirm_password)) return 2;
    if (!register_shop_address(shop_address) || !register_shop_owner_name(shop_owner_name)
    || !register_shop_owner_phone_number(shop_owner_phone_number)
    || !register_shop_owner_id_number(shop_owner_id_number))
        return 3;
    if (!register_pay_password(pay_password, confirm_pay_password)) return 4;

    copy_to_register_requests_data();

    DB& db = DB::getInstance();
    // insert the seller data to register request list
    db.insert_register_request_data(rrd);

    return 0;
}

bool RegisterSeller::register_account(const std::string& account) {

    // if the account already exists
    for (const auto& it : all_seller_data) {
        if (it.account == account) {
            return false;
        }
    }

    seller.account = account;
    return true;
}

bool RegisterSeller::register_password(const std::string& password, const std::string& confirm_password) {

    // add some password rule
    // password length must be greater than 8 bits and less than 16 bits.
    if (password != confirm_password) return false;
    if (password.size() < 8 || password.size() > 16) return false;

    seller.password = password;
    return true;
}

bool RegisterSeller::register_shop_name(const std::string& shop_name) {

    // if the shop_name already exists
    for (const auto& it : all_seller_data) {
        if (it.shop_name == shop_name) {
            return false;
        }
    }

    seller.shop_name = shop_name;
    return true;
}

bool RegisterSeller::register_shop_address(const std::string& shop_address) {

    seller.shop_address = shop_address;
    return true;
}

bool RegisterSeller::register_shop_owner_name(const std::string& shop_owner_name) {

    seller.shop_owner_name = shop_owner_name;
    return true;
}

bool RegisterSeller::register_shop_owner_phone_number(const std::string& shop_owner_phone_number) {

    // std::regex phone_regex("^1(3\\d|47|5([0-3]|[5-9])|8(0|2|[5-9]))\\d{8}$");
    std::regex phone_regex("(.*)");

    //
    if (!std::regex_match(shop_owner_phone_number, phone_regex)) {
        return false;
    }

    seller.shop_owner_phone_number = shop_owner_phone_number;
    return true;
}

bool RegisterSeller::register_shop_owner_id_number(const std::string& shop_owner_id_number) {

//    std::regex id_number_regex("^([1-9]\\d{5}[12]\\d{3}(0[1-9]|1[012])(0[1-9]|[12][0-9]|3[01])\\d{3}[0-9xX])$");
    std::regex id_number_regex("(.*)");

    //
    if (!std::regex_match(shop_owner_id_number, id_number_regex)) {
        return false;
    }

    seller.shop_owner_id_number = shop_owner_id_number;
    return true;
}

bool RegisterSeller::register_pay_password(const std::string& pay_password, const std::string& confirm_pay_password) {

    // add some pay password rule.
    // only support number password, and password length must be 6.
    std::regex regex_pay_password("\\d*");

    if (pay_password.size() != 6) return false;
    if (pay_password != confirm_pay_password) return false;
    if (!std::regex_match(pay_password, regex_pay_password)) return false;

    seller.wallet.money = 0;
    seller.wallet.password = pay_password;
    return true;
}

// copy the seller's information to RegisterRequestsData list.
// apply to manager
void RegisterSeller::copy_to_register_requests_data() {
    rrd.account = seller.account;
    rrd.password = seller.password;
    rrd.shop_name = seller.shop_name;
    rrd.shop_address = seller.shop_address;
    rrd.shop_owner_name = seller.shop_owner_name;
    rrd.shop_owner_phone_number = seller.shop_owner_phone_number;
    rrd.shop_owner_id_number = seller.shop_owner_id_number;
}