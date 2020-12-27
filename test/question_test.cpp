#include "QuestionSystem.h"

int main() {
    DB& db = DB::getInstance();
    QuestionSystem s;
    SellerData seller;
    seller.shop_address = "shenyang";
    seller.shop_owner_phone_number = "12345678";
    seller.shop_name = "shop";
    seller.id = "22020094912";
    db.insert_seller_data(seller);
    std::cout << db.select_seller_data("22020094912").shop_address;
    std::string question = "where is your address?";
    std::string ans = s.GetAnswer(seller.shop_owner_id_number, question);
}