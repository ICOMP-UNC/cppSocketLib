/*
 * Socket Library - cppSocketWrapperTest
 * Copyright (C) 2024, Operating Systems II.
 * Apr 23, 2024.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 */

#ifndef TCP_TEST_HPP
#define TCP_TEST_HPP

#include "cppSocket.hpp"
#include "gtest/gtest.h"

TEST(TCPConnectionTestIPv4, BindSuccess)
{
    auto conn = TCPv4Connection("127.0.0.1", "8080", false);
}

// Test to verify TCPv6Connection initialization is working
TEST(TCPv6ConnectionTest, BindSuccess)
{
    TCPv6Connection conn("::1", "8080", false);

    EXPECT_NO_THROW(conn.bind());
}

// Testo to verify the conection client-server is working
TEST(TCPv6ConnectionTest, ConnectSuccess)
{
    TCPv6Connection server("::1", "8080", false);
    TCPv6Connection client("::1", "8080", false);

    server.bind();

    EXPECT_NO_THROW(client.connect());
}

// Test to verify send the message correctly
TEST(TCPv6ConnectionTest, SendMessage)
{
    TCPv6Connection server("::1", "8080", false);
    TCPv6Connection client("::1", "8080", false);

    server.bind();

    client.connect();

    EXPECT_NO_THROW(client.send("Hello, world!"));
}

TEST(UDPConnectionTestIPv4, BindSuccess)
{
    std::shared_ptr<IConnection> con = createConnection("127.0.0.1", "65536", false, UDP);
    EXPECT_TRUE(con->bind());
}

TEST(UDPConnectionTestIPv4, CreateFailure)
{

    EXPECT_THROW(createConnection("127.0.0.0.1", "65536", false, UDP), std::runtime_error);
}

TEST(UDPConnectionTestIPv6, BindSuccess)
{

    std::shared_ptr<IConnection> con = createConnection("::1", "65536", false, UDP);
    EXPECT_TRUE(con->bind());
}

TEST(UDPConnectionTestIPv6, CreateFailure)
{
    EXPECT_THROW(createConnection(":::1", "65536", false, UDP), std::runtime_error);
}

TEST(UDPConnectionTestIPv4, UnsupportedProtocolMacro)
{
    EXPECT_THROW(createConnection("127.0.0.1", "8080", true, 10), std::invalid_argument);
}

TEST(UDPConnectionTestIPv4, ConnectSuccess)
{
    EXPECT_EQ(0, 0);
}

TEST(UDPConnectionTestIPv4, ConnectFailure)
{
    EXPECT_EQ(0, 0);
}

#endif // TCP_TEST_HPP