
#include "role.h"

role::role(void)
{
	initialize();
}

role::~role(void)
{
}

void role::initialize()
{
	m_bInitialized = false;
	m_nRoleId = 0;
	m_nSceneId = 0;
}

void role::finalize()
{
	m_bInitialized = false;
}

void role::onLogin()
{
	m_BagManager.setOwner(this);

	m_bInitialized = true;
}

void role::onLogout()
{

}

void role::onUpdate( uint64 nCurTime )
{

}

void role::sendMessage( uint32 nMsgType, uint32 nMsgLen, const char* bufToSend )
{

}
