#ifndef __BASIC_STREAM_H__
#define __BASIC_STREAM_H__

#include "buffer.h"
#include "common.h"
#include "Poco/SharedPtr.h"

class BasicStream : public Buffer
{
public:
    BasicStream();
    virtual ~BasicStream();

    void clear();

    void swap(BasicStream& other);

    void resize(Container::size_type sz);

    void readSize(int32& v);
    void writeSize(int32 v);
    void rewriteSize(int32 v, Container::iterator dest);

    void write(byte v);
    void read(byte& v);

    void write(bool v);
    void read(bool& v);

    void write(int16 v);
    void write(uint16 v);
    void read(int16& v);
    void read(uint16& v);

    void write(int32 v);
    void write(uint32 v);
    void read(int32& v);
    void read(uint32& v);

    void write(int64 v);
    void read(int64& v);

    void write(float v);
    void read(float& v);

    void write(double v);
    void read(double& v);

    void write(const char*);
    void write(const std::string& v);
    void read(std::string& v);

    void write(const std::wstring& v);
    void read(std::wstring& v);

    unsigned left_size()
    {
        return b.end() - i;
    }

    int32 pass_size()
    {
        return (i - b.begin());
    }

    static void write(byte* dest, int v)
    {
    #ifdef VENUS_BIG_ENDIAN
        const byte* src = reinterpret_cast<const byte*>(&v) + sizeof(int32) - 1;
        *dest++ = *src--;
        *dest++ = *src--;
        *dest++ = *src--;
        *dest = *src;
    #else
        const byte* src = reinterpret_cast<const byte*>(&v);
        *dest++ = *src++;
        *dest++ = *src++;
        *dest++ = *src++;
        *dest = *src;
    #endif
    }

    static void read(const byte* src, int & v)
    {
    #ifdef VENUS_BIG_ENDIAN
        byte* dest = reinterpret_cast<byte*>(&v) + sizeof(int32) - 1;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest = *src;
    #else
        byte* dest = reinterpret_cast<byte*>(&v);
        *dest++ = *src++;
        *dest++ = *src++;
        *dest++ = *src++;
        *dest = *src;
    #endif
    }
    
    static void read(const byte* src, uint32 & v)
	{
	#ifdef VENUS_BIG_ENDIAN
		byte* dest = reinterpret_cast<byte*>(&v) + sizeof(int32) - 1;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest = *src;
	#else
		byte* dest = reinterpret_cast<byte*>(&v);
		*dest++ = *src++;
		*dest++ = *src++;
		*dest++ = *src++;
		*dest = *src;
	#endif
	}

    static int32 read(const byte* src)
	{
    	int v = 0;
	#ifdef VENUS_BIG_ENDIAN
		byte* dest = reinterpret_cast<byte*>(&v) + sizeof(int32) - 1;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest = *src;
	#else
		byte* dest = reinterpret_cast<byte*>(&v);
		*dest++ = *src++;
		*dest++ = *src++;
		*dest++ = *src++;
		*dest = *src;
	#endif
		return v;
	}

    void pop_front(size_t n)
	{
		if (n <= 0)
		{
			return;
		}

		if (n >= b.size())
		{
			b.clear();
		}

		b.resize(b.size() - n);
		::memmove(b.begin(), b.begin() + n, b.size());
		i = b.begin();
	}

    void append(const byte* ptr, size_t n)
	{
    	if (n <= 0) return;
		int size = b.size();
		resize(size + n);
		memcpy(b.begin() + size, ptr, n);
	}

private:
    const Container::size_type _messageSizeMax;
};

typedef Poco::AutoPtr<BasicStream> BasicStreamPtr;

#endif
