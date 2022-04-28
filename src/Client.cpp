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


    void Client::Send_txt(Encode* en) {
        int num_read;
        std::queue<std::string>tmp_file_name;
        while (!en->empty()) {
            std::string x = en->back();
            tmp_file_name.push(x);
            FILE* fp = fopen(x.c_str(), "r");
            memset(res, 0, sizeof res);
            strcpy(res, x.c_str());
            num_read = strlen(res);
            res[num_read++] = '\r';
            res[num_read++] = '\n';
            socket_file->Send(res, sizeof res);
            bzero(res, sizeof res);
            while ((num_read = fread(res, sizeof(char), sizeof(res) - 2, fp)) > 0) {
                res[num_read++] = '\r';
                res[num_read++] = '\n';
                socket_file->Send(res, sizeof res);
//                std::cout << res << std::endl;
                bzero(res, sizeof res);
            }
            res[0] = '\r';
            res[1] = '\n';
            socket_file->Send(res, sizeof res);
            bzero(res, sizeof res);
            fclose(fp);
        }
        res[0] = '\r';
        res[1] = '\n';
        socket_file->Send(res, sizeof res);
        bzero(res, sizeof res);
        while (!tmp_file_name.empty()) {
            std::string tmp = tmp_file_name.front();
            tmp_file_name.pop();
            tmp = "rm " + tmp;
            system(tmp.c_str());
        }
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
            std::string newFileName = "en_" + com.back();

			clock_t start,end; // typedef long clock_t
			start = clock();

			en->file_encode(com.back(), true, newFileName);
			end = clock();
			double duration =(double)(end-start)/CLOCKS_PER_SEC;
			std::cout << "压缩耗时: " << duration << "ms" << std::endl;
			Send_txt(en);
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
	void Client::printHelp() {
		if (com[0] == "send") {
			std::cout << "send -[] filename" << std::endl;
			std::cout << "usage:" << std::endl;
			std::cout << "	-al  算数编码" << std::endl;
			std::cout << "	-ar  自适应算数编码" << std::endl;
			std::cout << "	-lz  lz算法" << std::endl;
			std::cout << "	-rl  rle编码" << std::endl;
			std::cout << "	-hf  哈夫曼编码" << std::endl;
		}
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
				} else if (com[1] == "?") {
					printHelp();
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



