#ifndef ROOTANA_GLOBALS_H
#define ROOTANA_GLOBALS_H

#include "utils/Exotic.h"
#include <vector>
#include <map>

#ifndef G__DICTIONARY
/// Provide 'extern' linkage except in CINT dictionary
#define EXTERN extern
#else
#define EXTERN
#endif

namespace rootana {

EXTERN std::vector<exotic::Dsssd *> DsssdList;
// EXTERN std::vector<exotic::Ppac *> PpacList;
// EXTERN std::vector<exotic::Si *> SiList;

EXTERN std::map<std::string, std::string> DetTable;

}


#endif // end
