#include "DFG.h"
#include "CGRA.h"
#include "TEC.h"
#include "mcqd.h"
#include "config.h"
#include "tool.h"

DEFINE_string(dfg_file, "data/test.txt", "dfg file.");
DEFINE_int32(II, 3, "initiation interval");
DEFINE_int32(pea_column, 3, "CGRA columns");//列数，包含LSU
DEFINE_int32(pea_row, 2, "CGRA rows");//行数

using namespace std;

int main(int argc, char** argv)
{
    #ifdef EXE
    google::ParseCommandLineFlags(&argc, &argv, true);
    #endif
    LOG(INFO) << "GLOG TEST!";
    
    int level_flag;		
    int *flag2;				 
    clock_t start, end;
    
    flag2 = & level_flag;
    
    
    timeval tv, tv1;    
    gettimeofday(&tv, 0);
 
    /* 创建DFG */
    DFG *D = new DFG();
   /* 创建CGRA */
    CGRA *C = new CGRA(FLAGS_pea_row,FLAGS_pea_column);
    cout << "the number of DFG's nodes:" << D->numDFGnodes << endl;
    cout << "II:" << FLAGS_II << endl;
    /* 资源约束判断 */
    //level_flag = D->Constraint_Level(C->PENum,FLAGS_II);//To do:可能会数组越界，因为未折叠前不止ii层
    //CHECK_EQ(level_flag, 0) << "Original resource constraints are not satisffied!";
    /* DFG折叠 */
    D->CreatMDFG(FLAGS_II);
      
    /* DFG折叠后是否满足 每一层的DFG数量不超过PENum */
    /* to do :资源判断要LSU和load,PE和普通算子匹配 */
    *flag2 = D->Constraint_Level(C->PENum,FLAGS_II);
    CHECK_EQ(*flag2, 0) << "Modulo resource constraints are not satisffied!";

    /* 创建TEC */
    TEC *T = new TEC(C,FLAGS_II);

    /* 创建兼容表 */
    /* 初始化DnodeCnodePair，DnodeCnodePair用来存放 DFGNode-TECPE 对 */  
    /* DnodeCnodePair共有 DFG结点总数*TECPENode结点总数，两列，一列存DFG结点编号，一列存TECPE结点编号 */ 
    bool **conn;
    int size;   
    int PEnum = C->PENum ;
    creatCGTable(D, T, size,conn,PEnum);
    

    /* MCQ方法 */
    int *qmax;
    int qsize;
    int smax = D->numDFGnodes;
    //cout << "---------- Example 1: run max clique with improved coloring ----------------"<<endl;
    clock_t start1 = time(NULL);
    clock_t start2 = clock();
    Maxclique m(conn, size);   
    m.mcq(qmax, qsize,smax);  // run max clique with improved coloring
    
    //cout << "-----Example 2: run max clique with improved coloring and dynamic sorting of vertices -----------"<<endl;
    //clock_t start1 = time(NULL);
    //clock_t start2 = clock();
    //Maxclique md(conn, size, 0.025);  //(3rd parameter is optional - default is 0.025
    //md.mcqdyn(qmax, qsize, smax); // run max clique with improved coloring and dynamic sorting of vertices    
   
    cout << "Maximum clique: ";
    for (int i = 0; i < qsize; i++)
      cout << qmax[i] << " ";
    cout << endl;
    cout << "Size = " << qsize << endl;
    cout << "Number of steps = " << m.steps() << endl;
    cout << "Time = " << difftime(time(NULL), start1) << endl;
    cout << "MCQ's Time (precise) = " << ((double) (clock() - start2)) / CLOCKS_PER_SEC << endl << endl;

    show(D,qmax,qsize,FLAGS_II,FLAGS_pea_row,FLAGS_pea_column);



   
    for (int i = 0;i < size; i++)
        delete [] conn[i];
    delete [] conn;
    gettimeofday(&tv1, 0); 
    cout<<"--------------------------------------------"<<endl;
    cout<< "spand time:" <<(tv1.tv_sec - tv.tv_sec + (double)(tv1.tv_usec - tv.tv_usec) / CLOCKS_PER_SEC)<<endl;      
  
    if (D->numDFGnodes > qsize ){
        cout << "can't find a solution" << endl;
        cout << "the size of solution:" << qsize << endl;       
        cout << "the number of DFG'nodes:" << D->numDFGnodes << endl;
        vector<int> nodes;
        
        for(int m = 0; m < qsize; m++)
        {
            // 存放的唯一ID号
            int op_order = qmax[m] / (FLAGS_pea_row * FLAGS_pea_column);

            nodes.push_back(op_order);

            
        }

        for(int i = 0; i < D->numDFGnodes; i++)
        {
            
            
            // int op_order = D->DFGnodesList[i]->nodelabel;
            if (!count(nodes.begin(), nodes.end(), i))
            {
                cout << "Element  not found:   " << D->DFGnodesList[i]->nodelabel<<endl;
            }
        }


        return -1;
    }           
      
   
   delete [] qmax;
   delete C;
   delete T;
   delete D;
   return 0;
}

