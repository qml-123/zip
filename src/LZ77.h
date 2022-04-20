//
// Created by ByteDance on 2022/4/12.
//

#ifndef ZIP_LZ77_H
#define ZIP_LZ77_H

#include "com.h"
#include "Base_Text_Process.h"
namespace qml {
//    using namespace std;
//编码所需的三元组结构:(偏移量,匹配的符号个数,前向缓冲区中的第一个符号)
    class Three_mark {
    public:
        unsigned int elem[3];

        Three_mark(unsigned location = 0, unsigned length = 0, unsigned symbol = 0) {
            elem[0] = location; //匹配字符串的开始字符串与先行缓冲区的距离
            elem[1] = length;  //匹配长度
            elem[2] = symbol;  //字符的ascii码值
        }

        void set(unsigned location, unsigned length, unsigned symbol) {
            elem[0] = location;
            elem[1] = length;
            elem[2] = symbol;
        }

        bool iseof() {
            if (elem[0] == 0 && elem[1] == 0 && elem[2] == myeof) {
                return true;
            } else return false;
        }

        //重载对象赋值
        Three_mark operator=(Three_mark op2) {
            for (int i = 0; i < 3; i++) {
                elem[i] = op2.elem[i];
            }
            return *this;
        }
    };



/*
将编码后的标记转为二进制
输出缓冲，当转换未完成时返回false,
*/
    class Marks2bin : public std::deque<unsigned> {
    private:
        int index_b = 0;
        bool complete = true;
        bool flag = false;
    public:
        //将三元组转为二进制
        void marks2bin(deque <Three_mark> a) {
            if (a.empty())return;
            int index_a = 0;//二进制串的index,从低位往高位增加
            int index = 2; //用来取三元组的elem
            unsigned b_temp = 0;//临时值
            if (!complete) { //如果上次转换未完成，则取出队列b尾的元素
                b_temp = deque::back();
                deque::pop_back();
            }
            Three_mark a_temp = a.front();
            a.pop_front();
            if (a_temp.iseof()) {
                if (flag == false) {
                    a.push_front(a_temp);
                }
                flag = !flag;
            }
            //把三元组一个一个取出来转为二进制
            //每个三元组从后往前依此取elem[2],elem[1],elem[0],并转为二进制
            while (!((a.empty()) && (index == 0 && index_a == LogN))) {
                for (; index_b < 32; index_b++, index_a++) {
                    //代表elem[2]已经转为二进制，接下来转elem[1]:length
                    if (index == 2 && index_a == A * 8) {
                        index = 1;
                        index_a = 0;
                    }
                    //elem[2]和elem[1]都转为了二进制，接下来转elem[0]:location
                    if (index == 1 && index_a == LogL) {
                        index = 0;
                        index_a = 0;
                    }
                    ////处理完了当前的三元组，处理下一个
                    if (index == 0 && index_a == LogN) {
                        //先判断还有没有下一个
                        if (a.empty()) {
                            if (index_b == 0) {
                                complete = true;
                            } else { //没有补齐,仍需要一次操作进行补齐
                                deque::push_back(b_temp);
                                complete = false;
                            }
                            return;
                        }
                        //处理下一个
                        a_temp = a.front();
                        a.pop_front();//取出队列a中的元素
                        //下一个是eof
                        if (a_temp.iseof()) {
                            if (flag == false) {
                                a.push_front(a_temp);
                            }
                            flag = !flag;
                        }
                        index = 2; //复位
                        index_a = 0;
                    }
                    //index_a就是要处理的二进制位，从低位开始
                    //例如N的ascii码值为78，二进制为1001110
                    //index_a从低位开始，拿78的二进制的第index_a位与1相与,如果为1
                    //则将b_temp的第index_b位置为1
                    if ((a_temp.elem[index]) & 1 << index_a)
                        b_temp |= 1 << index_b;
                }
                index_b = 0;
                deque::push_back(b_temp); //存储编码后的二进制结果
                b_temp = 0;
            }
        }

