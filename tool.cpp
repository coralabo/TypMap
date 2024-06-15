#include "config.h"
#include "DFG.h"
#include "TEC.h"
#include "tool.h"

using namespace std;
DECLARE_string(dfg_file);


void strtok(string& str, vector<string>& cont, string defstr = " ")
{ 
    cont.clear();
    size_t size = 0;
    size_t begpos = 0;
    size_t endpos = 0;
    begpos = str.find_first_not_of(defstr);
    while (begpos != std::string::npos)
    {
        size++;
        endpos = str.find_first_of(defstr, begpos);

        if (endpos == std::string::npos)
        {
            endpos = str.size();
        }

        string ssubstr = str.substr(begpos, endpos - begpos);

        cont.push_back(ssubstr);
        begpos = str.find_first_not_of(defstr, endpos+1);
    }
}

int ReadDataFromFileLBLIntoString(vector<vector<int>> &DFG_node)
{
    ifstream fin(FLAGS_dfg_file); 
    cout << "********************************" << endl;
    cout << "the name of File:"  << FLAGS_dfg_file << endl;
    cout << "********************************" << endl;
    string s;
    int nodes = 0;
    vector<int> linedata;
    int line = 0;
    while(getline(fin,s))
    {
        vector<string> vec;
        strtok(s,vec,",");
        linedata.clear();
        for(int i = 0; i < vec.size(); i++)
        {
            
           //atoi:ascii to integer
        
           int e = atoi(vec[i].c_str());
            linedata.push_back(e);

        }
        DFG_node.push_back(linedata);
        line++;
    }
    
    fin.close();
    return line;
}



void show(DFG *D, int qmax[],int qsize,int II,int PERow, int PEcolumn){
    
    bool ispalce = false;
    for (int k = 0; k < II; k++){
        for(int z = 0; z < PEcolumn; z++){
            cout << "--------";
        }
        cout << "["<< k <<"]";
        for(int z = 0; z < PEcolumn; z++){
            cout << "--------" ;
        }
        cout<<endl;                          
        for (int i = 0; i < PERow; i++) {//行
            for (int j = 0; j < PEcolumn; j++) {//列
                for (int l = 0; l < qsize; l++){
                    int postion = qmax[l] % (PEcolumn * PERow);
                    int op_order = qmax[l] / (PEcolumn * PERow);//DFG算子的序列号,从0开始顺序编号
                    int op_label = D->DFGnodesList[op_order]->nodelabel;
                    int time_step = D->DFGnodesList[op_order]->nodelevel;
                    if (time_step == k && postion / PEcolumn == i && postion % PEcolumn == j){
                       
                        ispalce = true;
                        cout << "|\t  "<< op_label << "\t  ";
                    }
                }
                
                if (ispalce == false){
                    cout << "|\t\t  ";
                }
                ispalce = false;
            }
            cout << "|  " << endl;
        }
    }
     //全部画完,换行
    for(int z = 0; z < PEcolumn; z++){
            cout << "--------";
    }
    cout << "---";
    for(int z = 0; z < PEcolumn; z++){
        cout << "--------" ;
    }
    cout<<endl;  
    
} 

