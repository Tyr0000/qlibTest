// LabelEdit.cpp : implementation file
//

#include "stdafx.h"
#include "LabelEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLabelEdit

#define BK_COLOR_ERR	RGB(255, 255, 0)

CLabelEdit::CLabelEdit()
{
	m_IsNumeric= true;
	m_ReadOnly= FALSE;
	m_IsValidVal= TRUE;
	MinVal= 0;
	MaxVal= 0;
	Val= 0;
	
	m_hBr= ::CreateSolidBrush(BK_COLOR_ERR);
}

CLabelEdit::~CLabelEdit()
{
	DeleteObject(m_hBr);
}


BEGIN_MESSAGE_MAP(CLabelEdit, CEdit)
	//{{AFX_MSG_MAP(CLabelEdit)
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &CLabelEdit::OnEnKillfocus)
	ON_WM_CTLCOLOR_REFLECT()
//	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_CHANGE, &CLabelEdit::OnEnChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLabelEdit message handlers


void CLabelEdit::BindToCmmnd(BOARD_CMMND *pCmmnd)
{
  m_pCmmnd= pCmmnd;
	if (pCmmnd->CmmndCodeAdd == 0xFF)
		m_ReadOnly= TRUE;
}

#define dx					10

BOOL CLabelEdit::Create(RECT Rect, CWnd *Parent, UINT Id)
{
	SIZE TxtSize;
	RECT TxtRect;
//	int dx;

	::GetTextExtentPoint32(::GetWindowDC(Parent->m_hWnd), m_pCmmnd->Title, strlen(m_pCmmnd->Title), &TxtSize);
/*
	TxtRect.left= Rect.left;
	TxtRect.top= Rect.top;
  TxtRect.right= TxtRect.left+ TxtSize.cx;
	TxtRect.bottom= TxtRect.top+ TxtSize.cy;
*/	
 	TxtRect.left= Rect.left- dx- TxtSize.cx;
  TxtRect.right= Rect.left- dx;
	TxtRect.bottom= Rect.bottom;
	TxtRect.top= TxtRect.bottom- TxtSize.cy;;
	m_Title.Create(m_pCmmnd->Title, ES_LEFT | WS_CHILD | WS_VISIBLE, TxtRect, Parent);  
/*
  dx= TxtRect.right+ 5;
  Rect.left= Rect.left+ dx;
	Rect.right= Rect.right+ dx;
	::GetWindowRect(m_Title.m_hWnd, &TxtRect);
	m_Right= TxtRect.right;
*/
  m_Right= Rect.right;

	return CEdit::Create(ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | WS_EX_CLIENTEDGE, 
		                   Rect, Parent, Id);

}

BOOL CLabelEdit::Create(TCHAR *pTitle, RECT Rect, CWnd *Parent, UINT Id, BOOL CalcOnly)
{
	SIZE TxtSize;
	RECT TxtRect;
//	int dx;
	HDC hDc;
	
	hDc= ::GetWindowDC(Parent->m_hWnd);
	::GetTextExtentPoint32(hDc, pTitle, strlen(pTitle), &TxtSize);
	::ReleaseDC(Parent->m_hWnd, hDc);
/*
	TxtRect.left= Rect.left;
	TxtRect.top= Rect.top;
  TxtRect.right= TxtRect.left+ TxtSize.cx;
	TxtRect.bottom= TxtRect.top+ TxtSize.cy;
*/	
 	TxtRect.left= Rect.left- dx- TxtSize.cx;
  TxtRect.right= Rect.left- dx;
	TxtRect.bottom= Rect.bottom;
	TxtRect.top= TxtRect.bottom- TxtSize.cy;
	if (!CalcOnly)
		m_Title.Create(pTitle, ES_LEFT | WS_CHILD | WS_VISIBLE, TxtRect, Parent);  
/*
  dx= TxtRect.right+ 5;
  Rect.left= Rect.left+ dx;
	Rect.right= Rect.right+ dx;
	::GetWindowRect(m_Title.m_hWnd, &TxtRect);
	m_Right= TxtRect.right;
*/
  m_Right= Rect.right;
  m_XX= Rect.right- TxtRect.left;

	if (CalcOnly) return NULL;
	
	return CEdit::Create(ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | WS_EX_CLIENTEDGE, 
		                   Rect, Parent, Id);
}


