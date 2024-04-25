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

#include "gtest/gtest.h"
#include "cppSocketLib.hpp"

TEST(TCPConnectionTestIPv4, BindSuccess) {
    auto conn = TCPv4Connection("127.0.0.1","8080",false);
    
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

#endif // TCP_TEST_HPP