#ifndef EXOTIC_MIDAS_DATABASE_H
#define EXOTIC_MIDAS_DATABASE_H
#include "midas/Odb.h"
#include "utils/ErrorExotic.h"
#include <memory>

#ifdef USE_ROOT
#include <RVersion.h>
#include <TNamed.h>

#if ROOT_VERSION_CODE < ROOT_VERSION(5, 30, 0)

// ROOT doesn't play nice with std::auto_ptr until version 5.30 or so.
// Workaround is to fake the auto ptr for those who have earlier versions.
// This will disable reading of midas::Database objects from files created
// w/ other versions, though.

#define MIDAS_XML_CLASS_VERSION 4
#else
#define MIDAS_XML_CLASS_VERSION 3
#endif

#endif // #ifdef USE_ROOT

namespace midas {

/// Generic database reading class
/*!
 * With this class, users can read data from either the ODB or from
 * a saved XML (or .mid) file using a single structure. Whether to read
 * from the ODB or a file is specified by the constructor argument, which
 * is either the path to a file containing XML data, or "online" to read
 * from the ODB.
 */
class Database
#ifdef USE_ROOT
    : public TNamed
#endif
{
 private:
   /// Flag specifying online mode
   bool fIsOnline;

   /// Flag specifying 'zombie' status
   bool fIsZombie;

 public:
   /// Default constructor for ROOT I/O
   Database() : fIsOnline(false), fIsZombie(false) {}

   /// Determines online or offline mode
   Database(const char *filename) : fIsOnline(false), fIsZombie(false) {
      /*!
			 * \param filename Name of the XML (or .mid) file from which to read
			 *  data. \note Specifying "online" reads from the ODB if connected to an
			 *  experiment.
			 */
      fIsOnline = true;
      if (Odb::GetHandle() == 0)
         fIsZombie = true;

      if (fIsZombie) {
         exotic::utils::Error("midas::Database::Database", __FILE__, __LINE__)
             << "Failed opening the database: \"" << filename << "\"";
      }
   }

   /// Tell the public if a zombie or not
   bool IsZombie() const { return fIsZombie; }

   /// Dump odb contents to an output stream
   void Dump(std::ostream &strm) const {
      if (fIsZombie)
         return;
      if (fIsOnline) {
         exotic::utils::Error("Database::Dump", __FILE__, __LINE__)
             << "Currently not supported for online database";
         return;
      }
   }

   /// Default dump to std::cout
   void Dump() const { Dump(std::cout); }

   /// Read a single value
   template <typename T>
   bool ReadValue(const char *path, T &value) const {
      /*!
			 * \param [in] path Path ODB directory path of what is to be read
			 * \param [out] array The value you want to read
			 * \tparam T The type of the data to be read
			 * \returns true if read was successful, false otherwise
			 */
      if (fIsZombie)
         return false;
      if (fIsOnline)
         return Odb::ReadValue(path, value);
      else
         return false;
   }

   /// Read the length of an array
   int ReadArrayLength(const char *path) const {
      /*!
			 * \param [in] path Path ODB directory path of what is to be read
			 * \returns The length of the array upon success, -1 upon failure.
			 */
      if (fIsZombie)
         return -1;
      else if (fIsOnline)
         return Odb::ReadArraySize(path);
      else
         return -1;
   }

   /// Read an array
   template <typename T>
   int ReadArray(const char *path, T *array, int length) const {
      /*!
			 * \param [in] path Path ODB directory path of what is to be read
			 * \param [out] array The value you want to read
			 * \param [in] length Length of the array to be read
			 * \tparam T The type of the data in the array to be read
			 * \returns The length of the array that was read (0 if error)
			 */
      if (fIsZombie)
         return 0;
      if (fIsOnline)
         return Odb::ReadArray(path, array, length);
      else
         return 0;
   }

   /// Print value of a parameter
   void Print(const char *path) const {
      if (fIsZombie) {
         std::cout << "Zombie!\n";
         return;
      }
      if (fIsOnline) {
         std::cout << "Print() not yet available for online data.\n";
         return;
      }
   }

   /// Check if a path exists
   bool CheckPath(const char *path) {
      if (fIsZombie)
         return false;
      if (fIsOnline) {
         int arrlen = midas::Odb::ReadArraySize(path);
         return (arrlen != -1);
      } else return false;
   }
};

} // namespace midas

#ifdef __MAKECINT__
#pragma link C++ function midas::Database::ReadValue(const char *, Char_t &);
#pragma link C++ function midas::Database::ReadValue(const char *, Short_t &);
#pragma link C++ function midas::Database::ReadValue(const char *, Int_t &);
#pragma link C++ function midas::Database::ReadValue(const char *, Long64_t &);
#pragma link C++ function midas::Database::ReadValue(const char *, UChar_t &);
#pragma link C++ function midas::Database::ReadValue(const char *, UShort_t &);
#pragma link C++ function midas::Database::ReadValue(const char *, UInt_t &);
#pragma link C++ function midas::Database::ReadValue(const char *, ULong64_t &);
#pragma link C++ function midas::Database::ReadValue(const char *, Float_t &);
#pragma link C++ function midas::Database::ReadValue(const char *, Double_t &);
#pragma link C++ function midas::Database::ReadValue(const char *, std::string &);

#pragma link C++ function midas::Database::ReadArray(const char *, Char_t *, int);
#pragma link C++ function midas::Database::ReadArray(const char *, Short_t *, int);
#pragma link C++ function midas::Database::ReadArray(const char *, Int_t *, int);
#pragma link C++ function midas::Database::ReadArray(const char *, Long64_t *, int);
#pragma link C++ function midas::Database::ReadArray(const char *, UChar_t *, int);
#pragma link C++ function midas::Database::ReadArray(const char *, UShort_t *, int);
#pragma link C++ function midas::Database::ReadArray(const char *, UInt_t *, int);
#pragma link C++ function midas::Database::ReadArray(const char *, ULong64_t *, int);
#pragma link C++ function midas::Database::ReadArray(const char *, Float_t *, int);
#pragma link C++ function midas::Database::ReadArray(const char *, Double_t *, int);
#pragma link C++ function midas::Database::ReadArray(const char *, std::string *, int);

#endif

#endif
