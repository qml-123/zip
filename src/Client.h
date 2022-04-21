//
// Created by ByteDance on 2022/4/12.
//

#ifndef ZIP_CLIENT_H
#define ZIP_CLIENT_H

#include "com.h"
#include "Socket.h"
#include "Factory.h"
#include "Base_Text_Process.h"
namespace qml{
    class Client{
    public:
        ~Client();
        void Start();
        void handle();
        void split();
        int ReadFile();
        Client();
        void init_file();
        void SendFile();
        void Send_txt(Encode* en);
        void SendCom();

    private:
        Socket* socket_com;
        Socket* socket_file;
        char res[128];
        char cmd[128];
        char filename[128];
        std::string txt;
        std::vector<std::string> com;
        EncodeFactory* encodeFactory;
        DecodeFactory* decodeFactory;
    };
}

#endif //ZIP_CLIENT_H
