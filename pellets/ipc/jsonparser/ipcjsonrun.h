#ifndef ZPUBLIC_JSON_PARSER_IPC_RUN
#define ZPUBLIC_JSON_PARSER_IPC_RUN

/*
接口参数支持如下类型
	1.基本类型：char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long
				float, double
	2.标准库：string&, wstring&, list&, vector&(其中list, vector 等的模板参数必须是支持的类型)
	3.字符串：const char*, const wchar_t*

结构体成员支持如下类型：
	1.基本类型：char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long
				float, double
	2.标准库：string, wstring, list, vector(其中list, vector 等的模板参数必须是支持的类型)
	3.字符串：char[], wchar_t[]


注意项：
	1.创建一个导出的全局函数方法如下：
	int PrintValue(int v);
	ADD_GLOBAL_FUNC_1(PrintValue, int);	//将PrintValue加入函数库

	2.创建一个导出接口的类函数方法如下：
	class IExport
	{
		DECLARE_CLASS_CREATE_FUNC(IExport);　//声明该类的静太创建函数
	public:
		int PrintValue(int v);
	}
	DECLARE_EXPORT_INTERFACE(IExport); //将IExport加入创建工厂，可能通过对象名来创建对象
	ADD_CLASS_FUNC_1(IExport, PrintValue, int);

	4.接口参数的结构体必须有附加声明，如：
	struct Param
	{
		int n;
		std::string str;
		std::list<int> lst;
	}
	DECLARE_JSON_CONVERSION_3(Param, n, str, lst);//声明Param的Json转换函数

	5.调用全局函数时直接创建Json字符串，如：
	std::string json_str = CreateGlobalCall("PrintValue", 3888);
	zl::Ipc::ipcReturnParse parse = ipcRemoteRunManager::Inst().Run(json_str.c_str());
	int n_ret_value = -1;
	if (parse.Value(n_ret_value)) {...};use n_ret_value
	else {...}error

	6.调用接口的类函数时，要先创建对象，再通过对象ID调用，如：
	std::string json_str = CreateObjectCreateCall("IExport");
	zl::Ipc::ipcReturnParse parse = ipcRemoteRunManager::Inst().Run(json_str.c_str());
	parse.Value(n_id);

	json_str = CreateClass(n_id, "PrintValue", 3888);
	int n_ret = ipcRemoteRunManager::Inst().Run(json_str.c_str());

	7.C++调用时，用KJsonParam来创建调用函数。Html就得自己组建Json字符串

	8.各种Json字符串说明
	  调用全局函数时的Json字符串
		{"func_name":"PrintValue","operation":"run","param_array":[3888]}

	  调用类函数时的Json字符串
		{"func_name":"PrintValue","id":1,"operation":"run","param_array":[100]}

	  调用分配类实例时的Json字符串
		{"class_name":"IExport","operation":"alloc"}

	  调用释放类实例时的Json字符串

		{"id":1,"operation":"dealloc"}

	  返回的Json字符串
		{"code":0,"message":"succeed","value":1}


*/

/*
未完成：
	1.增加std::map支持
	2.增加注释

	3.让Call的调用简单一点，如下：
	KGlobalRun_0<class R>
	{
		KGlobalRun_0(const char* class_name);
		void Run();
		int RetCode();
		std::string& RetMsg();
		R& RetValue();
	}
	KGlobalRun_1<class R, class P1>;


*/
#pragma warning(push)

#pragma warning( disable : 4244 ) //关闭类型转换损失警告，如double转成int
#pragma warning( disable : 4018 ) //关闭比较操作左右类型不一至警告，如int > double
#pragma warning( disable : 4996 ) //关闭C库函数不安全的警告，如strcpy, wcscpy等

#include "ipcjsonconvert.h"
#include "ipcjsonparam.h"
#include "ipcfuncmgr.h"

#pragma warning(pop)

#endif