#ifndef _FOO_H_
#define _FOO_H_

/* Library file defnitions*/
#include "proHeader.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<errno.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<sstream>
#include<string>
#include<algorithm>
#include<ctime>
#include<math.h>


#define LGTBLUE "\e[94m"
#define MAGENTA "\e[35m"
#define LGTCYAN "\e[96m"
#define YELLOW "\e[33m"
#define RED "\e[91m"
#define LGTYELLOW "\e[93m"
#define BOLD "\e[1m"
#define DARK_GREY "\e[100m"
#define RESET_COLOR "\e[0m"  

using namespace std;

class FileDetails {
          
          public: 
                  string path;
                  string fileName;
                  char fileType;
                  string perms;
                  int noOfLinks ;
                  string userName ;
                  string groupName;
                  string  fSize;
                  long long int fileSize;
                  string moTime;
                  time_t timeSort;
  };


/* Decalaration of the functions defined in filereversal.c */
extern bool my_basic_ls_sort(const string &left ,const string &right);
extern char fileType(struct stat fileAttrs);
extern string findAndPrintPermissions(struct stat fileAttrs);
extern long int findLinks(struct stat fileAttrs);
extern string getUserName(struct stat fileAttrs);
extern string  getGroupName(struct stat fileAttrs);
extern string getFileSize(struct stat fileAttrs);
extern long long int getFsSort(struct stat fileAttrs);
extern time_t getTimeSort(struct stat fileAttrs);
extern string getTime(struct stat fileAttrs);
extern string getColouredPrint(struct stat fileAttrs , string fname);
extern vector<FileDetails> calling_R(FileDetails fd ,int lflag ,int aflag , int noflag , int Sflag ,int tflag , const char* path,vector<FileDetails> &superVector);
extern long long int getTotalBlocks(struct stat fileAttrs);
extern vector<FileDetails> filter_a(FileDetails fd ,const char* path, vector<FileDetails> &superVector);
extern vector<FileDetails> filter_noOption(FileDetails fd,const char* path, vector<FileDetails> &superVector);
extern void filter_l(const char* path, vector<FileDetails> &superVector);
extern void filter_S( const char* path, vector<FileDetails> &superVector);
extern vector<FileDetails> filter_d( const char* path, vector<FileDetails> &superVector);
extern void filter_t(const char* path, vector<FileDetails> &superVector);
extern vector<string> addFilesToList(vector<string> vp , const char* path);
extern char* makePath(string fileName , const char* path);
extern bool my_func_to_sort_size(const FileDetails fd1 ,const FileDetails fd2);
extern bool my_func_to_sort_time(const FileDetails fd1 ,const FileDetails fd2); 
extern char* getResolveLink(struct stat fileAttrs, char* fileNamePath);
extern void colorText(string text , const char* path);

#endif

