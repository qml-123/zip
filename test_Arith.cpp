//
// Created by ByteDance on 2022/4/20.
//

#include <iostream>
#include "src/com.h"
#include "src/Arith.h"
int main(int argc, char** argv) {
    qml::ARITH_Encode* en = new qml::ARITH_Encode();
    qml::ARITH_Decode* de = new qml::ARITH_Decode();
    en->file_encode(argv[1], false, std::string(argv[1]));
    de->file_decode(argv[2], false, "de_" + std::string(argv[1]));
    return 0;
}
