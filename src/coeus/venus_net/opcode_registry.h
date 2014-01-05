#ifndef __OPCODE_REGISTRY_H__
#define __OPCODE_REGISTRY_H__

#include "common.h"
#include "singleton.h"

namespace Venus
{

class NetworkMessage;
template <typename T>
struct OpcodeHandler
{
    typedef void (T::*MessageHandler)(NetworkMessage&);
	std::string name;
    MessageHandler handler;
};

template <typename T>
class OpcodeRegistry : Venus::Singleton<OpcodeRegistry<T>>
{
    static const int MAX_OPCODE_TABLE_SIZE = 0xffff;

#define MAKE_HANDLER(m, f) \
    OpcodeHandler(m, std::bind(&T::f, std::placeholders::_1, std::placeholders::_2));

#define DEFINE_HANDLER(m, f) \
    _opcodeTable[m] = MAKE_HANDLER(#m, f);

public:
	OpcodeHandler<T>* operator[](uint32 opcode)
	{
		return _opcodeTable[opcode];
	}

    const std::string& messageName(uint32 opcode)
    {
        return OpcodeHandler<T>[opcode].name;
    }

private:
    OpcodeHandler<T> _opcodeTable[MAX_OPCODE_TABLE_SIZE];
};

}

#endif