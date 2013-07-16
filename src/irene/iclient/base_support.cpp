#include "stdafx.h"
#include "base_support.h"

CBaseSupport::CBaseSupport()
{

}

CBaseSupport::~CBaseSupport()
{

}

int CBaseSupport::Register( IClientOperate* iOper )
{
    //iOper->RegisterMessageCallback();
    return 0;
}

int CBaseSupport::Unregister( IClientOperate* iOper )
{
    //iOper->UnregisterMessageCallback();
    return 0;
}
