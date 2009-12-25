#include "stdafx.h"
#include "lswitch.h"

static const luaL_Reg lswitch_funcs[] = {
  {"trace", lswitch_trace},
  {"activate", lswitch_activate},
  {NULL, NULL}
};

int lswitch_activate(lua_State * L)
{
	HWND hwnd = (HWND)lua_touserdata(L, 1);
	//SetActiveWindow(hwnd);
	if (IsIconic(hwnd)) {
		ShowWindow(hwnd, SW_RESTORE);
	} else {
		ShowWindow(hwnd, SW_SHOW);
	}
	//SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
	SetForegroundWindow(hwnd);
	BringWindowToTop(hwnd);
	return 0;
}

int lswitch_trace(lua_State *L)
{
	TRACE("%s\n", lua_tostring(L, 1));
	return 0;
}

int lswitch_openswitch(lua_State *L)
{
	luaL_register(L, "switch", lswitch_funcs);
	return 0;
}