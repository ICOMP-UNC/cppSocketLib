#include "libraryCpp.hpp"

IConnection::IConnection(const std::string &address, const std::string &port,
                         bool isBlocking)
    : address_(address), m_port_(port), isBlocking_(isBlocking) {}

IConnection::~IConnection() {}

int IConnection::getSocket() { return socket_fd_; }

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
                             const std::string &port, bool isBlocking)
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

  addrinfo *raw_addrinfo =
      nullptr;  // Raw pointer to store the result of getaddrinfo
  int res = getaddrinfo(address.c_str(), port.c_str(), &hints, &raw_addrinfo);
  if (res != 0) {
    std::cerr << "Error getting address" << std::endl;
    exit(1);
  }
  m_addrinfo = std::unique_ptr<addrinfo>(
      raw_addrinfo);  // Wrap raw pointer in the smart pointer

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
    return false;
  }

  return true;
}

bool UDPConnection::send(const std::string &message) {
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

std::string UDPConnection::receive() {
  std::vector<char> recv_message(MESSAGE_LENGTH, 0);
  socklen_t addr_len = sizeof(m_addrinfo);

  // int bytes_received = ::recv(m_socket, recv_message, MESSAGE_SIZE, 0);
  int bytes_received =
      ::recv(m_socket, recv_message.data(), recv_message.size(), 0);

  if (bytes_received < 0) {
    std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  return std::string(recv_message.begin(),
                     recv_message.begin() + bytes_received);
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
      return std::make_unique<UDPConnection>(address, port, isBlocking);
      break;
    case Protocol::UDPv6:
      return std::make_unique<UDPConnection>(address, port, isBlocking);
      break;
    default:
      throw std::invalid_argument("Unsupported protocol");
  }
}

int main() {
  std::shared_ptr<IConnection> con1 =
      std::make_shared<UDPConnection>("::1", "7658", true);
  std::shared_ptr<IConnection> con2 =
      std::make_shared<UDPConnection>("::1", "7658", true);
  std::shared_ptr<IConnection> con3 =
      std::make_shared<UDPConnection>("127.0.0.2", "7672", true);
  std::shared_ptr<IConnection> con4 =
      std::make_shared<UDPConnection>("127.0.0.2", "7672", true);

  std::jthread thread1([&con1]() {
    try {
      con1->bind();
      std::string message = con1->receive();
      std::cout << "Received message: " << message << std::endl;
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }

    // Process the received message from con1
  });

  std::jthread thread2([&con2]() {
    try {
      con2->connect();
      con2->send("Hola mundo! IPv6");
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  });

  std::jthread thread4([&con4]() {
    try {
      con4->connect();
      con4->send("Hola mundo! IPv4");
      // Process the received message from con2
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  });

  std::jthread thread3([&con3]() {
    try {
      con3->bind();
      std::string message = con3->receive();
      std::cout << "Received message: " << message << std::endl;
      // Process the received message from con1
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  });

  // Wait for the threads to finish
  thread3.join();
  thread4.join();
  thread1.join();
  thread2.join();

  return 0;
}