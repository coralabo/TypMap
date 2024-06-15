#include "DFG.h"
#include "TEC.h"

void show(DFG *D, int qmax[],int qsize,int II,int PERow, int PEcolumn);
void  creatCGTable(DFG* D, TEC* T, int& size, bool**& conn,int PEnum);
int ReadDataFromFileLBLIntoString(vector<vector<int>> &DFG_node);
