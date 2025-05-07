#include "INCLUDE/MOW-P.h"

Node *NodeSet, *BestLambda, *BestSolution;
Node **LambdaNeighbors;

Node *FirstNode;

int *CostMatrix, *FillingPeriods;

int DimensionSaved;
int Dimension;
int Dim;
int TotalNodes;

double Precision, Optimum, LastTime, EntryTime, StartTime;

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

double *LambdaNeighborsQuality;
int **LambdaNeighborsToursCost;
int **LambdaNeighborsToursDimension;

FILE *ParameterFile, *ProblemFile, *InitToursFile, *OutputToursFile;

int newOptimum, muSize, Run, Lambda, MaxIterations, MaxTime, TimeHorizon,
Iteration, MaxDailyDuration, Loading;

int AtLeastOneFeasibleNeighbor, Strategy;

CostFunction Distance, D;

//const int FOUR_MINUTES = 240;
static void AllocateID();

int main(int argc, char * argv[]){
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

    if (Strategy == SW)
		SW_Strategy();
    else if (Strategy == GREEDY)
        Greedy_Strategy();
    else{
		AllocateID();
		ID_Strategy();
	}

    //printff("\n#LAMBDA = %d\n", Lambda);
    //printff("Best Gap = %0.3lf %, Cost = %0.3lf\n", (100.0 * (BestQuality - Optimum) / Optimum), BestQuality);
    //printff("Time = %0.3f sec. \n", (GetTime() - LastTime));
    if(newOptimum)
      printff("New Optimum : %lf\n", BestQuality);

    //WriteInitTours(InitToursFileName, BestQuality);
    WriteSolution(OutputToursFileName, BestQuality);
}

// For ID (BEST and ANY)
static void AllocateID(){
	LambdaNeighbors = (Node **) malloc(Lambda * sizeof(Node *));
	LambdaNeighborsToursCost = (int **) malloc(Lambda * sizeof(int *));
	LambdaNeighborsToursDimension = (int **) malloc(Lambda * sizeof(int *));

	LambdaNeighborsQuality = (double *) malloc(Lambda * sizeof(double *));

	for (int i = 0; i < Lambda; i++) {
    	LambdaNeighbors[i] = (Node *) calloc(TimeHorizon * (Dimension + 1), sizeof(Node));
		LambdaNeighborsToursCost[i] = (int *) calloc(TimeHorizon, sizeof(int));
		LambdaNeighborsToursDimension[i] = (int *) calloc(TimeHorizon, sizeof(int));
	}
}
