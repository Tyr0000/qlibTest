// DymanicMenuDlg.h : header file
//

#pragma once
//#include "afxcmn.h"
//#include "DlgData.h"
#include "TreeMenu.h"


// CDlgMain dialog
class CDlgMain : public CDialog
{
// Construction
public:
	CDlgMain(CWnd* pParent = NULL);	// standard constructor
	~CDlgMain();
// Dialog Data
	enum { IDD = IDD_DLG_MAIN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTreeMenu m_TreeMenu;
	CTreeMenu m_TstMenu;

	void SetRadioBttnState(DATA_SET_TYPE DlgType);
	void CreateBuildDlg(DATA_SET_TYPE DlgType);
	void SetActBuildDlg(DATA_SET_TYPE DlgType);
//	void SaveItemData(FILE *F, MENU_ITEM_DATA *pItemData, HTREEITEM hItem, int ItemId, int ParentId= -1);
	void ClearMenu();
//	void Load(FILE *F);
	void OnActDlgChanged(DATA_SET_TYPE DlgType);
	
	afx_msg void OnBnClickedButtonAddMenuItem();
	afx_msg void OnBnClickedButtonDelMenuItem();
	afx_msg void OnBnClickedButtonDelAllMenuItems();
	afx_msg void OnBnClickedButtonAddMenuRootItem();
	afx_msg void OnBnClickedRadioParameterDlg();
	afx_msg void OnBnClickedRadioTbl();
	afx_msg void OnBnClickedRadioGraph();
	afx_msg void OnBnClickedRadio2dGraph();
	afx_msg void OnBnClickedRadio3dGraph();
	afx_msg void OnTvnSelchangedTreeMenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLoadMenu();
	afx_msg void OnBnClickedButtonSaveItems();
	virtual BOOL DestroyWindow();
//	afx_msg void OnBnClickedButtonTst();
	afx_msg void OnNMClickTreeMenu2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTreeMenu2(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonSaveData();
	afx_msg void OnBnClickedCheckSaveItems();
	afx_msg void OnBnClickedCheckSaveData();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoadData();
	afx_msg void OnBnClickedButtonLoadExtMenu();
	afx_msg void OnBnClickedButtonImportItem();
	afx_msg void OnBnClickedButtonImportItemData();
	afx_msg void OnBnClickedButtonExportItem();
	afx_msg void OnBnClickedButtonImportItemData2();
	afx_msg void OnBnClickedButtonImportMenu();
};
