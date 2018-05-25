#include "DlgData.h"

#pragma once


// CDlgTblBuilder dialog

class CDlgTblBuilder : public CDlgData
{
	DECLARE_DYNAMIC(CDlgTblBuilder)

public:
	CDlgTblBuilder(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTblBuilder();

// Dialog Data
//	enum { IDD = IDD_DLG_TBL_BUILDER };
	enum { IDD = IDD_DLG_DATA_2D};

protected:
//	DATA_LENGTH_TYPE m_DataLengthType;
//	unsigned __int32 m_NumOfRows;
//	unsigned __int32 m_NumOfCols;
//	unsigned __int32 *m_pData32; 
	DATA_2D_DESCRIPTOR m_DataDescriptor;
	CListCtrl m_TblData;
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	char m_Title[MAX_CAPTION_LEN];
	char m_AxisX[MAX_CAPTION_LEN];
	char m_AxisY[MAX_CAPTION_LEN];
	

	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnCbnSelchangeComboDataLen();
	virtual BOOL OnInitDialog();

	virtual void vSaveDataDescriptor(FILE *F);
	virtual void vLoadDataDescriptor(FILE *F);
	void vSaveData(FILE *F);
	void vLoadData(FILE *F, bool LoadInfo= true);
	void FillTbl();
};
