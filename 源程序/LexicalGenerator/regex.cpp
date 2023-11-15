#include "regex.h"

REGEX::REGEX()
{
    //优先数表初始化，isp:栈内，icp:栈外
    isp['(']=0;
    isp[')']=7;
    isp['*']=6;
    isp['?']=6;
    isp['&']=4;
    isp['|']=2;

    icp['(']=7;
    icp[')']=0;
    icp['*']=5;
    icp['?']=5;
    icp['&']=3;
    icp['|']=1;
}

bool REGEX::isTrans(const char c)
{
    return !isOperator(c);
}

bool REGEX::isOperator(const char c)
{
    return c=='('||c==')'||c=='?'||c=='|'||c=='*'||c=='&';
}

bool REGEX::isLegal(const string regex)//字符是否合法、括号是否匹配
{
    int len=regex.length();
    int num=0;//跟踪括号数量
    for(int i=0;i<len;i++)
    {
        char c=regex[i];
        if(!(isTrans(c)||isOperator(c)))
            return false;
        if(c=='(')
            num++;
        else if(c==')')
        {
            if(num<=0)//先出现右括号
                return false;
            num--;
        }
    }
    return num==0;
}

string REGEX::delEmpBrk(string regex)
{
    string res=regex;
    bool f=false;
    while(!f)
    {
        f=true;
        for(int i=0;i<res.length()-1;i++)
        {
            if(res[i]=='('&&res[i+1]==')')
            {
                f=false;
                if(i<res.length()-1)
                    res=res.substr(0,i)+res.substr(i+2);
                else
                    res=res.substr(0,i);
            }
        }
    }
    return res;
}

string REGEX::addOP(string regex)
{
    string res=delEmpBrk(regex);
    int len=res.length();
    for(int i=0;i<len-1;)//主要是情况枚举
    {
        char c1=res[i],c2=res[i+1];
        if(isTrans(c1)||c1==')'||c1=='?'||c1=='*')
        {
            if(isTrans(c2)||c2=='(')
            {
                res=(res.substr(0,i+1)+'&')+res.substr(i+1);
                len=res.length();
                i+=2;
            }
            else
                i++;
        }
        else
            i++;
    }
    return res;
}

bool REGEX::postfix(string regex,string &res)
{
    if(!isLegal(regex))
        return false;

    regex=addOP(regex);

    //开始转换
    stack<char> s;
    string str="";
    int len=regex.length();
    for(int i=0;i<len;)
    {
        char c=regex[i];
        if(isTrans(c))
        {
            str+=c;
            i++;
        }
        else
        {
            if(s.empty())
            {
                s.push(c);
                i++;
            }
            else
            {
                char ch=s.top();
                //①当前操作符栈外优先数大于栈顶操作符栈内优先数，当前操作符入栈；
                if(icp[c]>isp[ch])
                {
                    s.push(c);
                    i++;
                }
                //②若小于，栈顶操作符退栈并输出（或并入储存后缀表达式的字符串）；
                else if(icp[c]<isp[ch])
                {
                    while(!s.empty()&&icp[c]<isp[s.top()])
                    {
                        str+=s.top();
                        s.pop();
                    }
                }
                //③若等于，栈顶操作符退栈但不输出，如果退‘(’，读入下一字符。
                else
                {
                    if(s.top()=='(')
                        i++;
                    s.pop();
                }
            }

        }
    }
    while(!s.empty())
    {
        str+=s.top();
        s.pop();
    }

    res=str;

    return true;
}
