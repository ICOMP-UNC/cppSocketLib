// connection.hpp - Header file for connection.cpp
#ifndef CONNECTION_H
#define CONNECTION_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#define TCP 1   // Macro for TCP
#define UDP 2   // Macro for UDP
#define ERROR -1   // Macro for error
#define MESSAGE_SIZE 1024  // Buffer size for messages 

enum class Protocol { TCPv4, TCPv6, UDPv4, UDPv6 };

class IConnection {
 public:
  
  IConnection(const std::string &address, const std::string &port,
              bool isBlocking);
  virtual ~IConnection();

  virtual bool bind() = 0;
  virtual int connect() = 0;
  virtual bool send(const std::string &message) = 0;
  virtual std::string receive() = 0;
  virtual bool changeOptions() = 0;

  int getSocket();

 protected:
  std::string address_;
  std::string m_port_;
  bool isBlocking_;
  int socket_fd_;
};

class TCPv4Connection : public IConnection {
 public:
  TCPv4Connection(const std::string &address, const std::string &port,
                  bool isBlocking);
  bool bind() override;
  int connect() override;
  bool send(const std::string &message) override;
  std::string receive() override;
  bool changeOptions() override;
};

class TCPv6Connection : public IConnection {
 public:
  TCPv6Connection(const std::string &address, const std::string &port,
                  bool isBlocking);
  bool bind() override;
  int connect() override;
  bool send(const std::string &message) override;
  std::string receive() override;
  bool changeOptions() override;
};





class UDPv4Connection : public IConnection {
 public:

    

  UDPv4Connection(const std::string &address, const std::string &port,
                  bool isBlocking);
  bool bind() override;
  int connect() override;
  bool send(const std::string &message) override;
  std::string receive() override;
  bool changeOptions() override;
};

class UDPConnection : public IConnection {
 public:
  UDPConnection(const std::string &address, const std::string &port,
                bool isBlocking);
  ~UDPConnection();

  bool bind() override;
  int connect() override;
  bool send(const std::string &message) override;
  std::string receive() override;
  bool changeOptions() override;

 private:
  struct addrinfo *addrinfo;
  int m_socket;
};

IConnection *createConnection(const std::string &address,
                              const std::string &port, bool isBlocking,
                              int protocolMacro);

#endif  // CONNECTION_H