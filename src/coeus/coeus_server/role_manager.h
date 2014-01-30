#ifndef __ROLE_MANAGER_H__
#define __ROLE_MANAGER_H__

#include "venus_net/venus_net.h"
#include "role_manager.h"

class Role;

class RoleManager
{
public:
	RoleManager(void);
	virtual ~RoleManager(void);

private:

	uint64 getFreeRoleId();

public:
	Role* createNewRole();
	bool removeRole(Role* pRole);

	Role* findRole(uint64 nRoleId) const;
	Role* findRole(std::string szRoleName) const;

	void onRoleLogin(Role* pRole);
	void onRoleLogout(Role* pRole);
	void onUpdate(uint64 nCurTime) const;

	READONLY_PROP(std::set<uint64>, m_stFreeRoles, FreeRoles);

private:
	std::map<uint64, Role *> m_mRolePool;
};

#endif
