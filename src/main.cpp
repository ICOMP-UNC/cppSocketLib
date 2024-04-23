#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#define TCP 1
#define UDP 2
#define TCP_BACKLOG 10

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
    Connection(const std::string& address, const std::string& port, bool isBlocking)
        : address_(address), port_(port), isBlocking_(isBlocking)
    {
    }

    virtual ~Connection()
    {
    }

    virtual bool bind() = 0;
    virtual int connect() = 0;
    virtual bool send(const std::string& message) = 0;
    virtual std::string receive() = 0;
    virtual bool changeOptions() = 0;

  protected:
    std::string address_;
    std::string port_;
    bool isBlocking_;
    int socket_fd_;
};

class TCPv4Connection : public Connection
{

  private:
    struct addrinfo* addrinfo = NULL;

  public:
    TCPv4Connection(const std::string& address, const std::string& port, bool isBlocking)
        : Connection(address, port, isBlocking)
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

        int res = getaddrinfo(address.c_str(), port.c_str(), &hints, &addrinfo);

        if (res != 0)
        {
            throw std::runtime_error(gai_strerror(res));
        }

        socket_fd_ = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);

        if (socket_fd_ < 0)
        {
            throw std::runtime_error("Error creating socket");
        }
    }

    ~TCPv4Connection()
    {
        close(socket_fd_);
        freeaddrinfo(addrinfo);
    }

    bool bind() override
    {
        if (::bind(socket_fd_, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0)
        {
            throw std::runtime_error("Error: cannot bind socket");
            return false;
        }

        return true;
    }

    int connect() override
    {
        int res = ::listen(socket_fd_, TCP_BACKLOG); // Escuchar conexiones entrantes
        if (res < 0)
        {
            throw std::runtime_error("Error: cannot listen on socket");
            return false;
        }

        int client_fd = ::accept(socket_fd_, NULL, NULL); // Aceptar una conexión entrante
        if (client_fd < 0)
        {
            throw std::runtime_error("Error: cannot accept connection");
            return true;
        }

        return client_fd;
    }

    bool send(const std::string& message) override
    {
        const char* messagePtr = message.c_str();
        int n = ::send(socket_fd_, messagePtr, message.size(), 0); // contesta al cliente mediante el mismo fd
        if (n < 0)
        {
            throw std::runtime_error("Error: message sending failure");
            return false;
        }
        return true;
    }

    bool sendto(const std::string& message, int fdDestiny)
    {
        const char* messagePtr = message.c_str();
        int n = ::send(fdDestiny, messagePtr, message.size(), 0); // contesta al cliente mediante el mismo fd
        if (n < 0)
        {
            throw std::runtime_error("Error: message sending failure");
        }
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
    TCPv6Connection(const std::string& address, int port, bool isBlocking) : Connection(address, port, isBlocking)
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

    bool send(const std::string& message) override
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
    UDPv4Connection(const std::string& address, int port, bool isBlocking) : Connection(address, port, isBlocking)
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

    bool send(const std::string& message) override
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
    UDPv6Connection(const std::string& address, int port, bool isBlocking) : Connection(address, port, isBlocking)
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

    bool send(const std::string& message) override
    {
        std::cout << "UDPv6 " << message << std::endl;
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

Connection* createConnection(const std::string& address, int port, bool isBlocking, int protocolMacro)
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
    Connection* con1 = createConnection("::1", 8080, false, TCP);
    Connection* con2 = createConnection("::1", 8080, false, UDP);
    Connection* con3 = createConnection("127.0.0.1", 8080, false, TCP);
    Connection* con4 = createConnection("127.0.0.1", 8080, false, UDP);

    con1->send("Hola");
    con2->send("Hola");
    con3->send("Hola");
    con4->send("Hola");

    delete con1;
    delete con2;
    delete con3;
    delete con4;

    return 0;
}
