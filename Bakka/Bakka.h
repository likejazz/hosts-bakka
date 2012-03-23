// Bakka.h : main header file for the PROJECT_NAME application
//

#pragma once

#define APP_NAME			L"Bakka"
#define APP_AGENT			L"User-Agent: Mozilla/4.0 (compatible; Bakka; Windows NT 6.0)\r\n"

#define HOST_FILE_RELPATH	"\\drivers\\etc\\hosts" //'hosts' file's relative path from SYSTEM DIR
#define HOST_BFILE_POSTFIX	"_backup_bakka"

#define BAKKA_START			L"BAKKA_START"
#define BAKKA_END			L"BAKKA_END"

#define BAKKA_START_TAG		L"## BAKKA_START: DO NOT MODIFY\n"
#define BAKKA_END_TAG		L"\n## BAKKA_END: DO NOT MODIFY"

#define BTNTEXT_RESTORE		_T("원래대로")
#define BTNTEXT_NOTUSED		_T("사용 안함")

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CBakkaApp:
// See Bakka.cpp for the implementation of this class
//

class CBakkaApp : public CWinApp
{
public:
	CBakkaApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CBakkaApp theApp;