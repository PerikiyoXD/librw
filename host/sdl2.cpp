#ifdef LIBRW_SDL2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <rw.h>
#include "host.h"

using namespace host;
using namespace rw;

#ifdef RW_OPENGL

SDL_Window *window;

static int keyCodeToSkKey(SDL_Keycode keycode) {
	switch (keycode) {
	case SDLK_SPACE: return ' ';
	case SDLK_QUOTE: return '\'';
	case SDLK_COMMA: return ',';
	case SDLK_MINUS: return '-';
	case SDLK_PERIOD: return '.';
	case SDLK_SLASH: return '/';

	case SDLK_0: return '0';
	case SDLK_1: return '1';
	case SDLK_2: return '2';
	case SDLK_3: return '3';
	case SDLK_4: return '4';
	case SDLK_5: return '5';
	case SDLK_6: return '6';
	case SDLK_7: return '7';
	case SDLK_8: return '8';
	case SDLK_9: return '9';

	case SDLK_SEMICOLON: return ';';
	case SDLK_EQUALS: return '=';

	case SDLK_a: return 'A';
	case SDLK_b: return 'B';
	case SDLK_c: return 'C';
	case SDLK_d: return 'D';
	case SDLK_e: return 'E';
	case SDLK_f: return 'F';
	case SDLK_g: return 'G';
	case SDLK_h: return 'H';
	case SDLK_i: return 'I';
	case SDLK_j: return 'J';
	case SDLK_k: return 'K';
	case SDLK_l: return 'L';
	case SDLK_m: return 'M';
	case SDLK_n: return 'N';
	case SDLK_o: return 'O';
	case SDLK_p: return 'P';
	case SDLK_q: return 'Q';
	case SDLK_r: return 'R';
	case SDLK_s: return 'S';
	case SDLK_t: return 'T';
	case SDLK_u: return 'U';
	case SDLK_v: return 'V';
	case SDLK_w: return 'W';
	case SDLK_x: return 'X';
	case SDLK_y: return 'Y';
	case SDLK_z: return 'Z';

	case SDLK_LEFTBRACKET: return '[';
	case SDLK_BACKSLASH: return '\\';
	case SDLK_RIGHTBRACKET: return ']';
	case SDLK_BACKQUOTE: return '`';
	case SDLK_ESCAPE: return KEY_ESC;
	case SDLK_RETURN: return KEY_ENTER;
	case SDLK_TAB: return KEY_TAB;
	case SDLK_BACKSPACE: return KEY_BACKSP;
	case SDLK_INSERT: return KEY_INS;
	case SDLK_DELETE: return KEY_DEL;
	case SDLK_RIGHT: return KEY_RIGHT;
	case SDLK_LEFT: return KEY_LEFT;
	case SDLK_DOWN: return KEY_DOWN;
	case SDLK_UP: return KEY_UP;
	case SDLK_PAGEUP: return KEY_PGUP;
	case SDLK_PAGEDOWN: return KEY_PGDN;
	case SDLK_HOME: return KEY_HOME;
	case SDLK_END: return KEY_END;
	case SDLK_CAPSLOCK: return KEY_CAPSLK;
	case SDLK_SCROLLLOCK: return KEY_NULL;
	case SDLK_NUMLOCKCLEAR: return KEY_NULL;
	case SDLK_PRINTSCREEN: return KEY_NULL;
	case SDLK_PAUSE: return KEY_NULL;

	case SDLK_F1: return KEY_F1;
	case SDLK_F2: return KEY_F2;
	case SDLK_F3: return KEY_F3;
	case SDLK_F4: return KEY_F4;
	case SDLK_F5: return KEY_F5;
	case SDLK_F6: return KEY_F6;
	case SDLK_F7: return KEY_F7;
	case SDLK_F8: return KEY_F8;
	case SDLK_F9: return KEY_F9;
	case SDLK_F10: return KEY_F10;
	case SDLK_F11: return KEY_F11;
	case SDLK_F12: return KEY_F12;
	case SDLK_F13: return KEY_NULL;
	case SDLK_F14: return KEY_NULL;
	case SDLK_F15: return KEY_NULL;
	case SDLK_F16: return KEY_NULL;
	case SDLK_F17: return KEY_NULL;
	case SDLK_F18: return KEY_NULL;
	case SDLK_F19: return KEY_NULL;
	case SDLK_F20: return KEY_NULL;
	case SDLK_F21: return KEY_NULL;
	case SDLK_F22: return KEY_NULL;
	case SDLK_F23: return KEY_NULL;
	case SDLK_F24: return KEY_NULL;

	case SDLK_KP_0: return KEY_NULL;
	case SDLK_KP_1: return KEY_NULL;
	case SDLK_KP_2: return KEY_NULL;
	case SDLK_KP_3: return KEY_NULL;
	case SDLK_KP_4: return KEY_NULL;
	case SDLK_KP_5: return KEY_NULL;
	case SDLK_KP_6: return KEY_NULL;
	case SDLK_KP_7: return KEY_NULL;
	case SDLK_KP_8: return KEY_NULL;
	case SDLK_KP_9: return KEY_NULL;
	case SDLK_KP_DECIMAL: return KEY_NULL;
	case SDLK_KP_DIVIDE: return KEY_NULL;
	case SDLK_KP_MULTIPLY: return KEY_NULL;
	case SDLK_KP_MINUS: return KEY_NULL;
	case SDLK_KP_PLUS: return KEY_NULL;
	case SDLK_KP_ENTER: return KEY_NULL;
	case SDLK_KP_EQUALS: return KEY_NULL;

	case SDLK_LSHIFT: return KEY_LSHIFT;
	case SDLK_LCTRL: return KEY_LCTRL;
	case SDLK_LALT: return KEY_LALT;
	case SDLK_LGUI: return KEY_NULL;
	case SDLK_RSHIFT: return KEY_RSHIFT;
	case SDLK_RCTRL: return KEY_RCTRL;
	case SDLK_RALT: return KEY_RALT;
	case SDLK_RGUI: return KEY_NULL;
	case SDLK_MENU: return KEY_NULL;
	}
	return KEY_NULL;
}

