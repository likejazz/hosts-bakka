// HostsUpdater.cpp: implementation of the CHostsUpdater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Bakka.h"
#include "BakkaDlg.h"
#include "HostsUpdater.h"
#include <afx.h>
#include <afxwin.h>
#include <afxinet.h>
#include <iostream>
#include <stdlib.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CString g_strHostFile;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHostsUpdater::CHostsUpdater(LPCTSTR pstrAgent = NULL)
	: CInternetSession(APP_NAME, 1, PRE_CONFIG_INTERNET_ACCESS, 0, 0, 0)
{
}
void CHostsUpdater::OnStatusCallback(DWORD  dwContext , DWORD dwInternetStatus,
	LPVOID  lpvStatusInformation , DWORD  dwStatusInformationLength )
{
    // Status callbacks need thread-state protection. 
    AFX_MANAGE_STATE(AfxGetAppModuleState( ));

    CString strStatus;

	switch (dwInternetStatus)
	{
	case INTERNET_STATUS_RESOLVING_NAME:
		strStatus.Format(L"Resolving name %s", lpvStatusInformation);
		break;

	case INTERNET_STATUS_NAME_RESOLVED:
		strStatus.Format(L"Name resolved %s", lpvStatusInformation);
		break;

	case INTERNET_STATUS_HANDLE_CREATED:
		strStatus.Format(L"Handle created");
		break;

	case INTERNET_STATUS_CONNECTING_TO_SERVER:
		strStatus.Format(L"Connecting to socket address");
		break;

	case INTERNET_STATUS_CONNECTED_TO_SERVER:
		strStatus.Format(L"Connected to socket address");
		break;

	case INTERNET_STATUS_SENDING_REQUEST:
		strStatus.Format(L"Sending request");
		break;

	case INTERNET_STATUS_REQUEST_SENT:
		strStatus.Format(L"Request sent");
		break;

	case INTERNET_STATUS_RECEIVING_RESPONSE:
        return;
		strStatus.Format(L"Receiving response");
		break;

	case INTERNET_STATUS_RESPONSE_RECEIVED:
		strStatus.Format(L"Response received");
		break;

	case INTERNET_STATUS_CLOSING_CONNECTION:
		return;
		strStatus.Format(L"Closing the connection to the server");
		break;

	case INTERNET_STATUS_CONNECTION_CLOSED:
		return;
		strStatus.Format(L"Connection to the server closed");
		break;

	case INTERNET_STATUS_HANDLE_CLOSING:
		return;
		strStatus.Format(L"Handle closed");
		break;
	
	//Check MSDN information about CInternetSession dwFlag INTERNET_FLAG_ASYNC
	//I have never been able to get this to work to my liking
	case INTERNET_STATUS_REQUEST_COMPLETE:
  		strStatus.Format(L"Request complete");
		break;

	case INTERNET_STATUS_REDIRECT:
		strStatus.Format(L"Being redirected");
		break;

	default:
		strStatus.Format(L"Unknown status: %d", dwInternetStatus);
		break;
	}

	StepIt();
	ShowStatus(strStatus);
}

//use this function to update any status, edit or what ever control
//that needs updating
void CHostsUpdater::ShowStatus(LPCTSTR str)
{
	CBakkaDlg * pBakkaDlg = (CBakkaDlg*) AfxGetMainWnd();
	pBakkaDlg->ShowStatus(str);
}

void CHostsUpdater::ShowEditBox(LPCTSTR str)
{
	CBakkaDlg * pBakkaDlg = (CBakkaDlg*) AfxGetMainWnd();
	pBakkaDlg->ShowEditBox(str);
}

void CHostsUpdater::StepIt()
{
	CBakkaDlg * pBakkaDlg = (CBakkaDlg*) AfxGetMainWnd();
	pBakkaDlg->StepIt();
}

void CHostsUpdater::StepIt(DWORD pos)
{
	CBakkaDlg * pBakkaDlg = (CBakkaDlg*) AfxGetMainWnd();
	pBakkaDlg->StepIt(pos);
}

