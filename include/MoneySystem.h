//
// Created by dbdxzwh on 2020/12/28.
//

#ifndef DATA_STRUCTURE_HOMEWORK_MONEYSYSTEM_H
#define DATA_STRUCTURE_HOMEWORK_MONEYSYSTEM_H

#include "DB.h"
#include "base.h"
#include "IDgenerator.h"

class MoneySystem {
public:
    //顾客充值
    bool RechargeMoney(const std::string&, double);
    //（顾客或商家）提现
    bool WithdrawMoney(const std::string&, double);
    //两人（顾客或商家）之间的转账
    bool TransferMoney(const std::string&, const std::string&, double);
};


#endif //DATA_STRUCTURE_HOMEWORK_MONEYSYSTEM_H
