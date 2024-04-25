#include <gtest/gtest.h>
#include "cppSocketLib.hpp"


TEST(TCPConnectionTestIPv4, BindSuccess) {
    TCPv4Connection conn = TCPv4Connection("127.0.0.1","8080",false);
    
}

// Test to verify TCPv6Connection initialization is working
TEST(TCPv6ConnectionTest, BindSuccess) {
    TCPv6Connection conn("::1", "8080", false);

    EXPECT_NO_THROW(conn.bind());
}

// Testo to verify the conection client-server is working
TEST(TCPv6ConnectionTest, ConnectSuccess) {
    TCPv6Connection server("::1", "8080", false);
    TCPv6Connection client("::1", "8080", false);

    server.bind();

    EXPECT_NO_THROW(client.connect());
}

// Test to verify send the message correctly
TEST(TCPv6ConnectionTest, SendMessage) {
    TCPv6Connection server("::1", "8080", false);
    TCPv6Connection client("::1", "8080", false);

    server.bind();

    client.connect();

    EXPECT_NO_THROW(client.send("Hello, world!"));
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}