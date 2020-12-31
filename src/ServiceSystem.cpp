//
// Created by dbdxzwh on 2020/12/27.
//

#include "ServiceSystem.h"
#include <algorithm>

bool cmp(const std::pair<double, int>& p1, const std::pair<double, int>& p2) {
    return p1.first > p2.first;
}

void GetNext(std::string t, vector<int>& next)
{
    next[0] = -1;
    next[1] = 0;
    int k;
    for (int j = 2;t[j] != '\0';j++) {
        k=next[j-1];
        if (k == -1) {
            next[j]=0;
            continue;
        }
        else {
            while (t[j-1] != t[k] && k!=-1)
                k=next[k];
            if(t[j-1] == t[k])
                next[j]=k+1;
            else
                next[j] = 0;
        }
    }
}

bool KMP(std::string s, std::string t)
{
    vector<int> next(100);
    GetNext(t, next);

    int i=0,j=0;
    while (s[i]!='\0' && t[j] != '\0') {
        if (s[i] == t[j]) {
            i++;
            j++;
        }
        else {
            j = next[j];
            if (-1 == j){
                i++;
                j++;
            }
        }
    }
    if (t[j] == '\0')
        return true;
    else
        return false;
}

bool ServiceSystem::CmpSameLabel(const std::vector<std::string>& v, const std::string& s) {
    for(int i=0; i<v.size(); i++)
        if(v[i] == s)
            return true;
    return false;
}

vector<std::string> ServiceSystem::GetHighestSim(const std::vector<std::string>& v, const std::string& s) {
    BasicOperation op;
    std::vector<std::pair<double, int>> sim;
    for(int i=0; i<v.size(); i++) {
        std::pair<double, int> add;
        add.first = op.GetWordsSim(v[i], s);
        add.second = i;
        sim.push_back(add);
    }

    sort(sim.begin(), sim.end(), cmp);

    vector<std::string> sort_by_sim;
    for(int i=0; i<sim.size(); i++)
        sort_by_sim.push_back(v[sim[i].second]);

    return sort_by_sim;
}

vector<std::string> ServiceSystem::GetSamePart(const std::vector<std::string>& v, const std::string& s) {
    vector<std::string> sort_by_part;

    for(int i=0; i<v.size(); i++)
        if(KMP(v[i], s))
            sort_by_part.push_back(v[i]);

    return sort_by_part;
}

