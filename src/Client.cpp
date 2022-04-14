//
// Created by ByteDance on 2022/4/12.
//

//
// Created by ByteDance on 2022/4/12.
//
#include "Factory.h"
#include "Client.h"
#include "com.h"

namespace qml {
    Client::Client() {
        try {
            socket_com = new Socket("127.0.0.1", 0);
        } catch (std::bad_alloc &e) {
            std::cerr << e.what() << std::endl;
        }

        bzero(cmd, sizeof cmd);
        encodeFactory = new EncodeFactory();
        decodeFactory = new DecodeFactory();
    }

    void Client::Start() {
        NetWork* nw = new NetWork("127.0.0.1", PORT);

        if (socket_com->Connect(nw)) {
            ASSERT(false, "connect error");
        }

        delete nw;
        bzero(res, sizeof res);
        int num = socket_com->Recv(res, sizeof res);
        res[num] = 0;
        PORT2 = atoi(res);
//        std::cout << PORT2 << std::endl;
        handle();
    }

    void Client::split() {
        com.clear();
        std::string s;
        int len = strlen(cmd);
        for (int i = 0; i < len; i++) {
            if (cmd[i] == ' ') {
                if (!s.empty()) {
                    com.push_back(s);
                    s.clear();
                }
            } else if (cmd[i] != '\n') {
                s.push_back(cmd[i]);
            }
        }
        if (!s.empty())
            com.push_back(s);
    }


    void Client::Send_txt() {
        FILE* fp = fopen(filename, "r");
        memset(res, 0, sizeof res);
        int num_read;
        bzero(res, sizeof res);
        while ((num_read = fread(res, sizeof(char), sizeof res, fp)) > 0) {
            socket_file->Send(res, num_read);
            bzero(res, sizeof res);
        }
        fclose(fp);
    }


    void Client::SendFile() {
        EncodeType type = qml::check_en(com[1]);
        if (com[0] != "send" || type == NO_EN) {
            puts("error");
        } else {
            bzero(cmd, sizeof cmd);
            int m = 0;
            for (auto i : com) {
                for (auto j : i) {
                    cmd[m++] = j;
                }
                cmd[m++] = ' ';
            }
            cmd[m] = 0;
            socket_com->Send(cmd, sizeof cmd);
            init_file();
            Encode* en = encodeFactory->CreateEncode(type);
            strcpy(filename, "12123_tmp_12123");
            en->file_encode(com.back(), true, filename);
            Send_txt();
            system("rm 12123_tmp_12123");
            delete socket_file;
        }
    }
    void Client::init_file() {
        try {
            socket_file = new Socket("127.0.0.1", 0);
        } catch (std::bad_alloc &e) {
            std::cerr << e.what() << std::endl;
        }
        NetWork* sock = new NetWork("127.0.0.1", PORT2);
        socket_file->Connect(sock);
        delete sock;
    }
    void Client::SendCom() {
        int num_read;
        socket_com->Send(cmd, sizeof cmd);
        init_file();
        bzero(res, sizeof res);
        while ((num_read = socket_file->Recv(res, sizeof res)) > 0) {
            res[num_read] = 0;
            std::cout << res;
            bzero(res, sizeof res);
        }
        delete socket_file;
    }
    void Client::handle() {
        bzero(cmd ,sizeof cmd);
        std::cout << "->";
        while (fgets(cmd, sizeof cmd, stdin) != NULL) {
            split();
            if (com.empty()) {
                std::cout << "->";
                continue;
            } else if ((int)com.size() > 3) {
                puts("error");
            } else if ((int) com.size() == 3) {
                SendFile();
            } else {
                if (com[0] == "exit") {
                    break;
                } else if (com[0] == "cls") {
                    system("clear");
                } else {
                    SendCom();
                }
            }
            bzero(cmd, sizeof cmd);
            std::cout << "->";
        }
    }

    Client::~Client() {
        delete socket_file;
        delete socket_com;
        delete encodeFactory;
        delete decodeFactory;
    }

}



