#include "INCLUDE/MOW-P.h"


int Distance_ATSP(Node * Na, Node * Nb)
{
    int n = DimensionSaved;
    if ((Na->Id <= n) == (Nb->Id <= n)){
      eprintf("ERROR in Distance_ATSP\n");
      return 9999999;
    }
    if (abs(Na->Id - Nb->Id) == n)
        return 0;
    return Na->Id <= n ? Na->C[Nb->Id - n] : Nb->C[Na->Id - n];
}

int D_FUNCTION(Node * Na, Node * Nb)
{
    return Fixed(Na, Nb) ? 0 : Distance(Na, Nb);
}

int C_EXPLICIT(Node * Na, Node * Nb)
{
    return Na->Id < Nb->Id ? Nb->C[Na->Id] : Na->C[Nb->Id];
}