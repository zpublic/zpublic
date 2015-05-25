// ModuleSample.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ModuleABC.h"



// 静态绑定消息，在模块初始化时加载绑定
BEGIN_MESSAGE_MAP(ModuleABC)
	DEFINE_MESSAGE_BINDING(Msg_Module_Initialzed, &OnModuleInit)
END_MESSAGE_MAP

int ModuleABC::GetID()
{
	return ModuleDefine::Module_ModuleABC;
}

std::wstring ModuleABC::GetName()
{
	return L"ModuleABC";
}

int ModuleABC::Initialize()
{
	return 0;
}

void ModuleABC::Uninitialize()
{

}

int ModuleABC::GetVersion()
{
	return 1;
}

IPlugin* ModuleABC::GetModulePtr()
{
	return _instance;
}

void ModuleABC::OnModuleInit(int param1, int param2, int senderID)
{
	UNREFERENCED_PARAMETER(param2);
	UNREFERENCED_PARAMETER(senderID);

	// 动态绑定消息，在模块运行过程中加载绑定
	BindMessage(Msg_Test_1, &ModuleABC::OnMessageTest1);

	// 发送消息
	ProduceMessage(this->GetID(), Msg_Test_1, param1, reinterpret_cast<int>(L"HI"));
}

void ModuleABC::OnMessageTest1(int param1, int param2, int senderID)
{
	UNREFERENCED_PARAMETER(param2);
	UNREFERENCED_PARAMETER(senderID);

	ProduceMessage(this->GetID(), Msg_Test_2, param1, reinterpret_cast<int>(L"HI"));
}

ModuleABC* ModuleABC::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new ModuleABC();
	}

	return _instance;
}

ModuleABC::~ModuleABC()
{
	delete _instance;
}
