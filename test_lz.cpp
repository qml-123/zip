//
// Created by ByteDance on 2022/4/20.
//

#include "src/com.h"
#include "src/LZ77.h"
int main(int argc, char** argv) {
    qml::Encode * en = new qml::File2mark();
    qml::Decode* de = new qml::File2char();
    en->file_encode(std::string(argv[1]), false, std::string(argv[2]));
    de->file_decode(std::string(argv[2]), false, "de_" + std::string(argv[1]));
    return 0;
}
