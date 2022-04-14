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

#include<iomanip>
#include<fstream>
#include<math.h>
#include<string>

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

#endif //ZIP_COM_H
