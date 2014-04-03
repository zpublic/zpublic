#ifndef _MESSAGE_STREAM_H_
#define _MESSAGE_STREAM_H_

#include "common.h"
#include "buffer.h"

class MessageStream : public Buffer
{
public:
	MessageStream():Buffer(4096){}
	virtual ~MessageStream(){ i = 0; b.clear(); }

	MessageStream& operator <<(const int8 v);
	MessageStream& operator <<(const uint8& v);
	MessageStream& operator <<(const int16& v);
	MessageStream& operator <<(const uint16& v);
	MessageStream& operator <<(const int32& v);
	MessageStream& operator <<(const uint32& v);
	MessageStream& operator <<(const int64& v);
	MessageStream& operator <<(const uint64& v);
	MessageStream& operator <<(const float& v);
	MessageStream& operator <<(const double& v);
	MessageStream& operator <<(const std::string& v);
    MessageStream& operator >>(int8& v);
	MessageStream& operator >>(uint8& v);
	MessageStream& operator >>(int16& v);
	MessageStream& operator >>(uint16& v);
	MessageStream& operator >>(int32& v);
	MessageStream& operator >>(uint32& v);
	MessageStream& operator >>(const int64& v);
	MessageStream& operator >>(const uint64& v);
	MessageStream& operator >>(float& v);
	MessageStream& operator >>(double& v);
	MessageStream& operator >>(std::string& v);

private:
	void resize(uint32 sz);
};

#endif
