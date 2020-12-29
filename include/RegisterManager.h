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

    // return 0, if you register ok
    // return 1, if the account already exists
    // return 2, if the password is illegal
    int Register(const std::string&, const std::string&, const std::string&);

    void print() {
        manager.print();
    }
private:
    bool register_account(const std::string&);
    bool register_password(const std::string&, const std::string&);

    ManagerData manager;
    std::vector<ManagerData> all_manager_data;
};


#endif //DATA_STRUCTURE_HOMEWORK_REGISTERMANAGER_H
