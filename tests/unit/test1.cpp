#include <gtest/gtest.h>

int suma(int a, int b) {
    return a + b;
}

TEST(SumaTest, SumaBasica) {
    int a = 3;
    int b = 5;

    int resultado = suma(a, b);

    EXPECT_EQ(resultado, 8);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}