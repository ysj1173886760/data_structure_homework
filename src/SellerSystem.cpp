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
        db.open();
        add_item.owner = seller.shop_name;
        db.insert_item_data(add_item);
        db.close();
        return true;
    }
    return false;
}

bool SellerSystem::remove_item(const std::string& seller_id, ItemData& rm_item) {
    DB& db = DB::getInstance();
    BasicOperation op;
    SellerData seller = db.select_seller_data(seller_id);

    ItemData item;
    if(op.GetItem(seller.shop_name, rm_item.name, item)) {
        db.open();
        db.delete_item_data(rm_item.id);
        db.close();
        return true;
    }

    std::cout << "ERROR : not find remove target!!!";
    return false;
}

bool SellerSystem::modify_item(const std::string& seller_id, ItemData& mod_item) {
    DB& db = DB::getInstance();
    BasicOperation op;
    SellerData seller = db.select_seller_data(seller_id);

    ItemData item;
    if(op.GetItem(seller.shop_name, mod_item.name, item)) {
        item = mod_item;
        return true;
    }

    std::cout << "ERROR : not find remove target!!!";
    return false;
}