//////////////////////////////////////////////////////////////////////
///		@file		AutoResource.h
///		@author		luopeng
///		@date		2005-7-21 15:26:24
///
///		@brief		资源智能指针
//////////////////////////////////////////////////////////////////////

#ifndef AUTORESOURCE_H
#define AUTORESOURCE_H

#include <stdio.h>

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef WIN32

class HandlePolicy
{
public:
	void ReleaseResource(HANDLE& hValue)
	{
		if (hValue != INVALID_HANDLE_VALUE && hValue != NULL)
		{
            CloseHandle(hValue);
			hValue = NULL;
		}
	}
};


class SocketPolicy
{
public:
	void ReleasePolicy(SOCKET& value)
	{
		if (value != INVALID_SOCKET)
		{
			closesocket(value);
			value = INVALID_SOCKET;
		}
	}
};


class RegKeyPolicy
{
public:
	void ReleaseResource(HKEY& hKey)
	{
		if (hKey != NULL)
		{
			RegCloseKey(hKey);
			hKey = NULL;
		}
	}
};

#endif

class LibraryReleasePolicy
{
public:
	void ReleaseResource(HMODULE& hValue)
	{
		if (hValue != NULL)
		{
			FreeLibrary(hValue);
			hValue = NULL;
		}
	}
};

class CriticalSectionPolicy
{
public:
	void ReleaseResource(CRITICAL_SECTION& cs)
	{
		DeleteCriticalSection(&cs);
	}
};


class FilePointerPolicy
{
public:
	void ReleaseResource(FILE*& fp)
	{
		if (fp != NULL)
		{
			fclose(fp);
			fp = NULL;
		}
	}
};

class FileIDPolicy
{
public:
	void ReleaseResource(int& value)
	{
		if (value != -1 && value != 0)
		{
			close(value);			
			value = 0;
		}
	}
};

template 
	<
	typename T, 
	typename ReleasePolicy
	>
class AutoResource : public ReleasePolicy
{	
	template<typename U>
	struct Type
	{
		typedef U value_type;
		typedef U& reference_type;
		typedef U* pointer_type;
	};

	template<typename V>
	struct Type<V&>
	{
		typedef V value_type;
		typedef V& reference_type;
		typedef V* pointer_type;
	};

	typedef typename Type<T>::value_type value_type;
	typedef typename Type<T>::reference_type reference_type;
	typedef typename Type<T>::pointer_type pointer_type;

public:	
	AutoResource(value_type value = value_type()):m_Value(value)
	{
 	}

	template 
		<
		typename T1, 
		typename RP1
		>
	AutoResource(AutoResource<T1, RP1>& value): m_Value(value.Release())
	{
	}

	~AutoResource(void)
	{
		Reset();
	}

	void Reset(value_type value = T())
	{		
		ReleaseResource(m_Value);					
		m_Value = value;
	}

	AutoResource<T, ReleasePolicy>& operator=(AutoResource<T, ReleasePolicy>& value)
	{
		Reset(value.Release());
	}

	operator value_type()
	{
		return m_Value;
	}

	pointer_type operator &()
	{
		return &m_Value;
	}

	value_type Release()
	{
		value_type value = m_Value;
		m_Value = T();
		return value;
	}

private:
	value_type m_Value;
};

//#ifdef WIN32
template 
	<
	>
class AutoResource<CRITICAL_SECTION, CriticalSectionPolicy> : public CriticalSectionPolicy
{	
	typedef CRITICAL_SECTION value_type;
	typedef CRITICAL_SECTION& reference_type;
	typedef CRITICAL_SECTION* pointer_type;

public:	
	AutoResource()
	{
		InitializeCriticalSection(&m_Value);
 	}

	template 
		<
		typename T1, 
		typename RP1
		>
	AutoResource(AutoResource<T1, RP1>& value): m_Value(value.Release())
	{
	}

	~AutoResource(void)
	{
		ReleaseResource(m_Value);
	}

	template
		<
		typename T, 
		typename ReleasePolicy
		>
	AutoResource<T, ReleasePolicy>& operator=(AutoResource<T, ReleasePolicy>& value)
	{
		Reset(value.Release());
	}

	operator value_type()
	{
		return m_Value;
	}

	pointer_type operator &()
	{
		return &m_Value;
	}
	

private:
	value_type m_Value;
};

//#endif

#ifdef WIN32

typedef AutoResource<HANDLE, HandlePolicy> AutoHandle;
typedef AutoResource<HKEY, RegKeyPolicy> AutoRegKey;
typedef AutoResource<SOCKET, SocketPolicy> AutoSocket;

#else

typedef AutoResource<int, FileIDPolicy> AutoSocket;  // linux下关闭方式同文件ID

#endif
typedef AutoResource<CRITICAL_SECTION, CriticalSectionPolicy> AutoCriticalSection;
typedef AutoResource<FILE*, FilePointerPolicy> AutoFilePointer;
typedef AutoResource<int, FileIDPolicy> AutoFile;
typedef AutoResource<HMODULE, LibraryReleasePolicy> AutoLibrary;

#endif
