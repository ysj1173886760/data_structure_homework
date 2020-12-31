//
// Created by dbdxzwh on 2020/12/26.
//

#ifndef DATA_STRUCTURE_HOMEWORK_BASICOPERATION_H
#define DATA_STRUCTURE_HOMEWORK_BASICOPERATION_H

#include "DB.h"
#include "base.h"
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;
using namespace boost;

class BasicOperation {
public:
    //将当前时间转换为字符串
    std::string time2str();
    //将一句话转换为多个单词的数组
    vector<std::string> sentence2word(const string&);
    //比较两个单词的相似度并返回
    double GetWordsSim(const std::string&, const std::string&);
    //传入商家的ID返回该商家的所有商品信息
    vector<ItemData> GetShopItems(const std::string&);
    //传入商家的名称，商品的名称，返回的对应商品的信息存在第三个参数中
    bool GetItem(const std::string&, const std::string&, ItemData&);
    //传入顾客的ID与商家的名称，返回该顾客的订单花费
    double GetCost(const std::string&, const std::string&);
    //传入商家的名称，将商家的信息存在第二个参数中
    bool GetSeller(const std::string&, SellerData&);
    //传入顾客的账号，返回顾客的ID
    std::string get_user_id_by_account(const std::string &account);
    std::string get_seller_id_by_account(const std::string &account);
    std::string get_manager_id_by_account(const std::string &account);
    std::string get_seller_id_by_name(const std::string &name);
};


#endif //DATA_STRUCTURE_HOMEWORK_BASICOPERATION_H
