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
