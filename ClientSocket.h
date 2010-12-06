#if !defined(AFX_CLIENTSOCKET_H__E8F0C4F0_3239_40F1_8DDC_4BC4D6D7431A__INCLUDED_)
#define AFX_CLIENTSOCKET_H__E8F0C4F0_3239_40F1_8DDC_4BC4D6D7431A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//

class CNetDiskToolDoc;


/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

class CClientSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CClientSocket(CNetDiskToolDoc* pDoc);
	virtual ~CClientSocket();

// Overrides
public:
	CNetDiskToolDoc* m_pDoc;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__E8F0C4F0_3239_40F1_8DDC_4BC4D6D7431A__INCLUDED_)
