#include "INCLUDE/MOW-P.h"


int Forbidden(Node * Na, Node * Nb){
    if ((Na->Id <= DimensionSaved) == (Nb->Id <= DimensionSaved))
        return 1;
    return 0;
}