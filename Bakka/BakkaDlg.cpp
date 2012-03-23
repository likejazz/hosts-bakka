// BakkaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxinet.h"
#include "Bakka.h"
#include "BakkaDlg.h"
#include "HostsUpdater.h"
#include "SystemTray.h"
#include "Version.h"
#include "IniFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString g_strHostFile;
CString g_strHostBackupFile;

CString g_strTitle, g_strRestoreBtnName;
CString g_strButtonName1, g_strButtonName2, g_strButtonName3, g_strButtonName4, g_strButtonName5;
CString g_strButtonHost1, g_strButtonHost2, g_strButtonHost3, g_strButtonHost4, g_strButtonHost5;
CString g_strButtonFile1, g_strButtonFile2, g_strButtonFile3, g_strButtonFile4, g_strButtonFile5;

#define	WM_ICON_NOTIFY			WM_APP+10

CSystemTray m_TrayIcon;
CIniFile iniFile(L".\\Bakka.ini", 1024);

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CBakkaDlg dialog
CBakkaDlg::CBakkaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBakkaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBakkaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBakkaDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CBakkaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CBakkaDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CBakkaDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CBakkaDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CBakkaDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CBakkaDlg::OnBnClickedButton6)
	ON_MESSAGE(WM_ICON_NOTIFY, &CBakkaDlg::OnTrayNotification)
END_MESSAGE_MAP()


// CBakkaDlg message handlers

