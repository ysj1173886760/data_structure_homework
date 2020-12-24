//
// Created by sheep on 2020/12/24.
//

#ifndef DATA_STRUCTURE_HOMEWORK_DB_H
#define DATA_STRUCTURE_HOMEWORK_DB_H

#include <string>
#include <vector>
#include "base.h"
#include "json.h"

class DB {
private:
    std::vector<UserData> user_data;
    std::vector<ItemData> item_data;

    void load_user_data(std::string);
    void load_item_data(std::string);

    void save_user_data(std::string);
    void save_item_data(std::string);

public:
    void open();
    void close();

    void insert_user_data(UserData);
    void insert_item_data(ItemData);

    void delete_user_data(UserData);
    void delete_item_data(ItemData);

    std::vector<UserData> select_all_user_data();
    std::vector<ItemData> select_all_item_data();

    UserData select_user_data(std::string);
    ItemData select_item_data(std::string);

    void modify_user_data(std::string, UserData);
    void modify_item_data(std::string, ItemData);
};


#endif //DATA_STRUCTURE_HOMEWORK_DB_H
