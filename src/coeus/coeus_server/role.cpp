
#include "role.h"

Role::Role(void)
{
	initialize();
}

Role::~Role(void)
{
}

void Role::initialize()
{
	m_bInitialized = false;
	m_nRoleId = 0;
	m_nSceneId = 0;
}

void Role::finalize()
{
	m_bInitialized = false;
}

void Role::onLogin()
{
	m_BagManager.setOwner(this);

	m_bInitialized = true;
}

void Role::onLogout()
{

}

void Role::onUpdate( uint64 nCurTime )
{

}

void Role::sendMessage( uint32 nMsgType, uint32 nMsgLen, const char* bufToSend )
{

}
