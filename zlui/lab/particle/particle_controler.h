#pragma once

struct ParticleItem;

template
    <
    typename Creator        = ParticleCreator,
    typename InitControler  = ParticleInitControler_Fixed
    >
class ParticleControler
{
public:
    ParticleControler() {}
    virtual ~ParticleControler() {}

    virtual void Update(float fTimeInterval)
    {
        ClearItemDied(fTimeInterval);
        std::list<ParticleItem*>::iterator it;
        for (it = m_listItems.begin(); it != m_listItems.end(); ++it)
        {
            UpdateItem(*it, fTimeInterval);
        }
    }
    virtual void Render()
    {
        std::list<ParticleItem*>::iterator it;
        for (it = m_listItems.begin(); it != m_listItems.end(); ++it)
        {
            RenderItem(*it);
        }
    }

    virtual void UpdateItem(ParticleItem* pItem, float fTimeInterval)
    {
        float fRightSpeed = pItem->m_fSpeed * cos(pItem->m_fAngle);
        float fUpSpeed = pItem->m_fSpeed * sin(pItem->m_fAngle);
        pItem->m_fxPos += fRightSpeed * fTimeInterval;
        pItem->m_fyPos -= fUpSpeed * fTimeInterval;

        float fLiffRatio = (pItem->m_fLife - pItem->m_fLifeLive) / pItem->m_fLife;
        pItem->m_fSpeed = pItem->m_fFullSpeed * fLiffRatio;
    }
    virtual void RenderItem(ParticleItem* pItem)
    {
        ;
    }

protected:
    Creator         m_Creator;
    InitControler   m_InitControler;
    std::list<ParticleItem*> m_listItems;

protected:
    void ClearItemDied(float fTimeInterval)
    {
        std::list<ParticleItem*>::iterator it;
        for (it = m_listItems.begin(); it != m_listItems.end(); )
        {
            ///> 在这把item的已存活时间加了，有点不科学
            (*it)->m_fLifeLive += fTimeInterval;
            if ((*it)->m_fLifeLive > (*it)->m_fLife)
            {
                m_Creator.ReleaseItem(*it);
                it = m_listItems.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
};
