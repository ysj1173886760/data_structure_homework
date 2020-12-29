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

// 这个到底是返回 true 还是 false，因为需要向管理员发出申请，所以怎么定义注册成功
// 传入参数
// 账号，密码，密码确认，店铺名字，店铺地址
// 拥有者姓名，拥有者手机号，拥有者身份证，支付密码，支付密码确认
bool RegisterSeller::Register(const std::string& account,
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
    if (!(register_account(account) && register_password(password, confirm_password) &&
    register_pay_password(pay_password, confirm_pay_password) && register_shop_name(shop_name) &&
    register_shop_address(shop_address) && register_shop_owner_name(shop_owner_name) &&
    register_shop_owner_phone_number(shop_owner_phone_number) && register_shop_owner_id_number(shop_owner_id_number))) {
        std::cout << "Register Seller fail" << std::endl;
        return false;
    }

    copy_to_register_requests_data();

    DB& db = DB::getInstance();

    // 将申请放入请求列表中
    db.insert_register_request_data(rrd);

    return true;
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

    // password rule
    if (password != confirm_password) {
        return false;
    }

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

    // pay password rule
    if (pay_password != confirm_pay_password) {
        return false;
    }

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