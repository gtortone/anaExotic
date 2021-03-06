#ifndef HAVE_EXOTIC_H_
#define HAVE_EXOTIC_H_

#include "midas/Event.h"
#include "utils/VariableStructs.h"
#include <stdint.h>

namespace exotic {

// ======= Class definitions ======== //
//
// Detector
//
class Detector {

 private:
   std::string name;

 public:
   Detector(std::string s) { name = s; };
   virtual ~Detector(){};

   virtual void reset()=0;
   virtual void read_data(const midas::Event &event)=0;
   virtual void calculate()=0;

   std::string getName(void) { return name; };
};

//
// Double-Sided Silicon Strip Detector
//

class Dsssd : public Detector {

 public: // Constants
   static const int MAX_CHANNELS = 32;

 public: // Methods
   Dsssd(std::string alias);
   void reset();
   void read_data(const midas::Event &event);
   void calculate();

 public:
   // Raw values
   uint raw[MAX_CHANNELS];
   // Calibrated energy signals
   double ecal[MAX_CHANNELS];
   // Number of connected detector links
   uint validLinks;

 public: // Subclasses
   //
   // Dsssd Variables Class
   //
   class Variables {
    public: // Methods
      Variables();
      void reset();

    public: // Data
      /// Adc variables for the energy signals
      exotic::utils::AdcVariables<MAX_CHANNELS> adc;
   };

 public: // Subclass instances
   /// Instance of Dsssd::Variables
   Dsssd::Variables variables;
};

//
// Micro Channel Plate
//

class Mcp : public Detector {

 public: // Constants
   // up, down, right, left, cathode
   static const int MAX_CHANNELS = 10;

 public: // Methods
   Mcp(std::string alias);
   void reset();
   void read_data(const midas::Event &event);
   void calculate();

 public:
   // signal order: UP_E, UP_T, DOWN_E, DOWN_T, RIGHT_E, RIGHT_T, LEFT_E, LEFT_T, CATHODE_E, CATHODE_T
   // Raw values
   uint raw[MAX_CHANNELS];
   // Calibrated energy signals
   double ecal[MAX_CHANNELS];
   // Number of connected detector links
   uint validLinks;
   // Position
   double x,y;

 public: // Subclasses
   //
   // Dsssd Variables Class
   //
   class Variables {
    public: // Methods
      Variables();
      void reset();

    public: // Data
      /// Adc variables for the energy signals
      exotic::utils::AdcVariables<MAX_CHANNELS> adc;
   };

 public: // Subclass instances
   /// Instance of Dsssd::Variables
   Dsssd::Variables variables;
};

} // namespace exotic

#endif