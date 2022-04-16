//
// Created by ByteDance on 2022/4/12.
//

#ifndef ZIP_SOCKET_H
#define ZIP_SOCKET_H
#include "com.h"



namespace qml {
    typedef struct sockaddr* SP;
    typedef struct NetWork
    {
        int fd;			// socket描述符
        int type;		// 协议类型 SOCK_STREAM/SOCK_DGRAM
        socklen_t len; 	// 地址长度
        struct sockaddr_in addr; // 通信地址
        NetWork() {

        }
        NetWork(char* ip, int port) {
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip);
            len = sizeof(addr);
            type = SOCK_STREAM;
        }
    };

    class Socket{
    public:

        Socket(char* ip, uint16_t port);
        Socket() {
            nw = new NetWork;
        }
        int getFd() {
            return nw->fd;
        }

        void init(NetWork* n);

        void Bind();

        void Listen();

        int Connect(NetWork* n);

        int Connect(Socket* n);

        // 发送数据
        int Send(void* buf, uint32_t len);

        // 接收数据
        int Recv(void* buf, uint32_t len);

        ~Socket();

        NetWork* nw;
    };

}

#endif //ZIP_SOCKET_H
