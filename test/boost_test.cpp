//
// Created by sheep on 2020/12/25.
//

#include "IDgenerator.h"
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;
using namespace boost;

int main()
{
    IDgenerator& instance = IDgenerator::get_instance();
    std::cout << instance.generateID(Type::user);
    return 0;
}

//int main() {
//    vector<string> v(100, "aaa");
//    v[1] = "";
//    v[53] = "";
//
//    progress_display pd(v.size());
//    for (auto pos = v.begin(); pos != v.end(); pos++) {
//        ++pd;
//        if (pos->empty()) {
//            cout << "string empty : " << pos - v.begin() << endl;
//        }
//    }
//    return 0;
//}