/*
int CLabelEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CLabelEdit::SetTitle(char *pTitle)
{

}
*/

void CLabelEdit::MoveHor(int Shift)
{
	RECT ParentRect, TxtRect, EditRect;

	::GetWindowRect(GetParent()->m_hWnd, &ParentRect);
	::GetWindowRect(m_Title.m_hWnd, &TxtRect);
	::GetWindowRect(m_hWnd, &EditRect);


		::MoveWindow(m_Title.m_hWnd, 
		             TxtRect.left- ParentRect.left+ dx,  TxtRect.top- ParentRect.top,
		             TxtRect.right- TxtRect.left,  TxtRect.bottom- TxtRect.top,
                 FALSE);
		::MoveWindow(m_hWnd, 
		             EditRect.left- ParentRect.left+ dx,  EditRect.top- ParentRect.top,
		             EditRect.right- EditRect.left,  EditRect.bottom- EditRect.top,
                 FALSE);
//	::SetWindowPos(m_Title.m_hWnd, 0, 
//		             TxtRect.left- ParentRect.left+ dx,  TxtRect.top- ParentRect.top, 0, 0,
//                 SWP_NOSIZE | SWP_NOZORDER);
}

BOOL CLabelEdit::PreTranslateMessage(MSG* pMsg) 
{
    if (WM_KEYDOWN == pMsg->message)
    {
			if (m_ReadOnly)
        return TRUE;        

        if (VK_RETURN == pMsg->wParam)
        {
					::MessageBox(0, "jhk", "jh", MB_OK);
//					EndEdit(true);
            return TRUE;        
        } // if

        if (VK_ESCAPE == pMsg->wParam)
        {
 //           EndEdit();
            return TRUE;
        } // if
    } // if
    if (pMsg->message == WM_CHAR)
    {		
				
    		if (m_IsNumeric &&
    			  (((pMsg->wParam < '0') || (pMsg->wParam > '9')) && (pMsg->wParam != '.') && (pMsg->wParam != '-') && (pMsg->wParam != 8)))
				{
					pMsg->wParam= 0;
					return TRUE;
				}
    }
	return CEdit::PreTranslateMessage(pMsg);
}

BOOL CLabelEdit::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Title.DestroyWindow();
	return CEdit::DestroyWindow();
}

void CLabelEdit::CheckVal()
{
	TCHAR StrVal[32];
	
	GetWindowText(StrVal, 32);
	Val= _tstol(StrVal);	
	if ((MinVal != 0) != 0 && (MaxVal != 0))
		if ((Val < MinVal) || (Val > MaxVal))
			m_IsValidVal= FALSE;
		else
			m_IsValidVal= TRUE;
}

void CLabelEdit::OnEnKillfocus()
{
//	CheckVal();
}

HBRUSH CLabelEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  Change any attributes of the DC here
	
	if (!m_IsValidVal)
	{
		::SetBkColor(pDC->m_hDC, BK_COLOR_ERR);
		return m_hBr;
	}
	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

void CLabelEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL IsValidValue= TRUE;
//	int Val;
	TCHAR tStr[32];
	
	if (m_IsNumeric)
	{
		if (((nChar < '0') || (nChar > '9')) && (nChar != '.') && (nChar != '-') && (nChar != 8))
		{
//			IsValidValue= FALSE; 
	CEdit::OnChar(0, nRepCnt, nFlags);
	return;
		}
	}
L_RET:		
/*
	if (m_IsValidVal != IsValidValue)
	{
		m_IsValidVal= IsValidValue;
		UpdateWindow();
	}
*/
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CLabelEdit::OnEnChange()
{
	BOOL IsValidValue= TRUE;
//	int Val;
	TCHAR tStr[32];

	if ((MinVal == 0) && (MaxVal == 0))
		return;
	::GetWindowText(m_hWnd, tStr, 32);
	Val= _tstol(tStr);
	if ((Val < MinVal) || (Val > MaxVal))
		IsValidValue= FALSE;

	if (m_IsValidVal != IsValidValue)
	{
		m_IsValidVal= IsValidValue;
		RedrawWindow();
	}
}
