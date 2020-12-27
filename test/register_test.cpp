//
// Created by shrimp on 2020/12/27.
//

#include "RegisterUser.h"
#include "RegisterSeller.h"
#include "RegisterManager.h"
#include "DB.h"
#include <vector>

int main() {
    RegisterManager m;
    m.Register();
    m.print();

    DB& db = DB::getInstance();
    db.open();

    std::vector<ManagerData> md = db.select_all_manager_data();
    for (auto it : md) {
        std::cout << "testManager" << std::endl;
        it.print();
    }

    db.close();
}