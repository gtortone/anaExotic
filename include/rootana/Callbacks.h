#ifndef ROOTANA_CALLBACKS_H
#define ROOTANA_CALLBACKS_H
#include "utils/ErrorExotic.h"
#include "midas/Event.h"
#include "Application.h"

// Declare callback functions as extern "C"
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

/// Run start transition handler.
EXTERN_C void rootana_run_start(int transition, int run, int time);

/// Run stop transition handler
EXTERN_C void rootana_run_stop(int transition, int run, int time);

/// Run pause transition handler
EXTERN_C void rootana_run_pause(int transition, int run, int time);

/// Run resume transition handler
EXTERN_C void rootana_run_resume(int transition, int run, int time);

/// Event handler callback function
EXTERN_C void rootana_handle_event(const void* pheader, const void* pdata, int size);


// Inlined implementations

inline void rootana_run_start(int transition, int run, int time)
{
	/*!
	 *  Calls rootana::App::run_start()
	 */
	rootana::App::instance()->run_start(run);
}

inline void rootana_run_stop(int transition, int run, int time)
{
	/*!
	 *  Calls rootana::App::run_stop()
	 */
	rootana::App::instance()->run_stop(run);
}

inline void rootana_run_pause(int transition, int run, int time)
{
	/*!
	 * Prints an information message.
	 */
	exotic::utils::Info("rootana") << "Pausing run " << run;
}

inline void rootana_run_resume(int transition, int run, int time)
{
	/*!
	 * Prints an information message.
	 */
	exotic::utils::Info("rootana") << "Resuming run " << run;
}


#endif
