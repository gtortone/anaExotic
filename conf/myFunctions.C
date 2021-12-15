#include "conf/helpers.h"

#include "TMath.h"

#define UP_E        MCP("A")->ecal[0]
#define DOWN_E      MCP("A")->ecal[2]
#define RIGHT_E     MCP("A")->ecal[4]
#define LEFT_E      MCP("A")->ecal[6]
#define CATHODE_E   MCP("A")->ecal[8]

#define UP_T        MCP("A")->ecal[1]
#define DOWN_T      MCP("A")->ecal[3]
#define RIGHT_T     MCP("A")->ecal[5]
#define LEFT_T      MCP("A")->ecal[7]
#define CATHODE_T   MCP("A")->ecal[9]

double QUP(void) {
    return MCP("A")->raw[0];
}

double QDOWN(void) {
   return MCP("A")->raw[2];
}

double UP(void) {
   return 1000 * UP_E / (UP_E + DOWN_E);
}

double DOWN(void) {
   return 1000 * DOWN_E / (UP_E + DOWN_E);
}

double RIGHT(void) {
   return 1000 * RIGHT_E / (RIGHT_E + LEFT_E);
}

double LEFT(void) {
   return 1000 * LEFT_E / (RIGHT_E + LEFT_E);
}

double X(void) {
   double value;
   value = (RIGHT_E - LEFT_E) / (RIGHT_E + LEFT_E);
   return value;
   //return ( ((1000 * UP_E / (UP_E + DOWN_E)) - (1000 * DOWN_E / (UP_E + DOWN_E)) + 2000) / ((1000 * UP_E / (UP_E + DOWN_E)) + (1000 * DOWN_E / (UP_E + DOWN_E)))  );
}

double Y(void) {
   double value;
   value = (UP_E - DOWN_E) / (UP_E + DOWN_E);
   return value;
   //return ( ((1000 * RIGHT_E / (RIGHT_E + LEFT_E)) - (1000 * LEFT_E / (RIGHT_E + LEFT_E)) + 2000) / ((1000 * RIGHT_E / (RIGHT_E + LEFT_E)) + (1000 * LEFT_E / (RIGHT_E + LEFT_E)))  );
}

double QX(void) {
   return (UP_E + DOWN_E);
}

double QY(void) {
   return (RIGHT_E + LEFT_E);
}

