#ifdef LIBRW_SDL2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <rw/librwconf.h>
#include "host.h"

using namespace host;

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
	host::WindowRect r;
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
	host::MouseState ms = {};

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

	host::MouseState ms = {};
	ms.buttons = buttons;
	if(callbacks.mouseButton) callbacks.mouseButton(&ms);
}
#endif

enum mousebutton {
BUTTON_LEFT = 0x1,
BUTTON_MIDDLE = 0x2,
BUTTON_RIGHT = 0x4,
};

/* Window and GL context creation.
 *
 * Context attributes are window-creation hints, so profile selection and
 * window creation are a single operation. */

static SDL_GLContext glcontext;

static void *procThunk(const char *name)   { return (void*)SDL_GL_GetProcAddress(name); }
static void  swapThunk(void *w)            { SDL_GL_SwapWindow((SDL_Window*)w); }
static void  intervalThunk(void *w, int i) { SDL_GL_SetSwapInterval(i); }
static void  sizeThunk(void *w, int *ww, int *hh)
	{ SDL_GL_GetDrawableSize((SDL_Window*)w, ww, hh); }

/* ---- host display data --------------------------------------------------
 *
 * malloc is used because this runs before the application initializes librw. */

static int currentDisplay;
static int numDisplays;
static int currentMode;

/* Where the window was before going fullscreen, so leaving fullscreen puts it
 * back at the same place and size rather than at the configured defaults. */
static int windowedX, windowedY, windowedW, windowedH;

/* Parallel portable/native arrays retain refresh rate and pixel format. */
static host::DisplayMode *modes;
static SDL_DisplayMode *nativeModes;
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
	free(nativeModes);
	modes       = (host::DisplayMode*)malloc((n+1) * sizeof(host::DisplayMode));
	nativeModes = (SDL_DisplayMode*)malloc((n+1) * sizeof(SDL_DisplayMode));
	if(modes == 0 || nativeModes == 0){
		numModes = 0;
		return;
	}

	/* entry 0: current mode, windowed */
	if(SDL_GetCurrentDisplayMode(currentDisplay, &m) == 0){
		modes[0].width  = m.w;
		modes[0].height = m.h;
		modes[0].depth  = modeDepth(&m);
		modes[0].flags  = 0;
		nativeModes[0]  = m;
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
		if(j < numModes){
			if(m.refresh_rate > nativeModes[j].refresh_rate)
				nativeModes[j] = m;
			continue;	/* already have this resolution */
		}
		modes[numModes].width  = m.w;
		modes[numModes].height = m.h;
		modes[numModes].depth  = modeDepth(&m);
		modes[numModes].flags  = host::DISPLAYMODEEXCLUSIVE;
		nativeModes[numModes]  = m;
		numModes++;
	}
}

static int topoNumDisplays(void)      { return numDisplays; }

/* Report the display the window is really on, not the one selected at
 * startup -- the user can drag the window to another monitor. SDL fullscreens
 * a window on its own display, so this only affects reporting. */
static int
topoCurrentDisplay(void)
{
	if(window){
		int d = SDL_GetWindowDisplayIndex(window);
		if(d >= 0)
			currentDisplay = d;
	}
	return currentDisplay;
}
static int topoNumVideoModes(void)    { return numModes; }
static int topoCurrentVideoMode(void) { return currentMode; }

static bool
topoSetDisplay(int n)
{
	numDisplays = SDL_GetNumVideoDisplays();
	if(n < 0 || n >= numDisplays)
		return 0;
	currentDisplay = n;
	buildModeList();
	currentMode = 0;
	return 1;
}

static bool
topoDisplayName(int n, char *buf, int buflen)
{
	const char *name;
	if(n < 0 || n >= SDL_GetNumVideoDisplays())
		return 0;
	name = SDL_GetDisplayName(n);
	if(name == 0)
		return 0;
	strncpy(buf, name, buflen-1);
	buf[buflen-1] = '\0';
	return 1;
}

/* Applying a mode does NOT recreate the window or the GL context: SDL
 * switches an existing window in place, so every raster librw has uploaded
 * stays valid. The resize event fires and the app resizes its cameras. */
