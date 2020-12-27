//
// Created by sheep on 2020/12/27.
//

#include <random>
#include <sys/time.h>
#include <map>
#include <iostream>
#include "RBTree.hpp"

using namespace std;

const int MAX = 1e7;
const int testSize = 1e6;

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<int> dis(-MAX, MAX);

vector<int> getRandomData() {
    vector<int> data;
    for (int i = 0; i < testSize; i++) {
        data.emplace_back(dis(mt));
    }
    return data;
}

// insert data test
auto single_insert_test(int testCase) {
    map<int, int> mp;
    RBTree<int, int> rbTree;
    timeval start, end;

    vector<int> data = getRandomData();

    int stlSec = 0, rbtSec = 0, stlUsec = 0, rbtUsec = 0;

    gettimeofday(&start, NULL);
    for (auto &x: data) {
        mp[x]++;
    }
    gettimeofday(&end, NULL);

    printf("case %d insert %d data\n", testCase, testSize);

    stlSec = end.tv_sec - start.tv_sec;
    stlUsec = end.tv_usec - start.tv_usec;

    printf("STL map run time %d.%06d s\n", stlSec, stlUsec);

    gettimeofday(&start, NULL);
    for (const auto &x: data) {
        rbTree[x]++;
    }
    gettimeofday(&end, NULL);

    rbtSec = end.tv_sec - start.tv_sec;
    rbtUsec = end.tv_usec - start.tv_usec;

    printf("RBTree run time %d.%06d s\n", rbtSec, rbtUsec);

    return std::tuple<int, int, int, int> (stlSec, stlUsec, rbtSec, rbtUsec);
}

void test_insert(int times) {
    int stlSec = 0, stlUsec = 0, rbtSec = 0, rbtUsec = 0;

    for (int i = 1; i <= times; i++) {
        auto [_stlSec, _stlUsec, _rbtSec, _rbtUsec] = single_insert_test(i);
        stlSec += _stlSec;
        stlUsec += _stlUsec;
        rbtSec += _rbtSec;
        rbtUsec += _rbtUsec;
    }

    printf("STL map average total time %d.%06d s\n", stlSec, stlUsec);

    printf("RBTree average total time %d.%06d s\n", rbtSec, rbtUsec);
}

void iterator_test() {
    vector<int> data = getRandomData();
    vector<int> iter_ans, inorder_ans;

    RBTree<int, int> rbTree;

    for (auto &x: data) {
        rbTree.insert(x);
    }

    for (auto it = rbTree.begin(); it != rbTree.end(); ++it) {
        iter_ans.push_back((*it).second);
    }

    inorder_ans = rbTree.getInorderTraversal(false);

    if (iter_ans.size() != inorder_ans.size()) {
        std::cout << "error in size" << std::endl;
        std::cout << iter_ans.size() << " " << inorder_ans.size() << std::endl;
        return;
    }
    for (int i = 0; i < iter_ans.size(); i++) {
        if (iter_ans[i] != inorder_ans[i]) {
            std::cout << "difference between " << iter_ans[i] << " " << inorder_ans[i] << std::endl;
            return;
        }
    }
    std::cout << "comparison done" << std::endl;
}
//
void single_delete_test(int testCase) {
    vector<int> data = getRandomData();

    map<int, int> mp;
    RBTree<int, int> rbTree;

    for (auto &x: data) {
        mp[x]++;
        rbTree[x]++;
    }

    timeval start, end;

    gettimeofday(&start, NULL);
    for (auto &x: data) {
        mp[x]--;
        if (mp[x] == 0) mp.erase(x);
    }
    gettimeofday(&end, NULL);

    printf("case %d delete %d data\n", testCase, testSize);

    printf("STL map run time %ld.%06ld s\n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    for (auto &x: data) {
        rbTree.remove(x);
    }
    gettimeofday(&end, NULL);

    printf("RBTree run time %ld.%06ld s\n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);

}
//
void single_query_test(int testCase) {
    vector<int> data = getRandomData();

    map<int, int> mp;
    RBTree<int, int> rbTree;

    for (auto &x: data) {
        mp[x]++;
        rbTree[x]++;
    }

    timeval start, end;

    printf("case %d query\n", testCase);

    gettimeofday(&start, NULL);
    printf("%d\n", mp[data[0]]);
    gettimeofday(&end, NULL);

    printf("STL map run time %ld.%010ld s\n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    printf("%d\n", rbTree[data[0]]);
    gettimeofday(&end, NULL);

    printf("RBTree run time %ld.%010ld s\n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);

}
//
void single_traversal_test(int testCase) {
    vector<int> data = getRandomData();

    map<int, int> mp;
    RBTree<int, int> rbTree;

    for (auto &x: data) {
        mp[x]++;
        rbTree[x]++;
    }

    timeval start, end;

    vector<int> rec;

    printf("case %d traversal\n", testCase);

    gettimeofday(&start, NULL);
    for (auto it = mp.begin(); it != mp.end(); ++it) {
        rec.push_back(it->first);
    }
    gettimeofday(&end, NULL);

    printf("STL map run time %ld.%06ld s\n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);

    rec.clear();

    gettimeofday(&start, NULL);
    for (auto it = rbTree.begin(); it != rbTree.end(); ++it) {
        rec.push_back(it->val);
    }
    gettimeofday(&end, NULL);

    printf("RBTree run time %ld.%06ld s\n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);

}

int main() {

//    for (int i = 0; i < 10; i++) {
//        std::cout << dis(mt) << " ";
//    }
//    test_insert(1);
//    iterator_test();
//    single_delete_test(1);
//    single_query_test(1);
    single_traversal_test(1);
//    RBTree<string, int> rbt;
//    rbt["asd"] = 1;
//    rbt["bcd"] = 2;
//    cout << rbt["asd"] << endl;
//    rbt["asd"]++;
//    cout << rbt["asd"];
    return 0;
}