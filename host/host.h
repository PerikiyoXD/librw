#ifndef RW_HOST_H
#define RW_HOST_H

/* The host layer.
 *
 * Boundary, stated once:
 *
 *   librw   Given a drawable surface, load, represent, convert and render
 *           RenderWare objects into it. Knows nothing about windows, input
 *           or the event loop. Never includes SDL or GLFW.
 *
 *   host    Owns the window, the GL context, the event loop, the input
 *           devices and the display topology. Knows nothing about RW
 *           objects. Includes SDL/GLFW; librw does not.
 *
 *   app     Everything else. The skeleton example harness is one app;
 *           a game is another. Supplies host::callbacks and uses librw.
 *
 * The test for any piece of code: does it still make sense with no window
 * on screen? If yes it belongs in librw, if no it belongs in host.
 *
 * The host owns the program entry point. That is forced, not chosen:
 * win32 needs WinMain and SDL2 redefines main to SDL_main. The app is
 * therefore entered through host::callbacks, not through its own main().
 */

namespace host {

/* Canonical key codes. The host maps native keycodes onto these, so the app
 * never sees a GLFW_KEY_* or SDLK_*. Values match the historical sk::Key so
 * existing app code keeps working.
 *
 * Codes below 128 are ASCII. */
enum Key
{
	KEY_ESC    = 128,

	KEY_F1     = 129,
	KEY_F2     = 130,
	KEY_F3     = 131,
	KEY_F4     = 132,
	KEY_F5     = 133,
	KEY_F6     = 134,
	KEY_F7     = 135,
	KEY_F8     = 136,
	KEY_F9     = 137,
	KEY_F10    = 138,
	KEY_F11    = 139,
	KEY_F12    = 140,

	KEY_INS    = 141,
	KEY_DEL    = 142,
	KEY_HOME   = 143,
	KEY_END    = 144,
	KEY_PGUP   = 145,
	KEY_PGDN   = 146,

	KEY_UP     = 147,
	KEY_DOWN   = 148,
	KEY_LEFT   = 149,
	KEY_RIGHT  = 150,

	KEY_BACKSP = 168,
	KEY_TAB    = 169,
	KEY_CAPSLK = 170,
	KEY_ENTER  = 171,
	KEY_LSHIFT = 172,
	KEY_RSHIFT = 173,
	KEY_LCTRL  = 174,
	KEY_RCTRL  = 175,
	KEY_LALT   = 176,
	KEY_RALT   = 177,

	KEY_NULL,	/* unmapped */
	KEY_NUMKEYS,
};

struct MouseState
{
	int   posx, posy;
	int   buttons;		/* bits 0-2: left, middle, right */
	float wheelDelta;
};

/* What the app asks the host to create. Chosen before the window exists,
 * because GL context attributes are window-creation hints -- you cannot
 * create a window and then decide you wanted multisampling or ES. */
struct Config
{
	const char *title;
	int         width, height;

	/* Initial window position. Hosts whose toolkit places the window for
	 * them use it only as the position to restore to when leaving
	 * fullscreen before the window has been moved. */
	int         x, y;

	int         numSamples;		/* MSAA; 1 = off */
	bool        fullscreen;
};

struct WindowRect
{
	int x, y, w, h;
};

enum DisplayModeFlags
{
	DISPLAYMODEEXCLUSIVE = 1
};

struct DisplayMode
{
	int width, height, depth;
	unsigned int flags;
};

/* Window-system data exposed to the application. This deliberately mirrors
 * only the drawable-surface part of rw::EngineOpenParams; host does not
 * include rw.h or traffic in RenderWare objects. */
struct Surface
{
	void *window;
	void *glcontext;
	bool gles;
	int glversion;
	void *(*getProc)(const char *name);
	void  (*swapBuffers)(void *window);
	void  (*setSwapInterval)(void *window, int interval);
	void  (*getFramebufferSize)(void *window, int *w, int *h);
	int width, height;
	int numSamples;
	bool fullscreen;
	const char *title;
};

extern Surface surface;

/* LIFECYCLE RULE, for host implementors:
 *
 *   Nothing in namespace rw may be USED before callbacks.rwInitialize()
 *   returns. Including rw.h is fine; calling into librw is not.
 *
 * The host runs first: it creates the window and the GL context before the
 * app gets rwInitialize() and calls rw::Engine::init(). Until that returns,
 * librw is uninitialised. rw::Engine::memfuncs holds null function pointers,
 * so rwNewT and rwFree call through null, and the allocation tracker is not
 * set up. Such a call compiles cleanly and faults intermittently at runtime.
 *
 * Use malloc/free for the host's own bookkeeping; the host is not a librw
 * allocation client.
 *
 * Order:
 *   callbacks.initialize()   no window, no librw
 *   create window + context  no librw
 *   callbacks.rwInitialize() librw becomes usable during this call
 *   ... event loop ...       librw usable
 *   callbacks.rwTerminate()  librw usable until this returns
 *   destroy window           no librw
 */

/* Everything the host delivers to the app. The app defines this object; the
 * host only declares it, so there is no registration ordering problem and no
 * chance of the loop running before the app is reachable.
 *
 * A null pointer means "not interested"; the host must check. */
struct Callbacks
{
	/* lifecycle, in call order */
	bool (*initialize)(int argc, char **argv);	/* before any window exists */
	bool (*rwInitialize)(void);			/* window ready; open librw */
	void (*rwTerminate)(void);

	/* polled each iteration; the app's own reason to stop */
	bool (*shouldQuit)(void);

	/* input */
	void (*keyDown)(int key);		/* host::Key or ASCII */
	void (*keyUp)(int key);
	void (*charInput)(int c);
	void (*mouseMove)(const MouseState *m);
	void (*mouseButton)(const MouseState *m);
	void (*mouseWheel)(const MouseState *m);

	/* Window client/drawable size. Coordinates are always client-relative. */
	void (*resize)(const WindowRect *r);

	/* Called after the native window changed presentation policy. GL hosts
	 * leave this null; the Win32/D3D9 host uses it to reset the swapchain. */
	bool (*presentationChanged)(int width, int height, bool fullscreen);

	/* per frame, seconds since previous idle */
	void (*idle)(float timeDelta);
};

extern Callbacks callbacks;		/* defined by the app */

/* Config the app wants. Defined by the app, and read by the host AFTER
 * callbacks.initialize() returns -- that is the app's chance to set it. */
extern Config config;

/* Ask the loop to exit after the current frame. */
void requestQuit(void);

void setMousePosition(int x, int y);

bool setFullscreen(bool enable);
bool isFullscreen(void);

/* Host internal, implemented in host.cpp: the event loop's stop condition. */
bool quitting(void);

}

#endif
