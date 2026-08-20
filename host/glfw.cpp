#ifdef LIBRW_GLFW

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include <rw.h>
#include "host.h"

using namespace host;
using namespace rw;

#ifdef RW_OPENGL

GLFWwindow *window;
int keymap[GLFW_KEY_LAST+1];

static void
initkeymap(void)
{
	int i;
	for(i = 0; i < GLFW_KEY_LAST+1; i++)
		keymap[i] = KEY_NULL;
	keymap[GLFW_KEY_SPACE] = ' ';
	keymap[GLFW_KEY_APOSTROPHE] = '\'';
	keymap[GLFW_KEY_COMMA] = ',';
	keymap[GLFW_KEY_MINUS] = '-';
	keymap[GLFW_KEY_PERIOD] = '.';
	keymap[GLFW_KEY_SLASH] = '/';
	keymap[GLFW_KEY_0] = '0';
	keymap[GLFW_KEY_1] = '1';
	keymap[GLFW_KEY_2] = '2';
	keymap[GLFW_KEY_3] = '3';
	keymap[GLFW_KEY_4] = '4';
	keymap[GLFW_KEY_5] = '5';
	keymap[GLFW_KEY_6] = '6';
	keymap[GLFW_KEY_7] = '7';
	keymap[GLFW_KEY_8] = '8';
	keymap[GLFW_KEY_9] = '9';
	keymap[GLFW_KEY_SEMICOLON] = ';';
	keymap[GLFW_KEY_EQUAL] = '=';
	keymap[GLFW_KEY_A] = 'A';
	keymap[GLFW_KEY_B] = 'B';
	keymap[GLFW_KEY_C] = 'C';
	keymap[GLFW_KEY_D] = 'D';
	keymap[GLFW_KEY_E] = 'E';
	keymap[GLFW_KEY_F] = 'F';
	keymap[GLFW_KEY_G] = 'G';
	keymap[GLFW_KEY_H] = 'H';
	keymap[GLFW_KEY_I] = 'I';
	keymap[GLFW_KEY_J] = 'J';
	keymap[GLFW_KEY_K] = 'K';
	keymap[GLFW_KEY_L] = 'L';
	keymap[GLFW_KEY_M] = 'M';
	keymap[GLFW_KEY_N] = 'N';
	keymap[GLFW_KEY_O] = 'O';
	keymap[GLFW_KEY_P] = 'P';
	keymap[GLFW_KEY_Q] = 'Q';
	keymap[GLFW_KEY_R] = 'R';
	keymap[GLFW_KEY_S] = 'S';
	keymap[GLFW_KEY_T] = 'T';
	keymap[GLFW_KEY_U] = 'U';
	keymap[GLFW_KEY_V] = 'V';
	keymap[GLFW_KEY_W] = 'W';
	keymap[GLFW_KEY_X] = 'X';
	keymap[GLFW_KEY_Y] = 'Y';
	keymap[GLFW_KEY_Z] = 'Z';
	keymap[GLFW_KEY_LEFT_BRACKET] = '[';
	keymap[GLFW_KEY_BACKSLASH] = '\\';
	keymap[GLFW_KEY_RIGHT_BRACKET] = ']';
	keymap[GLFW_KEY_GRAVE_ACCENT] = '`';
	keymap[GLFW_KEY_ESCAPE] = KEY_ESC;
	keymap[GLFW_KEY_ENTER] = KEY_ENTER;
	keymap[GLFW_KEY_TAB] = KEY_TAB;
	keymap[GLFW_KEY_BACKSPACE] = KEY_BACKSP;
	keymap[GLFW_KEY_INSERT] = KEY_INS;
	keymap[GLFW_KEY_DELETE] = KEY_DEL;
	keymap[GLFW_KEY_RIGHT] = KEY_RIGHT;
	keymap[GLFW_KEY_LEFT] = KEY_LEFT;
	keymap[GLFW_KEY_DOWN] = KEY_DOWN;
	keymap[GLFW_KEY_UP] = KEY_UP;
	keymap[GLFW_KEY_PAGE_UP] = KEY_PGUP;
	keymap[GLFW_KEY_PAGE_DOWN] = KEY_PGDN;
	keymap[GLFW_KEY_HOME] = KEY_HOME;
	keymap[GLFW_KEY_END] = KEY_END;
	keymap[GLFW_KEY_CAPS_LOCK] = KEY_CAPSLK;
	keymap[GLFW_KEY_SCROLL_LOCK] = KEY_NULL;
	keymap[GLFW_KEY_NUM_LOCK] = KEY_NULL;
	keymap[GLFW_KEY_PRINT_SCREEN] = KEY_NULL;
	keymap[GLFW_KEY_PAUSE] = KEY_NULL;

	keymap[GLFW_KEY_F1] = KEY_F1;
	keymap[GLFW_KEY_F2] = KEY_F2;
	keymap[GLFW_KEY_F3] = KEY_F3;
	keymap[GLFW_KEY_F4] = KEY_F4;
	keymap[GLFW_KEY_F5] = KEY_F5;
	keymap[GLFW_KEY_F6] = KEY_F6;
	keymap[GLFW_KEY_F7] = KEY_F7;
	keymap[GLFW_KEY_F8] = KEY_F8;
	keymap[GLFW_KEY_F9] = KEY_F9;
	keymap[GLFW_KEY_F10] = KEY_F10;
	keymap[GLFW_KEY_F11] = KEY_F11;
	keymap[GLFW_KEY_F12] = KEY_F12;
	keymap[GLFW_KEY_F13] = KEY_NULL;
	keymap[GLFW_KEY_F14] = KEY_NULL;
	keymap[GLFW_KEY_F15] = KEY_NULL;
	keymap[GLFW_KEY_F16] = KEY_NULL;
	keymap[GLFW_KEY_F17] = KEY_NULL;
	keymap[GLFW_KEY_F18] = KEY_NULL;
	keymap[GLFW_KEY_F19] = KEY_NULL;
	keymap[GLFW_KEY_F20] = KEY_NULL;
	keymap[GLFW_KEY_F21] = KEY_NULL;
	keymap[GLFW_KEY_F22] = KEY_NULL;
	keymap[GLFW_KEY_F23] = KEY_NULL;
	keymap[GLFW_KEY_F24] = KEY_NULL;
	keymap[GLFW_KEY_F25] = KEY_NULL;
	keymap[GLFW_KEY_KP_0] = KEY_NULL;
	keymap[GLFW_KEY_KP_1] = KEY_NULL;
	keymap[GLFW_KEY_KP_2] = KEY_NULL;
	keymap[GLFW_KEY_KP_3] = KEY_NULL;
	keymap[GLFW_KEY_KP_4] = KEY_NULL;
	keymap[GLFW_KEY_KP_5] = KEY_NULL;
	keymap[GLFW_KEY_KP_6] = KEY_NULL;
	keymap[GLFW_KEY_KP_7] = KEY_NULL;
	keymap[GLFW_KEY_KP_8] = KEY_NULL;
	keymap[GLFW_KEY_KP_9] = KEY_NULL;
	keymap[GLFW_KEY_KP_DECIMAL] = KEY_NULL;
	keymap[GLFW_KEY_KP_DIVIDE] = KEY_NULL;
	keymap[GLFW_KEY_KP_MULTIPLY] = KEY_NULL;
	keymap[GLFW_KEY_KP_SUBTRACT] = KEY_NULL;
	keymap[GLFW_KEY_KP_ADD] = KEY_NULL;
	keymap[GLFW_KEY_KP_ENTER] = KEY_NULL;
	keymap[GLFW_KEY_KP_EQUAL] = KEY_NULL;
	keymap[GLFW_KEY_LEFT_SHIFT] = KEY_LSHIFT;
	keymap[GLFW_KEY_LEFT_CONTROL] = KEY_LCTRL;
	keymap[GLFW_KEY_LEFT_ALT] = KEY_LALT;
	keymap[GLFW_KEY_LEFT_SUPER] = KEY_NULL;
	keymap[GLFW_KEY_RIGHT_SHIFT] = KEY_RSHIFT;
	keymap[GLFW_KEY_RIGHT_CONTROL] = KEY_RCTRL;
	keymap[GLFW_KEY_RIGHT_ALT] = KEY_RALT;
	keymap[GLFW_KEY_RIGHT_SUPER] = KEY_NULL;
	keymap[GLFW_KEY_MENU] = KEY_NULL;
}

