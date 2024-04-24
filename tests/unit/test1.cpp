#include <gtest/gtest.h>
#include "cppSocketLib.hpp"


TEST(UDPConnectionTestIPv4, BindSuccess) {
    UDPConnection connection("127.0.0.1", "12345", true, false);
    EXPECT_TRUE(connection.bind());
}

TEST(UDPConnectionTestIPv4, BindFailure) {
    UDPConnection connection("127.0.0.0.1", "65536", false, false);
    // EXPECT_THROW(connection.bind(), std::runtime_error); // Tira la excepción pero falla el test
    EXPECT_FALSE(connection.bind());
}

TEST(UDPConnectionTestIPv4, ConnectSuccess) {
    
}

TEST(UDPConnectionTestIPv4, ConnectFailure) {
    
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