void  creatCGTable(DFG* D, TEC* T, int& size, bool**& conn, int PEnum)
{
    int** DnodeCnodePair;
    DnodeCnodePair = new int* [D->numDFGnodes* PEnum];
    for (int i = 0; i < D->numDFGnodes * PEnum; i++)
    {
        DnodeCnodePair[i] = new int[2];
        memset(DnodeCnodePair[i], 0, 2 * sizeof(int));
    }    

    /* 创建DnodeCnodePair */
    int count = 0;   /* 记录DnodeCnodePair中的 DFGNode-CGRANode 对数 */
    for (int i = 0; i < D->numDFGnodes; i++)
    {
        for (int j = 0; j < T->TECPENum; j++)
        { 
            
            if(D->DFGnodesList[i]->nodelevel == T->TECnodesList[j]->PEtimeslot)//只有DFG算子的层数和TECPE的层数一一对应才可以
            {
                if(D->DFGnodesList[i]->kind == 0)//普通算子
                {
                    if(T->TECnodesList[j]->PEkind == 0)//放在PE上
                    {
                        DnodeCnodePair[count][0] = D->DFGnodesList[i]->nodelabel;    
                        DnodeCnodePair[count][1] = T->TECnodesList[j]->PEorder; /*TECPE从0开始编号,也是唯一编号  */
                        
                        count ++;
                    }
                    else
                    {
                        DnodeCnodePair[count][0] = -1;    
                        DnodeCnodePair[count][1] = -1; 
                        count ++;
                    }
                }
                else if(D->DFGnodesList[i]->kind == 1)//存储算子
                {
                    if(T->TECnodesList[j]->PEkind == 1)//放在LSU上
                    {
                        DnodeCnodePair[count][0] = D->DFGnodesList[i]->nodelabel;    
                        DnodeCnodePair[count][1] = T->TECnodesList[j]->PEorder; /*TECPE从0开始编号,也是唯一编号  */
                        count ++;
                        
                    }
                    else
                    {
                        DnodeCnodePair[count][0] = -1;    
                        DnodeCnodePair[count][1] = -1; 
                        count ++;

                    }
                }
            }
        }
    }  
    // cout<<"兼容图顶点个数:"<<count<<endl;
    
    
    /* 创建兼容图的边 */

    /* 初始化兼容表,bool型 */
    bool** CGTable = new bool* [count];
    for (int i = 0; i < count; i++) 
    {
        CGTable[i] = new bool[count];
        memset(CGTable[i], false, count * sizeof(bool));
    }
   
    
    //创建兼容表，0，1
    for(int i = 0; i < count; i++)
    {			 
        for(int k = 0; k < i; k++)
        {     
            int i_dnode = DnodeCnodePair[i][0];  /* i的DFG算子编号 */
            int k_dnode = DnodeCnodePair[k][0];  /* k的DFG算子编号 */
            int i_pnode = DnodeCnodePair[i][1];  /* i的TECPE结点编号 */    
            int k_pnode = DnodeCnodePair[k][1];  /* k的TECPE结点编号 */ 
            
            //获得DFG算子的原始的时间步 
            int i_time = D->getNodeTime(i_dnode);
            int k_time = D->getNodeTime(k_dnode);
            

            // int *flag3;
            // *flag3 = 0;
            // if(i_time == -1 || k_time == -1 )
            // {
            //     * flag3 = 1;
            // }

            // CHECK_EQ(*flag3, 0) << "DFG编号失败";
            
            int dep_len = i_time - k_time;

            if (i_pnode != -1 && k_pnode != -1 && i_dnode != -1 && k_dnode != -1 )     /* DFG算子放在正确的TECPE上  */
            {
                //cout<<"i_dnode="<<i_dnode<<endl;
                //所代表的两个DFG算子之间有边，不包括在自己到自己的边
                if((D->DFGgraphHasEdge(i_dnode, k_dnode) || D->DFGgraphHasEdge(k_dnode, i_dnode) )&& (i_dnode != k_dnode))
                {
                    
                    //短依赖
                    if(dep_len==1 || dep_len==-1) 
                    {
                        //如果TEC有边
                        if(T->TECgraphHasEdge(i_pnode,k_pnode)|| T->TECgraphHasEdge(k_pnode,i_pnode))
                        {
                            CGTable[i][k] = true;
                            CGTable[k][i] = true; 
                        }
                        else
                        {
                            CGTable[i][k] = false;                                        
                            CGTable[k][i] = false;     
                        }
                    }

                    //长依赖
                   //if(dep_len > 1 || dep_len<-1)
                    else
                    {
                        
                        // cout<<"T->levelNum"<<T->levelNum<<endl;
                        //两个算子在同一层，不可以，因为长依赖源点和目标点要放在同一PE上。即使II=1
                        if(i_time % T->levelNum == k_time % T->levelNum)
                        {
                            CGTable[i][k] = false;                                        
                            CGTable[k][i] = false;  
                        }
                        else//两个算子不在同一层
                        {
                            // cout<<" "<<i_dnode<<" "<<i_pnode<<" "<<k_dnode<<" "<<k_pnode<<endl;
                            if(i_pnode % PEnum == k_pnode % PEnum)//源点和目标点在同一个PE上
                            {
                                CGTable[i][k] = true;
                                CGTable[k][i] = true; 
                            }
                            else
                            {
                                CGTable[i][k] = false;                                        
                                CGTable[k][i] = false;  
                            }
                        }
                    }
                }                    
                //DFG没有边
                else
                {
                    if(i_dnode != k_dnode && i_pnode != k_pnode)//算子不同，PE不同
                    { 
                        CGTable[i][k] = true;
                        CGTable[k][i] = true; 
                    }
                    else
                    {
                        CGTable[i][k] = false;                                        
                        CGTable[k][i] = false;    
                    }
                }
            }
            
        }
         
    }
    /* 初始化conn */
    set<int> v;          
    multimap<int, int> e; /* 存放兼容表CGTable边为1的 "DFGNode-TECNode" 边集合 */
    for (int i = 0; i < count; i++)
    {      
        for (int j = 0 ; j < i; j++)
        {
            //cout << ""<<DnodeCnodePair[i][0]<<" " <<DnodeCnodePair[i][1] <<"j="<<DnodeCnodePair[j][0]<<" " <<DnodeCnodePair[i][1] <<" "<<CGTable[i][j]<<endl;
            if (CGTable[i][j] == true)/* 在兼容表CGTable,两顶点有边的顶点和其边所构成的兼容图中寻找最大团 */
            {   
                //cout << ""<<DnodeCnodePair[i][0]<<" " <<DnodeCnodePair[i][1] <<"and"<<DnodeCnodePair[j][0]<<" " <<DnodeCnodePair[j][1] <<" "<<CGTable[i][j]<<endl;              
                v.insert(i);
                v.insert(j);
                e.insert(make_pair(i, j)); /* 边中，顶点编号大的会在前面 */                                                                             
            }
        }    
    } 
    for (size_t i = 0; i < D->numDFGnodes* PEnum; i++)
        delete[] DnodeCnodePair[i];
    delete[] DnodeCnodePair;

   
    size = *v.rbegin() + 1;  
      
    conn = new bool* [size];
    for (size_t i = 0; i < size; i++) 
    {
        conn[i] = new bool[size];
        memset(conn[i], 0, size * sizeof(bool));
    }
    /* conn存放顶点与顶点之间的连接关系，双向 */
    for (multimap<int, int>::iterator it = e.begin(); it != e.end(); it++)
    {
        conn[it->first][it->second] = true;
        conn[it->second][it->first] = true;
    }

    /* 输出兼容图的边集合和顶点集合的大小，以及兼容图的密度 */
    cout << "|E| = " << e.size() << "  |V| = " << v.size() << " p = " << (double) e.size() / (v.size() * (v.size() - 1) / 2) << endl;  

    /* 释放CGTable兼容表空间 */
    for (int i = 0; i < count; i++)
        delete[] CGTable[i];
    delete[] CGTable;
}
