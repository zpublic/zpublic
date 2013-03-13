#include "ipcjsonprocessor.h"
#include "JsonParser/ipcjsonrun.h"

namespace zl
{
namespace Ipc
{

std::string ipcJsonProcessor::Process(const char* szBuffer)
{
	Json::Value json_value;
	std::string json_operation;

	json_operation = ipcRemoteRunManager::Inst().GetOperation(szBuffer, json_value);
	if (json_operation == defJsonOperateAlloc)
	{
		int nID = 0;
		std::string json_ret;
		ipcReturnParse parse;
		
		json_ret = ipcRemoteRunManager::Inst().AllocObject(json_value);
		
		parse = json_ret;
		if (parse.Value(nID))
		{
			this->AddObject(nID);
		}

		return json_ret;
	}
	else if (json_operation == defJsonOperateDealloc)
	{
		int nID = 0;
		Json::Value json_id_value = json_value[defJsonInstID];
		if (json_id_value.isInt())
		{
			nID = json_id_value.asInt();
			this->RemoveObject(nID);
		}
		
		return ipcRemoteRunManager::Inst().DeallocObject(json_value);
	}
	else if (json_operation == defJsonOperateRun)
	{
		return ipcRemoteRunManager::Inst().RunFunc(json_value);
	}
	
	return ipcJsonReturn(enumRet_Error).ToString();
}

void ipcJsonProcessor::Disconnect(void)
{
	ClearAll();
}

void ipcJsonProcessor::AddObject(int nID)
{
	m_lstObjects.push_back(nID);
}

void ipcJsonProcessor::RemoveObject(int nID)
{
	m_lstObjects.remove(nID);
}

void ipcJsonProcessor::ClearAll(void)
{
	int nID;
	std::list<int>::iterator iter;
	
	for (iter = m_lstObjects.begin(); iter != m_lstObjects.end(); ++iter)
	{
		nID = *iter;
		ipcRemoteRunManager::Inst().DeallocObject(nID);		
	}
	
	m_lstObjects.clear();
}

IMsgProcessor* ipcJsonProcessor::Clone()
{
	return new ipcJsonProcessor;
}

void ipcJsonProcessor::Delete(IMsgProcessor* pProcessor)
{
	if (pProcessor)
	{
		delete pProcessor;
	}
}

}
}