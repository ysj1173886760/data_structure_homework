//
// Created by dbdxzwh on 2020/12/27.
//

#include "ServiceSystem.h"
#include <algorithm>

bool cmp(const std::pair<double, int>& p1, const std::pair<double, int>& p2) {
    return p1.first > p2.first;
}

std::string ServiceSystem::GetHighestSim(const std::vector<std::string>& v, const std::string& s) {
    BasicOperation op;
    std::vector<std::pair<double, int>> sim;
    for(int i=0; i<v.size(); i++) {
        std::pair<double, int> add;
        add.first = op.GetWordsSim(v[i], s);
        add.second = i;
        sim.push_back(add);
    }
    sort(sim.begin(), sim.end(), cmp);
    if(sim[0].first == 1)
        return v[sim[0].second];
    else {
        std::cout << "Did you mean " << v[sim[0].second] << "?" << std::endl;
        return v[sim[0].second];
    }
}