static void KeyUp(int key) { if(callbacks.keyUp) callbacks.keyUp(key); }
static void KeyDown(int key) { if(callbacks.keyDown) callbacks.keyDown(key); }

static void
keypress(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key >= 0 && key <= GLFW_KEY_LAST){
		if(action == GLFW_RELEASE) KeyUp(keymap[key]);
		else if(action == GLFW_PRESS)   KeyDown(keymap[key]);
		else if(action == GLFW_REPEAT)  KeyDown(keymap[key]);
	}
}

static void
charinput(GLFWwindow *window, unsigned int c)
{
	if(callbacks.charInput) callbacks.charInput(c);
}

static void
resize(GLFWwindow *window, int w, int h)
{
	rw::Rect r;
	r.x = 0;
	r.y = 0;
	r.w = w;
	r.h = h;
	if(callbacks.resize) callbacks.resize(&r);
}

static void
mousemove(GLFWwindow *window, double x, double y)
{
	host::MouseState ms;
	ms.posx = x;
	ms.posy = y;
	if(callbacks.mouseMove) callbacks.mouseMove(&ms);
}

static void
mousebtn(GLFWwindow *window, int button, int action, int mods)
{
	static int buttons = 0;
	host::MouseState ms;

	switch(button){
	case GLFW_MOUSE_BUTTON_LEFT:
		if(action == GLFW_PRESS)
			buttons |= 1;
		else
			buttons &= ~1;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		if(action == GLFW_PRESS)
			buttons |= 2;
		else
			buttons &= ~2;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if(action == GLFW_PRESS)
			buttons |= 4;
		else
			buttons &= ~4;
		break;
	}

	ms.buttons = buttons;
	if(callbacks.mouseButton) callbacks.mouseButton(&ms);
}

