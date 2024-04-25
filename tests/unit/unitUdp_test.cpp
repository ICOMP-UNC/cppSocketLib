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

#include "cppSocketLib.hpp"

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
