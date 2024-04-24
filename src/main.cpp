#include "cppSocketLib.hpp"

IConnection::IConnection(const std::string &address, const std::string &port,
                         bool isBlocking)
    : address_(address), m_port_(port), isBlocking_(isBlocking) {}

IConnection::~IConnection() {}

TCPv4Connection::TCPv4Connection(const std::string &address,
                                 const std::string &port, bool isBlocking)
    : IConnection(address, port, isBlocking) {
  // Implementación del constructor aquí
}

bool TCPv4Connection::bind() {
  // Implementación del método bind
  return true;
}

int TCPv4Connection::connect() {
  // Implementación del método connect
  return 0;
}
int TCPv4Connection::getSocket() { return 0; }

bool TCPv4Connection::send(const std::string &message) {
  // Implementación del método send
  return true;
}

std::string TCPv4Connection::receive() {
  // Implementación del método receive
  return "";
}

bool TCPv4Connection::changeOptions() {
  // Implementación del método changeOptions
  return true;
}

TCPv6Connection::TCPv6Connection(const std::string &address,
                                 const std::string &port, bool isBlocking)
    : IConnection(address, port, isBlocking) {
  // Implementación del constructor aquí
}

bool TCPv6Connection::bind() {
  // Implementación del método bind
  return true;
}

int TCPv6Connection::getSocket() { return 0; }

int TCPv6Connection::connect() {
  // Implementación del método connect
  return 0;
}

bool TCPv6Connection::send(const std::string &message) {
  // Implementación del método send
  return true;
}

std::string TCPv6Connection::receive() {
  // Implementación del método receive
  return "";
}

bool TCPv6Connection::changeOptions() {
  // Implementación del método changeOptions
  return true;
}

UDPConnection::UDPConnection(const std::string &address,
                             const std::string &port, bool isBlocking, bool IPv6)
    : IConnection(address, port, isBlocking) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));

  if (IPv6) {
    hints.ai_family = AF_INET6;
  } else {
    hints.ai_family = AF_INET;
  }
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = 0;

  if (address.empty()) {
    hints.ai_flags = AI_PASSIVE;
  }

  addrinfo *rawAddrinfo =
      nullptr;  // Raw pointer to store the result of getaddrinfo
  int res = getaddrinfo(address.c_str(), port.c_str(), &hints, &rawAddrinfo);
  if (res != 0) {
    std::cerr << "Error getting address" << std::endl;
    exit(1);
  }
  m_addrinfo = std::unique_ptr<addrinfo>(
      rawAddrinfo);  // Wrap raw pointer in the smart pointer

  m_socket = socket(m_addrinfo->ai_family, m_addrinfo->ai_socktype,
                    m_addrinfo->ai_protocol);

  if (m_socket < 0) {
    throw std::runtime_error("Error creating socket");
  }
  if (!isBlocking) {
    int flags = fcntl(m_socket, F_GETFL, 0);
    fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);
  }
}

UDPConnection::~UDPConnection() {}

int UDPConnection::getSocket() { return m_socket; }

bool UDPConnection::bind() {
  // Bind socket to address
  if (::bind(m_socket, m_addrinfo->ai_addr, m_addrinfo->ai_addrlen) < 0) {
    throw std::runtime_error("Error binding socket to address: ");
    return false;
  }

  return true;
}

int UDPConnection::connect() {
  // Connect socket to address
  if (::connect(m_socket, m_addrinfo->ai_addr, m_addrinfo->ai_addrlen) < 0) {
    throw std::runtime_error("Error getting address");
  }

  return true;
}

bool UDPConnection::send(const std::string &message) {
  ssize_t sentBytes = ::send(m_socket, message.c_str(), message.size(), 0);
  if (sentBytes == ERROR) {
    std::cout << "Error sending data: " << strerror(errno) << std::endl;
    return false;
  } else if (static_cast<size_t>(sentBytes) != message.size()) {
    std::cout << "Incomplete data sent" << std::endl;
    return false;
  }
  return true;
}

std::string UDPConnection::receive() {
  std::vector<char> recvMessage(MESSAGE_LENGTH, 0);

  int bytesReceived =
      ::recv(m_socket, recvMessage.data(), recvMessage.size(), 0);

  if (bytesReceived < 0) {
    std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  return std::string(recvMessage.begin(), recvMessage.begin() + bytesReceived);
}

bool UDPConnection::changeOptions() {
  // Implementación del método changeOptions
  return true;
}

std::unique_ptr<IConnection> createConnection(const std::string &address,
                                              const std::string &port,
                                              bool isBlocking,
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

class ThreadManager {
 public:
  ThreadManager(std::jthread thread1, std::jthread thread2,
                std::jthread thread3, std::jthread thread4)
      : thread1_(std::move(thread1)),
        thread2_(std::move(thread2)),
        thread3_(std::move(thread3)),
        thread4_(std::move(thread4)) {}

  ~ThreadManager() {
    thread3_.join();
    thread4_.join();
    thread1_.join();
    thread2_.join();
  }

 private:
  std::jthread thread1_;
  std::jthread thread2_;
  std::jthread thread3_;
  std::jthread thread4_;
};

int main() {
  std::shared_ptr<IConnection> con1 =
      createConnection("::1", "7658", true, UDP);
  std::shared_ptr<IConnection> con2 =
      createConnection("::1", "7658", true, UDP);
  std::shared_ptr<IConnection> con3 =
      createConnection("127.0.0.2", "7672", true, UDP);
  std::shared_ptr<IConnection> con4 =
      createConnection("127.0.0.2", "7672", true, UDP);

  ThreadManager threads(
      std::jthread([&con1]() {
        try {
          con1->bind();
          std::string message = con1->receive();
          std::cout << "Received message: " << message << std::endl;
        } catch (const std::exception &e) {
          std::cerr << e.what() << std::endl;
        }
      }),
      std::jthread([&con2]() {
        try {
          con2->connect();
          con2->send("Hola mundo! IPv6");
        } catch (const std::exception &e) {
          std::cerr << e.what() << std::endl;
        }
      }),
      std::jthread([&con4]() {
        try {
          con4->connect();
          con4->send("Hola mundo! IPv4");
          // Process the received message from con2
        } catch (const std::exception &e) {
          std::cerr << e.what() << std::endl;
        }
      }),
      std::jthread([&con3]() {
        try {
          con3->bind();
          std::string message = con3->receive();
          std::cout << "Received message: " << message << std::endl;
          // Process the received message from con1
        } catch (const std::exception &e) {
          std::cerr << e.what() << std::endl;
        }
      }));

  return 0;
}
