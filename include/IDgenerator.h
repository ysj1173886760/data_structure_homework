//
// Created by dbdxzwh on 2020/12/26.
//

#ifndef DATA_STRUCTURE_HOMEWORK_IDGENERATOR_H
#define DATA_STRUCTURE_HOMEWORK_IDGENERATOR_H

#include "base.h"
#include "BasicOperation.h"

class IDgenerator {
private:
    IDgenerator() {}
public:
    ~IDgenerator() {}
    IDgenerator(const IDgenerator&) = delete;
    IDgenerator& operator=(const IDgenerator&) = delete;
    static IDgenerator& get_instance() {
        static IDgenerator instance;
        return instance;
    }

    //传入不同的数据类型，返回ID
    std::string generateID(Type type);
    //传入ID，返回不同的数据类型
    Type GetType(const std::string&);
};


#endif //DATA_STRUCTURE_HOMEWORK_IDGENERATOR_H
