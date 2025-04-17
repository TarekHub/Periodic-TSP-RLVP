#include "INCLUDE/MOW-P.h"
double GetCurrentQuality(){
    int offset;
    double quality = 0.0;

    for(int h = 0; h < TimeHorizon; h++){
        offset = h * (Dimension + 1);
        for(int i = 2; i <= Dimension; i++){
          	quality += pow(2, NodeSet[offset + i].Delay);
        }
    }
    return quality;
}