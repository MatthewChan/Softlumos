// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskTool.h"
#include "ClientSocket.h"

#include "netdisktooldoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket(CNetDiskToolDoc* pDoc)
{
	m_pDoc = pDoc;
}

CClientSocket::~CClientSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	m_pDoc->ProcessPendingRead();
	CAsyncSocket::OnReceive(nErrorCode);

}

void CClientSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pDoc->CloseConncet();
	CAsyncSocket::OnClose(nErrorCode);
}


