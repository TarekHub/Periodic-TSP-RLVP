#ifndef MOW_P_H
#define MOW_P_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <float.h>

#define Link(a, b) { ((a)->Suc = (b))->Pred = (a); }
#define Follow(b, a)\
    { Link((b)->Pred, (b)->Suc); Link(b, b); Link(b, (a)->Suc); Link(a, b); }
#define Fixed(a, b) ((a)->FixedTo1 == (b))

#define LinkVisit(a, b) { ((a)->NextVisit = (b))->PrevVisit = (a); }

#define Delete(a) { Link(a->Pred, a->Suc); a->Suc = NULL; a->Pred = NULL; a->IsVisited = 0;}
#define Add(a, b, c) { Link(a, b); Link(b, c); b->IsVisited = 1;}
#define PRANDMAX INT_MAX

/* Node structure */
typedef struct Node Node;
struct Node {
  int Id;
  int *C;

  Node *PrevVisit;
  Node *NextVisit;
  int IsVisited;
  int Delay;

  Node *Pred, *Suc;
  Node *FixedTo1;

  int Day;
  int *FillingPeriod;
  int InitDelay;
};

typedef int (*CostFunction) (Node * Na, Node * Nb);

extern Node *NodeSet, *BestLambda, *BestSolution;
extern Node *FirstNode;
extern int *CostMatrix, *FillingPeriods;

extern int BestLambdaDay;

extern double CurrentQuality;
extern double LogCurrentQuality;
extern int *CurrentToursCost;
extern int *CurrentToursDimension;

extern double BestLambdaQuality;
extern int *BestLambdaToursCost;
extern int *BestLambdaToursDimension;

extern double BestQuality;
extern int *BestToursCost;
extern int *BestToursDimension;

extern int Dimension;
extern int DimensionSaved;
extern int Dim;
extern int TotalNodes;

extern char *ParameterFileName, *ProblemFileName, *InitToursFileName, *OutputToursFileName;
extern char *LastLine;
extern int newOptimum, muSize, Run, Runs, Lambda, MaxIterations, TimeHorizon, Iteration, MaxDailyDuration, FourMinutes;

extern double Precision, Optimum;

extern double **mu;

extern FILE *ParameterFile, *ProblemFile, *InitToursFile, *OutputToursFile;

extern CostFunction Distance, D;
extern const int FOUR_MINUTES;

extern int AtLeastOneFeasibleNeighbor;

void Init();
void Allocate();
void ReadParameters(void);
void ReadProblem(void);
void ReadInitTours(void);
char *ReadLine(FILE * InputFile);
void eprintf(const char *fmt, ...);
void printff(const char * fmt, ...);
int fscanint(FILE *f, int *v);

int D_FUNCTION(Node * Na, Node * Nb);
int Distance_ATSP(Node * Na, Node * Nb);
int C_EXPLICIT(Node * Na, Node * Nb);

int Forbidden(Node * Na, Node * Nb);

void InitMu(void);

void InitQuality();
void InitToursCost();
double GetCurrentQuality();

void *BitFlip();
int GetDimension(Node * Solution, int day);

void DeepCopy(Node * CibleSolution, Node * SourceSolution);
void WriteSolution(char *FileName, double Quality);
void WriteInitTours(char *FileName, double Quality);

double GetTime();


#endif //MOW_P_H
