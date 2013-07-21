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
    return iOper->RegisterMessageCallback(
        S2C::auth::descriptor(),
        std::bind(&CBaseSupport::onAuth, this, std::placeholders::_1));
}

int CBaseSupport::Unregister( IClientOperate* iOper )
{
    return iOper->UnregisterMessageCallback(S2C::auth::descriptor());
}

void CBaseSupport::onAuth( google::protobuf::Message* message )
{
    S2C::auth* msg = dynamic_cast<S2C::auth*>(message);
    assert(msg != NULL);
    MessageBoxA(0, msg->password().c_str(), 0, 0);
}
