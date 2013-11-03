#ifndef OPCODES_HANDLER
#define OPCODES_HANDLER

#include <common.h>
#include <network_common.h>

class GameSession;
struct OpcodeHandler
{
    typedef std::function<void (GameSession*, const NetworkMessage&)> MessageHandler;

    OpcodeHandler(){}
    OpcodeHandler(const std::string& name, const MessageHandler& handler) :
        message_name(name), message_handler(handler)
    {
    }

	std::string message_name;
    MessageHandler message_handler;
};

class OpcodeTable 
    : public boost::noncopyable
{
	typedef std::hash_map<uint32, OpcodeHandler> OpcodeHandlerMap;

private:
    OpcodeTable();

public:
	~OpcodeTable(){}

	static OpcodeTable& instance()
	{
		static OpcodeTable _instance;
		return _instance;
	}

	OpcodeHandler* operator[](uint32 opcode)
	{
		OpcodeHandlerMap::iterator iter = _opcodeTable.find(opcode);
		if (iter != _opcodeTable.end())
		{
			return &iter->second;
		}
		else
		{
			return nullptr;
		}
	}

private:
	OpcodeHandlerMap _opcodeTable;
};
#endif