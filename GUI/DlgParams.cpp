// DlgParams.cpp : implementation file
//

#include "stdafx.h"
#include "DlgParams.h"
#include "LabelEdit.h"

#define dxRight				10
#define dyTop					10
#define dy					  5
#define x							90
#define y							20

// std::vector<PARAM_DATA> ListParams;

// CDlgParams dialog

IMPLEMENT_DYNAMIC(CDlgParams, CDlgParamsBuilder)

CDlgParams::CDlgParams(CWnd* pParent /*=NULL*/)
	: CDlgParamsBuilder(pParent)
{
//	IDD= IDD_DLG_DUMMY;
}

CDlgParams::~CDlgParams()
{
int i, l;

	l= ListCntrl.size();
	for (i= 0; i < l; i++)
	{
		delete ListCntrl[i];	
	}
}

void CDlgParams::DoDataExchange(CDataExchange* pDX)
{
	CDlgData::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgParams, CDlgParamsBuilder)
END_MESSAGE_MAP()


// CDlgParams message handlers

BOOL CDlgParams::OnInitDialog()
{
	CDlgData::OnInitDialog();

	int i, l, xx= 0, yy= 0;
	CLabelEdit *pEdit, Edit;
	RECT Rect;
	
	::GetClientRect(m_hWnd, &Rect);
	Rect.right-= dxRight;
	Rect.left= Rect.right- x;
	
	l= ListParams.size();
	for (i= 0; i < l; i++)
	{	
		Rect.top= dyTop+ (y+ dy)*i;
		Rect.bottom= Rect.top+ y;
		Edit.Create(ListParams[i].Name, Rect, this, i, TRUE);
		if (xx < Edit.m_XX) xx= Edit.m_XX;
	}
	Rect.right= xx+ dxRight+ dxRight;
	Rect.left= Rect.right- x- dxRight;
	Rect.top= dyTop;
	for (i= 0; i < l; i++)
	{	
		Rect.bottom= Rect.top+ y;
		pEdit= new CLabelEdit;
		pEdit->MinVal= ListParams[i].MinValue;
		pEdit->MaxVal= ListParams[i].MaxValue;
		pEdit->Create(ListParams[i].Name, Rect, this, i);
		ListCntrl.push_back(pEdit);
		Rect.top= (Rect.bottom+ dy);
	}
	Rect.left= 0;
	Rect.bottom+= (y+ dy);
	Rect.top= 0;
	::AdjustWindowRectEx(&Rect, GetStyle(), FALSE, GetExStyle());
	::SetWindowPos(m_hWnd, 0, 0, 0, Rect.right- Rect.left, Rect.bottom- Rect.top, SWP_NOZORDER | SWP_NOMOVE);
return TRUE;  // return TRUE unless you set the focus to a control
// EXCEPTION: OCX Property Pages should return FALSE
}
/*
void CDlgParams::Load(FILE *F)
{
	int i, l;
	PARAM_DATA ParamData= {0};

	ListParams.clear();
	
	fread(&l, 1, 4, F);
	for (i= 0; i < l; i++)
	{
		fread(&ParamData, 1, sizeof(PARAM_DATA), F);
//		m_ListParameters.SetFItemText(m_ListParameters.GetRowByInd(i), ParamData.Name);
		ListParams.push_back(ParamData);
	}
}
*/
BOOL CDlgParams::DestroyWindow()
{
/*
int i, l;

	l= ListCntrl.size();
	for (i= 0; i < l; i++)
	{
		delete ListCntrl[i];	
	}
*/
	return CDlgParamsBuilder::DestroyWindow();
}
