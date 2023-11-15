#ifndef NFA_H
#define NFA_H

#include <iostream>
#include <algorithm>
#include <string>
#include <string.h>
#include <vector>
#include <list>
#include <set>
#include <QDebug>
#include "regex.h"

#define pii pair<int,int>
#define sti set<int>::iterator
#define  vsi vector<set<edge>>::iterator

using namespace std;

struct edge//转换边
{
    char CvType;//转换类型
    int vj;//转换的下一状态
    edge(){CvType='0',vj=0;}
    edge(char c,int v):CvType(c),vj(v){}
    bool operator<(const edge &t)const {if(CvType==t.CvType)return vj<t.vj;  else return CvType<t.CvType;}
    bool operator==(const edge &t)const {return vj==t.vj&&CvType==t.CvType;}
    bool operator!=(const edge &t)const {return vj!=t.vj||CvType!=t.CvType;}
};

class NFAGraph
{
public:
    NFAGraph();

    NFAGraph(string regex);

    void init(string regex);//初始化

    int getNumOfStatus();//获取状态数

    int getNumOfchar();//获取字符表字符数量

    int getInitState();//获取初始状态号

    int getAcceptState();//获取接收状态号

    vector<set<int>> getClosure();//获取ε-闭包

    map<int,int> getCharTrans(char ch);//获取字符转换，以<转换前状态，转换后状态>形式返回

    string getStateTrans(int row,char col);//获取某行列的状态转换

    set<char> getCharSet();//获取字符集

private:
    int NumOfStatus;//NFA结构的状态数

    int InitState;//初始状态

    int AcceptState;//接收状态

    vector<set<edge>> graph;//邻接表，从1开始

    set<char> cs;//字符表

    void DFS(set<int> &s,vector<bool> &f,int fa);//深搜ε-闭包
};

#endif // NFA_H
