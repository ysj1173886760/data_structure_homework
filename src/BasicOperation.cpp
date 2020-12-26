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