#if 0
static void
keypress(SDL_Window *window, int key, int scancode, int action, int mods)
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

	host::MouseState ms;
	ms.buttons = buttons;
	if(callbacks.mouseButton) callbacks.mouseButton(&ms);
}
#endif

enum mousebutton {
BUTTON_LEFT = 0x1,
BUTTON_MIDDLE = 0x2,
BUTTON_RIGHT = 0x4,
};

/* Window and GL context creation, moved out of librw's gl3device.cpp:
 * context attributes are window-creation hints, so profile selection and
 * window creation are one operation. */

static SDL_GLContext glcontext;

static void *procThunk(const char *name)   { return (void*)SDL_GL_GetProcAddress(name); }
static void  swapThunk(void *w)            { SDL_GL_SwapWindow((SDL_Window*)w); }
static void  intervalThunk(void *w, int i) { SDL_GL_SetSwapInterval(i); }
static void  sizeThunk(void *w, int *ww, int *hh)
	{ SDL_GetWindowSize((SDL_Window*)w, ww, hh); }

/* ---- display topology ---------------------------------------------------
 *
 * librw cannot enumerate monitors through OpenGL, so it forwards its
 * Engine::get*VideoMode / get*SubSystem calls here. This is the code that
 * used to live in gl3device.cpp, once per windowing library.
 *
 * malloc, not rwNewT: this runs before rw::Engine::init(). See the lifecycle
 * rule in host.h. */

static int currentDisplay;
static int numDisplays;
static int currentMode;

static rw::VideoMode *modes;
static int numModes;

