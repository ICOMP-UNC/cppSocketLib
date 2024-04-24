/*
 * DatabaseManagment - cppSocketWrapper
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

#include "main.hpp"

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

    addrinfo* raw_addrinfo = nullptr;
    int res = getaddrinfo(address.c_str(), port.c_str(), &hints, &raw_addrinfo);

    if (res != 0)
    {
        throw std::runtime_error(gai_strerror(res));
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
    close(m_socket);
}

bool TCPv4Connection::bind()
{
    if (::bind(m_socket, m_addrinfo->ai_addr, m_addrinfo->ai_addrlen) < 0)
    {
        throw std::runtime_error("Error: cannot bind socket");
    }

    int res = ::listen(m_socket, TCP_BACKLOG); // Escuchar conexiones entrantes
    if (res < 0)
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
        int client_fd = ::accept(m_socket, NULL, NULL); // Aceptar una conexión entrante
        if (client_fd < 0)
        {
            throw std::runtime_error("Error: cannot accept connection");
        }

        return client_fd;
    }

    // connect client
    if (::connect(m_socket, m_addrinfo->ai_addr, m_addrinfo->ai_addrlen) == -1)
    {
        throw std::runtime_error("Error: cannot connect");
    }

    return true;
}

bool TCPv4Connection::send(const std::string& message)
{
    const char* messagePtr = message.c_str();
    int n = ::send(m_socket, messagePtr, message.size(), 0); // contesta al cliente mediante el mismo fd
    if (n < 0)
    {
        throw std::runtime_error("Error: message sending failure");
    }
    return true;
}

bool TCPv4Connection::sendto(const std::string& message, int fdDestiny)
{
    const char* messagePtr = message.c_str();
    int n = ::send(fdDestiny, messagePtr, message.size(), 0); // contesta al cliente mediante el mismo fd
    if (n < 0)
    {
        throw std::runtime_error("Error: message sending failure");
    }
    return true;
}

std::string TCPv4Connection::receiveFrom(int socket)
{
    char buffer[MESSAGE_SIZE] = "";

    int n = ::recv(socket, buffer, sizeof(buffer) - 1, 0);
    if (n < 0)
    {
        throw std::runtime_error("Error: failed to receive message");
    }
    else if (n == 0)
    {
        throw std::runtime_error("Connection closed by peer");
    }

    return std::string(buffer, n);
}

std::string TCPv4Connection::receive()
{
    char buffer[MESSAGE_SIZE] = "";

    int n = ::recv(m_socket, buffer, sizeof(buffer) - 1, 0);
    if (n < 0)
    {
        throw std::runtime_error("Error: failed to receive message");
    }
    else if (n == 0)
    {
        throw std::runtime_error("Connection closed by peer");
    }

    return std::string(buffer, n);
}

bool TCPv4Connection::changeOptions()
{
    // Implementación del método changeOptions
    return true;
}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

TCPv6Connection::TCPv6Connection(const std::string& address, const std::string& port, bool isBlocking)
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

    int res = getaddrinfo(address.empty() ? nullptr : address.c_str(), port.c_str(), &hints, &addrinfo);

    if (res != 0)
    {
        throw std::runtime_error(gai_strerror(res));
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

    int res = ::listen(m_socket, TCP_BACKLOG); // Listen for incoming connections.
    if (res < 0)
    {
        throw std::runtime_error("Error: cannot listen on socket");
    }

    binded = true;
    return true;
}

int TCPv6Connection::connect()
{
    if (binded)
    {
        int client_fd = ::accept(m_socket, NULL, NULL);
        if (client_fd < 0)
        {
            throw std::runtime_error("Error: cannot accept connection");
        }
        return client_fd;
    }

    if (::connect(m_socket, addrinfo->ai_addr, addrinfo->ai_addrlen) == -1)
    {
        throw std::runtime_error("Error: cannot connect");
    }
    return true;
}

bool TCPv6Connection::send(const std::string& message)
{
    const char* messagePtr = message.c_str();
    int n = ::send(m_socket, messagePtr, message.size(), 0);
    if (n < 0)
    {
        throw std::runtime_error("Error: message sending failure");
    }
    return true;
}

std::string TCPv6Connection::receive()
{
    char buffer[MESSAGE_SIZE] = "";

    int n = ::recv(m_socket, buffer, sizeof(buffer) - 1, 0);
    if (n < 0)
    {
        throw std::runtime_error("Error: failed to receive message");
    }
    else if (n == 0)
    {
        throw std::runtime_error("Connection closed by peer");
    }

    return std::string(buffer, n);
}

bool TCPv6Connection::changeOptions()
{
    // Implementación del método changeOptions
    return true;
}

// -----------------------------------------------
// -----------------------------------------------
// -----------------------------------------------
// -----------------------------------------------

UDPConnection::UDPConnection(const std::string& address, const std::string& port, bool isBlocking)
    : IConnection(address, port, isBlocking)
{
}

/**
 * @brief Destroy the UDPConnection object.
 */
UDPConnection::~UDPConnection()
{
}

/**
 * @brief Bind the connection to a socket.
 *
 * @return true if the connection is successfully binded, false otherwise.
 */
bool UDPConnection::bind()
{
    return true;
}

/**
 * @brief Connects the socket to the address specified.
 *
 * @return int File descriptor of the socket.
 */
int UDPConnection::connect()
{
    return true;
}

/**
 * @brief Send a message through the connection.
 *
 * @param message Message to be sent.
 * @return true if the message is successfully sent, false if data is not sent
 * or is incomplete.
 */
bool UDPConnection::send(const std::string& message)
{
    return true;
}

/**
 * @brief Receive a message through the connection.
 *
 * @return std::string Received message. EXIT_FAILURE if the message is not
 * received.
 */
std::string UDPConnection::receive()
{
    return std::string("h");
}

/**
 * @brief Change the options of the connection.
 *
 * @return true if the options are successfully changed, false otherwise.
 */
bool UDPConnection::changeOptions()
{
    return true;
}

/**
 * @brief Factory function to create a connection object based on the protocol.
 *
 * @param address IP address of the remote host.
 * @param port Port number for the connection.
 * @param isBlocking Flag indicating whether the connection is blocking.
 * @param protocolMacro Macro representing the protocol (TCP or UDP).
 * @return Pointer to the created IConnection object.
 */
IConnection* createConnection(const std::string& address, const std::string& port, bool isBlocking, int protocolMacro)
{
    Protocol protocol;

    // Determine if the address is IPv4 or IPv6
    bool isIPv6 = address.find(':') != std::string::npos;

    // Determine the protocol based on the macro
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
        return new TCPv4Connection(address, port, isBlocking);
    case Protocol::TCPv6:
        return new TCPv6Connection(address, port, isBlocking);
    case Protocol::UDPv4:
        return new UDPConnection(address, port, isBlocking);
    case Protocol::UDPv6:
        return new UDPConnection(address, port, isBlocking);
    default:
        throw std::invalid_argument("Unsupported protocol");
    }
}

int main()
{
    TCPv4Connection server = TCPv4Connection("127.0.0.1", "8080", false);
    TCPv4Connection client = TCPv4Connection("127.0.0.1", "8080", false);

    int chd = fork();
    if (chd == 0)
    {
        client.connect();
        std::string msg = client.receive();
        std::cout << "Client received: " << msg << std::endl;
        client.send("hello from client");
        exit(1);
    }

    server.bind();
    int client_fd = server.connect();
    server.sendto("hello from server", client_fd);
    std::string msg = server.receiveFrom(client_fd);
    std::cout << "Server received: " << msg << std::endl;

    int status;
    wait(&status);

    return 0;
}
