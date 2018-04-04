#pragma once
#include "afxcmn.h"
#include <vector>
#include "resource.h"
#include "DlgData.h"
#include "FListCtrl.h"

typedef struct _PARAM_DATA
{
	MENU_STR Name;
	int Type;
	int Offset;
	int Multiplier;
	int Addition;
	int MinValue;
	int MaxValue;
}
PARAM_DATA;

// CDlgParamsBuilder dialog

class CDlgParamsBuilder : public CDlgData
{
	DECLARE_DYNAMIC(CDlgParamsBuilder)

public:
	CDlgParamsBuilder(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgParamsBuilder();

// Dialog Data
	enum { IDD = IDD_DLG_PARAMS_BUILDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	std::vector<PARAM_DATA> ListParams;
	void SaveDataDescriptor(FILE *F);
	void LoadDataDescriptor(FILE *F);
	void vSaveDataDescriptor(FILE *F);
	void vLoadDataDescriptor(FILE *F);
//	void SaveData(FILE *F);
//	void LoadData(FILE *F);
	void FillParamList();
	
	afx_msg void OnComboboxExpand(NMHDR * pNotifyStruct, LRESULT * result);

	CFListCtrl m_ListParameters;
	afx_msg void OnBnClickedButtonMoveDown();
	afx_msg void OnBnClickedButtonMoveUp();
	afx_msg void OnBnClickedButtonDelField2();
	afx_msg void OnBnClickedButtonAddField();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonDelField();
};
