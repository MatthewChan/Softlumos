#if !defined(AFX_TYPEDLG_H__1025676A_B4E1_45A1_A7B9_7986EC19FEF7__INCLUDED_)
#define AFX_TYPEDLG_H__1025676A_B4E1_45A1_A7B9_7986EC19FEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTypeDlg dialog

class CTypeDlg : public CDialog
{
// Construction
public:
	BOOL m_bLocal;
	CTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTypeDlg)
	enum { IDD = IDD_DIALOG_TYPE };
	BOOL	m_bControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioLocal();
	afx_msg void OnRadioRemote();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TYPEDLG_H__1025676A_B4E1_45A1_A7B9_7986EC19FEF7__INCLUDED_)
