#include "INCLUDE/MOW-P.h"

// SW Strategie
void ID_Strategy(){
	int lambdaCandidates , Accepted;
	double tempQuality;
	int i;

	while(Iteration < MaxIterations){
        BestLambdaQuality = DBL_MAX;
        tempQuality = GetCurrentQuality();

		AtLeastOneFeasibleNeighbor = lambdaCandidates = Accepted = 0;
		i = 0;

        while(lambdaCandidates < Lambda && Accepted == 0){
            LogCurrentQuality = tempQuality;
            BitFlip();

            if(AtLeastOneFeasibleNeighbor){
				if(BestLambdaQuality < LogCurrentQuality)
					Accepted = 1;
				else{
					// Save Neighbor in rejected neighbors
					LambdaNeighborsQuality[i] = BestLambdaQuality;

					for(int h = 0 ; h < TimeHorizon ; h++){
                		LambdaNeighborsToursCost[i][h] = BestLambdaToursCost[h];
                		LambdaNeighborsToursDimension[i][h] = BestLambdaToursDimension[h];
            		}

					DeepCopy(LambdaNeighbors[i], BestLambda);
					i++;
				}

				AtLeastOneFeasibleNeighbor = 0;
            }

			lambdaCandidates++;
        }

		if(Accepted){
            LogCurrentQuality = BestLambdaQuality;
            for(int h = 0 ; h < TimeHorizon ; h++){
                CurrentToursCost[h] = BestLambdaToursCost[h];
                CurrentToursDimension[h] = BestLambdaToursDimension[h];
            }
            DeepCopy(NodeSet, BestLambda);
		}

		// ANY
		else if(Strategy == ID_ANY){
			int AnyIndex = rand() % i;

            LogCurrentQuality = LambdaNeighborsQuality[AnyIndex];
            for(int h = 0 ; h < TimeHorizon ; h++){
                CurrentToursCost[h] = LambdaNeighborsToursCost[AnyIndex][h];
                CurrentToursDimension[h] = LambdaNeighborsToursDimension[AnyIndex][h];
            }
			DeepCopy(NodeSet, LambdaNeighbors[AnyIndex]);

		// BEST
		}else{
			int BestIndex = 0;
			double MinQuality = DBL_MAX;

			// Trouver le voisin avec la qualité minimale
			for(int j = 0; j < i; j++){
              if(LambdaNeighborsQuality[j] < MinQuality){
                  MinQuality = LambdaNeighborsQuality[j];
                  BestIndex = j;
              }
			}

          	// Appliquer le meilleur voisin
          	LogCurrentQuality = LambdaNeighborsQuality[BestIndex];
          	for(int h = 0; h < TimeHorizon; h++){
				CurrentToursCost[h] = LambdaNeighborsToursCost[BestIndex][h];
				CurrentToursDimension[h] = LambdaNeighborsToursDimension[BestIndex][h];
			}

			DeepCopy(NodeSet, LambdaNeighbors[BestIndex]);
		}

		// Best Solution
		if(LogCurrentQuality < BestQuality){
			BestQuality = LogCurrentQuality;
			for(int h = 0 ; h < TimeHorizon ; h++){
				BestToursCost[h] = CurrentToursCost[h];
				BestToursDimension[h] = CurrentToursDimension[h];
			}
			DeepCopy(BestSolution, NodeSet);
            printff("* Gap = %0.3lf %, Cost = %0.3lf,  Time = %0.3f sec.\n", (100.0 * (BestQuality - Optimum) / Optimum), BestQuality, GetTime() - LastTime);
		}

        Iteration += lambdaCandidates;
    }
}

/*
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

                //int calculatedDimension = GetDimension(BestSolution, BestLambdaDay);
                //assert(calculatedDimension == BestToursDimension[BestLambdaDay]);
                printff("* Gap = %0.3lf %, Cost = %0.3lf,  Time = %0.3f sec.\n", (100.0 * (BestQuality - Optimum) / Optimum), BestQuality, GetTime() - LastTime);
            }

            LogCurrentQuality = BestLambdaQuality;
            for(int h = 0 ; h < TimeHorizon ; h++){
                CurrentToursCost[h] = BestLambdaToursCost[h];
                CurrentToursDimension[h] = BestLambdaToursDimension[h];
            }
            DeepCopy(NodeSet, BestLambda);
            //printff("Gap = %0.3lf %, Cost = %0.3lf,  Time = %0.3f sec.\n", (100.0 * (LogCurrentQuality - Optimum) / Optimum), LogCurrentQuality, GetTime() - LastTime);
            // int calculatedDimension = GetDimension(NodeSet, BestLambdaDay);
            // assert(calculatedDimension == CurrentToursDimension[BestLambdaDay]);
        }
*/
