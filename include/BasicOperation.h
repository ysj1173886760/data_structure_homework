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
    std::string time2str();
    vector<std::string> sentence2word(const string&);
    //比较两个单词的相似度并返回
    double GetWordsSim(const std::string&, const std::string&);
    vector<ItemData> GetShopItems(const std::string&);
    //获取对应商家的商品结构，保存在参数三中
    bool GetItem(const std::string&, const std::string&, ItemData&);
    double GetCost(const std::string&);
    //根据商家名称获得商家结构
    bool GetSeller(const std::string&, SellerData&);
};


#endif //DATA_STRUCTURE_HOMEWORK_BASICOPERATION_H
