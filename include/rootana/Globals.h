#ifndef ROOTANA_GLOBALS_H
#define ROOTANA_GLOBALS_H

#include "rootana/DetectorContainer.h"

#ifndef G__DICTIONARY
/// Provide 'extern' linkage except in CINT dictionary
#define EXTERN extern
#else
#define EXTERN
#endif

EXTERN DetectorContainer detList;

#endif // end
