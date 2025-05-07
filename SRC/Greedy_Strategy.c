#include "INCLUDE/MOW-P.h"

static int Feasible(Node * N, int chosenDay, Node **NPred, Node **NSuc);

// Greedy Strategie
void Greedy_Strategy(){
    EntryTime = GetTime();

    while(GetTime() - EntryTime <= MaxTime){
        int offset, chosenDay;
        Node *N, *N_Urg = 0, *NPred = 0, *NSuc = 0, *NPred_Urg = 0, * NSuc_Urg = 0;
        int MaxQuality = 0;

        for(int h = 0; h < TimeHorizon; h++){
            offset = h * (Dimension + 1);
            for(int i = 2; i <= Dimension; i++){
                N = &NodeSet[offset + i];
                double currentQUality = LogCurrentQuality;
                int feasible = Feasible(N, h, &NPred, &NSuc) && !N->IsVisited;

                if (feasible){
                    AddNodeKnown(N, h, NPred, NSuc);

                    if(currentQUality - LogCurrentQuality >= MaxQuality){
                        MaxQuality = currentQUality - LogCurrentQuality;
                        N_Urg = N;
                        NPred_Urg = NPred;
                        NSuc_Urg = NSuc;
                        chosenDay = h;
                    }

                    RemoveNode(N, h, &NPred, &NSuc);
                }
            }
        }

        // BitFlip
        if(N_Urg != 0){
            AddNodeKnown(N_Urg, chosenDay, NPred_Urg, NSuc_Urg);

            printff("* Gap = %0.3lf %, Cost = %0.3lf,  Time = %0.3f sec.\n",
                (100.0 * (BestQuality - Optimum) / Optimum), LogCurrentQuality, StartTime + (GetTime() - LastTime));
        }else{
            break;
        }
    }
}

int Feasible(Node * N, int chosenDay, Node **NPred, Node **NSuc)
{
    // Tour Cost
    Node *N1, *N2, *FirstN, *i, *j;
    int diff, cost, minCost = INT_MAX;

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

    diff = i->C[N->Id] + N->C[j->Id] - i->C[j->Id];

    if( ((CurrentToursDimension[chosenDay] - 1 + 1) * Loading) + CurrentToursCost[chosenDay] + diff > MaxDailyDuration)
        return 0;
    return 1;
}