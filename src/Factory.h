//
// Created by ByteDance on 2022/4/12.
//

#ifndef ZIP_FACTORY_H
#define ZIP_FACTORY_H
#include "com.h"
#include "Base_Text_Process.h"
#include "LZ77.h"
#include "Algorithm.h"
#include "Arith.h"

namespace qml {
    enum EncodeType {
        NO_EN = 0,
        LZ_EN = 1,
        AL_EN = 2,
        AR_EN = 3,
    };
    enum DecodeType {
        NO_DE = 0,
        LZ_DE = 1,
        AL_DE = 2,
        AR_DE = 3,
    };
    EncodeType check_en(std::string x);
    DecodeType check_de(std::string x);

    class EncodeFactory {
    public:
        Encode* CreateEncode(EncodeType et);
    };
    class DecodeFactory {
    public:
        Decode* CreateDecode(DecodeType dt);
    };
}


#endif //ZIP_FACTORY_H
