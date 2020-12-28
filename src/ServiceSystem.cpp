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

std::vector<ItemData> ServiceSystem::display_shop(const std::string& shop_name) {
    BasicOperation op;
    DB &db = DB::getInstance();
    vector<SellerData> sellers = db.select_all_seller_data();

    int tar;
    for(tar=0; tar<sellers.size(); tar++) {
        if(sellers[tar].shop_name == shop_name)
            break;
    }

    vector<ItemData> items = op.GetShopItems(sellers[tar].id);
    return items;
}

std::vector<ItemData> ServiceSystem::display_item(const std::string& item_name) {
    BasicOperation op;
    DB &db = DB::getInstance();
    vector<ItemData> items = db.select_all_item_data();
    for(int i=0; i<items.size(); i++) {
        if(op.GetWordsSim(items[i].name, item_name) > 0.8)
            items.push_back(items[i]);
    }
    return items;
}

int item_in_shop_list(const std::string& user_id, const std::string& item_id) {
    BasicOperation op;
    DB &db = DB::getInstance();
    UserData user = db.select_user_data(user_id);

    for(int i=0; i<user.shop_list.size(); i++) {
        if(user.shop_list[i].item_id == item_id)
            return i;
    }
    return -1;
}

void ServiceSystem::insert_shop_list(const std::string& user_id, const std::string& shop_name, const std::string& item_name, int num) {
    BasicOperation op;
    DB &db = DB::getInstance();
    IDgenerator& generator = IDgenerator::get_instance();
    UserData user = db.select_user_data(user_id);

    ItemData item;
    if(op.GetItem(shop_name, item_name, item)) {
        int tar = item_in_shop_list(user_id, item.id);
        if(item.store_num < num)
            std::cout << "ERROR : under stock!!!";

        //如果所购买的物品已经处在购物车中，则增加数量即可
        else if(tar!=-1)
            user.shop_list[tar].buy_num += num;

        else {
            Order order;
            order.id = generator.GenerateID(Type::Order);
            order.item_id = item.id;
            order.price = item.price;
            order.buy_num = num;
            order.time = op.time2str();
            for(int i=0; i<num; i++)
                user.shop_list.push_back(order);
        }
    }
}

void ServiceSystem::remove_shop_list(const std::string& user_id, const std::string& shop_name, const std::string& item_name, int num) {
    BasicOperation op;
    DB &db = DB::getInstance();
    IDgenerator& generator = IDgenerator::get_instance();
    UserData user = db.select_user_data(user_id);

    ItemData item;
    if(op.GetItem(shop_name, item_name, item)) {
        int tar = item_in_shop_list(user_id, item.id);

        if(tar!=-1)
            user.shop_list[tar].buy_num = max(user.shop_list[tar].buy_num-num, 0);

        else
            std::cout << "ERROR : you are removing sth doesn't in the shop_list!!!" << std::endl;
    }
}

void ServiceSystem::submit_shop_list(const std::string &user_id, const std::string &shop_id) {
    BasicOperation op;
    DB &db = DB::getInstance();
    IDgenerator& generator = IDgenerator::get_instance();
    UserData user = db.select_user_data(user_id);
    SellerData seller = db.select_seller_data(shop_id);

    for(int i=0; i<user.shop_list.size(); i++) {
        BuyItemRequestData add;
        add.user_id = user_id;
        add.buy_num = user.shop_list[i].buy_num;
        add.item_id = user.shop_list[i].item_id;
        add.time = user.shop_list[i].time;
        add.price = user.shop_list[i].price;
        std::cout << "please input remark : ";
        std::cin >> add.remark;
        add.id = generator.GenerateID(Type::BuyItemRequest);
    }

    user.shop_list.swap(user.current_order);

    double cost = op.GetCost(user_id);
    if(user.wallet.money > cost) {
        std::cout << "ERROR : your money is not enough!!!";
        return;
    }

    MoneySystem money_sys;
    money_sys.TransferMoney(user.id, seller.id, cost);

    MessageSystem message_sys;
    std::string user_info = "you have cost " + to_string(cost) + " in " + seller.shop_name;
    message_sys.SendMessage(user_id, user_info);
    //感觉给商家留言没啥必要 先留着吧
    //std::string seller_info = "you have got " + to_string(cost);
    //seller.message.push_back(seller_info);
}

void ServiceSystem::deal_BuyItemRequest(const std::string &seller_id) {
    BasicOperation op;
    DB &db = DB::getInstance();
    SellerData seller = db.select_seller_data(seller_id);

    if(seller.buy_item_request_list.size() == 0) {
        std::cout << "NO BuyItemRequest";
        return;
    }

    UserData user = db.select_user_data(seller.buy_item_request_list[0].user_id);

    for(int i=0; i<seller.buy_item_request_list.size(); i++) {
        ItemData item = db.select_item_data(seller.buy_item_request_list[i].item_id);

        int del=0;
        //库存不足 需要退货
        if(item.store_num < seller.buy_item_request_list[i].buy_num) {
            user.current_order.erase(user.current_order.begin()+i-del);

            MoneySystem money_sys;
            money_sys.TransferMoney(user.id, seller.id, item.price);

            MessageSystem message_sys;
            std::string info = "sorry, because under stock, your BuyItemRequest of " + item.name +
                               " is rejected and we have repay you";
            message_sys.SendMessage(user.id, info);
            del++;
        }

        else
            item.store_num--;
    }
}

int item_in_current_list(const std::string& user_id, const std::string& item_id) {
    BasicOperation op;
    DB &db = DB::getInstance();
    UserData user = db.select_user_data(user_id);

    for(int i=0; i<user.current_order.size(); i++) {
        if(user.current_order[i].item_id == item_id) {
            user.current_order.erase(user.current_order.begin()+i);
            return i;
        }
    }
    return -1;
}

void ServiceSystem::returnItem(const std::string& user_id, const Order& order) {
    BasicOperation op;
    DB &db = DB::getInstance();
    UserData user = db.select_user_data(user_id);
    std::string item_id = order.item_id;

    int tar = item_in_current_list(user_id, item_id);

    if(tar!=-1) {
        std::cout << "pls write why you wanner return it : ";
        std::string info;
        std::cin >> info;

        ItemData item = db.select_item_data(item_id);
        SellerData seller;
        if(op.GetSeller(item.owner, seller)) {
            MoneySystem money_sys;
            money_sys.TransferMoney(seller.id, user.id, item.price);

            MessageSystem message_sys;
            message_sys.SendMessage(seller.id, info);
        }
    }
}