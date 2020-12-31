//
// Created by dbdxzwh on 2020/12/28.
//

#include "SellerSystem.h"

//异常一：插入已有的商品返回false（此时应该选择修改操作）
bool SellerSystem::insert_item(const std::string& seller_id, ItemData& add_item) {
    DB& db = DB::getInstance();
    BasicOperation op;
    SellerData seller = db.select_seller_data(seller_id);

    ItemData item;
    if(!op.GetItem(seller.shop_name, add_item.name, item)) {

        add_item.owner = seller.shop_name;
        db.insert_item_data(add_item);
        return true;
    }
    return false;
}

//异常二：删除不存在的商品
bool SellerSystem::remove_item(const std::string& seller_id, const std::string& rm_name) {
    DB& db = DB::getInstance();
    BasicOperation op;
    SellerData seller = db.select_seller_data(seller_id);

    ItemData item;
    if(op.GetItem(seller.shop_name, rm_name, item)) {
        //db.open();
        db.delete_item_data(item.id);
        //db.close();
        return true;
    }

    //std::cout << "ERROR : not find remove target!!!" << std::endl;
    return false;
}

//异常三：修改不存在的商品
bool SellerSystem::modify_item(const std::string& seller_id, ItemData& mod_item, const std::string& mod_info) {
    DB& db = DB::getInstance();
    BasicOperation op;
    SellerData seller = db.select_seller_data(seller_id);

    ItemData item;
    if(op.GetItem(seller.shop_name, mod_item.name, item)) {

        if(mod_info == "price")
            item.price = mod_item.price;
        else if(mod_info == "store_num")
            item.store_num = mod_item.store_num;
        else
            item.des = mod_item.des;
        db.modify_item_data(item.id, item);

        return true;
    }

    //std::cout << "ERROR : not find remove target!!!" << std::endl;
    return false;
}