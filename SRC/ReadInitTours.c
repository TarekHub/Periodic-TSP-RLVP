#include "INCLUDE/MOW-P.h"

static const char Delimiters[] = " :=\n\t\r\f\v\xef\xbb\xbf";

static void Read_DAY_DIMENSION(FILE * InitToursFile, int day);
static void SetDelays();

void ReadInitTours(void)
{
  int i, day = -1;
  char *Line, *Keyword;

  if (!(InitToursFile = fopen(InitToursFileName, "r")))
    eprintf("Cannot open INIT_TOURS_FILE: \"%s\"", InitToursFileName);

  while ((Line = ReadLine(InitToursFile))) {
        if (!(Keyword = strtok(Line, Delimiters)))
            continue;
        for (i = 0; i < (int) strlen(Keyword); i++)
            Keyword[i] = (char) toupper(Keyword[i]);
        if (!strcmp(Keyword, "DAY"))
          day ++;
        else if (!strcmp(Keyword, "DIMENSION"))
          Read_DAY_DIMENSION(InitToursFile, day);
  }

  SetDelays();
  free(LastLine);
  LastLine = 0;
}

static void SetDelays(void)
{
  	int i, delay;
	Node *N;

    for (i=2; i <= Dim; i++){
      N = &NodeSet[i];
      delay = N->Delay;

      do{
        if (N->IsVisited){
          N->Delay = - *(N->FillingPeriod);
          delay = N->Delay;
        }else{
          N->Delay = delay;
        }
        delay ++;
      }while((N = N->NextVisit));
    }
}

static void Read_DAY_DIMENSION(FILE * InitToursFile, int day)
{
    Node *N, *Prev, *FirstTourNode;
    char *Token = strtok(0, Delimiters);
    int DimensionTour;
    int id;

    if (!Token || !sscanf(Token, "%d", &DimensionTour))
        eprintf("TOUR_DIMENSION: Integer expected");
    if (DimensionTour < 0)
        eprintf("TOUR_DIMENSION: < 0");

    CurrentToursDimension[day] = DimensionTour;
    FirstTourNode = &NodeSet[day * (Dimension + 1) + 1];

    for(int i = 0; i < DimensionTour; i++){
      fscanint(InitToursFile, &id);
   	  N = &NodeSet[day * (Dimension + 1) + id];
   	  if (id != 1){
      	Link(Prev, N);
        N->IsVisited = 1;
      }
      Prev = N;
    }

    Link(Prev, FirstTourNode);
}