#include "INCLUDE/MOW-P.h"

// SW Strategie
void SW_Strategy(){
	EntryTime = GetTime();

    while(GetTime() - EntryTime <= MaxTime){

        BestLambdaQuality = DBL_MAX;
        AtLeastOneFeasibleNeighbor = 0;
        double tempQuality = GetCurrentQuality();

        for(int l = 0 ; l < Lambda ; l++){
            LogCurrentQuality = tempQuality;
            BitFlip();
        }

        if(AtLeastOneFeasibleNeighbor){
            if(BestLambdaQuality < BestQuality){

                BestQuality = BestLambdaQuality;
                for(int h = 0 ; h < TimeHorizon ; h++){
                    BestToursCost[h] = BestLambdaToursCost[h];
                    BestToursDimension[h] = BestLambdaToursDimension[h];
                }
                DeepCopy(BestSolution, BestLambda);

                if(BestQuality < Optimum){
                    newOptimum = BestQuality;
                }

                printff("* Gap = %0.3lf %, Cost = %0.3lf,  Time = %0.3f sec.\n",
                    (100.0 * (BestQuality - Optimum) / Optimum), BestQuality, StartTime + (GetTime() - LastTime));
            }

            LogCurrentQuality = BestLambdaQuality;
            for(int h = 0 ; h < TimeHorizon ; h++){
                CurrentToursCost[h] = BestLambdaToursCost[h];
                CurrentToursDimension[h] = BestLambdaToursDimension[h];
            }
            DeepCopy(NodeSet, BestLambda);
        }
    }
}