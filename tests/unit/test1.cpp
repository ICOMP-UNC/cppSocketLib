#include <gtest/gtest.h>
#include "cppSocketLib.hpp"


TEST(TCPConnectionTestIPv4, BindSuccess) {
    TCPv4Connection conn = TCPv4Connection("127.0.0.1","8080",false);
    
}



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}