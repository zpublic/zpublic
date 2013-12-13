#pragma once

interface IBkExtraResource
{
    virtual BOOL LoadModuleResource(LPCWSTR lpszResType, UINT uResID, IStream** ppiStmResource) = NULL;
    virtual BOOL LoadGlobalResource(LPCWSTR lpszResType, UINT uResID, IStream** ppiStmResource) = NULL;
	virtual BOOL LoadThemeResource(LPCWSTR lpszResType, UINT uResID, IStream** ppiStmResource) = NULL;
};