
#include "role_manager.h"
#include "role.h"

#include "game_define_role.h"

role_manager::role_manager(void)
{
	for (int i = 0; i < ROLE_COUNT_MAX; i++)
	{
		m_stFreeRoles.insert((uint64)(BEGIN_ROLE_ID + i));
	}

	m_mRolePool.clear();
}

role_manager::~role_manager(void)
{
}

uint64 role_manager::getFreeRoleId()
{
	if (m_stFreeRoles.empty())
	{
		return INVALID_ROLE_ID;
	}

	uint64 nNewRoleId = *m_stFreeRoles.begin();
	m_stFreeRoles.erase(m_stFreeRoles.begin());

	return nNewRoleId;
}

role* role_manager::createNewRole()
{
	uint64 nNewRoleId = getFreeRoleId();
	if (INVALID_ROLE_ID == nNewRoleId)
	{
		return NULL;
	}

	role* pNewRole = NULL;
	std::map<uint64, role *>::iterator it = m_mRolePool.find(nNewRoleId);
	if (it == m_mRolePool.end()) // 创建新玩家
	{
		pNewRole = new role;
	}
	else // 重新初始化玩家池里面的玩家
	{
		pNewRole = it->second;
	}

	pNewRole->initialize();
	pNewRole->setRoleId(nNewRoleId);
	pNewRole->onLogin();

	m_mRolePool[nNewRoleId] = pNewRole;
	return pNewRole;
}

bool role_manager::removeRole( role* pRole )
{
	if (NULL == pRole)
	{
		return false;
	}

	uint64 nOldRoleId = pRole->getRoleId();
	std::map<uint64, role *>::iterator it = m_mRolePool.find(nOldRoleId);
	if (it == m_mRolePool.end() ||
		NULL == it->second)
	{
		return false;
	}

	it->second->onLogout();
	it->second->finalize();
	m_stFreeRoles.insert(nOldRoleId);
	return true;
}

role* role_manager::findRole( uint64 nRoleId ) const
{
	std::map<uint64, role *>::const_iterator cit = m_mRolePool.find(nRoleId);
	if (cit == m_mRolePool.end())
	{
		return NULL;
	}

	return cit->second;
}

role* role_manager::findRole( std::string szRoleName ) const
{
	std::map<uint64, role *>::const_iterator cit = m_mRolePool.begin();
	for (; cit != m_mRolePool.end(); ++cit)
	{
		if (NULL != cit->second &&
			0 == cit->second->getRoleName().compare(szRoleName))
		{
			return cit->second;
		}
	}

	return NULL;
}

void role_manager::onRoleLogin( role* pRole )
{
	if (NULL == pRole)
	{
		return;
	}

	pRole->onLogin();
}

void role_manager::onRoleLogout( role* pRole )
{
	if (NULL == pRole)
	{
		return;
	}

	pRole->onLogout();
}

void role_manager::onUpdate( uint64 nCurTime ) const
{
	std::map<uint64, role *>::const_iterator cit = m_mRolePool.begin();
	for (; cit != m_mRolePool.end(); ++cit)
	{
		if (NULL != cit->second)
		{
			cit->second->onUpdate(nCurTime);
		}
	}
}
