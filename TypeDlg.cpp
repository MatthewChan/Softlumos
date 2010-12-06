// TypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "netdisktool.h"
#include "TypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTypeDlg dialog


CTypeDlg::CTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTypeDlg)
	m_bControl = FALSE;
	//}}AFX_DATA_INIT
}


void CTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTypeDlg)
	DDX_Check(pDX, IDC_CHECK_CONTROL, m_bControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CTypeDlg)
	ON_BN_CLICKED(IDC_RADIO_LOCAL, OnRadioLocal)
	ON_BN_CLICKED(IDC_RADIO_REMOTE, OnRadioRemote)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTypeDlg message handlers

BOOL CTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

//	m_bLocal = TRUE;
	if (m_bLocal)
	{
		CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_LOCAL);
		pBtn->SetCheck(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_CONTROL)->EnableWindow(FALSE);
		CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_REMOTE);
		pBtn->SetCheck(TRUE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTypeDlg::OnRadioLocal() 
{
	// TODO: Add your control notification handler code here
	m_bLocal = TRUE;
	GetDlgItem(IDC_CHECK_CONTROL)->EnableWindow(TRUE);
//	CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_LOCAL);
//	pBtn->SetCheck(m_bLocal);
}

void CTypeDlg::OnRadioRemote() 
{
	// TODO: Add your control notification handler code here
	m_bLocal = FALSE;
	GetDlgItem(IDC_CHECK_CONTROL)->EnableWindow(FALSE);
//	CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_LOCAL);
//	pBtn->SetCheck(m_bLocal);
}
