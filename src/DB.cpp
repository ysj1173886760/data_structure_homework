//
// Created by sheep on 2020/12/24.
//

#include "DB.h"

void DB::load_user_data(std::string filename) {
    std::ifstream ifs;
    ifs.open(filename);

    // TODO: assert is slow, find a way to change it
    assert(ifs.is_open());

    Json::Reader reader;
    Json::Value root;

    root.clear();

    if (!reader.parse(ifs, root, false))
    {
        std::cout << "reader parse error: " << strerror(errno) << std::endl;
        return;
    }

    for (auto it = root.begin(); it != root.end(); it++) {
        Json::Value temp = *it;
        std::string id = temp["id"].asString();
        UserData newUser;
        newUser.id = id;
        newUser.account = temp["account"].asString();
        newUser.password = temp["password"].asString();
        newUser.real_name = temp["real_name"].asString();
        newUser.user_name = temp["user_name"].asString();
        newUser.email = temp["email"].asString();
        newUser.id_number = temp["id_number"].asString();
        newUser.age = temp["age"].asString();
        newUser.gender = temp["gender"].asString();
        std::vector<OrderList> history_order;
        for (int i = 0; i < temp["history_order"].size(); i++) {
            OrderList newOrder;
            newOrder.item_id = temp["history_order"][i]["item_id"].asString();
            newOrder.price = temp["history_order"][i]["price"].asString();
            newOrder.time = temp["history_order"][i]["time"].asString();
            newOrder.buy_num = temp["history_order"][i]["buy_num"].asInt();
            history_order.emplace_back(newOrder);
        }
        newUser.history_order = history_order;
        for (int i = 0; i < temp["message"].size(); i++) {
            newUser.message.emplace_back(temp["message"][i].asString());
        }
        for (int i = 0; i < temp["shop_list"].size(); i++) {
            newUser.shop_list.emplace_back(temp["shop_list"][i].asString());
        }
        for (int i = 0; i < temp["history_item"].size(); i++) {
            newUser.history_item.emplace_back(temp["history_item"][i].asString());
        }
        this->user_data[id] = newUser;
    }
    ifs.close();
}