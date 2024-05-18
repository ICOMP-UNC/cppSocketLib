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

#ifndef _CPP_SOCKET_LIB_HPP
#define _CPP_SOCKET_LIB_HPP

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

constexpr auto TCP = 1;                    // Macro for TCP
constexpr auto UDP = 2;                    // Macro for UDP
constexpr auto ERROR = -1;                 // Macro for error
constexpr auto MAX_MESSAGE_LENGTH = 10000; // Macro for message length
constexpr auto TCP_BACKLOG = 1024;         // Macro for maximum connections

/**
 * @brief Enumeration representing different network protocols.
 */
enum class Protocol
{
    TCPv4, ///< TCP IPv4 protocol.
    TCPv6, ///< TCP IPv6 protocol.
    UDPv4, ///< UDP IPv4 protocol.
    UDPv6  ///< UDP IPv6 protocol.
};

/**
 * @brief Abstract base class representing a network connection.
 */
class IConnection
{
public:
    /**
     * @brief Construct a new IConnection object.
     *
     * @param address IP address of the connection.
     * @param port Port number of the connection.
     * @param isBlocking Flag to set the connection as blocking or non-blocking.
     */
    IConnection(const std::string& address, const std::string& port, bool isBlocking);

    /**
     * @brief Destroy the IConnection object.
     */
    virtual ~IConnection();

    /**
     * @brief Bind the connection to a socket.
     *
     * @return true if the connection is successfully binded, false otherwise.
     */
    virtual bool bind() = 0;

    /**
     * @brief Connect the connection to a socket.
     *
     * @return int File descriptor of the socket.
     */
    virtual int connect() = 0;

    /**
     * @brief Send a message through the connection.
     *
     * @param message Message to be sent.
     * @return true if the message is successfully sent, false otherwise.
     */
    virtual bool send(const std::string& message) = 0;

    /**
     * @brief Send a message through a specific socket.
     *
     * @param message Message to be sent.
     * @param fdDestiny socket file descriptor to use to send message.
     * @return true if the message is successfully sent, false otherwise.
     */
    virtual bool sendto(const std::string& message, int fdDestiny) = 0;

    /**
     * @brief Receive a message through the connection.
     *
     * @return std::string Received message.
     */
    virtual std::string receive() = 0;

    /**
     * @brief Receive a message through a specific socket
     *
     * @return std::string Received message.
     */
    virtual std::string receiveFrom(int socket) = 0;

    /**
     * @brief Change the options of the connection.
     *
     * @return true if the options are successfully changed, false otherwise.
     */
    virtual bool changeOptions() = 0;

    /**
     * @brief Get the socket file descriptor.
     *
     * @return int File descriptor of the socket.
     */
    virtual int getSocket() = 0;

    /**
     * @brief Get the IP address of the connection.
     *
     * @return std::string IP address of the connection.
     */
    std::string GetPort()
    {
        return m_port;
    }

protected:
    std::string m_address; ///< IP address of the connection. */
    std::string m_port;    ///< Port number of the connection. */
    bool m_isBlocking;     ///< Flag to set the connection as blocking or non-blocking.*/
    int m_socket;          ///< File descriptor of the socket.
};

/**
 * @brief Class representing a TCP IPv4 connection.
 */
class TCPv4Connection : public IConnection
{
public:
    /**
     * @brief Constructor for TCPv4Connection class.
     *
     * @param address IP address of the remote host.
     * @param port Port number for the connection.
     * @param isBlocking Flag indicating whether the connection is blocking.
     */
    TCPv4Connection(const std::string& address, const std::string& port, bool isBlocking);

    ~TCPv4Connection();

    /**
     * @brief Bind the connection to a socket.
     *
     * @return true if the connection is successfully binded, false otherwise.
     */
    bool bind() override;

    /**
     * @brief Connect the connection to a socket.
     *
     * @return int File descriptor of the socket.
     */
    int connect() override;

    /**
     * @brief Send a message through the connection.
     *
     * @param message Message to be sent.
     * @return true if the message is successfully sent, false otherwise.
     */
    bool send(const std::string& message) override;

    /**
     * @brief Send a message through a specific socket.
     *
     * @param message Message to be sent.
     * @param fdDestiny socket file descriptor to use to send message.
     * @return true if the message is successfully sent, false otherwise.
     */
    bool sendto(const std::string& message, int fdDestiny) override;

    /**
     * @brief Receive a message through the connection.
     *
     * @return std::string Received message.
     */
    std::string receive() override;

    /**
     * @brief Receive a message through a specific socket
     *
     * @return std::string Received message.
     */
    std::string receiveFrom(int socket) override;

    /**
     * @brief Change the options of the connection.
     *
     * @return true if the options are successfully changed, false otherwise.
     */
    bool changeOptions() override;

    /**
     * @brief Get the socket file descriptor.
     *
     * @return int File descriptor of the socket.
     */
    int getSocket() override;

private:
    bool autoSelectPort;
    struct sockaddr_in addrinfo4;
    std::unique_ptr<addrinfo> m_addrinfo;
    bool binded = false;
};

