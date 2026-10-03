#include "network/server.h"
#include "message/textmessage.h"
Server::Server(int port) : __port(port) {}
Server::Server() : Server(5555) {}
void Server::addClient(int fd){
    std::lock_guard<std::mutex> guard(this->__mutex);
    this->__clients.insert(fd);
}
void Server::removeClient(int fd){
    std::lock_guard<std::mutex> guard(this->__mutex);
    if(this->__clients.count(fd)){
        this->closeSocket(fd);
        this->__clients.erase(fd);
    }
}
Server::~Server(){
    this->closeSocket(this->__serverSocket);
}
#ifdef __WIN32
#elif defined(__linux__)
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <thread>
#include <cstring>
void Server::start(){
    this->__serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(this->__serverSocket == -1){
        perror("socket");
        return;
    }
    printf("Socket created: %d\n", this->__serverSocket);
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->__port);
    if(bind(this->__serverSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address))
        == -1){
            perror("bind");
            close(this->__serverSocket);
            return;
    }
    printf("bind successful\n");
    if(listen(this->__serverSocket, 5) == -1){
        perror("listen");
        close(this->__serverSocket);
        return;
    }
    printf("listen successful\n");
    std::thread accepter(&Server::handleAccept, this);
    accepter.join();
}
void Server::handleAccept(){
    while(true){
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(
            this->__serverSocket,
            reinterpret_cast<sockaddr*>(&clientAddress),
            &clientAddressLength
        );
        if(clientSocket == -1){
            perror("accept");
        }else{
            printf("Accept successful: %d\n", clientSocket);
            this->addClient(clientSocket);
            std::thread recver(&Server::handleRecv, this, clientSocket);
            recver.detach();
        }
    }
}
void Server::closeSocket(int fd){
    close(fd);
}
void Server::handleRecv(int fd){
    printf("handleRecv %d\n", fd);
    std::string buffer;
    char temp[200];
    const std::string stopFlag = "[ASHSTOPFLAG]";
    while(true){
        int n = recv(fd, temp, sizeof(temp), 0);
        if(n <= 0){
            break;
        }
        buffer.append(temp, n);
        auto pos = buffer.find(stopFlag);
        if(pos != std::string::npos){
            std::string data = buffer.substr(0, pos);
            broadcast(TextMessage::parseFrom(data));
            std::cout << data << std::endl;
            buffer.clear();
        }
    }
    this->removeClient(fd);
}
void Server::broadcast(const Message& msg){
    std::lock_guard<std::mutex> guard(this->__mutex);
    std::string data = msg.encode();
    for(const auto& client : this->__clients){
        send(client, data.data(), data.size(), 0);
    }
}
#else
#error "Unsupported platform"
#endif