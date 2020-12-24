//
// Created by sheep on 2020/12/24.
//

// base data container
// set your own data container derived from this one

#ifndef DATA_STRUCTURE_HOMEWORK_BASE_H
#define DATA_STRUCTURE_HOMEWORK_BASE_H

#include <string>
#include <vector>

class Base {
public:
    std::string id;      // all items should contain one unique ID
};

// here`s an example

class ItemData: public Base {
public:
    std::string name;
    double price;
    std::string des;     // this is the description of a item
    int sell_num;

};

class UserData: public Base {
public:
    std::string account;
    std::string real_name;
    std::string user_name;
    std::string pass_word;
    std::string phone_number;
    std::string email;
    std::string id_number;
    //etc
};

#endif //DATA_STRUCTURE_HOMEWORK_BASE_H
