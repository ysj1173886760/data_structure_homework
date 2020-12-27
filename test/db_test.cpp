//
// Created by sheep on 2020/12/24.
//

#include <iostream>
#include "DB.h"

using namespace std;

int main() {
    DB& db = DB::getInstance();
    db.open();
    // select test
    vector<UserData> tmp = db.select_all_user_data();
    for (const auto &x: tmp) {
        x.print();
        cout << endl;
    }
//  delete test
//    db.delete_user_data("12312312");
//  insert test
//    UserData new_user_data;
//    new_user_data.id = "12312313";
//    new_user_data.account = "sheep3";
//    new_user_data.password = "000";
//    new_user_data.user_name = "sheep";
//    new_user_data.real_name = "real_name";
//    new_user_data.email = "321";
//    db.insert_user_data(new_user_data);
    // save data test
    ItemData new_item_data;
    new_item_data.price = 1.1;
    new_item_data.store_num = 10;
    new_item_data.owner = "asd";
    new_item_data.des = "dsa";
    new_item_data.sell_num = 1;
    string id = db.insert_item_data(new_item_data);
    cout << id << endl;
    db.close();
    return 0;
}