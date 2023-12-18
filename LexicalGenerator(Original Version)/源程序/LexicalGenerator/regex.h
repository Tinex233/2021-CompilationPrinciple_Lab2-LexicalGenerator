#ifndef REGEX_H
#define REGEX_H

#include <iostream>
#include <algorithm>
#include <string>
#include <string.h>
#include <unordered_map>
#include <stack>

using namespace std;

class REGEX
{
public:
    REGEX();

    static bool isTrans(const char c);//是否为转换

    static bool isOperator(const char c);//是否是合法运算符

    static bool isLegal(const string regex);//简单检测正则表达式是否合法，主要关注字符上的错误，忽略部分语法出现的错误

    static string addOP(string regex);//添加'&'连接符号

    bool postfix(string regex,string &res);//将中缀正则表达式转后缀，res传参接收结果，返回表达式是否合法的布尔值

private:
    unordered_map<char,int> isp,icp;

    static string delEmpBrk(string regex);//删除空括号

};

#endif // REGEX_H
