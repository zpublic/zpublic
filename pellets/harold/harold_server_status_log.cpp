#include "harold_header.h"
#include "harold_server_status_log.h"

void HaroldServerStatusLog::OnStatus(emHaroldServerStatus status, void* pData)
{
    HAROLD_PRINT("OnStatus:%d", (int)status);
}
