#include "message_stream.h"

MessageStream& MessageStream::operator <<(const int8 v)
{
	b.push_back(v);

	return (*this);
}

MessageStream& MessageStream::operator <<(const uint8&v)
{
	b.push_back((int8)v);

	return (*this);
}

MessageStream& MessageStream::operator <<(const int16& v)
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

	return (*this);
}

MessageStream& MessageStream::operator <<(const uint16& v)
{
	(*this) << (int16)v;

	return (*this);
}

MessageStream& MessageStream::operator <<(const int32& v)
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

	return (*this);
}

MessageStream& MessageStream::operator <<(const uint32&v)
{
	(*this) << (int32)v;

	return (*this);
}

MessageStream& MessageStream::operator <<(const int64& v)
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

    return (*this);
}

MessageStream& MessageStream::operator <<(const uint64& v)
{
	(*this) << (int64)v;

	return (*this);
}

MessageStream& MessageStream::operator <<(const float& v)
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

	return (*this);
}

MessageStream& MessageStream::operator <<(const double& v)
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

	return (*this);
}

MessageStream& MessageStream::operator <<(const std::string& v)
{
	int32 sz = static_cast<int32>(v.size());
	(*this) << sz;
	if(sz > 0)
	{
		Container::size_type pos = b.size();
		resize(pos + sz);
		memcpy(&b[pos], v.data(), sz);
	}

	return (*this);
}

MessageStream& MessageStream::operator >>(int8& v)
{
	if(i >= b.end())
	{
		throw("UnmarshalOutOfBounds exception.");
	}
	v = *i++;

	return (*this);
}

MessageStream& MessageStream::operator >>(uint8& v)
{
	if(i >= b.end())
	{
		throw("UnmarshalOutOfBounds exception.");
	}
	v = *i++;

	return (*this);
}

MessageStream& MessageStream::operator >>(int16& v)
{
	if(b.end() - i < static_cast<int32>(sizeof(int16)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}
	const byte* src = &(*i);
	i += sizeof(int16);
#ifdef VENUS_BIG_ENDIAN
	byte* dest = reinterpret_cast<byte*>(&v) + sizeof(short) - 1;
	*dest-- = *src++;
	*dest = *src;
#else
	byte* dest = reinterpret_cast<byte*>(&v);
	*dest++ = *src++;
	*dest = *src;
#endif

	return (*this);
}

MessageStream& MessageStream::operator >>(uint16& v)
{
	(*this) >> (int64)v;

	return (*this);
}

MessageStream& MessageStream::operator >>(int32& v)
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

	return (*this);
}

MessageStream& MessageStream::operator >>(uint32& v)
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

	return (*this);
}

MessageStream& MessageStream::operator >>(const int64& v)
{
	if(b.end() - i < static_cast<int32>(sizeof(int64)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}
	const byte* src = &(*i);
	i += sizeof(int64);
#ifdef VENUS_BIG_ENDIAN
	byte* dest = (byte*)(&v) + sizeof(int64) - 1;
	*dest-- = *src++;
	*dest-- = *src++;
	*dest-- = *src++;
	*dest-- = *src++;
	*dest-- = *src++;
	*dest-- = *src++;
	*dest-- = *src++;
	*dest = *src;
#else
	byte* dest = (byte*)(&v);
	*dest++ = *src++;
	*dest++ = *src++;
	*dest++ = *src++;
	*dest++ = *src++;
	*dest++ = *src++;
	*dest++ = *src++;
	*dest++ = *src++;
	*dest = *src;
#endif

	return (*this);
}

MessageStream& MessageStream::operator >>(const uint64& v)
{
	if(b.end() - i < static_cast<int32>(sizeof(uint64)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}
	const byte* src = &(*i);
	i += sizeof(int64);
#ifdef VENUS_BIG_ENDIAN
	byte* dest = (byte*)(&v) + sizeof(uint64) - 1;
	*dest-- = *src++;
	*dest-- = *src++;
	*dest-- = *src++;
	*dest-- = *src++;
	*dest-- = *src++;
	*dest-- = *src++;
	*dest-- = *src++;
	*dest = *src;
#else
	byte* dest = (byte*)(&v);
	*dest++ = *src++;
	*dest++ = *src++;
	*dest++ = *src++;
	*dest++ = *src++;
	*dest++ = *src++;
	*dest++ = *src++;
	*dest++ = *src++;
	*dest = *src;
#endif

	return (*this);
}

MessageStream& MessageStream::operator >>(float& v)
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

	return (*this);
}

MessageStream& MessageStream::operator >>(double& v)
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

	return (*this);
}

MessageStream& MessageStream::operator >>(std::string& v)
{
	int sz = 0;
	(*this) >> sz;
	if(sz > 0)
	{
		if(b.end() - i < sz)
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		std::string(reinterpret_cast<const char*>(&*i), reinterpret_cast<const char*>(&*i) + sz).swap(v);
		i += sz;
	}
	else
	{
	}

	return (*this);
}

void MessageStream::resize(uint32 sz)
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
