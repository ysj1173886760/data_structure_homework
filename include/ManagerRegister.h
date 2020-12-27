//
// Created by shrimp on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_MANAGERREGISTER_H
#define DATA_STRUCTURE_HOMEWORK_MANAGERREGISTER_H

#include "DB.h"
#include "base.h"
#include <vector>

class ManagerRegister {
public:
    ManagerRegister();
    void Register();

private:
    void register_account();
    void register_password();

    ManagerData manager;
    std::vector<ManagerData> all_manager_data;
};


#endif //DATA_STRUCTURE_HOMEWORK_MANAGERREGISTER_H
