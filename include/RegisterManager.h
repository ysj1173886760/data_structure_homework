//
// Created by shrimp on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_REGISTERMANAGER_H
#define DATA_STRUCTURE_HOMEWORK_REGISTERMANAGER_H

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
