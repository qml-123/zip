//
// Created by ByteDance on 2022/4/20.
//

#include "Arith.h"
namespace qml {
//初始话模型
    void ARITH_CODER::init_adapt_model() {
        unsigned int i = 0;
        //初始化字母表
        for (i = 0; i < NO_OF_CHARS; ++i) {
            this->char_to_index[i] = i + 1;
            this->index_to_char[i + 1] = (unsigned char) i;
        }
        //初始化频率与 累计频率
        for (i = 0; i <= NO_OF_SYMBOLS; ++i) {
            this->freq[i] = 1;
            this->cum_freq[i] = NO_OF_SYMBOLS - i;
        }
        this->freq[0] = 0;
    }

// 模型的更新
    void ARITH_CODER::update_model(unsigned int symbol) {
        unsigned int i = 0, cum = 0;
        unsigned int ch_i = 0, ch_symbol = 0;

        //处理频率即将溢出的情况 将所有的都缩减为原来的一半
        if (this->cum_freq[0] >= MAX_FREQUENCY) {
            for (cum = 0, i = NO_OF_SYMBOLS; i <= NO_OF_SYMBOLS; --i) {
                this->freq[i] = (this->freq[i] + 1) >> 1;
                this->cum_freq[i] = cum;
                cum += this->freq[i];
            }
        }
        //这样处理会使后面编码时大量出现的字符更容易使用较短的编码
        for (i = symbol; this->freq[i] == this->freq[i - 1]; --i);
        if (i < symbol) {
            ch_i = this->index_to_char[i];
            ch_symbol = this->index_to_char[symbol];
            this->index_to_char[i] = (unsigned char) ch_symbol;
            this->index_to_char[symbol] = (unsigned char) ch_i;
            this->char_to_index[ch_i] = symbol;
            this->char_to_index[ch_symbol] = i;
        }
        ++this->freq[i];
        while (i) {
            ++this->cum_freq[--i];
        }
    }

//复位计数器
    void ARITH_CODER::reset_IO() {
        this->in_counter = 0;
        this->out_counter = 0;
    }

//输出一个压缩数据位
    void ARITH_CODER::output_bit(unsigned int bit) {
        this->buffer >>= 1;
        if (bit) {
            this->buffer |= 0x80;
        }
        if (--this->bits_to_go == 0) {
            this->data_out[this->out_counter++] = (unsigned char) this->buffer;
            this->bits_to_go = 8;
        }
        //printf("%d  ", bit);
    }

//输出
    void ARITH_CODER::output_bits_follow(unsigned int bit) {
        output_bit(bit);
        for (; this->bits_to_follow; --this->bits_to_follow) {
            output_bit(!bit);
        }
    }

//编码初始化
    void
    ARITH_CODER::start_encode(unsigned char *dest, unsigned int dest_len, unsigned char *src, unsigned int src_len) {
        this->data_in = src;
        this->data_in_len = src_len;
        this->data_out = dest;
        this->data_out_len = dest_len;
        this->bits_to_follow = 0;
        this->buffer = 0;
        this->bits_to_go = 8;
        this->in_counter = 0;
        this->out_counter = 0;
        this->low = 0;
        this->high = TOP_VALUE;
        this->magic = 0;
    }

//编码一个符号
    void ARITH_CODER::encode_symbol(unsigned int symbol) {
        unsigned int range = 0;
        //printf("%d %d %d\n", FIRST_QTR, HALF, THIRD_QTR);
        //printf("begin:%d %d\n", this->low, this->high);
        range = this->high - this->low + 1;
        //printf("%u cum_freq:%d freq:%d\n",symbol,this->cum_freq[symbol], this->freq[symbol]);
        this->high = this->low + (range * this->cum_freq[symbol - 1]) / this->cum_freq[0] - 1;
        this->low = this->low + (range * this->cum_freq[symbol]) / this->cum_freq[0];
        //printf("update:%d %d\n", this->low, this->high);
        while (1) {
            if (this->high < HALF) {
                output_bits_follow(0);
            } else if (this->low >= HALF) {
                output_bits_follow(1);
                this->low -= HALF;
                this->high -= HALF;
            } else if (this->low >= FIRST_QTR && this->high < THIRD_QTR) {
                ++this->bits_to_follow;
                this->low -= FIRST_QTR;
                this->high -= FIRST_QTR;
            } else {
                break;
            }
            this->low = this->low << 1;
            this->high = (this->high << 1) + 1;
            //printf("process:%d %d\n", this->low, this->high);
        }
    }

//结束编码
    unsigned int ARITH_CODER::end_encode() {
        unsigned int ret = 0;

        encode_symbol(EOF_SYMBOL);
        ++this->bits_to_follow;
        //printf("end low:%d high:%d\n", this->low, this->high);
        if (this->low < FIRST_QTR) {
            output_bits_follow(0);
        } else {
            output_bits_follow(1);
        }
        this->buffer >>= this->bits_to_go;
        this->data_out[this->out_counter++] = (unsigned char) this->buffer;
        ret = this->out_counter;
        return ret;
    }

//输入一个压缩数据位
    unsigned int ARITH_CODER::input_bit() {
        unsigned int bit = 0;

        if (this->bits_to_go == 0) {
            this->buffer = this->data_in[this->in_counter++];
            this->bits_to_go = 8;
        }
        bit = this->buffer & 1;
        this->buffer >>= 1;
        --this->bits_to_go;
        return bit;
    }

//解码初始化
    void
    ARITH_CODER::start_decode(unsigned char *dest, unsigned int dest_len, unsigned char *src, unsigned int src_len) {
        unsigned int i = 0;

        this->data_in = src;
        this->data_in_len = src_len;
        this->data_out = dest;
        this->data_out_len = dest_len;
        this->bits_to_follow = 0;
        this->buffer = 0;
        this->bits_to_go = 0;
        this->in_counter = 0;
        this->out_counter = 0;
        this->low = 0;
        this->high = TOP_VALUE;
        this->magic = 0;
        for (i = 1; i <= BITS_MAX; ++i) {
            this->magic = (this->magic << 1) + input_bit();
        }
    }

//解码一个符号
    unsigned int ARITH_CODER::decode_symbol() {
        unsigned int range = 0;
        unsigned int cum = 0, symbol = 0;

        range = this->high - this->low + 1;
        cum = ((this->magic - this->low + 1) * this->cum_freq[0] - 1) / range;
        for (symbol = 1; this->cum_freq[symbol] > cum; ++symbol);
        this->high = this->low + (range * this->cum_freq[symbol - 1]) / this->cum_freq[0] - 1;
        this->low = this->low + (range * this->cum_freq[symbol]) / this->cum_freq[0];
        while (1) {
            if (this->high < HALF) {
            } else if (this->low >= HALF) {
                this->magic -= HALF;
                this->low -= HALF;
                this->high -= HALF;
            } else if (this->low >= FIRST_QTR && this->high < THIRD_QTR) {
                this->magic -= FIRST_QTR;
                this->low -= FIRST_QTR;
                this->high -= FIRST_QTR;
            } else {
                break;
            }
            this->low = this->low << 1;
            this->high = (this->high << 1) + 1;
            this->magic = (this->magic << 1) + input_bit();
        }

        return symbol;
    }

//结束解码
    unsigned int ARITH_CODER::end_decode() {
        unsigned int ret = this->out_counter;
        return ret;
    }

//编码
    void
    ARITH_CODER::arith_compress(unsigned char *dest, unsigned int *dest_len, unsigned char *src, unsigned int src_len) {
        unsigned int i = 0, symbol = 0;
        unsigned char ch = 0;

        init_adapt_model();
        reset_IO();
        start_encode(dest, *dest_len, src, src_len);
        for (i = 0; i < src_len; ++i) {
            ch = this->data_in[i];
            //printf("compress:%d %c\n", ch,ch);
            symbol = this->char_to_index[ch];
            //printf("%c\n", ch);
            encode_symbol(symbol);
            update_model(symbol);
            //printf("code_to_follow:%d\n", this->bits_to_follow);
        }
        *dest_len = end_encode();
    }

//解码
    void ARITH_CODER::arith_uncompress(unsigned char *dest, unsigned int *dest_len, unsigned char *src,
                                       unsigned int src_len) {
        unsigned int ch = 0, symbol = 0;

        init_adapt_model();
        start_decode(dest, *dest_len, src, src_len);
        while (1) {
            symbol = decode_symbol();
            if (symbol == EOF_SYMBOL) break;
            ch = this->index_to_char[symbol];
            this->data_out[this->out_counter++] = (unsigned char) ch;
            update_model(symbol);
        }
        *dest_len = end_decode();
    }

//下面实现一个接口，
    int get_file_size_by_stat(const char *file) {
        int ret;
        struct stat file_info;
        ret = stat(file, &file_info);
        return (!ret) ? file_info.st_size : -1;
    }

