#ifndef __BAG_COMMON_H__
#define __BAG_COMMON_H__

#include "common.h"
#include "bag_cell.h"
#include "game_define_item.h"

class bag_common
{
public:
	bag_common(void);
	virtual ~bag_common(void);

	PROP(enBagType, m_nBagType, BagType);
	READONLY_PROP(std::list<bag_cell>, m_mItems, Items);
};

#endif
