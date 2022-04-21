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
        else if (x == "-ar")
            return AR_EN;
        else if (x == "-rl")
            return RL_EN;
        else if (x == "-hf")
            return HF_EN;
        else
            return NO_EN;
    }

    DecodeType check_de(std::string x) {
        if (x == "-lz")
            return LZ_DE;
        else if (x == "-al")
            return AL_DE;
        else if (x == "-ar")
            return AR_DE;
        else if (x == "-rl")
            return RL_DE;
        else if (x == "-hf")
            return HF_DE;
        else
            return NO_DE;
    }

    Encode* EncodeFactory::CreateEncode(EncodeType et) {
        switch (et) {
            case LZ_EN:
                return new File2mark();
            case AL_EN:
                return new algorithmEncode();
            case AR_EN:
                return new ARITH_Encode();
            case RL_EN:
                return new RLE_Encode();
            case HF_EN:
                return new Hahuman();
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
            case AR_DE:
                return new ARITH_Decode();
            case RL_DE:
                return new RLE_Decode();
            case HF_DE:
                return new Hahuman();
            default:
                return NULL;
        }
    }

}