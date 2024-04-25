/*
 * Socket Library - cppSocketWrapper
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

IConnection::IConnection(const std::string& address, const std::string& port, bool isBlocking)
    : m_address(address), m_port(port), m_isBlocking(isBlocking)
{
}

IConnection::~IConnection()
{
}

TCPv4Connection::TCPv4Connection(const std::string& address, const std::string& port, bool isBlocking)
    : IConnection(address, port, isBlocking)
{
    // Implementación del constructor aquí
}

bool TCPv4Connection::bind()
{
    // Implementación del método bind
    return true;
}

int TCPv4Connection::connect()
{
    // Implementación del método connect
    return 0;
}
int TCPv4Connection::getSocket()
{
    return 0;
}

bool TCPv4Connection::send(const std::string& message)
{
    // Implementación del método send
    return true;
}

std::string TCPv4Connection::receive()
{
    // Implementación del método receive
    return "";
}

bool TCPv4Connection::changeOptions()
{
    // Implementación del método changeOptions
    return true;
}

TCPv6Connection::TCPv6Connection(const std::string& address, const std::string& port, bool isBlocking)
    : IConnection(address, port, isBlocking)
{
    // Implementación del constructor aquí
}

bool TCPv6Connection::bind()
{
    // Implementación del método bind
    return true;
}

int TCPv6Connection::getSocket()
{
    return 0;
}

int TCPv6Connection::connect()
{
    // Implementación del método connect
    return 0;
}

bool TCPv6Connection::send(const std::string& message)
{
    // Implementación del método send
    return true;
}

std::string TCPv6Connection::receive()
{
    // Implementación del método receive
    return "";
}

bool TCPv6Connection::changeOptions()
{
    // Implementación del método changeOptions
    return true;
}

UDPConnection::UDPConnection(const std::string& address, const std::string& port, bool isBlocking, bool IPv6)
    : IConnection(address, port, isBlocking)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    if (IPv6)
    {
        hints.ai_family = AF_INET6;
    }
    else
    {
        hints.ai_family = AF_INET;
    }
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;

    if (address.empty())
    {
        hints.ai_flags = AI_PASSIVE;
    }

    addrinfo* rawAddrinfo = nullptr; // Raw pointer to store the result of getaddrinfo

    const auto addrInfo = getaddrinfo(address.c_str(), port.c_str(), &hints, &rawAddrinfo);
    if (addrInfo != 0)
    {
        throw std::runtime_error ("Error getting address");
    }
    
    m_addrinfo = std::unique_ptr<addrinfo>(rawAddrinfo); // Wrap raw pointer in the smart pointer

    m_socket = socket(m_addrinfo->ai_family, m_addrinfo->ai_socktype, m_addrinfo->ai_protocol);

    if (m_socket < 0)
    {
        throw std::runtime_error("Error creating socket");
    }
    if (!isBlocking)
    {
        int flags = fcntl(m_socket, F_GETFL, 0);
        fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);
    }
}

UDPConnection::~UDPConnection()
{
}

int UDPConnection::getSocket()
{
    return m_socket;
}

bool UDPConnection::bind()
{
    // Bind socket to address
    if (::bind(m_socket, m_addrinfo->ai_addr, m_addrinfo->ai_addrlen) < 0)
    {
        throw std::runtime_error("Error binding socket to address: ");
    }

    return true;
}

int UDPConnection::connect()
{
    // Connect socket to address
    if (::connect(m_socket, m_addrinfo->ai_addr, m_addrinfo->ai_addrlen) < 0)
    {
        throw std::runtime_error("Error in conection");
    }

    return true;
}

bool UDPConnection::send(const std::string& message)
{
    ssize_t sentBytes = ::send(m_socket, message.c_str(), message.size(), 0);
    if (sentBytes == ERROR)
    {
        std::cout << "Error sending data: " << strerror(errno) << std::endl;
        return false;
    }
    else if (static_cast<size_t>(sentBytes) != message.size())
    {
        std::cout << "Incomplete data sent" << std::endl;
        return false;
    }
    return true;
}

std::string UDPConnection::receive()
{
    std::vector<char> recvMessage;

    recvMessage.reserve(MAX_MESSAGE_LENGTH);

    int bytesReceived = ::recv(m_socket, recvMessage.data(), recvMessage.size(), 0);

    if (bytesReceived < 0)
    {
        const auto errorMessage = std::string("Error receiving data: ") + strerror(errno);
        throw std::runtime_error (errorMessage);
    }
    recvMessage.resize(bytesReceived);

    return std::string(recvMessage.begin(), recvMessage.end());

}
bool UDPConnection::changeOptions()
{
     throw std::runtime_error ("Not implemented");
}

std::unique_ptr<IConnection> createConnection(const std::string& address, const std::string& port, bool isBlocking,
                                              int protocolMacro)
{
    Protocol protocol;

    // Determinar si la dirección es IPv4 o IPv6
    bool isIPv6 = address.find(':') != std::string::npos;

    // Determinar el protocolo basándose en el macro
    if (protocolMacro == TCP)
    {
        protocol = isIPv6 ? Protocol::TCPv6 : Protocol::TCPv4;
    }
    else if (protocolMacro == UDP)
    {
        protocol = isIPv6 ? Protocol::UDPv6 : Protocol::UDPv4;
    }
    else
    {
        throw std::invalid_argument("Unsupported protocol macro");
    }

    switch (protocol)
    {
    case Protocol::TCPv4:
        return std::make_unique<TCPv4Connection>(address, port, isBlocking);
        break;
    case Protocol::TCPv6:
        return std::make_unique<TCPv6Connection>(address, port, isBlocking);
        break;
    case Protocol::UDPv4:
        return std::make_unique<UDPConnection>(address, port, isBlocking, false);
        break;
    case Protocol::UDPv6:
        return std::make_unique<UDPConnection>(address, port, isBlocking, true);
        break;
    default:
        throw std::invalid_argument("Unsupported protocol");
    }
}