/**
 * @brief Class representing a TCP IPv6 connection.
 */
class TCPv6Connection : public IConnection
{
public:
    /**
     * @brief Construct a new TCPv6Connection object.
     *
     * @param address IP address of the connection.
     * @param port Port number of the connection.
     * @param isBlocking Flag to set the connection as blocking or non-blocking.
     */
    TCPv6Connection(const std::string& address, const std::string& port, bool isBlocking);

    ~TCPv6Connection();

    /**
     * @brief Bind the connection to a socket.
     *
     * @return true if the connection is successfully binded, false otherwise.
     */
    bool bind() override;

    /**
     * @brief Connect the connection to a socket.
     *
     * @return int File descriptor of the socket.
     */
    int connect() override;

    /**
     * @brief Send a message through the connection.
     *
     * @param message Message to be sent.
     * @return true if the message is successfully sent, false otherwise.
     */
    bool send(const std::string& message) override;

    /**
     * @brief Send a message through a specific socket.
     *
     * @param message Message to be sent.
     * @param fdDestiny socket file descriptor to use to send message.
     * @return true if the message is successfully sent, false otherwise.
     */
    bool sendto(const std::string& message, int fdDestiny) override;

    /**
     * @brief Receive a message through the connection.
     *
     * @return std::string Received message.
     */
    std::string receive() override;

    /**
     * @brief Receive a message through a specific socket
     *
     * @return std::string Received message.
     */
    std::string receiveFrom(int socket) override;

    /**
     * @brief Change the options of the connection.
     *
     * @return true if the options are successfully changed, false otherwise.
     */
    bool changeOptions() override;

    /**
     * @brief Get the socket file descriptor.
     *
     * @return int File descriptor of the socket.
     */
    int getSocket() override;

private:
    struct sockaddr_in6 addrinfo6;
    struct addrinfo* addrinfo = NULL;
    bool binded = false;
};

/**
 * @brief Class representing a UDP IPv6 and IPv4 connections.
 */
class UDPConnection : public IConnection
{
public:
    /**
     * @brief
     * - Construct a new UDPConnection object.
     * - Recognizes the IP version.
     * - Assigns the address and port to the connection.
     * - Sets the connection as blocking or non-blocking.
     * - Creates a socket for the connection.
     *
     * @param address IP address of the connection.
     * @param port Port number of the connection.
     * @param isBlocking Flag to set the connection as blocking or non-blocking.
     * @param IPv6 Flag to set the connection as IPv6 or IPv4.
     */
    UDPConnection(const std::string& address, const std::string& port, bool isBlocking, bool IPv6);

    /**
     * @brief Destroy the UDPConnection object.
     */
    ~UDPConnection();

    /**
     * @brief Bind the connection to a socket.
     *
     * @return true if the connection is successfully binded, false otherwise.
     */
    bool bind() override;

    /**
     * @brief Connects the socket to the address specified.
     *
     * @return int File descriptor of the socket.
     */
    int connect() override;

    /**
     * @brief Send a message through the connection.
     *
     * @param message Message to be sent.
     * @return true if the message is successfully sent, false if data is not sent
     * or is incomplete.
     */
    bool send(const std::string& message) override;

    /**
     * @brief Currently this method does the same as send().
     * @param message Message to be sent.
     */
    bool sendto(const std::string& message, int /*fdDestiny*/) override
    {
        return send(message);
    };

    /**
     * @brief Receive a message through the connection.
     *
     * @return std::string Received message. EXIT_FAILURE if the message is not
     * received.
     */
    std::string receive() override;

    /**
     * @brief Currently this method does the same as receive().
     */
    std::string receiveFrom(int /*socket*/) override
    {
        return receive();
    };

    /**
     * @brief Change the options of the connection.
     *
     * @return true if the options are successfully changed, false otherwise.
     */
    bool changeOptions() override;

    /**
     * @brief Get the socket file descriptor.
     *
     * @return int File descriptor of the socket.
     */
    int getSocket() override;

private:
    bool isIPv6, autoSelectPort;          ///< Flag to set the connection as blocking or non-blocking.*/
    struct sockaddr_in6 address6;         ///< IP address of the connection. */
    struct sockaddr_in address4;          ///< IP address of the connection. */
    std::unique_ptr<addrinfo> m_addrinfo; ///< Smart pointer for addrinfo */
};

/**
 * @brief Factory function to create a connection.
 *
 * @param address IP address of the connection.
 * @param port Port number of the connection.
 * @param isBlocking Flag to set the connection as blocking or non-blocking.
 * @param protocolMacro Macro representing the network protocol.
 * @return IConnection* Pointer to the created connection.
 */
std::unique_ptr<IConnection>
createConnection(const std::string& address, const std::string& port, bool isBlocking, int protocolMacro);

#endif // _CPP_SOCKET_LIB_HPP
