#pragma once
#include <unordered_set>
#include <mutex>
#include "message/message.h"
class Server{
private:
    int __port;
    int __serverSocket;
    std::unordered_set<int> __clients;
    std::mutex __mutex;
    void closeSocket(int fd);
    void handleAccept();
    void handleRecv(int fd);
public:
    Server();
    ~Server();
    explicit Server(int port);
    void addClient(int fd);
    void removeClient(int fd);
    void broadcast(const Message& msg);
    void start();
};