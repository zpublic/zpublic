#pragma once

// 定义消息ID
enum MsgDefine
{
	Msg_Reserve_Begin,
	Msg_Module_Initialzed,
	Msg_Reserve_End = 50,					// 保留消息ID的分界线，小于此分界线的消息为特殊消息(通用消息，系统消息等等)
									
	Msg_Test_Begin = 100,					// 模块定义消息ID时，须像这样列出分界线
	Msg_Test_1 = 123,
	Msg_Test_2,
	Msg_Test_End = 150,
};

// 定义模块ID
enum ModuleDefine							//每当加入一个模块时，要在此定义模块的ID
{
	Module_ALL,
	Module_ModuleCenter,

	Module_Define_begin = 50,					// 之前的值是保留值，模块定义从此开始
	Module_ModuleABC,

};