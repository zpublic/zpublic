#ifndef __OPCODE_REGISTRY_H__
#define __OPCODE_REGISTRY_H__

#include "common.h"
#include "singleton.h"

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
#endif