#include "INCLUDE/MOW-P.h"

#define Free(s) { free(s); s = 0; }

void Allocate()
{
    Free(CurrentToursCost);
    Free(CurrentToursDimension);
    Free(BestLambdaToursCost);
    Free(BestLambdaToursDimension);
    Free(BestToursCost);
    Free(BestToursDimension);

    Free(BestLambda);
    Free(BestSolution);

    CurrentToursCost = (int *) calloc(TimeHorizon, sizeof(int));
    CurrentToursDimension = (int *) calloc(TimeHorizon, sizeof(int));

    BestLambdaToursCost = (int *) calloc(TimeHorizon, sizeof(int));
    BestLambdaToursDimension = (int *) calloc(TimeHorizon, sizeof(int));

    BestToursCost = (int *) calloc(TimeHorizon, sizeof(int));
    BestToursDimension = (int *) calloc(TimeHorizon, sizeof(int));
}