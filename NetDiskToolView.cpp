// NetDiskToolView.cpp : implementation of the CNetDiskToolView class
//

#include "stdafx.h"
#include "NetDiskTool.h"
#include "MainFrm.h"

#include "NetDiskToolDoc.h"
#include "NetDiskToolView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetDiskToolView

IMPLEMENT_DYNCREATE(CNetDiskToolView, CView)

BEGIN_MESSAGE_MAP(CNetDiskToolView, CView)
	//{{AFX_MSG_MAP(CNetDiskToolView)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_CHAR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetDiskToolView construction/destruction

CNetDiskToolView::CNetDiskToolView()
{
	m_PageRows = 0;					//一页的行数
	m_PageCols = 0;					//一页的列数
	m_Rows = 0;						//总的行数
	
	m_TopLine = 0;						//最上面的行
	m_LeftChar = 0;					//最左面的列
	m_TextStart = 0;					//开始显示ASCII数据的位置
	m_HexStart = 0;					//开始显示十六进数据的位置
	m_LineHeight = 0;					//行高
	m_CharWidth = 0;					//字符宽度
	m_SelStart = 0;
	m_SelEnd = 0;			//被选中的字符串
	m_bEditHex = 0;					//是否在编辑Hex, NOZERO时表示在编辑Hex数据

	m_text = NULL;
	m_CharCount = 0;
}

CNetDiskToolView::~CNetDiskToolView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CNetDiskToolView drawing

void CNetDiskToolView::OnDraw(CDC* pDC)
{
//	CNetDiskToolDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CRect rc;
	GetClientRect(rc);

	if(NULL == m_text || 0 >= m_CharCount)
	{
		pDC->FillRect(rc, &CBrush(RGB(255,255,255)));
		return;
	}
	

	CDC	dc;
	dc.CreateCompatibleDC(pDC);

	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	dc.SelectObject(bm);
	dc.SetBoundsRect(&rc, DCB_DISABLE);

	CFont font;
	font.CreateFont ( m_LineHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, _T("FixedSys"));
	CFont* oldFont = dc.SelectObject ( &font );

	CBrush bkBrush(RGB(255,255,255));
    dc.FillRect(rc, &bkBrush);
	
	LONG lineCY, hexLineCX, textLineCX;
	LONG  nPos;
	char currentchar[20];

	m_HexStart = m_CharWidth * (8 - m_LeftChar);//计算二进制开始显示的位置
	m_TextStart = m_HexStart + m_CharWidth * 50;//计算ASCII码开始显示的位置

	for(LONG i = 0; i < m_PageRows; i++)//按行处理
	{
		lineCY = i * m_LineHeight;

		sprintf(currentchar, "%03Xh:", (m_TopLine-1+i)*16);	//chris
//		sprintf(currentchar, "%08Xh:", (m_TopLine+i)*16);

		dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		dc.SetBkColor(RGB(255,255,255));

		dc.TextOut(m_CharWidth*(1-m_LeftChar), lineCY, currentchar);//显示地址

//		memcpy(currentchar,&m_text[(m_TopLine+i-1)*16], 16);
//		currentchar[16] = '\0';
//
//		if((m_TopLine+i)*16 > m_CharCount)
//		{
//			currentchar[m_CharCount%16] = '\0';
//		}
//		
//		dc.TextOut (m_TextStart, lineCY, currentchar);//先打印本行的字符串，这样可以显示非ASCII文本

		for(UINT j = 0; j < 16; j++)
		{
			nPos = j + (m_TopLine+i-1)*16;
//			nPos = j + (m_TopLine+i)*16;	//chris

//			if(nPos > (m_CharCount-1))
//			if(nPos >= (m_CharCount-1))	//chris
//				goto bitblt;


			dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			dc.SetBkColor(RGB(255,255,255));
			textLineCX = m_TextStart + j * m_CharWidth;
			sprintf(currentchar, "%c", m_text[nPos]);
//			dc.TextOut (textLineCX, lineCY, currentchar);
			dc.TextOut (textLineCX, lineCY, m_text[nPos]);
			
		
			if(nPos >= m_SelStart && nPos <= m_SelEnd)//被选中则反色处理
			{
				dc.SetBkColor(::GetSysColor ( COLOR_HIGHLIGHT ));
				dc.SetTextColor(::GetSysColor ( COLOR_HIGHLIGHTTEXT ));

//				textLineCX = m_TextStart + j * m_CharWidth ;
				sprintf(currentchar, "%c", m_text[nPos]);
//				dc.TextOut (textLineCX, lineCY, currentchar);
				dc.TextOut (textLineCX, lineCY, m_text[nPos]);
			}
			else if (m_ModifyList.Find(nPos))
			{
				dc.SetTextColor(RGB(255, 0, 0));
				dc.SetBkColor(RGB(255,255,255));
			}
			else
			{
				dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
				dc.SetBkColor(RGB(255,255,255));
			}


			hexLineCX = m_HexStart + j * m_CharWidth *3 ;
			sprintf(currentchar, "%02X", ((UCHAR *)m_text)[nPos]);
			dc.TextOut (hexLineCX, lineCY, currentchar);

			if(nPos == m_SelEnd)//计算当前要编辑的字符位置
			{
//				dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
				dc.SetTextColor(RGB(255, 255, 255));	//chirs
				dc.SetBkColor(RGB(0,0,255));
				UCHAR cHex = m_text[nPos];

				if (0 != m_bEditHex)
				{
					//光标在Hex区域时
					if(m_bFirst)
					{
						cHex = cHex & 0xf0;
						cHex = cHex >> 4;
						sprintf(currentchar, "%X", cHex);
						dc.TextOut (hexLineCX, lineCY, currentchar);
					}
					else
					{
						hexLineCX = m_HexStart + j * m_CharWidth * 3 + m_CharWidth;
						cHex = cHex & 0x0f;
						sprintf(currentchar, "%X", cHex);
						dc.TextOut (hexLineCX, lineCY, currentchar);
					}
				}
				else
				{
					//光标在Text区域时
//					sprintf(currentchar, "%c", m_text[nPos]);
					dc.TextOut (textLineCX, lineCY, cHex);
				}
			}

			if(nPos >= (m_CharCount-1))	//chris
				goto bitblt;
		}
	}

bitblt:
	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &dc, 0, 0, SRCCOPY);
	dc.DeleteDC();
}

