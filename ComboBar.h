#if !defined(AFX_COMBOBAR_H__91C8F503_128C_4BE4_A3C9_D72EEC3DA4BD__INCLUDED_)
#define AFX_COMBOBAR_H__91C8F503_128C_4BE4_A3C9_D72EEC3DA4BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboBar window
#include "NetDiskToolDoc.h"

#include <afxtempl.h>


class CNetDiskToolDoc;


class CComboBar : public CDialogBar
{
// Construction
public:
	CComboBar();


	//{{AFX_DATA(CComboBar)
	enum { IDD = IDR_COMBOBAR };
	CComboBox	m_cbDisk;
	CComboBox	m_cbSector;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CNetDiskToolDoc* GetDocument();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboBar)
	public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CComboBar();

	// Generated message map functions
protected:
	HANDLE m_hDisk;
	CList<DISK_INFO, DISK_INFO>	m_DiskInfoList;		
	//{{AFX_MSG(CComboBar)
	afx_msg void OnSelchangeComboDisk();
	afx_msg void OnSelchangeComboSector();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOBAR_H__91C8F503_128C_4BE4_A3C9_D72EEC3DA4BD__INCLUDED_)
