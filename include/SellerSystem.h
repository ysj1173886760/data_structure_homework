//
// Created by dbdxzwh on 2020/12/28.
//

#ifndef DATA_STRUCTURE_HOMEWORK_SELLERSYSTEM_H
#define DATA_STRUCTURE_HOMEWORK_SELLERSYSTEM_H

#include "DB.h"
#include "base.h"

class SellerSystem {
public:
    // id item
    bool insert_item(const std::string&, ItemData&);
    bool remove_item(const std::string&, const std::string&);
    // price store_num des
    bool modify_item(const std::string&, ItemData&, const std::string&);
};


#endif //DATA_STRUCTURE_HOMEWORK_SELLERSYSTEM_H
