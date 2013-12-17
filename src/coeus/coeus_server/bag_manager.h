#ifndef __BAG_MANAGER_H__
#define __BAG_MANAGER_H__

#include "common.h"
#include "bag_common.h"

class role;

class bag_manager
{
public:
	bag_manager(void);
	virtual ~bag_manager(void);

	PROP(role*, m_pOwner, Owner);
	READONLY_PROP(bag_common, m_Package, Package);		// ±³°ü
	READONLY_PROP(bag_common, m_Storage, Storage);		// ²Ö¿â
};

#endif
