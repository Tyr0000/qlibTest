// DlgGraph2D_Builder.cpp : implementation file
//

#include "stdafx.h"
#include "DlgGraph2D_Builder.h"


// CDlgGraph2D_Builder dialog

IMPLEMENT_DYNAMIC(CDlgGraph2D_Builder, CDlgData)

CDlgGraph2D_Builder::CDlgGraph2D_Builder(CWnd* pParent /*=NULL*/)
	: CDlgData(DATA_SET_TYPE::DST_2D_GRAPH, CDlgGraph2D_Builder::IDD, pParent)
{

}

CDlgGraph2D_Builder::~CDlgGraph2D_Builder()
{
}

void CDlgGraph2D_Builder::DoDataExchange(CDataExchange* pDX)
{
	CDlgData::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgGraph2D_Builder, CDlgData)
END_MESSAGE_MAP()


// CDlgGraph2D_Builder message handlers

BOOL CDlgGraph2D_Builder::OnInitDialog()
{
	CDlgData::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgGraph2D_Builder::vSaveDataDescriptor(FILE *F)
{
	CDlgData::_SaveDataDescriptor(F);
//	fwrite(&m_DataDescriptor.m_NumOfDataX, 1, 4, F);
//	fwrite(&m_DataDescriptor.m_NumOfDataY, 1, 4, F);
	m_DataDescriptor.Save(F);
}

void CDlgGraph2D_Builder::vLoadDataDescriptor(FILE *F)
{
	CDlgData::_LoadDataDescriptor(F);
	m_DataDescriptor.Load(F);
}

void CDlgGraph2D_Builder::vSaveData(FILE *F)
{
}

void CDlgGraph2D_Builder::vLoadData(FILE *F)
{
}

void CDlgGraph2D_Builder::vDsplDataDescriptor()
{
	TCHAR tStr[32];

	CDlgData::_DsplDataDescriptor();
	
	_ltot(m_DataDescriptor.m_NumOfDataX , tStr, 10);
	SetDlgItemText(IDC_EDIT_NUM_OF_X, tStr);
	_ltot(m_DataDescriptor.m_NumOfDataY , tStr, 10);
	SetDlgItemText(IDC_EDIT_NUM_OF_Y, tStr);
}


