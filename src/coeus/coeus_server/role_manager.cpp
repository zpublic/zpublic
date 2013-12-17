
#include "role_manager.h"
#include "role.h"

#include "game_define_role.h"

RoleManager::RoleManager(void)
{
	for (int i = 0; i < ROLE_COUNT_MAX; i++)
	{
		m_stFreeRoles.insert((uint64)(BEGIN_ROLE_ID + i));
	}

	m_mRolePool.clear();
}

RoleManager::~RoleManager(void)
{
}

uint64 RoleManager::getFreeRoleId()
{
	if (m_stFreeRoles.empty())
	{
		return INVALID_ROLE_ID;
	}

	uint64 nNewRoleId = *m_stFreeRoles.begin();
	m_stFreeRoles.erase(m_stFreeRoles.begin());

	return nNewRoleId;
}

Role* RoleManager::createNewRole()
{
	uint64 nNewRoleId = getFreeRoleId();
	if (INVALID_ROLE_ID == nNewRoleId)
	{
		return NULL;
	}

	Role* pNewRole = NULL;
	std::map<uint64, Role *>::iterator it = m_mRolePool.find(nNewRoleId);
	if (it == m_mRolePool.end()) // 创建新玩家
	{
		pNewRole = new Role;
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

bool RoleManager::removeRole( Role* pRole )
{
	if (NULL == pRole)
	{
		return false;
	}

	uint64 nOldRoleId = pRole->getRoleId();
	std::map<uint64, Role *>::iterator it = m_mRolePool.find(nOldRoleId);
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

Role* RoleManager::findRole( uint64 nRoleId ) const
{
	std::map<uint64, Role *>::const_iterator cit = m_mRolePool.find(nRoleId);
	if (cit == m_mRolePool.end())
	{
		return NULL;
	}

	return cit->second;
}

Role* RoleManager::findRole( std::string szRoleName ) const
{
	std::map<uint64, Role *>::const_iterator cit = m_mRolePool.begin();
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

void RoleManager::onRoleLogin( Role* pRole )
{
	if (NULL == pRole)
	{
		return;
	}

	pRole->onLogin();
}

void RoleManager::onRoleLogout( Role* pRole )
{
	if (NULL == pRole)
	{
		return;
	}

	pRole->onLogout();
}

void RoleManager::onUpdate( uint64 nCurTime ) const
{
	std::map<uint64, Role *>::const_iterator cit = m_mRolePool.begin();
	for (; cit != m_mRolePool.end(); ++cit)
	{
		if (NULL != cit->second)
		{
			cit->second->onUpdate(nCurTime);
		}
	}
}
