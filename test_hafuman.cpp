#include <iostream>
#include "src/com.h"
#include "src/Hahuman.h"
int main(int argc, char** argv) {
    qml::Hahuman* h = new qml::Hahuman();
    h->file_encode(argv[1], false, argv[2]);
    h->file_decode(argv[2], false, "de_" + std::string(argv[1]));
    return 0;
}
