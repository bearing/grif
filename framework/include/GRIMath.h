#ifndef GRIF_FRAMEWORK_INCLUDE_GRIMATH_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIMATH_H_

#include <cmath>
#include <cstdlib>

double erfinv(double X);

double *SampleGauss(double Mean, double Sigma, int num);
double SampleGauss(double Mean, double Sigma);

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIMATH_H_