BOOL CBakkaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon



	/************************************************************************/
	/* Init ProgressCtrl                                                    */
	/************************************************************************/
	// Set ProgressCtrl's range and step
	m_ctlProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
	m_ctlProgress->SetRange(0, 100);
	m_ctlProgress->SetStep(5);



	/************************************************************************/
	/* Get System32 Directory                                               */
	/************************************************************************/
	CString path;
	LPTSTR p = path.GetBuffer(MAX_PATH);
	HRESULT hr = ::SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, p);
	if(SUCCEEDED(hr))
	{ /* succeeded */
		path.ReleaseBuffer();

		path += HOST_FILE_RELPATH;
		g_strHostFile = path;

		path += HOST_BFILE_POSTFIX;
		g_strHostBackupFile = path;

	} /* succeeded */
	else
	{ /* failed */
		path.ReleaseBuffer();

		ShowStatus(L"ERROR: Can't find windows system directory");

		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	} /* failed */



	/************************************************************************/
	/* Init tray icon                                                       */
	/************************************************************************/
	m_TrayIcon.Create(this, WM_ICON_NOTIFY,
	_T(VERSION_CAPTION), m_hIcon, IDR_POPUP_MENU);



	/************************************************************************/
	/* Init Buttons                                                         */
	/************************************************************************/
	// Read INI Information
	iniFile.GetString(L"global", L"title", g_strTitle, L"");

	// Set subtitle.
	if (g_strTitle == "") {
		g_strTitle = APP_NAME;
	} else {
		CString t_strTitle = g_strTitle;
		g_strTitle = APP_NAME;
		g_strTitle += " - ";
		g_strTitle += t_strTitle;
	}
	SetWindowText(g_strTitle);

	// Init RESTORE BUTTON
	iniFile.GetString(L"global", L"restore_btn_name", g_strRestoreBtnName, L"");
	if (g_strRestoreBtnName == "") {
		GetDlgItem(IDC_BUTTON6)->SetWindowText(BTNTEXT_RESTORE);
	} else {
		GetDlgItem(IDC_BUTTON6)->SetWindowText(g_strRestoreBtnName);
	}

	// Init BUTTON #1
	iniFile.GetString(L"button_1", L"name", g_strButtonName1, L"");
	iniFile.GetString(L"button_1", L"host", g_strButtonHost1, L"");
	iniFile.GetString(L"button_1", L"file", g_strButtonFile1, L"");
	if (g_strButtonName1 == "") {
		GetDlgItem(IDC_BUTTON1)->SetWindowText(BTNTEXT_NOTUSED);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_BUTTON1)->SetWindowText(g_strButtonName1);
	}

	// Init BUTTON #2
	iniFile.GetString(L"button_2", L"name", g_strButtonName2, L"");
	iniFile.GetString(L"button_2", L"host", g_strButtonHost2, L"");
	iniFile.GetString(L"button_2", L"file", g_strButtonFile2, L"");
	if (g_strButtonName2 == "") {
		GetDlgItem(IDC_BUTTON2)->SetWindowText(BTNTEXT_NOTUSED);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_BUTTON2)->SetWindowText(g_strButtonName2);
	}

	// Init BUTTON #3
	iniFile.GetString(L"button_3", L"name", g_strButtonName3, L"");
	iniFile.GetString(L"button_3", L"host", g_strButtonHost3, L"");
	iniFile.GetString(L"button_3", L"file", g_strButtonFile3, L"");
	if (g_strButtonName3 == "") {
		GetDlgItem(IDC_BUTTON3)->SetWindowText(BTNTEXT_NOTUSED);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_BUTTON3)->SetWindowText(g_strButtonName3);
	}

	// Init BUTTON #4
	iniFile.GetString(L"button_4", L"name", g_strButtonName4, L"");
	iniFile.GetString(L"button_4", L"host", g_strButtonHost4, L"");
	iniFile.GetString(L"button_4", L"file", g_strButtonFile4, L"");
	if (g_strButtonName4 == "") {
		GetDlgItem(IDC_BUTTON4)->SetWindowText(BTNTEXT_NOTUSED);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_BUTTON4)->SetWindowText(g_strButtonName4);
	}

	// Init BUTTON #5
	iniFile.GetString(L"button_5", L"name", g_strButtonName5, L"");
	iniFile.GetString(L"button_5", L"host", g_strButtonHost5, L"");
	iniFile.GetString(L"button_5", L"file", g_strButtonFile5, L"");
	if (g_strButtonName4 == "") {
		GetDlgItem(IDC_BUTTON5)->SetWindowText(BTNTEXT_NOTUSED);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_BUTTON5)->SetWindowText(g_strButtonName5);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBakkaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if (nID == SC_CLOSE) {
			m_TrayIcon.MinimiseToTray(this);
		} else {
			CDialog::OnSysCommand(nID, lParam);
		}
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBakkaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBakkaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Read Hosts, Remove BAKKA range. And Backup
BOOL CBakkaDlg::ReadHosts(CStringArray* hosts) {

	CStdioFile f;
	CString tmp;
	BOOL bDontRead = FALSE;
	int i = 0;

	if( !f.Open( g_strHostFile, CFile::modeRead | CFile::modeNoTruncate , NULL ) ) {
		ShowStatus(_T("ERROR: Can't find windows hosts file"));
		return FALSE;
	}

	while(f.ReadString(tmp)) {
		i++;

		if (tmp.Find(BAKKA_START) != LB_ERR) {
			bDontRead = TRUE;
		}

		if (!bDontRead) {
			hosts->Add(tmp + _T("\n"));
		}

		if (tmp.Find(BAKKA_END) != LB_ERR)
		{
			bDontRead = FALSE;
		}
	}

	f.Close();

	// if you can find a backup file delete it.
	try {
		f.Remove(g_strHostBackupFile);
	} catch(CFileException* pEx) {
		pEx->Delete();
#ifdef _DEBUG
		afxDump << g_strHostBackupFile << " does not exist.\n";
#endif
	}

	try {
		f.Rename(g_strHostFile, g_strHostBackupFile);
	} catch(CFileException* pEx) {
		ShowStatus(L"ERROR: You don't have enough privileges to change Windows hosts");
		pEx->Delete();
		return FALSE;
	}

	return TRUE;
}

void CBakkaDlg::ShowEditBox(LPCTSTR str) {
	CEdit * ce;
	ce = (CEdit*) GetDlgItem(IDC_EDIT);

	int length = ce->GetWindowTextLength();
	ce->SetSel(length, length);
	ce->ReplaceSel(str);
}

void CBakkaDlg::ShowStatus(LPCTSTR str) {
	CStatic * cs;
	cs = (CStatic*) GetDlgItem(IDC_STATIC);

	cs->SetWindowTextW(str);
}

void CBakkaDlg::StepIt() {
	m_ctlProgress->StepIt();
}

void CBakkaDlg::StepIt(DWORD pos) {
	m_ctlProgress->SetPos(pos);
}

void CBakkaDlg::ResetEditCtrlAndProgress() {
	StepIt(0);

	CEdit * ce;
	ce = (CEdit*) GetDlgItem(IDC_EDIT);
	int length = ce->GetWindowTextLength();
	ce->SetSel(0, length);
	ce->ReplaceSel(L"");

	StepIt();
}

// just eyecandy
void CBakkaDlg::DummyAndDone(int p) {
		for (int i = 0;i <= p;i++) {
			StepIt();
			Sleep(100);
		}

		StepIt(100);
		ShowStatus(L"Done");
}

