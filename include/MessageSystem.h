//
// Created by dbdxzwh on 2020/12/28.
//

#ifndef DATA_STRUCTURE_HOMEWORK_MESSAGESYSTEM_H
#define DATA_STRUCTURE_HOMEWORK_MESSAGESYSTEM_H

#include "DB.h"
#include "base.h"
#include "IDgenerator.h"

class MessageSystem {
public:
    //传入接收信息的人的ID、信息内容
    void SendMessage(const std::string&, const std::string&);
};


#endif //DATA_STRUCTURE_HOMEWORK_MESSAGESYSTEM_H
