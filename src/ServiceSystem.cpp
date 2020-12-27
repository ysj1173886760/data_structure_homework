//
// Created by dbdxzwh on 2020/12/27.
//

#include "ServiceSystem.h"
#include <algorithm>

bool cmp(const std::pair<double, int>& p1, const std::pair<double, int>& p2) {
    return p1.first > p2.first;
}

std::string ServiceSystem::GetHighestSim(const std::vector<std::string>& v, const std::string& s) {
    BasicOperation op;
    std::vector<std::pair<double, int>> sim;
    for(int i=0; i<v.size(); i++) {
        std::pair<double, int> add;
        add.first = op.GetWordsSim(v[i], s);
        add.second = i;
        sim.push_back(add);
    }
    sort(sim.begin(), sim.end(), cmp);
    if(sim[0].first == 1)
        return v[sim[0].second];
    else {
        std::cout << "Did you mean " << v[sim[0].second] << "?" << std::endl;
        return v[sim[0].second];
    }
}

void ServiceSystem::display_shop(const std::string& shop_name) {
    BasicOperation op;
    DB &db = DB::getInstance();
    vector<SellerData> sellers = db.select_all_seller_data();

    int tar;
    for(tar=0; tar<sellers.size(); tar++) {
        if(sellers[tar].shop_name == shop_name)
            break;
    }

    vector<ItemData> items = op.GetShopItems(sellers[tar].id);
    for(int i=0; i<items.size(); i++) {
        std::cout << "name : " << items[i].name << std::endl;
        std::cout << "price : " << items[i].price << std::endl;
        std::cout << "left : " << items[i].store_num << std::endl;
        std::cout << "description : " << items[i].des << std::endl;
    }
}

void ServiceSystem::display_item(const std::string& item_name) {
    BasicOperation op;
    DB &db = DB::getInstance();
    vector<ItemData> items = db.select_all_item_data();
    for(int i=0; i<items.size(); i++) {
        if(op.GetWordsSim(items[i].name, item_name) > 0.8) {
            std::cout << "Shop : " << items[i].owner << std::endl;
            std::cout << "Name : " << items[i].name << std::endl;
            std::cout << "Price : " << items[i].price << std::endl;
        }
    }
}

void ServiceSystem::insert_shop_list(const std::string& user_id, const std::string& item_name, int num) {
    BasicOperation op;
    DB &db = DB::getInstance();
    UserData user = db.select_user_data(user_id);
    for(int i=0; i<num; i++)
        user.shop_list.push_back(item_name);
}





























