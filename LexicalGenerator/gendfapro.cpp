#include "gendfapro.h"
#include "dfa.h"

string GenDFAProgram(DFAGraph &dfa)
{
    if(!dfa.isMin())
        return "This DFA isn't the minimized DFA!";
    string res="int state=";
    int initState=dfa.getInitState();
    res+=to_string(initState);
    res+=";\nchar trans;\n";
    string cyCondition="while((";//循环条件
    string cyBody="{\n\tswitch(state)\n\t{\n";//循环体内容
    vector<set<edge>> g=dfa.getGraph();
    for(int i=1;i<g.size();i++)
    {
        if(g[i].size()>0)//状态i有转换
            cyCondition+="state=="+to_string(i)+"||";
        if(g[i].size()>0)
        {
            cyBody+="\tcase "+to_string(i)+":\n";
            cyBody+="\t\tswitch(trans)\n\t\t{\n";
            for(auto j:g[i])
            {
                cyBody+="\t\tcase \'";
                cyBody+=j.CvType;
                cyBody+="\':\n";
                cyBody+="\t\t\tstate="+to_string(j.vj);
                cyBody+=";\n\t\t\tbreak;\n";
            }
            cyBody+="\t\tdefault:\n\t\t\tstate=-1;\n\t\t\tbreak;\n\t\t}\n";
            cyBody+="\t\tbreak;\n";
        }
    }
    cyCondition=cyCondition.substr(0,cyCondition.length()-2)+")&&cin>>trans)\n";
    cyBody+="\tdefault:\n\t\tstate=-1;\n\t\tbreak;\n\t}\n}";
    string pgend="\nif(";
    for(int i=1;i<g.size();i++)
    {
        if(dfa.acceptState(i))
            pgend+="state=="+to_string(i)+"||";
    }
    pgend=pgend.substr(0,pgend.size()-2)+")\n";
    pgend+="\tcout<<\"Accept\"<<endl;\nelse\n\tcout<<\"Error\"<<endl;";
    res+=cyCondition;
    res+=cyBody;
    res+=pgend;

    return res;
}
