#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>

#define TCP 1
#define UDP 2
#define MESSAGE_SIZE 1024

enum class Protocol
{
    TCPv4,
    TCPv6,
    UDPv4,
    UDPv6
};

class Connection
{
  public:
    Connection(const std::string &address, const std::string &port, bool isBlocking)
        : address_(address), port_(port), isBlocking_(isBlocking)
    {
    }

    virtual ~Connection()
    {
    }

    virtual bool bind() = 0;
    virtual int connect() = 0;
    virtual bool send(const std::string &message) = 0;
    virtual std::string receive() = 0;
    virtual bool changeOptions() = 0;

    int getSocket()
    {
        return socket_fd_;
    }

  protected:
    std::string address_;
    const std::string &port_;
    bool isBlocking_;
    int socket_fd_;
};

class TCPv4Connection : public Connection
{
  public:
    TCPv4Connection(const std::string &address, const std::string &port, bool isBlocking)
        : Connection(address, port, isBlocking)
    {
    }
    bool bind() override
    {
        // Implementación del método bind
        return true;
    }

    int connect() override
    {
        // Implementación del método connect
        return true;
    }

    bool send(const std::string &message) override
    {
        std::cout << "TCPv4 " << message << std::endl;
        // Implementación del método send
        return true;
    }

    std::string receive() override
    {
        // Implementación del método receive
        return "message";
    }

    bool changeOptions() override
    {
        // Implementación del método changeOptions
        return true;
    }
    // Implement the virtual methods here
};

class TCPv6Connection : public Connection
{
  public:
    TCPv6Connection(const std::string &address, const std::string &port, bool isBlocking)
        : Connection(address, port, isBlocking)
    {
    }
    bool bind() override
    {
        // Implementación del método bind
        return true;
    }

    int connect() override
    {
        // Implementación del método connect
        return true;
    }

    bool send(const std::string &message) override
    {
        // Implementación del método send
        std::cout << "TCPv6 " << message << std::endl;
        return true;
    }

    std::string receive() override
    {
        // Implementación del método receive
        return "message";
    }

    bool changeOptions() override
    {
        // Implementación del método changeOptions
        return true;
    }
    // Implement the virtual methods here
};

class UDPv4Connection : public Connection
{
  public:
    UDPv4Connection(const std::string &address, const std::string &port, bool isBlocking)
        : Connection(address, port, isBlocking)
    {
    }
    bool bind() override
    {
        // Implementación del método bind
        return true;
    }

    int connect() override
    {
        // Implementación del método connect
        return true;
    }

    bool send(const std::string &message) override
    {
        std::cout << "UDPv4 " << message << std::endl;
        return true;
    }

    std::string receive() override
    {
        // Implementación del método receive
        return "message";
    }

    bool changeOptions() override
    {
        // Implementación del método changeOptions
        return true;
    }
    // Implement the virtual methods here
};

class UDPv6Connection : public Connection
{
  public:
    UDPv6Connection(const std::string &address, const std::string &port, bool isBlocking)
        : Connection(address, port, isBlocking)
    {

        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET6;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = 0;

        if (address.empty())
        {
            hints.ai_flags = AI_PASSIVE;
        }

        int res = getaddrinfo(address.c_str(), port.c_str(), &hints, &addrinfo);
        if (res != 0)
        {
            std::cerr << "Error al obtener la dirección" << std::endl;
            exit(1);
        }
        socket_fd = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
        // freeaddrinfo(addrinfo);
        if (socket_fd < 0)
        {
            std::cerr << "Error al crear el socket" << std::endl;
            exit(1);
        }
        if (!isBlocking)
        {
            int flags = fcntl(socket_fd, F_GETFL, 0);
            fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
        }
    }

    ~UDPv6Connection()
    {
        freeaddrinfo(addrinfo);
    }

    int getPort()
    {
        return port_;
    }

    bool bind() override
    {
        // Vincular el socket a la dirección
        if (::bind(socket_fd, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0)
        {
            std::cerr << "Error al vincular el socket a la dirección: " << strerror(errno) << "\n" << std::endl;
            return false;
        }

        return true;
    }

    int connect() override
    {
        // Conectar el socket a la dirección
        if (::connect(socket_fd, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0)
        {
            std::cerr << "Error al conectar el socket a la dirección: " << strerror(errno) << "\n" << std::endl;
            return false;
        }

        return true;
    }

    bool send(const std::string &message) override
    {
        ssize_t sent_bytes = ::send(socket_fd, message.c_str(), message.size(), 0);
        if (sent_bytes == -1)
        {
            std::cerr << "Error sending data: " << strerror(errno) << std::endl;
            return false;
        }
        else if (static_cast<size_t>(sent_bytes) != message.size())
        {
            std::cerr << "Incomplete data sent" << std::endl;
            return false;
        }

        return true;
    }

    std::string receive() override
    {
        char recv_message[MESSAGE_SIZE];
        memset(recv_message, 0, MESSAGE_SIZE);

        socklen_t addr_len = sizeof(addrinfo);

        int bytes_received =
            ::recv(socket_fd, recv_message, MESSAGE_SIZE, 0);

        if (bytes_received < 0)
        {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }

        return std::string(recv_message);
    }

    bool changeOptions() override
    {
        // Implementación del método changeOptions
        return true;
    }
  private:
    int port_;
    bool isBlocking_;
    std::string address_;
    struct sockaddr_in6 addr;
    int socket_fd;
    struct addrinfo *addrinfo = nullptr;
};

Connection *createConnection(const std::string &address, const std::string &port, bool isBlocking, int protocolMacro)
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
        return new TCPv4Connection(address, port, isBlocking);
    case Protocol::TCPv6:
        return new TCPv6Connection(address, port, isBlocking);
    case Protocol::UDPv4:
        return new UDPv4Connection(address, port, isBlocking);
    case Protocol::UDPv6:
        return new UDPv6Connection(address, port, isBlocking);
    default:
        throw std::invalid_argument("Unsupported protocol");
    }
}

int main()
{
    Connection *con1 = createConnection("::1", "7658", true, UDP);
    Connection *con2 = createConnection("::1", "7658", true, UDP);


    // Create two threads for communication
    std::thread thread1([&]() {
        con1->bind();
        std::string message = con1->receive();
        std::cout << "Received message: " << message << std::endl;
        // Process the received message from con1
    });

    std::thread thread2([&]() {
        con2->connect();
        con2->send("Hola gil");
        // Process the received message from con2
    });

    // Wait for the threads to finish
    thread1.join();
    thread2.join();

    delete con1;
    delete con2;

    return 0;
}
