#include "conf/helpers.h"

#include "TMath.h"

double myfunc1(void) {
   return TMath::Sqrt(DSSSD("front")->ecal[0]);
}

double myfunc2(void) {
   return (DSSSD("front")->ecal[0] + DSSSD("back")->ecal[0]);
}

double myfunc3(void) {
   return (DSSSD("front")->ecal[0] * 2);
}

double myfunc4(void) {
   //printf("%d\n", DSSSD("front")->raw[0]);
   return DSSSD("front")->raw[0];
}
