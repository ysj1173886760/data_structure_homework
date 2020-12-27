//
// Created by shrimp on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_REGISTERMANAGER_H
#define DATA_STRUCTURE_HOMEWORK_REGISTERMANAGER_H

// 管理员注册时与卖家公用同一个列表吗？
// 除了账号密码其他的需要注册吗？
#include "DB.h"
#include "base.h"
#include <vector>

class RegisterManager {
public:
    RegisterManager();
    void Register();

    void print() {
        manager.print();
    }
private:
    void register_account();
    void register_password();

    ManagerData manager;
    std::vector<ManagerData> all_manager_data;
};


#endif //DATA_STRUCTURE_HOMEWORK_REGISTERMANAGER_H
