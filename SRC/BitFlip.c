#include "INCLUDE/MOW-P.h"

static Node *RandomNode();
static void UpdateQuality(int drop, int add);
static void UpdateTourCost(int drop, int add, int day);

static int Feasible(int chosenDay);

static void RemoveNode(Node *N, int chosenDay, Node **NPred, Node **NSuc);
static void AddNode(Node *N, int chosenDay);
static void AddNodeKnown(Node *N, int chosenDay, Node *i, Node *j);
static void UpdateDelays(Node *N, int initialDelay);
static int AreAlmostEqual(double a, double b, int SignificantDigits);
double round_to_sigfigs(double value, int sigfigs);

void *BitFlip()
{
  Node *N, *NPred = 0, *NSuc = 0;
  int chosenDay, chosendaydimension;
  double currentQuality;

  N = RandomNode();
  chosenDay = N->Day - 1 ;

  double logcurrentQuality = LogCurrentQuality;
  chosendaydimension = CurrentToursDimension[chosenDay];

  // Si le nœud est a supprimer
  if (N->IsVisited)
    RemoveNode(N, chosenDay, &NPred, &NSuc);
  else
    AddNode(N, chosenDay);

  // Comparer avec le voisin
  if(LogCurrentQuality <= BestLambdaQuality && Feasible(chosenDay)){
    AtLeastOneFeasibleNeighbor = 1;

    BestLambdaDay = chosenDay;
    BestLambdaQuality = LogCurrentQuality;

    for(int h = 0 ; h < TimeHorizon ; h++){
      BestLambdaToursCost[h] = CurrentToursCost[h];
      BestLambdaToursDimension[h] = CurrentToursDimension[h];
    }

    DeepCopy(BestLambda, NodeSet);
  }

  // Restorer la solution
  if (N->IsVisited)
    RemoveNode(N, chosenDay, &NPred, &NSuc);
  else
    AddNodeKnown(N, chosenDay, NPred, NSuc);


  //assert(fabs(LogCurrentQuality - logcurrentQuality) < Precision);
  //assert(LogCurrentQuality == logcurrentQuality);
  //assert(CurrentToursDimension[chosenDay] == chosendaydimension);
  return N;
}

static void RemoveNode(Node *N, int chosenDay, Node **NPred, Node **NSuc){
  Node *NextVisit;
  int drop;

  NextVisit = N;
  drop = N->Delay;

  if (!N->PrevVisit) {
    N->Delay = N->InitDelay;
  } else {
    N->Delay = N->PrevVisit->Delay + 1;
  }
  *NPred = N->Pred;
  *NSuc = N->Suc;

  Delete(N);

  CurrentToursDimension[chosenDay] -= 1;
  UpdateQuality(drop, N->Delay);
  UpdateTourCost(((*NPred)->C[N->Id] + N->C[(*NSuc)->Id]), (*NPred)->C[(*NSuc)->Id], chosenDay);

  UpdateDelays(NextVisit, N->Delay);
}

static void AddNode(Node *N, int chosenDay) {
  Node *N1, *N2, *FirstN, *i, *j;
  int cost, minCost = INT_MAX, drop;

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

  drop = N->Delay;
  N->Delay = - *N->FillingPeriod;

  Add(i, N, j);

  CurrentToursDimension[chosenDay] += 1;
  UpdateQuality(drop, (- *N->FillingPeriod));
  UpdateTourCost(i->C[j->Id], (i->C[N->Id] + N->C[j->Id]), chosenDay);

  UpdateDelays(N, N->Delay);
}

static void AddNodeKnown(Node *N, int chosenDay, Node *i, Node *j) {
  int drop;

  drop = N->Delay;
  N->Delay = - *N->FillingPeriod;

  Add(i, N, j);

  CurrentToursDimension[chosenDay] += 1;
  UpdateQuality(drop, (- *N->FillingPeriod));
  UpdateTourCost(i->C[j->Id], (i->C[N->Id] + N->C[j->Id]), chosenDay);

  UpdateDelays(N, N->Delay);
}

static void UpdateDelays(Node *N, int initialDelay) {
  int delay = initialDelay;
  Node *NextVisit = N;

  while ((NextVisit = NextVisit->NextVisit) && !NextVisit->IsVisited) {
    delay++;
    UpdateQuality(NextVisit->Delay, delay);
    NextVisit->Delay = delay;
  }
}

static Node *RandomNode()
{
  int randomIndex;

  do {
    randomIndex = rand() % TotalNodes;
  } while (NodeSet[randomIndex].Id == 0 || NodeSet[randomIndex].Id == 1);

  return &NodeSet[randomIndex];
}

static void UpdateQuality(int drop, int add)
{
  	LogCurrentQuality += pow(2, add) - pow(2, drop);
}

static void UpdateTourCost(int drop, int add, int day)
{
  CurrentToursCost[day] += add - drop;
}

int Feasible(int chosenDay)
{
    if( ((CurrentToursDimension[chosenDay] - 1) * Loading) +
            CurrentToursCost[chosenDay] > MaxDailyDuration){
      return 0;}
    return 1;
}

static int AreAlmostEqual(double a, double b, int SignificantDigits) {
    if (a == b) {
        return 1;
    }

    // Calcule la tolérance en fonction des chiffres significatifs
    double scale = pow(10, floor(log10(fmax(fabs(a), fabs(b)))) - SignificantDigits + 1);
    return fabs(a - b) < scale;
}

