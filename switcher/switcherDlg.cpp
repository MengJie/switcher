// switcherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "switcher.h"
#include "switcherDlg.h"
#include "lswitch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const static int BUFFER_LEN = 1024;

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


// CswitcherDlg dialog




CswitcherDlg::CswitcherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CswitcherDlg::IDD, pParent), L(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CswitcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEARCH, m_edSearch);
	DDX_Control(pDX, IDC_LIST_RESULT, m_liResult);
}

BEGIN_MESSAGE_MAP(CswitcherDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_EXIT, &CswitcherDlg::OnBnClickedExit)
	ON_EN_CHANGE(IDC_SEARCH, &CswitcherDlg::OnEnChangeSearch)
//	ON_LBN_SELCHANGE(IDC_LIST1, &CswitcherDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CswitcherDlg message handlers

BOOL CswitcherDlg::OnInitDialog()
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

	// TODO: Add extra initialization here
	L = lua_open();
	luaL_openlibs(L);
	lswitch_openswitch(L);

	if (luaL_dofile(L, "init.lua")) {
		TRACE("%s\n", lua_tostring(L, 1));
		lua_pop(L, 1);
	}

	InitPList();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CswitcherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CswitcherDlg::OnPaint()
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
HCURSOR CswitcherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CswitcherDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	RegisterHotKey(this->GetSafeHwnd(), 1, MOD_ALT, VK_BACK);

	return 0;
}

void CswitcherDlg::OnDestroy()
{
	CDialog::OnDestroy();
	UnregisterHotKey(this->GetSafeHwnd(), 1);
	lua_close(L);
}

void CswitcherDlg::InitPList()
{
	lua_newtable(L);
	lua_setglobal(L, "plist");
	m_nListIdx = 1;
	EnumWindows(EnumWindowCB, (LPARAM)this);
}

LRESULT CswitcherDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_HOTKEY == message) {
		this->ShowWindow(SW_NORMAL);
		this->SetForegroundWindow();
		this->InitPList();
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CswitcherDlg::OnCancel()
{
	this->ShowWindow(SW_MINIMIZE);
}

void CswitcherDlg::OnOK()
{
	this->ShowWindow(SW_MINIMIZE);
	lua_getglobal(L, "onok");
	if (lua_pcall(L, 0, 0, 0)) {
		TRACE("%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

void CswitcherDlg::OnBnClickedExit()
{
	CDialog::OnCancel();
}

void CswitcherDlg::OnEnChangeSearch()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	static TCHAR searchtext[BUFFER_LEN];

	this->m_edSearch.GetLine(0, searchtext, BUFFER_LEN);

	lua_getglobal(L, "onsearchtextchange");
	lua_pushstring(L, searchtext);
	if (lua_pcall(L, 1, 0, 0)) {
		TRACE("%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	this->m_liResult.ResetContent();

	lua_getglobal(L, "result");

	int idx = 1;
	lua_pushnumber(L, (lua_Number)idx);
	lua_gettable(L, -2);

	while (!lua_isnil(L, -1)) {
		lua_getfield(L, -1, "program");
		this->m_liResult.AddString(lua_tostring(L, -1));

		lua_pop(L, 2); // item.program and item
		
		++idx;
		lua_pushnumber(L, (lua_Number)idx);
		lua_gettable(L, -2);
	}
	lua_pop(L, 1); // result
}

BOOL CswitcherDlg::EnumWindowCB(HWND hwnd, LPARAM lParam)
{
	static TCHAR module[BUFFER_LEN];
	static TCHAR wclass[BUFFER_LEN];
	static TCHAR program[BUFFER_LEN];
	//::GetClassName(hwnd, module, BUFFER_LEN);
	//TRACE("class : %s\n", module);
	//if (0 == strcmp(module, "Shell_TrayWnd")) {
	//	::EnumChildWindows(hwnd, EnumChildWindowCB, lParam);
	//}
	if (!::IsWindowVisible(hwnd)) return true;
	::GetWindowText(hwnd, module, BUFFER_LEN);
	if (0 == module[0]) return true;
	::GetClassName(hwnd, wclass, BUFFER_LEN);
	::_snprintf(program, BUFFER_LEN, "%s [%s]", module, wclass);

	CswitcherDlg * pThis = (CswitcherDlg *)lParam;
	lua_State * L = pThis->L;
	int & idx = pThis->m_nListIdx;

	lua_getglobal(L, "plist");
	
	lua_pushnumber(L, (lua_Number)idx++);
	lua_pushstring(L, program);
	lua_settable(L, -3);

	lua_pushnumber(L, (lua_Number)idx++);
	lua_pushlightuserdata(L, (void *)hwnd);
	lua_settable(L, -3);

	lua_pop(L, 1);

	return true;
}

BOOL CswitcherDlg::EnumChildWindowCB(HWND hwnd, LPARAM lParam)
{
	const int BUFFER_LEN = 1024;
	static TCHAR module[BUFFER_LEN];
	::GetWindowText(hwnd, module, BUFFER_LEN);
	//if (0 == module[0]) return true;
	TRACE("Tray : %s\n", module);
	return true;
}
