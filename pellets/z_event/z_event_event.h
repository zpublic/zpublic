#pragma once

class z_event_event
{
public:
    z_event_event();
    ~z_event_event();

public:
    int set();


private:
    int m_nEvents;          ///> event关注的事件类型

};
