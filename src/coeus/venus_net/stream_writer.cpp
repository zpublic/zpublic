#include "stream_writer.h"

StreamWriter::StreamWriter(char* buff, int size) :
_buff(buff),
_size(size),
_offset(0)
{
}

StreamWriter::StreamWriter(byte* buff, int size) :
    _buff((char*)buff),
    _size(size),
    _offset(0)
{
}

StreamWriter::~StreamWriter()
{
}

void StreamWriter::write(const char* buff, int32 size)
{
	if ((_size - _offset) < size)
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	memcpy(&_buff[_offset], buff, size);
	_offset += size;
}

void StreamWriter::write(const byte* buff, int32 size)
{
    write((const char*)buff, size);
}

StreamWriter & StreamWriter::operator <<(const int8 v)
{
	if ((_size - _offset) < static_cast<int32>(sizeof(int8)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	_buff[_offset++] = v;
	return (*this);
}

StreamWriter & StreamWriter::operator <<(const vector<int8> v)
{
	(*this) << (int16)v.size();
	for (uint32 i = 0; i < v.size(); i++)
	{
		if ((_size - _offset) < static_cast<int32>(sizeof(int8)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		_buff[_offset++] = v[i];
	}
	return (*this);
}

StreamWriter & StreamWriter::operator <<(const uint8 v)
{
	if ((_size - _offset) < static_cast<int32>(sizeof(uint8)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	_buff[_offset++] = v;
	return (*this);
}

StreamWriter & StreamWriter::operator <<(const vector<uint8> v)
{
	(*this) << (int16)v.size();
	for (uint32 i = 0; i < v.size(); i++)
	{
		if ((_size - _offset) < static_cast<int32>(sizeof(uint8)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		_buff[_offset++] = v[i];
	}
	return (*this);
}

StreamWriter & StreamWriter::operator <<(const int16 v)
{
	if ((_size - _offset) < static_cast<int32>(sizeof(int16)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	char* dest = &_buff[_offset];
	_offset += sizeof(int16);
#ifdef VENUS_BIG_ENDIAN
	const byte* src = reinterpret_cast<const byte*>(&v) + sizeof(int16) - 1;
	*dest++ = *src--;
	*dest = *src;
#else
	const byte* src = reinterpret_cast<const byte*>(&v);
	*dest++ = *src++;
	*dest = *src;
#endif

	return (*this);
}

StreamWriter & StreamWriter::operator <<(const vector<int16> v)
{
	(*this) << (int16)v.size();
	for (uint32 i = 0; i < v.size(); i++)
	{
		if ((_size - _offset) < static_cast<int32>(sizeof(int16)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		char* dest = &_buff[_offset];
		_offset += sizeof(int16);
	#ifdef VENUS_BIG_ENDIAN
		const byte* src = reinterpret_cast<const byte*>(&v[i]) + sizeof(int16) - 1;
		*dest++ = *src--;
		*dest = *src;
	#else
		const byte* src = reinterpret_cast<const byte*>(&v[i]);
		*dest++ = *src++;
		*dest = *src;
	#endif
	}

	return (*this);
}

StreamWriter & StreamWriter::operator <<(const uint16 v)
{
	if ((_size - _offset) < static_cast<int32>(sizeof(uint16)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	char* dest = &_buff[_offset];
	_offset += sizeof(uint16);
#ifdef VENUS_BIG_ENDIAN
	const byte* src = reinterpret_cast<const byte*>(&v) + sizeof(uint16) - 1;
	*dest++ = *src--;
	*dest = *src;
#else
	const byte* src = reinterpret_cast<const byte*>(&v);
	*dest++ = *src++;
	*dest = *src;
#endif

	return (*this);
}

StreamWriter & StreamWriter::operator <<(const vector<uint16> v)
{
	(*this) << (int16)v.size();
	for (uint32 i = 0; i < v.size(); i++)
	{
		if ((_size - _offset) < static_cast<int32>(sizeof(uint16)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		char* dest = &_buff[_offset];
		_offset += sizeof(int16);
	#ifdef VENUS_BIG_ENDIAN
		const byte* src = reinterpret_cast<const byte*>(&v[i]) + sizeof(uint16) - 1;
		*dest++ = *src--;
		*dest = *src;
	#else
		const byte* src = reinterpret_cast<const byte*>(&v[i]);
		*dest++ = *src++;
		*dest = *src;
	#endif
	}

	return (*this);
}

StreamWriter & StreamWriter::operator <<(const int32 v)
{
	if ((_size - _offset) < static_cast<int32>(sizeof(int32)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	char* dest = &_buff[_offset];
	_offset += sizeof(int32);
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

StreamWriter & StreamWriter::operator <<(const vector<int32> v)
{
	(*this) << (int16)v.size();
	for (uint32 i = 0; i < v.size(); i++)
	{
		if ((_size - _offset) < static_cast<int32>(sizeof(int32)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		char* dest = &_buff[_offset];
		_offset += sizeof(int32);
	#ifdef VENUS_BIG_ENDIAN
		const byte* src = (const byte*)(&v[i]) + sizeof(int32) - 1;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest = *src;
	#else
		const byte* src = reinterpret_cast<const byte*>(&v[i]);
		*dest++ = *src++;
		*dest++ = *src++;
		*dest++ = *src++;
		*dest = *src;
	#endif
	}

	return (*this);
}

StreamWriter & StreamWriter::operator <<(const uint32 v)
{
	if ((_size - _offset) < static_cast<int32>(sizeof(uint32)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	char* dest = &_buff[_offset];
	_offset += sizeof(uint32);
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

StreamWriter & StreamWriter::operator <<(const vector<uint32> v)
{
	(*this) << (int16)v.size();
	for (uint32 i = 0; i < v.size(); i++)
	{
		if ((_size - _offset) < static_cast<int32>(sizeof(uint32)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		char* dest = &_buff[_offset];
		_offset += sizeof(int32);
	#ifdef VENUS_BIG_ENDIAN
		const byte* src = (const byte*)(&v[i]) + sizeof(uint32) - 1;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest = *src;
	#else
		const byte* src = reinterpret_cast<const byte*>(&v[i]);
		*dest++ = *src++;
		*dest++ = *src++;
		*dest++ = *src++;
		*dest = *src;
	#endif
	}

	return (*this);
}

StreamWriter & StreamWriter::operator <<(const int64 v)
{
	if ((_size - _offset) < static_cast<int32>(sizeof(int64)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	char* dest = &_buff[_offset];
	_offset += sizeof(int64);
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

StreamWriter & StreamWriter::operator <<(const vector<int64> v)
{
	(*this) << (int16)v.size();
	for (uint32 i = 0; i < v.size(); i++)
	{
		if ((_size - _offset) < static_cast<int32>(sizeof(int64)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		char* dest = &_buff[_offset];
		_offset += sizeof(int64);
	#ifdef VENUS_BIG_ENDIAN
		const byte* src = reinterpret_cast<const byte*>(&v[i]) + sizeof(int64) - 1;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest = *src;
	#else
		const byte* src = reinterpret_cast<const byte*>(&v[i]);
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

    return (*this);
}

StreamWriter & StreamWriter::operator <<(const uint64 v)
{
	if ((_size - _offset) < static_cast<int32>(sizeof(uint64)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	char* dest = &_buff[_offset];
	_offset += sizeof(uint64);
#ifdef VENUS_BIG_ENDIAN
    const byte* src = reinterpret_cast<const byte*>(&v) + sizeof(uint64) - 1;
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

StreamWriter & StreamWriter::operator <<(const vector<uint64> v)
{
	(*this) << (int16)v.size();
	for (uint32 i = 0; i < v.size(); i++)
	{
		if ((_size - _offset) < static_cast<int32>(sizeof(uint64)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		char* dest = &_buff[_offset];
		_offset += sizeof(int64);
	#ifdef VENUS_BIG_ENDIAN
		const byte* src = reinterpret_cast<const byte*>(&v[i]) + sizeof(uint64) - 1;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest = *src;
	#else
		const byte* src = reinterpret_cast<const byte*>(&v[i]);
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

    return (*this);
}

StreamWriter & StreamWriter::operator <<(const float v)
{
	if ((_size - _offset) < static_cast<int32>(sizeof(float)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	char* dest = &_buff[_offset];
	_offset += sizeof(float);
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

StreamWriter & StreamWriter::operator <<(const vector<float> v)
{
	(*this) << (int16)v.size();
	for (uint32 i = 0; i < v.size(); i++)
	{
		if ((_size - _offset) < static_cast<int32>(sizeof(float)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		char* dest = &_buff[_offset];
		_offset += sizeof(float);
	#ifdef VENUS_BIG_ENDIAN
		const byte* src = reinterpret_cast<const byte*>(&v[i]) + sizeof(float) - 1;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest = *src;
	#else
		const byte* src = reinterpret_cast<const byte*>(&v[i]);
		*dest++ = *src++;
		*dest++ = *src++;
		*dest++ = *src++;
		*dest = *src;
	#endif
	}

	return (*this);
}

StreamWriter & StreamWriter::operator <<(const double v)
{
	if ((_size - _offset) < static_cast<int32>(sizeof(double)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	char* dest = &_buff[_offset];
	_offset += sizeof(double);
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

StreamWriter & StreamWriter::operator <<(const vector<double> v)
{
	(*this) << (int16)v.size();
	for (uint32 i = 0; i < v.size(); i++)
	{
		if ((_size - _offset) < static_cast<int32>(sizeof(double)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		char* dest = &_buff[_offset];
		_offset += sizeof(double);
	#ifdef VENUS_BIG_ENDIAN
		const byte* src = reinterpret_cast<const byte*>(&v[i]) + sizeof(double) - 1;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest++ = *src--;
		*dest = *src;
	#else
		const byte* src = reinterpret_cast<const byte*>(&v[i]);
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

	return (*this);
}

StreamWriter & StreamWriter::operator <<(const string &v)
{
	int16 sz = static_cast<int16>(v.size());
	(*this) << sz;
	if(sz > 0)
	{
		if ((_size - _offset) < sz)
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		char* dest = &_buff[_offset];
		memcpy(dest, v.data(), sz);
		_offset += sz;
	}

	return (*this);
}

StreamWriter & StreamWriter::operator <<(const vector<string> &v)
{
	(*this) << (int16)v.size();
	for (uint32 i = 0; i < v.size(); i++)
	{
		int16 sz = static_cast<int16>(v.size());
		(*this) << sz;
		if(sz > 0)
		{
			if ((_size - _offset) < sz)
			{
				throw("UnmarshalOutOfBounds exception.");
			}

			char* dest = &_buff[_offset];
			memcpy(dest, v[i].data(), sz);
			_offset += sz;
		}
	}
	return (*this);
}