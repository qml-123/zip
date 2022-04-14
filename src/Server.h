//
// Created by ByteDance on 2022/4/12.
//

#ifndef ZIP_SERVER_H
#define ZIP_SERVER_H

#include "com.h"
#include "Socket.h"
#include "Factory.h"
#include "Base_Text_Process.h"

namespace qml {
    class Server {
    public:
        Server();

        ~Server();

        void start();

        void accept_network(Socket *s, Socket *c);

        void handle();

        void response();

        void spilt();

        void SendFile();

        void RecvFile();

        void Send_txt();

        void Cat();

        void Ls();

        void init_file_socket();

        int ReadFile(); //0 success
    private:
        Socket *socket_server;
        Socket *socket_client;
        Socket *socket_file_server;
        Socket *socket_file_client;
        char cmd[128];
        char res[128];
        char filename[128];
        std::string txt;
        std::vector<std::string> com;
        EncodeFactory* encodeFactory;
        DecodeFactory* decodeFactory;
    };
}
#endif //ZIP_SERVER_H

