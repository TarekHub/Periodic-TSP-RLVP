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

	BestQuality = LogCurrentQuality;
	for(int h = 0 ; h < TimeHorizon ; h++){
		BestToursCost[h] = CurrentToursCost[h];
		BestToursDimension[h] = CurrentToursDimension[h];
	}
	DeepCopy(BestSolution, NodeSet);

}

int Feasible(Node *N, int chosenDay, Node **NPred, Node **NSuc, int *impact)
{
	Node *N1, *N2, *FirstN;
	int cost, minCost = INT_MAX;

	FirstN = &NodeSet[chosenDay * (Dimension + 1) + 1];
	N1 = FirstN;

	// Trouver l'arête qui fait le moins de dégâts possible
	do {
		N2 = N1->Suc;  // N2 est le successeur de N1

		// (Coût de N1->N + Coût de N->N2) - Coût de N1->N2
		cost = (N1->C[N->Id] + N->C[N2->Id]) - N1->C[N2->Id];

		if (cost < minCost) {
			minCost = cost;
			*NPred = N1;
			*NSuc = N2;
		}

		N1 = N1->Suc;  // Passer au nœud suivant
	} while (N1 != FirstN);  // Jusqu'à revenir au premier nœud

	// Calcul d'impact correct en utilisant les nœuds stockés dans NPred et NSuc
	*impact = (*NPred)->C[N->Id] + N->C[(*NSuc)->Id] - (*NPred)->C[(*NSuc)->Id];

	// Vérification de la faisabilité
	if (!N->IsVisited &&
		((CurrentToursDimension[chosenDay] - 1 + 1) * Loading) +
		CurrentToursCost[chosenDay] + (*impact) < MaxDailyDuration)
		return 1;

	return 0;
}