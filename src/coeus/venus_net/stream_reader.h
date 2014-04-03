#ifndef __STREAM_READER_H__
#define __STREAM_READER_H__

#include "common.h"
using namespace std;

class StreamReader
{
public:
	StreamReader(const char* buff, int32 size);
    StreamReader(const byte* buff, int32 size);
	~StreamReader();

	StreamReader& read(char* buff, int32 size);
    StreamReader& read(byte* buff, int32 size);

	StreamReader& operator >>(int8& v);
	StreamReader& operator >>(vector<int8> &v);
	StreamReader& operator >>(uint8& v);
	StreamReader& operator >>(vector<uint8> &v);
	StreamReader& operator >>(int16& v);
	StreamReader& operator >>(vector<int16> &v);
	StreamReader& operator >>(uint16& v);
	StreamReader& operator >>(vector<uint16> &v);
	StreamReader& operator >>(int32& v);
	StreamReader& operator >>(vector<int32> &v);
	StreamReader& operator >>(uint32& v);
	StreamReader& operator >>(vector<uint32> &v);
	StreamReader& operator >>(int64& v);
	StreamReader& operator >>(vector<int64> &v);
	StreamReader& operator >>(uint64& v);
	StreamReader& operator >>(vector<uint64> &v);
	StreamReader& operator >>(float& v);
	StreamReader& operator >>(vector<float> &v);
	StreamReader& operator >>(double& v);
	StreamReader& operator >>(vector<double> &v);
	StreamReader& operator >>(string &v);
	StreamReader& operator >>(vector<string> &v);

public:
	const char* _buff;
	int32 _size;
	int32 _offset;
};

#endif

