#pragma once

#include <string>

///> 玩家资源背包管理类
class CPlayerResBag
{
public:
    CPlayerResBag();
    ~CPlayerResBag();
	typedef std::pair<std::wstring, int> TypeItem;
	typedef std::map<std::wstring, int> TypeClass;
	typedef std::map<std::wstring, TypeClass> TypeBag;

    TypeBag& GetBag()
    {
        z_mutex_guard g(m_mutex_mapBag);
        return m_mapBag;
    }

    int GetResNum(LPCWSTR lpClassName, LPCWSTR lpName)
    {
        z_mutex_guard g(m_mutex_mapBag);
        auto it = m_mapBag.find(lpClassName);
        if (it != m_mapBag.end())
        {
			auto item = it->second.find(lpName);
			if (item != it->second.end())
				return item->second;
        }
        return 0;
    }

    void AddResNum(LPCWSTR lpClassName, LPCWSTR lpName, int n)
    {
        z_mutex_guard g(m_mutex_mapBag);
        auto it = m_mapBag.find(lpClassName);
        if (it != m_mapBag.end())
        {
			auto item = it->second.find(lpName);
			if(item != it->second.end())
			{
				item->second += n;
			}
			else
			{
				it->second[lpName] = n;
			}
        }
        else
        {
			m_mapBag.insert(std::make_pair(lpClassName, TypeClass()));
			m_mapBag[lpClassName][lpName] = n;
        }
    }

    bool SubResNum(LPCWSTR lpClassName, LPCWSTR lpName, int n)
    {
        z_mutex_guard g(m_mutex_mapBag);
        auto it = m_mapBag.find(lpClassName);
        if (it != m_mapBag.end())
        {
			auto item = it->second.find(lpName);
			if(item->second >= n)
            {
                item->second -= n;
            }
            else
            {
                it->second.erase(item);
            }
            return true;
        }
        return false;
    }

private:
	TypeBag				 m_mapBag;
    z_mutex              m_mutex_mapBag;
};

