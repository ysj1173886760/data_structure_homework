//
// Created by sheep on 2020/12/24.
//

#ifndef DATA_STRUCTURE_HOMEWORK_DB_H
#define DATA_STRUCTURE_HOMEWORK_DB_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <assert.h>
#include <iostream>
#include "base.h"
#include "json.h"

class DB {
private:
    std::unordered_map<std::string, UserData> user_data;
    std::unordered_map<std::string, ItemData> item_data;

    bool parse_json_from_file(const std::string&, Json::Value&);
    bool save_json_to_file(const std::string&, const Json::Value&);

    void load_user_data(const std::string&);
    void load_item_data(const std::string&);

    void save_user_data(const std::string&);
    void save_item_data(const std::string&);

    DB() {}

public:
    DB (const DB&) = delete;
    DB& operator=(const DB&) = delete;

    static DB& getInstance() {
        static DB instance;
        return instance;
    }

    void open();
    void close();

    // pass all info you need to insert
    // return the new id if insert succeed
    // otherwise, return a empty string
    std::string insert_user_data(const UserData&);
    std::string insert_item_data(const ItemData&);

    // pass the data id you need to delete
    // whether the id exist or not
    // because the result is the same
    void delete_user_data(const std::string&);
    void delete_item_data(const std::string&);

    // get a vector contains all data you want
    // enjoy your self
    std::vector<UserData> select_all_user_data();
    std::vector<ItemData> select_all_item_data();

    // pass the id of the data you want to get
    // if the id don`t exist, you will get a data with empty id
    UserData select_user_data(const std::string&);
    ItemData select_item_data(const std::string&);

    // pass the id of the data you want to modify
    // remember you have to parse all data but not the data you want to modify
    // because the origin data are going to be overwritten by new one
    // so pls be careful
    void modify_user_data(const std::string&, const UserData&);
    void modify_item_data(const std::string&, const ItemData&);
};


#endif //DATA_STRUCTURE_HOMEWORK_DB_H
