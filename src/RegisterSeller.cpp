//
// Created by shrimp on 2020/12/27.
//

#include "RegisterSeller.h"
#include <string>
#include <regex>

RegisterSeller::RegisterSeller() {
    DB& db = DB::getInstance();

    db.open();
    all_seller_data = db.select_all_seller_data();
    db.close();
}

RegisterSeller::~RegisterSeller() {

}

void RegisterSeller::Register() {
    register_account();
    register_password();
    register_pay_password();

    register_shop_owner_name();
    register_shop_owner_phone_number();
    register_shop_owner_id_number();

    register_shop_name();
    register_shop_address();

    DB& db = DB::getInstance();
    db.open();

    copy_to_register_requests_data();
    db.insert_register_request_data(rrd);
    db.insert_seller_data(seller);

    db.close();
}

void RegisterSeller::register_account() {
    std::string account;

    while (true) {
        std::cout << "input your account: " << std::endl;

        std::cin >> account;

        bool check = false;
        for (auto it : all_seller_data) {
//            std::cout << "in for" << std::endl;
            if (it.account == account) {
                std::cout << "the account already exists!" << std::endl;
                check = true;
                break;
            }
        }
        if (check)
            continue;

        break;
    }

    seller.account = account;
}

void RegisterSeller::register_password() {
    std::string password;
    std::string confirm_password;
    while (true) {
        std::cout << "input your password: ";

        std::cin >> password;

        // password rule
        if (password.size() <= 6) {
            std::cout << "password is too simple" << std::endl;
            continue;
        }

        std::cout << "confirm your password: ";

        std::cin >> confirm_password;

        if (password != confirm_password) {
            std::cout << "password != confirm_password" << std::endl;
            continue;
        }

        break;
    }

    seller.password = password;
}

void RegisterSeller::register_shop_name() {
    std::string shop_name;

    while (true) {
        std::cout << "shop_name" << std::endl;

        std::cin >> shop_name;

        bool check = false;
        for (auto it : all_seller_data) {
            if (it.shop_name == shop_name) {
                check = true;
                break;
            }
        }

        if (check) continue;
        break;
    }

    seller.shop_name = shop_name;
}

void RegisterSeller::register_shop_address() {
    std::string shop_address;

    std::cout << "shop_address" << std::endl;

    std::cin >> shop_address;

    seller.shop_address = shop_address;
}

void RegisterSeller::register_shop_owner_name() {
    std::string shop_owner_name;

    std::cout << "shop_owner_name" << std::endl;
    std::cin >> shop_owner_name;

    seller.shop_owner_name = shop_owner_name;
}

void RegisterSeller::register_shop_owner_phone_number() {
    std::string phone_number;
//    std::regex phone_regex("^1(3\\d|47|5([0-3]|[5-9])|8(0|2|[5-9]))\\d{8}$");
    std::regex phone_regex("(.*)");
    while (true) {
        std::cout << "input your phone number: ";

        std::cin >> phone_number;

        if (!std::regex_match(phone_number, phone_regex)) {
            std::cout << "phone number is illegal!" << std::endl;
            continue;
        }

        break;
    }

    seller.shop_owner_phone_number = phone_number;
}

void RegisterSeller::register_shop_owner_id_number() {
//    std::regex id_number_regex("^([1-9]\\d{5}[12]\\d{3}(0[1-9]|1[012])(0[1-9]|[12][0-9]|3[01])\\d{3}[0-9xX])$");
    std::regex id_number_regex("(.*)");
    std::string id_number;

    while (true) {
        std::cout << "input your idCard number: ";

        std::cin >> id_number;

        if (!std::regex_match(id_number, id_number_regex)) {
            std::cout << "idCard is illegal!" << std::endl;
            continue;
        }

        break;
    }

    seller.shop_owner_id_number = id_number;
}

void RegisterSeller::register_pay_password() {
    std::string password;
    std::string confirm_password;

    while (true) {
        std::cout << "pay password: ";
        std::cin >> password;
        std::cout << "confirm pay password: ";
        std::cin >> confirm_password;

        // pay_password rule
        if (password == confirm_password)
            break;
    }
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