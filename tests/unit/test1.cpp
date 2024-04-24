#include "cppSocketLib.hpp"
#include <gtest/gtest.h>

TEST(UDPConnectionTestIPv4, BindSuccess) {

    std::shared_ptr<IConnection> con = createConnection("127.0.0.1", "65536", false, UDP);
    EXPECT_TRUE(con->bind());
}

TEST(UDPConnectionTestIPv4, CreateFailure) {

    try
    {
        std::shared_ptr<IConnection> con1 = createConnection("127.0.0.0.1", "65536", false, UDP);
    }
    catch(const std::exception& e)
    {
        EXPECT_THROW(throw std::runtime_error("Error getting address"), std::runtime_error);
    }
    
}

TEST(UDPConnectionTestIPv6, BindSuccess) {

    std::shared_ptr<IConnection> con = createConnection(":::1", "65536", false, UDP);
    EXPECT_TRUE(con->bind());
}

TEST(UDPConnectionTestIPv6, CreateFailure) {

    try
    {
        std::shared_ptr<IConnection> con1 = createConnection(":::1", "65536", false, UDP);
    }
    catch(const std::exception& e)
    {
        EXPECT_THROW(throw std::runtime_error("Error getting address"), std::runtime_error);
    }
    
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
