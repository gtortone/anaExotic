#include "midas/Database.h"
#include "midas/Event.h"
#include "utils/ErrorExotic.h"
#include "utils/Functions.h"
#include "utils/Valid.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
//#include "Defaults.hxx"
#include "utils/Exotic.h"

namespace eutils = exotic::utils;

//
// Helper function for ::variables::set(const char*)
namespace {

//
// Perform the action of creating a database from it's string name,
// checking if it's valid, giving error if not, or forwarding on to
// the overloaded method taking a db pointer if it's OK.
template <class T>
bool do_setv(T *t, const char *dbfile) {
   midas::Database db(dbfile);
   if (db.IsZombie()) {
      eutils::Error("", __FILE__, __LINE__) << "Zombie database: " << dbfile;
      return false;
   }
   return t->set(&db);
}

//
// Make sure a database is valid
bool check_db(const midas::Database *db, const char *cl) {
   bool success = db && !db->IsZombie();
   if (!success) {
      std::stringstream zmsg, where;
      where << cl << "::Variables::set";
      if (db)
         zmsg << ", IsZombie() " << db->IsZombie();
      eutils::Error(where.str().c_str(), __FILE__, __LINE__)
          << "Invalid database: 0x" << db << zmsg.str();
   }
   return success;
}

uint16_t channel_map(int module, int channel, const midas::Event &event, int number = 0) {

   std::stringstream bankName;
   uint16_t *data;
   int length;
   uint16_t snum, sample;
   
   bankName << "M00" << module;
   data = event.GetBankPointer<uint16_t>(bankName.str().c_str(), &length, true);

   snum = data[4];   // number of samples per channel
   if(number > snum) {
      eutils::Error("Megamp channel map", __FILE__, __LINE__) << "Invalid sample number" << number;
      return 0;
   }

   sample = data[((snum + 1) * (channel+1)) + number];
   std::cout << "module: " << module << " channel: " << channel << " sample: " << sample << std::endl;
   return sample;
   
}

template <class T>
void channel_map(T* output, int numch, const int* channels, const int* modules, const midas::Event &event) {
   
   for (int i=0; i<numch; ++i) {
		output[i] = channel_map(modules[i],channels[i], event);
	}
}



} // namespace

// ==================== Class exotic::RunParameters ==================== //

exotic::RunParameters::RunParameters() {
   /// ::
   reset();
}

void exotic::RunParameters::reset() {
   /// ::
   double *parrays[4] = {run_start, run_stop, trigger_start, trigger_stop};
   for (int i = 0; i < 4; ++i) {
      std::fill(parrays[i], parrays[i] + MAX_FRONTENDS, 0.);
   }
}

bool exotic::RunParameters::read_data(const midas::Database *db) {
   /// ::
   if (db == 0 || db->IsZombie()) {
      eutils::Error("exotic::RunParameters::read_data", __FILE__, __LINE__) << "Zombie database";
      return false;
   }

   bool success = true;
   if (success)
      success = db->ReadArray("/Experiment/Run Parameters/TSC_RunStart", run_start, MAX_FRONTENDS);
   if (success)
      success = db->ReadArray("/Experiment/Run Parameters/TSC_RunStop", run_stop, MAX_FRONTENDS);
   if (success)
      success = db->ReadArray("/Experiment/Run Parameters/TSC_TriggerStart", trigger_start, MAX_FRONTENDS);
   if (success)
      success = db->ReadArray("/Experiment/Run Parameters/TSC_TriggerStop", trigger_stop, MAX_FRONTENDS);

   if (!success)
      eutils::Error("exotic::RunParameters::read_data", __FILE__, __LINE__) << "Failed reading one of the ODB parameters.";

   return success;
}

// ================ class exotic::Dsssd ========================= //

exotic::Dsssd::Dsssd() : variables() {
   /// ::
   reset();
}

void exotic::Dsssd::reset() {
   //eutils::reset_data(efront, eback, hit_front, hit_back, tfront, tback);
   eutils::reset_array(MAX_CHANNELS, ecal);
}

bool exotic::Dsssd::set_variables(const char* dbfile)
{
   /*!
    * \param [in] dbfile Name of the XML database file
    * \note Passing \c "online" looks at the online ODB.
    */
   return do_setv(&this->variables, dbfile);
}

// void exotic::Dsssd::read_data(const vme::V785 adcs[], const vme::V1190& tdc)
// {
// 	/*!
// 	 * Copies adc data into \c this->ecal[] with channel and module mapping taken
// 	 * from variables.adc.channel and variables.adc.modules
// 	 *
// 	 * Delegates work to dutils::channel_map()
// 	 * \param [in] adcs Array of vme::V785 adc modules from which data can be taken
// 	 * \param [in] tdc vme::V1190 tdc module from which data can be read
// 	 */
// }

void exotic::Dsssd::read_data(const midas::Event &event) {
   channel_map(ecal, MAX_CHANNELS, variables.adc.channel, variables.adc.module, event);
}

void exotic::Dsssd::calculate() {
   /*!
	 * Does a linear transformation on each element in \c this->ecal[] using the slopes and offsets
	 * from variables.adc_slope and variables.adc_offset, respectively.
	 *
	 * Delegates the work to eutils::linear_calibrate
	 * \note Do we want to add a zero suppression threshold here?
	 */
   eutils::linear_calibrate(ecal, MAX_CHANNELS, variables.adc);
}

// ================ class dragon::Dsssd::Variables ================ //

exotic::Dsssd::Variables::Variables() {
   /*! Calls reset() */
   reset();
}

void exotic::Dsssd::Variables::reset() {
   /// ::
   // std::fill(adc.module, adc.module + MAX_CHANNELS, DSSSD_MODULE);
   eutils::index_fill(adc.channel, adc.channel + MAX_CHANNELS);

   std::fill(adc.pedestal, adc.pedestal + MAX_CHANNELS, 0);
   std::fill(adc.slope, adc.slope + MAX_CHANNELS, 1.);
   std::fill(adc.offset, adc.offset + MAX_CHANNELS, 0.);
}

bool exotic::Dsssd::Variables::set(const char *dbfile) {
   /*!
	 * \param [in] dbfile Path of the odb file from which you are extracting variable values
	 */
   return do_setv(this, dbfile);
}

bool exotic::Dsssd::Variables::set(const midas::Database *db) {
   /*!
	 * \param [in] Pointer to a constructed database.
	 */
   bool success = check_db(db, "exotic::Dsssd");

   if (success)
      success = db->ReadArray("/exotic/dsssd/variables/adc/module", adc.module, MAX_CHANNELS);
   if (success)
      success = db->ReadArray("/exotic/dsssd/variables/adc/channel", adc.channel, MAX_CHANNELS);
   if (success)
      success = db->ReadArray("/exotic/dsssd/variables/adc/slope", adc.slope, MAX_CHANNELS);
   if (success)
      success = db->ReadArray("/exotic/dsssd/variables/adc/offset", adc.offset, MAX_CHANNELS);

   return success;
}
