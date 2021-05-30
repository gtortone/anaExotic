#include "conf/helpers.h"

double myfunc1(void) {
   return DSSSD("front")->ecal[1];
}

double myfunc2(void) {
   return (DSSSD("front")->ecal[0] + DSSSD("back")->ecal[0]);
}

double myfunc3(void) {
   return (DSSSD("front")->ecal[0] * 2);
}
