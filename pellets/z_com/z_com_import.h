/***********************
*!!!!!!!!!!!!!!!!!!!!!!*
*!!!!Do not Modify!!!!!*
*!!!!!!!!!!!!!!!!!!!!!!*
************************
*如果你要修改任何相关代码，或者有什么建议，请提交到
*https://github.com/zpublic/zpublic
********************************************************/
#pragma once
#include "import/z_com_loader.hpp"

/*

使用方法：

ZLComLoader<ICompute> loader;
if (SUCCEEDED(loader.Initialize("c:\\1.dll"))
{
    ICompute* i = loader.GetInterface();
}

 */
