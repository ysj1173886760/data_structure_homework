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

    std::string generateID(enum Type type);
};


#endif //DATA_STRUCTURE_HOMEWORK_IDGENERATOR_H