/////////////////////////////////////////////////////////////////////////////
// CNetDiskToolView printing

BOOL CNetDiskToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNetDiskToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNetDiskToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CNetDiskToolView diagnostics

#ifdef _DEBUG
void CNetDiskToolView::AssertValid() const
{
	CView::AssertValid();
}

void CNetDiskToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNetDiskToolView message handlers

void CNetDiskToolView::SetText(char *input, int len)
{
	if (NULL == input || 0 >= len)
	{
		m_text = NULL;
		m_CharCount = 0;
		return;
	}


	m_text = input;

	m_ModifyList.RemoveAll();

	m_CharCount = len;
	m_Rows = m_CharCount/16;

	m_bEditHex = 1;
	m_TopLine = 1;
	m_LeftChar = 0;
	m_SelStart = 0;
	m_SelEnd = 0;
	m_StartDrag = FALSE;
	m_bFirst = TRUE;

	SetPositionInfo(m_SelStart);


	CFont font;
	m_LineHeight = 15;
	font.CreateFont ( m_LineHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, _T("FixedSys"));
	CFont* oldFont = GetDC()->SelectObject ( &font );
	
	CSize size = GetDC()->GetTextExtent("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	m_LineHeight = size.cy;
	m_CharWidth = size.cx / 62;

	m_MaxCols = 75;
	
	GetPageRowCols();
	SetVertScrollBar();
	SetHorzScrollBar();
}


void CNetDiskToolView::OnSetFocus(CWnd* pOldWnd) 
{

	CView::OnSetFocus(pOldWnd);
	
	m_bFocused = TRUE;
}

void CNetDiskToolView::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);
	
	m_bFocused = FALSE;
}

//处理鼠标按键及拖动
void CNetDiskToolView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CView::OnLButtonDown(nFlags, point);
	m_StartDrag = TRUE;
	m_bFirst = TRUE;

	m_SelStart = m_SelEnd = Point2Pos(point);
	SetPositionInfo(m_SelStart);

	Invalidate(FALSE);
}

void CNetDiskToolView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CView::OnLButtonUp(nFlags, point);
	
	if(m_StartDrag)
	{
		m_SelEnd = Point2Pos(point);
		Invalidate(FALSE);
		m_StartDrag = FALSE;
	}
}


