#pragma once
#include "harold_server_def.h"

class IHaroldServerObserver
{
    virtual unsigned int ObserverType() = 0;
};
