#include "host.h"

namespace host {

/* Defaults; the app overrides these from callbacks.initialize(). */
Config config = {
	"librw",	/* title      */
	1280, 800,	/* width, height */
	100, 100,	/* x, y       */
	1,		/* numSamples */
	false,		/* fullscreen */
};

Surface surface;

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
