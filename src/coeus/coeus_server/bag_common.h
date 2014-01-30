#ifndef __BAG_COMMON_H__
#define __BAG_COMMON_H__

#include "venus_net/venus_net.h"
#include "bag_cell.h"
#include "game_define_item.h"

class BagCommon
{
public:
	BagCommon(void);
	virtual ~BagCommon(void);

	PROP(enBagType, m_nBagType, BagType);
	READONLY_PROP(std::list<BagCell>, m_mItems, Items);
};

#endif
