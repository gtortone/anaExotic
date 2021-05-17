#include "midas/Database.h"
#include "utils/Definitions.h"
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

//
// Set a bank name from the ODB
bool odb_set_bank(midas::Bank_t *bank, const midas::Database *db, const char *path, int arrayLen = 0) {
   bool success;
   if (!arrayLen) {
      std::string bkName;
      success = db->ReadValue(path, bkName);
      if (success)
         eutils::set_bank_name(bkName.data(), *bank);
   } else {
      std::vector<std::string> bkNames(arrayLen);
      success = db->ReadArray(path, &bkNames[0], arrayLen);
      if (success) {
         for (int i = 0; i < arrayLen; ++i) {
            eutils::set_bank_name(bkNames[i].data(), *(bank + i));
         }
      }
   }
   return success;
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
// 	eutils::channel_map(ecal, MAX_CHANNELS, variables.adc.channel, variables.adc.module, adcs);
// 	eutils::channel_map(tfront, variables.tdc_front.channel, tdc);
// 	eutils::channel_map(tback,  variables.tdc_back.channel,  tdc);
// }

void exotic::Dsssd::read_data(void) {
   ecal[0] = 10;
   ecal[1] = 20;
   eutils::Info("exotic::read_data", __FILE__, __LINE__);
}

void exotic::Dsssd::calculate() {
   /*!
	 * Does a linear transformation on each element in \c this->ecal[] using the slopes and offsets
	 * from variables.adc_slope and variables.adc_offset, respectively. Also calibrates the TDC
	 * signal; calculates efront, hit_front, eback, and hit_back.
	 *
	 * Delegates the work to dutils::linear_calibrate
	 * \note Do we want to add a zero suppression threshold here?
	 */
   eutils::linear_calibrate(ecal, MAX_CHANNELS, variables.adc);
   // eutils::linear_calibrate(tfront, variables.tdc_front);
   // eutils::linear_calibrate(tback,  variables.tdc_back);

   if (eutils::is_valid_any(ecal, 16)) {
      const double *const pmax_front = std::max_element(ecal, ecal + 16);
      //efront = *pmax_front;
      //hit_front = pmax_front - ecal;
   }

   if (eutils::is_valid_any(ecal + 16, 16)) {
      const double *const pmax_back = std::max_element(ecal + 16, ecal + 32);
      //eback  = *pmax_back;
      //hit_back = pmax_back - ecal;
   }
}

// ================ class dragon::Dsssd::Variables ================ //

exotic::Dsssd::Variables::Variables() {
   /*! Calls reset() */
   reset();
}

void exotic::Dsssd::Variables::reset() {
   /// ::
   // std::fill(adc.module, adc.module + MAX_CHANNELS, DSSSD_MODULE);
   // eutils::index_fill(adc.channel, adc.channel + MAX_CHANNELS, DSSSD_ADC0);

   std::fill(adc.pedestal, adc.pedestal + MAX_CHANNELS, 0);
   std::fill(adc.slope, adc.slope + MAX_CHANNELS, 1.);
   std::fill(adc.offset, adc.offset + MAX_CHANNELS, 0.);

   // tdc_front.module  = 0; // unused
   // tdc_front.channel = DSSSD_TDC0;
   // tdc_front.slope   = 1.;
   // tdc_front.offset  = 0.;

   // tdc_back.module  = 0; // unused
   // tdc_back.channel = DSSSD_TDC0 + 1;
   // tdc_back.slope   = 1.;
   // tdc_back.offset  = 0.;
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

   // if(success) success = db->ReadValue("/exotic/dsssd/variables/tdc_front/channel", tdc_front.channel);
   // if(success) success = db->ReadValue("/exotic/dsssd/variables/tdc_front/slope",   tdc_front.slope);
   // if(success) success = db->ReadValue("/exotic/dsssd/variables/tdc_front/offset",  tdc_front.offset);

   // if(success) success = db->ReadValue("/exotic/dsssd/variables/tdc_back/channel", tdc_back.channel);
   // if(success) success = db->ReadValue("/exotic/dsssd/variables/tdc_back/slope",   tdc_back.slope);
   // if(success) success = db->ReadValue("/exotic/dsssd/variables/tdc_back/offset",  tdc_back.offset);

   return success;
}
