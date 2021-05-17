#ifndef HAVE_EXOTIC_H_
#define HAVE_EXOTIC_H_

#include "utils/VariableStructs.h"
#include "midas/Database.h"
#include <stdint.h>

namespace exotic {

// ======= Class definitions ======== //
///
/// Global run parameters
///
class RunParameters {
 public:                               // Constants
   static const int MAX_FRONTENDS = 2; //!
 public:                               // Methods
   /// Constructor, calls reset()
   RunParameters();
   /// Sets all data to defaults
   void reset();
   /// Reads data from the ODB or a midas file
   bool read_data(const midas::Database *db);

 public: // Data
   /// Run start time from the tsc (in seconds)
   /*! [0]: head, [1]: tail */
   double run_start[MAX_FRONTENDS];
   /// Run stop time from the tsc (in seconds)
   /*! [0]: head, [1]: tail */
   double run_stop[MAX_FRONTENDS];
   /// Trigger start time from the tsc (in seconds)
   /*! [0]: head, [1]: tail */
   double trigger_start[MAX_FRONTENDS];
   /// Trigger stop time from the tsc (in seconds)
   /*! [0]: head, [1]: tail */
   double trigger_stop[MAX_FRONTENDS];
};

//
// Double-Sided Silicon Strip Detector
//

class Dsssd {

 public: // Constants
   static const int MAX_CHANNELS = 32;

 public: // Methods
   Dsssd();
   void reset();
   //void read_data(const vme::V785 adcs[], const vme::V1190 &tdc);
   void read_data(void);
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