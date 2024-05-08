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

IConnection::IConnection(const std::string &address, const std::string &port, bool isBlocking)
    : m_address(address), m_port(port), m_isBlocking(isBlocking)
{
}

IConnection::~IConnection()
{
}

TCPv4Connection::TCPv4Connection(const std::string &address, const std::string &port, bool isBlocking)
    : IConnection(address, port, isBlocking)
{
    struct addrinfo hints;
    hints.ai_flags = 0;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (address.empty())
    {
        // for listening connections
        hints.ai_flags = AI_PASSIVE; // ANY_ADDRESS
    }

    addrinfo *raw_addrinfo = nullptr;
    int resultAddrInfo = getaddrinfo(address.c_str(), port.c_str(), &hints, &raw_addrinfo);

    if (resultAddrInfo != 0)
    {
        throw std::runtime_error(gai_strerror(resultAddrInfo));
    }

    m_addrinfo = std::unique_ptr<addrinfo>(raw_addrinfo);

    m_socket = socket(m_addrinfo->ai_family, m_addrinfo->ai_socktype, m_addrinfo->ai_protocol);

    if (m_socket < 0)
    {
        throw std::runtime_error("Error creating socket");
    }
}

TCPv4Connection::~TCPv4Connection()
{
    ::close(m_socket);
}

bool TCPv4Connection::bind()
{
    if (::bind(m_socket, m_addrinfo->ai_addr, m_addrinfo->ai_addrlen) < 0)
    {
        throw std::runtime_error("Error: cannot bind socket");
    }

    int resultListen = ::listen(m_socket, TCP_BACKLOG); // Escuchar conexiones entrantes
    if (resultListen < 0)
    {
        throw std::runtime_error("Error: cannot listen on socket");
    }

    binded = true;

    return true;
}

int TCPv4Connection::connect()
{
    if (binded)
    {
        // for server accept new connection
        int clientFd = ::accept(m_socket, NULL, NULL); // Aceptar una conexión entrante
        if (clientFd < 0)
        {
            throw std::runtime_error("Error: cannot accept connection");
        }

        return clientFd;
    }

    // connect client
    if (::connect(m_socket, m_addrinfo->ai_addr, m_addrinfo->ai_addrlen) == -1)
    {
        throw std::runtime_error("Error: cannot connect");
    }

    return true;
}

bool TCPv4Connection::send(const std::string &message)
{
    int numberBytes = ::send(m_socket, message.c_str(), message.size(), 0); // contesta al cliente mediante el mismo fd
    if (numberBytes < 0)
    {
        throw std::runtime_error("Error: message sending failure");
    }
    return true;
}

bool TCPv4Connection::sendto(const std::string &message, int fdDestiny)
{
    int numberBytes = ::send(fdDestiny, message.c_str(), message.size(), 0); // contesta al cliente mediante el mismo fd
    if (numberBytes < 0)
    {
        throw std::runtime_error("Error: message sending failure");
    }
    return true;
}

std::string TCPv4Connection::receiveFrom(int socket)
{
    std::vector<char> recvMessage;
    recvMessage.resize(MAX_MESSAGE_LENGTH);

    int bytesReceived = ::recv(socket, recvMessage.data(), recvMessage.size(), 0);
    if (bytesReceived < 0)
    {
        throw std::runtime_error("Error: failed to receive message");
    }
    else if (bytesReceived == 0)
    {
        throw std::runtime_error("Connection closed by peer receiveFrom");
    }

    recvMessage.resize(bytesReceived);
    return std::string(recvMessage.begin(), recvMessage.end());
}

std::string TCPv4Connection::receive()
{
    std::vector<char> recvMessage;
    recvMessage.resize(MAX_MESSAGE_LENGTH);

    int bytesReceived = ::recv(m_socket, recvMessage.data(), recvMessage.size(), 0);
    if (bytesReceived < 0)
    {
        throw std::runtime_error("Error: failed to receive message");
    }
    else if (bytesReceived == 0)
    {
        throw std::runtime_error("Connection closed by peer receive");
    }

    recvMessage.resize(bytesReceived);
    return std::string(recvMessage.begin(), recvMessage.end());
}

bool TCPv4Connection::changeOptions()
{
    throw std::runtime_error("Not implemented.");
}

int TCPv4Connection::getSocket()
{
    return m_socket;
}

