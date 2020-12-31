//
// Created by dbdxzwh on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_SERVICESYSTEM_H
#define DATA_STRUCTURE_HOMEWORK_SERVICESYSTEM_H

#include "DB.h"
#include "base.h"
#include "BasicOperation.h"
#include "IDgenerator.h"
#include "MoneySystem.h"
#include "MessageSystem.h"

class ServiceSystem {
private:
    bool CmpSameLabel(const std::vector<std::string>&, const std::string&);
    //从所有名称中寻找最接近的那一个，返回编号以及名称
    vector<std::string> GetHighestSim(const std::vector<std::string>&, const std::string&);
    vector<std::string> GetSamePart(const std::vector<std::string>&, const std::string&);
public:
    //传入商家名称 显示某个商家商品名称以及价格
    std::vector<ItemData> search_shop(const std::string&);
    //传入商品名称（相似度匹配） 查看卖这种商品的所有商家的名称以及价格
    std::vector<ItemData> search_item_sim(const std::string&);
    //传入商品名称（部分匹配）
    std::vector<ItemData> search_item_part(const std::string&);
    //传入商品的标签（精准匹配）
    std::vector<ItemData> search_item_label(const std::string&);
    //user_id shop_id item_name num
    bool insert_shop_list(const std::string&, const std::string&, const std::string&, int num=1);
    //从购物车中删除多件商品
    bool remove_shop_list(const std::string&, const std::string&, const std::string&, int num=1);
    //根据商家id与顾客id提交订单，完成款项的转移，并留言
    bool submit_shop_list(const std::string&, const std::string&);
    //商家处理购买订单
    void deal_BuyItemRequest(const std::string&);
    //顾客退还商品
    bool returnItem(const std::string&, const Order&);
};


#endif //DATA_STRUCTURE_HOMEWORK_SERVICESYSTEM_H
