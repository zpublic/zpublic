#include "basic_stream.h"

BasicStream::BasicStream():
Buffer(8192),
_messageSizeMax(8192)
{
}

BasicStream::~BasicStream()
{
	i = 0;
	b.clear();
}

void BasicStream::clear()
{
	i = 0;
	b.clear();
}

void BasicStream::swap(BasicStream& other)
{
    swapBuffer(other);
}

void BasicStream::resize(Container::size_type sz)
{
    int iPos = 0;
    if (0 != i)
    {
        iPos = i - b.begin();
    }

    b.resize(sz);

    if (0 == i)
    {
    	i = b.begin();
    }
    else
    {
        i = b.begin() + iPos;
    }
}

void BasicStream::readSize(int32& v)
{
	byte byte;
	read(byte);
	unsigned val = static_cast<unsigned char>(byte);
	if(val == 255)
	{
		read(v);
		if(v < 0)
		{
			throw("UnmarshalOutOfBounds exception.");
		}
	}
	else
	{
		v = static_cast<int32>(static_cast<unsigned char>(byte));
	}
}

inline void BasicStream::writeSize(int32 v)
{
    assert(v >= 0);
    if(v > 254)
    {
        write(byte(255));
        write(v);
    }
    else
    {
        write(static_cast<byte>(v));
    }
}

void BasicStream::rewriteSize(int32 v, Container::iterator dest)
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

void BasicStream::read(byte& v)
{
	if(i >= b.end())
	{
		throw("UnmarshalOutOfBounds exception.");
	}
	v = *i++;
}

void BasicStream::write(bool v)
{
    b.push_back(static_cast<byte>(v));
}

void BasicStream::read(bool& v)
{
	if(i >= b.end())
	{
		throw("UnmarshalOutOfBounds exception.");
	}
	v = (*i++) == 0 ? false : true;
}

void BasicStream::write(byte v)
{
    b.push_back(v);
}

void BasicStream::write(int16 v)
{
    Container::size_type pos = b.size();
    resize(pos + sizeof(short));
    byte* dest = &b[pos];
#ifdef VENUS_BIG_ENDIAN
    const byte* src = reinterpret_cast<const byte*>(&v) + sizeof(short) - 1;
    *dest++ = *src--;
    *dest = *src;
#else
    const byte* src = reinterpret_cast<const byte*>(&v);
    *dest++ = *src++;
    *dest = *src;
#endif
}

void BasicStream::write(uint16 v)
{
    write((int16)v);
}

void BasicStream::read(int16& v)
{
    if(b.end() - i < static_cast<int32>(sizeof(short)))
    {
    	throw("UnmarshalOutOfBounds exception.");
    }
    const byte* src = &(*i);
    i += sizeof(short);
#ifdef VENUS_BIG_ENDIAN
    byte* dest = reinterpret_cast<byte*>(&v) + sizeof(short) - 1;
    *dest-- = *src++;
    *dest = *src;
#else
    byte* dest = reinterpret_cast<byte*>(&v);
    *dest++ = *src++;
    *dest = *src;
#endif
}

void BasicStream::read(uint16& v)
{
    read((int16&)v);
}

