#pragma once

#include "harold_header.h"

#define harold_observer_type_auth           0x1
#define harold_observer_type_request        0x2
#define harold_observer_type_status         0x4
#define harold_observer_type_all            harold_observer_type_auth|harold_observer_type_request|harold_observer_type_status
