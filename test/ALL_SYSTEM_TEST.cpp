//
// Created by dbdxzwh on 2020/12/29.
//

#include "DB.h"
#include "base.h"
#include "SellerSystem.h"
#include "ServiceSystem.h"
#include "IDgenerator.h"

using namespace std;

void GetItemInfo(ItemData& item) {
    IDgenerator& g = IDgenerator::get_instance();
    item.id = g.generateID(Type::Item);

    cout << "请输入商品名称：";
    cin >> item.name;
    cout << "请输入商品价格：";
    cin >> item.price;
    cout << "请输入商品库存：";
    cin >> item.store_num;
    item.sell_num = 0;
}

void GetSellerInfo(SellerData& seller) {
    IDgenerator& g = IDgenerator::get_instance();
    seller.id = g.generateID(Type::Seller);

    seller.shop_name = "test_shop_name";
    seller.password = "123456789";
    seller.shop_address = "DB university";
    seller.wallet.money = 1000;
}

string GetModInfo(const string& name, ItemData& item) {
    item.name = name;
    string mods_info[3] = {"price","store_num","des"};
    cout << "请输入要修改的信息：" << endl;
    cout << "1: 商品的价格" << endl;
    cout << "2: 商品的库存" << endl;
    cout << "3: 商品的描述" << endl;

    int choice;
    cin >> choice;
    switch (choice) {
        case 1:
            cout << "请输入商品新的价格" << endl;
            cin >> item.price;
            break;
        case 2:
            cout << "请输入商品新的库存"  << endl;
            cin >> item.store_num;
            break;
        case 3:
            cout << "请输入商品新的描述" << endl;
            cin >> item.des;
            break;
        default:
            break;
    }

    return mods_info[choice-1];
}

void GetUserInfo(UserData& user) {
    IDgenerator& g = IDgenerator::get_instance();
    user.id = g.generateID(Type::User);
    user.wallet.money = 10;
}

void checkMessage(const string& user_id) {
    DB& db = DB::getInstance();
    UserData user = db.select_user_data(user_id);

    if(user.message.empty()) {
        cout << "no message" << endl;
        return;
    }

    while(!user.message.empty()) {
        string m = user.message.front();
        cout << m << endl;
        user.message.erase(user.message.begin());
    }
    db.modify_user_data(user.id, user);
}

void display_shop_list(const string& user_id) {
    DB& db = DB::getInstance();
    UserData user = db.select_user_data(user_id);

    for(int i=0; i<user.shop_list.size(); i++) {
        user.shop_list[i].print();
    }
}

int main() {
    DB& db = DB::getInstance();
    db.open();

    int choice;
    SellerData seller;

    //先添加一个商家（之后换为注册环节）
    GetSellerInfo(seller);
    string seller_id = db.insert_seller_data(seller);

    //商家身份 处理商品
//    SellerSystem seller_sys;
//    cout << "请选择你的操作: " << endl;
//    cout << "1: 添加商品" << endl;
//    cout << "2: 删除商品" << endl;
//    cout << "3: 修改商品" << endl;
//    cout << "0: 结束操作" << endl;
//
//    cin >> choice;
//    ItemData item;
//    string rm_name, mod_name, mod_info;
//    while(choice) {
//        switch (choice) {
//            case 1:
//                GetItemInfo(item);
//                seller_sys.insert_item(seller_id, item);
//                break;
//            case 2:
//                cout << "请输入删除商品的名称：" << endl;
//                cin >> rm_name;
//                seller_sys.remove_item(seller_id, rm_name);
//                break;
//            case 3:
//                cout << "请输入修改商品的名称：" << endl;
//                cin >> mod_name;
//                mod_info = GetModInfo(mod_name, item);
//                seller_sys.modify_item(seller_id, item, mod_info);
//                break;
//            default:
//                cout << "ERROR : wrong choice!!!" << endl;
//                break;
//        }
//        cout << "请选择你的操作: " << endl;
//        cout << "1: 添加商品" << endl;
//        cout << "2: 删除商品" << endl;
//        cout << "3: 修改商品" << endl;
//        cout << "0: 结束操作" << endl;
//        cin >> choice;
//    }

    ServiceSystem ser_sys;

    std::vector<ItemData> items = ser_sys.search_item_label("food");
    for(int i=0; i<items.size(); i++) {
        items[i].print();
        cout << endl;
    }

    //顾客身份 买
    vector<SellerData> sellers = db.select_all_seller_data();
    seller = sellers[0];

    UserData user;
    vector<UserData> users = db.select_all_user_data();
    user = users[0];
    GetUserInfo(user);
    string user_id = db.insert_user_data(user);

    cout << "1: 查看消息" << endl;
    cout << "2: 添加购物车" << endl;
    cout << "3: 删除购物车" << endl;
    cout << "4: 提交订单" << endl;
    cout << "5: 显示订单" << endl;
    cout << "0: 结束操作" << endl;
    cin >> choice;

    int add_num, rm_num;
    string add_item_name, rm_item_name;
    vector<ItemData> items;
    while(choice) {
        switch (choice) {
            case 1:
                checkMessage(user.id);
                break;
            case 2:
                cout << "请输入要添加的商品名称：";
                cin >> add_item_name;
                cout << "请输入要添加商品的数量：";
                cin >> add_num;
                ser_sys.insert_shop_list(user.id, seller.shop_name, add_item_name, add_num);
                break;
            case 3:
                cout << "请输入要删除的商品名称：";
                cin >> rm_item_name;
                cout << "请输入要删除的商品数量：";
                cin >> rm_num;
                ser_sys.remove_shop_list(user.id, seller.shop_name, rm_item_name, rm_num);
                break;
            case 4:
                ser_sys.submit_shop_list(user.id, seller.id);
                break;
            case 5:
                display_shop_list(user.id);
                break;
            default:
                cout << "ERROR : wrong choice!!!" << endl;
                break;
        }

        cout << "1: 查看消息" << endl;
        cout << "2: 添加购物车" << endl;
        cout << "3: 删除购物车" << endl;
        cout << "4: 提交订单" << endl;
        cout << "5: 显示订单" << endl;
        cout << "0: 结束操作" << endl;
        cin >> choice;
    }

    ser_sys.deal_BuyItemRequest(seller.id);

    /*测试一：
    顾客买了一件，商家处理，不退货    ok
     */
    /*测试二：
    顾客买了两件，商家处理，退货一件    ok
    user = db.select_user_data(user.id);
    Order order = user.current_order[0];
    ser_sys.returnItem(user.id, order);
     */
    /*测试三：
    顾客买了多件，钱不够，商家不处理订单
     */

    db.close();
}