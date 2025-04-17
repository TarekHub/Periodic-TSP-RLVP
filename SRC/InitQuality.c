#include "INCLUDE/MOW-P.h"

void InitQuality(){
  int offset;

  CurrentQuality = 0.0;
  for(int h = 0; h < TimeHorizon; h++){
    offset = h * (Dimension + 1);
    for(int i = 2; i <= Dimension; i++){
      CurrentQuality += pow(2, NodeSet[offset + i].Delay);
    }
  }
}