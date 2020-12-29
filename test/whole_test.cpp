//////
////// Created by dbdxzwh on 2020/12/28.
//////
//
//#include "ServiceSystem.h"
//#include "QuestionSystem.h"
//
//using namespace std;
//
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
//
//int main() {
//    BasicOperation op;
//    DB& db = DB::getInstance();
//    db.open();
//
//    vector<UserData> users = db.select_all_user_data();
//    UserData user = users[0];
//    vector<SellerData> sellers = db.select_all_seller_data();
//    SellerData seller = sellers[0];
//    //顾客操作一：显示某商家所有商品的信息
//    cout << "操作一：" << endl;
//    ServiceSystem ser_system;
//    vector<ItemData> items_1 = ser_system.display_shop("shop_name");
//    for(int i=0; i<items_1.size(); i++)
//        items_1[i].print();
//    //顾客操作二：查看卖某种商品的所有商家的比对（货比货）
//    cout << "操作二：" << endl;
//    vector<ItemData> items_2 = ser_system.display_item("add");
//    for(int i=0; i<items_2.size(); i++)
//        items_2[i].print();
//    //顾客操作三：添加购物车
//    cout << "操作三：" << endl;
//    ser_system.insert_shop_list(user.id, seller.shop_name, "add", 2);
//    display_shop_list(user.id);
//    //顾客操作四：删除购物车
//    cout << "操作四：" << endl;
//    ser_system.remove_shop_list(user.id, seller.shop_name, "add");
//    display_shop_list(user.id);
//    //顾客操作四：提交订单
//    ser_system.submit_shop_list(user.id, seller.id);
//    display_BuyItemRequest(seller.id);
//    cout << "shop list:" << endl;
//    display_shop_list(user.id);
//    cout << "cur order:" << endl;
//    display_current_list(user.id);
//    cout << "buy item request:" << endl;
//    display_BuyItemRequest(seller.id);
//    //商家操作五：
//    ser_system.deal_BuyItemRequest(seller.id);
//    cout << "buy item request:" << endl;
//    display_BuyItemRequest(seller.id);
//    //商家操作六：
//    Order order = user.current_order[0];
//    ser_system.returnItem(user.id, order);
//}