///////////////////////////////////////////////////////////////
//
//	Filename: 	KFileVersion.h
//	Creator:	lichenglin  <lichenglin@kingsoft.net>
//  Date:       2007-1-17  19:54
//	Comment:	获取exe、dll文件版本信息
//
///////////////////////////////////////////////////////////////


#ifndef _KFILEVERSION_H_
#define _KFILEVERSION_H_



class KFileVersion  
{
public:
	KFileVersion();
	~KFileVersion();

	/**
	* @brief   指定要获取哪个文件的版本信息
	* @param   [in]  pszFileName,    文件名, 必须是带资源信息的文件, 如exe、dll
	* @return  如果成功返回TRUE, 否则返回FALSE
	*/
	BOOL SetFile( LPCTSTR pszFileName );


	/**
	* @brief   获取具体的版本信息
	* @param   [in]   pszInfoType,   版本信息类型, 见上述取值
	* @param   [out]  strInfo,       版本信息
	* @return  如果成功返回TRUE, 失败返回FALSE
	*/
	BOOL  GetVersionInfo( LPCTSTR pszInfoType, CString& strInfo );


private:
	LPVOID	m_lpBuffer;
	LPVOID	m_lpData;
	UINT	m_uDataSize;

	CString m_strLangInfo;

private:
	struct TRANSLATION 
	{ 
		WORD wLangID;    // language ID 
		WORD wCodePage;  // code page 
	};

};


#endif // _KFILEVERSION_H_
