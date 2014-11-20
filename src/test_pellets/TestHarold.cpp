#include "stdafx.h"
#include "TestHarold.h"
#include "harold/harold.h"
#include "harold/harold_server_impl.h"
#include "harold/harold_server_auth_log.h"
#include "harold/harold_server_request_log.h"

CTestHarold::CTestHarold(void)
{
    TEST_ADD(CTestHarold::testHarold);
}

void CTestHarold::testHarold()
{
    IHaroldServerAuthObserver* pAuth = new HaroldServerAuthLog;
    IHaroldServerRequestObserver* pRequest = new HaroldServerRequestLog;

    IHaroldServer* pServer = new HaroldServerImpl;
    pServer->InsertObserver(static_cast<IHaroldServerObserver*>(pAuth), harold_observer_type_auth);
    pServer->InsertObserver(static_cast<IHaroldServerObserver*>(pRequest), harold_observer_type_request);
    pServer->StartServer();
    ::Sleep(30000);
    pServer->StopServer();
    delete pAuth;
    delete pRequest;
    delete pServer;
}
