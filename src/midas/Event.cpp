#include "midas/Event.h"
#include "utils/Functions.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <stdint.h>

namespace {

// Helper function to calculate full timestamp values
inline uint64_t read_timestamp(uint32_t tscl, uint32_t tsch) {
   // tscl: lower bits 29..0 bits, tsch: upper bits 35..28
   uint64_t tscfull = tscl & 0x1fffffff; // bits 28..0

   // check for match in overlap bit 29
   int bit29h = (tsch >> 1) & 0x1, bit29l = (tscl >> 29) & 0x1;
   if (bit29h != bit29l) { // adjust upper bits to account for difference
      if (bit29l < bit29h)
         ++tsch;
      else
         --tsch;
   }
   // append bits > 29
   tscfull |= ((uint64_t)(tsch >> 1) << (uint64_t)29);
   return tscfull;
}

} // namespace

// ========= Class midas::Event ========= //

midas::Event::Event(char *buf, int size) : fCoincWindow(0),
                                           fClock(std::numeric_limits<uint64_t>::max()),
                                           fTriggerTime(0.) {
   /*!
	 * \param buf Buffer containing the entirity of the event data (header + actual data)
	 * \param size Size of the data portion of the event (not including the header)
	 */
   Init(0, buf, buf + sizeof(midas::Event::Header), size);
}

midas::Event::Event(const void *header, const void *data, int size) : fCoincWindow(0),
                                                                      fClock(std::numeric_limits<uint64_t>::max()),
                                                                      fTriggerTime(0.) {
   /*!
	 * \param header Pointer to event header (midas::Event::Header struct)
	 * \param data Pointer to the data portion of an event
	 * \param size Size in bytes of the data portion of the event
	 */
   Init(0, header, data, size);
}

void midas::Event::CopyDerived(const midas::Event &other) {
   /*!
	 * Copies all data fields and calls TMidasEvent::Copy().
	 */
   fClock = other.fClock;
   fTriggerTime = other.fTriggerTime;
   fCoincWindow = other.fCoincWindow;
   //other.CopyFifo(fFifo);
   TMidasEvent::Copy(other);
}

void midas::Event::PrintSingle(FILE *where) const {
   std::stringstream sstr;
   sstr << "Singles event: id, ser, trig, clock: "
        << GetEventId() << ", " << GetSerialNumber() << ", " << fTriggerTime << ", " << fClock;
   fprintf(where, "%s\n", sstr.str().c_str());
}

void midas::Event::Init(const char *tsbank, const void *header, const void *addr, int size) {
   memcpy(GetEventHeader(), header, sizeof(midas::Event::Header));
   memcpy(GetData(), addr, GetDataSize());
   SetBankList();

   if (tsbank != 0) {
      int tsclength;
      uint32_t *ptsc = GetBankPointer<uint32_t>(tsbank, &tsclength, true, true);
      if (!ptsc)
         throw(std::invalid_argument(tsbank));

      // Read: firmware revision, write timestamp, routing, sync number
      uint32_t version = *ptsc++;
      uint32_t bkts = *ptsc++;
      uint32_t route = *ptsc++;

      // Suppress compiler warning about unused values
      if (version && bkts && route)
         ;

      // Check version
      uint32_t versions[] =
          {0x01130215, 0x01121212, 0x01120925, 0x01120809, 0x01120810, 0x01120910};

      bool okVersion = false;
      for (uint32_t v = 0; v < sizeof(versions) / sizeof(uint32_t); ++v) {
         if (version == versions[v]) {
            okVersion = true;
            break;
         }
      }
      if (okVersion == false) {
         exotic::utils::Warning("midas::Event::Init") << "Unknown TSC version 0x" << std::hex << version << std::dec << " (id, serial #: " << GetEventId() << ", " << GetSerialNumber() << ")" << EXOTIC_ERR_FILE_LINE;
      }

      // Get TSC4 info
      // uint32_t ctrl = *ptsc++, roll = *ptsc++;
      // uint32_t nch = ctrl & READ14;         // number of channels
      // bool overflow = (ctrl >> 15) & READ1; // overflow flag
      // uint32_t tsch = (ctrl >> 16) & READ8; // upper tsc bits 35..28

      // if (overflow && false) {
      //    exotic::utils::Warning("midas::Event::Init") << "IO32 TSC in overflow condition. Event Serial #, Id: " << GetSerialNumber() << ", " << GetEventId() << "\n";
      // }

      // for (uint32_t i = 0; i < nch; ++i) {
      //    uint32_t tscl = *ptsc++, ch = (tscl >> 30) & READ2;
      //    assert(ch < MAX_FIFO);

      //    uint64_t tscfull = read_timestamp(tscl, tsch | (roll << 8));
      //    fFifo[ch].push_back(tscfull);

      //    if (ch == TRIGGER_CHANNEL && fClock == std::numeric_limits<uint64_t>::max()) {
      //       fClock = tscfull;
      //       fTriggerTime = fClock / DRAGON_TSC_FREQ;
      //    }
      // }
   } // if tsbank != 0
}

double midas::Event::TimeDiff(const Event &other) const {
   /*!
	 * Takes into account rollover by checking that the abslute difference is < 0x1fffffff
	 * \note 'this' - 'other'
	 */
   return fTriggerTime - other.fTriggerTime;
}
