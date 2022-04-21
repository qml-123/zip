//
// Created by ByteDance on 2022/4/16.
//

#include "Hahuman.h"

namespace qml {
    bool Hahuman::sort_str(const char *file1, LinkHTNode& HT) {
        char ch;//临时存储每次拿到的字符
        int s;//用于转换ASCLL码对应的十进制数值

        int* strw = new int[100];//临时存储英文文章统计后的每种字符的个数

        //初始化统计字符频数数组
        for (int i = 0; i < 100; i++)
        {
            strw[i] = 0;
        }

        // 分类文件字符
        std::ifstream ifile(file1, std::ios::in);
        if (!ifile)
        {
//            cout << "打开源文件失败" << endl;
            return false;
        }
        while (ifile.get(ch))
        {
            s = ch;
            if (s == 10) { strw[0]++; }//换行符
            if ((s - 31) > 0) { strw[s - 31]++; }//32-127号的字符

        }
        //统计出现的字符种类
        s = 0;
        for (int i = 0; i < 100; i++)
        {
            if (strw[i] != 0)s++;
        }

        //初始化哈夫曼树
        HT = new HTNode[2 * s];//0号位不存，需要多开一个单元内存
        for (int i = 0; i < 2 * s; i++)
        {
            HT[i].Ascii = 0; HT[i].weight = 0;
            HT[i].LC = HT[i].RC = HT[i].P = 0;
        }
        HT[0].weight = s;//叶子结点数

        //剔除未出现的字符，存储字符和权值
        int  k;
        if (strw[0] != 0)//换行符单独判断存储
        {
            HT[1].Ascii = 10;
            HT[1].weight = strw[0];
            HT[0].Ascii += HT[1].weight;
            k = 2;//有换行符就从2位置开始存储之后的字符
        }
        else { k = 1; }//否则，从1号位开始
        for (int i = 1; i < 100; i++)
        {
            if (strw[i] != 0)
            {
                HT[k].Ascii = i + 31;
                HT[k].weight = strw[i];
                HT[0].Ascii += HT[k].weight;
                k++;
            }
        }
        delete[]strw;//销毁临时空间
    }

    void Hahuman::HeapAdjust(LinkHTNode &HT, int s, int m) {
        HTNode rc = HT[s];//暂存堆顶结点
        for (int j = 2 * s; j <= m; j *= 2)
        {
            if (j < m && HT[j].weight > HT[j + 1].weight)++j;//左右孩子比较大小，取小的一个
            if (rc.weight <= HT[j].weight)break;
            HT[s] = HT[j]; s = j;//较小的孩子与父节点比较，小的最后作为的父节点
        }
        HT[s] = rc;
    }

    void Hahuman::CreatHeap(LinkHTNode &HT) {
        int n = HT[0].weight;//初始堆的个数，即叶子节点数
        for (int i = n / 2; i > 0; --i)
            HeapAdjust(HT, i, n);
    }

    void Hahuman::HeapSort(LinkHTNode &HT, LinkHTStr &Str) {
        CreatHeap(HT);
        Str = new HTStr[HT[0].weight];//动态建立Str
        int n = HT[0].weight;//叶子结点数
        int k = 0;
        int i = 2 * n - 1;//哈夫曼树表尾
        //创建哈夫曼树
        for (; i > 1; i--)
        {
            k++;
            HT[i] = HT[1];//选出的堆顶元素放到哈夫曼树尾
            if (k % 2) { HT[1] = HT[n]; n--; }//只选出一个最小数时，堆尾放上上对顶
            else //选出两个最小数之后，合成新的节点，合成新的结点放在堆顶
            {
                HT[1].weight = HT[i + 1].weight + HT[i].weight;
                HT[1].Ascii = 0; HT[1].LC = i + 1;
                HT[1].RC = i;
            }
            HeapAdjust(HT, 1, n);//堆排
        }

        //挑选叶子节点储存到另一个空间
        n--;
        for (; i <= k + 1; i++)
        {
            if (HT[i].LC) { HT[HT[i].LC].P = i; }
            if (HT[i].RC) { HT[HT[i].RC].P = i; }
            if (HT[i].Ascii) {
                Str[n].pos = i; Str[n].s = HT[i].Ascii; n++;
            }
        }
    }

    void Hahuman::HTFmanCode(LinkHTNode &HT, LinkHTStr &Str) {
        char* code = new char[HT[0].weight];//临时的存储每一个字符的编码
        int n = HT[0].weight - 1;
        int f, c;//f为父结点，c为子结点

        for (int i = 0; i < HT[0].weight; i++)
        {
            f = HT[Str[i].pos].P;//父节点
            c = Str[i].pos;//子节点

            while (f)
            {
                if (c == HT[f].LC) { code[n] = '0'; }
                else { code[n] = '1'; }
                n--;
                c = f;//往根的方向回溯，子结点更改为它的父结点
                f = HT[f].P;
            }
            //整理每次编好的字符编码，存储到Str[i].code中
            int length = HT[0].weight - n - 1;
            Str[i].code = new char[length + 1];
            Str[i].code[length] = '\0';
            for (int k = 0; k < length; k++)
            {
                Str[i].code[k] = code[n + 1];
                n++;
            }
        }
    }

