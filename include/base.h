//
// Created by sheep on 2020/12/24.
//

// base data container
// set your own data container derived from this one

#ifndef DATA_STRUCTURE_HOMEWORK_BASE_H
#define DATA_STRUCTURE_HOMEWORK_BASE_H

#include <string>
#include <utility>
#include <vector>
#include <iostream>

class Base {
public:
    std::string id;         // all items should contain one unique ID

    explicit Base(std::string _id = "") : id(std::move(_id)) {}
};

// here`s an example

class ItemData: public Base {
public:
    std::string owner;      //seller
    std::string name;       //item name
    double price;           //item price
    std::string des;        // this is the description of a item
    int sell_num;           //sell_num of this item
    int store_num;          //num of stored item of seller

    std::vector<std::string> label; //label of item

    using Base::Base;
    explicit ItemData(std::string _owner = "",
             std::string _name = "",
             std::string _des = "",
             int _sell_num = 0,
             int _store_num = 0,
             double _price = 0.0,
             std::string _id = ""):
             owner(std::move(_owner)),
             name(std::move(_name)),
             des(std::move(_des)),
             sell_num(_sell_num),
             store_num(_store_num),
             price(_price),
             Base(std::move(_id)) {
        label.clear();
    }

    void print() const {
        std::cout << "owner: " << owner << std::endl;
        std::cout << "price: " << price << std::endl;
        std::cout << "name: " << name << std::endl;
        std::cout << "des: " << des << std::endl;
        std::cout << "sell_num: " << sell_num << std::endl;
        std::cout << "store_num: " << store_num << std::endl;

        std::cout << "label_size: " << label.size() << std::endl;
        for (const auto &x : label) {
            std::cout << x << std::endl;
        }
    }
};

class Order: public Base {
public:
    std::string item_id;    //item id of the order
    double price;      //price of the order
    std::string time;       //time when you buy
    int buy_num;

    using Base::Base;
    explicit Order(std::string _item_id = "",
          std::string _time = "",
          double _price = 0.0,
          int _buy_num = 0,
          std::string _id = ""):
          item_id(std::move(_item_id)),
          time(std::move(_time)),
          price(_price),
          buy_num(_buy_num),
          Base(std::move(_id)) {}

    void print() const {
        std::cout << "item_id: " << item_id << std::endl;
        std::cout << "price: " << price << std::endl;
        std::cout << "time: " << time << std::endl;
        std::cout << "buy_num: " << buy_num << std::endl;
    }
};

class Wallet {
public:
    std::string password;
    double money;

    explicit Wallet(std::string _password = "",
           double _money = 0.0):
           password(std::move(_password)),
           money(_money) {}

    void print() const {
        std::cout << "password: " << password << std::endl;
        std::cout << "money: " << money << std::endl;
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

    std::vector<Order> history_order;
    std::vector<std::string> message;
    std::vector<Order> shop_list;
    std::vector<Order> current_order;
    std::vector<std::string> history_item;
    //etc
    Wallet wallet;

    using Base::Base;
    explicit UserData(std::string _account = "",
             std::string _password = "",
             std::string _real_name = "",
             std::string _user_name = "",
             std::string _phone_number = "",
             std::string _email = "",
             std::string _id_number = "",
             std::string _age = "",
             std::string _gender = "",
             std::string _id = ""):
             account(std::move(_account)),
             password(std::move(_password)),
             real_name(std::move(_real_name)),
             user_name(std::move(_user_name)),
             phone_number(std::move(_phone_number)),
             email(std::move(_email)),
             id_number(std::move(_id_number)),
             age(std::move(_age)),
             gender(std::move(_gender)),
             Base(std::move(_id)) {
        history_order.clear();
        message.clear();
        shop_list.clear();
        current_order.clear();
        history_item.clear();
    }
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
        for (const auto &x : shop_list) {
            x.print();
        }

        std::cout << "current_order: " << current_order.size() << std::endl;
        for (const auto &x : current_order) {
            x.print();
        }

        std::cout << "history_item_num: " << history_item.size() << std::endl;
        for (const auto &x: history_item) {
            std::cout << x << std::endl;
        }

        wallet.print();
    }
};

class ManagerData: public Base {
public:
    std::string account;
    std::string password;

    void print() const {
        std::cout << "account: " << account << std::endl;
        std::cout << "password: " << password << std::endl;
    }
};

class QuestionData {
public:
    std::string question;
    std::string answer;

    explicit QuestionData(std::string _question = "",
                 std::string _answer = ""):
                 question(std::move(_question)),
                 answer(std::move(_answer)) {}

    void print() const {
        std::cout << "question: " << question << std::endl;
        std::cout << "answer: " << answer << std::endl;
    }
};

class ChangeItemRequestData: public Base {  //退货
public:
    std::string item_id;
    std::string des;
    std::string time;
    std::string user_id;
    int buy_num;
    double price;

    using Base::Base;
    explicit ChangeItemRequestData(std::string _item_id = "",
                                   std::string _user_id = "",
                                   std::string _time = "",
                                   std::string _des = "",
                                   int _buy_num = 0,
                                   double _price = 0.0,
                                   std::string _id = ""):
                                   item_id(std::move(_item_id)),
                                   user_id(std::move(_user_id)),
                                   des(std::move(_des)),
                                   time(std::move(_time)),
                                   buy_num(_buy_num),
                                   price(_price),
                                   Base(std::move(_id)) {}

