///////////////////////////////////////////////////////////////
//
//	Filename: 	KComMD5.h
//	Creator:	lichenglin  <lichenglin@kingsoft.net>
//	Date:		2007-8-29  17:36
//	Comment:	
//
///////////////////////////////////////////////////////////////


#ifndef _KComMD5_h_
#define _KComMD5_h_


class KComMD5 : IMD5
{
public:
	virtual HRESULT GetMD5Code( 
		void*          pBuffer, 
		int            nBufSize, 
		unsigned char  MD5Code[16] 
	    );

	virtual HRESULT GetMD5String( 
		void*          pBuffer, 
		int            nBufSize, 
		char*          pszMD5String, 
		int            nSize 
		);

	virtual HRESULT GetFileMD5Code( 
		LPCTSTR        pszFilePath, 
		unsigned char  MD5Code[16] 
	    );

	virtual HRESULT GetFileMD5String( 
		LPCTSTR        pszFilePath, 
		char*          pszMD5String, 
		int            nSize 
		);


	KAS_BEGIN_COM_MAP( KComMD5 )
		KAS_COM_INTERFACE_ENTRY( IMD5 )
	KAS_END_COM_MAP()
};



KAS_OBJECT_ENTRY_AUTO( CLSID_KMD5, KComMD5 )


#endif // _KComMD5_h_