#include "host.h"

namespace sk {

using namespace rw;

/* Canonical input types live in the host layer now -- key codes and mouse
 * state are a windowing concern, not an example-harness one. Pulled in here
 * so existing app code keeps writing sk::KEY_ESC and sk::MouseState. */
using namespace host;

enum EventStatus
{
	EVENTERROR,
	EVENTPROCESSED,
	EVENTNOTPROCESSED
};

enum Event
{
	INITIALIZE,
	RWINITIALIZE,
	RWTERMINATE,
	SELECTDEVICE,
	PLUGINATTACH,
	KEYDOWN,
	KEYUP,
	CHARINPUT,
	MOUSEMOVE,
	MOUSEBTN,
	MOUSEWHEEL,
	RESIZE,
	IDLE,
	QUIT
};

struct Globals
{
	const char *windowtitle;
	int32 width;
	int32 height;
	bool32 quit;
};
extern Globals globals;

struct Args
{
	int argc;
	char **argv;
};
extern Args args;

bool InitRW(void);
void TerminateRW(void);
Camera *CameraCreate(int32 width, int32 height, bool32 z);
void CameraDestroy(rw::Camera *cam);
void CameraSize(Camera *cam, Rect *r, float viewWindow = 0.0f, float aspectRatio = 0.0f);
void CameraMove(Camera *cam, V3d *delta);
void CameraPan(Camera *cam, V3d *pos, float angle);
void CameraTilt(Camera *cam, V3d *pos, float angle);
void CameraRotate(Camera *cam, V3d *pos, float angle);
void SetMousePosition(int x, int y);
EventStatus EventHandler(Event e, void *param);

}

sk::EventStatus AppEventHandler(sk::Event e, void *param);

#include "imgui.h"
#include "imgui_impl_rw.h"
