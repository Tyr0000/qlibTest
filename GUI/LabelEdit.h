#if !defined(AFX_LABELEDIT_H__48E83503_26E3_402C_BDBB_F6DEF9ED4D80__INCLUDED_)
#define AFX_LABELEDIT_H__48E83503_26E3_402C_BDBB_F6DEF9ED4D80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LabelEdit.h : header file
//
#include <tchar.h>
#include "Commands.h"

typedef union _VAL_TYPE
{
	unsigned __int8 uVal8;
	unsigned __int16 uVal16;
	unsigned __int32 uVal32;
}
VAL_TYPE;


/////////////////////////////////////////////////////////////////////////////
// CLabelEdit window

class CLabelEdit: public CEdit
{
// Construction
public:
	CLabelEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabelEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_IsNumeric;
	void MoveHor(int dx);
	int m_Right;
	int m_XX;
	unsigned __int32 MinVal;
	unsigned __int32 MaxVal;
	unsigned __int32 Val;
//	int m_Bottom;
//	void SetTitle(char *pTitle);
	BOOL Create(RECT Rect, CWnd *Parent, UINT Id);
	BOOL Create(TCHAR *pTitle, RECT Rect, CWnd *Parent, UINT Id, BOOL CalcOnly= false);
	BOARD_CMMND *m_pCmmnd;
	void BindToCmmnd(BOARD_CMMND *pCmmnd);
	CStatic m_Title;
//	CEdit   m_Edit;
//	CComboBox m_ComboBox;
	virtual ~CLabelEdit();

	// Generated message map functions
protected:
	HBRUSH m_hBr;
	BOOL m_ReadOnly;
	BOOL m_IsValidVal;
	void CheckVal();
	//{{AFX_MSG(CLabelEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocus();
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnChange();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LABELEDIT_H__48E83503_26E3_402C_BDBB_F6DEF9ED4D80__INCLUDED_)
