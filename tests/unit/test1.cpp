#include <gtest/gtest.h>

int sum(int a, int b) {
    return a + b;
}

TEST(sumTest, basicSum) {
    int a = 3;
    int b = 5;

    int result = sum(a, b);

    EXPECT_EQ(result, 8);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}