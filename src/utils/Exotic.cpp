#include "utils/Exotic.h"
#include "midas/Database.h"
#include "midas/Event.h"
#include "utils/ErrorExotic.h"
#include "utils/Functions.h"
#include "utils/Valid.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace eutils = exotic::utils;

namespace {

uint16_t channel_map(int module, int channel, const midas::Event &event, int number = 0) {

   std::stringstream bankName;
   uint16_t *data;
   int length;
   uint16_t snum, sample;

   bankName << "M00" << module;
   data = event.GetBankPointer<uint16_t>(bankName.str().c_str(), &length, true);

   snum = data[4]; // number of samples per channel
   if (number > snum) {
      eutils::Error("Megamp channel map", __FILE__, __LINE__) << "Invalid sample number" << number;
      return 0;
   }

   sample = data[((snum + 1) * (channel + 1)) + number];
   std::cout << "module: " << module << " channel: " << channel << " sample: " << sample << std::endl;
   return sample;
}

template <class T>
void channel_map(T *output, int numch, const int *channels, const int *modules, const midas::Event &event) {

   for (int i = 0; i < numch; ++i) {
      output[i] = channel_map(modules[i], channels[i], event);
   }
}

} // namespace

// ================ class exotic::Dsssd ========================= //

exotic::Dsssd::Dsssd(std::string alias) : Detector(alias), variables() {
   reset();
}

void exotic::Dsssd::reset() {
   //eutils::reset_data(efront, eback, hit_front, hit_back, tfront, tback);
   eutils::reset_array(MAX_CHANNELS, ecal);
}

bool exotic::Dsssd::set_variables(const char *dbfile) {
   /*!
    * \param [in] dbfile Name of the XML database file
    * \note Passing \c "online" looks at the online ODB.
    */
   //return do_setv(&this->variables, dbfile);
   return 1;
}

// void exotic::Dsssd::read_data(const vme::V785 adcs[], const vme::V1190& tdc)
// {
// 	/*!
// 	 * Copies adc data into \c this->ecal[] with channel and module mapping taken
// 	 * from variables.adc.channel and variables.adc.modules
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
   reset();
}

void exotic::Dsssd::Variables::reset() {
   // set default values
   std::fill(adc.module, adc.module + MAX_CHANNELS, 0);
   eutils::index_fill(adc.channel, adc.channel + MAX_CHANNELS);
   std::fill(adc.pedestal, adc.pedestal + MAX_CHANNELS, 0);
   std::fill(adc.slope, adc.slope + MAX_CHANNELS, 1.);
   std::fill(adc.offset, adc.offset + MAX_CHANNELS, 0.);
}