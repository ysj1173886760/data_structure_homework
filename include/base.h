//
// Created by sheep on 2020/12/24.
//

// base data container
// set your own data container derived from this one

#ifndef DATA_STRUCTURE_HOMEWORK_BASE_H
#define DATA_STRUCTURE_HOMEWORK_BASE_H

#include <string>
#include <vector>
#include <iostream>

class Base {
public:
    std::string id;      // all items should contain one unique ID
};

// here`s an example

class ItemData: public Base {
public:
    std::string owner;
    std::string name;
    double price;
    std::string des;     // this is the description of a item
    int sell_num;
    int store_num;

    std::vector<std::string> label;

};

class OrderList: public Base {
public:
    std::string item_id;
    std::string price;
    std::string time;
    int buy_num;

    void print() const {
        std::cout << "item_id: " << item_id << std::endl;
        std::cout << "price: " << price << std::endl;
        std::cout << "time: " << time << std::endl;
        std::cout << "buy_num: " << buy_num << std::endl;
    }
};

class UserData: public Base {
public:
    std::string account;
    std::string password;

    std::string real_name;
    std::string user_name;
    std::string phone_number;
    std::string email;
    std::string id_number;
    std::string age;
    std::string gender;

    std::vector<OrderList> history_order;
    std::vector<std::string> message;
    std::vector<std::string> shop_list;
    std::vector<std::string> history_item;
    //etc

    // print them out, easy to debug
    void print() const {
        std::cout << "account: " << account << std::endl;
        std::cout << "password: " << password << std::endl;
        std::cout << "real_name: " << real_name << std::endl;
        std::cout << "user_name: " << user_name << std::endl;
        std::cout << "phone_number: " << phone_number << std::endl;
        std::cout << "email: " << email << std::endl;
        std::cout << "id_number: " << id_number << std::endl;
        std::cout << "age: " << age << std::endl;
        std::cout << "gender: " << gender << std::endl;

        std::cout << "history_order_num: " << history_order.size() << std::endl;
        for (const auto &x: history_order) {
            x.print();
        }

        std::cout << "message_num: " << message.size() << std::endl;
        for (const auto &x: message) {
            std::cout << x << std::endl;
        }

        std::cout << "shop_list_num: " << shop_list.size() << std::endl;
        for (const auto &x: shop_list) {
            std::cout << x << std::endl;
        }

        std::cout << "history_item_num: " << history_item.size() << std::endl;
        for (const auto &x: history_item) {
            std::cout << x << std::endl;
        }
    }
};

class ManagerData: public Base {
public:
    std::string account;
    std::string password;
};

class QuestionData {
public:
    std::string question;
    std::string answer;
};

class ChangeItemRequestData: public Base {
    std::string item_id;
    std::string des;
    std::string time;
};

class BuyItemRequestData: public Base {
    std::string item_id;
    std::string remake;
    std::string user_id;
    std::string time;
};

class SellerData: public Base{
public:
    std::string account;
    std::string password;
    std::string shop_name;
    std::string shop_address;
    std::string shop_owner_name;
    std::string shop_owner_phone_number;
    std::string shop_owner_id_number;

    std::vector<std::string> message;
    std::vector<ChangeItemRequestData> change_item_request_list;
    std::vector<BuyItemRequestData> buy_item_request_list;
    std::vector<std::string> discount;
};

class RegisterRequestData: public Base {
    std::string account;
    std::string password;
    std::string shop_name;
    std::string shop_address;
    std::string shop_owner_name;
    std::string shop_owner_phone_number;
    std::string shop_owner_id_number;
};

// TODO: money system
#endif //DATA_STRUCTURE_HOMEWORK_BASE_H
