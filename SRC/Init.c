#include "INCLUDE/MOW-P.h"

void Init()
{
    // Init Quality
    BestQuality = LogCurrentQuality = GetCurrentQuality();

    // Init Dimension and Cost lists
    InitToursCost();
    for(int h = 0 ; h < TimeHorizon ; h++){
        BestLambdaToursCost[h] = BestToursCost[h] = CurrentToursCost[h];
        BestLambdaToursDimension[h] = BestToursDimension[h] = CurrentToursDimension[h];
    }

    BestLambda = (Node *) calloc(TimeHorizon * (Dimension + 1), sizeof(Node));
    BestSolution = (Node *) calloc(TimeHorizon * (Dimension + 1), sizeof(Node));

    // Set BestLambda & BestSolution
    DeepCopy(BestLambda, NodeSet);
    DeepCopy(BestSolution, NodeSet);
}