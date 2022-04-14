//
// Created by ByteDance on 2022/4/12.
//
#include "com.h"
#include "Factory.h"
namespace qml {
    EncodeType check_en(std::string x) {
        if (x == "-lz")
            return LZ_EN;
        else if (x == "-al")
            return AL_EN;
        else
            return NO_EN;
    }

    DecodeType check_de(std::string x) {
        if (x == "-lz")
            return LZ_DE;
        else if (x == "-al")
            return AL_DE;
        else
            return NO_DE;
    }

    Encode* EncodeFactory::CreateEncode(EncodeType et) {
        switch (et) {
            case LZ_EN:
                return new File2mark();
            case AL_EN:
                return new algorithmEncode();
            default:
                return NULL;
        }
    }

    Decode* DecodeFactory::CreateDecode(DecodeType dt) {
        switch (dt) {
            case LZ_DE:
                return new File2char();
            case AL_DE:
                return new algorithmDecode();
            default:
                return NULL;
        }
    }

}