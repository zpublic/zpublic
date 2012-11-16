/*
 * string.hpp
 *
 *  Created on: 2012-11-14
 *      Author: kevin
 */

#ifndef _STRING_HPP_
#define _STRING_HPP_

#include "memory/allocer.hpp"
#include "algorithm/stringmatch.hpp"
#include "stdlib.h"

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
		inline size_t GetStrLen(const char* str)
		{
			size_t i;
			for(i=0; str[i] != '\0'; i++);
			
			return i;
		}

		size_t find(char* pattern)
		{
			size_t patternlen = GetStrLen(pattern);
			return SundayMatchString(m_string, m_size, pattern, patternlen);
		}

		size_t rfind(char* pattern)
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

	private:
		char*	m_string;
		size_t	m_size;
		size_t	m_capacity;
	};
}

#endif