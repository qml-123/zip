//
// Created by ByteDance on 2022/4/12.
//

#include "Socket.h"

namespace qml{

    Socket::Socket(char* ip, uint16_t port) {

        try {
            nw = new NetWork();
        } catch (std::bad_alloc &e) {
            std::cerr << e.what() << std::endl;
        }

        // 创建socket对象
        nw->fd = socket(AF_INET, SOCK_STREAM, 0);
        if(0 > nw->fd)
        {
            delete nw;
            ASSERT(false, "socket() error");
        }

        nw->addr.sin_family = AF_INET;
        nw->addr.sin_port = htons(port);
        nw->addr.sin_addr.s_addr = inet_addr(ip);
        nw->len = sizeof(nw->addr);
        nw->type = SOCK_STREAM;
    }


    void Socket::init(NetWork *n) {
        nw->addr = n->addr;
        nw->len = n->len;
        nw->type = n->type;
    }

    // 发送数据
    int Socket::Send(void* buf, uint32_t len) {
        return write(nw->fd, buf, len);
    }

    // 接收数据
    int Socket::Recv(void* buf, uint32_t len) {
        return read(nw->fd, buf, len);
    }

    void Socket::Bind() {
        if (bind(nw->fd, (SP)&nw->addr, nw->len)) {
            ASSERT(false, "bind() error");
        }
    }

    void Socket::Listen() {
        if (listen(nw->fd, 50)) {
            ASSERT(false, "listen error");
        }
    }

    int Socket::Connect(NetWork *n) {
        return connect(nw->fd, (SP)&n->addr, n->len);
    }

    int Socket::Connect(Socket* n) {
        return Connect(n->nw);
    }

    Socket::~Socket() {
        if(close(nw->fd))
        {
            delete nw;
            ASSERT(false, "close() error");
        }
        delete nw;
    }


}