        void toend() {
            unsigned b_temp = 0;
            if (!complete) {
                b_temp = deque::back();
                deque::pop_back();
            } //取出队列b尾的元素
            Three_mark a_temp(0, 0, myeof);//不能省略结束标志
            int index = 2;
            int index_a = 0;
            flag = true;
            while (flag) {
                for (; index_b < 32; index_b++, index_a++) {
                    if (index == 2 && index_a == A * 8) {
                        index = 1;
                        index_a = 0;
                    }
                    if (index == 1 && index_a == LogL) {
                        index = 0;
                        index_a = 0;
                    }
                    if (index == 0 && index_a == LogN) {
                        flag = false;
                        break;
                    }
                    if ((a_temp.elem[index]) & 1 << index_a)b_temp |= 1 << index_b;
                }
                if (index_b == 32) {
                    index_b = 0;
                    deque::push_back(b_temp);
                    b_temp = 0;
                }
            }
            for (; index_b < 32; index_b++, index_a++) {
                b_temp |= 1 << index_b;
            }
            deque::push_back(b_temp);
            complete = true;
            return;
        }

        bool iscomplete() {
            return complete;
        }

        void clear() {
            deque::clear();
            index_b = 0;
            complete = true;
            flag = false;
        }
    };

    class Bin2marks : public std::deque<Three_mark> {
    private:
        int index_a = 0;
        int index = 2;
        bool complete = true;
        bool flag = false;
    public:
        void bin2marks(deque<unsigned> b) {
            if (b.empty())return;
            int index_b = 0; //正在处理的二进制第index_b位
            Three_mark a_temp(0, 0, 0);
            if (!complete) {
                a_temp = deque::back();
                deque::pop_back();
            } //取出队列a尾的元素
            unsigned b_temp = b.front();
            b.pop_front();//取出队列b中的元素
            while (!((b.empty()) && (index_b == 32))) {
                //二进制从低位往高位处理
                switch (index) {
                    //转elem[2]
                    case 2:
                        while (index_a < A * 8) {
                            if (index_b == 32) {
                                if (b.empty()) {
                                    deque::push_back(a_temp);
                                    complete = false;
                                    return;
                                } else {
                                    b_temp = b.front();
                                    b.pop_front();
                                    index_b = 0;
                                }
                            }
                            //b_temp转为二进制后取出第index_b位的数字,转为a_temp.elem[index]上的数字
                            //例如第一个N的三元组为(0,0,78)
                            //就是把它的二进制数读取后，从右往左逐位左移，恢复出78
                            //当恢复elem[2]时，最多有8位二进制
                            if ((b_temp) & 1 << index_b)(a_temp.elem[index]) |= 1 << index_a;
                            index_a++;
                            index_b++;
                        }
                        index = 1; //要恢复elem[1]
                        index_a = 0; //复位
                    case 1:
                        while (index_a < LogL) {
                            if (index_b == 32) {
                                if (b.empty()) {
                                    deque::push_back(a_temp);
                                    complete = false;
                                    return;
                                } else {
                                    b_temp = b.front();
                                    b.pop_front();
                                    index_b = 0;
                                }
                            }
                            if ((b_temp) & 1 << index_b)(a_temp.elem[index]) |= 1 << index_a;
                            index_a++;
                            index_b++;
                        }
                        //处理elem[0]
                        index = 0;
                        index_a = 0;
                    case 0:
                        while (index_a < LogN) {
                            if (index_b == 32) {
                                if (b.empty()) {
                                    deque::push_back(a_temp);
                                    complete = false;
                                    return;
                                } else {
                                    b_temp = b.front();
                                    b.pop_front();
                                    index_b = 0;
                                }
                            }
                            if ((b_temp) & 1 << index_b)(a_temp.elem[index]) |= 1 << index_a;
                            index_a++;
                            index_b++;
                        }
                        index = 2;
                        index_a = 0;
                        if (flag) {
                            if (a_temp.iseof()) {
                                flag = false;
                                deque::push_back(a_temp);
                            } else {
                                complete = true;
                                return;
                            }
                        } else {
                            if (a_temp.iseof()) {
                                flag = true;
                            } else {
                                deque::push_back(a_temp);
                            }
                        }
                        a_temp.set(0, 0, 0);
                }
            }
        }

        bool toend() {
            if (flag) {
                if (complete == false) {
                    deque::pop_back();
                }
            }//将flag后的未完成尾部的mark取出
            return flag;
        }

        bool iscomplete() {
            return complete;
        }

