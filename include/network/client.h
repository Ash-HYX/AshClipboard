#pragma once
#include <string>
#include "message/message.h"
class Client{
private:
    int __port;
    int __clientSocket;
    void handleRecv();
public:
    Client();
    ~Client();
    explicit Client(int port);
    void connectTo(const char* host);
    bool sendMessage(const Message& msg);
private:
    bool sendAll(const void* data, std::size_t size);
};