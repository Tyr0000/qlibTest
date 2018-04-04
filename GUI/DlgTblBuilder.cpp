// DlgTblBuilder.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTblBuilder.h"
#include "u_File.h"

#pragma warning (disable: 4996)

// CDlgTblBuilder dialog

//#define DATA_ITEM(Row, Col)	 *(m_pData32+ Row* m_NumOfCols+ Col)
#define DATA_ITEM_8(Row, Col)	 *(unsigned __int8 *)(m_pData+ (Row* m_NumOfCols+ Col)*m_DataPtrLen)
#define DATA_ITEM_16(Row, Col) *(unsigned __int16 *)(m_pData+ (Row* m_NumOfCols+ Col)*m_DataPtrLen)
#define DATA_ITEM_32(Row, Col) *(unsigned __int32 *)(m_pData+ (Row* m_NumOfCols+ Col)*m_DataPtrLen)

IMPLEMENT_DYNAMIC(CDlgTblBuilder, CDlgData)

CDlgTblBuilder::CDlgTblBuilder(CWnd* pParent /*=NULL*/)
	: CDlgData(DATA_SET_TYPE::DST_2D_TBL, CDlgTblBuilder::IDD, pParent)
{
//	m_NumOfRows= 0;
//	m_NumOfCols= 0;
//  m_pData32= NULL;
}

CDlgTblBuilder::~CDlgTblBuilder()
{
//	if (m_pData32 != NULL)
//		delete [] m_pData32;
}

void CDlgTblBuilder::DoDataExchange(CDataExchange* pDX)
{
	CDlgData::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_TblData);
}


BEGIN_MESSAGE_MAP(CDlgTblBuilder, CDlgData)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CDlgTblBuilder::OnBnClickedButtonLoad)
	ON_CBN_SELCHANGE(IDC_COMBO_DATA_LEN, &CDlgTblBuilder::OnCbnSelchangeComboDataLen)
END_MESSAGE_MAP()


// CDlgTblBuilder message handlers

