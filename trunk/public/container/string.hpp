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
#include "vector.hpp"

namespace zl
{
	class basic_string
	{
	public:
		basic_string():m_size(0),m_capacity(0),m_string(0)
		{
			
		}
		
		~basic_string()
		{
			_Free(m_string);
		}

		basic_string(const char* buf)
		{
			m_size = GetStrLen(buf);
			m_string = _Allocate(m_string, m_size * 2);
			m_capacity = m_size * 2;
			memcpy(m_string, buf, m_size + 1);
		}

		basic_string(const basic_string& x)
		{
			this->m_size = x.Size();
			this->m_capacity = x.Capacity();
			this->m_string = _Allocate(this->m_string, this->m_capacity);

			memcpy(this->m_string, x.c_str(), (this->m_size + 1) * sizeof(char));
		}

	public:

		basic_string& operator=(const basic_string& x)
		{
			if(x.Size() >= this->m_capacity)
            {
                if(this->m_string != NULL)
                    _Free(this->m_string);
                this->m_size = x.Size();
                this->m_capacity = x.Capacity();
                this->m_string = _Allocate(m_string, this->m_capacity);
            }
            
            this->m_size = x.Size();
			memcpy(this->m_string, x.c_str(), (x.Size()+1) * sizeof(char));
			return ( *this );
		}

		void operator=(const char* buf)
		{
			size_t len = GetStrLen(buf);
			if(len >= this->m_capacity)
			{
				if(this->m_string != NULL)
					_Free(this->m_string);
				this->m_size = GetStrLen(buf);
				this->m_string = _Allocate(m_string, m_size * 2);
				m_capacity = this->m_size * 2;
			}
			
			size_t i;
			for(i=0; i < m_size; i++)
				this->m_string[i] = buf[i];
			this->m_string[i] = '\0';
		}

		basic_string& operator+=(const char* buf)
		{
			size_t len = GetStrLen(buf);
			size_t oldsize = this->m_size;
			char* oldstring = this->m_string;
			bool flag = false;

			this->m_size += len;
			if(this->m_size >= this->m_capacity)
			{
				char* tmp = NULL;
				tmp = _Allocate(tmp, this->m_size + 1);
				this->m_capacity = this->m_size + 1;
				for(size_t i = 0; i < oldsize; i++)
					tmp[i] = this->m_string[i];
				this->m_string = tmp;
				flag = true;
			}
			size_t i;
			for(i = oldsize; i < this->m_size; i++)
				this->m_string[i] = buf[i-oldsize];
			this->m_string[i] = '\0';
			
			if(oldstring != NULL && flag)
				_Free(oldstring);

			return ( *this );
		}

		basic_string operator+(const char* buf)
		{
			basic_string tmp = *this;
			return tmp += buf;
		}

		basic_string& operator+=(basic_string& x)
		{
			return (*this += x.c_str());
		}

		basic_string operator+(basic_string& x)
		{
			basic_string tmp = *this;
			return tmp += x;
		}

		bool operator==(const basic_string& x)
		{
			return compare(x) == 0;
		}

		bool operator!=(const basic_string& x)
		{
			return compare(x) != 0;
		}

		bool operator>(const basic_string& x)
		{
			return compare(x) == 1;
		}

		bool operator<(const basic_string& x)
		{
			return compare(x) == -1;
		}

		bool operator<=(const basic_string& x)
		{
			return compare(x) <= 0;
		}

		bool operator>=(const basic_string& x)
		{
			return compare(x) >= 0;
		}

		int compare(const basic_string& x)
		{
			size_t minimal;
			bool flag = false;
			
			if(m_size < x.Size()) 
			{
				minimal = m_size;
				flag = true; //this字串比较短
			}
			else
			{
				minimal = x.Size();
			}

			const char* p = x.c_str();
			for(size_t i = 0; i < minimal; i++)
			{
				if(m_string[i] > p[i])
					return 1;
				else if(m_string[i] < p[i])
					return -1;
			}

			
			if(m_size == x.Size())
				return 0;
			else if(flag)
				return -1;
			else
				return 1;
			
		}

		inline size_t GetStrLen(const char* str)
		{
			size_t i;
			for(i=0; str[i] != '\0'; i++);
			
			return i;
		}

		size_t Size() const
		{
			return m_size;
		}

		size_t Capacity() const
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

		const char* c_str() const
		{
			return (const char*)m_string;
		}
		
		int count(const char* pattern)
		{
			size_t patternlen = GetStrLen(pattern);
			int ret = -1;
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

		basic_string GetSub(size_t start, size_t len)
		{
			if(start + len > this->m_size)
				return *this; 

			char* p = NULL;
			p = _Allocate(p, len+1);
			size_t i;
			for(i = 0; i < len; i++)
			{
				p[i] = this->m_string[i + start];
			}
			p[i] = '\0';
			basic_string substring = p;
			_Free(p);

			return substring;
		}

		int split(const char* x, zl::CSimpleVector<basic_string *>& vecSplit)
		{
			size_t len = GetStrLen(x);
			int ret = -1;
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
					tmp = new basic_string;
					if(this->GetSub(tmp, pos, ret))
					{
						vecSplit.Add(tmp);
						count++;
					}
				
				}
			} while (ret != -1);
			
			//最后一个子串
			tmp = new basic_string;
			this->GetSub(tmp, pos, m_size - pos);
			vecSplit.Add(tmp);
			count++;

			return count;
		}
		
		void swap(basic_string& x)
		{
			basic_string tmp;
			tmp = x;
			x = *this;
			*this = tmp;
		}

		basic_string replace(const char* pattern, const char* word)
		{
			size_t patlen = GetStrLen(pattern);
			size_t wordlen = GetStrLen(word);

			char* tmp = NULL;
			if(wordlen > patlen)
			{
				size_t maxlen = this->m_size / patlen * (wordlen - patlen) + this->m_size % patlen;
				tmp = _Allocate(tmp, maxlen);
			}
			else
			{
				tmp = _Allocate(tmp, this->m_capacity);
			}


			int ret = 0;
			size_t pos = 0;
			const char* p = this->m_string;
			char* q = tmp;

			do 
			{
				pos += ret;
				p += ret;
				q += ret;
				ret = SundayMatchString(p, m_size-ret-1, pattern, patlen);
				if(ret != -1)
				{
					memcpy(q, p, sizeof(char) * ret);
					memcpy(q + ret, word, sizeof(char)*wordlen);
					p += patlen;
					q += wordlen;
				}
			} while (ret != -1);

			memcpy(q, p, this->m_size - pos + 1);
			
			return basic_string(tmp);
		}

	private:
		size_t	m_size;
		size_t	m_capacity;
        char*   m_string;
	};
}

#endif
