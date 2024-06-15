#ifndef DFG_H
#define DFG_H

#include <vector>
using namespace std;

class DFGnode{
public:
    DFGnode();
    int nodeorder;//DFG算子的序列号，从0开始，为唯一ID
    int nodelabel;//DFG算子的编号，就是它的名字
    int oldlevel;//DFG原始时间步
    int nodelevel;//DFG折叠后的时间步
    int kind;//DFG算子的种类
    
};

class DFGedge{
public:
    DFGedge();//DFGedge���캯��
    int edgeorder;//
    int prenode;//
    int posnode;//
   
};

class DFG{
public:
    int numDFGnodes;
    int numDFGedges;
    vector<DFGnode*> DFGnodesList;//
    vector<DFGedge*> DFGedgesList;//DFG边集合
    DFG();
    DFG* CreatDFG();//通过文件创建一个DFG
    ~DFG();
    bool DFGgraphHasEdge(size_t begin, size_t end);
    bool DFGHasNode(size_t node);
    int getNodeTime( int nodeLabel);
    void CreatMDFG(int II);
    int Constraint_Level(int PENum,int II);
};

#endif // DFG_H

