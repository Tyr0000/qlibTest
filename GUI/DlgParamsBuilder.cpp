// DlgParameters.cpp : implementation file
//

#include "stdafx.h"
#include "DlgParamsBuilder.h"
#include "DlgParams.h"
#include "DataDescriptorImport.h"

#pragma warning (disable: 4996)

// CDlgParamsBuilder dialog

IMPLEMENT_DYNAMIC(CDlgParamsBuilder, CDlgData)

CDlgParamsBuilder::CDlgParamsBuilder(CWnd* pParent /*=NULL*/)
	: CDlgData(DATA_SET_TYPE::DST_PARAMS, CDlgParamsBuilder::IDD, pParent)
{
}

CDlgParamsBuilder::~CDlgParamsBuilder()
{
}

void CDlgParamsBuilder::DoDataExchange(CDataExchange* pDX)
{
CDlgData::DoDataExchange(pDX);
DDX_Control(pDX, IDC_LIST_FIELDS, m_ListParameters);
}


BEGIN_MESSAGE_MAP(CDlgParamsBuilder, CDlgData)
//	ON_NOTIFY(FLNM_COMBOBOX_EXPAND, IDC_LIST_FIELDS, OnComboboxExpand)

	ON_BN_CLICKED(IDC_BUTTON_MOVE_DOWN, &CDlgParamsBuilder::OnBnClickedButtonMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, &CDlgParamsBuilder::OnBnClickedButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FIELD, &CDlgParamsBuilder::OnBnClickedButtonAddField)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDlgParamsBuilder::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_DEL_FIELD, &CDlgParamsBuilder::OnBnClickedButtonDelField)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CDlgParamsBuilder::OnBnClickedButtonImport)
END_MESSAGE_MAP()


// CDlgParamsBuilder message handlers

typedef enum
{
	CI_NAME= 0,
	CI_UNITS,
	CI_VAL_TYPE,
	CI_OFFSET,
	CI_MULTIPLIER,
	CI_ADDITION,
	CI_MIN_VAL,
	CI_MAX_VAL,
	CI_NUM_OF_DECIMAL_PLACES
	
}
COLUMN_IND;

