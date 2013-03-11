#ifndef ZPUBLIC_JSON_PARSER_IPC_LOCATION
#define ZPUBLIC_JSON_PARSER_IPC_LOCATION

#include <string>

#pragma warning(push)
#pragma warning( disable : 4996 )

namespace zl
{
namespace Ipc
{

#define UNKNOWN_LOCATION	"unknown_location"
#define HERE_LOCATION		__FILE__, __FUNCTION__, __LINE__

/*
@ Creator: zzd
@ Brief	: 记录代码位置，调试时使用
@ Example:
	class A : public ipcLocation
	{...}

	A a;
	a.Set(HERE_LOCATION);//a位记录下此代码的位置
*/

class ipcLocation
{
public:
	ipcLocation(const char* file = UNKNOWN_LOCATION, const char* func = UNKNOWN_LOCATION, int line = -1)
		: file_(file), func_(func){line_ = IntToString(line);}
	ipcLocation(ipcLocation& l){
		file_ = l.file_; func_ = l.func_; line_ = l.line_;
	}
	virtual ~ipcLocation(){}

	void Set(const char* file, const char* func, int line)
	{
		file_ = file;
		func_ = func;
		line_ = IntToString(line);
	} 
	
	const char* File(void){return file_.c_str();}
	const char* Func(void){return func_.c_str();}
	const char* Line(void){return line_.c_str();}
	
protected:
	std::string IntToString(int n)
	{
		char buffer[30];
		sprintf(buffer, "%d", n);
		return buffer;
	}

private:
	std::string file_;
	std::string func_;
	std::string line_;
};

}
}

#pragma warning(pop)
#endif