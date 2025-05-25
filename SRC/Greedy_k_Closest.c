#include "INCLUDE/MOW-P.h"

static int Feasible(Node * N, int chosenDay, Node **NPred, Node **NSuc, int * impact);

// Greedy Strategie
void Greedy_k_Closest(){
    EntryTime = GetTime();
	Node * N, *NPred = 0, *NSucc = 0, *NChosen = 0, *NChosen_Pred = 0, *NChosen_Succ = 0;

	int impact;

	for(int day = 0; day < TimeHorizon; day++){
		int offset = day * (Dimension + 1);
		do{
			NChosen = NChosen_Pred = NChosen_Succ = 0;
			impact = 0;

			typedef struct {
				Node *node;
				Node *pred;
				Node *succ;
				int impact; // Impact pour trier
			} NodeInfo;

			NodeInfo candidateNodes[Dimension];
			int candidateCount = 0;

			// Identifier tous les nœuds faisables et calculer leur impact.
			for(int i = 2; i <= Dimension; i++) {
				N = &NodeSet[offset + i];
				if(Feasible(N, day, &NPred, &NSucc, &impact)) {
					candidateNodes[candidateCount].node = N;
					candidateNodes[candidateCount].pred = NPred;
					candidateNodes[candidateCount].succ = NSucc;
					candidateNodes[candidateCount].impact = impact;
					candidateCount++;
				}
			}

			// Trier les candidats par impact croissant
			for(int i = 0; i < candidateCount; i++) {
				for(int j = i+1; j < candidateCount; j++) {
					if(candidateNodes[i].impact > candidateNodes[j].impact) {
						NodeInfo temp = candidateNodes[i];
						candidateNodes[i] = candidateNodes[j];
						candidateNodes[j] = temp;
					}
				}
			}

			// Prendre au maximum K nœuds les plus proches
			int actualK = (candidateCount < K) ? candidateCount : K;

			// Parmi les K plus proches, choisir le plus urgent
			int maxDelay = INT_MIN;
			for(int i = 0; i < actualK; i++) {
				if(candidateNodes[i].node->Delay > maxDelay) {
					maxDelay = candidateNodes[i].node->Delay;
					NChosen = candidateNodes[i].node;
					NChosen_Pred = candidateNodes[i].pred;
					NChosen_Succ = candidateNodes[i].succ;
					impact = candidateNodes[i].impact;
				}
			}

			if(NChosen != 0) {
				AddNodeKnown(NChosen, day, NChosen_Pred, NChosen_Succ);
				printff("* Gap = %0.3lf %%, Cost = %0.3lf, Time = %0.3f sec.\n",
					  (100.0 * (BestQuality - Optimum) / Optimum), LogCurrentQuality, StartTime + (GetTime() - LastTime));
			}

		}while(NChosen != 0);
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
			*NPred = N1;  // Stocke directement dans NPred
			*NSuc = N2;   // Stocke directement dans NSuc
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