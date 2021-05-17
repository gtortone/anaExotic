#ifndef MIDAS_EVENT_HXX
#define MIDAS_EVENT_HXX
#include "TMidasEvent.h"
#include "utils/ErrorExotic.h"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <typeinfo>
#include <vector>

namespace midas {

typedef char Bank_t[5];

class Event : public TMidasEvent {

 public:

   typedef TMidas_EVENT_HEADER Header;

 private:
   // Coincidence window (in us)
   double fCoincWindow;

   // Timestamp value in clock cycles since BOR
   uint64_t fClock;

   // Timestamp value in uSec
   double fTriggerTime;

 public:
   // Empty constructor
   Event() : TMidasEvent() {}

   // Construct from event callback parameters
   Event(const void *header, const void *data, int size);

   // Construct from direct polling parameters,
   Event(char *buf, int size);

   // Copy constructor
   Event(const Event &other) : TMidasEvent(other) { CopyDerived(other); }

   // Assignment operator
   Event &operator=(const Event &other) {
      CopyDerived(other);
      return *this;
   }

   // Copies event header information into another one
   void CopyHeader(Header &destination) const { memcpy(&destination, &fEventHeader, sizeof(Header)); }

   /// Read an event from a TMidasFile
   // bool ReadFromFile(TMidasFile &file) {
   //    Clear();
   //    return file.Read(this);
   // }

   /// Returns trigger time in uSec
   double TriggerTime() const { return fTriggerTime; }

   /// Returns the trigger time in clock cycles
   uint64_t ClockTime() const { return fClock; }

   /// Copy fifo values to an external vector array
   void CopyFifo(std::vector<uint64_t> *pfifo) const;

   /// Prints timestamp information for a singles event
   void PrintSingle(FILE *where = stdout) const;

   /// Calculates difference of timestamps
   double TimeDiff(const Event &other) const;

   /// Destructor, empty
   virtual ~Event() {}

   /// Bank finding routine (templated)
   template <typename T>
   T *GetBankPointer(const Bank_t name, int *length, bool reportMissing = false, bool checkType = false) const {
      /*!
			 * \param [in] name Name of the bank to search for
			 * \param [out] Length of the returned bank
			 * \param [in] reportMissing True means a warning message is printed if the bank is absent
			 * \param [in] checkType Specifies whether or not to check that the template parameter
			 *  matches the TID of the bank. If this parameter is set to true and the types do
			 *  not match, the error is fatal.
			 * \returns Pointer to the beginning of the bank
			 */
      void *pbk;
      int type;
      int bkfound = FindBank(name, length, &type, &pbk);

      if (!bkfound && reportMissing) {
         
         exotic::utils::Warning("midas::Event::GetBankPointer<T>", __FILE__, __LINE__)
             << "Couldn't find the MIDAS bank \"" << name << "\". Skipping...\n";
      }
      if (bkfound && checkType) {
         switch (type) {
         case 1:
            assert(typeid(T) == typeid(unsigned char));
            break; // TID_BYTE   1
         case 2:
            assert(typeid(T) == typeid(char));
            break; // TID_SBYTE  2
         case 3:
            assert(typeid(T) == typeid(unsigned char));
            break; // TID_CHAR   3
         case 4:
            assert(typeid(T) == typeid(uint16_t));
            break; // TID_WORD   4
         case 5:
            assert(typeid(T) == typeid(int16_t));
            break; // TID_SHORT  5
         case 6:
            assert(typeid(T) == typeid(uint32_t));
            break; // TID_DWORD  6
         case 7:
            assert(typeid(T) == typeid(int32_t));
            break; // TID_INT    7
         case 8:
            assert(typeid(T) == typeid(bool));
            break; // TID_BOOL   8
         case 9:
            assert(typeid(T) == typeid(float));
            break; // TID_FLOAT  9
         case 10:
            assert(typeid(T) == typeid(double));
            break; // TID_DOUBLE 10
         default:
            fprintf(stderr, "Unknown type id: %i\n", type);
            assert(false);
            break;
         }
      }
      return bkfound ? reinterpret_cast<T *>(pbk) : 0;
   }

 private:
   /// Helper function for copy constructor / assignment operator
   void CopyDerived(const Event &other);

   /// Helper function for constructors
   void Init(const char *tsbank, const void *header, const void *addr, int size);

 public:
   /// Class to compare by event id
   struct CompareId {
      /// Returns true if event id of lhs is less-than event id of rhs
      bool operator()(const Event &lhs, const Event &rhs) const { return lhs.GetEventId() < rhs.GetEventId(); }
   };

   /// Class to compare by serial number
   struct CompareSerial {
      /// Returns true if serial number of lhs is less-than serial number of rhs
      bool operator()(const Event &lhs, const Event &rhs) const { return lhs.GetSerialNumber() < rhs.GetSerialNumber(); }
   };
};

} // namespace midas

#endif // #ifndef MIDAS_EVENT_HXX