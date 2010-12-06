// ComboBar.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskTool.h"
#include "ComboBar.h"
#include "NetDiskToolView.h"
#include "ReEdit.h"

#include <winioctl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboBar

CComboBar::CComboBar()
{
}

CComboBar::~CComboBar()
{
}

void CComboBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComboBar)
	DDX_Control(pDX, IDC_COMBO_DISK, m_cbDisk);
	DDX_Control(pDX, IDC_COMBO_SECTOR, m_cbSector);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CComboBar, CDialogBar)
	//{{AFX_MSG_MAP(CComboBar)
	ON_CBN_SELCHANGE(IDC_COMBO_DISK, OnSelchangeComboDisk)
	ON_CBN_SELCHANGE(IDC_COMBO_SECTOR, OnSelchangeComboSector)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboBar message handlers


//BOOL CALLBACK EnumChildProc(  HWND hwnd,      // handle to child window
//							LPARAM lParam   // application-defined value
//							)
//{
//	char classname[200];
//	CComboBar *pBar = (CComboBar *)lParam;
//		
//	GetClassName( hwnd, classname, 200 );
//	if (0 == strcmp( classname, "Edit" ))
//	{
//		CWnd* pWnd = CWnd::FromHandlePermanent(hwnd);
//
//		if (NULL == pWnd)
//		{
//			CReEdit *pEdit = new CReEdit;
//			pEdit->SubclassWindow(hwnd);
//		}
//	}
//
//	return TRUE;
//}




BOOL CComboBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	BOOL bRet = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	UpdateData(FALSE);

	m_cbSector.AddString("0");
	GetDocument()->GetDiskList(this);
	m_cbDisk.SetCurSel(0);
	m_cbSector.SetCurSel(0);

//	EnumChildWindows( m_hWnd, EnumChildProc, (LPARAM)this );

	CWnd *pWnd = m_cbSector.GetWindow(GW_CHILD);

	if (NULL != pWnd)
	{
		char classname[200];
		GetClassName( pWnd->m_hWnd, classname, 200 );
		if (0 == strcmp( classname, "Edit" ))
		{
			if (NULL == CWnd::FromHandlePermanent(pWnd->m_hWnd))
			{
				CReEdit *pEdit = new CReEdit;
				bRet = pEdit->SubclassWindow(pWnd->m_hWnd);
			}
		}
	}

		
	return bRet;
}

BOOL CComboBar::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam)
		{
			CString strSector;
			m_cbSector.GetWindowText(strSector);
			if (!strSector.IsEmpty())
			{
				GetDocument()->OpenSector(strSector);
				GetDocument()->GetView()->SetFocus();
			}
		}
	}

	
	return CDialogBar::PreTranslateMessage(pMsg);
}

CNetDiskToolDoc* CComboBar::GetDocument()
{
	return theApp.GetDocumnet();
}

void CComboBar::OnSelchangeComboSector() 
{
	// TODO: Add your control notification handler code here
	GetDocument()->OpenSector();
}

void CComboBar::OnSelchangeComboDisk() 
{
	// TODO: Add your control notification handler code here
	GetDocument()->OpenSector();
}



