// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskTool.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_nPort = 0;
	m_szAddress = _T("");
	m_szPassword = _T("");
	m_szUserName = _T("");
	m_szDestName = _T("");
	//}}AFX_DATA_INIT
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_nPort);
	DDX_Text(pDX, IDC_EDIT_SERVER_ADDRESS, m_szAddress);
	DDX_Text(pDX, IDC_EDIT_CTRL_PASSWORD, m_szPassword);
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_szUserName);
	DDX_Text(pDX, IDC_EDIT_CTRL_DESTNAME, m_szDestName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_bLocalData)
	{
		GetDlgItem(IDC_EDIT_CTRL_DESTNAME)->EnableWindow(FALSE);
	}
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
