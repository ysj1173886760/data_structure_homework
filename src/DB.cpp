//
// Created by sheep on 2020/12/24.
//

#include "DB.h"

void DB::open() {
    std::cout << "loading userdata..." << std::endl;
    load_user_data("../data/data.json");

    std::cout << "data load complete" << std::endl;
}

void DB::close() {
    std::cout << "saving userdata..." << std::endl;
    save_user_data("../data/data.json");

    std::cout << "data save complete" << std::endl;
}

bool DB::parse_json_from_file(const std::string &filename, Json::Value &root) {
    std::ifstream ifs;
    ifs.open(filename);

    if (!ifs.is_open()) {
        std::cout << "error opening file: " << filename << std::endl;
        return false;
    }

    std::string line, json_str;
    while (getline(ifs, line)) {
        json_str.append(line);
    }

    ifs.close();

    Json::CharReaderBuilder char_reader_builder;
    std::unique_ptr<Json::CharReader> const json_reader(char_reader_builder.newCharReader());

    root.clear();

    bool res;
    JSONCPP_STRING errors;

    res = json_reader->parse(json_str.c_str(), json_str.c_str() + json_str.length(),
                             &root, &errors);

    if (!res || !errors.empty()) {
        std::cout << "parsing error" << errors << std::endl;
        return false;
    }

    return true;
}

bool DB::save_json_to_file(const std::string &filename, const Json::Value &root) {
    std::ofstream ofs;
    ofs.open(filename);

    if (!ofs.is_open()) {
        std::cout << "error opening file: " << filename << std::endl;
        return false;
    }

    Json::StreamWriterBuilder writer_builder;
    std::unique_ptr<Json::StreamWriter> json_writer(writer_builder.newStreamWriter());
    json_writer->write(root, &ofs);
    ofs.close();

    return true;
}

// this is about user data

void DB::load_user_data(const std::string &filename) {
    Json::Value root;

    if (!parse_json_from_file(filename, root)) {
        return;
    }

    for (auto it = root.begin(); it != root.end(); it++) {
        Json::Value temp = *it;
        std::string id = temp["id"].asString();
        UserData new_user;
        new_user.id = id;
        new_user.account = temp["account"].asString();
        new_user.password = temp["password"].asString();
        new_user.real_name = temp["real_name"].asString();
        new_user.user_name = temp["user_name"].asString();
        new_user.email = temp["email"].asString();
        new_user.id_number = temp["id_number"].asString();
        new_user.age = temp["age"].asString();
        new_user.gender = temp["gender"].asString();
        std::vector<OrderList> history_order;
        for (int i = 0; i < temp["history_order"].size(); i++) {
            OrderList newOrder;
            newOrder.item_id = temp["history_order"][i]["item_id"].asString();
            newOrder.price = temp["history_order"][i]["price"].asString();
            newOrder.time = temp["history_order"][i]["time"].asString();
            newOrder.buy_num = temp["history_order"][i]["buy_num"].asInt();
            history_order.emplace_back(newOrder);
        }
        new_user.history_order = history_order;
        for (int i = 0; i < temp["message"].size(); i++) {
            new_user.message.emplace_back(temp["message"][i].asString());
        }
        for (int i = 0; i < temp["shop_list"].size(); i++) {
            new_user.shop_list.emplace_back(temp["shop_list"][i].asString());
        }
        for (int i = 0; i < temp["history_item"].size(); i++) {
            new_user.history_item.emplace_back(temp["history_item"][i].asString());
        }
        this->user_data[id] = new_user;
    }
}

void DB::save_user_data(const std::string &filename) {
    Json::Value root;

    Json::Value new_user_data;
    for (const auto &[id, value]: this->user_data) {
        new_user_data.clear();
        new_user_data["id"] = value.id;
        new_user_data["account"] = value.account;
        new_user_data["password"] = value.password;
        new_user_data["real_name"] = value.real_name;
        new_user_data["user_name"] = value.user_name;
        new_user_data["email"] = value.email;
        new_user_data["id_number"] = value.id_number;
        new_user_data["age"] = value.age;
        new_user_data["gender"] = value.gender;

        Json::Value history_order;
        history_order.clear();
        for (const auto &x: value.history_order) {
            Json::Value order;
            order.clear();
            order["item_id"] = x.item_id;
            order["price"] = x.price;
            order["time"] = x.time;
            order["buy_num"] = x.buy_num;
            history_order.append(order);
        }
        new_user_data["history_order"] = history_order;

        Json::Value message;
        message.clear();
        for (int i = 0; i < value.message.size(); i++) {
            message[i] = value.message[i];
        }
        new_user_data["message"] = message;

        Json::Value shop_list;
        shop_list.clear();
        for (int i = 0; i < value.shop_list.size(); i++) {
            shop_list[i] = value.shop_list[i];
        }
        new_user_data["shop_list"] = shop_list;

        Json::Value history_item;
        history_item.clear();
        for (int i = 0; i < value.history_item.size(); i++) {
            history_item[i] = value.history_item[i];
        }
        new_user_data["history_item"] = history_item;

        root.append(new_user_data);
    }

    if (!save_json_to_file(filename, root)) {
        return;
    }
}

