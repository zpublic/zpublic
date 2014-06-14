#pragma once
#include "z_event_event.h"

class z_event_reactor
{
public:
    z_event_reactor();
    ~z_event_reactor();

public:
    int init();
    int uninit();

    int dispatch();

    int set(z_event_event* pEvent);
};
