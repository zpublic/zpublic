#include "stdafx.h"
#include "harold_server_status_log.h"

void HaroldServerStatusLog::OnStatus(emHaroldServerStatus status, void* pData)
{
    std::cout << "OnStatus " << (int)status << std::endl;
}