void BasicStream::write(int32 v) // Inlined for performance reasons.
{
	Container::size_type pos = b.size();
	resize(pos + sizeof(int32));
	byte* dest = &b[pos];
#ifdef VENUS_BIG_ENDIAN
	const byte* src = (const byte*)(&v) + sizeof(int32) - 1;
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

void BasicStream::write(uint32 v)
{
	write((int32)v);
}

void BasicStream::read(int32& v)
{
	if(b.end() - i < static_cast<int32>(sizeof(int32)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}
	const byte* src = &(*i);
	i += sizeof(int32);
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

void BasicStream::read(uint32& v)
{
	if(b.end() - i < static_cast<int32>(sizeof(uint32)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}
	const byte* src = &(*i);
	i += sizeof(int32);
#ifdef VENUS_BIG_ENDIAN
	byte* dest = reinterpret_cast<byte*>(&v) + sizeof(uint32) - 1;
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

void BasicStream::write(int64 v)
{
    Container::size_type pos = b.size();
    resize(pos + sizeof(int64));
    byte* dest = &b[pos];
#ifdef VENUS_BIG_ENDIAN
    const byte* src = reinterpret_cast<const byte*>(&v) + sizeof(int64) - 1;
    *dest++ = *src--;
    *dest++ = *src--;
    *dest++ = *src--;
    *dest++ = *src--;
    *dest++ = *src--;
    *dest++ = *src--;
    *dest++ = *src--;
    *dest = *src;
#else
    const byte* src = reinterpret_cast<const byte*>(&v);
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest = *src;
#endif
}

void BasicStream::read(int64& v)
{
    if(b.end() - i < static_cast<int32>(sizeof(int64)))
    {
    	throw("UnmarshalOutOfBounds exception.");
    }
    const byte* src = &(*i);
    i += sizeof(int64);
#ifdef VENUS_BIG_ENDIAN
    byte* dest = reinterpret_cast<byte*>(&v) + sizeof(int64) - 1;
    *dest-- = *src++;
    *dest-- = *src++;
    *dest-- = *src++;
    *dest-- = *src++;
    *dest-- = *src++;
    *dest-- = *src++;
    *dest-- = *src++;
    *dest = *src;
#else
    byte* dest = reinterpret_cast<byte*>(&v);
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest = *src;
#endif
}

void BasicStream::write(float v)
{
    Container::size_type pos = b.size();
    resize(pos + sizeof(float));
    byte* dest = &b[pos];
#ifdef VENUS_BIG_ENDIAN
    const byte* src = reinterpret_cast<const byte*>(&v) + sizeof(float) - 1;
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

void BasicStream::read(float& v)
{
    if(b.end() - i < static_cast<int32>(sizeof(float)))
    {
    	throw("UnmarshalOutOfBounds exception.");
    }
    const byte* src = &(*i);
    i += sizeof(float);
#ifdef VENUS_BIG_ENDIAN
    byte* dest = reinterpret_cast<byte*>(&v) + sizeof(float) - 1;
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

void BasicStream::write(double v)
{
    Container::size_type pos = b.size();
    resize(pos + sizeof(double));
    byte* dest = &b[pos];
#ifdef VENUS_BIG_ENDIAN
    const byte* src = reinterpret_cast<const byte*>(&v) + sizeof(double) - 1;
    *dest++ = *src--;
    *dest++ = *src--;
    *dest++ = *src--;
    *dest++ = *src--;
    *dest++ = *src--;
    *dest++ = *src--;
    *dest++ = *src--;
    *dest = *src;
#else
    const byte* src = reinterpret_cast<const byte*>(&v);
#  if defined(__arm__) && defined(__linux)
    dest[4] = *src++;
    dest[5] = *src++;
    dest[6] = *src++;
    dest[7] = *src++;
    dest[0] = *src++;
    dest[1] = *src++;
    dest[2] = *src++;
    dest[3] = *src;
#  else
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest = *src;
#  endif
#endif
}

void BasicStream::read(double& v)
{
    if(b.end() - i < static_cast<int32>(sizeof(double)))
    {
    	throw("UnmarshalOutOfBounds exception.");
    }
    const byte* src = &(*i);
    i += sizeof(double);
#ifdef VENUS_BIG_ENDIAN
    byte* dest = reinterpret_cast<byte*>(&v) + sizeof(double) - 1;
    *dest-- = *src++;
    *dest-- = *src++;
    *dest-- = *src++;
    *dest-- = *src++;
    *dest-- = *src++;
    *dest-- = *src++;
    *dest-- = *src++;
    *dest = *src;
#else
    byte* dest = reinterpret_cast<byte*>(&v);
#  if defined(__arm__) && defined(__linux)
    dest[4] = *src++;
    dest[5] = *src++;
    dest[6] = *src++;
    dest[7] = *src++;
    dest[0] = *src++;
    dest[1] = *src++;
    dest[2] = *src++;
    dest[3] = *src;
#  else
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest++ = *src++;
    *dest = *src;
#  endif
#endif
}

void BasicStream::write(const std::string& v)
{
	int16 sz = static_cast<int16>(v.size());

	write(sz);
	if(sz > 0)
	{
		Container::size_type pos = b.size();
		resize(pos + sz);
		memcpy(&b[pos], v.data(), sz);
	}
}

void BasicStream::read(std::string& v)
{
	int16 sz;
	read(sz);
	if(sz > 0)
	{
		if(b.end() - i < (int32)sz)
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		std::string(reinterpret_cast<const char*>(&*i), reinterpret_cast<const char*>(&*i) + sz).swap(v);
		i += sz;
	}
	else
	{
		v.clear();
	}
}

void BasicStream::write(const std::wstring& v)
{
}


void BasicStream::read(std::wstring& v)
{
}