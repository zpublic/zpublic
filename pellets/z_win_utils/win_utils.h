#pragma once

#include "win_utils_header.h"
#include "clipboard.hpp"
#include "directory.hpp"
#include "file_version.hpp"
#include "ini.hpp"
#include "message.hpp"
#include "path.hpp"
#include "taskbar_position.hpp"
#include "usid.hpp"
#include "register.hpp"
#include "system_version.hpp"
#include "wow64.hpp"
#include "system_path.hpp"
#include "autorun.hpp"
#include "console_color.hpp"
#include "environment_var.hpp"

#ifdef _GDIPLUS_H
#include "gdiplus.hpp"
#endif

#ifndef Z_WIN_UTILS_NOUSE
using namespace zl::WinUtils;
#endif
