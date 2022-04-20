//
// Created by ByteDance on 2022/4/20.
//

#ifndef ZIP_ARITH_H
#define ZIP_ARITH_H
#include "com.h"
#include "Base_Text_Process.h"

namespace qml {
    class ARITH_CODER
    {
    private:
        unsigned char * data_in;                                                                       // 数据输入指针
        unsigned char * data_out;                                                                      // 数据输出指针
        unsigned char index_to_char[NO_OF_SYMBOLS];                                                    // 索引到字符映射
        unsigned int char_to_index[NO_OF_CHARS];                                                       // 字符到索引映射
        unsigned int freq[NO_OF_SYMBOLS + 1];                                                          // 频率表
        unsigned int cum_freq[NO_OF_SYMBOLS + 1];                                                      // 累计频率表
        unsigned int bits_to_follow;                                                                   // 待输出的数据位数
        unsigned int buffer;                                                                           // 缓存区
        unsigned int bits_to_go;                                                                       // 缓存区待操作位数
        unsigned int low;                                                                              // 下边界
        unsigned int high;                                                                             // 上边界
        unsigned int magic;                                                                            // 解码时当前的值
        unsigned int in_counter;                                                                       // 输入数据计数器
        unsigned int data_in_len;                                                                      // 输入数据的长度
        unsigned int out_counter;                                                                      // 输出数据计数器
        unsigned int data_out_len;                                                                     // 输出数据长度

        //初始化自适应频率
        void init_adapt_model();
        //模型的更新(更新频率表)
        void update_model( unsigned int symbol);
        //输入输出长度复位
        void reset_IO();
        //编码初始化
        void start_encode(unsigned char * dest, unsigned int dest_len,unsigned char * src, unsigned int src_len);
        //将一个符号压入输出缓冲区
        void output_bit( unsigned int bit);
        //编码压入输出缓冲区
        void output_bits_follow(unsigned int bit);
        //编码一个符号
        void encode_symbol(unsigned int symbol);
        //结束编码
        unsigned int end_encode();
        //解码初始化
        void start_decode(unsigned char * dest, unsigned int dest_len,unsigned char * src, unsigned int src_len);
        //将一个编码转为符号输入
        unsigned int input_bit();
        //解压一个符号
        unsigned int decode_symbol();
        //结束解码
        unsigned int end_decode();
    public:
        //编码
        void arith_compress(unsigned char * dest, unsigned int * dest_len,unsigned char * src, unsigned int src_len);
        //解码
        void arith_uncompress(unsigned char * dest, unsigned int * dest_len, unsigned char * src, unsigned int src_len);
    };

    class ARITH_Encode : public Encode
    {
    private:
        ARITH_CODER coder;
    public:
        bool file_encode(std::string,bool o2file,std::string);
    };
    class ARITH_Decode : public Decode
    {
    private:
        ARITH_CODER coder;
    public:
        bool file_decode(std::string, bool o2file, std::string);
    };

}


#endif //ZIP_ARITH_H
