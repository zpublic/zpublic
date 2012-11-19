/*
 * string.hpp
 *
 *  Created on: 2012-11-14
 *      Author: kevin
 */

#ifndef _STRING_HPP_
#define _STRING_HPP_

#include "../memory/allocer.hpp"
#include "../algorithm/stringmatch.hpp"
#include "array.hpp"

namespace zl
{
	class basic_string
	{
	public:
		basic_string():m_size(0),m_capacity(0),m_string(NULL)
		{
			
		}
		
		~basic_string()
		{
			m_size = 0;
			m_capacity = 0;
			_Free(m_string);
			m_string = NULL;
		}

		basic_string(const char* buf)
		{
			m_size = GetStrLen(buf);
			m_string = _Allocate(m_string, m_size * 2);
			m_capacity = m_size * 2;
			
			size_t i;
			for(i=0; i < m_size; i++)
				m_string[i] = buf[i];
			m_string[i] = '\0';
		}

	public:

		basic_string& operator=(basic_string& x)
		{
			if(this->m_string != NULL)
				_Free(this->m_string);
			this->m_string = _Allocate(m_string, x.m_capacity);
			if(this->m_string != NULL)
			{
				size_t i;
				for(i = 0; i < x.m_size; i++)
				{
					this->m_string[i] = x.c_str()[i]; 
				}
				this->m_string[i] = '\0';
			}
			this->m_size = x.m_size;
			this->m_capacity = x.m_capacity;

			return ( *this );
		}

		void operator=(const char* buf)
		{
			this->m_size = GetStrLen(buf);
			this->m_string = _Allocate(m_string, m_size * 2);
			m_capacity = this->m_size * 2;

			size_t i;
			for(i=0; i < m_size; i++)
				this->m_string[i] = buf[i];
			this->m_string[i] = '\0';
		}


		inline size_t GetStrLen(const char* str)
		{
			size_t i;
			for(i=0; str[i] != '\0'; i++);
			
			return i;
		}

		size_t Size()
		{
			return m_size;
		}

		size_t Capacity()
		{
			return m_capacity;
		}

		size_t find(const char* pattern)
		{
			size_t patternlen = GetStrLen(pattern);
			return SundayMatchString(m_string, m_size, pattern, patternlen);
		}

		size_t rfind(const char* pattern)
		{
			size_t patterlen = GetStrLen(pattern);
			for(size_t i = m_size - patterlen; i >= 0;  i--)
			{
				size_t j;
				for(j = 0; j < patterlen; j++)
				{
					if(m_string[i+j] != pattern[j])
						break;
				}
				if(j == patterlen)
					return i;
			}

			return -1;
		}

		void lower()
		{
			for(size_t i = 0; i < m_size; i++)
			{
				if(m_string[i] >= 'A' && m_string[i] <= 'Z')
				{
					m_string[i] += 32;
				}
			}
		}

		void upper()
		{
			for(size_t i = 0; i < m_size; i++)
			{
				if(m_string[i] >= 'a' && m_string[i] <= 'z')
				{
					m_string[i] -= 32;
				}
			}
		}

		const char* c_str()
		{
			return (const char*)m_string;
		}
		
		int count(const char* pattern)
		{
			size_t patternlen = GetStrLen(pattern);
			size_t ret = -1;
			int count = 0;
			const char* p = m_string;

			do 
			{
				p += ret + 1;
				ret = SundayMatchString(p, m_size-ret-1, pattern, patternlen);
				if(ret != -1)
					count++;
			} while(ret != -1);
			
			return count;
		}

		bool GetSub(basic_string* substring, size_t start, size_t len)
		{
			if(start + len > this->m_size)
				return false; 
			
			char* p = NULL;
			p = _Allocate(p, len+1);
			size_t i;
			for(i = 0; i < len; i++)
			{
				p[i] = this->m_string[i + start];
			}
			p[i] = '\0';
			*substring = p;
			_Free(p);

			return true;
		}

		int split(const char* x, CArrayVariable<basic_string *>& splitarray)
		{
			size_t len = GetStrLen(x);
			size_t ret = -1;
			size_t pos = 0;
			int count = 0;
			const char* p = m_string;
			basic_string* tmp = NULL;

			do 
			{
				pos += ret + 1;
				p += ret + 1;
				ret = SundayMatchString(p, m_size-ret-1, x, len);
				if(ret != -1)
				{
					tmp = _Allocate(tmp, 1);
					memset(tmp,	0,	sizeof(tmp));
					if(this->GetSub(tmp, pos, ret - pos));
				}
			} while (ret != -1);
			
			//最后一个子串
			tmp = _Allocate(tmp, 1);
			memset(tmp,	0,	sizeof(tmp));
			this->GetSub(tmp, pos, m_size - pos);

			return count;
		}
	
	private:
		char*	m_string;
		size_t	m_size;
		size_t	m_capacity;
	};
}

#endif