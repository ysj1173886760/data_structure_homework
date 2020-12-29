//
// Created by dbdxzwh on 2020/12/28.
//

#include "SellerSystem.h"

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

bool SellerSystem::remove_item(const std::string& seller_id, const std::string& rm_name) {
    DB& db = DB::getInstance();
    BasicOperation op;
    SellerData seller = db.select_seller_data(seller_id);

    ItemData item;
    if(op.GetItem(seller.shop_name, rm_name, item)) {
        db.delete_item_data(item.id);
        return true;
    }

    std::cout << "ERROR : not find remove target!!!" << std::endl;
    return false;
}

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

    std::cout << "ERROR : not find remove target!!!" << std::endl;
    return false;
}