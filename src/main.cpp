#include <iostream>
#include "clipboard/clipboard.h"
#include "network/server.h"
#include "network/client.h"
#include "message/textmessage.h"
#include <cstring>
#include <thread>
int main(int argc, char* argv[]){

    if(argc < 2){
        return 1;
    }
    
    if(std::strcmp(argv[1], "server") == 0){
        Server server;
        server.start();
    }else if(std::strcmp(argv[1], "client") == 0){
        Client client;
        client.connectTo("127.0.0.1");
        std::thread th([&](){
        while(true){
            std::string s;
            std::cin >> s;
            client.sendMessage(TextMessage(s));
            }
        });
        th.join();
    }else{
        return 1;
    }

    return 0;
}