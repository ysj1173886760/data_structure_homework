//
// Created by sheep on 2020/12/24.
//

#include <iostream>
#include "DB.h"

using namespace std;

int main() {
    DB& db = DB::getInstance();
    db.open();
    // select test
    vector<UserData> tmp = db.select_all_user_data();
    for (const auto &x: tmp) {
        x.print();
        cout << endl;
    }
    return 0;
}