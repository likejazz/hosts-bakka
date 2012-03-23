// MyInternetSession.h: interface for the CHostsUpdater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYINTERNETSESSION_H__67F1B46F_249C_11D6_B00A_0080C6F1016A__INCLUDED_)
#define AFX_MYINTERNETSESSION_H__67F1B46F_249C_11D6_B00A_0080C6F1016A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afx.h>
#include <afxwin.h>
#include <afxinet.h>
#include <iostream>
#include <stdlib.h>

class CHostsUpdater : public CInternetSession  
{
public:
	CHostsUpdater(LPCTSTR pstrAgent);

	virtual void OnStatusCallback(DWORD dwContext, 
								  DWORD dwInternetStatus,
								  LPVOID lpvStatusInformation, 
								  DWORD dwStatusInformationLength);

	DWORD GetAndSetHosts(LPCTSTR lpstrServer,
					 int nPort, 
					 CString lpstrFile,
					 CStringArray* hosts);

	void ShowStatus(LPCTSTR str);
	void ShowEditBox(LPCTSTR str);
	void StepIt();
	void StepIt(DWORD pos);

};

#endif // !defined(AFX_MYINTERNETSESSION_H__67F1B46F_249C_11D6_B00A_0080C6F1016A__INCLUDED_)