void CNetDiskToolView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CView::OnLButtonDblClk(nFlags, point);

	if (m_SelEnd == Point2Pos(point))
		m_bFirst = !m_bFirst;

	Invalidate(FALSE);
}



void CNetDiskToolView::OnMouseMove(UINT nFlags, CPoint point) 
{

	CView::OnMouseMove(nFlags, point);
	if(m_StartDrag)
	{
		m_SelEnd = Point2Pos(point);
		Invalidate(FALSE);
	}
}

//根据鼠标位置计算对应的字符在缓冲区的位置
LONG CNetDiskToolView::Point2Pos( CPoint point )
{
	if(m_text == NULL)
		return 0;

	LONG line = m_TopLine + point.y / m_LineHeight - 1;
	LONG charset;
	
	m_bEditHex = 1;
	if (point.x < m_HexStart)
	{
		charset = 0;
	}
	else if (point.x < m_HexStart + m_CharWidth * 48)
	{
		charset = (point.x - m_HexStart) / (m_CharWidth * 3);
	}
	else if (point.x < m_TextStart)
	{
		charset = 15;
	}
	else if(point.x >= m_TextStart && point.x < m_TextStart + 16 * m_CharWidth)
	{
		charset = (point.x - m_TextStart)/m_CharWidth;
		m_bEditHex = 0;
	}
	else
	{
		charset = 15;
		m_bEditHex = 0;
	}
	return ((line * 16 + charset)>m_CharCount? m_CharCount-1 : (line * 16 + charset));
}

void CNetDiskToolView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	if (NULL != m_text)
	{
		GetPageRowCols();
	}
	SetVertScrollBar();
	SetHorzScrollBar();
}

