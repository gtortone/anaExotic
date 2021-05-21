#ifndef HAVE_EXOTIC_H_
#define HAVE_EXOTIC_H_

#include "midas/Database.h"
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
   virtual bool set_variables(const char *dbfile)=0;
   virtual void read_data(const midas::Event &event)=0;
   virtual void calculate()=0;

   std::string getName(void) { return name; };
};

//
// Double-Sided Silicon Strip Detector
//

class Dsssd : public Detector {

 public: // Constants
   static const int MAX_CHANNELS = 8;

 public: // Methods
   Dsssd(std::string alias);
   void reset();
   bool set_variables(const char *dbfile);
   void read_data(const midas::Event &event);
   void calculate();

 public:
   // Calibrated energy signals
   double ecal[MAX_CHANNELS];

 public: // Subclasses
   //
   // Dsssd Variables Class
   //
   class Variables {
    public: // Methods
      Variables();
      bool set(const char *dbfile);
      bool set(const midas::Database *db);
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