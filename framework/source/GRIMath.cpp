#include "GRIMath.h"

double erfinv(double P) {
  double      Y,A,B,X,Z,W,WI,SN,SD,F,Z2,SIGMA;
  double      A1=-.5751703,A2=-1.896513,A3=-.5496261E-1;
  double      B0=-.1137730,B1=-3.293474,B2=-2.374996,B3=-1.187515;
  double      C0=-.1146666,C1=-.1314774,C2=-.2368201,C3=.5073975e-1;
  double      D0=-44.27977,D1=21.98546,D2=-7.586103;
  double      E0=-.5668422E-1,E1=.3937021,E2=-.3166501,E3=.6208963E-1;
  double      F0=-6.266786,F1=4.666263,F2=-2.962883;
  double      G0=.1851159E-3,G1=-.2028152E-2,G2=-.1498384,G3=.1078639E-1;
  double      H0=.9952975E-1,H1=.5211733,H2=-.6888301E-1;

  X=P;
  if (X < 0) {
    SIGMA = -1;
  } else {
    SIGMA = 1;
  }

  if(X<-1 || X>1) {
    return -1;
  }

  Z=fabs(X);
  if (Z>.85) {
    A=1-Z;
    B=Z;
    W=sqrt(-log(A+A*B));
    if (W>=2.5) {
      if (W>=4.) {
        WI=1./W;
        SN=((G3*WI+G2)*WI+G1)*WI;
        SD=((WI+H2)*WI+H1)*WI+H0;
        F=W+W*(G0+SN/SD);
      } else {
        SN=((E3*W+E2)*W+E1)*W;
        SD=((W+F2)*W+F1)*W+F0;
        F=W+W*(E0+SN/SD);
      }
    } else {
      SN=((C3*W+C2)*W+C1)*W;
      SD=((W+D2)*W+D1)*W+D0;
      F=W+W*(C0+SN/SD);
    }
  } else {
    Z2=Z*Z;
    F=Z+Z*(B0+A1*Z2/(B1+Z2+A2/(B2+Z2+A3/(B3+Z2))));
  }
    Y=SIGMA*F;
    return Y;
}

double *SampleGauss(double Mean, double Sigma, int num) {
  double *N = new double[num];
  for(int i=0; i<num; i++) {
    double rnd = double(rand() % RAND_MAX)/RAND_MAX;
    N[i]=sqrt(2.0)*Sigma*erfinv(rnd*2-1) + Mean;
  }
  return N;
}

double SampleGauss(double Mean, double Sigma){
  double rnd = double(rand() % RAND_MAX)/RAND_MAX;
  return (sqrt(2.0)*Sigma*erfinv(rnd*2-1) + Mean);
}
