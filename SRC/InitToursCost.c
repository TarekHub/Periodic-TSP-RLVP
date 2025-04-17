#include "INCLUDE/MOW-P.h"

void InitToursCost()
{
    Node * FirstNode = &NodeSet[1];
	Node * N;

    for(int h = 0; h < TimeHorizon; h++){
      int offset = h * (Dimension + 1);
      N = &NodeSet[offset + 1];

      CurrentToursCost[h] = 0;
      int i =0;
      while (N->Suc->Id != FirstNode->Id){
            CurrentToursCost[h] += N->C[(N->Suc)->Id];
            N = N->Suc;
            i++;
      };
      //printff("%d\n", i+1);
      CurrentToursCost[h] += N->C[FirstNode->Id];
    }
}