BOOL CDlgTblBuilder::OnInitDialog()
{
	CDlgData::OnInitDialog();

	// TODO:  Add extra initialization here

//	m_DataLengthType= DLT_32;
//	((CComboBox *)GetDlgItem(IDC_COMBO_DATA_LEN))->SetCurSel(m_DataLengthType);
//	m_TblData.InsertColumn(0, "X", LVCFMT_CENTER, 100);
//	m_TblData.InsertColumn(1, "Y", LVCFMT_CENTER, 100);
	
	FillTbl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTblBuilder::OnBnClickedButtonLoad()
{
	char *pFN, *pShortFN;
	TCHAR StrVal[128]; 
	FILE *F;
	int CntCol;
	TCHAR tStr[32];
		
	if (!_UpdateDataDescriptorFromGUI())
		return;
		
	m_DataDescriptor.UpdateDataDescriptorFromGUI(m_hWnd);
/*		
	GetDlgItemText(IDC_EDIT_NUM_OF_COLS, tStr, 32);
	m_DataDescriptor.m_NumOfDataX= _tstoi(tStr);
	GetDlgItemText(IDC_EDIT_NUM_OF_ROWS, tStr, 32);
	m_DataDescriptor.m_NumOfDataY= _tstoi(tStr);
*/	
/*
	if ((m_NumOfRows <= 0) || (m_NumOfCols <= 0))
	{
		m_NumOfRows= 0; 
		m_NumOfCols= 0;
		::MessageBox(m_hWnd, _T("Wrong nunber of rows/columns"), _T("User error"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	m_DataLen= (m_NumOfRows* m_NumOfCols)* m_DataPtrLen;

	pFN= SelectFileToOpen(m_hWnd, _T("Select a table data file"), _T("data files\0*.*\0\0"), &pShortFN);
	if (pFN == NULL)
		return;
#ifdef _UNICODE	
	F= _wfopen(pFN, L"rb");
#else
	F= fopen(pFN, "rb");
#endif	
	if (F == NULL)
		return;
	m_TblData.DeleteAllItems();
	for (CntCol= 0; CntCol < m_NumOfCols; CntCol++)
		m_TblData.DeleteColumn(0);
	if (m_pData32 != NULL)
	{
		delete [] m_pData32;
		m_pData32= NULL;
	}
//	Load(F);
	fclose(F);
	
	FillTbl();
*/	
}

void CDlgTblBuilder::OnCbnSelchangeComboDataLen()
{
//	m_DataLengthType= (DATA_LENGTH_TYPE)(((CComboBox *)GetDlgItem(IDC_COMBO_DATA_LEN))->GetCurSel());
}

void CDlgTblBuilder::vSaveData(FILE *F)
{
	int CntRow, CntCol;
	DWORD BytesNum;
	unsigned __int32 Data32;
//	TCHAR StrVal[128]; 
/*
	switch (m_DataLengthType)
	{
		case DLT_8:  ptrLen= 1; pData= &Data8;  break;
		case DLT_16: ptrLen= 2; pData= &Data16; break;
		case DLT_32: ptrLen= 4; pData= &Data32; break;
	}
*/
//	fwrite(&m_DataPtrLen, 1, 4, F);
//	fwrite(&m_NumOfRows, 1, 4, F);
//	fwrite(&m_NumOfCols, 1, 4, F);
	
	m_DataDescriptor.Save(F);
/*	
	for (CntRow= 0; CntRow < m_NumOfRows; CntRow++)
	{
		for (CntCol= 0; CntCol < m_NumOfCols; CntCol++)
		{
//			Data32= DATA_ITEM(CntRow, CntCol);

			switch (m_DataPtrLen)
			{
				case DLT_8:
				break;
				case DLT_16:
				break;
				case DLT_32:
				break;
			}

			fwrite(&Data32, 1, m_DataPtrLen, F);
		}
	}
*/	
}

void CDlgTblBuilder::vLoadData(FILE *F, bool LoadInfo)
{
	int CntRow, CntCol;
	unsigned __int32 Data32;
	
/*
	fread(&m_DataLengthType, 1, 4, F);
	switch (m_DataLengthType)
	{
		case DLT_8:  ptrLen= 1; pData= &Data8;  break;
		case DLT_16: ptrLen= 2; pData= &Data16; break;
		case DLT_32: ptrLen= 4; pData= &Data32; break;
	}
	::GetDlgItemText(m_hWnd, IDC_EDIT_NUM_OF_ROWS, StrVal, 128);
	m_NumOfRows= atoi(StrVal);
	::GetDlgItemText(m_hWnd, IDC_EDIT_NUM_OF_COLS, StrVal, 128);
	m_NumOfCols= atoi(StrVal);
*/
	m_DataDescriptor.Load(F);
/*	
	if (LoadInfo)
	{
		fread(&m_DataPtrLen, 1, 4, F);
		fread(&m_NumOfRows, 1, 4, F);
		fread(&m_NumOfCols, 1, 4, F);
	}	
	if ((m_NumOfRows <= 0) || (m_NumOfCols <= 0))
	{
		m_NumOfRows= 0; 
		m_NumOfCols= 0;
		return;
	}
	
	m_pData32= new unsigned __int32[m_NumOfRows* m_NumOfCols];

	for (CntRow= 0; CntRow < m_NumOfRows; CntRow++)
	{
		for (CntCol= 0; CntCol < m_NumOfCols; CntCol++)
		{
			Data32= 0;
			fread(&Data32, 1, m_DataPtrLen, F);
//		DATA_ITEM(CntRow, CntCol)= Data32;
		}
	}
*/	
}

void CDlgTblBuilder::vSaveDataDescriptor(FILE *F)
{
	CDlgData::_SaveDataDescriptor(F);
	m_DataDescriptor.Save(F);
}

void CDlgTblBuilder::vLoadDataDescriptor(FILE *F)
{
	CDlgData::_LoadDataDescriptor(F);
	m_DataDescriptor.Load(F);
}

void CDlgTblBuilder::FillTbl()
{
	int CntRow, CntCol;
	TCHAR StrVal[128]; 
/*
	for (CntCol= 0; CntCol < m_NumOfCols; CntCol++)
		m_TblData.InsertColumn(CntCol, "", LVCFMT_CENTER, 100);

	for (CntRow= 0; CntRow < m_NumOfRows; CntRow++)
	{
		m_TblData.InsertItem(CntRow, "");
		for (CntCol= 0; CntCol < m_NumOfCols; CntCol++)
		{
//		  _stprintf(StrVal, _T("%i"), DATA_ITEM(CntRow, CntCol));	
		  m_TblData.SetItemText(CntRow, CntCol, StrVal);
		}
	}
	
	itoa(m_NumOfRows, StrVal, 10);
	::SetDlgItemText(m_hWnd, IDC_EDIT_NUM_OF_ROWS, StrVal);
	itoa(m_NumOfCols, StrVal, 10);
	::SetDlgItemText(m_hWnd, IDC_EDIT_NUM_OF_COLS, StrVal);
	
	switch (m_DataPtrLen)
	{
		case 1: ((CComboBox *)GetDlgItem(IDC_COMBO_DATA_LEN))->SetCurSel(0);
		break;
		case 2: ((CComboBox *)GetDlgItem(IDC_COMBO_DATA_LEN))->SetCurSel(1);
		break;
		case 4: ((CComboBox *)GetDlgItem(IDC_COMBO_DATA_LEN))->SetCurSel(2);
		break;
	}
*/	
}