//
// Created by dbdxzwh on 2020/12/26.
//

#include "IDgenerator.h"
std::string IDgenerator::GenerateID(Type type) {
    BasicOperation op;
    std::string id = "";
    id += (int)type+'0';
    id += op.time2str();
    return id;
}

Type IDgenerator::GetType(const string& id) {
    return Type(id[0]-'0');
}