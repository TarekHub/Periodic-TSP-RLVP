#include "INCLUDE/MOW-P.h"

// DeepCopy
void DeepCopy(Node * CibleSolution, Node * SourceSolution)
{
  	int j = 0;
    for (j = 0; j < (TimeHorizon * (Dimension + 1)); j++) {

      //Values
      CibleSolution[j].Id = SourceSolution[j].Id;
      CibleSolution[j].IsVisited = SourceSolution[j].IsVisited;
      CibleSolution[j].Delay = SourceSolution[j].Delay;
      CibleSolution[j].Day = SourceSolution[j].Day;
      CibleSolution[j].InitDelay = SourceSolution[j].InitDelay;

      //Constant Pointers
      CibleSolution[j].FillingPeriod = SourceSolution[j].FillingPeriod;
      CibleSolution[j].C = SourceSolution[j].C;

      // Pointers
      if (SourceSolution[j].PrevVisit == NULL)
        CibleSolution[j].PrevVisit = NULL;
      else
        CibleSolution[j].PrevVisit = &CibleSolution[(SourceSolution[j].PrevVisit->Day - 1) * (Dimension + 1) + SourceSolution[j].PrevVisit->Id];

      if (SourceSolution[j].NextVisit == NULL)
        CibleSolution[j].NextVisit = NULL;
      else
        CibleSolution[j].NextVisit = &CibleSolution[(SourceSolution[j].NextVisit->Day - 1) * (Dimension + 1) + SourceSolution[j].NextVisit->Id];

      if(SourceSolution[j].Pred == NULL)
        CibleSolution[j].Pred = NULL;
      else
        CibleSolution[j].Pred = &CibleSolution[(SourceSolution[j].Pred->Day - 1) * (Dimension + 1) + SourceSolution[j].Pred->Id];

      if(SourceSolution[j].Suc == NULL)
        CibleSolution[j].Pred = NULL;
      else
        CibleSolution[j].Suc = &CibleSolution[(SourceSolution[j].Suc->Day - 1) * (Dimension + 1) + SourceSolution[j].Suc->Id];
    }
}