static void
mousewheel(GLFWwindow *window, double x, double y)
{
	host::MouseState ms;
	ms.wheelDelta = y;
	if(callbacks.mouseWheel) callbacks.mouseWheel(&ms);
}

/* Window and GL context creation. This moved out of librw's gl3device.cpp:
 * context attributes are window-creation hints, so choosing a profile and
 * creating the window are one operation and cannot be split across the
 * boundary. */

static void
glfwerr(int error, const char *desc)
{
	fprintf(stderr, "GLFW Error: %s\n", desc);
}

static void *procThunk(const char *name)   { return (void*)glfwGetProcAddress(name); }
static void  swapThunk(void *w)            { glfwSwapBuffers((GLFWwindow*)w); }
static void  intervalThunk(void *w, int i) { glfwSwapInterval(i); }
static void  sizeThunk(void *w, int *ww, int *hh)
	{ glfwGetFramebufferSize((GLFWwindow*)w, ww, hh); }

/* ---- display topology ---------------------------------------------------
 *
 * librw cannot enumerate monitors through OpenGL, so it forwards its
 * Engine::get*VideoMode / get*SubSystem calls here. This is the code that
 * used to live in gl3device.cpp, once per windowing library. */

static GLFWmonitor *monitor;
static int   currentMonitor;
static int   numMonitors;
static int   currentMode;

/* Modes for the selected monitor, deduplicated by resolution keeping the
 * highest refresh rate, with entry 0 the monitor's current mode (windowed). */
static rw::VideoMode *modes;
static int numModes;

static int
modeDepth(const GLFWvidmode *m)
{
	int bits = m->redBits + m->greenBits + m->blueBits;
	int depth;
	for(depth = 1; depth < bits; depth <<= 1)
		;
	return depth;
}

static void
buildModeList(void)
{
	int i, n;
	const GLFWvidmode *vm = glfwGetVideoModes(monitor, &n);

	/* Plain malloc, not rwNewT: the host runs before rw::Engine::init(), so
	 * Engine::memfuncs is still null here and rwFree/rwNewT would call
	 * through a null pointer. The host is not a librw allocation client
	 * anyway -- its own bookkeeping has no business in librw's heap. */
	free(modes);
	modes = (rw::VideoMode*)malloc((n+1) * sizeof(rw::VideoMode));
	if(modes == nil){
		numModes = 0;
		return;
	}

	/* entry 0: current mode, windowed */
	const GLFWvidmode *cur = glfwGetVideoMode(monitor);
	modes[0].width  = cur->width;
	modes[0].height = cur->height;
	modes[0].depth  = modeDepth(cur);
	modes[0].flags  = 0;
	numModes = 1;

	for(i = 0; i < n; i++){
		int j;
		for(j = 1; j < numModes; j++)
			if(modes[j].width == vm[i].width &&
			   modes[j].height == vm[i].height &&
			   modes[j].depth == modeDepth(&vm[i]))
				break;
		if(j < numModes)
			continue;	/* already have this resolution */
		modes[numModes].width  = vm[i].width;
		modes[numModes].height = vm[i].height;
		modes[numModes].depth  = modeDepth(&vm[i]);
		modes[numModes].flags  = rw::VIDEOMODEEXCLUSIVE;
		numModes++;
	}
}

static int32  topoNumDisplays(void)      { return numMonitors; }
static int32  topoCurrentDisplay(void)   { return currentMonitor; }
static int32  topoNumVideoModes(void)    { return numModes; }
static int32  topoCurrentVideoMode(void) { return currentMode; }

static rw::bool32
topoSetDisplay(int32 n)
{
	GLFWmonitor **mons = glfwGetMonitors(&numMonitors);
	if(n < 0 || n >= numMonitors)
		return 0;
	currentMonitor = n;
	monitor = mons[n];
	buildModeList();
	currentMode = 0;
	return 1;
}

static rw::bool32
topoDisplayName(int32 n, char *buf, int32 buflen)
{
	GLFWmonitor **mons = glfwGetMonitors(&numMonitors);
	if(n < 0 || n >= numMonitors)
		return 0;
	strncpy(buf, glfwGetMonitorName(mons[n]), buflen-1);
	buf[buflen-1] = '\0';
	return 1;
}

