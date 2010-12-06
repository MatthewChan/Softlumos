// ReEdit.cpp : implementation file
//

#include "stdafx.h"
#include "netdisktool.h"
#include "ReEdit.h"
#include "ComboBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReEdit

CReEdit::CReEdit()
{
}

CReEdit::~CReEdit()
{
}


BEGIN_MESSAGE_MAP(CReEdit, CEdit)
	//{{AFX_MSG_MAP(CReEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReEdit message handlers

void CReEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	if ((GetKeyState(VK_CONTROL) >= 0) 
		&& VK_RETURN != nChar
		&& VK_BACK != nChar
		&& ('0' > nChar || nChar > '9')
		&& ('a' > nChar || nChar > 'f')
		&& ('A' > nChar || nChar > 'F'))
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

