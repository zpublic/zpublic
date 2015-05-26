
#include "module_sample.h"



// 静态绑定消息，在模块初始化时加载绑定
BEGIN_MESSAGE_MAP(ModuleSample)
    DEFINE_MESSAGE_BINDING(Msg_Module_Initialzed, &OnModuleInit)
END_MESSAGE_MAP

int ModuleSample::GetID()
{
    return ModuleDefine::Module_ModuleSample;
}

std::wstring ModuleSample::GetName()
{
    return L"ModuleSample";
}

int ModuleSample::Initialize()
{
    return 0;
}

void ModuleSample::Uninitialize()
{

}

int ModuleSample::GetVersion()
{
    return 1;
}

IPlugin* ModuleSample::GetModulePtr()
{
    return _instance;
}

void ModuleSample::OnModuleInit(int param1, int param2, int senderID)
{
    UNREFERENCED_PARAMETER(param2);
    UNREFERENCED_PARAMETER(senderID);

    // 动态绑定消息，在模块运行过程中加载绑定
    BindMessage(Msg_Test_1, &ModuleSample::OnMessageTest1);

    // 发送消息
    ProduceMessage(this->GetID(), Msg_Test_1, param1, reinterpret_cast<int>(L"HI"));
}

void ModuleSample::OnMessageTest1(int param1, int param2, int senderID)
{
    UNREFERENCED_PARAMETER(param2);
    UNREFERENCED_PARAMETER(senderID);

    ProduceMessage(this->GetID(), Msg_Test_2, param1, reinterpret_cast<int>(L"HI"));
}

ModuleSample* ModuleSample::Instance()
{
    if (_instance == nullptr)
    {
        _instance = new ModuleSample();
    }

    return _instance;
}

ModuleSample::~ModuleSample()
{
    delete _instance;
}
