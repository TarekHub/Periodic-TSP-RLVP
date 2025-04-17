#include "INCLUDE/MOW-P.h"

int GetDimension(Node * Solution, int day){

    Node * FirstNode = &Solution[day * (Dimension + 1) + 1];
    Node * N;
    int Dim = 0;

    N = FirstNode;
    do
		Dim++;
    while ((N = N->Suc) != FirstNode);

    return Dim;
}