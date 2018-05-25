// DlgLoadData.cpp : implementation file
//

#include "stdafx.h"
#include "DlgLoadData.h"


// CDlgLoadData dialog

IMPLEMENT_DYNAMIC(CDlgLoadData, CDialog)

CDlgLoadData::CDlgLoadData(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLoadData::IDD, pParent)
{

}

CDlgLoadData::~CDlgLoadData()
{
}

void CDlgLoadData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLoadData, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CDlgLoadData::OnBnClickedButtonLoad)
END_MESSAGE_MAP()


// CDlgLoadData message handlers

void CDlgLoadData::OnBnClickedButtonLoad()
{

}

BOOL CDlgLoadData::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CComboBox *)GetDlgItem(IDC_COMBO_DATA_LEN))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
