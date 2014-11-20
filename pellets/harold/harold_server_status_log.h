#pragma once
#include "i_harold_server_status_observer.h"

class HaroldServerStatusLog : public IHaroldServerStatusObserver
{
public:
    virtual void OnStatus(emHaroldServerStatus status, void* pData);
};
