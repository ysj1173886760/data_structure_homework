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

int main() {
    //先添加一个商家（之后换为注册环节）
    DB& db = DB::getInstance();
    db.open();
    SellerData seller;
    GetSellerInfo(seller);
    string seller_id = db.insert_seller_data(seller);

    //以商家身份登录
    SellerSystem seller_sys;
    cout << "请选择你的操作: " << endl;
    cout << "1: 添加商品" << endl;
    cout << "2: 删除商品" << endl;
    cout << "3: 修改商品" << endl;
    cout << "0: 结束操作" << endl;

    int choice;
    cin >> choice;
    ItemData item;
    string rm_name, mod_name, mod_info;
    while(choice) {
        switch (choice) {
            case 1:
                GetItemInfo(item);
                seller_sys.insert_item(seller_id, item);
                break;
            case 2:
                cout << "请输入删除商品的名称：" << endl;
                cin >> rm_name;
                seller_sys.remove_item(seller_id, rm_name);
                break;
            case 3:
                cout << "请输入修改商品的名称：" << endl;
                cin >> mod_name;
                mod_info = GetModInfo(mod_name, item);
                seller_sys.modify_item(seller_id, item, mod_info);
                break;
            default:
                cout << "ERROR : wrong choice!!!" << endl;
                break;
        }
        cout << "请选择你的操作: " << endl;
        cout << "1: 添加商品" << endl;
        cout << "2: 删除商品" << endl;
        cout << "3: 修改商品" << endl;
        cout << "0: 结束操作" << endl;
        cin >> choice;
    }

    db.close();
}






