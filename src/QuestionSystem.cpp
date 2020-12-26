//
// Created by dbdxzwh on 2020/12/26.
//

#include "QuestionSystem.h"

bool QuestionSystem::CheckKeyWord(const std::vector<std::string>& s, const std::vector<std::string>& v) {
    BasicOperation op;
    for(int i=0; i<s.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            double sim = op.GetWordsSim(s[i], v[j]);
            if (sim > 0.6)
                return true;
        }
    }
    return false;
}

std::string QuestionSystem::GetAnswer(const std::string& id, const std::string& question) {
    BasicOperation op;
    //问题的匹配我们设计为关键词的匹配
    //商家的信息包括商家电话号、商家地址、商品信息
    std::vector<string> word = op.sentence2word(question);
    //TODO: SellerData seller = db.select_seller_data(id);
    SellerData seller;

    //商家电话号、商家地址都是直接从商家编号可知
    if(CheckKeyWord({"phone","tel","telephone"}, word))
        return seller.shop_owner_phone_number;
    else if(CheckKeyWord({"address","where","place"}, word))
        return seller.shop_address;

    //商品信息需要从商家编号到商家对应的商品编号可知
    std::vector<ItemData> sel_items;  //所咨询商家所有的商品
    sel_items = op.GetShopItems(id);
    for(int i=0; i<sel_items.size(); i++) {
        if(CheckKeyWord({sel_items[i].name}, word)) {
            if(CheckKeyWord({"price","money","cost"}, word))
                return to_string(sel_items[i].price);
            else if(CheckKeyWord({"description","detail"}, word))
                return sel_items[i].des;
            else if(CheckKeyWord({"sales","volume"}, word))
                return to_string(sel_items[i].sell_num);
        }
    }
}








