#include "cppSocketLib.hpp"

class ThreadManager
{
  public:
    ThreadManager(std::jthread thread1, std::jthread thread2, std::jthread thread3, std::jthread thread4)
        : thread1_(std::move(thread1)), thread2_(std::move(thread2)), thread3_(std::move(thread3)),
          thread4_(std::move(thread4))
    {
    }

    ~ThreadManager()
    {
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

int main()
{
    std::shared_ptr<IConnection> con1 = createConnection("::1", "7658", true, UDP);
    std::shared_ptr<IConnection> con2 = createConnection("::1", "7658", true, UDP);
    std::shared_ptr<IConnection> con3 = createConnection("127.0.0.2", "7672", true, UDP);
    std::shared_ptr<IConnection> con4 = createConnection("127.0.0.2", "7672", true, UDP);

    ThreadManager threads(std::jthread([&con1]() {
                              try
                              {
                                  con1->bind();
                                  std::string message = con1->receive();
                                  std::cout << "Received message: " << message << std::endl;
                              }
                              catch (const std::exception& e)
                              {
                                  std::cerr << e.what() << std::endl;
                              }
                          }),
                          std::jthread([&con2]() {
                              try
                              {
                                  con2->connect();
                                  con2->send("Hola mundo! IPv6");
                              }
                              catch (const std::exception& e)
                              {
                                  std::cerr << e.what() << std::endl;
                              }
                          }),
                          std::jthread([&con4]() {
                              try
                              {
                                  con4->connect();
                                  con4->send("Hola mundo! IPv4");
                                  // Process the received message from con2
                              }
                              catch (const std::exception& e)
                              {
                                  std::cerr << e.what() << std::endl;
                              }
                          }),
                          std::jthread([&con3]() {
                              try
                              {
                                  con3->bind();
                                  std::string message = con3->receive();
                                  std::cout << "Received message: " << message << std::endl;
                                  // Process the received message from con1
                              }
                              catch (const std::exception& e)
                              {
                                  std::cerr << e.what() << std::endl;
                              }
                          }));

    return 0;
}
