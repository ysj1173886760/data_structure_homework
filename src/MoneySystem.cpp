//
// Created by dbdxzwh on 2020/12/28.
//

#include "MoneySystem.h"

bool MoneySystem::RechargeMoney(const std::string& id, double money) {
    DB& db = DB::getInstance();
    IDgenerator& generator = IDgenerator::get_instance();
    Type type = generator.GetType(id);
    if(type == Type::User) {
        UserData user = db.select_user_data(id);
        user.wallet.money += money;
        return true;
    }
    else if(type == Type::Seller) {
        SellerData seller = db.select_seller_data(id);
        seller.wallet.money += money;
        return true;
    }
    else {
        std::cout << "ERROR : this id didn't support recharge money operation!!!" << std::endl;
        return false;
    }//TODO : 发消息
}

bool MoneySystem::WithdrawMoney(const std::string& id, double money) {
    DB& db = DB::getInstance();
    IDgenerator& generator = IDgenerator::get_instance();
    Type type = generator.GetType(id);

    if(type == Type::User) {
        UserData user = db.select_user_data(id);
        if(user.wallet.money - money >= 0) {
            user.wallet.money -= money;
        }
        else {
            std::cout << "ERROR : you don't have enough money" << std::endl;
            return false;
        }//TODO : 发消息
    }
    else if(type == Type::Seller) {
        SellerData seller = db.select_seller_data(id);
        seller.wallet.money -= money;
    }
    else {
        std::cout << "ERROR : this id didn't support withdraw money operation!!!" << std::endl;
        return false;
    }
    return true;
}

bool MoneySystem::TransferMoney(const std::string& id_1, const std::string& id_2, double money) {
    DB &db = DB::getInstance();
    IDgenerator &generator = IDgenerator::get_instance();
    Type type_1 = generator.GetType(id_1);
    Type type_2 = generator.GetType(id_2);

    if ((type_1 == Type::User || type_1 == Type::Seller) && (type_2 == Type::User || type_2 == Type::Seller)) {

        if (type_1 == Type::User) {
            UserData user = db.select_user_data(id_1);
            if (user.wallet.money - money >= 0)
                user.wallet.money -= money;
            else {
                std::cout << "ERROR : you don't have enough money" << std::endl;
                return false;
            }
            db.modify_user_data(user.id, user);
            //TODO : 发消息
        }

        else {
            SellerData seller = db.select_seller_data(id_1);
            if (seller.wallet.money - money >= 0)
                seller.wallet.money -= money;
            else {
                std::cout << "ERROR : you don't have enough money" << std::endl;
                return false;
            }
            db.modify_seller_data(seller.id, seller);
        }

        if (type_2 == Type::User) {
            UserData user = db.select_user_data(id_2);
            user.wallet.money += money;
            db.modify_user_data(user.id, user);
            //TODO : 发消息
        }

        else {
            SellerData seller = db.select_seller_data(id_2);
            seller.wallet.money += money;
            db.modify_seller_data(seller.id, seller);
        }
    }

    else {
        std::cout << "ERROR : id didn't support transfer money operation!!!" << std::endl;
        return false;
    }
    return true;
}