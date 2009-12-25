#ifndef _LSWITCH_H_
#define _LSWITCH_H_

#include "lua.hpp"

int lswitch_trace(lua_State * L);
int lswitch_activate(lua_State * L);

int lswitch_openswitch(lua_State *L);

#endif /* _LSWITCH_H_ */