//
// Created by ByteDance on 2022/4/14.
//

#include "Algorithm.h"

namespace qml {
    long long string2ll(std::string a) {
        long long ans = 0;
        long long wei = 1;
        for (int i = a.length() - 1; i >= 0; i--) {
            ans += (a[i] - '0')*wei;
            wei *= 10;
        }
        return ans;
    }

    bool algorithmEncode::file_encode(std::string ori_file, bool o2file, std::string en_file) {
        file_names.push_back(en_file);
        std::string original = "";//原文
        std::string code = "";//编码
        std::vector<int> fre;
        int total = 256;//总共可能出现的字符
        long long underflow = -1;
        long long undernum = 0;//下溢位数
        //初始化Low High
        long long l = 0;
        long long h = 9999999999;
        //读进原文
        std::string file = ori_file;
        std::ifstream in(file);
        std::string tmp;
        while (getline(in, tmp)) {
            if (original == "") original = tmp;
            else {
                original += '\n' + tmp;
            }
        }
        original += "#";
        //初始化概率表,每一项均为1，读取到某个字符后，将该字符的次数+1
        for (int i = 0; i <= 255; i++) {
            fre.push_back(1);
        }
        //开始处理原文
        for (int i = 0; i < original.length(); i++) {
            int pos = int(original[i]);
            long long range = (h - l) + 1;
            long long num = 0;
            for (int j = 0; j < pos; j++) {
                num += fre[j];
            }
            h = l + range * (num + fre[pos]) / (total + 1) - 1;
            l = l + range * num / (total + 1);
            total++;
            fre[pos]++;
            //接下来开始移位
            //开始移位
            while ((l / head) == (h / head)) { //当low和head的有效数字相同时
                long long d = l / head;
                code = code + char(l / head + '0');
                l = (l  % head) * 10; //移除d,将0添加到low的末尾
                h = (h   % head) * 10 + 9; //移除d，将9添加到high的末尾
                /*当再次出现高位收敛时，判断收敛的数是否跟underflow相同。
                如果相同，那说明High向Low收敛，那么需要输出undernum个9；
                如果不相同，那说明Low向High收敛，那么需要输出undernum个0
                */
                char add = '0';
                if (d == underflow) {// 当再次出现高位收敛时，判断收敛的数是否跟underflow相同。
                    add = '9';
                }
                while (undernum) {
                    undernum--;
                    code = code + add;
                }
                underflow = -1;
            }
            //处理下溢
            //如果Low和High的最高位不相同，但是在次高位上出现Low是9并且High是0时，那就有可能出现下溢。
            //当可能出现下溢时，删除次高位，再将9放入High的末尾，0放入Low的末尾。
            //并且记录Low的最高位的数字，以及移位的位数
            while (h - l < second) {
                if (underflow == -1)underflow = l / head; //underflow是low的最高位数字
                undernum++; //移位的位数
                int tmp = l / head;
                l = tmp * head + (l%second) * 10;
                tmp = h / head;
                h = tmp * head + (h%second) * 10 + 9;
            }
        }
        //全部遍历完成,将剩下的low,high相加/2
        long long m = (l + h) / 2;
        std::string magic = std::to_string(m);
        while (magic.length() < 10) {
            magic = magic + "0";
        }
        code = code + magic;
        std::ofstream out(en_file);
        out << code;
    }

    bool algorithmDecode::file_decode(std::string en_file, bool o2file, std::string ori_file) {
        std::string original = "";//原文
        std::string code = "";//编码
        std::vector<int> fre;
        int total = 256;//总共可能出现的字符
        long long underflow = -1;
        long long undernum = 0;
        //初始化Low High
        long long l = 0;
        long long h = 9999999999;
        //读进编码
        std::ifstream in(en_file);
        in >> code;
        //初始化概率表
        //解码时只要能保证与编码更新概率表的机制是相同的即可
        for (int i = 0; i <= 255; i++) {
            fre.push_back(1);
        }
        long long curcode = string2ll(code.substr(0, 10));
        int pos = 10;//当前运行到第几个code了
        int cur = 257;
        //开始解码
        while (cur != int('#')) {
            long long tmp = l;
            long long range = (h - l) + 1;
            long long num = 0;
            for (int j = 0; j < fre.size(); j++) {
                long long th = tmp + range * (num + fre[j]) / (total + 1) - 1;
                long long tl = tmp + range * (num) / (total + 1);
                num += fre[j];
                if (curcode >= tl && curcode < th) {
                    cur = j;
                    l = tl;
                    h = th;
                    total++;
                    fre[cur]++;
                    break;
                }
            }
            if (cur != int('#')) {
                original = original + char(cur);
            }
            //开始移位
            while ((l / head) == (h / head)) {
                code = code + char(l / head + '0');
                l = (l  % head) * 10;
                h = (h   % head) * 10 + 9;
                curcode = (curcode%head) * 10 + code[pos] - '0';
                pos++;
            }
            //处理下溢
            while (h - l < second) {
                long long tmp = curcode / head;
                curcode = tmp * head + (curcode%second) * 10 + code[pos] - '0';
                pos++;
                tmp = l / head;
                l = tmp * head + (l%second) * 10;
                tmp = h / head;
                h = tmp * head + (h%second) * 10 + 9;
            }

        }
        std::ofstream out(ori_file);
        out << original;
    }
}