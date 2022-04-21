//
// Created by ByteDance on 2022/4/21.
//

#include "Rle.h"
#include "com.h"
#define    BITS_MAX               8                                                                // 允许的最大位数
#define    TOP_VALUE              ((1<<BITS_MAX) - 1)
namespace qml {
    void RLE_CODER::start_encode(unsigned char * dest, unsigned int dest_len, unsigned char * src, unsigned int src_len)
    {
        this->data_in = src;
        this->data_in_len = src_len;
        this->data_out = dest;
        this->data_out_len = dest_len;
        reset_IO();
    }
    void RLE_CODER::start_decode(unsigned char * dest, unsigned int dest_len, unsigned char * src, unsigned int src_len)
    {
        this->data_in = src;
        this->data_in_len = src_len;
        this->data_out = dest;
        this->data_out_len = dest_len;
        reset_IO();
    }
    void RLE_CODER::reset_IO()
    {
        this->in_counter = 0;
        this->out_counter = 0;
    }
    void RLE_CODER::encode_symbol(unsigned char symbol, unsigned char num)
    {
        this->data_out[this->out_counter++] = symbol;
        this->data_out[this->out_counter++] = num;
    }

    void RLE_CODER::decode_symbol(unsigned char symbol)
    {
        this->data_out[this->out_counter++] = symbol;
    }

    void RLE_CODER::rle_compress(unsigned char * dest, unsigned int * dest_len,unsigned char * src, unsigned int src_len)
    {
        unsigned int i = 0;
        unsigned int cnt = 1;
        unsigned char symbol = 0;

        reset_IO();
        start_encode(dest, *dest_len, src, src_len);
        for (i = 0; i < this->data_in_len; ++i)
        {
            if (i == 0) symbol = this->data_in[i];
            else
            {
                if (symbol == this->data_in[i])
                {
                    ++cnt;
                }
                else
                {
                    while (cnt > TOP_VALUE)
                    {
                        encode_symbol(symbol, TOP_VALUE);
                        cnt -= TOP_VALUE;
                    }
                    encode_symbol(symbol, cnt);
                    symbol = this->data_in[i];
                    cnt = 1;
                }
            }
        }
        if (cnt > 0)
        {
            while (cnt > TOP_VALUE)
            {
                encode_symbol(symbol, TOP_VALUE);
                cnt -= TOP_VALUE;
            }
            encode_symbol(symbol, cnt);
        }
        *dest_len = this->out_counter;
    }

    void RLE_CODER::rle_uncompress(unsigned char * dest, unsigned int * dest_len,unsigned char * src, unsigned int src_len)
    {
        unsigned int i = 0;
        unsigned char symbol = 0, cnt = 0;

        reset_IO();
        start_decode(dest, *dest_len, src, src_len);

        for (i = 0; i < this->data_in_len; i+=2)
        {
            symbol = this->data_in[i]; cnt = this->data_in[i + 1];
            while (cnt--)
            {
                decode_symbol(symbol);
            }
        }
        *dest_len = this->out_counter;
    }
    bool RLE_Encode::file_encode(std::string src, bool o2file, std::string dst)
    {
        unsigned char * buffer_src = NULL, *buffer_dst = NULL;
        FILE * file_src = NULL, *file_dst = NULL;
        unsigned int len_src = 0, len_dst = 0;

        len_src = get_file_size_by_stat(src.c_str());
        buffer_src = (unsigned char*)malloc(len_src);
        file_src = fopen(src.c_str(), "rb");
        if (len_src != -1) fread(buffer_src, sizeof(char), len_src, file_src);
        else {
            fclose(file_src);
            return 0;
        }
        fclose(file_src);

        len_dst = 4*len_src + 256;
        buffer_dst = (unsigned char*)malloc(len_dst);
        coder.rle_compress(buffer_dst, &len_dst, buffer_src, len_src);
        file_dst = fopen(dst.c_str(), "wb");
        fwrite(buffer_dst, sizeof(char), len_dst, file_dst);
        fclose(file_dst);
        // printf("%d %d\n", len_src, len_dst);

        free(buffer_src);
        free(buffer_dst);
        return 1;
    }

    bool RLE_Decode::file_decode(std::string src, bool o2file, std::string dst)
    {
        unsigned char * buffer_src = NULL, *buffer_dst = NULL;
        FILE * file_src = NULL, *file_dst = NULL;
        unsigned int len_src = 0, len_dst = 0;

        len_src = get_file_size_by_stat(src.c_str());
        buffer_src = (unsigned char*)malloc(len_src);
        file_src = fopen(src.c_str(), "rb");
        if (len_src != -1) fread(buffer_src, sizeof(char), len_src, file_src);
        else return 0;
        fclose(file_src);

        len_dst = 4 * len_src + 256;
        buffer_dst = (unsigned char*)malloc(len_dst);
        coder.rle_uncompress(buffer_dst, &len_dst, buffer_src, len_src);
        file_dst = fopen(dst.c_str(), "wb");
        fwrite(buffer_dst, sizeof(char), len_dst, file_dst);
        fclose(file_dst);

        free(buffer_src);
        free(buffer_dst);
        return 1;
    }
}
