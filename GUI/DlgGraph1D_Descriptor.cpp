// DlgGraph1D_Descriptor.cpp : implementation file
//

#include "stdafx.h"
#include "DlgGraph1D_Descriptor.h"


// CDlgGraph1D_Descriptor dialog

IMPLEMENT_DYNAMIC(CDlgGraph1D_Descriptor, CDlgData)

CDlgGraph1D_Descriptor::CDlgGraph1D_Descriptor(UINT ResId, CWnd* pParent /*=NULL*/)
	: CDlgData(DST_1D_GRAPH, ResId, pParent)
{

}

CDlgGraph1D_Descriptor::~CDlgGraph1D_Descriptor()
{
}

void CDlgGraph1D_Descriptor::DoDataExchange(CDataExchange* pDX)
{
	CDlgData::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgGraph1D_Descriptor, CDlgData)
END_MESSAGE_MAP()


// CDlgGraph1D_Descriptor message handlers

void CDlgGraph1D_Descriptor::LoadDataDescriptor(FILE *F)
{
	unsigned int i;
	POINT_NAME PointName;
	
	m_PointNamesList.clear();
	
	CDlgGraph1D_Descriptor::_LoadDataDescriptor(F);

	fread(&m_NumOfPoints, 1, 4, F);
	for (i= 0; i < m_NumOfPoints; i++)
	{	
		fread(&PointName.StrLen, 1, 2, F);
		fread(&PointName.tStr, 1, ((PointName.StrLen+ 1)* sizeof(TCHAR)), F);
		m_PointNamesList.push_back(PointName);
	}
}