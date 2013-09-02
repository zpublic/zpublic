#pragma once

template<int MaxCount = 10>
class ParticleCreator
{
private:
    typedef struct tagParticleItemHold
    {
        tagParticleItemHold() : m_bUse(FALSE) {}
        ParticleItem    m_item;
        BOOL            m_bUse;
    }ParticleItemHold;

public:
    ParticleItem* CreateItem()
    {
        for (int i = 0; i < MaxCount; ++i)
        {
            if (m_items[i].m_bUse == FALSE)
            {
                m_items[i].m_bUse = TRUE;
                return &(m_items[i].m_item);
            }
        }
        return NULL;
    }

    void ReleaseItem(ParticleItem* pItem)
    {
        assert(pItem);
        for (int i = 0; i < MaxCount; ++i)
        {
            if (&(m_items[i].m_item) == pItem)
            {
                m_items[i].m_bUse = FALSE;
                break;
            }
        }
        assert(false);
    }

protected:
    ParticleItemHold m_items[MaxCount];
};
