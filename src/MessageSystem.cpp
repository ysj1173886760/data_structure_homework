//
// Created by dbdxzwh on 2020/12/28.
//

#include "MessageSystem.h"

void MessageSystem::SendMessage(const std::string& receiver_id, const std::string& message) {
    DB& db = DB::getInstance();
    IDgenerator& generator = IDgenerator::get_instance();
    Type type = generator.GetType(receiver_id);

    if(type == Type::User) {
        UserData user = db.select_user_data(receiver_id);
        user.message.push_back(message);
        db.modify_user_data(user.id, user);
    }
    else if(type == Type::Seller) {
        SellerData seller = db.select_seller_data(receiver_id);
        seller.message.push_back(message);
        db.modify_seller_data(seller.id, seller);
    }
    else
        std::cout << "ERROR : id didn't support transfer money operation!!!" << std::endl;
}