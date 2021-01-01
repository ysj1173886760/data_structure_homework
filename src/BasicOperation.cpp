//
// Created by dbdxzwh on 2020/12/26.
//

#include "BasicOperation.h"

//将boost时间类型转换为字符串类型
//例如：20201226T130716->20201226130716
std::string BasicOperation::time2str() {
    string str = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
    int pos = str.find('T');
    str.erase(pos,1);
    return str;
}

//问题格式：每两个单词间有一个空格 允许以问号结尾 允许只填入一个单词
//例如：i am zwh->i,am,zwh     am i zwh?->am,i,zwh     zwh->zwh
vector<std::string> BasicOperation::sentence2word(const string& str) {
    int pre = 0, pos = 0;
    vector<string> word;
    while(pos < str.size()) {
        if(str[pos] == ' ') {
            word.push_back(str.substr(pre, pos-pre));
            pre = pos+1;
        }
        pos++;
    }
    if(str[pos-1]=='?')
        word.push_back(str.substr(pre, pos-pre-1));
    else
        word.push_back(str.substr(pre, pos-pre));
    return word;
}

//获取单词的相似度
double BasicOperation::GetWordsSim(const std::string& s1, const std::string& s2) {
    int m = s1.size(), n = s2.size();
    vector<vector<int>> dp(m+1, vector<int>(n+1));
    for (int i = 1; i <= m; i++)
        dp[i][0] = i;
    for (int j = 1; j <= n; j++)
        dp[0][j] = j;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1])
                dp[i][j] = dp[i-1][j-1];
            else
                dp[i][j] = min(dp[i-1][j] + 1,
                               min(dp[i][j-1] + 1, dp[i-1][j-1] + 1));
        }
    }
    return 1-double(dp[m][n])/max(m,n);
}

vector<ItemData> BasicOperation::GetShopItems(const std::string& id) {
    DB& db = DB::getInstance();
    SellerData seller = db.select_seller_data(id);
    std::vector<ItemData> items = db.select_all_item_data();
    std::vector<ItemData> sel_items;  //所咨询商家所有的商品
    for(int i=0; i<items.size(); i++) {
        if(items[i].owner == seller.shop_name)
            sel_items.push_back(items[i]);
    }
    return sel_items;
}

bool BasicOperation::GetItem(const std::string& shop_name, const std::string& item_name, ItemData& item) {
    DB &db = DB::getInstance();
    vector<SellerData> sellers = db.select_all_seller_data();

    int tar;
    for(tar=0; tar<sellers.size(); tar++) {
        if(sellers[tar].shop_name == shop_name)
            break;
    }

    int i;
    vector<ItemData> items = GetShopItems(sellers[tar].id);
    for(i=0; i<items.size(); i++) {
        if (items[i].name == item_name) {
            item = items[i];
            return true;
        }
    }
    return false;
}

double BasicOperation::GetCost(const std::string& user_id) {
    DB &db = DB::getInstance();
    UserData user = db.select_user_data(user_id);

    double cost = 0;
    for(int i=0; i<user.shop_list.size(); i++)
        cost += (user.shop_list[i].price * user.shop_list[i].buy_num);

    return cost;
}

bool BasicOperation::GetSeller(const std::string &shop_name, SellerData& seller) {
    DB &db = DB::getInstance();
    vector<SellerData> sellers = db.select_all_seller_data();

    int tar;
    for(tar=0; tar<sellers.size(); tar++) {
        if(sellers[tar].shop_name == shop_name) {
            seller = sellers[tar];
            return true;
        }
    }
    return false;
}

std::string BasicOperation::get_user_id_by_account(const std::string &account) {
    DB &db = DB::getInstance();
    vector<UserData> users = db.select_all_user_data();
    for (const auto &x : users) {
        if (x.account == account) {
            return x.id;
        }
    }
    return "";
}