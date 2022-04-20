//
// Created by ByteDance on 2022/4/12.
//

#ifndef ZIP_COM_H
#define ZIP_COM_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <assert.h>
#include <vector>
#include <deque>
#include <algorithm>
#include<iomanip>
#include<fstream>
#include<math.h>
#include<string>


#define    FIRST_QTR              (TOP_VALUE/4 + 1)                                                // 四分之一范围
#define    HALF                   (2*FIRST_QTR)                                                    // 四分之二范围
#define    THIRD_QTR              (3*FIRST_QTR)                                                    // 四分之三范围
#define    NO_OF_CHARS            256                                                              // 文本中允许的符号
#define    EOF_SYMBOL             (NO_OF_CHARS + 1)                                                // 特殊符号标号（结束符）
#define    NO_OF_SYMBOLS          (NO_OF_CHARS + 1)                                                // 符号表中的符号数
#define    MAX_FREQUENCY          16383                                                            // 频数上限(2^14-1)

#define ASSERT(x, w) \
    if (!(x)) { \
        std::cout << w << std::endl; \
        assert(x); \
    }

extern int PORT;
extern int PORT2;
extern const long long head;
extern const long long second;

#define LogN 8
#define LogL 3
typedef unsigned char char_A;
const unsigned A=sizeof(char_A);
const unsigned N=((unsigned)1<<LogN)-1; //255
const unsigned L=1 << LogN; //256
const unsigned int myeof=1;
const unsigned int charactereof='#';

int check(char*);
int rand_port();

//下面实现一个接口，
int get_file_size_by_stat(const char *file);

#endif //ZIP_COM_H
