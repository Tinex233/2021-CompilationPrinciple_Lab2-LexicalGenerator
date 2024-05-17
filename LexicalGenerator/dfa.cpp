#include "dfa.h"

bool setEqual(const set<int> &s1,const set<int> &s2)
{
    if(s1.size()!=s2.size())
        return false;
    sti it1=s1.begin(),it2=s2.begin();
    for(;it1!=s1.end()&&it2!=s2.end();it1++,it2++)
    {
        if(*it1!=*it2)
            return false;
    }
    return true;
}

bool transEqual(const set<edge> &s1,const set<edge> &s2)
{
    if(s1.size()!=s2.size())
        return false;
    stsi it1=s1.begin();
    for(;it1!=s1.end();it1++)
    {
        if(s2.count(*it1)==0)
            return false;
    }
    return true;
}

DFAGraph::DFAGraph(NFAGraph &nfa)
{
    init(nfa);
}

void DFAGraph::init(NFAGraph &nfa)
{
    isMinimized=false;
    vector<set<int>> closure=nfa.getClosure();//ε-闭包
    map<char,map<int,int>> chtrans;//字符转换表
    set<char> chs=nfa.getCharSet();//获取字符集
    for(auto c:chs)
    {
        if(c!='#')
        {
            cs.insert(c);
            chtrans[c]=nfa.getCharTrans(c);
        }
    }

    if(closure.size()<=1)
        return;

    states.push_back(set<int>());
    isAccepted.push_back(false);

    //加入初始状态
    int initst=nfa.getInitState();
    states.push_back(closure[initst]);
    graph.push_back(set<edge>());//下标0不用
    graph.push_back(set<edge>());//为初始状态创建邻接表
    InitState=states.size()-1;
    if(closure[initst].find(nfa.getAcceptState())!=closure[initst].end())
        isAccepted.push_back(true);
    else
    {
        isAccepted.push_back(false);
        isAccepted[0]=1;
    }
    //开始转换
    for(int i=1;i<states.size();i++)
    {
        for(mcmi mi=chtrans.begin();mi!=chtrans.end();mi++)
        {
            char ch=mi->first;//当前转换字符
            set<int> res;
            for(auto j:states[i])//遍历i的状态集合
            {
                map<int,int>::iterator it=chtrans[ch].find(j);
                if(it!=chtrans[ch].end())//当前字符ch，当前状态j，若找到转换的状态，进行集合并
                    set_union(res.begin(),res.end(),closure[it->second].begin(),closure[it->second].end(),inserter(res,res.begin()));
            }
            if(res.empty())
                continue;

            int sz=states.size();
            int w=addState(nfa,res);
            if(sz<states.size())
                graph.push_back(set<edge> ());
            graph[i].insert(edge(ch,w));
        }
    }
    NumOfStates=states.size()-1;
}

int DFAGraph::addState(NFAGraph &nfa,set<int> &s)
{
    for(int i=1;i<states.size();i++)
    {
        if(setEqual(s,states[i]))
            return i;
    }
    states.push_back(s);
    if(s.find(nfa.getAcceptState())!=s.end())
        isAccepted.push_back(true);
    else
    {
        isAccepted.push_back(false);
        isAccepted[0]=1;
    }
    return states.size()-1;
}

int DFAGraph::getNumOfStates()
{
    return NumOfStates;
}

vector<set<edge>> DFAGraph::getGraph()
{
    return graph;
}

bool DFAGraph::acceptState(int i)
{
    if(i>NumOfStates)
        return false;
    return isAccepted[i];
}

string DFAGraph::getState(int i)
{
    if(i>NumOfStates)
        return "error";
    string res="";
    for(sti j=states[i].begin();j!=states[i].end();)
    {
        res+=to_string(*j);
        j++;
        if(j!=states[i].end())
            res+=",";
    }
    return res;
}

bool DFAGraph::isMin()
{
    return isMinimized;
}

