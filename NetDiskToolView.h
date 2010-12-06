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
	BOOL m_bFirst;						//是否为十六进制数的高位字符
	BOOL m_StartDrag;					//是否在拖拽
	BOOL m_bFocused;

	LONG m_PageRows;					//一页的行数
	LONG m_PageCols;					//一页的列数
	LONG m_MaxCols;						//总的列数
	LONG m_Rows;						//总的行数

	LONG m_TopLine;						//最上面的行
	LONG m_LeftChar;					//最左面的列
	LONG m_TextStart;					//开始显示ASCII数据的位置
	LONG m_HexStart;					//开始显示十六进数据的位置
	UINT m_LineHeight;					//行高
	LONG m_CharWidth;					//字符宽度
	LONG m_SelStart, m_SelEnd;			//被选中的字符串
	BOOL m_bEditHex;					//是否在编辑Hex, NOZERO时表示在编辑Hex数据

	char* m_text;						//当前数据指针

	CList<UINT, UINT>	m_ModifyList;	//记录被修改过的位置

	void ChangeLeft();					//计算显示的最左边字符位置
	void ChangeTop();					//计算显示的最上边字符位置
	void GetPageRowCols();				//计算没一页可显示的行数和列数
	
	void SetHorzScrollBar();			//设置滚动条
	void SetVertScrollBar();
	LONG Point2Pos(CPoint point);		//根据鼠标位置计算对应的字符在缓冲区的位置


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
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	//处理鼠标滚轮
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
