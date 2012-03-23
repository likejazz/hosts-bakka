// IniFile.cpp : implementation file
//

#include "stdafx.h"
#include "IniFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CIniFile

CIniFile::CIniFile(LPCTSTR	strIniFileName,		// [in] new file name
				   INT		iMaxStringLength)	// [in] max length of a string that can be wtritten/read to/from the INI file by this instance
	: m_MAXSTRLEN(iMaxStringLength)
{
	SetIniFileName(strIniFileName);
}


CIniFile::~CIniFile()
{	
}


/////////////////////////////////////////////////////////////////////////////
// CIniFile operations

// PURPOSE: Get the name of the INI file.
CString	CIniFile::GetIniFileName()
{
	return m_strFileName ;
}

// PURPOSE: Set the name of the INI file.
void 
CIniFile::SetIniFileName(LPCTSTR lpIniFileName)	// [in] new file name
{
	m_strFileName = lpIniFileName;
}

// PURPOSE: Get the status of the ini file.  
//			This can be used, for example, to check the existence of the file.
BOOL 
CIniFile::GetStatus(CFileStatus& rStatus)	// [out] A reference to a user-supplied CFileStatus structure that will receive the status information.
{
	BOOL bRetVal = FALSE;
	bRetVal = CFile::GetStatus(m_strFileName, rStatus);
	return bRetVal;
}

// PURPOSE:	Read an integer to the INI file.
// NOTE:	The MSDN article on ::GetPrivateProfileInt(...) says: "If the value of the key is less than zero, the return value is zero."
//			However, ::GetPrivateProfileInt returns negative integers just fine.
UINT
CIniFile::GetInt(LPCTSTR lpSection,			// [in] section name
				 LPCTSTR lpKey,				// [in] key name
				 const INT iDefaultValue)	// [in] default value, which is returned, if the key is not found
{
	UINT iRet = ::GetPrivateProfileInt(lpSection, lpKey, iDefaultValue, m_strFileName);
	// ::GetLastError() is of no use here, because ::GetPrivateProfileInt() doesn't set an error-code.
	return iRet;
}

// PURPOSE:	Read a string from the INI file.
BOOL		// TRUE, if successful.  FALSE otherwise.
CIniFile::GetString(LPCTSTR lpSection,		// [in] section name
					LPCTSTR lpKey,			// [in] key name
					CString& strRet,		// [out] CString that receives the tring from the INI file
					LPCTSTR lpDefault)		// [in] default string, which is copied into strRet, if the key is not found.  NOTE: this pointer can't be NULL.
{
	ASSERT(lpDefault != NULL);
	ASSERT(lstrlen(lpDefault) < m_MAXSTRLEN);
	return CIniFile::GetString(lpSection, lpKey, strRet, lpDefault, m_MAXSTRLEN);
}

// PURPOSE:	Read a string from the INI file. (Helper function.)
// NOTE:	The calling code should check if strRet == lpDefault.
BOOL		// TRUE, if API call successful (incl. the case, when strRet == lpDefault).  
			// FALSE otherwise (incl. memory exception
CIniFile::GetString(LPCTSTR lpSection,		// [in] section name
					LPCTSTR lpKey,			// [in] key name
					CString& strRet,		// [out] CString that receives the tring from the INI file
					LPCTSTR lpDefault,		// [in] default string, which is copied into strRet, if the key is not found.  NOTE: this pointer can't be NULL. 
					const DWORD iSize)		// [in] buffer size
{	
	LPTSTR pBuffer;
	TRY
	{
		pBuffer = strRet.GetBuffer(iSize);		
	}
	CATCH(CMemoryException, pExc)	// GetBuffer() can throw a memory exceprion
	{
		THROW(pExc);				// rethrow
		return FALSE;
	}
	END_CATCH

	memcpy(pBuffer + iSize - 2, "xx",2);	// initialize the last 2 characters on the buffer with something other than \0
	DWORD iRet = ::GetPrivateProfileString(lpSection, lpKey, lpDefault, pBuffer, iSize, m_strFileName);
	ASSERT(iRet < iSize);
	BOOL bRet = (memcmp(pBuffer + iSize - 2, "\0\0", 2) != 0);	// check the last 2 characters of the buffer.
	strRet.ReleaseBuffer();	
	return bRet;
}

// PURPOSE:	Read a struct from the INI file
BOOL
CIniFile::GetStruct(LPCTSTR lpSection,		// [in] section name 		  
					LPCTSTR lpKey, 			// [in]	key name	  
					LPVOID	lpRetStruct,	// [out] pointer to thye return buffer
					const UINT iSizeStruct) // [in] size of the return buffer
{
	ASSERT(lpRetStruct != NULL);
	ASSERT(iSizeStruct > 0);
	return ::GetPrivateProfileStruct(lpSection, lpKey, lpRetStruct, iSizeStruct, m_strFileName);
}