std::vector<UserData> DB::select_all_user_data() {
    std::vector<UserData> return_value;
    for (const auto &[id, value]: this->user_data) {
        return_value.push_back(value);
    }
    return return_value;
}

std::string DB::insert_user_data(const UserData& inserted_user_data) {
    // TODO: id generator
    UserData new_user_data = inserted_user_data;
    this->user_data[new_user_data.id] = new_user_data;
    return new_user_data.id;
}

void DB::delete_user_data(const std::string &id) {
    if (this->user_data.count(id)) {
        this->user_data.erase(id);
    }
}

UserData DB::select_user_data(const std::string &id) {
    UserData return_value;
    if (this->user_data.count(id)) {
        return_value = user_data[id];
    }
    return return_value;
}

// user data end

// this is item data

void DB::load_item_data(const std::string &filename) {
    Json::Value root;

    if (!parse_json_from_file(filename, root)) {
        return;
    }

    for (auto it = root.begin(); it != root.end(); it++) {
        Json::Value temp = *it;
        std::string id = temp["id"].asString();
        ItemData new_item;
        new_item.owner = temp["owner"].asString();
        new_item.name = temp["name"].asString();
        new_item.price = temp["price"].asDouble();
        new_item.des = temp["des"].asString();
        new_item.sell_num = temp["sell_num"].asInt();
        new_item.store_num = temp["store_num"].asInt();

        for (int i = 0; i < temp["label"].size(); i++) {
            new_item.label.emplace_back(temp["label"][i].asString());
        }

        this->item_data[id] = new_item;
    }
}

void DB::save_item_data(const std::string &filename) {
    Json::Value root;

    Json::Value new_item_data;
    for (const auto &[id, value]: this->item_data) {
        new_item_data.clear();

        new_item_data["id"] = value.id;
        new_item_data["owner"] = value.owner;
        new_item_data["name"] = value.name;
        new_item_data["price"] = value.price;
        new_item_data["des"] = value.des;
        new_item_data["sell_num"] = value.sell_num;
        new_item_data["store_num"] = value.store_num;

        Json::Value label;
        label.clear();
        for (int i = 0; i < value.label.size(); i++) {
            label[i] = value.label[i];
        }
        new_item_data["label"] = label;

        root.append(new_item_data);
    }

    if (!save_json_to_file(filename, root)) {
        return;
    }
}

std::vector<ItemData> DB::select_all_item_data() {
    std::vector<ItemData> return_value;
    for (const auto &[id, value]: this->item_data) {
        return_value.push_back(value);
    }
    return return_value;
}

std::string DB::insert_item_data(const ItemData &inserted_user_data) {
    ItemData new_item_data = inserted_user_data;
    this->item_data[new_item_data.id] = new_item_data;
    return new_item_data.id;
}

void DB::delete_item_data(const std::string &id) {
    if (this->item_data.count(id)) {
        this->item_data.erase(id);
    }
}

ItemData DB::select_item_data(const std::string &id) {
    ItemData return_value;
    if (this->item_data.count(id)) {
        return_value = item_data[id];
    }
    return return_value;
}

// item data end

// manager data stuff

void DB::load_manager_data(const std::string &filename) {
    Json::Value root;

    if (!parse_json_from_file(filename, root)) {
        return;
    }

    for (auto it = root.begin(); it != root.end(); it++) {
        Json::Value temp = *it;
        std::string id = temp["id"].asString();
        ManagerData new_manager;
        new_manager.account = temp["account"].asString();
        new_manager.password = temp["password"].asString();

        this->manager_data[id] = new_manager;
    }
}

void DB::save_manager_data(const std::string &filename) {
    Json::Value root;

    Json::Value new_manager_data;
    for (const auto &[id, value]: this->manager_data) {
        new_manager_data.clear();

        new_manager_data["account"] = value.account;
        new_manager_data["password"] = value.password;
        new_manager_data["id"] = value.id;

        root.append(new_manager_data);
    }

    if (!save_json_to_file(filename, root)) {
        return;
    }
}

std::vector<ManagerData> DB::select_all_manager_data() {
    std::vector<ManagerData> return_value;
    for (const auto &[id, value]: this->manager_data) {
        return_value.push_back(value);
    }
    return return_value;
}

std::string DB::insert_manager_data(const ManagerData &inserted_manager_data) {
    ManagerData new_manager_data;
    this->manager_data[new_manager_data.id] = new_manager_data;
    return new_manager_data.id;
}

void DB::delete_manager_data(const std::string &id) {
    if (this->manager_data.count(id)) {
        this->manager_data.erase(id);
    }
}

ManagerData DB::select_manager_data(const std::string &id) {
    ManagerData return_value;
    if (this->manager_data.count(id)) {
        return_value = manager_data[id];
    }
    return return_value;
}

// manager data done
