//
// Created by dbdxzwh on 2020/12/28.
//

#include "MessageSystem.h"

void MessageSystem::SendMessage(const std::string& receiver_id, const std::string& message) {
    DB& db = DB::getInstance();
    IDgenerator generator = IDgenerator::get_instance();
    Type type = generator.GetType(receiver);

    if(type == Type::User) {
        UserData user = db.select_user_data(receiver);
        user.message.push_back(message);
    }
    else if(type == Type::Seller) {
        SellerData seller = db.select_seller_data(receiver);
        seller.message.push_back(message);
    }
    else
        std::cout << "ERROR : id didn't support transfer money operation!!!";
}