#include "windows.h"
#include <string>


int main()
{
	HMODULE tmp = LoadLibrary(L"ModuleManager.dll");

	bool(*fnLoadPlugin)(std::wstring path) = (bool(*)(std::wstring path)) GetProcAddress(tmp, "LoadPlugin");
	// 模块名称，暂时在这里加
	fnLoadPlugin(L"ModuleSample.dll");

	void(*fnBeginLoop)() = (void(*)())GetProcAddress(tmp, "BeginMessageLoop");

	fnBeginLoop();

	return 0;
}
