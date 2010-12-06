// NetDiskToolView.h : interface of the CNetDiskToolView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETDISKTOOLVIEW_H__6F916493_1D12_43F4_839B_799F7D63DF5D__INCLUDED_)
#define AFX_NETDISKTOOLVIEW_H__6F916493_1D12_43F4_839B_799F7D63DF5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxtempl.h>

class CNetDiskToolDoc;


#define SECTOR_SIZE 0x200

class CNetDiskToolView : public CView
{
protected: // create from serialization only
	CNetDiskToolView();
	DECLARE_DYNCREATE(CNetDiskToolView)

// Attributes
public:
	LONG m_CharCount;

// Operations
public:
	void SetText(char *input, int len);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetDiskToolView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
public:
	void OnUpdateSectorUI();
	void OnShortcutOpenSector(BOOL bNext);
	virtual ~CNetDiskToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bFirst;						//�Ƿ�Ϊʮ���������ĸ�λ�ַ�
	BOOL m_StartDrag;					//�Ƿ�����ק
	BOOL m_bFocused;

	LONG m_PageRows;					//һҳ������
	LONG m_PageCols;					//һҳ������
	LONG m_MaxCols;						//�ܵ�����
	LONG m_Rows;						//�ܵ�����

	LONG m_TopLine;						//���������
	LONG m_LeftChar;					//���������
	LONG m_TextStart;					//��ʼ��ʾASCII���ݵ�λ��
	LONG m_HexStart;					//��ʼ��ʾʮ�������ݵ�λ��
	UINT m_LineHeight;					//�и�
	LONG m_CharWidth;					//�ַ����
	LONG m_SelStart, m_SelEnd;			//��ѡ�е��ַ���
	BOOL m_bEditHex;					//�Ƿ��ڱ༭Hex, NOZEROʱ��ʾ�ڱ༭Hex����

	char* m_text;						//��ǰ����ָ��

	CList<UINT, UINT>	m_ModifyList;	//��¼���޸Ĺ���λ��

	void ChangeLeft();					//������ʾ��������ַ�λ��
	void ChangeTop();					//������ʾ�����ϱ��ַ�λ��
	void GetPageRowCols();				//����ûһҳ����ʾ������������
	
	void SetHorzScrollBar();			//���ù�����
	void SetVertScrollBar();
	LONG Point2Pos(CPoint point);		//�������λ�ü����Ӧ���ַ��ڻ�������λ��


// Generated message map functions
protected:
	void SetPositionInfo(LONG nPosition);
	//{{AFX_MSG(CNetDiskToolView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	//����������
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCharLeft();
	afx_msg void OnCharRight();
	afx_msg void OnLineUp();
	afx_msg void OnLineDown();
};

//#ifndef _DEBUG  // debug version in NetDiskToolView.cpp
//CNetDiskToolDoc* CNetDiskToolView::GetDocument()
//{ return (CNetDiskToolDoc*)m_pDocument; }
//#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETDISKTOOLVIEW_H__6F916493_1D12_43F4_839B_799F7D63DF5D__INCLUDED_)
