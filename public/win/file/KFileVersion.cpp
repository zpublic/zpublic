#include "stdafx.h"
#include "KFileVersion.h"

#pragma comment( lib, "version.lib" )



KFileVersion::KFileVersion()
{
	m_lpBuffer    = NULL;
	m_lpData      = NULL;
	m_uDataSize   = 0;
	m_strLangInfo = TEXT( "" );
}


KFileVersion::~KFileVersion()
{
	free( m_lpBuffer );
}



BOOL KFileVersion::SetFile( LPCTSTR pszFileName )
{
	free( m_lpBuffer );
	m_lpBuffer    = NULL;
	m_uDataSize   = 0;
	m_strLangInfo = TEXT( "" );


	if( pszFileName == NULL )
	{
		return FALSE;
	}

	size_t nFileNameLength = _tcslen( pszFileName );
	if( nFileNameLength == 0 || nFileNameLength >= MAX_PATH )
	{
		return FALSE;
	}

	if( !::PathFileExists( pszFileName ) )
	{
		return FALSE;
	}

	//-------------------------------------------------
	// 获取版本信息数据
	TCHAR szFileName[MAX_PATH];
	BOOL  bRet;
	DWORD dwHandle = 0;

	_tcsncpy_s( szFileName, pszFileName, MAX_PATH - 1 );
	DWORD dwSize = ::GetFileVersionInfoSize( szFileName, &dwHandle );

	if( dwSize == 0 )
	{
		return FALSE;
	}

	m_lpBuffer = malloc( dwSize );
	bRet = ::GetFileVersionInfo( szFileName, dwHandle, dwSize, m_lpBuffer );
	if( !bRet )
	{
		return FALSE;
	}

	//-------------------------------------------------
	// 获取版语言ID和CodePage
	DWORD* pLangInfo = NULL;

	bRet = ::VerQueryValue(
		m_lpBuffer,
		TEXT( "\\VarFileInfo\\Translation" ),
		(void**)&pLangInfo,
		&m_uDataSize
		);

	if( !bRet || m_uDataSize < 4 )
	{
		return FALSE;
	}

	TRANSLATION trans = *(TRANSLATION*)pLangInfo;
	m_strLangInfo.Format( TEXT( "%04x%04x" ), trans.wLangID, trans.wCodePage );

	return TRUE;
}




BOOL KFileVersion::GetVersionInfo( LPCTSTR pszInfoType, CString& strInfo )
{
	if( m_strLangInfo.IsEmpty() )
	{
		return FALSE;
	}


	CString strStringTable  = TEXT( "StringFileInfo" );
	CString strSubBlock;
	strSubBlock.Format( TEXT( "\\%s\\%s\\%s" ), strStringTable,	m_strLangInfo, pszInfoType );
	

	BOOL bRet = ::VerQueryValue(
		m_lpBuffer,
		(LPTSTR)(LPCTSTR)strSubBlock,
		&m_lpData,
		&m_uDataSize
		);

	
	if( bRet )
	{
		strInfo.Format( TEXT( "%s" ), m_lpData );
	}

	return bRet;
}