#ifndef CGRA_H
#define CGRA_H
#include "config.h"

using namespace std;

class CGRANode{      
public:   
    int PElabel;    
    int PEkind;/* PE的种类,0代表计算单元PE，1代表存储单元LSU */
    vector<int> PENeighbors;
};

class CGRA{
public:
    CGRA(int PERow, int PEColumn);
	int PENum;/* PE的总数 */
    vector<CGRANode*> CGRAnodesList;
    ~CGRA();
};
#endif