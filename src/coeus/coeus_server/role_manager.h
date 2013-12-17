#ifndef __ROLE_MANAGER_H__
#define __ROLE_MANAGER_H__

#include "common.h"
#include "role_manager.h"

class role;

class role_manager
{
public:
	role_manager(void);
	virtual ~role_manager(void);

private:

	uint64 getFreeRoleId();

public:
	role* createNewRole();
	bool removeRole(role* pRole);

	role* findRole(uint64 nRoleId) const;
	role* findRole(std::string szRoleName) const;

	void onRoleLogin(role* pRole);
	void onRoleLogout(role* pRole);
	void onUpdate(uint64 nCurTime) const;

	READONLY_PROP(std::set<uint64>, m_stFreeRoles, FreeRoles);

private:
	std::map<uint64, role *> m_mRolePool;
};

#endif
