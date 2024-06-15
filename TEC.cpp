#include "TEC.h"
#include "config.h"

using namespace std;
TECnode::TECnode()
{

}

TECedge::TECedge()
{

}
TEC::~TEC()
{
	for (auto node : TECnodesList)
	{
		delete node;
	}
    for (auto edge : TECedgesList)
	{
		delete edge;
	}
}

TEC::TEC(CGRA *C, int II)
{
    int i, j, k, count;   
    TECPENum = II *C->PENum;    
    levelNum =  II;
    count = 0;
    for(i = 0; i < II; i++)  
    {
        for(j = 0; j < C->PENum; j++)
        {
            TECnode* node = new TECnode();
            node->PEorder = count; //从0开始编号
            count = count + 1; 
            node->PEkind = C->CGRAnodesList[j]->PEkind;
            node->PEtimeslot = i;
            node->PElabel = C->CGRAnodesList[j]->PElabel;  //CGRA的编号
        
            if(II != 1)//II不为1的话，多一个自己到自己的邻居
            {
                
                if(i != II-1)//不是最后一层
                {
                    node->PENeighbors.push_back(node->PElabel + C->PENum * (i+1)) ;
                }
                else//最后一层
                {
                    node->PENeighbors.push_back(node->PElabel) ;
                }
                
            }     
            
            //除了自己到自己的 其他邻居
            for(k = 0; k < C->CGRAnodesList[j]->PENeighbors.size(); k++)
            {
                if(i != II -1)//不是最后一层
                {
                    
                    node->PENeighbors.push_back(C->CGRAnodesList[j]->PENeighbors[k] + C->PENum * (i+1));
                }
                else//最后一层
                {
                    node->PENeighbors.push_back(C->CGRAnodesList[j]->PENeighbors[k]);
                }
                
            }

            TECnodesList.push_back(node);   
        }
    }

    //TEC边的信息
    for (i = 0; i < TECPENum; i++) 
    {
        
        for (j = 0; j < TECnodesList[i]->PENeighbors.size(); j++) 
        {
            TECedge* edge  = new TECedge();    
            edge->firstPE  = TECnodesList[i]->PEorder;
            edge->secondPE = TECnodesList[i]->PENeighbors[j];
            TECedgesList.push_back(edge);     
        }
    }
}

bool TEC::TECgraphHasEdge(size_t begin, size_t end)
{
	bool flag = false;
	vector<TECedge*> edges = this->TECedgesList;
	for (auto edge : edges)
	{
		if (edge->firstPE == begin && edge->secondPE == end)
		{
			flag = true;
			break;
		}
	}
	return flag;
}