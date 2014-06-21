#include "stdafx.h"
#include "z_lua_bind_internal_pop.h"

template<>
void zl::LuaBind::pop(lua_State *L)
{
    lua_pop(L, 1);
}
