//
// Created by ByteDance on 2022/4/12.
//

#ifndef ZIP_BASE_TEXT_PROCESS_H
#define ZIP_BASE_TEXT_PROCESS_H
#include "com.h"
namespace qml {
    class Encode {
    public:
        virtual bool file_encode(std::string, bool o2file, std::string) = 0;
    };

    class Decode {
    public:
        virtual bool file_decode(std::string, bool o2file, std::string) = 0;
    };
}

#endif //ZIP_BASE_TEXT_PROCESS_H