    void Hahuman::Save(const char *filechar, const char *filecode, LinkHTStr Str, LinkHTNode HT) {
        std::ofstream ofilechar(filechar, std::ios::out);//储存字符，权值，编码的文件
        std::ofstream ofilecode(filecode, std::ios::binary);//存储整篇文章的编码的文件
        std::ofstream ofilebite(bitef, std::ios::binary);//存储每种字符的编码文件

        std::ifstream ifile(sourcef, std::ios::out);

        char ch;
        Bite B_Save;
        //储存字符，权值，编码到文件
        ofilechar << "字符" << std::setw(20) << "频数" << std::setw(20) << "编码" << std::endl;
        for (int i = 0; i < HT[0].weight; i++)
        {
            if (Str[i].s == 10) { ofilechar << "line break" << std::setw(20) << HT[Str[i].pos].weight << std::setw(22) << Str[i].code << std::endl; continue; }
            if (Str[i].s == 32) { ofilechar << "Space" << std::setw(17) << HT[Str[i].pos].weight << std::setw(22) << Str[i].code << std::endl; continue; }
            ofilechar << char(Str[i].s) << std::setw(21) << HT[Str[i].pos].weight << std::setw(22) << Str[i].code << std::endl;
            //下面的for用于存储编码的比特位（每种字符的）
            for (int k = 0; Str[i].code[k] != '\0'; k++)
            {
                if (Str[i].code[k] == '0')
                    B_Save.bit = 0;
                else B_Save.bit = 1;
                ofilebite << B_Save.bit;
            }
        }
        //存储整篇文章的编码
        //写入

        while (ifile.get(ch))
        {
            for (int i = 0; i < HT[0].weight; i++)
            {
                if (ch == char(Str[i].s))
                {
                    ofilecode << Str[i].code;
                }
            }
        }
        //关闭文件
        ofilechar.close();
        ofilecode.close();
        ofilebite.close();
        ifile.close();
    }

    void Hahuman::Transb() {
        FILE *fp1,*fp2;
        int i, n,sum;
        char c,ch[9];
        i = 0; n = 0;
        fp1 = fopen(codef.c_str(), "r");
        if (fp1 == NULL) {
            printf("打开赫夫曼码文件失败\n");
        }
        fp2 = fopen(codef2.c_str(), "wb");//打开，没有则创建，有则清空
        while (fscanf(fp1,"%c", &c,1) != EOF)                //读到文件结束
        {
            n++;
            ch[n] = c;
            if (n == 8)                              //每8位写入一次，不满8位的补0
            {
                sum = 0;
                for (i = 1; i <= n; i++)
                {
                    sum = sum * 2 + (ch[i] & 0x1);
                }
                fputc(sum, fp2);
                total++;
                n = n - 8;
            }
        }
        if(n)                  //补0
        {
            sum = 0;
            for (i = 1; i <= n; i++)
            {
                sum = sum * 2 + (ch[i] & 0x1);
            }
            sum = sum << (8 - n);
            fputc(sum, fp2);
            total++;
            n = n - 8;
        }
        fclose(fp1);
        fclose(fp2);
    }

    void Hahuman::transtxt() {
        FILE* fp1, *fp2;
        char temp[30];
        unsigned char ch;
        int i, j, n,k = 0, tmp, flag;
        fp1 = fopen(codef2.c_str(), "rb");
        if (fp1 == NULL) {
            printf("打开二进制赫夫曼码文件失败\n");
            exit(0);
        }
        fp2 = fopen(codef.c_str(), "w");//打开，没有则创建，有则清空

        while (fscanf(fp1, "%c", &ch, 1) != EOF)
        {
//		printf("%d\n", ch);
            for (i = 0; i < 8; i++)
            {
                temp[k++] = ((ch >> (7 - i)) & 0x1) + '0';
                fputc(temp[k - 1], fp2);
                //		printf("%c", temp[k-1]);
            }
            k = k - 8;
            //	printf("\n");
        }
        ch = 0;
        fclose(fp1);
        fclose(fp2);
    }

    void Hahuman::TransHT(const char* fileTrans, LinkHTNode HT) {
        int i = 1;
        int asc,n,lc,rc,fa;
        char str[202];
        FILE* fp=fopen(fileTrans, "r");
        if (fp == NULL)
        {
            std::cout << "打开赫夫曼树文件失败" << std::endl;
            exit(0);
        }
        fscanf(fp, "%ld", &transTol, 1);
        fgets(str, 200, fp);
        fgets(str, 200, fp);
//	cout << str;
        while (fscanf(fp, "%d", &asc, 1) != EOF)
        {
            fscanf(fp, "%d%d%d%d", &n, &lc, &rc, &fa, 4);
//		cout << asc << " "<<n << " "<<lc << " "<<rc << " "<< fa <<endl;
            HT[i].Ascii = asc;
            HT[i].LC = lc;
            HT[i].RC = rc;
            HT[i].P = fa;
            i++;
        }
        fclose(fp);
    }

