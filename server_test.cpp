#include <iostream>
#include "src/Server.h"
#include "src/com.h"

int main(int argc, char** argv) {
    if (argc != 2 || check(argv[1])) {
        ASSERT(false, "./server port");
    }
    PORT = atoi(argv[1]);
    qml::Server* s = new qml::Server();
    s->start();
    return 0;
}
