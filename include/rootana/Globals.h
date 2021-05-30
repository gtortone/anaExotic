#ifndef ROOTANA_GLOBALS_H
#define ROOTANA_GLOBALS_H

#include <map>
#include <vector>
#include "rootana/DetectorContainer.h"

#ifndef G__DICTIONARY
/// Provide 'extern' linkage except in CINT dictionary
#define EXTERN extern
#else
#define EXTERN
#endif

//namespace rootana {

EXTERN DetectorContainer detList;

//} // namespace rootana


#endif // end
