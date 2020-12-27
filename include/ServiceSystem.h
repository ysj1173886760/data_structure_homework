//
// Created by dbdxzwh on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_SERVICESYSTEM_H
#define DATA_STRUCTURE_HOMEWORK_SERVICESYSTEM_H

#include "DB.h"
#include "base.h"
#include "BasicOperation.h"

class ServiceSystem {
public:
    //从所有名称中寻找最接近的那一个，返回编号以及名称
    std::string GetHighestSim(const std::vector<std::string>&, const std::string&);
public:
    //根据商家id显示商家商品名称以及价格
    void display(const std::string&);
    //根据商品名称查看某种商品的详细信息
    void display_detail(const std::string&);
    //根据顾客id将商品名称与件数添加入购物车
    void insert_shop_list(const std::string&, const std::string&, int);
    //从购物车中删除多件商品
    void remove_shop_list(const std::string&, const std::string&, int);
    //根据商家id与顾客id提交订单(id用来更新各自的history_order)
    void submit_shop_list(const std::string&, const std::string&, const std::vector<std::string>&);
};


#endif //DATA_STRUCTURE_HOMEWORK_SERVICESYSTEM_H