static bool
topoSetVideoMode(int n)
{
	if(n < 0 || n >= numModes)
		return 0;
	if(window == 0){
		currentMode = n;
		return 1;	/* chosen before the window exists */
	}

	if(modes[n].flags & host::DISPLAYMODEEXCLUSIVE){
		if((SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) == 0){
			SDL_GetWindowPosition(window, &windowedX, &windowedY);
			SDL_GetWindowSize(window, &windowedW, &windowedH);
		}
		if(SDL_SetWindowDisplayMode(window, &nativeModes[n]) != 0)
			return 0;
		if(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
			return 0;
		currentMode = n;
		return 1;
	}
	if(SDL_SetWindowFullscreen(window, 0) != 0)
		return 0;
	SDL_SetWindowSize(window, windowedW, windowedH);
	SDL_SetWindowPosition(window, windowedX, windowedY);
	currentMode = n;
	return 1;
}

static bool
applyFullscreen(bool enable)
{
	if(window == 0){
		surface.fullscreen = enable;
		return true;
	}
	if(surface.fullscreen == enable)
		return true;
	if(enable){
		SDL_GetWindowPosition(window, &windowedX, &windowedY);
		SDL_GetWindowSize(window, &windowedW, &windowedH);
		if(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
			return false;
	}else{
		if(SDL_SetWindowFullscreen(window, 0) != 0)
			return false;
		SDL_SetWindowSize(window, windowedW, windowedH);
		SDL_SetWindowPosition(window, windowedX, windowedY);
	}
	surface.fullscreen = enable;
	return true;
}

static bool
topoVideoModeInfo(int n, host::DisplayMode *out)
{
	if(n < 0 || n >= numModes)
		return 0;
	*out = modes[n];
	return 1;
}

static int
topoMaxMultiSamplingLevels(void)
{
	return host::config.numSamples > 1 ? host::config.numSamples : 1;
}

static int  topoMultiSamplingLevels(void) { return host::config.numSamples; }
static bool topoSetMultiSamplingLevels(int n)
{
	/* MSAA is a context-creation attribute and the context already exists
	 * by the time librw can ask, so only a no-op change can succeed. */
	return n == host::config.numSamples;
}

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

	if(host::config.numSamples > 1){
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, host::config.numSamples);
	}else{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
	}

	int i;
	for(i = 0; profiles[i].profile; i++){
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profiles[i].profile);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, profiles[i].major);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, profiles[i].minor);
		window = SDL_CreateWindow(host::config.title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, host::config.width, host::config.height, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
		if(window)
			break;
	}
	if(window == 0){
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		return false;
	}

	glcontext = SDL_GL_CreateContext(window);
	if(glcontext == 0){
		fprintf(stderr, "SDL_GL_CreateContext: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetWindowPosition(window, host::config.x, host::config.y);

	SDL_GetWindowPosition(window, &windowedX, &windowedY);
	SDL_GetWindowSize(window, &windowedW, &windowedH);

	surface.window      = window;
	surface.glcontext   = glcontext;
	surface.gles        = profiles[i].profile == SDL_GL_CONTEXT_PROFILE_ES;
	surface.glversion   = profiles[i].major*10 + profiles[i].minor;
	surface.getProc     = procThunk;
	surface.swapBuffers = swapThunk;
	surface.setSwapInterval    = intervalThunk;
	surface.getFramebufferSize = sizeThunk;
	surface.width       = host::config.width;
	surface.height      = host::config.height;
	surface.numSamples  = host::config.numSamples;
	surface.fullscreen  = false;
	surface.title       = host::config.title;
	return true;
}

int
main(int argc, char *argv[])
{
	if(callbacks.initialize && !callbacks.initialize(argc, argv))
		return 0;

	if(!createSurface())
		return 0;
	if(host::config.fullscreen && !host::setFullscreen(true)){
		fprintf(stderr, "could not enter fullscreen: %s\n", SDL_GetError());
		return 0;
	}

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
					host::WindowRect r = { 0, 0, 0, 0 };
					SDL_GL_GetDrawableSize(window, &r.w, &r.h);
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
				if(event.key.repeat == 0 && event.key.keysym.sym == SDLK_RETURN &&
				   (event.key.keysym.mod & KMOD_ALT))
					host::setFullscreen(!host::isFullscreen());
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
				host::MouseState ms = {};
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
				host::MouseState ms = {};
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
				host::MouseState ms = {};
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
	free(nativeModes);
	modes = 0;
	nativeModes = 0;

	return 0;
}

namespace host {

bool setFullscreen(bool enable) { return applyFullscreen(enable); }
bool isFullscreen(void) { return surface.fullscreen; }

void
setMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(window, x, y);
}

}

#endif
#endif
