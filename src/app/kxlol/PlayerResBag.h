#pragma once

///> 玩家资源背包管理类
class CPlayerResBag
{
public:
    CPlayerResBag();
    ~CPlayerResBag();

    std::map<CString, int> GetBag()
    {
        z_mutex_guard g(m_mutex_mapBag);
        return m_mapBag;
    }

    int GetResNum(LPCWSTR lpName)
    {
        z_mutex_guard g(m_mutex_mapBag);
        auto it = m_mapBag.find(lpName);
        if (it != m_mapBag.end())
        {
            return it->second;
        }
        return 0;
    }

    void AddResNum(LPCWSTR lpName, int n)
    {
        z_mutex_guard g(m_mutex_mapBag);
        auto it = m_mapBag.find(lpName);
        if (it != m_mapBag.end())
        {
            it->second += n;
        }
        else
        {
            m_mapBag[lpName] = n;
        }
    }

    bool SubResNum(LPCWSTR lpName, int n)
    {
        z_mutex_guard g(m_mutex_mapBag);
        auto it = m_mapBag.find(lpName);
        if (it != m_mapBag.end() && it->second >= n)
        {
            if (it->second > n)
            {
                it->second -= n;
            }
            else
            {
                m_mapBag.erase(it);
            }
            return true;
        }
        return false;
    }

private:
    std::map<CString, int>          m_mapBag;
    z_mutex                         m_mutex_mapBag;
};

