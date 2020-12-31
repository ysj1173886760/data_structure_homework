//
// Created by sheep on 2020/12/24.
//

#include "DB.h"

void DB::open() {
//    std::cout << "loading userData..." << std::endl;
    load_user_data("../data/user_data.json");

//    std::cout << "loading itemData..." << std::endl;
    load_item_data("../data/item_data.json");

//    std::cout << "loading managerData..." << std::endl;
    load_manager_data("../data/manager_data.json");

//    std::cout << "loading sellerData..." << std::endl;
    load_seller_data("../data/seller_data.json");

//    std::cout << "loading registerRequestData..." << std::endl;
    load_register_request_data("../data/register_request_data.json");

//    std::cout << "data load complete" << std::endl;
}

void DB::close() {
//    std::cout << "saving userData..." << std::endl;
    save_user_data("../data/user_data.json");

//    std::cout << "saving itemData..." << std::endl;
    save_item_data("../data/item_data.json");

//    std::cout << "saving managerData..." << std::endl;
    save_manager_data("../data/manager_data.json");

//    std::cout << "saving sellerData..." << std::endl;
    save_seller_data("../data/seller_data.json");

//    std::cout << "saving registerRequestData..." << std::endl;
    save_register_request_data("../data/register_request_data.json");

//    std::cout << "data save complete" << std::endl;
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
        new_user.phone_number = temp["phone_number"].asString();
        new_user.email = temp["email"].asString();
        new_user.id_number = temp["id_number"].asString();
        new_user.age = temp["age"].asString();
        new_user.gender = temp["gender"].asString();

        std::vector<Order> history_order;
        for (int i = 0; i < temp["history_order"].size(); i++) {
            Order new_order;
            new_order.id = temp["history_order"][i]["id"].asString();
            new_order.item_id = temp["history_order"][i]["item_id"].asString();
            new_order.price = temp["history_order"][i]["price"].asDouble();
            new_order.time = temp["history_order"][i]["time"].asString();
            new_order.buy_num = temp["history_order"][i]["buy_num"].asInt();
            history_order.emplace_back(new_order);
        }
        new_user.history_order = history_order;

        for (int i = 0; i < temp["message"].size(); i++) {
            new_user.message.emplace_back(temp["message"][i].asString());
        }

        std::vector<Order> shop_list;
        for (int i = 0; i < temp["shop_list"].size(); i++) {
            Order new_order;
            new_order.id = temp["shop_list"][i]["id"].asString();
            new_order.item_id = temp["shop_list"][i]["item_id"].asString();
            new_order.price = temp["shop_list"][i]["price"].asDouble();
            new_order.time = temp["shop_list"][i]["time"].asString();
            new_order.buy_num = temp["shop_list"][i]["buy_num"].asInt();
            shop_list.emplace_back(new_order);
        }
        new_user.shop_list = shop_list;

        std::vector<Order> current_order;
        for (int i = 0; i < temp["current_order"].size(); i++) {
            Order new_order;
            new_order.id = temp["current_order"][i]["id"].asString();
            new_order.item_id = temp["current_order"][i]["item_id"].asString();
            new_order.price = temp["current_order"][i]["price"].asDouble();
            new_order.time = temp["current_order"][i]["time"].asString();
            new_order.buy_num = temp["current_order"][i]["buy_num"].asInt();
            current_order.emplace_back(new_order);
        }
        new_user.current_order = current_order;

        Wallet wallet;
        wallet.password = temp["wallet"]["password"].asString();
        wallet.money = temp["wallet"]["money"].asDouble();
        new_user.wallet = wallet;

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
        new_user_data["phone_number"] = value.phone_number;

        Json::Value history_order;
        history_order.clear();
        for (const auto &x: value.history_order) {
            Json::Value order;
            order.clear();
            order["id"] = x.id;
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
        for (const auto &x: value.shop_list) {
            Json::Value order;
            order.clear();
            order["id"] = x.id;
            order["item_id"] = x.item_id;
            order["price"] = x.price;
            order["time"] = x.time;
            order["buy_num"] = x.buy_num;
            shop_list.append(order);
        }
        new_user_data["shop_list"] = shop_list;

        Json::Value current_order;
        current_order.clear();
        for (const auto &x: value.current_order) {
            Json::Value order;
            order.clear();
            order["id"] = x.id;
            order["item_id"] = x.item_id;
            order["price"] = x.price;
            order["time"] = x.time;
            order["buy_num"] = x.buy_num;
            current_order.append(order);
        }
        new_user_data["current_order"] = current_order;

        Json::Value history_item;
        history_item.clear();
        for (int i = 0; i < value.history_item.size(); i++) {
            history_item[i] = value.history_item[i];
        }
        new_user_data["history_item"] = history_item;

        Json::Value wallet;
        wallet.clear();
        wallet["password"] = value.wallet.password;
        wallet["money"] = value.wallet.money;
        new_user_data["wallet"] = wallet;

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
    UserData new_user_data = inserted_user_data;
    IDgenerator &gen = IDgenerator::get_instance();
    new_user_data.id = gen.generateID(Type::User);
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

void DB::modify_user_data(const std::string &id, const UserData &modified_user_data) {
    this->user_data[id] = modified_user_data;
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
        new_item.id = id;
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
    IDgenerator &gen = IDgenerator::get_instance();
    new_item_data.id = gen.generateID(Type::Item);
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

void DB::modify_item_data(const std::string &id, const ItemData &modified_item_data) {
    this->item_data[id] = modified_item_data;
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
        new_manager.id = id;
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
    ManagerData new_manager_data = inserted_manager_data;
    IDgenerator &gen = IDgenerator::get_instance();
    new_manager_data.id = gen.generateID(Type::Manager);
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

void DB::modify_manager_data(const std::string &id, const ManagerData &modified_manager_data) {
    this->manager_data[id] = modified_manager_data;
}

// manager data done

// change item request data stuff
//
//void DB::load_change_item_request_data(const std::string &filename) {
//    Json::Value root;
//
//    if (!parse_json_from_file(filename, root)) {
//        return;
//    }
//
//    for (auto it = root.begin(); it != root.end(); it++) {
//        Json::Value temp = *it;
//        std::string id = temp["id"].asString();
//
//        ChangeItemRequestData new_change_item_request_data;
//        new_change_item_request_data.id = id;
//        new_change_item_request_data.des = temp["des"].asString();
//        new_change_item_request_data.item_id = temp["item_id"].asString();
//        new_change_item_request_data.time = temp["time"].asString();
//
//        this->change_item_request_data[id] = new_change_item_request_data;
//    }
//}
//
//void DB::save_change_item_request_data(const std::string &filename) {
//    Json::Value root;
//
//    Json::Value new_change_item_request_data;
//    for (const auto &[id, value]: this->change_item_request_data) {
//        new_change_item_request_data.clear();
//
//        new_change_item_request_data["item_id"] = value.item_id;
//        new_change_item_request_data["des"] = value.des;
//        new_change_item_request_data["time"] = value.time;
//        new_change_item_request_data["id"] = value.id;
//
//        root.append(new_change_item_request_data);
//    }
//
//    if (!save_json_to_file(filename, root)) {
//        return;
//    }
//}
//
//std::vector<ChangeItemRequestData> DB::select_all_change_item_request_data() {
//    std::vector<ChangeItemRequestData> return_value;
//    for (const auto &[id, value]: this->change_item_request_data) {
//        return_value.push_back(value);
//    }
//    return return_value;
//}
//
//std::string DB::insert_change_item_request_data(const ChangeItemRequestData &inserted_change_item_request_data) {
//    ChangeItemRequestData new_change_item_request_data = inserted_change_item_request_data;
//    this->change_item_request_data[new_change_item_request_data.id] = inserted_change_item_request_data;
//    return new_change_item_request_data.id;
//}
//
//void DB::delete_change_item_request_data(const std::string &id) {
//    if (this->change_item_request_data.count(id)) {
//        this->change_item_request_data.erase(id);
//    }
//}
//
//ChangeItemRequestData DB::select_change_item_request_data(const std::string &id) {
//    ChangeItemRequestData return_value;
//    if (this->change_item_request_data.count(id)) {
//        return_value = change_item_request_data[id];
//    }
//    return return_value;
//}
//
//void DB::modify_change_item_request_data(const std::string &id, const ChangeItemRequestData &modified_change_item_request_data) {
//    this->change_item_request_data[id] = modified_change_item_request_data;
//}
//
// change item request data done

// buy item request data stuff
//
//void DB::load_buy_item_request_data(const std::string &filename){
//    Json::Value root;
//
//    if (!parse_json_from_file(filename, root)) {
//        return;
//    }
//
//    for (auto it = root.begin(); it != root.end(); it++) {
//        Json::Value temp = *it;
//        std::string id = temp["id"].asString();
//
//        BuyItemRequestData new_buy_item_request_data;
//        new_buy_item_request_data.id = id;
//        new_buy_item_request_data.item_id = temp["item_id"].asString();
//        new_buy_item_request_data.remark= temp["remark"].asString();
//        new_buy_item_request_data.user_id = temp["user_id"].asString();
//        new_buy_item_request_data.time = temp["time"].asString();
//
//        this->buy_item_request_data[id] = new_buy_item_request_data;
//    }
//}
//
//void DB::save_buy_item_request_data(const std::string &filename) {
//    Json::Value root;
//
//    Json::Value new_buy_item_request_data;
//    for (const auto &[id, value]: this->buy_item_request_data) {
//        new_buy_item_request_data.clear();
//
//        new_buy_item_request_data["item_id"] = value.item_id;
//        new_buy_item_request_data["remark"] = value.remark;
//        new_buy_item_request_data["time"] = value.time;
//        new_buy_item_request_data["id"] = value.id;
//        new_buy_item_request_data["user_id"] = value.user_id;
//
//        root.append(new_buy_item_request_data);
//    }
//
//    if (!save_json_to_file(filename, root)) {
//        return;
//    }
//}
//
//std::vector<BuyItemRequestData> DB::select_all_buy_item_request_data() {
//    std::vector<BuyItemRequestData> return_value;
//    for (const auto &[id, value]: this->buy_item_request_data) {
//        return_value.push_back(value);
//    }
//    return return_value;
//}
//
//std::string DB::insert_buy_item_request_data(const BuyItemRequestData &inserted_buy_item_request_data) {
//    BuyItemRequestData new_buy_item_request_data = inserted_buy_item_request_data;
//    this->buy_item_request_data[new_buy_item_request_data.id] = inserted_buy_item_request_data;
//    return new_buy_item_request_data.id;
//}
//
//void DB::delete_buy_item_request_data(const std::string &id) {
//    if (this->buy_item_request_data.count(id)) {
//        this->buy_item_request_data.erase(id);
//    }
//}
//
//BuyItemRequestData DB::select_buy_item_request_data(const std::string &id) {
//    BuyItemRequestData return_value;
//    if (this->buy_item_request_data.count(id)) {
//        return_value = buy_item_request_data[id];
//    }
//    return return_value;
//}
//
//void DB::modify_buy_item_request_data(const std::string &id, const BuyItemRequestData &modified_buy_item_request_data) {
//    this->buy_item_request_data[id] = modified_buy_item_request_data;
//}
//
// buy item data done

// seller data stuff

void DB::load_seller_data(const std::string &filename) {
    Json::Value root;

    if (!parse_json_from_file(filename, root)) {
        return;
    }

    for (auto it = root.begin(); it != root.end(); it++) {
        Json::Value temp = *it;
        std::string id = temp["id"].asString();
        SellerData new_seller;
        new_seller.id = id;
        new_seller.account = temp["account"].asString();
        new_seller.password = temp["password"].asString();
        new_seller.shop_name= temp["shop_name"].asString();
        new_seller.shop_address= temp["shop_address"].asString();
        new_seller.shop_owner_id_number = temp["shop_owner_id_number"].asString();
        new_seller.shop_owner_phone_number = temp["shop_owner_phone_number"].asString();
        new_seller.shop_owner_name = temp["shop_owner_name"].asString();
        new_seller.shop_owner_id_number = temp["shop_owner_id_number"].asString();

        std::vector<Order> history_order;
        for (int i = 0; i < temp["history_order"].size(); i++) {
            Order new_order;
            new_order.id = temp["history_order"][i]["id"].asString();
            new_order.item_id = temp["history_order"][i]["item_id"].asString();
            new_order.price = temp["history_order"][i]["price"].asDouble();
            new_order.time = temp["history_order"][i]["time"].asString();
            new_order.buy_num = temp["history_order"][i]["buy_num"].asInt();
            history_order.emplace_back(new_order);
        }
        new_seller.history_order = history_order;

        for (int i = 0; i < temp["message"].size(); i++) {
            new_seller.message.emplace_back(temp["message"][i].asString());
        }

        std::vector<ChangeItemRequestData> change_item_request_list;
        for (int i = 0; i < temp["change_item_request_list"].size(); i++) {
            ChangeItemRequestData new_change_item_request;
            new_change_item_request.item_id = temp["change_item_request_list"][i]["item_id"].asString();
            new_change_item_request.des = temp["change_item_request_list"][i]["des"].asString();
            new_change_item_request.time = temp["change_item_request_list"][i]["time"].asString();
            new_change_item_request.id = temp["change_item_request_list"][i]["id"].asString();
            new_change_item_request.buy_num = temp["change_item_request_list"][i]["buy_num"].asInt();
            new_change_item_request.price = temp["change_item_request_list"][i]["price"].asDouble();
            new_change_item_request.user_id = temp["change_item_request_list"][i]["user_id"].asString();
            change_item_request_list.emplace_back(new_change_item_request);
        }
        new_seller.change_item_request_list = change_item_request_list;

        std::vector<BuyItemRequestData> buy_item_request_list;
        for (int i = 0; i < temp["buy_item_request_list"].size(); i++) {
            BuyItemRequestData new_buy_item_request_data;
            new_buy_item_request_data.item_id = temp["buy_item_request_list"][i]["item_id"].asString();
            new_buy_item_request_data.time = temp["buy_item_request_list"][i]["time"].asString();
            new_buy_item_request_data.id = temp["buy_item_request_list"][i]["id"].asString();
            new_buy_item_request_data.user_id = temp["buy_item_request_list"][i]["user_id"].asString();
            new_buy_item_request_data.remark = temp["buy_item_request_list"][i]["remark"].asString();
            new_buy_item_request_data.buy_num = temp["buy_item_request_list"][i]["buy_num"].asInt();
            new_buy_item_request_data.price = temp["buy_item_request_list"][i]["price"].asDouble();
            buy_item_request_list.emplace_back(new_buy_item_request_data);
        }
        new_seller.buy_item_request_list = buy_item_request_list;

        for (int i = 0; i < temp["discount"].size(); i++) {
            new_seller.discount.emplace_back(temp["discount"][i].asString());
        }

        std::vector<QuestionData> question_list;
        for (int i = 0; i < temp["question_list"].size(); i++) {
            QuestionData new_question;
            new_question.answer = temp["question_list"][i]["answer"].asString();
            new_question.question = temp["question_list"][i]["question"].asString();
            question_list.emplace_back(new_question);
        }
        new_seller.question_list = question_list;

        Wallet wallet;
        wallet.money = temp["wallet"]["money"].asDouble();
        wallet.password = temp["wallet"]["password"].asString();
        new_seller.wallet = wallet;

        this->seller_data[id] = new_seller;
    }
}

void DB::save_seller_data(const std::string &filename) {
    Json::Value root;

    Json::Value new_seller_data;
    for (const auto &[id, value]: this->seller_data) {
        new_seller_data.clear();
        new_seller_data["id"] = value.id;
        new_seller_data["account"] = value.account;
        new_seller_data["password"] = value.password;
        new_seller_data["shop_name"] = value.shop_name;
        new_seller_data["shop_address"] = value.shop_address;
        new_seller_data["shop_owner_name"] = value.shop_owner_name;
        new_seller_data["shop_owner_phone_number"] = value.shop_owner_phone_number;
        new_seller_data["shop_owner_id_number"] = value.shop_owner_id_number;

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
        new_seller_data["history_order"] = history_order;

        Json::Value change_item_request_list;
        change_item_request_list.clear();
        for (const auto &x: value.change_item_request_list) {
            Json::Value change_item_request;
            change_item_request.clear();
            change_item_request["item_id"] = x.item_id;
            change_item_request["des"] = x.des;
            change_item_request["time"] = x.time;
            change_item_request["id"] = x.id;
            change_item_request["buy_num"] = x.buy_num;
            change_item_request["price"] = x.price;
            change_item_request["user_id"] = x.user_id;

            change_item_request_list.append(change_item_request);
        }
        new_seller_data["change_item_request_list"] = change_item_request_list;

        Json::Value buy_item_request_list;
        buy_item_request_list.clear();
        for (const auto &x: value.buy_item_request_list) {
            Json::Value buy_item_request;
            buy_item_request["time"] = x.time;
            buy_item_request["item_id"] = x.item_id;
            buy_item_request["remark"] = x.remark;
            buy_item_request["user_id"] = x.user_id;
            buy_item_request["id"] = x.id;
            buy_item_request["buy_num"] = x.buy_num;
            buy_item_request["price"] = x.price;

            buy_item_request_list.append(buy_item_request);
        }
        new_seller_data["buy_item_request_list"] = buy_item_request_list;

        Json::Value question_list;
        question_list.clear();
        for (const auto &x: value.question_list) {
            Json::Value question;
            question.clear();
            question["question"] = x.question;
            question["answer"] = x.answer;

            question_list.append(question);
        }
        new_seller_data["question_list"] = question_list;

        Json::Value message;
        message.clear();
        for (int i = 0; i < value.message.size(); i++) {
            message[i] = value.message[i];
        }
        new_seller_data["message"] = message;

        Json::Value discount;
        discount.clear();
        for (int i = 0; i < value.discount.size(); i++) {
            discount[i] = value.discount[i];
        }
        new_seller_data["discount"] = discount;

        Json::Value wallet;
        wallet.clear();
        wallet["password"] = value.wallet.password;
        wallet["money"] = value.wallet.money;
        new_seller_data["wallet"] = wallet;

        root.append(new_seller_data);
    }

    if (!save_json_to_file(filename, root)) {
        return;
    }
}

std::string DB::insert_seller_data(const SellerData &inserted_seller_data) {
    SellerData new_seller_data = inserted_seller_data;
    IDgenerator &gen = IDgenerator::get_instance();
    new_seller_data.id = gen.generateID(Type::Seller);
    this->seller_data[new_seller_data.id] = new_seller_data;
    return new_seller_data.id;
}

void DB::delete_seller_data(const std::string &id) {
    if (this->seller_data.count(id)) {
        this->seller_data.erase(id);
    }
}

std::vector<SellerData> DB::select_all_seller_data() {
    std::vector<SellerData> return_value;
    for (const auto &[id, value] : this->seller_data) {
        return_value.push_back(value);
    }
    return return_value;
}

SellerData DB::select_seller_data(const std::string &id) {
    SellerData return_value;
    if (this->seller_data.count(id)) {
        return_value = this->seller_data[id];
    }
    return return_value;
}

void DB::modify_seller_data(const std::string &id, const SellerData &modified_seller_data) {
    this->seller_data[id] = modified_seller_data;
}

// seller data done

// register request data stuff

void DB::load_register_request_data(const std::string &filename) {
    Json::Value root;

    if (!parse_json_from_file(filename, root)) {
        return;
    }

    for (auto it = root.begin(); it != root.end(); it++) {
        Json::Value temp = *it;
        std::string id = temp["id"].asString();
        RegisterRequestData new_register_request_data;
        new_register_request_data.id = id;
        new_register_request_data.shop_owner_id_number = temp["shop_owner_id_number"].asString();
        new_register_request_data.shop_owner_phone_number = temp["shop_owner_phone_number"].asString();
        new_register_request_data.shop_owner_name = temp["shop_owner_name"].asString();
        new_register_request_data.shop_address = temp["shop_address"].asString();
        new_register_request_data.password = temp["password"].asString();
        new_register_request_data.account = temp["account"].asString();
        new_register_request_data.shop_name = temp["shop_name"].asString();

        this->register_request_data[id] = new_register_request_data;
    }
}

void DB::save_register_request_data(const std::string &filename) {
    Json::Value root;

    Json::Value new_register_request_data;
    for (const auto &[id, value]: this->register_request_data) {
        new_register_request_data.clear();

        new_register_request_data["account"] = value.account;
        new_register_request_data["password"] = value.password;
        new_register_request_data["id"] = value.id;
        new_register_request_data["shop_name"] = value.shop_name;
        new_register_request_data["shop_address"] = value.shop_address;
        new_register_request_data["shop_owner_name"] = value.shop_owner_name;
        new_register_request_data["shop_owner_phone_number"] = value.shop_owner_phone_number;
        new_register_request_data["shop_owner_id_number"] = value.shop_owner_id_number;

        root.append(new_register_request_data);
    }

    if (!save_json_to_file(filename, root)) {
        return;
    }
}

std::vector<RegisterRequestData> DB::select_all_register_request_data() {
    std::vector<RegisterRequestData> return_value;
    for (const auto &[id, value]: this->register_request_data) {
        return_value.push_back(value);
    }
    return return_value;
}

std::string DB::insert_register_request_data(const RegisterRequestData &inserted_register_request_data) {
    RegisterRequestData new_register_data = inserted_register_request_data;
    IDgenerator &gen = IDgenerator::get_instance();
    new_register_data.id = gen.generateID(Type::RegisterRequest);
    this->register_request_data[new_register_data.id] = new_register_data;
    return new_register_data.id;
}

void DB::delete_register_request_data(const std::string &id) {
    if (this->register_request_data.count(id)) {
        this->register_request_data.erase(id);
    }
}

RegisterRequestData DB::select_register_request_data(const std::string &id) {
    RegisterRequestData return_value;
    if (this->register_request_data.count(id)) {
        return_value = this->register_request_data[id];
    }
    return return_value;
}

void DB::modify_register_request_data(const std::string &id, const RegisterRequestData &modified_register_request_data) {
    this->register_request_data[id] = modified_register_request_data;
}

// register request data done