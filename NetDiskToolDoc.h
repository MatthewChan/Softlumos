// NetDiskToolDoc.h : interface of the CNetDiskToolDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETDISKTOOLDOC_H__0321D3BC_50C7_4AAF_BF23_9490254F038D__INCLUDED_)
#define AFX_NETDISKTOOLDOC_H__0321D3BC_50C7_4AAF_BF23_9490254F038D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "ComboBar.h"
#include "Error_Code.h"	// Added by ClassView

class CNetDiskToolView;
class CClientSocket;
class CComboBar;

class CNetDiskToolDoc : public CDocument
{
protected: // create from serialization only
	CNetDiskToolDoc();
	DECLARE_DYNCREATE(CNetDiskToolDoc)

// Attributes
public:
	LPCONVECTION_MSG m_lpCM;

// Operations
public:
	void UpdateSectorUI(LPCTSTR szSector = NULL);
	void ShortcutOpenSector(BOOL bNext);
	BOOL GetDataType();
	BOOL OpenSector(LPCSTR strSector = NULL, BOOL bUpUI = TRUE);
	void CloseConncet();
	void ProcessPendingRead();
	void GetDiskList(CComboBar *pComboBar = NULL);
	CNetDiskToolView* GetView();

	virtual BOOL SaveModified();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetDiskToolDoc)
	public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
protected:

	void EnableAdminPrivilege(BOOL bEnable = TRUE);

	BOOL OpenSector(int nDisk, DWORD &dwSector,
		DWORD dwSectorCount = 1, char *szBuffer = NULL);
	
	BOOL WriteSector(int nDisk,
		DWORD dwSector = 0,
		DWORD dwSectorCount = 1, 
		const char *szBuffer = NULL);
	
	BOOL WriteSector();
		

	BOOL NetWriteStart();
	BOOL NetWriteDone();
	BOOL NetReadStart();
	BOOL NetReadDone();

	

	BOOL ConnectSocket(LPCTSTR strAddress, UINT nPort);

	virtual ~CNetDiskToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	LPSTR pcBuf;
	CComboBar *m_pComboBar;
	CClientSocket* m_pSocket;

// Generated message map functions
protected:
	BOOL m_bLocalData;
	DWORD m_dwConnectType;
	DWORD m_dwCurSector;
	DWORD m_dwDiskCount;
	LPDISK_INFO m_lpDI;


	//{{AFX_MSG(CNetDiskToolDoc)
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileOpen();
	afx_msg void OnFileType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETDISKTOOLDOC_H__0321D3BC_50C7_4AAF_BF23_9490254F038D__INCLUDED_)
