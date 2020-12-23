//
// Created by sheep on 2020/12/23.
//

#include <iostream>
#include "test.h"
#include "gtest/gtest.h"

int add(int a, int b) {
    return a + b;
}

TEST(test, c1) {
    EXPECT_EQ(4, add(1, 2));
}

GTEST_API_ int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}