        void clear() {
            deque::clear();
            index_a = 0;
            index = 2;
            complete = true;
            flag = false;
        }
    };

/*编码器*/
    class Encoder : public std::deque<char_A> {
    private:
        int boundary = -1;//观察缓冲器的起始位置下标:搜索缓冲区的右边界
        int location = 0; //偏移量
        int length = 0;//匹配长度
    public:
        //返回布尔变量，有码输出返回真并更新mark,无码输出时返回假
        bool encode(char_A ch, Three_mark *mark) {
            deque::push_back(ch);//入队
            switch (length) {
                case 0: //如果length代表0，则代表ch是可能连续匹配的字符串的第一个字符
                    for (int i = 1; i <= boundary; i++) {
                        if (ch == deque::at(boundary - i)) { //从搜索缓冲区的右边开始向左搜寻
                            location = i; //改变偏移量
                            length++;//匹配时boundary不变,length++
                            return false;
                        }
                    }
                    break;
                case L - 1:
                    break;
                default: ////ch是连续字符串的第二个及之后的字符,它前面至少有1个字符是已经被搜索到了
                    //对于第一个后的后面字符，它相对于在搜索缓冲区中找到的前一个字符的后一个位置
                    //例如:negatitions这个字符,第一个t,i都是未搜索到的，有marks编码
                    //第二个t是能搜索到的，并在搜索到时记录了偏移量Location,length
                    //第二个i也能搜索到，它相对于先行缓冲区中的第一个t的位置就是：boundary - location + lengt
                    //先行缓冲区的边界boundary-偏移量location+length，比较对应位置的字符是否相同
                    if (ch == deque::at(boundary - location + length)) {
                        length++;//匹配时boundary不变,length++
                        return false;
                    }
                        //连续字符串匹配中遇到不相同,即匹配o时,则要将Location移动
                    else {
                        //因为下标从0开始，所以i=location+1,定义到从第一个匹配的字符t开始
                        for (int i = location + 1, j; i <= boundary; i++) {
                            j = 0;
                            //deque::at(boudary+j)从negotiations的第二个t开始，依次匹配
                            //deque::at(boudary-i+j)代表向搜索缓冲区的左边搜寻，尝试能否重新匹配上连续的字符串
                            while (j <= length && deque::at(boundary - i + j) == deque::at(boundary + j)) {
                                j++;
                            }
                            //length为原来匹配上的长度
                            //能重新匹配上,即j>length
                            if (j > length) {
                                location = i;////修改偏移量
                                length++;//增加长度，以供下一个字符尝试能否更长匹配
                                return false;
                            }
                        }
                    }
                    break;
            }
            //没找到,则要转为mark三元组
            (*mark).set(location, length, ch);
            location = length = 0;
            for (int i = (int) deque::size() - N; i > 0; i--) {
                deque::pop_front();
            }//将搜索缓冲器滑到尾端，此时还有N个元素
            //未匹配时boundary=size,因为可能为新增一个未匹配上的字符,boundary要+1
            //也可能是匹配多个字符+后面一个没匹配上的字符，然后要修改boundary为boundary+匹配上的字符串长度length+1
            boundary = deque::size();
            return true;
        }

        void toend(Three_mark *mark) {
            deque::push_back(charactereof);//入队
            //		seetheprocess();//显示搜索窗与观察窗
            (*mark).set(location, length, charactereof);
            location = length = 0;
            for (int i = (int) deque::size() - N; i > 0; i--) {
                deque::pop_front();
            }//将搜索缓冲器滑到尾端，此时还有N个元素
            boundary = deque::size();//未匹配时boundary=size
            return;
        }

        void clear() {
            deque::clear();
            boundary = -1;//观察缓冲器的起始位置下标
            location = 0;//0代表未匹配，范围是1——N
            length = 0;//0代表未匹配，范围是1——L-1
        }
    };

    class Decoder : public std::deque<char_A> {
    public:
        bool decode(deque <char_A> *buffer, Three_mark *mark) {
            //处理不是第一次出现的字符的偏移量和长度
            //j是搜索缓冲区中搜寻到的第一个字符的起始位置
            //i<j+i
            //例如negotiations，恢复第二个ti时，deque::size()=7,输入的三元组为(3,2,111)
            //则i=0,j=7-3=4,4就是第一个t的起始位置,i向后移动elem[1]个位置
            for (int i = 0, j = (int) deque::size() - (*mark).elem[0]; i < (*mark).elem[1]; i++) {
                deque::push_back(deque::at(j + i));
                (*buffer).push_back(deque::back());
            }
            deque::push_back((*mark).elem[2]);
            (*buffer).push_back(deque::back());
            for (int i = 0, j = (int) deque::size() - N; i < j; i++) {
                deque::pop_front();
            }//将搜索缓冲器滑到尾端，此时还有N个元素
            return true;
        }

