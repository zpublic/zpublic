#pragma once

class z_task
{
public:
    z_task();
    virtual ~z_task();

    virtual void run() = 0;
};
