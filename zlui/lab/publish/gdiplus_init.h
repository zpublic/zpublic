#pragma once

class GdiPlusAutoInit
{
    class Inst
    {
    public:
        Inst():m_bInit(false)
        {

        }
        void Init()
        {
            if (m_bInit)
            {
                return ;
            }
            Gdiplus::GdiplusStartup(&m_gpToken,  &m_gpInput,  NULL); 
            m_bInit = true;
        }
        void UnInit()
        {
            if (!m_bInit)
            {
                return ;
            }
            Gdiplus::GdiplusShutdown(m_gpToken);
            m_bInit = false;
        }
    protected:
        bool m_bInit;
        GdiplusStartupInput m_gpInput; 
        ULONG_PTR           m_gpToken; 
    };
public:
    typedef Inst GdiPlusInit;
    GdiPlusAutoInit()
    {
        m_GdiPlusInit.Init();
    }
    ~GdiPlusAutoInit()
    {
        m_GdiPlusInit.UnInit();
    }
protected:
    GdiPlusInit m_GdiPlusInit;
};
