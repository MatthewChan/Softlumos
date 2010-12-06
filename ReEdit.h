#if !defined(AFX_REEDIT_H__EF4A73D4_3A44_451F_AE74_A326585EABF8__INCLUDED_)
#define AFX_REEDIT_H__EF4A73D4_3A44_451F_AE74_A326585EABF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReEdit window

class CReEdit : public CEdit
{
// Construction
public:
	CReEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReEdit)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CReEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REEDIT_H__EF4A73D4_3A44_451F_AE74_A326585EABF8__INCLUDED_)
