#if !defined(AFX_LOGINDLG_H__44E221B0_A2CB_46A1_8C97_0AAC5ADF7726__INCLUDED_)
#define AFX_LOGINDLG_H__44E221B0_A2CB_46A1_8C97_0AAC5ADF7726__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

class CLoginDlg : public CDialog
{
// Construction
public:
	BOOL m_bLocalData;
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_DIALOG_LOGIN };
	UINT	m_nPort;
	CString	m_szAddress;
	CString	m_szPassword;
	CString	m_szUserName;
	CString	m_szDestName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__44E221B0_A2CB_46A1_8C97_0AAC5ADF7726__INCLUDED_)
