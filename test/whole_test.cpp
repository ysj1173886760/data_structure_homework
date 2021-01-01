#include "ServiceSystem.h"
#include "QuestionSystem.h"

using namespace std;

//void display_shop_list(const string& user_id) {
//    DB &db = DB::getInstance();
//    UserData user = db.select_user_data(user_id);
//
//    for(int i=0; i<user.shop_list.size(); i++)
//        user.shop_list[i].print();
//}
//
//void display_current_list(const string& user_id) {
//    DB &db = DB::getInstance();
//    UserData user = db.select_user_data(user_id);
//
//    for(int i=0; i<user.current_order.size(); i++)
//        user.current_order[i].print();
//}
//
//void display_BuyItemRequest(const string& seller_id) {
//    DB &db = DB::getInstance();
//    SellerData seller = db.select_seller_data(seller_id);
//
//    for(int i=0; i<seller.buy_item_request_list.size(); i++)
//        seller.buy_item_request_list[i].print();
//}

int main() {
    BasicOperation op;
    DB& db = DB::getInstance();
    db.open();

    vector<UserData> users = db.select_all_user_data();
    UserData user1 = users[0];
    UserData user2 = users[1];
    UserData user3 = users[2];

    SellerData seller = db.select_seller_data("220201231103120");

    ServiceSystem ser_sys;
    ser_sys.insert_shop_list(user1.id, seller.shop_name, "new_2", 2);
    ser_sys.insert_shop_list(user2.id, seller.shop_name, "new_2", 4);
    ser_sys.insert_shop_list(user3.id, seller.shop_name, "new_2", 3);

    ser_sys.submit_shop_list(user1.id, "a");
    ser_sys.submit_shop_list(user2.id, "b");
    ser_sys.submit_shop_list(user3.id, "c");

    ser_sys.deal_BuyItemRequest(seller.id);

    db.close();
}