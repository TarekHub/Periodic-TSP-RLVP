#include "INCLUDE/MOW-P.h"

static int Feasible(Node * N, int chosenDay, Node **NPred, Node **NSuc, int * impact);

// Greedy Strategie
void Greedy_Strategy(){
    EntryTime = GetTime();
	Node * N, *NPred = 0, *NSucc = 0, *NUrg = 0, *NUrg_Pred = 0, * NUrg_Succ = 0;
	int impact;
	for(int day = 0; day < TimeHorizon; day++){
		int offset = day * (Dimension + 1);
		do{
			int MaxDelay = INT_MIN, MinImpact = INT_MAX;
			NUrg = NUrg_Pred = NUrg_Succ = 0;
			impact = 0;

			for(int i = 2; i <= Dimension; i++){
				N = &NodeSet[offset + i];
				if(Feasible(N, day, &NPred, &NSucc, &impact)){
					if(MaxDelay < N->Delay || (MaxDelay == N->Delay && MinImpact > impact)){
						MaxDelay = N->Delay;
						MinImpact = impact;
						NUrg = N;
						NUrg_Pred = NPred;
						NUrg_Succ = NSucc;
					}
				}
			}

			if (NUrg != 0){
				AddNodeKnown(NUrg, day, NUrg_Pred, NUrg_Succ);
				printff("* Gap = %0.3lf %, Cost = %0.3lf,  Time = %0.3f sec.\n",
                (100.0 * (BestQuality - Optimum) / Optimum), LogCurrentQuality, StartTime + (GetTime() - LastTime));
			}
		}while(NUrg != 0);
	}
}

int Feasible(Node * N, int chosenDay, Node **NPred, Node **NSuc, int * impact)
{
    // Tour Cost
    Node *N1, *N2, *FirstN, *i, *j;
    int cost, minCost = INT_MAX;

    N1 = FirstN = &NodeSet[chosenDay * (Dimension + 1) + 1];
    N2 = N1->Suc;

    // Trouver l'arête qui fait le moins de dégâts possible
    do {
        cost = (N1->C[N->Id] + N->C[N2->Id]) - N1->C[N2->Id];
        if (cost < minCost) {
            minCost = cost;
            i = N1;
            j = N2;
        }
    } while (((N1 = N1->Suc) != FirstN) && (N2 = N2->Suc));

    *NPred = i;
    *NSuc = j;

    *impact = i->C[N->Id] + N->C[j->Id] - i->C[j->Id];

    if  (!N->IsVisited &&
		((CurrentToursDimension[chosenDay] - 1 + 1) * Loading)
			+ CurrentToursCost[chosenDay] + (* impact) < MaxDailyDuration)
        return 1;
    return 0;
}