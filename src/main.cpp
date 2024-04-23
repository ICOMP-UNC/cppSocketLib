#include <iostream>
#include <memory>
#include <string>
#define TCP 1
#define UDP 2

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
    Connection(const std::string& address, int port, bool isBlocking)
        : address_(address), port_(port), isBlocking_(isBlocking)
    {
    }

    virtual ~Connection()
    {
    }

    virtual bool bind() = 0;
    virtual bool connect() = 0;
    virtual bool send(const std::string& message) = 0;
    virtual std::string receive() = 0;
    virtual bool changeOptions() = 0;

  protected:
    std::string address_;
    int port_;
    bool isBlocking_;
};

class TCPv4Connection : public Connection
{
  public:
    TCPv4Connection(const std::string& address, int port, bool isBlocking) : Connection(address, port, isBlocking)
    {
    }
    bool bind() override
    {
        // Implementación del método bind
        return true;
    }

    bool connect() override
    {
        // Implementación del método connect
        return true;
    }

    bool send(const std::string& message) override
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
    TCPv6Connection(const std::string& address, int port, bool isBlocking) : Connection(address, port, isBlocking)
    {
    }
    bool bind() override
    {
        // Implementación del método bind
        return true;
    }

    bool connect() override
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

    bool connect() override
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

    bool connect() override
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
