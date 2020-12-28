//
// Created by dbdxzwh on 2020/12/28.
//

#ifndef DATA_STRUCTURE_HOMEWORK_SELLERSYSTEM_H
#define DATA_STRUCTURE_HOMEWORK_SELLERSYSTEM_H

#include "DB.h"
#include "base.h"

class SellerSystem {
public:
    bool insert_item(const std::string&, ItemData&);
    bool remove_item(const std::string&, ItemData&);
    bool modify_item(const std::string&, ItemData&);
};


#endif //DATA_STRUCTURE_HOMEWORK_SELLERSYSTEM_H