static rw::bool32
topoSetVideoMode(int32 n)
{
	if(n < 0 || n >= numModes)
		return 0;
	currentMode = n;
	/* Applying it means recreating the window; not done yet, so this only
	 * records the selection. */
	return 1;
}

static rw::bool32
topoVideoModeInfo(int32 n, rw::VideoMode *out)
{
	if(n < 0 || n >= numModes)
		return 0;
	*out = modes[n];
	return 1;
}

static int32
topoMaxMultiSamplingLevels(void)
{
	GLint maxSamples = 0;
	glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
	return maxSamples == 0 ? 1 : maxSamples;
}

static int32     topoMultiSamplingLevels(void) { return host::config.numSamples; }
static rw::bool32 topoSetMultiSamplingLevels(int32 n)
{
	/* The context already exists by the time librw can ask, and MSAA is a
	 * window-creation hint, so only a no-op change can succeed. */
	return n == host::config.numSamples;
}

static const rw::DisplayTopology topology = {
	topoNumDisplays,
	topoCurrentDisplay,
	topoSetDisplay,
	topoDisplayName,

	topoNumVideoModes,
	topoCurrentVideoMode,
	topoSetVideoMode,
	topoVideoModeInfo,

	topoMaxMultiSamplingLevels,
	topoMultiSamplingLevels,
	topoSetMultiSamplingLevels,
};

static bool
createSurface(void)
{
	static const struct { int api, major, minor; } profiles[] = {
		{ GLFW_OPENGL_API,    3, 3 },
		{ GLFW_OPENGL_API,    2, 1 },
		{ GLFW_OPENGL_ES_API, 3, 1 },
		{ GLFW_OPENGL_ES_API, 2, 0 },
		{ 0, 0, 0 },
	};

	if(!glfwInit()){
		fprintf(stderr, "glfwInit() failed\n");
		return false;
	}
	glfwSetErrorCallback(glfwerr);

	if(!topoSetDisplay(0)){
		fprintf(stderr, "no monitor found\n");
		return false;
	}

	/* GLX rounds 1 sample up to 2x or 4x, so only hint when we mean it. */
	if(host::config.numSamples > 1)
		glfwWindowHint(GLFW_SAMPLES, host::config.numSamples);

	int i;
	for(i = 0; profiles[i].api; i++){
		glfwWindowHint(GLFW_CLIENT_API, profiles[i].api);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, profiles[i].major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, profiles[i].minor);
		window = glfwCreateWindow(host::config.width, host::config.height,
			host::config.title, nil, nil);
		if(window)
			break;
	}
	if(window == nil){
		fprintf(stderr, "glfwCreateWindow() failed\n");
		return false;
	}
	glfwMakeContextCurrent(window);

	engineOpenParams.window      = window;
	engineOpenParams.glcontext   = nil;	/* glfw keeps it internal */
	engineOpenParams.gles        = profiles[i].api == GLFW_OPENGL_ES_API;
	engineOpenParams.glversion   = profiles[i].major*10 + profiles[i].minor;
	engineOpenParams.getProc     = procThunk;
	engineOpenParams.swapBuffers = swapThunk;
	engineOpenParams.setSwapInterval   = intervalThunk;
	engineOpenParams.getFramebufferSize = sizeThunk;
	engineOpenParams.topology    = &topology;
	engineOpenParams.width       = host::config.width;
	engineOpenParams.height      = host::config.height;
	engineOpenParams.windowtitle = host::config.title;
	return true;
}

int
main(int argc, char *argv[])
{
	if(callbacks.initialize && !callbacks.initialize(argc, argv))
		return 0;

	if(!createSurface())
		return 0;

	if(callbacks.rwInitialize && !callbacks.rwInitialize())
		return 0;

	initkeymap();
	glfwSetKeyCallback(window, keypress);
	glfwSetCharCallback(window, charinput);
	glfwSetWindowSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mousemove);
	glfwSetMouseButtonCallback(window, mousebtn);
	glfwSetScrollCallback(window, mousewheel);

	float lastTime = glfwGetTime()*1000;
	while(!host::quitting() && !glfwWindowShouldClose(window)){
		float currTime  = glfwGetTime()*1000;
		float timeDelta = (currTime - lastTime)*0.001f;
		glfwPollEvents();

		if(callbacks.idle) callbacks.idle(timeDelta);

		lastTime = currTime;
	}

	if(callbacks.rwTerminate) callbacks.rwTerminate();

	glfwDestroyWindow(window);
	glfwTerminate();
	free(modes);
	modes = nil;

	return 0;
}

namespace host {

void
setMousePosition(int x, int y)
{
	glfwSetCursorPos(window, (double)x, (double)y);
}

}

#endif
#endif
