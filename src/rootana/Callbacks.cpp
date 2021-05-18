#include "rootana/Callbacks.h"
#include "midas/Event.h"

void rootana_handle_event(const void* pheader, const void* pdata, int size)
{
	/*!
	 * Figure out the TSC bank name from event id, then pass on the work to
	 * rootana::App::handle_event().
	 */
	const midas::Event::Header* head = reinterpret_cast<const midas::Event::Header*>(pheader);

	midas::Event e(pheader, pdata, head->fDataSize);
	rootana::App::instance()->handle_event(e);

	std::cout << "rootana_handle_event: size = " << size << std::endl;
}