BOOL CDlgParamsBuilder::OnInitDialog()
{
	CDlgData::OnInitDialog();
	
	HFROW hFRow;
	int i, l;  
	
	// TODO:  Add extra initialization here
	SetDlgItemTextA(IDC_EDIT_TITLE, m_Title);
	
	m_ListParameters.SetExtendedStyle(LVS_EX_GRIDLINES);  
	int iControlEdit= m_ListParameters.AddControl(FL_EDIT);						//0
	int iControlFL_COMBOBOX= m_ListParameters.AddControl(FL_COMBOBOX);					//1
	m_ListParameters.SetReadOnly(FALSE);
//	m_ListParameters.SetBkColor(RGB(50, 50, 50));

	m_ListParameters.AddControlComboItem(iControlFL_COMBOBOX, "8 bit");
	m_ListParameters.AddControlComboItem(iControlFL_COMBOBOX, "16 bit");
	m_ListParameters.AddControlComboItem(iControlFL_COMBOBOX, "32 bit");
	
//	m_ListParameters.InsertFColumn(0, "", 50, FL_LEFT, iControlEdit);
	m_ListParameters.InsertFColumn(CI_NAME, "Name", 350, FL_LEFT, iControlEdit);
	m_ListParameters.InsertFColumn(CI_UNITS, "Units", 250, FL_LEFT, iControlEdit);
	m_ListParameters.InsertFColumn(CI_VAL_TYPE, "Type", 120, FL_LEFT, iControlFL_COMBOBOX);
	m_ListParameters.InsertFColumn(CI_OFFSET, "Offset", 120, FL_LEFT, iControlEdit);
	m_ListParameters.InsertFColumn(CI_MULTIPLIER, "Multiplier", 130, FL_LEFT, iControlEdit);
	m_ListParameters.InsertFColumn(CI_ADDITION, "Addition", 120, FL_LEFT, iControlEdit);
	m_ListParameters.InsertFColumn(CI_MIN_VAL, "Min Value", 150, FL_LEFT, iControlEdit);
	m_ListParameters.InsertFColumn(CI_MAX_VAL, "Max Value", 150, FL_LEFT, iControlEdit);
	m_ListParameters.InsertFColumn(CI_NUM_OF_DECIMAL_PLACES, "Num of decimal places", 150, FL_LEFT, iControlEdit);

	for (i= 0; i < 15; i++)
	{
		hFRow= m_ListParameters.InsertFRow(FL_ROOT, FL_LAST, "");
//		m_ListParameters.SetFItemVal(hFRow, 0, i+ 1);
		m_ListParameters.SetColor(FL_FROW, hFRow, -1, FL_DONTSET, RGB(255, 255, 255));
//		m_ListParameters.SetColor(FL_FROW, hFRow, 0, FL_DONTSET, RGB(255, 255, 0));
		m_ListParameters.SetFItemControl(hFRow, 2, iControlFL_COMBOBOX);
	}
	l= ListParams.size();
	for (i= 0; i < l; i++)
	{
		hFRow= m_ListParameters.GetRowByInd(i);
		if (hFRow == NULL)
			hFRow= m_ListParameters.InsertFRow(FL_ROOT, FL_LAST, "");
		m_ListParameters.SetFItemText(hFRow, CI_NAME,  ListParams[i].Name);
		m_ListParameters.SetFItenValText(hFRow, CI_VAL_TYPE, ListParams[i].Type* 8, _T(" bit"));
		m_ListParameters.SetFItemVal(hFRow, CI_OFFSET, ListParams[i].Offset);
		m_ListParameters.SetFItemVal(hFRow, CI_MULTIPLIER, ListParams[i].Multiplier);
		m_ListParameters.SetFItemVal(hFRow, CI_ADDITION, ListParams[i].Addition);
		m_ListParameters.SetFItemVal(hFRow, CI_MIN_VAL, ListParams[i].MinValue);
		m_ListParameters.SetFItemVal(hFRow, CI_MAX_VAL, ListParams[i].MaxValue);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgParamsBuilder::OnComboboxExpand(NMHDR * pNotifyStruct, LRESULT * result)
{
	FL_NOTIFY *notify=(FL_NOTIFY*)pNotifyStruct;
	*result=FL_OK;
/*
	int iControl=notify->iControlProfile;

	if (!m_ListParameters.GetControlCombo(iControl)->GetSize())
	{
		m_ListParameters.AddControlComboItem(iControl, "8 bit");
		m_ListParameters.AddControlComboItem(iControl, "16 bit");
		m_ListParameters.AddControlComboItem(iControl, "32 bit");
	}
*/
}

void CDlgParamsBuilder::SaveDataDescriptor(FILE *F)
{
	HFROW hFRow;
	int i, l, j= 0;
	PARAM_DATA ParamData= {0};
	const TCHAR *pItemName;
	
	l= m_ListParameters.GetFRowCount();
	for (i= 0; i < l; i++)
	{
		pItemName= m_ListParameters.GetFItemText(m_ListParameters.GetRowByInd(i), CI_NAME); 
		if ((pItemName == NULL) || (pItemName == "") || (pItemName[0] == 0))
			continue;
		j++;
	}
//	l= j; // m_ListParameters.GetFRowCount();
	fwrite(&j, 1, 4, F);
	for (i= 0; i < l; i++)
	{
		hFRow= m_ListParameters.GetRowByInd(i);
		pItemName= m_ListParameters.GetFItemText(hFRow, CI_NAME).GetString(); 
		if ((pItemName == NULL) || (pItemName == "") || (pItemName[0] == 0))
			continue;
		strcpy(ParamData.Name, pItemName);
		strcpy(ParamData.Units, m_ListParameters.GetFItemText(hFRow, CI_UNITS));
//		ParamData.Type= m_ListParameters.GetFItemVal(hFRow,       CI_VAL_TYPE);
		ParamData.Offset=			m_ListParameters.GetFItemVal(hFRow, CI_OFFSET);  //_tstoi(m_ListParameters.GetFItemText(hFRow, 2));
		ParamData.Multiplier= m_ListParameters.GetFItemVal(hFRow, CI_MULTIPLIER);
		ParamData.Addition=   m_ListParameters.GetFItemVal(hFRow, CI_ADDITION); 
		ParamData.MinValue=   m_ListParameters.GetFItemVal(hFRow, CI_MIN_VAL); 
		ParamData.MaxValue=   m_ListParameters.GetFItemVal(hFRow, CI_MAX_VAL); 
		fwrite(&ParamData, 1, sizeof(PARAM_DATA), F);
	}
}

void CDlgParamsBuilder::LoadDataDescriptor(FILE *F)
{
	int i, l;
	PARAM_DATA ParamData= {0};

	ListParams.clear();
	
	CDlgData::_LoadDataDescriptor(F);
	
	fread(&l, 1, 4, F);
	for (i= 0; i < l; i++)
	{
		fread(&ParamData, 1, sizeof(PARAM_DATA), F);
//		m_ListParameters.SetFItemText(m_ListParameters.GetRowByInd(i), ParamData.Name);
		ListParams.push_back(ParamData);
	}
}

void CDlgParamsBuilder::vSaveDataDescriptor(FILE *F)
{
	HFROW hFRow;
	int i, l, j= 0;
	PARAM_DATA ParamData= {0};
	const char *pItemName;
	
	l= m_ListParameters.GetFRowCount();
	for (i= 0; i < l; i++)
	{
		pItemName= m_ListParameters.GetFItemText(m_ListParameters.GetRowByInd(i), 1); 
		if ((pItemName == NULL) || (pItemName == "") || (pItemName[0] == 0))
			continue;
		j++;
	}
//	l= j; // m_ListParameters.GetFRowCount();
	fwrite(&j, 1, 4, F);
	for (i= 0; i < l; i++)
	{
		hFRow= m_ListParameters.GetRowByInd(i);
		pItemName= m_ListParameters.GetFItemText(hFRow, CI_NAME); 
		if ((pItemName == NULL) || (pItemName == "") || (pItemName[0] == 0))
			continue;
		strcpy(ParamData.Name, pItemName);
//		ParamData.Type= m_ListParameters.GetFItemVal(hFRow,       CI_VAL_TYPE);
//		ParamData.Type/= 8;
		ParamData.Offset=			m_ListParameters.GetFItemVal(hFRow, CI_OFFSET);  //_tstoi(m_ListParameters.GetFItemText(hFRow, 2));
		ParamData.Multiplier= m_ListParameters.GetFItemVal(hFRow, CI_MULTIPLIER);
		ParamData.Addition=   m_ListParameters.GetFItemVal(hFRow, CI_ADDITION); 
		ParamData.MinValue=   m_ListParameters.GetFItemVal(hFRow, CI_MIN_VAL); 
		ParamData.MaxValue=   m_ListParameters.GetFItemVal(hFRow, CI_MAX_VAL); 
		fwrite(&ParamData, 1, sizeof(PARAM_DATA), F);
	}
}

void CDlgParamsBuilder::vLoadDataDescriptor(FILE *F)
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

void CDlgParamsBuilder::FillParamList()
{
	HFROW hFRow;
	int i, l, j= 0;
	PARAM_DATA ParamData= {0};
	const char *pItemName;

	ListParams.clear();
	l= m_ListParameters.GetFRowCount();
	for (i= 0; i < l; i++)
	{
		hFRow= m_ListParameters.GetRowByInd(i);
		pItemName= m_ListParameters.GetFItemText(hFRow, 1); 
		if ((pItemName == NULL) || (pItemName == "") || (pItemName[0] == 0))
			continue;
		strcpy(ParamData.Name, pItemName);
		ParamData.Offset=			m_ListParameters.GetFItemVal(hFRow, 3);  //_tstoi(m_ListParameters.GetFItemText(hFRow, 2));
		ParamData.Multiplier= m_ListParameters.GetFItemVal(hFRow, 4);
		ParamData.Addition=   m_ListParameters.GetFItemVal(hFRow, 5); 
		ParamData.MinValue=   m_ListParameters.GetFItemVal(hFRow, 6); 
		ParamData.MaxValue=   m_ListParameters.GetFItemVal(hFRow, 7); 
		ListParams.push_back(ParamData);
	}
}

void CDlgParamsBuilder::OnBnClickedButtonMoveDown()
{
// TODO: Add your control notification handler code here
}

void CDlgParamsBuilder::OnBnClickedButtonMoveUp()
{
// TODO: Add your control notification handler code here
}

void CDlgParamsBuilder::OnBnClickedButtonAddField()
{
	HFROW hFRow;
	int l;
	
	hFRow= m_ListParameters.InsertFRow(FL_ROOT, FL_LAST, "");
//	l= m_ListParameters.GetFRowCount();
//	m_ListParameters.SetFItemVal(hFRow, 0, l);
}

void CDlgParamsBuilder::OnBnClickedButtonTest()
{
	CDlgParams Dlg;

	FillParamList();
	Dlg.ListParams= ListParams;
	Dlg.DoModal();
//	Dlg.Create(CDlgParams::IDD, this);
}

void CDlgParamsBuilder::OnBnClickedButtonDelField()
{
	HFROW hFRow;
	int i, l;
	
	hFRow= m_ListParameters.GetSelectedFRow();
	m_ListParameters.DeleteFRow(hFRow);
//	m_ListParameters.InsertFRow(FL_ROOT, FL_LAST, "");
/*
	l= m_ListParameters.GetFRowCount();
	for (i= 0; i < l; i++)
	{
		hFRow= m_ListParameters.GetRowByInd(i);
		m_ListParameters.SetFItemVal(hFRow, 0,  i+ 1);
	}
*/
}

char TestParams[]=
"dialog = speed_settings,   \"Speed Sensors\"\r\n\
   field = \"#Crankshaft Speed Settings\"\r\n\
       field = \"Tooth Count\", trigg1_cnt\r\n\
       field = \"Pulse divider\", trigg1_div\r\n\
       field = \"Missing Tooth Wheel\", trigger_conf_missing\r\n\
   field = \"#Driven Wheel Speed Sensor Settings\"";

void CDlgParamsBuilder::OnBnClickedButtonImport()
{
	char ImportData[2048];
	
	SetDlgItemText(IDC_EDIT_IMPORT, TestParams);
	GetDlgItemText(IDC_EDIT_IMPORT, ImportData, 2048);
	char *pSym= ImportData;
	char *pValStr;
	bool IsEOL= false;
	PARAM_DATA ParamData= {0};

	if (strstr(pSym, "dialog") == NULL) return;
	SKIP_UNTIL_SYM(pSym, '"')
	pSym++;    
	pValStr= pSym;
	SKIP_UNTIL_SYM(pSym, '"')
	*pSym= 0;
	pSym++;    
	SetDlgItemText(IDC_EDIT_TITLE, pValStr);

	while (*pSym != 0)
	{
		pSym= strstr(pSym, "field");
		if (pSym == 0)
			break;
		{
			SKIP_UNTIL_SYM(pSym, '"')
			if (pSym == NULL)
					return;
			pSym++;    
			pValStr= pSym;
			SKIP_UNTIL_SYM(pSym, '"')
			pSym++; 
			*pSym= 0;
			SKIP_UNTIL_SYM(pSym, '"')
			pSym++; 
			strcpy(ParamData.Name, pValStr);
			SKIP_WHITE_SPACE(pSym)
			if (!IsEOL)
			{ }
			ListParams.push_back(ParamData);	
			HFROW hFRow;
			hFRow= m_ListParameters.InsertFRow(FL_ROOT, FL_LAST, "");
			m_ListParameters.SetFItemText(hFRow, CI_NAME,  pValStr);
		//	l= m_ListParameters.GetFRowCount();
		//	m_ListParameters.SetFItemVal(hFRow, 0, l);
		}
	}
}