    void print() const {
        std::cout << "item_id: " << item_id << std::endl;
        std::cout << "des: " << des << std::endl;
        std::cout << "time: " << time << std::endl;
        std::cout << "user_id: " << user_id << std::endl;
        std::cout << "buy_num: " << buy_num << std::endl;
        std::cout << "price: " << price << std::endl;
    }
};

class BuyItemRequestData: public Base {
public:
    std::string item_id;
    std::string remark;
    std::string user_id;
    std::string time;
    int buy_num;
    double price;

    using Base::Base;
    explicit BuyItemRequestData(std::string _item_id = "",
                       std::string _remark = "",
                       std::string _user_id = "",
                       std::string _time = "",
                       int _buy_num = 0,
                       double _price = 0,
                       std::string _id = ""):
                       item_id(std::move(_item_id)),
                       remark(std::move(_remark)),
                       user_id(std::move(_user_id)),
                       time(std::move(_time)),
                       buy_num(_buy_num),
                       price(_price),
                       Base(std::move(_id)) {}

    void print() const {
        std::cout << "item_id: " << item_id << std::endl;
        std::cout << "remark: " << remark << std::endl;
        std::cout << "user_id: " << user_id << std::endl;
        std::cout << "time: " << time << std::endl;
        std::cout << "buy_num: " << buy_num << std::endl;
        std::cout << "price: " << price << std::endl;
    }
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
    Wallet wallet;

    std::vector<Order> history_order;
    std::vector<std::string> message;
    std::vector<ChangeItemRequestData> change_item_request_list;
    std::vector<BuyItemRequestData> buy_item_request_list;
    std::vector<std::string> discount;

    std::vector<QuestionData> question_list;

    using Base::Base;
    explicit SellerData(std::string _account = "",
               std::string _password = "",
               std::string _shop_name = "",
               std::string _shop_address = "",
               std::string _shop_owner_name = "",
               std::string _shop_owner_phone_number = "",
               std::string _shop_owner_id_number = "",
               std::string _id = "") :
               account(std::move(_account)),
               password(std::move(_password)),
               shop_name(std::move(_shop_name)),
               shop_address(std::move(_shop_address)),
               shop_owner_name(std::move(_shop_owner_name)),
               shop_owner_phone_number(std::move(_shop_owner_phone_number)),
               shop_owner_id_number(std::move(_shop_owner_id_number)),
               Base(std::move(_id)) {
        history_order.clear();
        message.clear();
        change_item_request_list.clear();
        buy_item_request_list.clear();
        discount.clear();
        question_list.clear();
    }

    void print() const {
        std::cout << "account: " << account << std::endl;
        std::cout << "password: " << password << std::endl;
        std::cout << "shop_name: " << shop_name << std::endl;
        std::cout << "shop_address: " << shop_address << std::endl;
        std::cout << "shop_owner_name: " << shop_owner_name << std::endl;
        std::cout << "shop_owner_phone_number: " << shop_owner_phone_number << std::endl;
        std::cout << "shop_owner_id_number: " << shop_owner_id_number << std::endl;

        std::cout << "history_order_num: " << history_order.size() << std::endl;
        for (const auto &x : history_order) {
            x.print();
        }

        std::cout << "message_num: " << message.size() << std::endl;
        for (const auto &x : message) {
            std::cout << x << std::endl;
        }

        std::cout << "change_item_request_list_num: " << change_item_request_list.size();
        for (const auto &x : change_item_request_list) {
            x.print();
        }

        std::cout << "buy_item_request_list_num: " << buy_item_request_list.size();
        for (const auto &x : buy_item_request_list) {
            x.print();
        }

        std::cout << "discount: " << discount.size() << std::endl;
        for (const auto &x : discount) {
            std::cout << x << std::endl;
        }

        std::cout << "question_list_num: " << question_list.size();
        for (const auto &x : question_list) {
            x.print();
        }

        wallet.print();
    }
};

// add public
class RegisterRequestData: public Base {
public:
    std::string account;
    std::string password;
    std::string shop_name;
    std::string shop_address;
    std::string shop_owner_name;
    std::string shop_owner_phone_number;
    std::string shop_owner_id_number;

    using Base::Base;
    explicit RegisterRequestData(std::string _account = "",
                                 std::string _password = "",
                                 std::string _shop_name = "",
                                 std::string _shop_address = "",
                                 std::string _shop_owner_name = "",
                                 std::string _shop_owner_phone_number = "",
                                 std::string _shop_owner_id_number = "",
                                 std::string _id = "") :
                                 account(std::move(_account)),
                                 password(std::move(_password)),
                                 shop_name(std::move(_shop_name)),
                                 shop_address(std::move(_shop_address)),
                                 shop_owner_id_number(std::move(_shop_owner_id_number)),
                                 shop_owner_name(std::move(_shop_owner_name)),
                                 shop_owner_phone_number(std::move(_shop_owner_phone_number)),
                                 Base(std::move(_id)) {}

    void print() const {
        std::cout << "account: " << account << std::endl;
//        std::cout << "password: " << password << std::endl;
        std::cout << "shop_name" << shop_name << std::endl;
        std::cout << "shop_address: " << shop_address << std::endl;
        std::cout << "shop_owner_name: " << shop_owner_name << std::endl;
        std::cout << "shop_owner_phone_number: " << shop_owner_phone_number << std::endl;
        std::cout << "shop_owner_id_number: " << shop_owner_id_number << std::endl;
    }
};

enum class Type {
    User = 1,
    Seller,
    Manager,
    Item,
    Order,
    ChangeItemRequest,
    BuyItemRequest,
    RegisterRequest
};

#endif //DATA_STRUCTURE_HOMEWORK_BASE_H
