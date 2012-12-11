#include "stdafx.h"
#include "KComMD5.h"
#include "md5.h"


inline unsigned char HI4BIT( unsigned char by );
inline unsigned char LO4BIT( unsigned char by );
HRESULT Bin2Str( const void* pBuffer, int nBufSize, char* pOut, int nOutSize );



HRESULT KComMD5::GetMD5Code( 
	void*          pBuffer, 
	int            nBufSize, 
	unsigned char  MD5Code[16] 
    )
{
	if( pBuffer == NULL || nBufSize == 0 )
		return E_INVALIDARG;


	md5_state_t state = {0};
	::md5_init( &state );
	::md5_append( &state, (const md5_byte_t *)pBuffer, nBufSize );
	::md5_finish( &state, MD5Code );

	return S_OK;
}


HRESULT KComMD5::GetMD5String( 
	void*          pBuffer, 
	int            nBufSize, 
	char*          pszMD5String, 
	int            nSize 
	)
{
	if( nSize < 33 )          // 32 bytes md5 characters and 1 byte '\0'
		return E_INVALIDARG;

	if( pBuffer == NULL || pszMD5String == NULL )
		return E_INVALIDARG;


	const int nMD5Size = 16;
	unsigned char md5[nMD5Size] = {0};
	HRESULT hr = GetMD5Code( pBuffer, nBufSize, md5 );
	if( FAILED( hr ) )
		return hr;


	hr = Bin2Str( md5, nMD5Size, pszMD5String, nSize );

	return hr;
}


HRESULT KComMD5::GetFileMD5Code( 
	LPCTSTR        pszFilePath, 
	unsigned char  MD5Code[16] 
	)
{
	const size_t  MAX_HANDLED_SIZE = 128 * 1024 * 1024;

	HRESULT        hr            = S_OK;
	HANDLE         hFile         = INVALID_HANDLE_VALUE;
	HANDLE         hFileMapping  = NULL;
	void*          pBuffer       = NULL;
	ULARGE_INTEGER liFileSize;
	ULARGE_INTEGER liIter;
	ULONGLONG      ullDiff       = 0;
	DWORD          dwReadSize    = 0;


	try
	{
		if( pszFilePath == NULL || pszFilePath[0] == 0 )
			throw E_INVALIDARG;


		if( !::PathFileExists( pszFilePath ) )
			throw E_INVALIDARG;


		hFile = ::CreateFile(
			pszFilePath, 
			GENERIC_READ, 
			FILE_SHARE_READ, 
			NULL, 
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL, 
			NULL
			);
		if( hFile == INVALID_HANDLE_VALUE )
			throw E_FAIL;


		liFileSize.QuadPart = 0;
		liFileSize.LowPart = :: GetFileSize( hFile, &liFileSize.HighPart );
		if( liFileSize.LowPart == INVALID_FILE_SIZE  )
			throw E_FAIL;


		hFileMapping = ::CreateFileMapping( hFile, NULL, PAGE_READONLY, NULL, NULL, NULL );
		if( hFileMapping == NULL )
			throw E_FAIL;



		md5_state_t state = {0};
		::md5_init( &state );

		for( liIter.QuadPart = 0; 
			liIter.QuadPart < liFileSize.QuadPart; 
			liIter.QuadPart += MAX_HANDLED_SIZE 
			)
		{
			ullDiff = liFileSize.QuadPart - liIter.QuadPart;
			dwReadSize = (DWORD)min( (ULONGLONG)MAX_HANDLED_SIZE, ullDiff );

			pBuffer = ::MapViewOfFile( 
				hFileMapping, 
				FILE_MAP_READ, 
				liIter.HighPart, 
				liIter.LowPart, 
				dwReadSize 
				);

			if( pBuffer == NULL )
				throw E_FAIL;

			::md5_append( &state, (const md5_byte_t *)pBuffer, dwReadSize );


			UnmapViewOfFile( pBuffer );
			pBuffer = NULL;
		}

		::md5_finish( &state, MD5Code );

	}
	catch( HRESULT hResult )
	{
		hr = hResult;
	}


	if( pBuffer != NULL )
	{
		UnmapViewOfFile( pBuffer );
		pBuffer = NULL;
	}

	if( hFileMapping != NULL )
	{
		::CloseHandle( hFileMapping );
		hFileMapping = NULL;
	}

	if( hFile != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( hFile );
		hFile = NULL;
	}

	return hr;
}


HRESULT KComMD5::GetFileMD5String( 
	LPCTSTR        pszFilePath, 
	char*          pszMD5String, 
	int            nSize 
	)
{
	if( nSize < 33 )          // 32 bytes md5 characters and 1 byte '\0'
		return E_INVALIDARG;

	if( pszFilePath == NULL || pszFilePath[0] == 0 || pszMD5String == NULL )
		return E_INVALIDARG;


	const int nMD5Size = 16;
	unsigned char md5[nMD5Size] = {0};
	HRESULT hr = GetFileMD5Code( pszFilePath, md5 );
	if( FAILED( hr ) )
		return hr;


	hr = Bin2Str( md5, nMD5Size, pszMD5String, nSize );

	return hr;
}



//.----------------------------------------------------------------------------
// helper functions
//.----------------------------------------------------------------------------

inline unsigned char HI4BIT( unsigned char by )
{
	return by >> 4;
}

inline unsigned char LO4BIT( unsigned char by )
{
	return by & 0x0F;
}


HRESULT Bin2Str( const void* pBuffer, int nBufSize, char* pOut, int nOutSize )
{
	if( nOutSize < nBufSize * 2 + 1 )
		return E_INVALIDARG;

	if( pBuffer == NULL || pOut == NULL )
		return E_INVALIDARG;


	static const char code_table[] = "0123456789ABCDEF";
	const unsigned char* pInIter = (const unsigned char*)pBuffer;

	for( int i = 0; i < nBufSize; i++ )
	{
		*pOut++ = code_table[HI4BIT(*pInIter)];
		*pOut++ = code_table[LO4BIT(*pInIter)];

		pInIter++;
	}

	*pOut = '\0';
	return S_OK;
}