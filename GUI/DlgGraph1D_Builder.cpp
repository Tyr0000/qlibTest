// DlgGraph1D_Builder.cpp : implementation file
//

#include "stdafx.h"
#include "DlgGraph1D_Builder.h"


// CDlgGraph1D_Builder dialog

IMPLEMENT_DYNAMIC(CDlgGraph1D_Builder, CDlgGraph1D_Descriptor)

CDlgGraph1D_Builder::CDlgGraph1D_Builder(CWnd* pParent /*=NULL*/)
	: CDlgGraph1D_Descriptor(CDlgGraph1D_Builder::IDD, pParent)
{

}

CDlgGraph1D_Builder::~CDlgGraph1D_Builder()
{
}

void CDlgGraph1D_Builder::DoDataExchange(CDataExchange* pDX)
{
	CDlgGraph1D_Descriptor::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgGraph1D_Builder, CDlgGraph1D_Descriptor)
END_MESSAGE_MAP()


// CDlgGraph1D_Builder message handlers
BOOL CDlgGraph1D_Builder::OnInitDialog()
{
	CDlgGraph1D_Descriptor::OnInitDialog();

	// TODO:  Add extra initialization here
	HFROW hFRow;
	int i, l;  
	m_ListPointNames.SetExtendedStyle(LVS_EX_GRIDLINES);  
	int iControlEdit= m_ListPointNames.AddControl(FL_EDIT);						//0
	m_ListPointNames.InsertFColumn(0, "Point Name", 250, FL_LEFT, iControlEdit);
	for (i= 0; i < 100; i++)
	{
		hFRow= m_ListPointNames.InsertFRow(FL_ROOT, FL_LAST, "");
		m_ListPointNames.SetColor(FL_FROW, hFRow, -1, FL_DONTSET, RGB(255, 255, 255));
	}
	vDsplDataDescriptor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGraph1D_Builder::vSaveDataDescriptor(FILE *F)
{
	unsigned int i, l;
	POINT_NAME PointName;
	HFROW hFRow;
	const TCHAR *pItemName;
	
	CDlgGraph1D_Descriptor::_SaveDataDescriptor(F);

	m_PointNamesList.clear();

	for (i= 0; i < 100; i++)
	{
		pItemName= m_ListPointNames.GetFItemText(m_ListPointNames.GetRowByInd(i), 0).GetString(); 
		if ((pItemName == NULL) || (pItemName == "") || (pItemName[0] == 0))
			continue;
		_tcscpy(PointName.tStr, pItemName);
		PointName.StrLen= _tcslen(pItemName);	
		m_PointNamesList.push_back(PointName);	
	}
	fwrite(&m_NumOfPoints, 1, 4, F);
	for (i= 0; i < m_NumOfPoints; i++)
	{
		PointName= m_PointNamesList[i];
		fwrite(&PointName, 1, ((PointName.StrLen+ 1)* sizeof(TCHAR)+ 2), F);
	}
}

void CDlgGraph1D_Builder::vLoadDataDescriptor(FILE *F)
{
	unsigned int i, l;
	POINT_NAME PointName;
	HFROW hFRow;

	m_PointNamesList.clear();

	CDlgGraph1D_Descriptor::LoadDataDescriptor(F);
	
	fread(&m_NumOfPoints, 1, 4, F);
	for (i= 0; i < m_NumOfPoints; i++)
	{
		fread(&PointName, 1, ((PointName.StrLen+ 1)* sizeof(TCHAR)+ 2), F);
		m_PointNamesList.push_back(PointName);	
	}
}

void CDlgGraph1D_Builder::vDsplDataDescriptor()
{
unsigned int i, l;
HFROW hFRow;

	CDlgData::_DsplDataDescriptor();
	
	m_ListPointNames.Clear();
	
	l= m_PointNamesList.size();
	for (i= 0; i < l; i++)
	{
		hFRow= m_ListPointNames.GetRowByInd(i);
		m_ListPointNames.SetFItemText(hFRow, 0, m_PointNamesList[i].tStr);
	}	
}

void CDlgGraph1D_Builder::vSaveData(FILE *F)
{
}

void CDlgGraph1D_Builder::vLoadData(FILE *F)
{
}