    void Hahuman::Translate(const char *filecode, const char *fileTrans, LinkHTNode HT) {
        std::ifstream ifile(filecode, std::ios::in);
        std::ofstream ofile(fileTrans, std::ios::out);
        if (!ifile)
        {
            std::cout << "打开赫夫曼码文件失败" << std::endl;
            exit(0);
        }
        char ch;
        int f = 1;
        while (ifile.get(ch))
        {
            if (transTol == 0)
                break;
            if (ch == '0')f = HT[f].LC;
            else f = HT[f].RC;
            if (HT[f].LC == 0 && HT[f].RC == 0) {
                transTol--;
                ofile << char(HT[f].Ascii);
                f = 1;
                continue;
            }

        }
        ifile.close();
        ofile.close();
    }

    void Hahuman::compare(const char *filecode, const char *fileTrans) {
        FILE* fp1, * fp2;
        int i, n, m;          //n为原文长，m为相等的长度
        char c1, c2;
        i = 0; n = 0; m = 0;
        fp1 = fopen(filecode, "r");
        if (fp1 == NULL)
        {
            std::cout << "打开原文件失败" << std::endl;
            exit(0);
        }
        fp2 = fopen(fileTrans, "r");
        if (fp2 == NULL)
        {
            std::cout << "打开解码的文件失败" << std::endl;
            exit(0);
        }
        if (fp1 == NULL|| fp2 == NULL) {
            printf("open error\n");
        }
        while (fscanf(fp1, "%c", &c1, 1) != EOF)
        {
            n++;
            fscanf(fp2, "%c", &c2, 1);
            if (c1 == c2) m++;
        }
        d = m / n;
        std::cout << "文本正确率为：  " << d*100 << "%\n" << std::endl;
    }

    bool Hahuman::file_encode(std::string source, bool o2file, std::string code2) {
        HTNode* HT;
        HTStr* Str;

        sourcef = source;
        sort_str(sourcef.c_str(), HT);
        HeapSort(HT, Str);
        HTFmanCode(HT, Str);

        codef2 = code2;


        std::string file_name = sourcef.substr(0, sourcef.find(".") != sourcef.npos ? (int)sourcef.find(".") : (int)sourcef.size());


        //"保存分类好的字符及频数编码的文件名（连同后缀名）：";
        sortf = "en_" + file_name + "_freq";

        //"保存赫夫曼树的文件名（连同后缀名）：";
        bitef = "en_" + file_name + "_HTree";
        codef = "en_" + file_name + "_" + baseCode;

        file_names.push_back(code2);
        file_names.push_back(bitef);
        file_names.push_back(codef);

        Save(sortf.c_str(), codef.c_str(), Str, HT);

        Transb();

        ziptotal = HT[0].Ascii;

        d = (double(total) / ziptotal) * 100;

        std::ofstream of(bitef, std::ios::out);
        of << HT[0].Ascii << "-字符总个数" << std::endl;
        of << std::setw(7) << "ASCLL码" << std::setw(10) << "位置" << std::setw(10) << "左孩子" << std::setw(10) << "右孩子" << std::setw(10) << "父结点" << std::endl;
        //ASCLL为0表示非叶子结点
        for (int i = 1; i < 2 * HT[0].weight; i++)
        {
            of << std::setw(3) << HT[i].Ascii << std::setw(15) << i << std::setw(15) << HT[i].LC << std::setw(15) << HT[i].RC << std::setw(15) << HT[i].P << std::endl;
        }
        of.close();
		return true;
//        std::cout << "压缩率为：  " << d << "%\n" << std::endl;
    }


    bool Hahuman::file_decode(std::string en_code, bool o2file, std::string de_code) {
        HTNode* HT;
        HT = new HTNode[256];

        codef2 = en_code;
        sourcef = de_code;

        std::string file_name = codef2.substr(0, codef2.find(".") != codef2.npos ? (int)codef2.find(".") : (int)codef2.size());

        sortf = file_name + "_freq";

        //"保存赫夫曼树的文件名（连同后缀名）：";
        bitef = file_name + "_HTree";
        codef = file_name + "_" + baseCode;
        // "输入存赫夫曼树的文件名（连同后缀名）：";
//        scanf("%s", bitef);
//        cout << "存赫夫曼码的文件名（连同后缀名）：";
//        scanf("%s", codef2);
//        cout << "输出的翻译文件文件名（连同后缀名）：";
//        scanf("%s", transf);

        TransHT(bitef.c_str(),HT);
        transtxt();
        Translate(codef.c_str(), de_code.c_str(), HT);

        delete HT;
//		compare();	
		return true;
    }
}
