//
// Created by ByteDance on 2022/4/12.
//

#ifndef ZIP_BASE_TEXT_PROCESS_H
#define ZIP_BASE_TEXT_PROCESS_H
#include "com.h"
namespace qml {
    class Encode {
    protected:
        std::vector<std::string> file_names;
    public:
        virtual bool file_encode(std::string, bool o2file, std::string) = 0;

        std::string back() {
            if (file_names.empty()) {
                return "";
            }
            std::string ret = file_names.back();
            file_names.pop_back();
            return ret;
        }

        bool empty() {
            return file_names.empty();
        }
    };

    class Decode {
    public:
        virtual bool file_decode(std::string, bool o2file, std::string) = 0;
    };
}

#endif //ZIP_BASE_TEXT_PROCESS_H
