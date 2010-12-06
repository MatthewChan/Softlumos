// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "NetDiskTool.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
//	if (!m_wndToolBar.CreateEx(this) ||
//		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
//	{
//		TRACE0("Failed to create toolbar\n");
//		return -1;      // fail to create
//	}


//	if (!m_wndDlgBar.Create(this, IDR_COMBOBAR, 
//		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	if (!m_wndDlgBar.Create(this, IDR_COMBOBAR, 
		CBRS_ALIGN_TOP, NULL))

	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}


// 	if (!m_wndReBar.Create(this) ||
// //		!m_wndReBar.AddBar(&m_wndToolBar) ||
// 		!m_wndReBar.AddBar(&m_wndDlgBar))
// 	{
// 		TRACE0("Failed to create rebar\n");
// 		return -1;      // fail to create
// 	}

// 	m_wndDlgBar.SetParent(this);
// 	m_wndDlgBar.ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
//	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
//		CBRS_TOOLTIPS | CBRS_FLYBY);

	SetWindowPos(NULL, 0, 0, MIN_WIDTH, MIN_HEIGHT, SWP_NOZORDER|SWP_NOMOVE);
	CenterWindow();

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers




BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
// 	return ((CDialog*)this)->CDialog::PreTranslateMessage(pMsg);

	if (0 <= GetAsyncKeyState(VK_CONTROL))
	{
		if (PreTranslateInput(pMsg))
		{
			if ((WM_KEYDOWN != pMsg->message
				&& WM_CHAR != pMsg->message)
				|| VK_TAB != pMsg->wParam)
				return TRUE;
		}
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMinTrackSize.x = MIN_WIDTH;
	lpMMI->ptMinTrackSize.y = MIN_HEIGHT;
	
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}
