#include "stdafx.h"
#include "TestHarold.h"
#include "harold/harold.h"
#include "harold/harold_server_impl.h"
#include "harold/harold_server_auth_log.h"
#include "harold/harold_server_request_log.h"
#include "harold/harold_server_request_download.h"
#include "harold/harold_server_request_api.h"

CTestHarold::CTestHarold(void)
{
    TEST_ADD(CTestHarold::testHarold);
}

int process1(mg_connection* conn)
{
    mg_printf_data(conn, "hello %s", conn->remote_ip);
    return 1;
}

int process2(mg_connection* conn)
{
    mg_send_file(conn, "c:\\windows\\explorer.exe", NULL);
    return 3;
}

void CTestHarold::testHarold()
{
    IHaroldServerAuthObserver* pAuth = new HaroldServerAuthLog;
    IHaroldServerRequestObserver* pRequest = new HaroldServerRequestLog;

    HaroldServerRequestDownload* pRequest2 = new HaroldServerRequestDownload;
    pRequest2->InsertDownload("/1", "c:\\windows\\explorer.exe");
    pRequest2->InsertDownload("/1/1", "c:\\windows\\regedit.exe");

    HaroldServerRequestApi* pRequest3 = new HaroldServerRequestApi;
    pRequest3->InsertApi("/21", process1);
    pRequest3->InsertApi("/22", process2);

    IHaroldServer* pServer = new HaroldServerImpl;
    pServer->InsertObserver(static_cast<IHaroldServerObserver*>(pAuth), harold_observer_type_auth);
    pServer->InsertObserver(static_cast<IHaroldServerObserver*>(pRequest), harold_observer_type_request);
    pServer->InsertObserver(static_cast<IHaroldServerObserver*>(pRequest2), harold_observer_type_request);
    pServer->InsertObserver(static_cast<IHaroldServerObserver*>(pRequest3), harold_observer_type_request);
    pServer->StartServer();
    ::Sleep(30000);
    pServer->StopServer();
    delete pAuth;
    delete pRequest;
    delete pServer;
    delete pRequest2;
    delete pRequest3;
}
