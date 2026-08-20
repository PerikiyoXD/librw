#include "host.h"

namespace host {

/* Defaults; the app overrides these from callbacks.initialize(). */
Config config = { "librw", 1280, 800, 1, false };

/* Phase A: still filled in by the host and read by the app's InitRW.
 * Goes away when librw receives a Surface instead. */
rw::EngineOpenParams engineOpenParams;

static bool quitRequested;

void
requestQuit(void)
{
	quitRequested = true;
}

/* Host internal: the loop's stop condition. Either the window system asked
 * to close, or the app says it is done. */
bool
quitting(void)
{
	if(quitRequested)
		return true;
	return callbacks.shouldQuit && callbacks.shouldQuit();
}

}
