#ifndef __OPCODE_REGISTRY_H__
#define __OPCODE_REGISTRY_H__

#include "common.h"
#include "singleton.h"
#include "network_common.h"

namespace Venus
{

template <typename T>
struct OpcodeHandler
{
    //typedef void (T::*MessageHandler)(NetworkMessage&);
    typedef std::function<void (T*, const NetworkPacket::Ptr&)> MessageHandler;

    OpcodeHandler(const std::string& message_name, const MessageHandler& message_handler)
        : name(std::move(message_name)), handler(std::move(message_handler))
    {
    }

	std::string name;
    MessageHandler handler;
};

// 用于管理消息处理器注册的类
//    模板说明：
//    T - 消息处理函数类类型

template <typename T>
class OpcodeRegistry
{
public:
    OpcodeRegistry(){}
    virtual ~OpcodeRegistry(){}
	OpcodeHandler<T>& operator[](uint32 opcode)
	{
		return _opcodeTable[opcode];
	}

    const std::string& messageName(uint32 opcode)
    {
        return OpcodeHandler<T>[opcode].name;
    }

    void registerMessage(uint32 opcode, const OpcodeHandler<T>& handler)
    {
        _opcodeTable.insert(std::make_pair(opcode, handler));
    }

private:
     adap_map<uint32, OpcodeHandler<T>> _opcodeTable;
};

#define OPCODE_REGISTER_BEGIN(REGISTRY_NAME, CB_TARGET) \
    class REGISTRY_NAME : public Venus::OpcodeRegistry<CB_TARGET>, public Venus::Singleton<REGISTRY_NAME> \
    {                                \
        public: REGISTRY_NAME(){}    \
        public: template <typename T> void initialize() { 

#define OPCODE_REGISTER_END() }};

#define MAKE_HANDLER(m, f) \
    Venus::OpcodeHandler<T>(m, std::bind(&T::f, std::placeholders::_1, std::placeholders::_2))

#define REGISTER_HANDLER(m, f) \
    registerMessage(m, MAKE_HANDLER(#m, f))
}

#endif