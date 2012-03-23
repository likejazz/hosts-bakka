// BakkaDlg.h : header file
//

#pragma once

// CBakkaDlg dialog
class CBakkaDlg : public CDialog
{
// Construction
public:
	CBakkaDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BAKKA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	CProgressCtrl * m_ctlProgress;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void ShowEditBox(LPCTSTR str);
	void ShowStatus(LPCTSTR str);
	void StepIt();
	void StepIt(DWORD pos);
	void ResetEditCtrlAndProgress();
	void DummyAndDone(int p);
	void RestoreHosts(CStringArray* hosts);
	BOOL ReadHosts(CStringArray* hosts);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	BOOL DnsFlushResolverCache();
};
