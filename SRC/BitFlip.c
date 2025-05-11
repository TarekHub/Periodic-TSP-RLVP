#include "INCLUDE/MOW-P.h"

static Node *RandomNode();
static void UpdateQuality(int drop, int add);
static void UpdateTourCost(int drop, int add, int day);

static int Feasible(int chosenDay);

static void AddNode(Node *N, int chosenDay);
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

void RemoveNode(Node *N, int chosenDay, Node **NPred, Node **NSuc){
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
  Node *N1, *N2, *FirstN, *BestN1, *BestN2;
  int cost, minCost = INT_MAX, drop;

  FirstN = &NodeSet[chosenDay * (Dimension + 1) + 1];
  N1 = FirstN;

  // Trouver l'arête qui fait le moins de dégâts possible
  do {
    N2 = N1->Suc;  // N2 est le successeur de N1

    // Calcul du coût d'insertion pour un graphe asymétrique:
    // (Coût de N1->N + Coût de N->N2) - Coût de N1->N2
    cost = (N1->C[N->Id] + N->C[N2->Id]) - N1->C[N2->Id];

    if (cost < minCost) {
      minCost = cost;
      BestN1 = N1;
      BestN2 = N2;
    }

    N1 = N1->Suc;  // Passer au nœud suivant
  } while (N1 != FirstN);  // Jusqu'à revenir au premier nœud

  drop = N->Delay;
  N->Delay = -*N->FillingPeriod;

  // Insérer N entre BestN1 et BestN2
  Add(BestN1, N, BestN2);

  CurrentToursDimension[chosenDay] += 1;
  UpdateQuality(drop, (-*N->FillingPeriod));
  UpdateTourCost(BestN1->C[BestN2->Id], (BestN1->C[N->Id] + N->C[BestN2->Id]), chosenDay);
  UpdateDelays(N, N->Delay);
}

void AddNodeKnown(Node *N, int chosenDay, Node *i, Node *j) {
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
    if( ((CurrentToursDimension[chosenDay] - 1) * Loading) + CurrentToursCost[chosenDay] > MaxDailyDuration)
      return 0;
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