    bool ARITH_Encode::file_encode(std::string src, bool o2file, std::string dst) {
        unsigned char *buffer_src = NULL, *buffer_dst = NULL;
        FILE *file_src = NULL, *file_dst = NULL;
        unsigned int len_src = 0, len_dst = 0;

        len_src = get_file_size_by_stat(src.data());
        buffer_src = (unsigned char *) malloc(len_src);
        file_src = fopen(src.data(), "rb");
        if (len_src != -1) fread(buffer_src, sizeof(char), len_src, file_src);
        else return 0;
        fclose(file_src);

        len_dst = len_src + 256;
        buffer_dst = (unsigned char *) malloc(len_dst);
        coder.arith_compress(buffer_dst, &len_dst, buffer_src, len_src);
        file_dst = fopen(dst.data(), "wb");
        fwrite(buffer_dst, sizeof(char), len_dst, file_dst);
        fclose(file_dst);

        free(buffer_src);
        free(buffer_dst);
        return 1;
    }

    bool ARITH_Decode::file_decode(std::string src, bool o2file, std::string dst) {
        unsigned char *buffer_src = NULL, *buffer_dst = NULL;
        FILE *file_src = NULL, *file_dst = NULL;
        unsigned int len_src = 0, len_dst = 0;

        len_src = get_file_size_by_stat(src.data());
        buffer_src = (unsigned char *) malloc(len_src);
        file_src = fopen(src.data(), "rb");
        if (len_src != -1) fread(buffer_src, sizeof(char), len_src, file_src);
        else return 0;
        fclose(file_src);

        len_dst = 4 * len_src + 256;
        buffer_dst = (unsigned char *) malloc(len_dst);
        coder.arith_uncompress(buffer_dst, &len_dst, buffer_src, len_src);
        file_dst = fopen(dst.data(), "wb");
        fwrite(buffer_dst, sizeof(char), len_dst, file_dst);
        fclose(file_dst);

        free(buffer_src);
        free(buffer_dst);
        return 1;
    }
}