// DlgMenu.cpp : implementation file
//

#include "stdafx.h"
#include "DlgData.h"
#include "u_File.h"

#pragma warning (disable: 4996)

// CDlgData dialog

IMPLEMENT_DYNAMIC(CDlgData, CDialog)

CDlgData::CDlgData(DATA_SET_TYPE DataType, UINT ResId, CWnd* pParent /*=NULL*/)
	: CDialog(ResId, pParent)
{
	m_DataType= DataType;
/*
	m_DataPtrLen= DPL_UNDEFINED;
	m_DataOffset= -1;
	m_MinVal= 0;
	m_MaxVal= 0;
	m_DataLen= 0;
	m_pData= NULL;
	m_pData32= NULL;
*/	
}

CDlgData::~CDlgData()
{
//	if (m_pData != NULL)
//		free(m_pData);
}

void CDlgData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgData, CDialog)
END_MESSAGE_MAP() 


// CDlgData message handlers

BOOL CDlgData::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	TCHAR tStr[32];
//	_ltot(m_DataOffset, tStr, 10);
//	SetDlgItemText(IDC_EDIT_DATA_OFFSET, tStr);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgData::_SaveDataDescriptor(FILE *F)
{
	_UpdateDataDescriptorFromGUI();
	
	fwrite(&m_DataType, 1, 1, F);
	fwrite(&m_DataPtrLen, 1, 1, F);

//	fwrite(&m_DataOffset, 1, 4, F);
	fwrite(&m_MinVal, 1, 4, F);
	fwrite(&m_MaxVal, 1, 4, F);
//	fwrite(&m_DataLen, 1, 4, F);
	
}

void CDlgData::_LoadDataDescriptor(FILE *F)
{
	fread(&m_DataType, 1, 1, F);
	fread(&m_DataPtrLen, 1, 1, F);

//	fread(&m_DataOffset, 1, 4, F);
	fread(&m_MinVal, 1, 4, F);
	fread(&m_MaxVal, 1, 4, F);
//	fread(&m_DataLen, 1, 4, F);
}

void CDlgData::vSaveDataDescriptor(FILE *F)
{
}

void CDlgData::vLoadDataDescriptor(FILE *F)
{
}

void CDlgData::vSaveData(FILE *F)
{
//	fwrite(m_pData, 1, m_DataLen, F);
}

void CDlgData::vLoadData(FILE *F)
{
//	fread(m_pData, 1, m_DataLen, F);
}
/*
void CDlgData::SaveData(FILE *F)
{
}

void CDlgData::LoadData(FILE *F)
{
}
*/
bool CDlgData::LoadDataFromExtFile()
{
	TCHAR tStr[32];
	TCHAR *pFN;
	FILE *F;
	unsigned int FileOffset;
/*
	GetDlgItemText(IDC_EDIT_DATA_OFFSET_IN_FILE, tStr, 32);
	FileOffset= _tstoi(tStr);

	pFN= SelectFileToOpen(m_hWnd, _T("Select a data file"), _T("data files\0*.*\0\0"));
	if (pFN == NULL)
		return false;
#ifdef _UNICODE	
	F= _wfopen(pFN, L"rb");
#else
	F= fopen(pFN, "rb");
#endif	
	if (F == NULL)
		return false;

	if (m_pData != NULL)
		free(m_pData);

	m_pData= (unsigned __int8 *)malloc(m_DataLen);
	if (m_pData == NULL)
		return false;

	if (fseek(F, FileOffset, SEEK_SET) != 0)
	{
		fclose(F);
		 return false;
	}
	if (fread(m_pData, 1, m_DataLen, F) != m_DataLen)
		{
		fclose(F);
		 return false;
	}
	fclose(F);
*/	
	return true;
}

bool CDlgData::LoadDataFromExtFile(FILE *F, unsigned int FileOffset)
{		
/*
	if (m_pData != NULL)
		free(m_pData);
	m_pData= (unsigned __int8 *)malloc(m_DataLen);
	if (m_pData == NULL)
		return false;
	if (fseek(F, FileOffset, SEEK_SET) != 0) return false;
	if (fread(m_pData, 1, m_DataLen, F) != m_DataLen) return false;
*/
	return true;
}

bool CDlgData::_UpdateDataDescriptorFromGUI()
{
	FILE *F;
	int CntCol;
	TCHAR tStr[32];
		
	if (m_DataType == DATA_SET_TYPE::DST_PARAMS) return true;

	m_DataPtrLen= (DATA_PTR_LEN)((CComboBox *)GetDlgItem(IDC_COMBO_DATA_LEN))->GetCurSel();
	if (m_DataPtrLen <= DPL_UNDEFINED)
	{
		::MessageBox(m_hWnd, _T("Select a data pointer length"), _T("User error"), MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
// 0 -> 1
// 1 -> 2
// 2 -> 4	
	m_DataPtrLen= (DATA_PTR_LEN)(1 << m_DataPtrLen); 
	
//	GetDlgItemText(IDC_EDIT_DATA_OFFSET, tStr, 32);
//	m_DataOffset= _tstoi(tStr);
	GetDlgItemText(IDC_EDIT_MIN_VAL, tStr, 32);
	m_MinVal= _tstoi(tStr);
	GetDlgItemText(IDC_EDIT_MAX_VAL, tStr, 32);
	m_MaxVal= _tstoi(tStr);;	
	return true;
}

void CDlgData::_DsplDataDescriptor()
{
	TCHAR tStr[32];
		
	if (m_DataType == DATA_SET_TYPE::DST_PARAMS) return;

//  1 -> 0
//  2 -> 1
//  4 -> 2
	((CComboBox *)GetDlgItem(IDC_COMBO_DATA_LEN))->SetCurSel(m_DataPtrLen >> 1);
	
//	_ltot(m_DataOffset, tStr, 10);
//	SetDlgItemText(IDC_EDIT_DATA_OFFSET, tStr);
	_ltot(m_MinVal, tStr, 10);
	SetDlgItemText(IDC_EDIT_MIN_VAL, tStr);
	_ltot(m_MaxVal, tStr, 10);	
	SetDlgItemText(IDC_EDIT_MAX_VAL, tStr);
}

void CDlgData::vDsplDataDescriptor()
{
	_DsplDataDescriptor();
}

BOOL CDlgData::PreTranslateMessage(MSG* pMsg)
{
    if(WM_KEYDOWN == pMsg->message)
    {
//			if (m_ReadOnly)
//        return TRUE;        

        if(VK_RETURN == pMsg->wParam)
        {
					pMsg->wParam= 0;
					pMsg->lParam= 0;
					return 1;
        } // if

        if(VK_ESCAPE == pMsg->wParam)
        {
					pMsg->wParam= 0;
					pMsg->lParam= 0;
 //           EndEdit();
					return 1;
        } // if
    } // if
    
    if (pMsg->message == WM_COMMAND)
    {
			MessageBeep(0);
    }

	return CDialog::PreTranslateMessage(pMsg);
}