DFAGraph DFAGraph::toMinDFA()
{
    DFAGraph mdfa;
    if(NumOfStates<=0)
        return mdfa;

    vector<int> setNum;//集合号表，指示DFA集合被划分到minDFA的哪个集合中
    setNum.resize(NumOfStates+1);
    set<int> k1,k2;//划分为非终止状态集k1和终止状态集k2
    int mNumStates;//MDFA状态数

    if(isAccepted[0]==false)//是否全为接受状态
        mNumStates=1;
    else
        mNumStates=2;

    for(int i=1;i<isAccepted.size();i++)
    {
        if(mNumStates==2)
        {
            if(isAccepted[i])
            {
                setNum[i]=2;
                k2.insert(i);
            }
            else
            {
                setNum[i]=1;
                k1.insert(i);
            }
        }
        else
        {
            setNum[i]=1;
            k2.insert(i);
        }
    }

    deque<set<int>> dq;//状态集合
    if(k1.size()>1)
        dq.push_back(k1);
    if(k2.size()>1)
        dq.push_back(k2);

    bool part=true;//当上一次队列中集合发生过划分时，需要重新检查一遍是否可划分
    while(part)
    {
        if(mNumStates>2)//统计各集合划分情况，对多于一个元素的集合检查是否能划分
        {
            for(int ii=1;ii<=mNumStates;ii++)
            {
                set<int> sss;
                for(int ij=1;ij<=isAccepted.size();ij++)
                    if(setNum[ij]==ii)
                        sss.insert(ij);
                if(sss.size()>1)
                    dq.push_back(sss);
            }
        }
        part=false;
        while(!dq.empty())//开始最小化
        {
            set<int> s=dq.front();
            dq.pop_front();
            vector<set<edge>> ve;//转移集合表
            vector<set<int>> vs;//划分的集合
            for(auto i:s)//对集合s进行分割
            {
                //获取i的能转移到的集合号的集合
                set<edge> ste;
                for(int vi=1;vi<graph.size();vi++)
                {
                    for(auto cm:graph[i])//遍历i的邻接表
                        ste.insert(edge(cm.CvType,setNum[cm.vj]));//在集合号表中找到转换后状态所在的集合
                }

                if(vs.size()==0)
                {
                    ve.push_back(ste);
                    vs.push_back(set<int>());
                    vs[0].insert(i);
                }
                else
                {
                    bool fequal=false;//转换相同标志
                    for(int vi=0;vi<ve.size();vi++)
                    {
                        if(transEqual(ve[vi],ste))
                        {
                            vs[vi].insert(i);//转换相同
                            fequal=true;
                            break;
                        }
                    }
                    if(!fequal)//不同，划分
                    {
                        ve.push_back(ste);
                        vs.push_back(set<int>());
                        vs[vs.size()-1].insert(i);
                        part=true;
                    }
                }
            }
//            for(int i=0;i<vs.size();i++)
//            {
//                qDebug()<<"set"<<i;
//                for(auto vv:vs[i])
//                    qDebug()<<vv;
//            }
            //清算划分集合vs
            for(int j=1;j<vs.size();j++)//为划分出去的集合编号
            {
                if(vs[j].size()>1)
                    dq.push_back(vs[j]);
                mNumStates++;
                for(auto vk:vs[j])//遍历集合内状态，为其赋予新的状态号
                {
                    setNum[vk]=mNumStates;
                }
            }
        }
    }

    mdfa.InitState=setNum[InitState];//mdfa的初始状态是dfa初始状态被划分到的集合
    mdfa.NumOfStates=mNumStates;
    mdfa.states.resize(mNumStates+1);
    mdfa.isAccepted.resize(mNumStates+1);
    mdfa.isMinimized=true;
    mdfa.cs=set<char> (cs);
    mdfa.graph.resize(mNumStates+1);
    for(int t=1;t<mdfa.states.size();t++)
        mdfa.states[t].insert(t);

    //观察各状态编号
//    for(int ii=1;ii<setNum.size();ii++)
//        qDebug()<<"stateNum"<<ii<<":"<<setNum[ii];

    //合并状态集合
    //接受状态判断
    for(int i=1;i<setNum.size();i++)
    {
        if(isAccepted[i])
            mdfa.isAccepted[setNum[i]]=true;
        else
            mdfa.isAccepted[0]=true;
    }
//    for(int i=1;i<mdfa.isAccepted.size();i++)
//        qDebug()<<"accept"<<mdfa.isAccepted[i];

    //构建最小化DFA的邻接表
    for(int vi=1;vi<graph.size();vi++)
    {
        set<edge> ts;//存储状态合并后的集合转换
        for(auto ls:graph[vi])
        {
            int w=ls.vj;
            char ttype=ls.CvType;
            //映射到合并后集合
            w=setNum[w];
            ts.insert({ttype,w});
        }

        //处理mdfa的状态转换表
        for(auto ti:ts)
            mdfa.graph[setNum[vi]].insert(ti);
    }

    return mdfa;
}

int DFAGraph::getInitState()
{
    return InitState;
}

set<char> DFAGraph::getCharSet()
{
    return cs;
}