void CBakkaDlg::RestoreHosts(CStringArray* hosts) {
		CStdioFile f;
		if( f.Open( g_strHostFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText, NULL ) ) {
			for (int j = 0; j < hosts->GetSize(); j++) {
				f.WriteString(hosts->GetAt(j));
			}
		}
}

// When you click "BTN1"
void CBakkaDlg::OnBnClickedOk()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	ResetEditCtrlAndProgress();

	DWORD Status;
	CStringArray hosts;

	if (ReadHosts(&hosts)) {
		StepIt();
		CHostsUpdater *Session = NULL;
		Session = new CHostsUpdater(NULL);
		Status = Session->GetAndSetHosts(g_strButtonHost1, 80, g_strButtonFile1, &hosts);

		if (Status == HTTP_STATUS_OK) {
			DummyAndDone(1);
		} else {
			RestoreHosts(&hosts);
		}
	}

	DnsFlushResolverCache();
	
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
}


// When you click "BTN2"
void CBakkaDlg::OnBnClickedButton2()
{
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	ResetEditCtrlAndProgress();

	DWORD Status;
	CStringArray hosts;

	if (ReadHosts(&hosts)) {
		StepIt();
		CHostsUpdater *Session = NULL;
		Session = new CHostsUpdater(NULL);
		Status = Session->GetAndSetHosts(g_strButtonHost2, 80, g_strButtonFile2, &hosts);

		if (Status == HTTP_STATUS_OK) {
			DummyAndDone(1);
		} else {
			RestoreHosts(&hosts);
		}
	}

	DnsFlushResolverCache();

	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
}

// When you click "BTN3"
void CBakkaDlg::OnBnClickedButton3()
{
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	ResetEditCtrlAndProgress();

	DWORD Status;
	CStringArray hosts;

	if (ReadHosts(&hosts)) {
		StepIt();
		CHostsUpdater *Session = NULL;
		Session = new CHostsUpdater(NULL);
		Status = Session->GetAndSetHosts(g_strButtonHost3, 80, g_strButtonFile3, &hosts);

		if (Status == HTTP_STATUS_OK) {
			DummyAndDone(1);
		} else {
			RestoreHosts(&hosts);
		}
	}

	DnsFlushResolverCache();

	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
}

// When you click "BTN4"
void CBakkaDlg::OnBnClickedButton4()
{
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	ResetEditCtrlAndProgress();

	DWORD Status;
	CStringArray hosts;

	if (ReadHosts(&hosts)) {
		StepIt();
		CHostsUpdater *Session = NULL;
		Session = new CHostsUpdater(NULL);
		Status = Session->GetAndSetHosts(g_strButtonHost4, 80, g_strButtonFile4, &hosts);

		if (Status == HTTP_STATUS_OK) {
			DummyAndDone(1);
		} else {
			RestoreHosts(&hosts);
		}
	}

	DnsFlushResolverCache();

	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
}

// When you click "BTN5"
void CBakkaDlg::OnBnClickedButton5()
{
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	ResetEditCtrlAndProgress();

	DWORD Status;
	CStringArray hosts;

	if (ReadHosts(&hosts)) {
		StepIt();
		CHostsUpdater *Session = NULL;
		Session = new CHostsUpdater(NULL);
		Status = Session->GetAndSetHosts(g_strButtonHost5, 80, g_strButtonFile5, &hosts);

		if (Status == HTTP_STATUS_OK) {
			DummyAndDone(1);
		} else {
			RestoreHosts(&hosts);
		}
	}

	DnsFlushResolverCache();

	GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
}

// When you click "RESTORE"
void CBakkaDlg::OnBnClickedButton6()
{
	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	ResetEditCtrlAndProgress();

	ShowStatus(L"Restoring Windows hosts to factory settings");
	
	CStringArray hosts;
	if (ReadHosts(&hosts)) {
		StepIt();
		RestoreHosts(&hosts);
		StepIt();
		DummyAndDone(5);
	}

	DnsFlushResolverCache();

	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
}

LRESULT CBakkaDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(lParam) == WM_LBUTTONDBLCLK) {
		CSystemTray::MaximiseFromTray(this);
    }

    return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

BOOL CBakkaDlg::DnsFlushResolverCache()
{
    BOOL (WINAPI *DoDnsFlushResolverCache)();
	*(FARPROC *)&DoDnsFlushResolverCache = GetProcAddress(LoadLibrary(L"dnsapi.dll"), "DnsFlushResolverCache");
    if(!DoDnsFlushResolverCache) return FALSE;
    return DoDnsFlushResolverCache();
}