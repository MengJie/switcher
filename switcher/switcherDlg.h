// switcherDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <lua.hpp>

// CswitcherDlg dialog
class CswitcherDlg : public CDialog
{
// Construction
public:
	CswitcherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SWITCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	lua_State * L;
	int m_nListIdx;

	virtual void InitPList();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnDestroy();
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnBnClickedExit();
public:
	CEdit m_edSearch;
public:
	afx_msg void OnEnChangeSearch();
public:
	static BOOL CALLBACK EnumWindowCB(HWND hwnd, LPARAM lParam);
	static BOOL CALLBACK EnumChildWindowCB(HWND hwnd, LPARAM lParam);
public:
//	afx_msg void OnLbnSelchangeList1();
public:
	CListBox m_liResult;
};
