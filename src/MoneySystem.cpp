//
// Created by dbdxzwh on 2020/12/28.
//

#include "MoneySystem.h"

void MoneySystem::RechargeMoney(const std::string& user_id, double money) {
    DB& db = DB::getInstance();
    IDgenerator generator = IDgenerator::get_instance();
    Type type = generator.GetType(id);
    if(type == Type::user) {
        UserData user = db.select_user_data(user_id);
        user.wallet.money += money;
    }
    else
        std::cout << "ERROR : this id didn't support recharge money operation!!!";
    //TODO : 发消息
}

void MoneySystem::WithdrawMoney(const std::string& id, double money) {
    DB& db = DB::getInstance();
    IDgenerator generator = IDgenerator::get_instance();
    Type type = generator.GetType(id);

    if(type == Type::User) {
        UserData user = db.select_user_data(id);
        user.wallet.money -= money;
        //TODO : 发消息
    }
    else if(type == Type::Seller) {
        SellerData seller = db.select_seller_data(id);
        seller.wallet.money -= money;
    }
    else
        std::cout << "ERROR : this id didn't support withdraw money operation!!!";
}

void MoneySystem::TransferMoney(const std::string& id_1, const std::string& id_2, double money) {
    DB& db = DB::getInstance();
    IDgenerator generator = IDgenerator::get_instance();
    Type type_1 = generator.GetType(id_1);
    Type type_2 = generator.GetType(id_2);

    if((type_1 == Type::User || type_1 == Type::Seller) && (type_2 == Type::User || type_2 == Type::Seller)) {

        if(type_1 == Type::User) {
            UserData user = db.select_user_data(id);
            user.wallet.money -= money;
            //TODO : 发消息
        }
        else {
            SellerData seller = db.select_seller_data(id);
            seller.wallet.money -= money;
        }

        if(type_2 == Type::User) {
            UserData user = db.select_user_data(id);
            user.wallet.money += money;
            //TODO : 发消息
        }
        else {
            SellerData seller = db.select_seller_data(id);
            seller.wallet.money += money;
        }
    }
    else
        std::cout << "ERROR : id didn't support transfer money operation!!!";
}