//以下设置滚动条的位置等信息
void CNetDiskToolView::SetVertScrollBar()
{	
	if((m_Rows - m_TopLine +  2) < m_PageRows) m_TopLine = 1;

	SCROLLINFO si;
	ZeroMemory(&si,sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	si.nMin = 0;
	si.nMax = m_Rows-1;
	si.nPage = m_PageRows;
	si.nPos = m_TopLine-1;
	VERIFY(SetScrollInfo(SB_VERT, &si, TRUE));
	Invalidate(FALSE);
}

void CNetDiskToolView::SetHorzScrollBar()
{
	if((80 - m_LeftChar + 2) < m_PageCols) m_LeftChar = 0;
	
	SCROLLINFO si;
	ZeroMemory(&si,sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask =  SIF_PAGE | SIF_POS | SIF_RANGE;
	si.nMin = 0;
	si.nMax = m_MaxCols;
	si.nPage = m_PageCols;
	si.nPos = m_LeftChar;
	VERIFY(SetScrollInfo(SB_HORZ, &si, TRUE));
	Invalidate(FALSE);
}

BOOL CNetDiskToolView::PreCreateWindow(CREATESTRUCT& cs) 
{
	CWnd *pParentWnd = CWnd::FromHandlePermanent(cs.hwndParent);
	if (pParentWnd == NULL || ! pParentWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
	{
		//	View must always create its own scrollbars,
		//	if only it's not used within splitter
		cs.style |= (WS_HSCROLL | WS_VSCROLL);
// 		cs.dwExStyle |= WS_EX_CONTROLPARENT;
	}
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);	
	return CView::PreCreateWindow(cs);
}

//处理鼠标滚轮
BOOL CNetDiskToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	
	if(zDelta > 0)
	{
		m_TopLine = --m_TopLine < 1 ? 1 : m_TopLine;
	}
	else
	{
		if((m_TopLine + m_PageRows -2) < m_Rows)
			m_TopLine+=1;
	}
	SetVertScrollBar();
	SetHorzScrollBar();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

//计算没一页可显示的行数和列数
void CNetDiskToolView::GetPageRowCols()
{
	CRect rc;
	if (NULL != m_text)
	{
		GetClientRect(rc);
		m_PageRows =  rc.Height() / m_LineHeight;
		m_PageCols = rc.Width() / m_CharWidth;
	}
}

//下面是对滚动条的处理
void CNetDiskToolView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CView::OnHScroll(nSBCode, nPos, pScrollBar);

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	VERIFY(GetScrollInfo(SB_HORZ, &si));

	switch (nSBCode)
	{
	case SB_LEFT:
		m_LeftChar = 0;
		break;
	case SB_BOTTOM:
		m_LeftChar = 80 - m_PageCols + 1;
		break;
	case SB_LINEUP:
		m_LeftChar = m_LeftChar - 1;
		break;
	case SB_LINEDOWN:
		m_LeftChar = m_LeftChar + 1;
		break;
	case SB_PAGEUP:
		m_LeftChar = m_LeftChar - si.nPage + 1;
		break;
	case SB_PAGEDOWN:
		m_LeftChar = m_LeftChar + si.nPage - 1;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_LeftChar = si.nTrackPos;
		break;
	default:
		return;
	}

	if((m_LeftChar + m_PageCols - 2) > 80) m_LeftChar = 80 - m_PageCols +2;
	if(m_LeftChar < 0) m_LeftChar = 0;

	SetHorzScrollBar();
}

void CNetDiskToolView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CView::OnVScroll(nSBCode, nPos, pScrollBar);

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	VERIFY(GetScrollInfo(SB_VERT, &si));

	switch (nSBCode)
	{
	case SB_TOP:
		m_TopLine = 0;
		break;
	case SB_BOTTOM:
		m_TopLine = m_Rows  - m_PageRows + 1;
		break;
	case SB_LINEUP:
		m_TopLine = m_TopLine - 1;
		break;
	case SB_LINEDOWN:
		m_TopLine = m_TopLine + 1;
		break;
	case SB_PAGEUP:
		m_TopLine = m_TopLine - si.nPage + 1;
		break;
	case SB_PAGEDOWN:
		m_TopLine = m_TopLine + si.nPage - 1;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_TopLine = si.nTrackPos+1;
		break;
	default:
		return;
	}

	if((m_TopLine + m_PageRows -2) > m_Rows) m_TopLine = m_Rows - m_PageRows + 2;
	if(m_TopLine < 1) m_TopLine = 1;

	SetVertScrollBar();
}
//0-9a-fA-F键的处理
void CNetDiskToolView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	CView::OnChar(nChar, nRepCnt, nFlags);
	UINT input;
	UCHAR cHex = m_text[m_SelEnd];
	
		
	//	InsterModifyIndex(m_SelEnd);
	if(NULL == m_ModifyList.Find(m_SelEnd))
		m_ModifyList.AddHead(m_SelEnd);
	
	//用输入的字符替换对应位置的数据
	if (0 != m_bEditHex)
	{
		//光标在Hex区域时
		if(nChar > 47 && nChar < 58)
		{
			input = nChar - 48;
		}
		else if (nChar > 64 && nChar < 71)
		{
			input = nChar - 64 + 9;
		}
		else if (nChar > 96 && nChar < 103)
		{
			input = nChar - 96 + 9;
		}
		else
		{
			return;
		}


		if(m_bFirst)
		{
			input = input << 4;
			cHex = cHex & 0x0f;
			cHex = cHex | input;
			m_bFirst = FALSE;
			m_text[m_SelEnd] = cHex;
		}
		else
		{
			cHex = cHex & 0xf0;
			cHex = cHex | input;
			m_text[m_SelEnd] = cHex;

			if (m_SelEnd < SECTOR_SIZE -1)	//chris:不能超扇区的SIZE
			{
				m_bFirst = TRUE;
				m_SelEnd ++;
			}
		}
	}
	else
	{
		//光标在Text区域时
		m_text[m_SelEnd] = nChar;
		if (m_SelEnd < SECTOR_SIZE -1)	//chris:不能超扇区的SIZE
		{
			m_SelEnd ++;
		}
	}


	if(m_SelEnd == m_CharCount)
	{
		m_CharCount = m_SelEnd + 1;
		m_Rows = m_CharCount/16;
	}

	m_SelStart = m_SelEnd;
	ChangeTop();
	ChangeLeft();
	Invalidate(FALSE);

	GetDocument()->SetModifiedFlag(TRUE);
}

//#define VK_PRIOR          0x21
//#define VK_NEXT           0x22


BOOL CNetDiskToolView::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch (LOWORD(pMsg->wParam))
		{
		case VK_LEFT:
			OnCharLeft();
			break;
		case VK_RIGHT:
			OnCharRight();
			break;
		case VK_UP:
			OnLineUp();
			break;
		case VK_DOWN:
			OnLineDown();
			break;
		case VK_PRIOR:
			OnShortcutOpenSector(FALSE);
			TRACE("KeyDown with VK_PRIOR\n");
//			OnPrevSector();
			break;
		case VK_NEXT:
			OnShortcutOpenSector(TRUE);
			TRACE("KeyDown with VK_NEXT\n");
//			OnNextSector();
			break;
		default:
			break;
		}
	}

	if (WM_KEYUP == pMsg->message)
	{
		switch (LOWORD(pMsg->wParam))
		{
		case VK_PRIOR:
		case VK_NEXT:
			OnUpdateSectorUI();
			break;
		default:
			break;
		}
	}
	return CView::PreTranslateMessage(pMsg);
}

