//
// Created by ByteDance on 2022/4/20.
//

#include "src/com.h"
#include "src/Rle.h"
int main(int argc, char** argv) {
    qml::RLE_Encode* en = new qml::RLE_Encode();
    qml::RLE_Decode* de = new qml::RLE_Decode();
    en->file_encode(std::string(argv[1]), false, std::string(argv[1]));
    de->file_decode(std::string(argv[2]), false, "de_" + std::string(argv[1]));
    return 0;
}
