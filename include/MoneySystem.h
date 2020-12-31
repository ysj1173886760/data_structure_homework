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
    //传入ID、金额（ID可为顾客or商家）
    bool RechargeMoney(const std::string&, double);
    //传入ID、金额（ID可为顾客or商家）
    bool WithdrawMoney(const std::string&, double);
    //传入转账双方的ID，金额（双方可为顾客or商家）
    bool TransferMoney(const std::string&, const std::string&, double);
};


#endif //DATA_STRUCTURE_HOMEWORK_MONEYSYSTEM_H
