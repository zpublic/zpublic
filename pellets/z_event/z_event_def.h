#pragma once

enum z_event_ret
{
    z_event_ret_ok      = 0,    ///> succeed
};


enum z_event_events
{
    z_event_events_timeout      = 0x1,
    z_event_events_read         = 0x10,
    z_event_events_write        = 0x100,
    z_event_events_sign         = 0x1000,
};