//下面四个是对上下左右键的处理
void CNetDiskToolView::OnCharLeft()
{
	if(m_bFirst && m_SelEnd > 0)
	{
		m_SelEnd --;
		m_bFirst = FALSE;
	}
	else
	{
		m_bFirst = TRUE;
	}
	ChangeTop();
	ChangeLeft();
	m_SelStart = m_SelEnd;
	Invalidate(FALSE);
}

void CNetDiskToolView::OnCharRight()
{
	if(m_bFirst)
	{
		m_bFirst = FALSE;
	}
	else if( m_SelEnd < m_CharCount-1)
	{
		m_bFirst = TRUE;
		m_SelEnd++;
	}
	ChangeTop();
	ChangeLeft();
	m_SelStart = m_SelEnd;
	Invalidate(FALSE);
}

void CNetDiskToolView::OnLineUp()
{
	if(m_SelEnd > 15)
		m_SelEnd = m_SelEnd - 16;

	ChangeTop();
	ChangeLeft();
	m_SelStart = m_SelEnd;
	Invalidate(FALSE);
}

void CNetDiskToolView::OnLineDown()
{
	if(m_SelEnd + 16 < m_CharCount)
		m_SelEnd = m_SelEnd + 16;
	else
		m_SelEnd = m_CharCount-1;
	ChangeTop();
	ChangeLeft();
	m_SelStart = m_SelEnd;
	Invalidate(FALSE);
}

//计算显示的最上边字符位置
void CNetDiskToolView::ChangeTop()
{
	int newtop = m_TopLine;

	if((m_SelEnd/16 + 1) < m_TopLine)
	{
		newtop = m_SelEnd/16 + 1;
	}

	if((m_SelEnd/16 + 2) > m_TopLine+m_PageRows)
	{
		newtop = m_SelEnd/16 + 2 - m_PageRows;
	}

	if(newtop != m_TopLine)
	{
		m_TopLine = newtop;
		SetVertScrollBar();
	}
}

//计算显示的最左边字符位置
void CNetDiskToolView::ChangeLeft()
{
	int currentcol = m_SelEnd % 16;
	if(currentcol*3 + 14 < m_LeftChar)
	{
		m_LeftChar = currentcol*3 + 14;
		SetHorzScrollBar();
	}
	if(currentcol*3 + 14 - m_LeftChar+1 > m_PageCols)
	{
		m_LeftChar = currentcol*3 + 14 - m_PageCols +2;
		SetHorzScrollBar();
	}
}

void CNetDiskToolView::OnShortcutOpenSector(BOOL bNext)
{
	CNetDiskToolDoc *pDoc;
	pDoc = (CNetDiskToolDoc*)GetDocument();
	if (NULL != pDoc)
		pDoc->ShortcutOpenSector(bNext);
}

void CNetDiskToolView::OnUpdateSectorUI()
{
	CString szSector;
	CNetDiskToolDoc *pDoc;
	pDoc = (CNetDiskToolDoc*)GetDocument();
	if (NULL != pDoc)
		pDoc->UpdateSectorUI();
}


UINT CNetDiskToolView::OnGetDlgCode() 
{
	// TODO: Add your message handler code here and/or call default
    return DLGC_WANTARROWS | DLGC_WANTCHARS;
// 	return CView::OnGetDlgCode();
}

BOOL CNetDiskToolView::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if (WM_CHAR == message && VK_TAB == wParam)
		return FALSE;

	if (0 > GetAsyncKeyState(VK_CONTROL))
		return FALSE;

	if (0 > GetAsyncKeyState(VK_MENU))
		return FALSE;


	return CView::OnWndMsg(message, wParam, lParam, pResult);
}

void CNetDiskToolView::SetPositionInfo(LONG nPosition)
{
	CString szMsg;
	szMsg.Format(_T("Position:%03Xh"), nPosition);
	GetParentFrame()->SetMessageText(szMsg);
}
