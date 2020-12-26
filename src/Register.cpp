//
// Created by shrimp on 2020/12/25.
//

#include <string>
#include <regex>
#include "Register.h"

Register::Register() {
    DB& db = DB::getInstance();
    db.open();
    user_data = db.select_all_user_data();
    std::cout << "Register()" << std::endl;
    std::cout << user_data.size() << std::endl;
    db.close();
}

Register::~Register() {

}

std::vector<UserData> Register::register_get_user_data() {
    return user_data;
}

void Register::register_account(UserData &a) {
    std::string account;

    while (true) {
        std::cout << "input your account: " << std::endl;

        std::cin >> account;

        bool check = false;
        for (auto it : user_data) {
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

    a.account = account;
}

void Register::register_password(UserData &p) {
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

    p.password = password;
}

void Register::register_user_name(UserData &Name) {
    std::string user_name;

    while (true) {
        std::cout << "user_name" << std::endl;
        std::cin >> user_name;

        for (auto it : user_data) {
            if (it.user_name == user_name) {
                std::cout << "check user_name" << std::endl;
                continue;
            }
        }

        break;
    }

    Name.user_name = user_name;
}

//
void Register::register_phone_number(UserData &phone) {
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

    phone.phone_number = phone_number;
}

void Register::register_real_name(UserData &Name) {
    std::string real_name;

    std::cout << "real_name" << std::endl;
    std::cin >> real_name;

    Name.real_name = real_name;
}

//
void Register::register_id_number(UserData &ID) {
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

    ID.id_number = id_number;
}

void Register::register_email(UserData &E) {
    std::string email;
    //
    std::regex email_regex("(.*)(@qq.com|@gmail.com)");

    while (true) {
        std::cout << "email" << std::endl;
        std::cin >> email;

        if (std::regex_match(email, email_regex))
            break;
        else
            std::cout << "check email" << std::endl;
    }

    E.email = email;
}

Register_User::Register_User() {
//    user_data = register_get_user_data();
}

void Register_User::Register_Account() {

    register_account(data);
    register_password(data);
    register_user_name(data);
    register_phone_number(data);
    register_real_name(data);
    register_id_number(data);
    register_email(data);

}


// seller need to apply to the manager
void Register_Seller::Register_Account() {

    register_account(user);
    register_password(user);
    register_phone_number(user);
    register_real_name(user);
    register_id_number(user);
    register_email(user);

    copy_information();

    register_shop_name();
    register_shop_address();


}

// TODO: 应该添加店铺名字查重
void Register_Seller::register_shop_name() {
    std::string name;

    std::cout << "shop_name" << std::endl;
    std::cin >> name;

    seller_request.shop_name = name;
}

void Register_Seller::register_shop_address() {
    std::string address;

    std::cout << "address" << std::endl;
    std::cin >> address;

    seller_request.shop_address = address;
}

void Register_Seller::copy_information() {
    seller_request.account = user.account;
    seller_request.password = user.password;
    seller_request.shop_owner_phone_number = user.phone_number;
    seller_request.shop_owner_name = user.real_name;
    seller_request.shop_owner_id_number = user.id_number;
}

void Register_Seller::print() {
    std::cout << "account:" << seller_request.account << std::endl;
    std::cout << "password:" << seller_request.password << std::endl;
    std::cout << "owner_name:" << seller_request.shop_owner_name << std::endl;
    std::cout << "phone:" << seller_request.shop_owner_phone_number << std::endl;
    std::cout << "id_number:" << seller_request.shop_owner_id_number << std::endl;
    std::cout << "shop_name:" << seller_request.shop_name << std::endl;
    std::cout << "address:" << seller_request.shop_address << std::endl;
    std::cout << "id:" << seller_request.id << std::endl;
}