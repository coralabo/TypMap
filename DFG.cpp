#include "CGRA.h"
#include "DFG.h"
#include "config.h"
#include "tool.h"
#define childNum 4 /* 设置每个节点的孩子节点为4个 */
using namespace std;

DECLARE_string(dfg_file);
DFG::~DFG()
{
	for (auto node : DFGnodesList)
	{
		delete node;
	}
    for (auto edge : DFGedgesList)
	{
		delete edge;
	}
}



DFGnode::DFGnode()
{

}

DFGedge::DFGedge()
{

}

DFG::DFG()
{
    int i, j, k;
    vector<vector<int>> DFG_node;
    int lines = ReadDataFromFileLBLIntoString(DFG_node);
    
    numDFGnodes = lines;
    
    for (int i = 0; i < DFG_node.size(); ++i) {
       for (int j = 0; j < DFG_node[i].size(); j++){
           cout << DFG_node[i][j] << ",";
       }   
       cout << endl;
    } 
     
    int count_nodes = 0;
    for (int i = 0; i < DFG_node.size(); i++){
        DFGnode *node = new DFGnode();
        node->nodeorder = count_nodes;
        node->nodelabel = DFG_node[i][0];
        node->nodelevel = DFG_node[i][1];
        node->oldlevel =  DFG_node[i][1];
        node->kind = DFG_node[i][2];
        DFGnodesList.push_back(node);   
        count_nodes ++;
    }

    int edge_count = 0;
    for (int i = 0; i < DFG_node.size(); i++){
        int current_node_order = DFG_node[i][0];
        int child[childNum]={0}; 
        for (int j = 0; j < childNum; j++)
        {         
            child[j] = DFG_node[i][3 + j];
        }
        for (int l = 0; l < childNum; l++){
            if(child[l] != -1){/* 因为编号从0开始，所以-1表示不是孩子 */
                DFGedge *edge = new DFGedge();
                edge->edgeorder = edge_count;
                edge->prenode = current_node_order;
                edge->posnode = child[l];
                DFGedgesList.push_back(edge);   
                edge_count ++;
            }
        }
    }
    numDFGedges = edge_count;
    int numedges = numDFGedges; 
}


bool DFG::DFGgraphHasEdge(size_t begin, size_t end)
{
	bool flag = false;
	vector<DFGedge*> edges = this->DFGedgesList;
	for (auto edge : edges)
	{
		if (edge->prenode == begin && edge->posnode == end)
		{
			flag = true;
			break;
		}
	}
	return flag;
}

bool DFG::DFGHasNode(size_t n)
{
	bool flag = false;
 
	vector<DFGnode*> nodes = this->DFGnodesList;
	for (auto node : nodes)
	{
		if (node->nodelabel == n )
		{
			flag = true;
			break;
		}
	}
	return flag;
}

int DFG:: getNodeTime(int nodeLabel)
{
	vector<DFGnode*> nodesList = this->DFGnodesList;
	for (auto node : nodesList)
	{
		if (node->nodelabel == nodeLabel)
		{
			return node->oldlevel;
		}
	}
	return -1;
}

void DFG::CreatMDFG(int II){

    for(int i = 0; i <this->numDFGnodes; i++)
    { 
        this->DFGnodesList[i]->nodelevel = this->DFGnodesList[i]->nodelevel % II;    
    }
}

int DFG::Constraint_Level(int PENum, int II){
    int i;
    int numlevel[II]; 
    int flag;   
    
    flag = 0;
    for(i = 0; i < II; i++)
    {
        numlevel[i] = 0;
    }

    for(i = 0; i < this->numDFGnodes; i++)
    {
        numlevel[this->DFGnodesList[i]->nodelevel] ++;
    }

    for(i = 0; i < II; i++)
    {
        if(numlevel[i] > PENum)
        {
            flag = this->DFGnodesList[i]->nodelevel;
            printf("The %d Level of the DFG cannot meet the constrait! \n", flag);
            printf("*****************************************************************************\n");
            break;
        }
    }
    return flag;
}

