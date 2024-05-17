#ifndef DFA_H
#define DFA_H

#include <iostream>
#include <map>
#include <deque>
#include "nfa.h"

#define mcmi map<char,map<int,int>>::iterator
#define mcl map<char,list<pii>>::iterator
#define stsi set<edge>::iterator

using namespace std;

class DFAGraph
{
public:
    DFAGraph(){isMinimized=false;};

    DFAGraph(NFAGraph &nfa);

    void init(NFAGraph &nfa);

    int getNumOfStates();//获取状态数

    bool isMin();//询问是否最小化

    bool acceptState(int i);//询问是否接受状态

    vector<set<edge>> getGraph();//获取邻接表

    string getState(int i);//获取状态

    DFAGraph toMinDFA();//转换至最小化

    int getInitState();//获取初始状态编号

    set<char> getCharSet();//获取字符表

private:

    int InitState;//初始状态

    bool isMinimized;//最小化

    int NumOfStates;//状态数

    vector<set<edge>> graph;//邻接表//状态转换表，<转换名,<转换前状态,转换后状态>>，其中状态使用状态集中的索引

    vector<set<int>> states; //状态集

    vector<bool> isAccepted;//接受状态，isAccepted[0]为false时表示全部状态都是接受状态

    set<char> cs;//字符表

    int addState(NFAGraph &nfa,set<int> &s);//寻找状态是否已存在，不存在则加入状态，最终返回状态参数索引
};



#endif // DFA_H
