//
// Created by ByteDance on 2022/4/12.
//
#include "com.h"
#include "Server.h"
#include "Socket.h"

namespace qml{
    Server::Server() {
        try {
            socket_server = new Socket("127.0.0.1", PORT);
        } catch (std::bad_alloc &e) {
            std::cerr << e.what() << std::endl;
        }


        socket_server->Bind();
        socket_server->Listen();
        encodeFactory = new EncodeFactory();
        decodeFactory = new DecodeFactory();
    }

    void Server::start() {
        int pid;
        while (true) {
            try {
                socket_client = new Socket();
            } catch (std::bad_alloc &e) {
                std::cerr << e.what() << std::endl;
            }
            accept_network(socket_server, socket_client);
            std::cout << "new client..." << std::endl;
            if ((pid = fork()) < 0) {
                ASSERT(false, "fork() error");
            } else if (pid == 0) {
                delete socket_server;
                handle();
                delete socket_client;
                exit(0);
            }
            delete socket_client;
        }
    }

    void Server::RecvFile() {
        bzero(res, sizeof res);
        strcpy(filename, com.back().c_str());
        int num_read, len;
        std::string old_file = "en_";
        old_file += filename;
        while (1) {
            bzero(res, sizeof res);
            num_read = socket_file_client->Recv(res, sizeof res);
            if (res[0] == '\r' && res[1] == '\n')
                break;
            num_read -= 2;
            while (num_read > -1) {
                if (res[num_read] == '\r' && res[num_read + 1] == '\n')
                    break;
                num_read--;
            }
            res[num_read] = 0;
            FILE *fd = fopen(res, "w+");
            if (!fd) {
                std::cout << "error" << std::endl;
                return;
            }
            bzero(res, sizeof res);
            while ((num_read = socket_file_client->Recv(res, sizeof res)) > 0) {
                len = num_read - 2;
                if (res[0] == '\r' && res[1] == '\n')
                    break;
                while (len > -1) {
                    if (res[len] == '\r' && res[len + 1] == '\n')
                        break;
                    len--;
                }
                fwrite(res, sizeof(char), len, fd);
                bzero(res, sizeof res);
            }
            fclose(fd);
        }
        DecodeType type = check_de(com[1]);
        Decode* de = decodeFactory->CreateDecode(type);
        std::string new_file = "de_";
        new_file += filename;

		clock_t start,end; // typedef long clock_t
		start = clock();
		de->file_decode(old_file.c_str(), true, new_file);

		double duration =(double)(end-start)/CLOCKS_PER_SEC;
		std::cout << "解压耗时 " << duration << "ms" << std::endl;
	}

	int Server::ReadFile() {
		int fd = open(filename, O_RDONLY);
		if(fd == -1) {
			return 1;
		}
		memset(res, 0, sizeof res);
		int num_read;
		while ((num_read = read(fd, res, sizeof res)) > 0) {
			socket_file_client->Send(res, sizeof res);
			bzero(res, sizeof res);
		}
		close(fd);
		return 0;
	}

	void Server::Ls() {
		bzero(res, sizeof res);
		int rs = system("ls > 11_tmp_11.txt");
		if(rs < 0)
		{
			strcpy(res, "error");
			socket_file_client->Send(res, sizeof (res));
			rs = system("rm tmp.txt");
			ASSERT(false, "error");
		}

		strcpy(filename, "11_tmp_11.txt");

		if (ReadFile()) {
			bzero(res, sizeof res);
			strcpy(res, "error");
			socket_file_client->Send(res, sizeof (res));
			rs = system("rm 11_tmp_11.txt");
			return;
		}

		rs = system("rm 11_tmp_11.txt");
	}


	void Server::response() {
		spilt();
		size_t len = com.size();
		bzero(res, sizeof res);
		try {
			socket_file_client = new Socket();
		} catch (std::bad_alloc &e) {
			std::cerr << e.what() << std::endl;
		}
		accept_network(socket_file_server, socket_file_client);
		if ((int)len > 3) {
			strcpy(res, "error");
			socket_file_client->Send(res, sizeof(res));
		}
		else if (len == 3) { // send [-] [file]
			RecvFile();
			std::cout << "recv end!" << std::endl;
		}
		else if (len == 2) { //cat ...
			Cat();
			std::cout << "cat end" << std::endl;
		} else { //ls
			Ls();
			std::cout << "ls end" << std::endl;
		}
		delete socket_file_client;
	}

	void Server::Cat() {
		strcpy(filename, com.back().c_str());
		if (ReadFile()) {
			bzero(res, sizeof res);
			strcpy(res, "error\n");
			socket_file_client->Send(res, sizeof(res));
			return;
		}
		bzero(res, sizeof res);
		strcpy(res, "\n");
		socket_file_client->Send(res, sizeof res);
	}

	void Server::spilt() {
		com.clear();
		std::string x;
		int n = strlen(cmd);
		for (int i = 0; i < n; i++) {
			if (cmd[i] == ' ') {
				if (!x.empty()) {
					com.push_back(x);
					x.clear();
				}
			} else if (cmd[i] != '\n') {
				x.push_back(cmd[i]);
			}
		}
		if (!x.empty())
			com.push_back(x);
	}

	void Server::init_file_socket() {
		try {
			socket_file_server = new Socket("127.0.0.1", 0);
		} catch (std::bad_alloc &e) {
			std::cerr << e.what() << std::endl;
		}

		socket_file_server->Bind();
		socket_file_server->Listen();
		getsockname(socket_file_server->getFd(), (SP)&(socket_file_server->nw->addr), &socket_file_server->nw->len);
		std::sprintf(cmd, "%d",  ntohs(socket_file_server->nw->addr.sin_port));
		//        std::cout << cmd << std::endl;
		socket_client->Send(cmd, sizeof cmd);
	}

	void Server::handle() {
		char buffer[128];
		bzero(cmd, sizeof cmd);
		bzero(buffer, sizeof buffer);

		init_file_socket();

		bzero(cmd, sizeof cmd);
		int bytes_read;
		while ((bytes_read = socket_client->Recv(buffer, sizeof buffer)) > 0) {
			strncpy(cmd, buffer, sizeof cmd);
			response();
			bzero(buffer, sizeof buffer);
		}
		std::cout << "exit..." << std::endl;
	}

	void Server::accept_network(Socket* s, Socket* c) {
		NetWork* client = new NetWork();
		client->type = SOCK_STREAM;
		int fd = -1;
		while (fd == -1) {
			fd = accept(s->nw->fd, (SP)&client->addr, &client->len);
		}
		if(0 > fd)
		{
			ASSERT(false, "accept() error");
		}
		c->init(client);
		c->nw->fd = fd;
		delete client;
	}

	Server::~Server() {
		delete socket_server;
		delete socket_file_server;
		delete encodeFactory;
		delete encodeFactory;
	}
}
