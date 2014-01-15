#ifndef __STREAM_WRITER_H__
#define __STREAM_WRITER_H__

#include "common.h"
using namespace std;

class StreamWriter
{
public:
	StreamWriter(char* buff, int size);
    StreamWriter(byte* buff, int size);
	~StreamWriter();

	void write(const char* buff, int32 size);
    void write(const byte* buff, int32 size);

	StreamWriter& operator <<(const int8 v);
	StreamWriter& operator <<(const vector<int8> v);
	StreamWriter& operator <<(const uint8 v);
	StreamWriter& operator <<(const vector<uint8> v);
	StreamWriter& operator <<(const int16 v);
	StreamWriter& operator <<(const vector<int16> v);
	StreamWriter& operator <<(const uint16 v);
	StreamWriter& operator <<(const vector<uint16> v);
	StreamWriter& operator <<(const int32 v);
	StreamWriter& operator <<(const vector<int32> v);
	StreamWriter& operator <<(const uint32 v);
	StreamWriter& operator <<(const vector<uint32> v);
	StreamWriter& operator <<(const int64 v);
	StreamWriter& operator <<(const vector<int64> v);
	StreamWriter& operator <<(const uint64 v);
	StreamWriter& operator <<(const vector<uint64> v);
	StreamWriter& operator <<(const float v);
	StreamWriter& operator <<(const vector<float> v);
	StreamWriter& operator <<(const double v);
	StreamWriter& operator <<(const vector<double> v);
	StreamWriter& operator <<(const string &v);
	StreamWriter& operator <<(const vector<string> &v);

public:
	char* _buff;
	int32 _size;
	int32 _offset;
};

#endif
