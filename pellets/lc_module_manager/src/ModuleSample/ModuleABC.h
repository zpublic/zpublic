
#include "stdafx.h"

#include "../Module/Implement/ModuleBase/ModuleBase.hpp"


/**
* @brief 模块实现示例
*/

BEGIN_DECLARE_MODULE(ModuleABC)
/**
* @brief 模块示例类。\n
*		 模块定义必须包含在BEGIN_DECLARE_MODULE与END_DECLARE_MODULE两个宏之间；	编写定义前必须使用宏IMPLEMENT_MODULE_BASE对该模块类进行定义；\n
*		 模块必须按单例模式实现，实现格式必须与此类一致。
*/

class ModuleABC :public ModuleBase
{
	IMPLEMENT_MODULE_BASE(ModuleABC)

public:
	/**
	* @brief 初始化；模块管理器将会在加载模块对象之前调用此函数。
	*		 模块类必须实现此函数。
	* @return 0为初始化成功，非0为失败。
	*/
	int Initialize() override;

	/**
	* @brief 反初始化，模块管理器将会在卸载模块对象之前调用此函数。
	*		 模块类必须实现此函数。
	*/
	void Uninitialize() override;

	/**
	* @brief 获取模块的ID；实现模块类时，必须先到/src/Module/Implement/PubDef.h文件中定义该模块类的ID，以便模块管理器进行识别；
	*		 模块类必须实现此函数。
	* @return 模块ID
	*/
	int GetID() override;

	/**
	* @brief 获取模块对象的指针，以便模块管理器进行加载；
	*		 模块类必须实现此函数。
	* @return 模块对象指针
	*/
	IPlugin* GetModulePtr() override;

	/**
	* @brief 获取模块的名称，以便模块管理器进行识别；此名称必须为DLL的名字；
	*		 模块类必须实现此函数。
	* @return 模块名称
	*/
	std::wstring GetName() override;

	/**
	* @brief 获取模块的版本，以便模块管理器进行升级；
	*		 模块类必须实现此函数。
	* @return 模块名称
	*/
	int GetVersion() override;

	/**
	* @brief 消息响应函数示例，示范了动态绑定消息以及发送消息的方法。
	* @param[in] param1 消息参数1
	* @param[in] param2 消息参数2
	* @param[in] senderID 发送者ID
	*/
	void OnModuleInit(int param1, int param2, int senderID);
	void OnMessageTest1(int param1, int param2, int senderID);

	static ModuleABC* Instance();



private:
	ModuleABC(){};
	~ModuleABC();

	static ModuleABC* _instance;
};

ModuleABC* ModuleABC::_instance = nullptr;

END_DECLARE_MODULE(ModuleABC)