static int
modeDepth(const SDL_DisplayMode *m)
{
	int bits = SDL_BITSPERPIXEL(m->format);
	int depth;
	for(depth = 1; depth < bits; depth <<= 1)
		;
	return depth;
}

static void
buildModeList(void)
{
	SDL_DisplayMode m;
	int i, n;

	n = SDL_GetNumDisplayModes(currentDisplay);
	if(n < 0)
		n = 0;

	free(modes);
	modes = (rw::VideoMode*)malloc((n+1) * sizeof(rw::VideoMode));
	if(modes == nil){
		numModes = 0;
		return;
	}

	/* entry 0: current mode, windowed */
	if(SDL_GetCurrentDisplayMode(currentDisplay, &m) == 0){
		modes[0].width  = m.w;
		modes[0].height = m.h;
		modes[0].depth  = modeDepth(&m);
		modes[0].flags  = 0;
		numModes = 1;
	}else
		numModes = 0;

	for(i = 0; i < n; i++){
		int j;
		if(SDL_GetDisplayMode(currentDisplay, i, &m) != 0)
			continue;
		for(j = 1; j < numModes; j++)
			if(modes[j].width == m.w && modes[j].height == m.h &&
			   modes[j].depth == modeDepth(&m))
				break;
		if(j < numModes)
			continue;	/* already have this resolution */
		modes[numModes].width  = m.w;
		modes[numModes].height = m.h;
		modes[numModes].depth  = modeDepth(&m);
		modes[numModes].flags  = rw::VIDEOMODEEXCLUSIVE;
		numModes++;
	}
}

static int32 topoNumDisplays(void)      { return numDisplays; }
static int32 topoCurrentDisplay(void)   { return currentDisplay; }
static int32 topoNumVideoModes(void)    { return numModes; }
static int32 topoCurrentVideoMode(void) { return currentMode; }

static rw::bool32
topoSetDisplay(int32 n)
{
	numDisplays = SDL_GetNumVideoDisplays();
	if(n < 0 || n >= numDisplays)
		return 0;
	currentDisplay = n;
	buildModeList();
	currentMode = 0;
	return 1;
}

static rw::bool32
topoDisplayName(int32 n, char *buf, int32 buflen)
{
	const char *name;
	if(n < 0 || n >= SDL_GetNumVideoDisplays())
		return 0;
	name = SDL_GetDisplayName(n);
	if(name == nil)
		return 0;
	strncpy(buf, name, buflen-1);
	buf[buflen-1] = '\0';
	return 1;
}

