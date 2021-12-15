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
   //std::cout << "module: " << module << " channel: " << channel << " sample: " << sample << std::endl;
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
   validLinks = 0;
   reset();
}

void exotic::Dsssd::reset() {
   eutils::reset_array(MAX_CHANNELS, ecal);
}

void exotic::Dsssd::read_data(const midas::Event &event) {
   channel_map(raw, validLinks, variables.adc.channel, variables.adc.module, event);
   channel_map(ecal, validLinks, variables.adc.channel, variables.adc.module, event);
}

void exotic::Dsssd::calculate() {
   eutils::linear_calibrate(ecal, validLinks, variables.adc);
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

// ================ class exotic::Mcp ========================= //

exotic::Mcp::Mcp(std::string alias) : Detector(alias), variables() {
   validLinks = 0;
   reset();
}

void exotic::Mcp::reset() {
   eutils::reset_array(MAX_CHANNELS, ecal);
   eutils::reset_data(x, y);
}

void exotic::Mcp::read_data(const midas::Event &event) {
   channel_map(raw, validLinks, variables.adc.channel, variables.adc.module, event);
   channel_map(ecal, validLinks, variables.adc.channel, variables.adc.module, event);
}

void exotic::Mcp::calculate() {
   eutils::linear_calibrate(ecal, validLinks, variables.adc);
   // Position calculation if we have all valid anode signals
	if(eutils::is_valid_all(raw, MAX_CHANNELS)) {
		//const double Lhalf = 25.;  // half the length of a single side of the MCP (50/2 [mm])
		double sum = eutils::calculate_sum(ecal, ecal + MAX_CHANNELS);
		if(sum != 0) {
			//x = Lhalf * ( (anode[1] + anode[2]) - (anode[0] + anode[3]) ) / sum;
			//y = Lhalf * ( (anode[0] + anode[1]) - (anode[2] + anode[3]) ) / sum;
		}
	}
   

}

// ================ class dragon::Mcp::Variables ================ //

exotic::Mcp::Variables::Variables() {
   reset();
}

void exotic::Mcp::Variables::reset() {
   // set default values
   std::fill(adc.module, adc.module + MAX_CHANNELS, 0);
   eutils::index_fill(adc.channel, adc.channel + MAX_CHANNELS);
   std::fill(adc.pedestal, adc.pedestal + MAX_CHANNELS, 0);
   std::fill(adc.slope, adc.slope + MAX_CHANNELS, 1.);
   std::fill(adc.offset, adc.offset + MAX_CHANNELS, 0.);
}