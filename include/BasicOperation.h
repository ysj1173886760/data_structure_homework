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
};


#endif //DATA_STRUCTURE_HOMEWORK_BASICOPERATION_H