static rw::bool32
topoSetVideoMode(int32 n)
{
	if(n < 0 || n >= numModes)
		return 0;
	currentMode = n;
	/* Applying it means recreating the window; only the selection is
	 * recorded for now. */
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

static int32      topoMultiSamplingLevels(void) { return host::config.numSamples; }
static rw::bool32 topoSetMultiSamplingLevels(int32 n)
{
	/* MSAA is a context-creation attribute and the context already exists
	 * by the time librw can ask, so only a no-op change can succeed. */
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
	static const struct { int profile, major, minor; } profiles[] = {
		{ SDL_GL_CONTEXT_PROFILE_CORE, 3, 3 },
		{ SDL_GL_CONTEXT_PROFILE_CORE, 2, 1 },
		{ SDL_GL_CONTEXT_PROFILE_ES,   3, 1 },
		{ SDL_GL_CONTEXT_PROFILE_ES,   2, 0 },
		{ 0, 0, 0 },
	};

	if(!(SDL_InitSubSystem(SDL_INIT_VIDEO) == 0)){
		fprintf(stderr, "SDL_InitSubSystem: %s\n", SDL_GetError());
		return false;
	}

	if(!topoSetDisplay(0)){
		fprintf(stderr, "no display found: %s\n", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, host::config.numSamples);

	int i;
	for(i = 0; profiles[i].profile; i++){
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profiles[i].profile);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, profiles[i].major);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, profiles[i].minor);
		window = SDL_CreateWindow(host::config.title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, host::config.width, host::config.height, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
		if(window)
			break;
	}
	if(window == nil){
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		return false;
	}

	glcontext = SDL_GL_CreateContext(window);
	if(glcontext == nil){
		fprintf(stderr, "SDL_GL_CreateContext: %s\n", SDL_GetError());
		return false;
	}

	engineOpenParams.window      = window;
	engineOpenParams.glcontext   = glcontext;
	engineOpenParams.gles        = profiles[i].profile == SDL_GL_CONTEXT_PROFILE_ES;
	engineOpenParams.glversion   = profiles[i].major*10 + profiles[i].minor;
	engineOpenParams.getProc     = procThunk;
	engineOpenParams.swapBuffers = swapThunk;
	engineOpenParams.setSwapInterval    = intervalThunk;
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

	Uint64 lastTicks = SDL_GetPerformanceCounter();
	const float tickPeriod = 1.f / SDL_GetPerformanceFrequency();
	SDL_Event event;
	int mouseButtons = 0;

	SDL_StartTextInput();

	while(!host::quitting()){
		while(SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_QUIT:
				host::requestQuit();
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
				case SDL_WINDOWEVENT_RESIZED: {
					rw::Rect r;
					SDL_GetWindowPosition(window, &r.x, &r.y);
					r.w = event.window.data1;
					r.h = event.window.data2;
					if(callbacks.resize) callbacks.resize(&r);
					break;
				}
				}
				break;
			case SDL_KEYUP: {
				int c = keyCodeToSkKey(event.key.keysym.sym);
				if(callbacks.keyUp) callbacks.keyUp(c);
				break;
			}
			case SDL_KEYDOWN: {
				int c = keyCodeToSkKey(event.key.keysym.sym);
				if(callbacks.keyDown) callbacks.keyDown(c);
				break;
			}
			case SDL_TEXTINPUT: {
				char *c = event.text.text;
				while (int ci = *c) {
					if(callbacks.charInput) callbacks.charInput(ci);
					++c;
				}
				break;
			}
			case SDL_MOUSEMOTION: {
				host::MouseState ms;
				ms.posx = event.motion.x;
				ms.posy = event.motion.y;
				if(callbacks.mouseMove) callbacks.mouseMove(&ms);
				break;
			}
			case SDL_MOUSEBUTTONDOWN: {
				switch (event.button.button) {
				case SDL_BUTTON_LEFT: mouseButtons |= BUTTON_LEFT; break;
				case SDL_BUTTON_MIDDLE: mouseButtons |= BUTTON_MIDDLE; break;
				case SDL_BUTTON_RIGHT: mouseButtons |= BUTTON_RIGHT; break;
				}
				host::MouseState ms;
				ms.buttons = mouseButtons;
				if(callbacks.mouseButton) callbacks.mouseButton(&ms);
				break;
			}
			case SDL_MOUSEBUTTONUP: {
				switch (event.button.button) {
				case SDL_BUTTON_LEFT: mouseButtons &= ~BUTTON_LEFT; break;
				case SDL_BUTTON_MIDDLE: mouseButtons &= ~BUTTON_MIDDLE; break;
				case SDL_BUTTON_RIGHT: mouseButtons &= ~BUTTON_RIGHT; break;
				}
				host::MouseState ms;
				ms.buttons = mouseButtons;
				if(callbacks.mouseButton) callbacks.mouseButton(&ms);
				break;
			}
			}
		}
		Uint64 currTicks = SDL_GetPerformanceCounter();
		float timeDelta = (currTicks - lastTicks) * tickPeriod;

		if(callbacks.idle) callbacks.idle(timeDelta);

		lastTicks = currTicks;
	}

	SDL_StopTextInput();

	if(callbacks.rwTerminate) callbacks.rwTerminate();

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	free(modes);
	modes = nil;

	return 0;
}

namespace host {

void
setMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(window, x, y);
}

}

#endif
#endif
