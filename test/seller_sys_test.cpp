//
// Created by dbdxzwh on 2020/12/28.
//

#include "SellerSystem.h"
#include "IDgenerator.h"

int main() {
    BasicOperation op;
    DB& db = DB::getInstance();
    db.open();
    IDgenerator& gen = IDgenerator::get_instance();
    std::vector<SellerData> sellers = db.select_all_seller_data();

    if(sellers.size() == 0)
        return 0;

    ItemData item;
    item.name = "add1";
    item.price = 10;
    item.store_num = 10;
    item.des = "no des";
    item.sell_num = 5;
    item.id = gen.generateID(Type::Item);

    SellerData seller = sellers[0];
    SellerSystem sel_sys;
    sel_sys.insert_item(seller.id, item);
    sel_sys.remove_item(seller.id, item);
    sel_sys.modify_item(seller.id, item);



}