#include "CGRA.h"
#include "DFG.h"
#include <vector>

#ifndef _TEC_H_
#define _TEC_H_

using namespace std;

class TECnode             //����һ��TEC�е�PE�ڵ���
{
public:
    TECnode();
    int PEorder;//����TEC��PE����ţ���1��2��3��4��5��6��7��8
    int PElabel;          //CGRA的PE编号
    int PEtimeslot;       //����TEC��PE������ʱ��Ƭ����1��2
    int PEkind;//����PE�����ͣ�0����ͨFU,1��LSU
   // int PENeighbor[maxdegree + 1];//����TEC��PE��ͨ�ŵ�����PE     
    vector<int> PENeighbors; 
    
};

class TECedge          //����TECһ����edge�����ݽṹ�����ͣ���CGRA��ĳ����PE֮��ıߨD�D���ӹ�ϵ
{
public:
    TECedge();
    int firstPE;           //�����ӱ��еĵ�һ��PE���
    int secondPE;		   //�����ӱ��еĵڶ���PE���
    int firstPEslot;	   //�����ӱ��еĵ�һ��PE����ʱ��Ƭtimeslot
    int secondPEslot;	   //�����ӱ��еĵڶ���PE����ʱ��Ƭtimeslot  
    int edgeorder;         //�����ӱ��еıߵ����
};

class TEC
{
public:
    int levelNum;          /*  TEC的层数 */       
    int TECPENum;					 //����TEC���ܵ�PE��
    int TECedgs;				 //����TEC�е��ܱ���

    vector<TECnode*> TECnodesList;//
    vector<TECedge*> TECedgesList;//TEC边集合
    bool TECgraphHasEdge(size_t begin, size_t end);

   
    TEC(CGRA *C, int II);
    ~TEC();
};

#endif // TEC_H


