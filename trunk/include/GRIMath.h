#ifndef GRIMATH_H
#define GRIMATH_H

#include <cmath>
#include <cstdlib>

double erfinv(double X);

double *SampleGauss(double Mean, double Sigma, int num);
double SampleGauss(double Mean, double Sigma);

#endif  // GRIMATH_H
