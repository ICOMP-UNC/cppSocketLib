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

#define TCP 1
#define UDP 2
#define ERROR -1
#define MESSAGE_SIZE 1024

enum class Protocol
{
    TCPv4,
    TCPv6,
    UDPv4,
    UDPv6
};

class IConnection {
 public:
  IConnection(const std::string &address, const std::string &port,
              bool isBlocking)
      : address_(address), m_port_(port), isBlocking_(isBlocking) {}

  virtual ~IConnection() {}

  virtual bool bind() = 0;
  virtual int connect() = 0;
  virtual bool send(const std::string &message) = 0;
  virtual std::string receive() = 0;
  virtual bool changeOptions() = 0;

  int getSocket() { return socket_fd_; }

 protected:
  std::string address_;
  std::string m_port_;
  bool isBlocking_;
  int socket_fd_;
};

class TCPv4Connection : public IConnection {
 public:
  TCPv4Connection(const std::string &address, const std::string &port,
                  bool isBlocking)
      : IConnection(address, port, isBlocking) {}
  bool bind() override {
    // Implementación del método bind
    return true;
  }

  int connect() override {
    // Implementación del método connect
    return true;
  }

  bool send(const std::string &message) override {
    std::cout << "TCPv4 " << message << std::endl;
    // Implementación del método send
    return true;
  }

  std::string receive() override {
    // Implementación del método receive
    static std::string message = "message";
    return message;
  }

  bool changeOptions() override {
    // Implementación del método changeOptions
    return true;
  }
  // Implement the virtual methods here
};

class TCPv6Connection : public IConnection {
 public:
  TCPv6Connection(const std::string &address, const std::string &port,
                  bool isBlocking)
      : IConnection(address, port, isBlocking) {}
  bool bind() override {
    // Implementación del método bind
    return true;
  }

  int connect() override {
    // Implementación del método connect
    return true;
  }

  bool send(const std::string &message) override {
    // Implementación del método send
    std::cout << "TCPv6 " << message << std::endl;
    return true;
  }

  std::string receive() override {
    // Implementación del método receive
    static std::string message = "message";
    return message;
  }

  bool changeOptions() override {
    // Implementación del método changeOptions
    return true;
  }
  // Implement the virtual methods here
};

class UDPConnection : public IConnection {
 public:
  UDPConnection(const std::string &address, const std::string &port,
                bool isBlocking)
      : IConnection(address, port, isBlocking) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    bool isIPv6 = address.find(':') != std::string::npos;
    if (isIPv6) {
      hints.ai_family = AF_INET6;
    } else {
      hints.ai_family = AF_INET;
    }
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;

    if (address.empty()) {
      hints.ai_flags = AI_PASSIVE;
    }

    int res = getaddrinfo(address.c_str(), port.c_str(), &hints, &addrinfo);
    if (res != 0) {
      std::cerr << "Error getting address" << std::endl;
      exit(1);
    }
    m_socket = socket(addrinfo->ai_family, addrinfo->ai_socktype,
                        addrinfo->ai_protocol);
    // freeaddrinfo(addrinfo);
    if (m_socket< 0) {
      std::cerr << "Error creating socket" << std::endl;
      exit(1);
    }
    if (!isBlocking) {
      int flags = fcntl(m_socket, F_GETFL, 0);
      fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);
    }
  }

  ~UDPConnection() { freeaddrinfo(addrinfo); }

  bool bind() override {
    // Bind socket to address
    if (::bind(m_socket, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0) {
      throw std::runtime_error("Error binding socket to address: ");
      return false;
    }

    return true;
  }

  int connect() override {
    // Connect socket to address
    if (::connect(m_socket, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0) {
      throw std::runtime_error("Error getting address");
      return false;
    }

    return true;
  }

  bool send(const std::string &message) override {
    ssize_t sent_bytes = ::send(m_socket, message.c_str(), message.size(), 0);
    if (sent_bytes == ERROR) {
      std::cerr << "Error sending data: " << strerror(errno) << std::endl;
      return false;
    } else if (static_cast<size_t>(sent_bytes) != message.size()) {
      std::cerr << "Incomplete data sent" << std::endl;
      return false;
    }

    return true;
  }

  std::string receive() override {

    char recv_message[MESSAGE_SIZE];
    memset(recv_message, 0, MESSAGE_SIZE);

    socklen_t addr_len = sizeof(addrinfo);

    int bytes_received = ::recv(m_socket, recv_message, MESSAGE_SIZE, 0);

    if (bytes_received < 0) {
      std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }

    return recv_message;
  }

  bool changeOptions() override {
    // Implementación del método changeOptions
    return true;
  }

 private:
  struct addrinfo *addrinfo;
  int m_socket;
};

IConnection *createConnection(const std::string &address,
                              const std::string &port, bool isBlocking,
                              int protocolMacro) {
  Protocol protocol;

  // Determinar si la dirección es IPv4 o IPv6
  bool isIPv6 = address.find(':') != std::string::npos;

  // Determinar el protocolo basándose en el macro
  if (protocolMacro == TCP) {
    protocol = isIPv6 ? Protocol::TCPv6 : Protocol::TCPv4;
  } else if (protocolMacro == UDP) {
    protocol = isIPv6 ? Protocol::UDPv6 : Protocol::UDPv4;
  } else {
    throw std::invalid_argument("Unsupported protocol macro");
  }

  switch (protocol) {
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

int main() {
  IConnection *con1 = createConnection("::1", "7658", true, UDP);
  IConnection *con2 = createConnection("::1", "7658", true, UDP);
  IConnection *con3 = createConnection("127.0.0.2", "7672", true, UDP);
  IConnection *con4 = createConnection("127.0.0.2", "7672", true, UDP);

  std::jthread thread1([&]() {
    con1->bind();
    std::string message = con1->receive();
    std::cout << "Received message: " << message << std::endl;
    // Process the received message from con1
  });

  std::jthread thread2([&]() {
    con2->connect();
    con2->send("Hola mundo! IPv6");
    // Process the received message from con2
  });

  std::jthread thread4([&]() {
    con4->connect();
    con4->send("Hola mundo! IPv4");
    // Process the received message from con2
  });

  std::jthread thread3([&]() {
    con3->bind();
    std::string message = con3->receive();
    std::cout << "Received message: " << message << std::endl;
    // Process the received message from con1
  });

  // Wait for the threads to finish
  thread3.join();
  thread4.join();
  thread1.join();
  thread2.join();
  delete con1;
  delete con2;
  delete con3;
  delete con4;

  return 0;
}
