//
// Created by ByteDance on 2022/4/14.
//

#ifndef ZIP_ALGORITHM_H
#define ZIP_ALGORITHM_H
#include "com.h"
#include "Base_Text_Process.h"

namespace qml {
    long long string2ll(std::string a);

    class algorithmEncode : public Encode{
        bool file_encode(std::string, bool o2file, std::string);
    };

    class algorithmDecode : public Decode {
        bool file_decode(std::string, bool o2file, std::string);
    };
}


#endif //ZIP_ALGORITHM_H
