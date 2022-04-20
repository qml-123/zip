//
// Created by ByteDance on 2022/4/21.
//

#ifndef ZIP_RLE_H
#define ZIP_RLE_H
#include "com.h"
#include "Base_Text_Process.h"

namespace qml {
    class RLE_CODER
    {
//目前实现的是固定码组[字符，连续出现的次数],压缩效率在文件重复字符少的情况下特别低
//更合理的方法是使用不定长编码，当次数为1时省略次数，需要在编解码付出额外的复杂度
    private:
        unsigned char * data_in;                                                                       // 数据输入指针
        unsigned char * data_out;                                                                      // 数据输出指针
        unsigned int in_counter;                                                                       // 输入数据计数器
        unsigned int data_in_len;                                                                      // 输入数据长度
        unsigned int out_counter;                                                                      // 输出数据计数器
        unsigned int data_out_len;                                                                     // 输出数据长度
        //编码初始化
        void start_encode(unsigned char * dest, unsigned int dest_len, unsigned char * src, unsigned int src_len);
        //解码初始化
        void start_decode(unsigned char * dest, unsigned int dest_len, unsigned char * src, unsigned int src_len);
        //输入输出长度复位
        void reset_IO();
        //编码一组符号
        void encode_symbol(unsigned char symbol, unsigned char num);
        //编码一组符号
        void decode_symbol(unsigned char symbol);
    public:
        //编码
        void rle_compress(unsigned char * dest, unsigned int * dest_len, unsigned char * src, unsigned int src_len);
        //解码
        void rle_uncompress(unsigned char * dest, unsigned int * dest_len, unsigned char * src, unsigned int src_len);
    };
    class RLE_Encode
    {
    private:
        RLE_CODER coder;
    public:
        bool file_encode(std::string, bool o2file, std::string);
    };
    class RLE_Decode
    {
    private:
        RLE_CODER coder;
    public:
        bool file_decode(std::string, bool o2file, std::string);
    };
}

#endif //ZIP_RLE_H
