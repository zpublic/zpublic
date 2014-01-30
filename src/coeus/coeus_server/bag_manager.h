#ifndef __BAG_MANAGER_H__
#define __BAG_MANAGER_H__

#include "venus_net/venus_net.h"
#include "bag_common.h"

class Role;

class BagManager
{
public:
	BagManager(void);
	virtual ~BagManager(void);

	PROP(Role*, m_pOwner, Owner);
	READONLY_PROP(BagCommon, m_Package, Package);		// ±³°ü
	READONLY_PROP(BagCommon, m_Storage, Storage);		// ²Ö¿â
};

#endif
