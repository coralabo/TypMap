#include "CGRA.h"

CGRA::~CGRA()
{
	for (auto node : CGRAnodesList)
	{
		delete node;
	}
}


CGRA::CGRA(int PERow, int PEColumn)
{   
    PENum = PERow * PEColumn;//PEA������������LSU��Ԫ	
    cout << PERow <<PEColumn <<endl;
    int i, j, k;           
    for( j = 0; j < PERow; j++)//PE的行数
	{
      	for(i = 0; i < PEColumn;i++)
		{	
			 		
           	CGRANode* node = new CGRANode();
			int PeCurrent = i + j * PEColumn;
			node->PElabel = PeCurrent;/* PElabel */
			int PeN = i + ((j - 1 + PERow) % PERow ) * PEColumn;
			int PeS = i +((j + 1) % PERow) * PEColumn;
			int PeW =((i - 1) + PEColumn) % PEColumn + j * PEColumn;
      		int PeE =(i + 1) % PEColumn + j * PEColumn;

			

			if(i == 0 ) //LSU第一列
			{			
				node->PEkind = 1;
				for(int m = 0;m < PEColumn -1;m++)
				{
					node->PENeighbors.push_back(PeCurrent  + 1+m); 
				}
				 
			}
			else//PE
			{  
				node->PEkind = 0;
				if( j == 0 || j == PERow -1)//第一行和最后一行
				{
					if(i == 1)//PE的第一列
					{
						node->PENeighbors.push_back(PeE); 
						
					}
					else if(i == PEColumn-1)//PE的最后一列
					{
						node->PENeighbors.push_back(PeW); 
					}
					else //PE的中间列
					{
						node->PENeighbors.push_back(PeW);
						node->PENeighbors.push_back(PeE); 
					}
					if(j == 0)//PE的第一行
					{
						node->PENeighbors.push_back(PeS); 
					}
					
					if(j == PERow -1)//PE的最后一行
					{
						node->PENeighbors.push_back(PeN); 
					}

				}
				else//中间行
				{
					node->PENeighbors.push_back(PeN); 
					node->PENeighbors.push_back(PeS); 
					if(i == 1)//PE的第一列
					{
						node->PENeighbors.push_back(PeE); 
						
					}
					else if(i == PEColumn-1)//PE的最后一列
					{
						node->PENeighbors.push_back(PeW); 
					}
					else //PE的中间列
					{
						node->PENeighbors.push_back(PeW);
						node->PENeighbors.push_back(PeE); 
					}
					
				}
				node->PENeighbors.push_back(j*PEColumn); 
				
				
			} 
			CGRAnodesList.push_back(node);  
		}
	}
	// for(int  i = 0; i < CGRAnodesList.size(); i++)
	// {
	// 	cout<<"PElabel:"<<CGRAnodesList[i]->PElabel<<endl;
	// 	cout<<"neighbor:";
	// 	for(int j = 0; j < CGRAnodesList[i]->PENeighbors.size(); j ++ )
	// 	{
	// 		cout<<CGRAnodesList[i]->PENeighbors[j]<<" ";
	// 	}
	// 	cout<<endl;
	// }
	
}

