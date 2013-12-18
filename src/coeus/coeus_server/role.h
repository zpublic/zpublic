#ifndef __ROLE_H__
#define __ROLE_H__

#include "common.h"
#include "bag_manager.h"

class Role
{
public:
	Role(void);
	virtual ~Role(void);

	void initialize();
	void finalize();

public:

	void onLogin();
	void onLogout();
	void onUpdate(uint64 nCurTime);

	void sendMessage(uint32 nMsgType, uint32 nMsgLen, const char* bufToSend);

public:
	READONLY_PROP(bool, m_bInitialized, Initialized);				// 用户是否已经初始化

	PROP(uint64, m_nRoleId, RoleId);
	PROP(std::string, m_szRoleName, RoleName);
	PROP(uint32, m_nSceneId, SceneId);
	READONLY_PROP(BagManager, m_BagManager, BagManager);
};

#endif
