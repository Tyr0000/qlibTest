#include <vector>
#include "DlgData.h"

#pragma once

#define WM_ACT_DLG			WM_USER+ 1
#define WM_SHOW_DLG			WM_USER+ 2
#define WM_HIDE_DLG			WM_USER+ 3

#define MENU_ITEM_MAX_LEN			50

typedef void (*PF_ON_ACT_DLG_CHANGED)(DATA_SET_TYPE);

typedef enum
{
	MII_ROOT,
	MII_PARAMS,
	MII_TBL,
	MII_1D,
	MII_2D,
	MII_3D
}
MENU_ITEM_IMAGE;

typedef enum
{
	MT_BUILDER,
	MT_WORK
}
MENU_TYPE;

#define DECL_MENU_ITEM_DATA \
	int ItemId;\
	int ParentId;\
	DATA_SET_TYPE ItemType;\
	TCHAR ItemName[MENU_ITEM_MAX_LEN];

typedef struct _MENU_ITEM_CNTRLS
{
	HTREEITEM hItem;
	CDlgData *pDlg;
}
MENU_ITEM_CNTRLS;

typedef struct _MENU_ITEM_DATA
{
	DECL_MENU_ITEM_DATA
	HTREEITEM hItem;
	CDlgData *pDlg;
}
MENU_ITEM_DATA;


// CTreeMenu

class CTreeMenu : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeMenu)

public:
	CTreeMenu();
	virtual ~CTreeMenu();

protected:
	DECLARE_MESSAGE_MAP()
	
private:
	std::vector<MENU_ITEM_DATA> m_ItemList;
// std::vector<HTREEITEM> m_ItemList;
	CDlgData *m_pActDlg;
	
	HICON m_hIconRoot;
	HICON m_hIconParams;
	HICON m_hIconTbl;
	HICON m_hIcon2D;
	HICON m_hIcon3D;
	CImageList m_ImgList;

	int ArrangeItemBranch(HTREEITEM hItemToArrange, bool DeleteData= false);
	void ArrangeAllItems(bool DeleteData= false);
	void ActivateDlg();
	CDlgData *CreateDlg(DATA_SET_TYPE DlgType); //, bool ShowDlg= true);
	void WriteMenuKey(FILE *F);
	bool CheckMenuKey(FILE *F);
public:
	HWND m_hWndMenuName;
	CWnd *m_pDlgPlaceHolder;
	MENU_TYPE m_MenuType;
	HTREEITEM m_hSelItem;
	PF_ON_ACT_DLG_CHANGED OnActDlgChanged;
//	void ClearData();
	void AttachImageList();
	void DeleteItem(HTREEITEM hItemToDelete, BOOL DeleteAll);
	void Clear(BOOL DeleteAll= TRUE);	
	int AddItem(TCHAR *ItemName, HTREEITEM hItemParent);
	void SaveItemBranch(FILE *ItemF, FILE *DataF= NULL);
	void Save(FILE *ItemF, FILE *DataF= NULL);
	void _Load(FILE *ItemF, FILE *DataF);
	void LoadSubItems(char *FN);
	void LoadItems(char *FN, char *pMenuTitle);
	void LoadData(char *FN);
	virtual BOOL DestroyWindow();
	void ShowDlg();
	void HideDlg();
	void ChangeActDlgType(DATA_SET_TYPE DlgType);
//	afx_msg void OnNMReturn(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnTvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
//	afx_msg void OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


