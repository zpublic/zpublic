#pragma once

#include "luasrc/lua.hpp"
#define lua_toulong(L, x)   static_cast<ULONG>(lua_tointeger((L), (x)))
#define lua_toint(L, x)     static_cast<int>(lua_tointeger((L), (x)))
