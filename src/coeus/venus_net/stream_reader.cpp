#include "stream_reader.h"

StreamReader::StreamReader(const char* buff, int size) :
_buff(buff),
_size(size),
_offset(0)
{
}

StreamReader::StreamReader(const byte* buff, int32 size) :
    _buff((const char*)buff),
    _size(size),
    _offset(0)
{

}

StreamReader::~StreamReader()
{

}

StreamReader& StreamReader::read(char* buff, int32 size)
{
	if(_size - _offset < size)
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	memcpy(buff, &_buff[_offset], size);
	_offset += size;

	return (*this);
}

StreamReader& StreamReader::read(byte* buff, int32 size)
{
    return read((char*)buff, size);
}

StreamReader& StreamReader::operator >>(int8& v)
{
	if(_size - _offset < static_cast<int32>(sizeof(int8)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	v = _buff[_offset++];

	return (*this);
}

StreamReader& StreamReader::operator >>(vector<int8> &v)
{
	int16 sz = 0;
	(*this) >> sz;

	v.resize(sz);
	for (int16 i = 0; i < sz; i++)
	{
		if(_size - _offset < static_cast<int32>(sizeof(int8)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}
		v[i] = _buff[_offset++];
	}
	return (*this);
}

StreamReader& StreamReader::operator >>(uint8& v)
{
	if(_size - _offset < static_cast<int32>(sizeof(uint8)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	v = _buff[_offset++];

	return (*this);
}

StreamReader& StreamReader::operator >>(vector<uint8> &v)
{
	int16 sz = 0;
	(*this) >> sz;

	v.resize(sz);
	for (int16 i = 0; i < sz; i++)
	{
		if(_size - _offset < static_cast<int32>(sizeof(uint8)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}
		v[i] = _buff[_offset++];
	}

	return (*this);
}

StreamReader& StreamReader::operator >>(int16& v)
{
	if(_size - _offset < static_cast<int32>(sizeof(int16)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	const char* src = &_buff[_offset];
	_offset += sizeof(int16);
#ifdef VENUS_BIG_ENDIAN
	char* dest = reinterpret_cast<char*>(&v) + sizeof(int16) - 1;
	*dest-- = *src++;
	*dest = *src;
#else
	byte* dest = reinterpret_cast<byte*>(&v);
	*dest++ = *src++;
	*dest = *src;
#endif

	return (*this);
}

StreamReader& StreamReader::operator >>(vector<int16> &v)
{
	int16 sz = 0;
	(*this) >> sz;

	v.resize(sz);
	for (int16 i = 0; i < sz; i++)
	{
		if(_size - _offset < static_cast<int32>(sizeof(int16)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		const char* src = &_buff[_offset];
		_offset += sizeof(int16);
	#ifdef VENUS_BIG_ENDIAN
		char* dest = reinterpret_cast<char*>(&v[i]) + sizeof(int16) - 1;
		*dest-- = *src++;
		*dest = *src;
	#else
		byte* dest = reinterpret_cast<byte*>(&v[i]);
		*dest++ = *src++;
		*dest = *src;
	#endif
	}

	return (*this);
}

StreamReader& StreamReader::operator >>(uint16& v)
{
	if(_size - _offset < static_cast<int32>(sizeof(uint16)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	const char* src = &_buff[_offset];
	_offset += sizeof(uint16);
#ifdef VENUS_BIG_ENDIAN
	char* dest = (char*)(&v) + sizeof(uint16) - 1;
	*dest-- = *src++;
	*dest = *src;
#else
	byte* dest = reinterpret_cast<byte*>(&v);
	*dest++ = *src++;
	*dest = *src;
#endif

	return (*this);
}

StreamReader& StreamReader::operator >>(vector<uint16> &v)
{
	int16 sz = 0;
	(*this) >> sz;

	v.resize(sz);
	for (int16 i = 0; i < sz; i++)
	{
		if(_size - _offset < static_cast<int32>(sizeof(uint16)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		const char* src = &_buff[_offset];
		_offset += sizeof(int16);
	#ifdef VENUS_BIG_ENDIAN
		char* dest = reinterpret_cast<char*>(&v[i]) + sizeof(uint16) - 1;
		*dest-- = *src++;
		*dest = *src;
	#else
		byte* dest = reinterpret_cast<byte*>(&v[i]);
		*dest++ = *src++;
		*dest = *src;
	#endif
	}

	return (*this);
}

StreamReader& StreamReader::operator >>(int32& v)
{
	if(_size - _offset < static_cast<int32>(sizeof(int32)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	const char* src = &_buff[_offset];
	_offset += sizeof(int32);
#ifdef VENUS_BIG_ENDIAN
	char* dest = reinterpret_cast<char*>(&v) + sizeof(int32) - 1;
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

StreamReader& StreamReader::operator >>(vector<int32> &v)
{
	int16 sz = 0;
	(*this) >> sz;

	v.resize(sz);
	for (int16 i = 0; i < sz; i++)
	{
		if(_size - _offset < static_cast<int32>(sizeof(int32)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		const char* src = &_buff[_offset];
		_offset += sizeof(int32);
	#ifdef VENUS_BIG_ENDIAN
		char* dest = reinterpret_cast<char*>(&v[i]) + sizeof(int32) - 1;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest = *src;
	#else
		byte* dest = reinterpret_cast<byte*>(&v[i]);
		*dest++ = *src++;
		*dest++ = *src++;
		*dest++ = *src++;
		*dest = *src;
	#endif
	}

	return (*this);
}

StreamReader& StreamReader::operator >>(uint32& v)
{
	if(_size - _offset < static_cast<int32>(sizeof(uint32)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	const char* src = &_buff[_offset];
	_offset += sizeof(uint32);
#ifdef VENUS_BIG_ENDIAN
	char* dest = reinterpret_cast<char*>(&v) + sizeof(uint32) - 1;
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

StreamReader& StreamReader::operator >>(vector<uint32> &v)
{
	int16 sz = 0;
	(*this) >> sz;

	v.resize(sz);
	for (int16 i = 0; i < sz; i++)
	{
		if(_size - _offset < static_cast<int32>(sizeof(uint32)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		const char* src = &_buff[_offset];
		_offset += sizeof(uint32);
	#ifdef VENUS_BIG_ENDIAN
		char* dest = reinterpret_cast<char*>(&v[i]) + sizeof(int32) - 1;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest = *src;
	#else
		byte* dest = reinterpret_cast<byte*>(&v[i]);
		*dest++ = *src++;
		*dest++ = *src++;
		*dest++ = *src++;
		*dest = *src;
	#endif
	}

	return (*this);
}

StreamReader& StreamReader::operator >>(int64& v)
{
	if(_size - _offset < static_cast<int32>(sizeof(int64)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	const char* src = &_buff[_offset];
	_offset += sizeof(int64);
#ifdef VENUS_BIG_ENDIAN
	char* dest = (char*)(&v) + sizeof(int64) - 1;
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

StreamReader& StreamReader::operator >>(vector<int64> &v)
{
	int16 sz = 0;
	(*this) >> sz;

	v.resize(sz);
	for (int16 i = 0; i < sz; i++)
	{
		if(_size - _offset < static_cast<int32>(sizeof(int64)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		const char* src = &_buff[_offset];
		_offset += sizeof(int64);
	#ifdef VENUS_BIG_ENDIAN
		char* dest = (char*)(&v[i]) + sizeof(int64) - 1;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest = *src;
	#else
		byte* dest = (byte*)(&v[i]);
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

StreamReader& StreamReader::operator >>(uint64& v)
{
	if(_size - _offset < static_cast<int32>(sizeof(uint64)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	const char* src = &_buff[_offset];
	_offset += sizeof(uint64);
#ifdef VENUS_BIG_ENDIAN
	char* dest = (char*)(&v) + sizeof(uint64) - 1;
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

StreamReader& StreamReader::operator >>(vector<uint64> &v)
{
	int16 sz = 0;
	(*this) >> sz;

	v.resize(sz);
	for (int16 i = 0; i < sz; i++)
	{
		if(_size - _offset < static_cast<int32>(sizeof(uint64)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		const char* src = &_buff[_offset];
		_offset += sizeof(uint64);
	#ifdef VENUS_BIG_ENDIAN
		char* dest = (char*)(&v[i]) + sizeof(uint64) - 1;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest = *src;
	#else
		byte* dest = (byte*)(&v[i]);
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

StreamReader& StreamReader::operator >>(float& v)
{
	if(_size - _offset < static_cast<int32>(sizeof(float)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	const char* src = &_buff[_offset];
	_offset += sizeof(float);
#ifdef VENUS_BIG_ENDIAN
	char* dest = reinterpret_cast<char*>(&v) + sizeof(float) - 1;
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

StreamReader& StreamReader::operator >>(vector<float> &v)
{
	int16 sz = 0;
	(*this) >> sz;

	v.resize(sz);
	for (int16 i = 0; i < sz; i++)
	{
		if(_size - _offset < static_cast<int32>(sizeof(float)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		const char* src = &_buff[_offset];
		_offset += sizeof(float);
	#ifdef VENUS_BIG_ENDIAN
		char* dest = reinterpret_cast<char*>(&v[i]) + sizeof(float) - 1;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest = *src;
	#else
		byte* dest = reinterpret_cast<byte*>(&v[i]);
		*dest++ = *src++;
		*dest++ = *src++;
		*dest++ = *src++;
		*dest = *src;
	#endif
	}

	return (*this);
}

StreamReader& StreamReader::operator >>(double& v)
{
	if(_size - _offset < static_cast<int32>(sizeof(double)))
	{
		throw("UnmarshalOutOfBounds exception.");
	}

	const char* src = &_buff[_offset];
	_offset += sizeof(double);
#ifdef VENUS_BIG_ENDIAN
	char* dest = reinterpret_cast<char*>(&v) + sizeof(double) - 1;
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

StreamReader& StreamReader::operator >>(vector<double> &v)
{
	int16 sz = 0;
	(*this) >> sz;

	v.resize(sz);
	for (int16 i = 0; i < sz; i++)
	{
		if(_size - _offset < static_cast<int32>(sizeof(double)))
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		const char* src = &_buff[_offset];
		_offset += sizeof(double);
	#ifdef VENUS_BIG_ENDIAN
		char* dest = reinterpret_cast<char*>(&v[i]) + sizeof(double) - 1;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest-- = *src++;
		*dest = *src;
	#else
		byte* dest = reinterpret_cast<byte*>(&v[i]);
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

StreamReader& StreamReader::operator >>(std::string& v)
{
	int16 sz = 0;
	(*this) >> sz;
	if(sz > 0)
	{
		if(_size - _offset < sz)
		{
			throw("UnmarshalOutOfBounds exception.");
		}

		const char* dest = &_buff[_offset];
		std::string((const char*)(dest), (const char*)(dest + sz)).swap(v);
		_offset += sz;
	}
	else
	{
	}
	return (*this);
}

StreamReader& StreamReader::operator >>(vector<string> &v)
{
	int16 sz = 0;
	(*this) >> sz;

	v.resize(sz);
	for (int16 i = 0; i < sz; i++)
	{
		int16 sz1 = 0;
		(*this) >> sz1;
		if(sz1 > 0)
		{
			if(_size - _offset < sz1)
			{
				throw("UnmarshalOutOfBounds exception.");
			}

			const char* dest = &_buff[_offset];
			std::string((const char*)(dest), (const char*)(dest + sz1)).swap(v[i]);
			_offset += sz1;
		}
		else
		{
		}
	}
	return (*this);
}
