#pragma once
#include "particle.h"

template
    <
    typename Creator,
    typename InitControler
    >
class ParticleGdiPlusBaseT : public zl::ParticleControler<Creator, InitControler>
{
    typedef zl::ParticleControler<Creator, InitControler> Base;
public:
    ParticleGdiPlusBaseT()
    {
        m_pImage = NULL;
        m_pGraphics = NULL;
        zl::ParticleInitInfo initInfo = {sizeof(zl::ParticleInitInfo)};
        initInfo.m_fAngle = 3.14f / 2 * 3;
        initInfo.m_fFullSpeedMin = 30.0f;
        initInfo.m_fFullSpeedMax = 200.0f;
        initInfo.m_fLifeMin = 2.0f;
        initInfo.m_fLifeMax = 5.0f;
        initInfo.m_fSpeedBegin = 0.2f;
        initInfo.m_fxPosMin = 0.0f;
        initInfo.m_fxPosMax = 500.0f;
        initInfo.m_fyPosMin = 0.0f;
        initInfo.m_fyPosMax = 200.0f;
        m_InitControler.SetInfo(initInfo);
    }
    virtual ~ParticleGdiPlusBaseT() {}

    void Render(HDC hDc, Image* pImage, CRect& rcClient)
    {
        if (hDc && pImage)
        {
            m_pImage    = pImage;
            Graphics graphics(hDc);
            Bitmap bmp(rcClient.Width(), rcClient.Height());
            m_pGraphics = new Graphics(&bmp);
            m_pGraphics->Clear(Color::White);
            Base::Render();
            graphics.DrawImage(&bmp, 0, 0);
            delete m_pGraphics;
        }
    }

    virtual void Update(float fTimeInterval)
    {
        zl::ParticleItem* pItem = m_Creator.CreateItem();
        if (pItem)
        {
            m_InitControler.InitItem(pItem);
            m_listItems.push_back(pItem);
        }
        Base::Update(fTimeInterval);
    }

    virtual void RenderItem(zl::ParticleItem* pItem)
    {
        if (m_pImage && m_pGraphics)
        {
            m_pGraphics->DrawImage(m_pImage, (INT)pItem->m_fxPos, (INT)pItem->m_fyPos);
            //m_pGraphics->DrawImage(m_pImage, pItem->m_fxPos, pItem->m_fyPos, m_pImage->GetWidth(), m_pImage->GetHeight());
        }
    }

protected:
    Image*          m_pImage;
    Graphics*       m_pGraphics;
};

typedef ParticleGdiPlusBaseT<zl::ParticleCreator, zl::ParticleInitControler_NomalRandom> ParticleGdiBase;