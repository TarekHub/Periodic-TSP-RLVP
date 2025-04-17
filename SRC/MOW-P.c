#include "INCLUDE/MOW-P.h"

Node *NodeSet, *BestLambda, *BestSolution;
Node *FirstNode;

int *CostMatrix, *FillingPeriods;

int DimensionSaved;
int Dimension;
int Dim;
int TotalNodes;

double Precision, Optimum;

char *ParameterFileName, *ProblemFileName, *InitToursFileName, *OutputToursFileName;
char *LastLine;

double **mu;

int BestLambdaDay;

double CurrentQuality;
double LogCurrentQuality;
int *CurrentToursCost;
int *CurrentToursDimension;

double BestLambdaQuality;
int *BestLambdaToursCost;
int *BestLambdaToursDimension;

double BestQuality;
int *BestToursCost;
int *BestToursDimension;

FILE *ParameterFile, *ProblemFile, *InitToursFile, *OutputToursFile;

int newOptimum, muSize, Run, Runs, Lambda, MaxIterations, TimeHorizon, Iteration, MaxDailyDuration, FourMinutes;
int AtLeastOneFeasibleNeighbor;

CostFunction Distance, D;

//const int FOUR_MINUTES = 240;

int main(int argc, char * argv[]){
    double LastTime;
    newOptimum = 0;

    if (argc >= 2)
        ParameterFileName = argv[1];


    ReadParameters();
    Allocate();
    ReadProblem();
    ReadInitTours();

    Init();

    srand(time(NULL));
    LastTime = GetTime();

    while(Iteration < MaxIterations){
      BestLambdaQuality = DBL_MAX;
      AtLeastOneFeasibleNeighbor = 0;
      double tempQuality = GetCurrentQuality();

      for(int l = 0 ; l < Lambda ; l++){
        LogCurrentQuality = tempQuality;
        BitFlip();
      }

      Iteration += Lambda;

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
    }

    printff("\n#LAMBDA = %d\n", Lambda);
    printff("Best Gap = %0.3lf %, Cost = %0.3lf\n", (100.0 * (BestQuality - Optimum) / Optimum), BestQuality);
    printff("Time = %0.3f sec. \n", GetTime() - LastTime);
    if(newOptimum)
      printff("New Optimum : %lf\n", BestQuality);

    //WriteInitTours(InitToursFileName, BestQuality);
    WriteSolution(OutputToursFileName, BestQuality);
}
