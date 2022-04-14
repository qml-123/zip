#include <iostream>
#include "src/com.h"
#include "src/Client.h"
int main(int argc, char** argv) {
    if (argc != 2 || check(argv[1])) {
        ASSERT(false, "help:: ./client port");
    }
    PORT = atoi(argv[1]);
    qml::Client* c = new qml::Client();
    c->Start();
    return 0;
}
