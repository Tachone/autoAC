
#define _CRT_SECURE_NO_WARNINGS   //vs 2013用于忽略c语言安全性警告  
#include <cstdio>  
#include <stdlib.h>
#include <iostream>  
#include <fstream>  
#include <string> 
#include <cstring>  
#include <regex>  
#include <vector> 
#include <ctime>
#include <queue>  
#include <algorithm>  
#include <winsock2.h> 
#include <windows.h>
#include <map> 
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "EndOfFile")
#pragma comment(lib, "ws2_32.lib")  

using namespace std;

void regexGetcom(string &allHtml);
bool analyUrl(char *url);
void GetCode(string &allHtml);
string ReplaceDiv(string &CodeHtml);

string ASCtoHex(int num);//十进制转换成十六进制
string GetRescode(string &CodeHtml);

void GetResult(string &allHtml, int Prob);  //解析出state.php中的结果，空间，时间

char* U2G(const char* utf8);  //UTF-8到GB2312的转换
char* G2U(const char* gb2312);  //GB2312到UTF-8的转换


