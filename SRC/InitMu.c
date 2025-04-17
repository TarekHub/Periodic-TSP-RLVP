#include "INCLUDE/MOW-P.h"


void InitMu(void){
  int i;
  mu = (double **)malloc(muSize * sizeof(double *));

  for (i = 0; i < muSize; i++) {
    mu[i] = (double *)malloc(2 * sizeof(double));
  }

  for (i = 0; i < muSize; i++) {
    double a = (double)i / (muSize - 1);
    mu[i][0] = a;
    mu[i][1] = 1.0 - a;
  }
}