std::vector<ItemData> ServiceSystem::search_shop(const std::string& shop_name) {
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

std::vector<ItemData> ServiceSystem::search_item_sim(const std::string& item_name) {
    BasicOperation op;
    DB &db = DB::getInstance();
    vector<ItemData> items = db.select_all_item_data();
    vector<ItemData> ret_items;
    for(int i=0; i<items.size(); i++) {
        if(op.GetWordsSim(items[i].name, item_name) > 0.5)
            ret_items.push_back(items[i]);
    }
    return ret_items;
}

std::vector<ItemData> ServiceSystem::search_item_part(const std::string& item_name) {
    DB& db = DB::getInstance();

    vector<ItemData> ret_items;
    vector<ItemData> items = db.select_all_item_data();

    for(int i=0; i<items.size(); i++) {
        if(KMP(items[i].name, item_name))
            ret_items.push_back(items[i]);
    }

    return ret_items;
}

std::vector<ItemData> ServiceSystem::search_item_label(const std::string& label) {
    DB& db = DB::getInstance();

    vector<ItemData> ret_items;
    vector<ItemData> items = db.select_all_item_data();

    for(int i=0; i<items.size(); i++) {
        if(CmpSameLabel(items[i].label, label))
            ret_items.push_back(items[i]);
    }

    return ret_items;
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

//异常一：买的商品超过了已有的库存
//异常二：买的商品不存在于该商家中（异常在GetItem函数中处理）
bool ServiceSystem::insert_shop_list(const std::string& user_id, const std::string& shop_name, const std::string& item_name, int num) {
    BasicOperation op;
    DB &db = DB::getInstance();
    IDgenerator& generator = IDgenerator::get_instance();
    UserData user = db.select_user_data(user_id);

    ItemData item;
    if(op.GetItem(shop_name, item_name, item)) {
        int tar = item_in_shop_list(user_id, item.id);
        if(item.store_num < num) {
            //std::cout << "ERROR : under stock!!!" << std::endl;
            return false;
        }

        //如果所购买的物品已经处在购物车中，则增加数量即可
        else if(tar!=-1)
            user.shop_list[tar].buy_num += num;

        else {
            Order order;
            order.id = generator.generateID(Type::Order);
            order.item_id = item.id;
            order.price = item.price;
            order.buy_num = num;
            order.time = op.time2str();
            user.shop_list.push_back(order);
            db.modify_user_data(user.id, user);
        }
    }

    return true;
}

//异常三：删除的商品不存在与购物车中
bool ServiceSystem::remove_shop_list(const std::string& user_id, const std::string& shop_name, const std::string& item_name, int num) {
    BasicOperation op;
    DB &db = DB::getInstance();
    IDgenerator& generator = IDgenerator::get_instance();
    UserData user = db.select_user_data(user_id);

    ItemData item;
    if(op.GetItem(shop_name, item_name, item)) {
        int tar = item_in_shop_list(user_id, item.id);

        if(tar!=-1) {
            user.shop_list[tar].buy_num = user.shop_list[tar].buy_num - num;
            if(user.shop_list[tar].buy_num <= 0)
                user.shop_list.erase(user.shop_list.begin()+tar);
            db.modify_user_data(user_id, user);
        }

        else {
            return false;
            std::cout << "ERROR : you are removing sth doesn't in the shop_list!!!" << std::endl;
        }
    }

    return true;
}

//异常四：提交订单的花销超出自己钱包的money（在transfer中处理）
bool ServiceSystem::submit_shop_list(const std::string &user_id, const std::string& remark) {
    BasicOperation op;
    DB &db = DB::getInstance();
    IDgenerator& generator = IDgenerator::get_instance();
    UserData user = db.select_user_data(user_id);

    for(int i=0; i<user.shop_list.size(); i++) {
        std::string item_id = user.shop_list[i].item_id;
        ItemData item = db.select_item_data(item_id);
        std::string shop_name = item.owner;
        SellerData seller;
        op.GetSeller(shop_name, seller);

        double cost = user.shop_list[i].price * user.shop_list[i].buy_num;
        MoneySystem money_sys;
        if (money_sys.TransferMoney(user.id, seller.id, cost)) {

            user = db.select_user_data(user_id);
            seller = db.select_seller_data(seller.id);

            BuyItemRequestData add;
            add.user_id = user_id;
            add.buy_num = user.shop_list[i].buy_num;
            add.item_id = user.shop_list[i].item_id;
            add.time = user.shop_list[i].time;
            add.price = user.shop_list[i].price;
//            std::cout << "please input remark : ";
//            std::cin >> add.remark;
            add.id = generator.generateID(Type::BuyItemRequest);
            seller.buy_item_request_list.push_back(add);

            db.modify_seller_data(seller.id, seller);
            db.modify_user_data(user.id, user);

            MessageSystem message_sys;
            std::string user_info = "you have cost " + to_string(cost) + " in " + seller.shop_name;
            message_sys.SendMessage(user.id, user_info);

            db.modify_seller_data(seller.id, seller);
        }
    }

    user.shop_list.swap(user.current_order);

    return true;
    //感觉给商家留言没啥必要 先留着吧
    //std::string seller_info = "you have got " + to_string(cost);
    //seller.message.push_back(seller_info);
}

//显示一：购买清单为空显示 无购买请求
void ServiceSystem::deal_BuyItemRequest(const std::string &seller_id) {
    BasicOperation op;
    MessageSystem message_sys;
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
            if(money_sys.TransferMoney(seller.id, user.id, item.price)) {

                std::string info = "sorry, because under stock, your BuyItemRequest of " + item.name +
                                   " is rejected and we have repay you";
                message_sys.SendMessage(user.id, info);
                del++;
            }
        }

        else {
            item.store_num-=seller.buy_item_request_list[i].buy_num;
            std::string info = "your " + to_string(seller.buy_item_request_list[i].buy_num) + " "
                    + item.name + " have arrived, pls check";
            message_sys.SendMessage(user.id, info);
            db.modify_item_data(item.id, item);
        }
    }

    seller.buy_item_request_list.clear();
    db.modify_seller_data(seller.id, seller);
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

bool ServiceSystem::returnItem(const std::string& user_id, const Order& order) {
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
            if (money_sys.TransferMoney(seller.id, user.id, order.price*order.buy_num)) {
                item.store_num += order.buy_num;
                MessageSystem message_sys;
                message_sys.SendMessage(seller.id, info);
            }

            db.modify_item_data(item.id, item);
            return true;
        }
        return false;
    }
    else
        return false;
}

bool confirm(const std::string& user_id, const Order& order) {
    DB& db = DB::getInstance();
    std::string item_id = order.item_id;
    UserData user = db.select_user_data(user_id);

    int tar = item_in_current_list(user_id, item_id);

    if(tar!=-1) {
        user.current_order.erase(user.current_order.begin() + tar);
        user.history_order.push_back(order);
        return true;
    } else
        return false;
}