        void toend(deque <char_A> *buffer) {
            if ((!(*buffer).empty()) && (*buffer).back() == charactereof) {
                (*buffer).pop_back();
            }
            return;
        }

        void clear() {
            deque::clear();
        }
    };

/*读入文件，完成编码*/
    class File2mark : public Encode{
    private:
        std::fstream file;
        Encoder encoder;
        std::deque <char_A> charin; //存放输入流
        std::deque <Three_mark> marks; //存放编码后的三元组结果
        Marks2bin binbuffer;
        char_A char_A_temp;
        Three_mark mark_temp = Three_mark(0, 0, 0); //存放临时的mark
    public:
        bool file_encode(std::string ifilename, bool o2file, std::string ofilename = "") {
            file.open(ifilename, std::ios::in | std::ios::binary);
            if (!file) {
                std::cout << "cannot open file:" << ifilename << std::endl;
                return false;
            }
            while (file.read((char *) &char_A_temp, A)) {
                charin.push_back(char_A_temp);
            }
            charin.push_back(file.gcount() == 0 ? 0 : (char_A_temp & ((1 << 8 * (A - 1)) - 1)) |
                                                      (unsigned) file.gcount() << 8 * (A - 1));//补齐字
            file.close();
            //LZ77编码并将输出三元组存入markdeque这一队列中
            for (int i = 0, j = charin.size(); i < j; i++) {
                bool myflag = encoder.encode(charin[i], &mark_temp); //代表是否需要编码
                //如果字符charin[i]搜寻到,即f=false，则不用编码，不用放入marks中
                if (myflag) {
                    marks.push_back(mark_temp);
                }
            }
            encoder.toend(&mark_temp); //处理结尾eof
            marks.push_back(mark_temp);
            //不把二进制存文件就返回
            if (o2file == false) {
                return true;
            }//不必存入文件则返回
            //三元组转换成二进制数据存入binbuffer中
            binbuffer.marks2bin(marks);
            binbuffer.toend();
            //将二进制流binbuffer写入文件
            file.open(ofilename, std::ios::out | std::ios::binary);
            if (!file) {
                std::cout << "cannot open file:" << ofilename << std::endl;
                return false;
            }
            for (int i = 0; i < binbuffer.size(); i++) {
                file.write((char *) &binbuffer[i], 4);
            }
            clear();
            file.close();
            return true;
        }

        void clear() {
            encoder.clear();
            charin.clear();
            marks.clear();
            binbuffer.clear();
            mark_temp = Three_mark(0, 0, 0);
        }
    };

/*读取二进制文件，转为三元组后解码为源文本*/
    class File2char : public Decode{
    private:
        std::fstream file;
        Decoder decoder;
        std::deque <char_A> charout;
        std::deque<unsigned> binbuffer;
        Bin2marks marks;
        unsigned unsigned_temp;
    public:
        bool file_decode(std::string ifilename, bool o2file, std::string ofilename) {
            //将文件中数据读入binbuffer
            file.open(ifilename, std::ios::in | std::ios::binary);
            if (!file) {
                std::cout << "cannot open file:" << ifilename << std::endl;
                return false;
            }
            while (file.read((char *) &unsigned_temp, 4)) {
                binbuffer.push_back(unsigned_temp);
            }
            file.close();
            //二进制数据转换成三元组存入marks中
            marks.bin2marks(binbuffer);
            marks.toend();
            //LZ77用三元组队列marks解码并存入charout
            for (int i = 0; i < marks.size(); i++) {
                decoder.decode(&charout, &marks[i]);
            }
            decoder.toend(&charout);
            if (o2file == false) {
                return true;
            }//不必存入文件则返回
            //解码后写入文件
            file.open(ofilename, std::ios::out | std::ios::binary);
            if (!file) {
                std::cout << "cannot open file:" << ofilename << std::endl;
                return false;
            }
            bool thelast = true;
            if (charout.empty()) thelast = false;
            for (int i = 0, j = charout.size() - thelast; i < j; i++) {
                file.write((char *) &charout[i], A);
            }
            if (thelast) {
                file.write((char *) &charout.back(), charout.back() >> 8 * (A - 1));
                clear();
            }
            file.close();
            return true;
        }

        void clear() {
            decoder.clear();
            charout.clear();
            binbuffer.clear();
            marks.clear();
        }
    };
}


#endif //ZIP_LZ77_H
