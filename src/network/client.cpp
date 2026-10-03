#include "network/client.h"

Client::Client(int port) : __port(port) {}
Client::Client() : Client(5555) {}
#ifdef __WIN32
#elif defined(__linux__)
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <thread>
void Client::connectTo(const char* host){
    this->__clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(this->__clientSocket == -1){
        perror("socket");
        return;
    }
    printf("client socket created: %d\n", this->__clientSocket);

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->__port);
    if(inet_pton(AF_INET, host, &serverAddress.sin_addr) <= 0){
        perror("inet_pton");
        close(this->__clientSocket);
        return;
    }
    if(connect(this->__clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1){
        perror("connect");
        close(this->__clientSocket);
        return;
    }
    std::thread recver(&Client::handleRecv, this);
    recver.detach();
    printf("connect to server\n");
}
bool Client::sendAll(const void* data, std::size_t size){
    bool flag = true;
    const char* buffer = static_cast<const char*>(data);
    int len = strlen(buffer);
    int sent = 0;
    while(sent < len){
        int n = send(this->__clientSocket, buffer + sent, len - sent, 0);
        if(n == -1){
            perror("send");
            flag = false;
            break;
        }
        sent += n;
    }
    printf("sent %d bytes\n", sent);
    return flag;
}
bool Client::sendMessage(const Message& message){
    auto encodingStr = message.encode();
    printf("Msg is %s \n", encodingStr.c_str());
    return sendAll(encodingStr.data(), encodingStr.size());
}
void Client::handleRecv(){
    std::string buffer;
    char temp[200];
    const std::string stopFlag = "[ASHSTOPFLAG]";
    while(true){
        int n = recv(this->__clientSocket, temp, sizeof(temp), 0);
        if(n <= 0){
            break;
        }
        buffer.append(temp, n);
        auto pos = buffer.find(stopFlag);
        if(pos != std::string::npos){
            std::string data = buffer.substr(0, pos);
            std::cout << data << std::endl;
            buffer.clear();
        }
    }
}
Client::~Client(){
    close(this->__clientSocket);
}
#else
#error "Unsupported platform"
#endif