//Lets get the file via http
DWORD CHostsUpdater::GetAndSetHosts(LPCTSTR lpstrServer, int nPort, CString strFile, CStringArray* hosts)
{
	//Check what file types we will allow to be requested
	CString extension = strFile.Right(3);

	if(extension == "exe")
	{
		return 0;
	}
	if(extension == "com")
	{
		return 0;
	}
	if (extension == "dll")
	{
		return 0;
	}
	if (extension == "bat")
	{
		return 0;
	}
	if (extension == "sys")
	{
		return 0;
	}
	if (extension == "inf")
	{
		return 0;
	}

	DWORD dwAccessType = PRE_CONFIG_INTERNET_ACCESS;
	DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_DONT_CACHE;

	//the verb we will be using for this connection
	//if NULL then GET is assumed
	LPCTSTR pstrVerb = L"GET";
	
	//the address of the url in the request was obtained from
	LPCTSTR pstrReferer = NULL;

	//Http version we are using; NULL = HTTP/1.0
	LPCTSTR pstrVersion = NULL;

	//For the Accept request headers if we need them later on
	//LPCTSTR pstrAcceptTypes = "Accept: audio/x-aiff, audio/basic, audio/midi, audio/mpeg, audio/wav, image/jpeg, image/gif, image/jpg, image/png, image/mng, image/bmp, text/plain, text/html, text/htm\r\n";
	LPCTSTR pstrAcceptTypes = NULL;
	CString szHeaders = L"Accept: audio/x-aiff, audio/basic, audio/midi, audio/mpeg, audio/wav, image/jpeg, image/gif, image/jpg, image/png, image/mng, image/bmp, text/plain, text/html, text/htm\r\n";

	//the server port we need changed
	//nPort = INTERNET_INVALID_PORT_NUMBER
	unsigned short usPort = nPort;
	
	//Username we will use if a secure site comes into play
	LPCTSTR pstrUserName = NULL; 
	//The password we will use
	LPCTSTR pstrPassword = NULL;

	CHostsUpdater Session(NULL);

	//Set any CInternetSession options we  may need
	int ntimeOut = 3;
	Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,1000* ntimeOut);
	Session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,1000);
	Session.SetOption(INTERNET_OPTION_CONNECT_RETRIES,1);

	//Enable or disable status callbacks
	Session.EnableStatusCallback(TRUE);

	CHttpConnection* pServer = NULL;   
	CHttpFile* pFile = NULL;
	DWORD dwRet;
	try {		

		StepIt();
		pServer = Session.GetHttpConnection(lpstrServer, INTERNET_FLAG_DONT_CACHE, usPort, pstrUserName, pstrPassword);
		pFile = pServer->OpenRequest(pstrVerb, strFile, pstrReferer, 1, 
				&pstrAcceptTypes, pstrVersion, dwHttpRequestFlags);

		pFile->AddRequestHeaders(szHeaders);
		pFile->AddRequestHeaders(APP_AGENT, HTTP_ADDREQ_FLAG_ADD_IF_NEW);
		pFile->SendRequest();

		pFile->QueryInfoStatusCode(dwRet);//Check wininet.h for info
										  //about the status codes

		if (dwRet == HTTP_STATUS_DENIED)
		{
			return dwRet;
		}

		if (dwRet == HTTP_STATUS_MOVED ||
			dwRet == HTTP_STATUS_REDIRECT ||
			dwRet == HTTP_STATUS_REDIRECT_METHOD)
		{
			CString strNewAddress;
			//again check wininet.h for info on the query info codes
			//there is alot one can do and re-act to based on these codes
			pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewAddress);
			
			int nPos = strNewAddress.Find(_T("Location: "));
			if (nPos == -1)
				{
					return 0;
				}
			strNewAddress = strNewAddress.Mid(nPos + 10);
			nPos = strNewAddress.Find('\n');
			if (nPos > 0)
				strNewAddress = strNewAddress.Left(nPos);

			pFile->Close();      
			delete pFile;
			pServer->Close();  
			delete pServer;

			CString strServerName;
			CString strObject;
			INTERNET_PORT nNewPort;
			DWORD dwServiceType;

			if (!AfxParseURL(strNewAddress, dwServiceType, strServerName, strObject, nNewPort))
				{
					return 0;
				}

			StepIt();
			pServer = Session.GetHttpConnection(strServerName, nNewPort, 
				pstrUserName, pstrPassword);
			pFile = pServer->OpenRequest(pstrVerb, strObject, 
				pstrReferer, 1, &pstrAcceptTypes, pstrVersion, dwHttpRequestFlags);
			pFile->AddRequestHeaders(szHeaders);
			pFile->SendRequest();

			pFile->QueryInfoStatusCode(dwRet);
			if (dwRet != HTTP_STATUS_OK)
				{
					return dwRet;
				}
		}

		if(dwRet == HTTP_STATUS_OK)
		{
			char buf[2000];
			CString tmp;
			int numread;

			CString filepath;
			filepath.Format(g_strHostFile);
			CStdioFile myfile(filepath, CFile::modeCreate|CFile::modeWrite|CFile::typeText);
			
			for (int j = 0; j < hosts->GetSize(); j++) {
				myfile.WriteString(hosts->GetAt(j));
			}

			StepIt();
			myfile.WriteString(BAKKA_START_TAG);
			while ((numread = pFile->Read(buf,sizeof(buf)-1)) > 0) {
				buf[numread] = '\0';
				myfile.Write(buf, numread);
				
				tmp = buf;
				tmp.Replace(L"\n", L"\r\n");

				ShowEditBox(tmp);
				ShowStatus(L"Fetching hosts from server");
			}
			StepIt();
			myfile.WriteString(BAKKA_END_TAG);
			myfile.Close();
			StepIt();

		} else {
			ShowStatus(L"ERROR: Can't fetch hosts from server, lost from internet?");
		}

		pFile->Close();      
		delete pFile;
		pServer->Close();  
		delete pServer;
		Session.Close();
	}

	catch (CInternetException* pEx) 
	{
		// catch any exceptions from WinINet      
		ShowStatus(L"ERROR: Can't fetch hosts from server");

		pEx->Delete();
		if(pFile)
			delete pFile;
		if(pServer)
			delete pServer;
		Session.Close(); 
		return 0;
	}

	return dwRet;
}