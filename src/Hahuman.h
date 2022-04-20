//
// Created by ByteDance on 2022/4/16.
//

#ifndef ZIP_HAHUMAN_H
#define ZIP_HAHUMAN_H

#include "com.h"
#include "Base_Text_Process.h"

namespace qml {
    //哈夫曼树
    struct HTNode
    {
        int Ascii;//字符对应的ASCLL码
        int weight;//权值
        int LC, RC, P;//左，右，父结点下标
    };

    //存储叶子节点编码在哈夫曼树对应的位置及，编码
    struct HTStr
    {
        int pos;//叶子节点的字符在哈夫曼树中的位置
        int s;//ASCLL码值
        char* code;//字符对应的哈夫曼编码
    };

    struct Bite {
        unsigned int bit : 1;
    };

    class Hahuman : public Encode, Decode {
    public:
        typedef HTNode* LinkHTNode;
        typedef HTStr* LinkHTStr;

        bool file_encode(std::string, bool o2file, std::string);

        bool file_decode(std::string, bool o2file, std::string);

        //分类函数
        bool sort_str(const char* file1, LinkHTNode& HT);

        //初建堆
        /*
         * 哈夫曼树
         * 堆头
         * 队尾
         */
        void HeapAdjust(LinkHTNode& HT, int s, int m);

        void CreatHeap(LinkHTNode& HT);

        //堆排序
        void HeapSort(LinkHTNode& HT, LinkHTStr& Str);

        //哈夫曼编码
        void HTFmanCode(LinkHTNode& HT,LinkHTStr& Str);

        //存储
        void Save(const char* filechar, const char* filecode, LinkHTStr Str, LinkHTNode HT);

        //编码写为二进制文件
        void Transb();

        void transtxt();

        //保存哈夫曼树读取哈夫曼树
        void TransHT(const char* fileTrans, LinkHTNode HT);

        //通过01文件编码翻译文章
        void Translate(const char* filecode, const char* fileTrans, LinkHTNode HT);

        //比较原文和解码后的文章
        void compare(const char* filecode, const char* fileTrans);

        ~Hahuman();
    private:
        std::string baseCode = "Code.bat";
        std::string codef;
        std::string bitef;
        std::string sortf;
        std::string sourcef;   //源文件名
        std::string codef2;    //加码后的文件名
        long ziptotal;
        long total;
        double d;
    };
}


#endif //ZIP_HAHUMAN_H
