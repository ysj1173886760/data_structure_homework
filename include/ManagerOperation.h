//
// Created by shrimp on 2020/12/28.
//

#ifndef DATA_STRUCTURE_HOMEWORK_MANAGEROPERATION_H
#define DATA_STRUCTURE_HOMEWORK_MANAGEROPERATION_H

#include "RegisterManager.h"
#include "RegisterSeller.h"
#include "base.h"
#include "DB.h"
#include <string>
#include <vector>

class ManagerOperation {
public:
    ManagerOperation();

    // 获得一个申请列表对象的 vector
    std::vector<RegisterRequestData> view_application_list();

    // 接受商家的申请
    // 传入：商家的账号和店名
    bool accept_shop_application(const std::string& account,
                                 const std::string& shop_name);

    // 拒绝商家申请
    // 传入：商家的账号与店名
    bool reject_shop_application(const std::string& account,
                                 const std::string& shop_name);

    // 创建新的管理员账号（注意，只有主管理员才能创建新的账号）
    // 传入：账号，密码
    bool register_manager(const std::string& account,
                          const std::string& password);
private:

    // 返回一个 商家信息 的对象
    SellerData copy_request_to_seller(const RegisterRequestData& rqd);
};


#endif //DATA_STRUCTURE_HOMEWORK_MANAGEROPERATION_H
