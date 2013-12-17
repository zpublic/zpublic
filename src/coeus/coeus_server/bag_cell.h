
#ifndef __BAG_CELL_H__
#define __BAG_CELL_H__

#include "common.h"

class bag_cell
{
public:
	bag_cell(void);
	virtual ~bag_cell(void);

	PROP(uint32, m_nItemId, ItemId);
	PROP(uint32, m_nItemCount, ItemCount);
};

#endif
