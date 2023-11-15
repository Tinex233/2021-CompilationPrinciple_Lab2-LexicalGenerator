#include "nfa.h"

NFAGraph::NFAGraph(){}

NFAGraph::NFAGraph(string regex)
{
    init(regex);
}

void NFAGraph::init(string regex)
{
    NumOfStatus=0;
    stack<pii> ss;//后缀计算栈，存储状态
    graph.push_back(set<edge>());
    int len=regex.length();

    for(int i=0;i<len;i++)
    {
        char cur=regex[i];
        if(REGEX::isTrans(cur))//创建基本正则表达式单字符
        {
            cs.insert(cur);
            int start=++NumOfStatus,end=++NumOfStatus;//创建两个新状态
            graph.push_back(set<edge>()),graph.push_back(set<edge>());
            graph[start].insert(edge(cur,end));
            ss.push({start,end});
        }
        else if(cur=='&')//连接
        {
            pii nt=ss.top();
            ss.pop();
            pii ns=ss.top();
            ss.pop();

            int nsleft=ns.first,nsright=ns.second;
            int ntleft=nt.first,ntright=nt.second;

            graph[nsright].insert(edge('#',ntleft));
            ss.push({nsleft,ntright});
            cs.insert('#');
        }
        else if(cur=='|')//选择
        {
            pii nt=ss.top();
            ss.pop();
            pii ns=ss.top();
            ss.pop();

            int nsleft=ns.first,nsright=ns.second;
            int ntleft=nt.first,ntright=nt.second;

            int start=++NumOfStatus,end=++NumOfStatus;
            graph.push_back(set<edge>()),graph.push_back(set<edge>());
            graph[start].insert(edge('#',nsleft));
            graph[start].insert(edge('#',ntleft));
            graph[nsright].insert(edge('#',end));
            graph[ntright].insert(edge('#',end));
            ss.push({start,end});
            cs.insert('#');
        }
        else if(cur=='*')//闭包
        {
            pii ns=ss.top();
            ss.pop();
            int nsleft=ns.first,nsright=ns.second;

            int start=++NumOfStatus,end=++NumOfStatus;
            graph.push_back(set<edge>()),graph.push_back(set<edge>());

            graph[start].insert(edge('#',end));
            graph[start].insert(edge('#',nsleft));
            graph[nsright].insert(edge('#',end));
            graph[nsright].insert(edge('#',nsleft));
            ss.push({start,end});
            cs.insert('#');
        }
        else if(cur=='?')//可选
        {
            pii ns=ss.top();
            ss.pop();
            int nsleft=ns.first,nsright=ns.second;

            int start=++NumOfStatus,end=++NumOfStatus;
            graph.push_back(set<edge>()),graph.push_back(set<edge>());

            graph[start].insert(edge('#',end));
            graph[start].insert(edge('#',nsleft));
            graph[nsright].insert(edge('#',end));
            ss.push({start,end});
            cs.insert('#');
        }
    }
    if(!ss.empty())
    {
        InitState=ss.top().first;
        AcceptState=ss.top().second;
        //qDebug()<<InitState<<"t"<<AcceptState<<endl;
    }
}

int NFAGraph::getNumOfStatus()
{
    return NumOfStatus;
}

int NFAGraph::getNumOfchar()
{
    return cs.size();
}

set<char> NFAGraph::getCharSet()
{
    return cs;
}

string NFAGraph::getStateTrans(int row, char col)
{
    if(row>NumOfStatus)
        return "";
    set<int> s;
    for(auto i:graph[row])
    {
        if(i.CvType==col)
            s.insert(i.vj);
    }
    string res="";

    for(sti it=s.begin();it!=s.end();)
    {
        res+=to_string(*it);
        it++;
        if(it!=s.end())
            res+=",";
    }
    return res;
}

int NFAGraph::getInitState()
{
    return InitState;
}

int NFAGraph::getAcceptState()
{
    return AcceptState;
}

void NFAGraph::DFS(set<int> &s,vector<bool> &f,int fa)
{
    s.insert(fa);
    f[fa]=true;
    for(auto node:graph[fa])
    {
        if(node.CvType=='#')
            DFS(s,f,node.vj);
    }
}

vector<set<int>> NFAGraph::getClosure()
{
    vector<set<int>> v;
    if(NumOfStatus<=0)
        return v;
    v.push_back(set<int>());

    for(int i=1;i<=NumOfStatus;i++)
    {
        set<int> s;
        vector<bool> f(NumOfStatus+1,false);
        DFS(s,f,i);
        v.push_back(s);
    }

    return v;
}

map<int,int> NFAGraph::getCharTrans(char ch)
{
    map<int,int> m;
    for(int i=1;i<=NumOfStatus;i++)
    {
        for(auto j:graph[i])
        {
            if(j.CvType==ch)
                m[i]=j.vj;
        }
    }
    return m;
}
