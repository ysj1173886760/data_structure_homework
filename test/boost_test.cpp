//
// Created by sheep on 2020/12/25.
//

#include <boost/timer/timer.hpp>
#include <boost/timer/progress_display.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace boost::timer;

int main() {
    vector<string> v(100, "aaa");
    v[1] = "";
    v[53] = "";

    progress_display pd(v.size());
    for (auto pos = v.begin(); pos != v.end(); pos++) {
        ++pd;
        if (pos->empty()) {
            cout << "string empty : " << pos - v.begin() << endl;
        }
    }
    return 0;
}
