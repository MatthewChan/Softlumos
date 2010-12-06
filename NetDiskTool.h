// NetDiskTool.h : main header file for the NETDISKTOOL application
//

#if !defined(AFX_NETDISKTOOL_H__E762E603_5C33_4B31_B19E_180F4CA37BC1__INCLUDED_)
#define AFX_NETDISKTOOL_H__E762E603_5C33_4B31_B19E_180F4CA37BC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Error_Code.h"	// Added by ClassView
#include "NetDiskToolDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CNetDiskToolApp:
// See NetDiskTool.cpp for the implementation of this class
//

class CNetDiskToolApp : public CWinApp
{
public:
	CNetDiskToolDoc * GetDocumnet();
	CNetDiskToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetDiskToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNetDiskToolApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CNetDiskToolApp theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETDISKTOOL_H__E762E603_5C33_4B31_B19E_180F4CA37BC1__INCLUDED_)
