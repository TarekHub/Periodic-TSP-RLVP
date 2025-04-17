#include "INCLUDE/MOW-P.h"


static const char Delimiters[] = " :=\n\t\r\f\v\xef\xbb\xbf";
static void Read_DIMENSION();
static void Read_EDGE_WEIGHT_SECTION();
static void Read_INIT_LAST_VISITED_SECTION();
static void CreateNodes();
static void Read_FILLING_PERIOD_SECTION(void);
static int FixEdge(Node * Na, Node * Nb);


void ReadProblem()
{
    int i;
    char *Line, *Keyword;

    if (!(ProblemFile = fopen(ProblemFileName, "r")))
        eprintf("Cannot open PROBLEM_FILE: \"%s\"", ProblemFileName);

    while ((Line = ReadLine(ProblemFile))) {
        if (!(Keyword = strtok(Line, Delimiters)))
            continue;
        for (i = 0; i < (int) strlen(Keyword); i++)
            Keyword[i] = (char) toupper(Keyword[i]);
		if (!strcmp(Keyword, "DIMENSION"))
          Read_DIMENSION();
        else if (!strcmp(Keyword, "EDGE_WEIGHT_SECTION"))
          Read_EDGE_WEIGHT_SECTION();
        else if (!strcmp(Keyword, "FILLING_PERIOD_SECTION"))
          Read_FILLING_PERIOD_SECTION();
        else if (!strcmp(Keyword, "INIT_LAST_VISITED_SECTION"))
          Read_INIT_LAST_VISITED_SECTION();
        else if (!strcmp(Keyword, "EOF"))
            break;
    }

    TotalNodes = (Dimension + 1) * TimeHorizon;
    free(LastLine);
    LastLine = 0;
}

static void Read_DIMENSION()
{
    char *Token = strtok(0, Delimiters);

    if (!Token || !sscanf(Token, "%d", &Dimension))
        eprintf("DIMENSION: Integer expected");
    if (Dimension < 0)
        eprintf("DIMENSION: < 0");
    DimensionSaved = Dim = Dimension;;
}

static void Read_INIT_LAST_VISITED_SECTION(void)
{
  Node * N;
  int id, i, lastVisited = 0;

	for(i = 0; i < Dimension; i++) {
          fscanint(ProblemFile, &id);
          if (id <= 0 || id > Dim)
            eprintf("DAILY_WASTE_RATE_SECTION: Node number out of range: %d", id);
          N = &NodeSet[id];
          if (!fscanf(ProblemFile, "%d", &lastVisited))
      		eprintf("DAILY_WASTE_RATE_SECTION: Missing last visited for node %d", id);
    	  N->Delay = N->InitDelay = lastVisited - *(N->FillingPeriod);
	}
}

static void Read_FILLING_PERIOD_SECTION(void)
{
  int Id, i, id, h, w, offset;
  Node *N = 0;

  FillingPeriods = (int *) calloc((size_t) Dimension , sizeof(int));

  for (h = 0; h < TimeHorizon; h++) {
    offset = h * (Dimension + 1);
	id = 1;
    N = &NodeSet[offset + id];

	do{
      N->FillingPeriod = &FillingPeriods[(size_t) (N->Id - 1)];
      id += 1;
      N = &NodeSet[offset + id];
    }while(id <= Dim);
  }

  for (i = 1; i <= Dim; i++) {
    fscanint(ProblemFile, &Id);
    if (Id <= 0 || Id > Dim)
      eprintf("DEMAND_SECTION: Node number out of range: %d", Id);
    N = &NodeSet[Id];
    if (!fscanf(ProblemFile, "%d", &w))
      eprintf("DEMAND_SECTION: Missing demand for node %d", Id);
    *(N->FillingPeriod) = w;
  }
}

static void Read_EDGE_WEIGHT_SECTION()
{
  Node *Ni;
  int i, j, id, offset, W, h;
  double w;

  if (!FirstNode)
    CreateNodes();

  Dim = Dimension;
  CostMatrix = (int *) calloc((size_t) Dim * Dim, sizeof(int));
  for (h = 0; h < TimeHorizon; h++) {
    offset = h * (Dimension + 1);
	id = 1;
    Ni = &NodeSet[offset + id];

    do{
      Ni->C = &CostMatrix[(size_t) (Ni->Id - 1) * Dim] - 1;
      id += 1;
      Ni = &NodeSet[offset + id];
    }while(id <= Dim);
  }

  for (i = 1; i <= Dim; i++) {
    Ni = &NodeSet[i];
    for (j = 1; j <= Dim; j++) {
      if (!fscanf(ProblemFile, "%lf", &w))
        eprintf("EDGE_WEIGHT_SECTION: Missing weight");
      W = round(1 * w);
      Ni->C[j] = W;
    }
  }
}

static void CreateNodes()
{
    Node *Prev = 0, *N = 0;
    int h, offset, i;
    if (Dimension <= 0)
        eprintf("DIMENSION is not positive (or not specified)");

    // Dim = DimensionSaved;
    // Dimension = 2 * DimensionSaved;

    NodeSet = (Node *) calloc(TimeHorizon * (Dimension + 1), sizeof(Node));
    for (h = 0; h < TimeHorizon; h++) {
        offset = h * (Dimension + 1);

        for (i = 1; i <= Dimension; i++, Prev = N) {
            N = &NodeSet[offset + i];
			if (i==1) N->IsVisited = 1;
            N->Id = i;
            N->Day = h + 1;

            if (h != TimeHorizon - 1)
            	N->NextVisit = &NodeSet[(h + 1) * (Dimension + 1) + i];
            if (offset != 0)
        		N->PrevVisit = &NodeSet[(h - 1) * (Dimension + 1) + i];
        }
    }
    FirstNode = &NodeSet[1];
}