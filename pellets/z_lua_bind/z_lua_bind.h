#pragma once
///> 请将该文件包含到stdafx.h


///> 包含lua头文件，及全局定义
#include "z_lua_bind_def.h"

///> 内部细节相关实现
#include "z_lua_bind_internal.hpp"

///> 全局变量
#include "z_lua_bind_global.hpp"

///> 函数绑定
#include "z_lua_bind_function.hpp"

///> 用于辅助注册一系列的函数给lua
#include "z_lua_function_reg.h"

///> 封装lua_State
#include "z_lua_state.h"


