//
// Created by dbdxzwh on 2020/12/28.
//

#ifndef DATA_STRUCTURE_HOMEWORK_SELLERSYSTEM_H
#define DATA_STRUCTURE_HOMEWORK_SELLERSYSTEM_H

#include "DB.h"
#include "base.h"

class SellerSystem {
public:
    //传入商家的ID、添加的商品结构
    bool insert_item(const std::string&, ItemData&);
    //传入商家的ID、删除的商品名称
    bool remove_item(const std::string&, const std::string&);
    //传入商家的ID、要修改的商品结构、修改的信息（price or store_num or des）
    bool modify_item(const std::string&, ItemData&, const std::string&);
};


#endif //DATA_STRUCTURE_HOMEWORK_SELLERSYSTEM_H