TCPv6Connection::TCPv6Connection(const std::string &address, const std::string &port, bool isBlocking)
    : IConnection(address, port, isBlocking)
{
    struct addrinfo hints;
    hints.ai_flags = 0;
    hints.ai_family = AF_INET6; // IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (address.empty())
    {
        // for listening connections
        hints.ai_flags = AI_PASSIVE; // ANY_ADDRESS
    }

    int resultAddrInfo = getaddrinfo(address.empty() ? nullptr : address.c_str(), port.c_str(), &hints, &addrinfo);

    if (resultAddrInfo != 0)
    {
        throw std::runtime_error(gai_strerror(resultAddrInfo));
    }
    m_socket = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);

    if (m_socket < 0)
    {
        throw std::runtime_error("Error creating socket");
    }
}

TCPv6Connection::~TCPv6Connection()
{
    close(m_socket);
    if (addrinfo != NULL)
    {
        freeaddrinfo(addrinfo);
    }
}

bool TCPv6Connection::bind()
{
    if (::bind(m_socket, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0)
    {
        throw std::runtime_error("Error: cannot bind socket");
    }

    int resultListen = ::listen(m_socket, TCP_BACKLOG); // Listen for incoming connections.
    if (resultListen < 0)
    {
        throw std::runtime_error("Error: cannot listen on socket");
    }
    binded = true;

    return binded;
}

int TCPv6Connection::connect()
{
    if (binded)
    {
        int clientFd = ::accept(m_socket, NULL, NULL);
        if (clientFd < 0)
        {
            throw std::runtime_error("Error: cannot accept connection");
        }
        return clientFd;
    }

    if (::connect(m_socket, addrinfo->ai_addr, addrinfo->ai_addrlen) == -1)
    {
        throw std::runtime_error("Error: cannot connect");
    }
    binded = true;

    return true;
}

bool TCPv6Connection::send(const std::string &message)
{
    int numberBytes = ::send(m_socket, message.c_str(), message.size(), 0);
    if (numberBytes < 0)
    {
        throw std::runtime_error("Error: message sending failure");
    }
    return true;
}

std::string TCPv6Connection::receiveFrom(int socket)
{
    std::vector<char> recvMessage;
    recvMessage.reserve(MAX_MESSAGE_LENGTH);

    int bytesReceived = ::recv(m_socket, recvMessage.data(), recvMessage.size(), 0);
    if (bytesReceived < 0)
    {
        throw std::runtime_error("Error: failed to receive message");
    }
    else if (bytesReceived == 0)
    {
        throw std::runtime_error("Connection closed by peer");
    }

    recvMessage.resize(bytesReceived);
    return std::string(recvMessage.begin(), recvMessage.end());
}

std::string TCPv6Connection::receive()
{
    std::vector<char> recvMessage;
    recvMessage.reserve(MAX_MESSAGE_LENGTH);

    int bytesReceived = ::recv(m_socket, recvMessage.data(), recvMessage.size(), 0);
    if (bytesReceived < 0)
    {
        throw std::runtime_error("Error: failed to receive message");
    }
    else if (bytesReceived == 0)
    {
        throw std::runtime_error("Connection closed by peer");
    }

    recvMessage.resize(bytesReceived);
    return std::string(recvMessage.begin(), recvMessage.end());
}

bool TCPv6Connection::changeOptions()
{
    throw std::runtime_error("Not implemented.");
}

int TCPv6Connection::getSocket()
{
    return m_socket;
}

UDPConnection::UDPConnection(const std::string &address, const std::string &port, bool isBlocking, bool IPv6)
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

    addrinfo *rawAddrinfo = nullptr; // Raw pointer to store the result of getaddrinfo

    const auto resultAddrInfo = getaddrinfo(address.c_str(), port.c_str(), &hints, &rawAddrinfo);
    if (resultAddrInfo != 0)
    {
        throw std::runtime_error("Error getting address");
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

bool UDPConnection::send(const std::string &message)
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
        throw std::runtime_error(errorMessage);
    }
    recvMessage.resize(bytesReceived);

    return std::string(recvMessage.begin(), recvMessage.end());
}
bool UDPConnection::changeOptions()
{
    throw std::runtime_error("Not implemented");
}

std::unique_ptr<IConnection> createConnection(const std::string &address, const std::string &port, bool isBlocking,
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
