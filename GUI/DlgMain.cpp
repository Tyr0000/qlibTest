/* 
а это пример графика
	at_shift_up_1_2		= array,	U08,	0,	[8],	'kph',	1.0,	0.0,	0.0,	255,	0
вот пример как задана таблица 12х12
	at_settings_all		= array,	U08,	0,	[12x12],	'',	1.0,	0.0,	0.0,	255,	0
*/



// DymanicMenuDlg.cpp : implementation file
//

#include "stdafx.h"
#include <vector>
//#include <deque>
#include "DlgMain.h"
#include "DlgParamsBuilder.h"
#include "DlgParams.h"
#include "DlgTblBuilder.h"
#include "DlgGraph3DBuilder.h"
#include "DlgLoadData.h"
#include "u_File.h"

#pragma warning (disable: 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//HTREEITEM hSelItem= 0;
UINT ActRadioBttn= 0;
DATA_SET_TYPE ActDlgType= DATA_SET_TYPE::DST_UNDEFINED;
CDlgData *pActDlg= NULL;
CDlgParamsBuilder *pDlgParameters= NULL;
CDlgTblBuilder *pDlgTblBuilder= NULL;
CDlgGraph3D_Builder *pDlgGraph3DBuilder= NULL;
CDlgMain *pDlgMain;
HWND hWndPlaceHolder;
RECT RectPlaceHolder;
HDC hDcPlaceHolder;
HBRUSH hBrBck= NULL;
HPEN hPen= NULL;
bool SaveItens= false;
bool SaveData= false;
bool ItemTypeCnanged= false;
bool DataSaved= false;
bool MenuItemsSaved= false;

//std::deque<MENU_ITEM_DATA> MenuList;
//std::vector<MENU_ITEM_DATA> MenuList;
//std::vector<HTREEITEM> MenuList;

void OnActDlgChanged(DATA_SET_TYPE DlgType)
{
	ActDlgType= DlgType;
	pDlgMain->OnActDlgChanged(ActDlgType);
}

// CDlgMain dialog

CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMain::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgMain::~CDlgMain()
{
//	m_TreeMenu.ClearMenu();
//	m_TstMenu.ClearMenu();
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
CDialog::DoDataExchange(pDX);
DDX_Control(pDX, IDC_TREE_MENU_BUILDER, m_TreeMenu);
DDX_Control(pDX, IDC_TREE_MENU_TST, m_TstMenu);
}

BEGIN_MESSAGE_MAP(CDlgMain, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ADD_MENU_ITEM, &CDlgMain::OnBnClickedButtonAddMenuItem)
	ON_BN_CLICKED(IDC_BUTTON_DEL_MENU_ITEM, &CDlgMain::OnBnClickedButtonDelMenuItem)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MENU_ROOT_ITEM, &CDlgMain::OnBnClickedButtonAddMenuRootItem)
	ON_BN_CLICKED(IDC_RADIO_PARAMETER_DLG, &CDlgMain::OnBnClickedRadioParameterDlg)
	ON_BN_CLICKED(IDC_RADIO_TBL, &CDlgMain::OnBnClickedRadioTbl)
	ON_BN_CLICKED(IDC_RADIO_GRAPH, &CDlgMain::OnBnClickedRadioGraph)
	ON_BN_CLICKED(IDC_RADIO_2D_GRAPH, &CDlgMain::OnBnClickedRadio2dGraph)
	ON_BN_CLICKED(IDC_RADIO_3D_GRAPH, &CDlgMain::OnBnClickedRadio3dGraph)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_MENU, &CDlgMain::OnBnClickedButtonLoadMenu)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_ITEMS, &CDlgMain::OnBnClickedButtonSaveItems)
//	ON_BN_CLICKED(IDC_BUTTON_TST, &CDlgMain::OnBnClickedButtonTst)
//	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MENU_BUILDER, &CDlgMain::OnTvnSelchangedTreeMenu)
	ON_NOTIFY(NM_CLICK, IDC_TREE_MENU_TST, &CDlgMain::OnNMClickTreeMenu2)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_MENU_TST, &CDlgMain::OnNMDblclkTreeMenu2)
	ON_BN_CLICKED(IDC_BUTTON_DEL_ALL_MENU_ITEMS, &CDlgMain::OnBnClickedButtonDelAllMenuItems)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_DATA, &CDlgMain::OnBnClickedButtonSaveData)
	ON_BN_CLICKED(IDC_CHECK_SAVE_ITEMS, &CDlgMain::OnBnClickedCheckSaveItems)
	ON_BN_CLICKED(IDC_CHECK_SAVE_DATA, &CDlgMain::OnBnClickedCheckSaveData)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgMain::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DATA, &CDlgMain::OnBnClickedButtonLoadData)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_EXT_MENU, &CDlgMain::OnBnClickedButtonLoadExtMenu)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_ITEM, &CDlgMain::OnBnClickedButtonImportItem)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_ITEM_DATA, &CDlgMain::OnBnClickedButtonImportItemData)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_ITEM, &CDlgMain::OnBnClickedButtonExportItem)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_ITEM_DATA2, &CDlgMain::OnBnClickedButtonImportItemData2)
END_MESSAGE_MAP()


// CDlgMain message handlers

BOOL CDlgMain::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	pDlgMain= this;
	hWndPlaceHolder= GetDlgItem(IDC_STATIC_PLACEHOLDER_DLG)->m_hWnd;
	::GetClientRect(hWndPlaceHolder, &RectPlaceHolder);
	hDcPlaceHolder= ::GetDC(hWndPlaceHolder);
//	SelectObject(hDcPlaceHolder, ::GetStockObject(LTGRAY_BRUSH));
//	SelectObject(hDcPlaceHolder, ::GetStockObject(NULL_PEN));
	hBrBck= ::CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
	hPen= CreatePen(PS_INSIDEFRAME, 0, ::GetSysColor(COLOR_3DFACE));
	SelectObject(hDcPlaceHolder, hBrBck);
	SelectObject(hDcPlaceHolder, hPen);
	::Rectangle(hDcPlaceHolder, 0, 0, RectPlaceHolder.right, RectPlaceHolder.bottom);
//	m_TreeMenu.InsertItem("Menu");

	m_TreeMenu.m_MenuType= MT_BUILDER;
	m_TreeMenu.m_pDlgPlaceHolder= GetDlgItem(IDC_STATIC_PLACEHOLDER_DLG);
	m_TreeMenu.OnActDlgChanged= ::OnActDlgChanged;
	m_TreeMenu.AttachImageList();
	
	m_TstMenu.m_MenuType= MT_WORK;
	m_TreeMenu.m_hWndMenuName= GetDlgItem(IDC_STATIC_MENU_NAME)->m_hWnd;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgMain::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgMain::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CDlgMain::DestroyWindow()
{
	m_TreeMenu.Clear();
	m_TstMenu.Clear();
	
	::ReleaseDC(hWndPlaceHolder, hDcPlaceHolder);
	DeleteObject(hBrBck);
	DeleteObject(hPen);
	
	return CDialog::DestroyWindow();
}

void CDlgMain::OnBnClickedButtonDelMenuItem()
{
	HTREEITEM hItem;
	char ItemName[MENU_ITEM_MAX_LEN];
	
	hItem= m_TreeMenu.GetSelectedItem();
	if (hItem == NULL)
		return;
	m_TreeMenu.DeleteItem(hItem, TRUE);
	m_TreeMenu.UpdateWindow();
}

void CDlgMain::OnBnClickedButtonDelAllMenuItems()
{
	m_TreeMenu.Clear();
	m_TstMenu.Clear();
	SetRadioBttnState(DST_UNDEFINED);
}

void CDlgMain::OnBnClickedButtonAddMenuItem()
{
static int ItemId= 0;
	HTREEITEM hItemParent, hItem;
	TCHAR ItemName[MENU_ITEM_MAX_LEN];
/*	
	itoa(ItemId, ItemName, 10);
	SetDlgItemText(IDC_EDIT_MENU_ITEM_NAME, ItemName);
	ItemId++;
*/
	GetDlgItemText(IDC_EDIT_MENU_ITEM_NAME, ItemName, MENU_ITEM_MAX_LEN);
	hItemParent= m_TreeMenu.GetSelectedItem(); 
/*
	hItem= m_TreeMenu.InsertItem(ItemName, hItemParent);
	m_TreeMenu.SetItemData(hItem, 0); 
	if ((hItemParent == 0) || (hItemParent == TVI_ROOT))
		m_TreeMenu.Select(hItem, TVGN_CARET);
	else
		m_TreeMenu.Expand(hItemParent, TVE_EXPAND);
	m_TreeMenu.UpdateWindow();
*/
	if (m_TreeMenu.AddItem(ItemName, hItemParent) == -1)
		::MessageBox(m_hWnd, _T("Item name already exists"), _T("User error"), MB_ICONEXCLAMATION | MB_OK);
/*
	((CButton *)GetDlgItem(IDC_RADIO_PARAMETER_DLG))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_RADIO_TBL))->SetCheck(false);     
	((CButton *)GetDlgItem(IDC_RADIO_GRAPH))->SetCheck(false);   
	((CButton *)GetDlgItem(IDC_RADIO_2D_GRAPH))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_RADIO_3D_GRAPH))->SetCheck(false);
*/
}

void CDlgMain::OnBnClickedButtonAddMenuRootItem()
{
	HTREEITEM hItem;
	TCHAR ItemName[128];
	
	GetDlgItemText(IDC_EDIT_MENU_ITEM_NAME, ItemName, 128);
//	hItemParent= m_TreeMenu.GetSelectedItem();
	if (m_TreeMenu.AddItem(ItemName, 0) == -1)
	{
		::MessageBox(m_hWnd, _T("Item name already exists"), _T("User error"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}
/*
	hItem= m_TreeMenu.InsertItem(ItemName, 0);
	m_TreeMenu.SetItemData(hItem, 0);
	m_TreeMenu.UpdateWindow();
	m_TreeMenu.SelectItem(hItem);
*/
	((CButton *)GetDlgItem(IDC_RADIO_PARAMETER_DLG))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_RADIO_TBL))->SetCheck(false);     
	((CButton *)GetDlgItem(IDC_RADIO_GRAPH))->SetCheck(false);   
	((CButton *)GetDlgItem(IDC_RADIO_2D_GRAPH))->SetCheck(false);
	((CButton *)GetDlgItem(IDC_RADIO_3D_GRAPH))->SetCheck(false);
}

void CDlgMain::SetRadioBttnState(DATA_SET_TYPE DlgType)
{
	CButton *pButton;
	
	if (ActRadioBttn != 0)
	{
		pButton= (CButton *)GetDlgItem(ActRadioBttn);
		if (pButton->GetCheck() != 0)
			pButton->SetCheck(0);
	}
	switch (DlgType)
	{
		case DST_UNDEFINED:
			::Rectangle(hDcPlaceHolder, 0, 0, RectPlaceHolder.right, RectPlaceHolder.bottom);
			ActRadioBttn= 0;
		break;
		case DST_PARAMS:
			((CButton *)GetDlgItem(IDC_RADIO_PARAMETER_DLG))->SetCheck(1);
			ActRadioBttn= IDC_RADIO_PARAMETER_DLG;
		break;
		case DST_2D_TBL:
			((CButton* )GetDlgItem(IDC_RADIO_TBL))->SetCheck(1);
			ActRadioBttn= IDC_RADIO_TBL;
		break;
		case DST_1D_GRAPH:
			((CButton* )GetDlgItem(IDC_RADIO_GRAPH))->SetCheck(1);
			ActRadioBttn= IDC_RADIO_GRAPH;
		break;
		case DST_2D_GRAPH:
			((CButton* )GetDlgItem(IDC_RADIO_2D_GRAPH))->SetCheck(1);
			ActRadioBttn= IDC_RADIO_2D_GRAPH;
		break;
		case DST_3D_GRAPH:
			((CButton* )GetDlgItem(IDC_RADIO_3D_GRAPH))->SetCheck(1);
			ActRadioBttn= IDC_RADIO_3D_GRAPH;
		break;
	}
//	if (ActRadioBttn != 0)	
//		((CButton* )GetDlgItem(ActRadioBttn))->SetCheck(1);
}

void CDlgMain::CreateBuildDlg(DATA_SET_TYPE DlgType)
{
//	CDlgData *pDlg; 
	
  switch (DlgType)
  {
		case DST_PARAMS:
			if (pActDlg == NULL)
			{
				pActDlg= new CDlgParamsBuilder;
			}
//			if (F != NULL)
//				pDlg->Load(F);
			pActDlg->Create(CDlgParamsBuilder::IDD, GetDlgItem(IDC_STATIC_PLACEHOLDER_DLG));
		break;
		case DST_2D_TBL:
			if (pActDlg == NULL)
			{
				pActDlg= new CDlgTblBuilder;
			}
			pActDlg->Create(CDlgTblBuilder::IDD, GetDlgItem(IDC_STATIC_PLACEHOLDER_DLG));
		break;
		case DST_1D_GRAPH:
			if (pActDlg == NULL)
			{
				pActDlg= new CDlgGraph3D_Builder;
			}
			pActDlg->Create(CDlgGraph3D_Builder::IDD, GetDlgItem(IDC_STATIC_PLACEHOLDER_DLG));
		break;
		case DST_2D_GRAPH:
			if (pActDlg == NULL)
			{
				pActDlg= new CDlgGraph3D_Builder;
			}
			pActDlg->Create(CDlgGraph3D_Builder::IDD, GetDlgItem(IDC_STATIC_PLACEHOLDER_DLG));
		break;
		case DST_3D_GRAPH:
			if (pActDlg == NULL)
			{
				pActDlg= new CDlgGraph3D_Builder;
			}
			pActDlg->Create(CDlgGraph3D_Builder::IDD, GetDlgItem(IDC_STATIC_PLACEHOLDER_DLG));
		break;
	}
}

void CDlgMain::SetActBuildDlg(DATA_SET_TYPE DlgType)
{	
	HTREEITEM hItem;
	
	hItem= m_TreeMenu.GetSelectedItem();
	if (hItem == NULL)
	{
		::MessageBox(m_hWnd, _T("Select a manu item"), _T("Info"), MB_OK);
		SetRadioBttnState(DST_UNDEFINED);
		return;
	}

//	pActDlg= (CDlgData *)m_TreeMenu.GetItemData(hItem);
	if ((pActDlg != NULL) && (DlgType != pActDlg->m_DataType))
	{
		if (::MessageBox(m_hWnd, _T("The dialod is already assigned to this menu item.\r\nDo you want change the dialod type?"), _T("Info"), MB_ICONQUESTION | MB_YESNO) != IDYES)
		{
			SetRadioBttnState(pActDlg->m_DataType);
			return;
		}
		delete pActDlg;
		pActDlg= NULL;
		m_TreeMenu.SetItemData(hItem, NULL);
	}
	
  if ((pActDlg != NULL) && (pActDlg->m_hWnd != NULL))	 
  {
		pActDlg->ShowWindow(SW_RESTORE);
		return;
  }
  CreateBuildDlg(DlgType);
	m_TreeMenu.SetItemData(hItem, (DWORD_PTR)pActDlg);
//	SetRadioBttnState(DlgType);
}

void CDlgMain::OnBnClickedRadioParameterDlg()
{
/*
	HTREEITEM hItem;
	
	hItem= m_TreeMenu.GetSelectedItem();
	if (hItem == NULL)
		return;
	pDlgParameters= (CDlgParamsBuilder *)m_TreeMenu.GetItemData(hItem);
	if (pActDlg != NULL)
	{
		pActDlg->CloseWindow();
		pActDlg->DestroyWindow();
		delete pActDlg;
	}
	if (pDlgParameters == NULL)
	{
		pDlgParameters= new CDlgParamsBuilder;
		pDlgParameters->Create(CDlgParamsBuilder::IDD, GetDlgItem(IDC_STATIC_PLACEHOLDER_DLG));
		pActDlg= pDlgParameters;
	}
*/ 
	if (ActRadioBttn == IDC_RADIO_PARAMETER_DLG)
		return;

	if (ActDlgType != DST_UNDEFINED)
	{
		if (::MessageBox(m_hWnd, _T("Do you really want to change dialog type?"), _T("Attention"), MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			((CButton *)GetDlgItem(ActRadioBttn))->SetCheck(1);
			((CButton *)GetDlgItem(IDC_RADIO_PARAMETER_DLG))->SetCheck(0);
			return;
		}
		ItemTypeCnanged= true;
	}
	ActRadioBttn= IDC_RADIO_PARAMETER_DLG;
	m_TreeMenu.ChangeActDlgType(DST_PARAMS);	
}

void CDlgMain::OnBnClickedRadioTbl()
{
	if (ActRadioBttn == IDC_RADIO_TBL)
		return;
		
	if (ActDlgType != DST_UNDEFINED)
	{
		if (::MessageBox(m_hWnd, _T("Do you really want to change dialog type?"), _T("Attention"), MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			((CButton *)GetDlgItem(IDC_RADIO_TBL))->SetCheck(0);
			((CButton *)GetDlgItem(ActRadioBttn))->SetCheck(1);
			return;
		}
		ItemTypeCnanged= true;
	}
	ActRadioBttn= IDC_RADIO_TBL;
	m_TreeMenu.ChangeActDlgType(DST_2D_TBL);
}

void CDlgMain::OnBnClickedRadioGraph()
{
	if (ActRadioBttn == IDC_RADIO_GRAPH)
		return;
		
	if (ActDlgType != DST_UNDEFINED)
	{
		if (::MessageBox(m_hWnd, _T("Do you really want to change dialog type?"), _T("Attention"), MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			((CButton *)GetDlgItem(IDC_RADIO_GRAPH))->SetCheck(0);
			((CButton *)GetDlgItem(ActRadioBttn))->SetCheck(1);
			return;
		}
		ItemTypeCnanged= true;
	}
	ActRadioBttn= IDC_RADIO_GRAPH;
	m_TreeMenu.ChangeActDlgType(DST_1D_GRAPH);	
}

void CDlgMain::OnBnClickedRadio2dGraph()
{
	if (ActRadioBttn == IDC_RADIO_2D_GRAPH)
		return;

	if (ActDlgType != DST_UNDEFINED)
	{
		if (::MessageBox(m_hWnd, _T("Do you really want to change dialog type?"), _T("Attention"), MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			((CButton *)GetDlgItem(IDC_RADIO_2D_GRAPH))->SetCheck(0);
			((CButton *)GetDlgItem(ActRadioBttn))->SetCheck(1);
			return;
		}
		ItemTypeCnanged= true;
	}
	ActRadioBttn= IDC_RADIO_2D_GRAPH;
	m_TreeMenu.ChangeActDlgType(DST_2D_GRAPH);	
}

void CDlgMain::OnBnClickedRadio3dGraph()
{
	if (ActRadioBttn == IDC_RADIO_3D_GRAPH)
		return;
		
	if (ActDlgType != DST_UNDEFINED)
	{
		if (::MessageBox(m_hWnd, _T("Do you really want to change dialog type?"), _T("Attention"), MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			((CButton *)GetDlgItem(IDC_RADIO_3D_GRAPH))->SetCheck(0);
			((CButton *)GetDlgItem(ActRadioBttn))->SetCheck(1);
			return;
		}
		ItemTypeCnanged= true;
	}
	ActRadioBttn= IDC_RADIO_3D_GRAPH;
	m_TreeMenu.ChangeActDlgType(DST_3D_GRAPH);	
}

void CDlgMain::OnTvnSelchangedTreeMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	m_TreeMenu.m_hSelItem= pNMTreeView->itemNew.hItem;
/*
  HTREEITEM hItem;
  
  hItem= m_TreeMenu.GetSelectedItem();
	if (hItem == NULL)
	{
		::MessageBoxA(m_hWnd, "Select a manu item", "Info", MB_OK);
		return;
	}
*/
// DT_DEFINED	

	if (pActDlg != NULL)
	{
//		pActDlg->CloseWindow();
		if ((pActDlg->m_hWnd != NULL) && ::IsWindow(pActDlg->m_hWnd))
			pActDlg->ShowWindow(SW_HIDE);
		pActDlg= NULL;
		SetRadioBttnState(DST_UNDEFINED);
	}
	
	pActDlg= (CDlgData *)m_TreeMenu.GetItemData(pNMTreeView->itemNew.hItem);
  
  if (pActDlg == NULL)
	{
		SetRadioBttnState(DST_UNDEFINED);
//		SetActDlg(DT_DEFINED);
//		::Rectangle(hDcPlaceHolder, 0, 0, RectPlaceHolder.right, RectPlaceHolder.bottom);
//		GetDlgItem(IDC_STATIC_PLACEHOLDER_DLG)->UpdateWindow();
//		GetDlgItem(IDC_STATIC_PLACEHOLDER_DLG)->RedrawWindow(NULL, NULL, RDW_ERASE | RDW_ERASENOW);
//		RedrawWindow(NULL, NULL, RDW_ERASE | RDW_ERASENOW | RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_INTERNALPAINT);
//		UpdateWindow();
	}	
	else
	{
		if (pActDlg->m_hWnd == NULL)
//			SetActBuildDlg(pActDlg->m_DataType);
			CreateBuildDlg(pActDlg->m_DataType);
		else
		{
			SetRadioBttnState(pActDlg->m_DataType);
			pActDlg->ShowWindow(SW_RESTORE);
		}
	}
}

void CDlgMain::ClearMenu()
{
	m_TreeMenu.Clear();
	m_TstMenu.Clear();
}

void CDlgMain::OnActDlgChanged(DATA_SET_TYPE DlgType)
{
	SetRadioBttnState(DlgType);
}
/*
void CDlgMain::Load(FILE *FF)
{
	MENU_ITEM_DATA ItemData= {0};
	HTREEITEM hItem;
	FILE *F;
	unsigned char *pData;
	int i, DataLen;
	size_t NumOfBytes;
	
//	ClearMenu();
	m_TstMenu.Clear(); 

	F= fopen("menu", "rb");
	if (F == 0)
		return;
	pData= (unsigned char *)&ItemData+ sizeof(HTREEITEM)+ sizeof(CDlgData *);
	DataLen= sizeof(MENU_ITEM_DATA)- (sizeof(HTREEITEM)+ + sizeof(CDlgData *));
	while (TRUE)
	{
		NumOfBytes= fread(pData, 1, DataLen, F);
		if (NumOfBytes != DataLen)
			break;
		if (ItemData.ItemType == DST_PARAMS)
		{
			ItemData.pDlg=  new CDlgParams(); // CreateDlg(DST_PARAMS);
			ItemData.pDlg->Load(F);
		}
		else
			ItemData.pDlg= NULL;
		MenuList.push_back(ItemData);
	}
	fclose(F);
	DataLen= MenuList.size();
	for (i= 0; i < DataLen; i++)
	{
		ItemData= MenuList[i];
		if (ItemData.ParentId == -1)
		{
			MenuList[i].hItem= m_TstMenu.InsertItem(ItemData.ItemName, 0);
		}
		else
		{
			MenuList[i].hItem= m_TstMenu.InsertItem(ItemData.ItemName, MenuList[MenuList[i].ParentId].hItem);
		}
		if (MenuList[i].pDlg != NULL)
			m_TstMenu.SetItemData(MenuList[i].hItem, (DWORD_PTR)MenuList[i].pDlg);
	}
	MenuList.clear();
}
*/
void CDlgMain::OnBnClickedButtonLoadMenu()
{
	CWaitCursor WaitCursor; 
	MENU_ITEM_DATA ItemData= {0};
	HTREEITEM hItem;
	FILE *F;
	unsigned char *pData;
	int i, DataLen;
	size_t NumOfBytes;
	char *pFN, *pShortFN;
		
	if (ItemTypeCnanged && DataSaved && !MenuItemsSaved)
	{
		if (::MessageBox(m_hWnd, _T("Menu items was not saved. The data file may be unavalabale for next loading.\r\n\
														Click Camcel to if you want to save menu items fist."), _T("Attention"), MB_ICONEXCLAMATION | MB_OKCANCEL) == IDCANCEL)
			return;
	}
		
	pFN= SelectFileToOpen(m_hWnd, "Select a menu file", "menu items files\0*.mif\0\0", &pShortFN);
	if (pFN == NULL)
		return;
	
	if (pFN == NULL)
		return;
		
	ItemTypeCnanged= false; 
	DataSaved= false;
	MenuItemsSaved= false;
		
	m_TreeMenu.LoadItems(pFN, pShortFN);
//	m_TstMenu.Load(pFN, pShortFN);
	SetRadioBttnState(DST_UNDEFINED);
}
/*
void CDlgMain::SaveItemData(FILE *F, MENU_ITEM_DATA *pItemData, HTREEITEM hItem, int ItemId, int ParentId)
{
	CString ItemName;
	CDlgData *pDlg;
	unsigned char *pData;

	pItemData->ItemId= ItemId;
	pItemData->ParentId= ParentId;
	pItemData->hItem= hItem;
	ItemName= m_TreeMenu.GetItemText(hItem);
	strcpy(pItemData->ItemName, ItemName);
	pDlg= (CDlgData *)m_TreeMenu.GetItemData(hItem);
	if (pDlg != NULL)
		pItemData->ItemType= pDlgm_DataType;
	else 
		pItemData->ItemType= DST_UNDEFINED;
		
	pData= (unsigned char *)pItemData+ sizeof(HTREEITEM)+ sizeof(CDlgData *);
	fwrite(pData, sizeof(MENU_ITEM_DATA)- (sizeof(HTREEITEM)+ sizeof(CDlgData *)), 1, F);
	if (pDlg != NULL)
		pDlg->Save(F);
}
*/
void CDlgMain::OnBnClickedButtonSaveItems()
{
	CWaitCursor WaitCursor; 
	int i, ItemId= 0, ParentId= 0, ChildId= 0, ItemCnt;
	HTREEITEM hItem, hNextItem, hChildItem;
	MENU_ITEM_DATA ItemData= {0};
	FILE *F;
		
	char *pFN, *pShortFN;
				
	ItemCnt= m_TreeMenu.GetCount();
	if (ItemCnt == 0)
	{
		::MessageBox(m_hWnd, _T("There are no menu items"), _T("Info"), MB_OK);
		return;
	}
	pFN= SelectFileToSave(m_hWnd, _T("Enter a menu item file"), _T("menu data files\0*.mif\0\0"), &pShortFN);
	
	if (pFN == NULL)
		return;
  i= strlen(pFN);
  if ((i < 4) || (pFN[i- 4] != '.'))
		strcpy(&pFN[i], ".mif");
	F= fopen(pFN, "wb+");
	if (F == 0)
		return;
	m_TreeMenu.Save(F);
	fclose(F);
}

void CDlgMain::OnBnClickedButtonSaveData()
{
	// TODO: Add your control notification handler code here
}

//void CDlgMain::OnBnClickedButtonTst()
//{
//	// TODO: Add your control notification handler code here
//}

void CDlgMain::OnNMClickTreeMenu2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	TVHITTESTINFO ht = {0};
	LPNMTREEVIEW lpTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	DWORD dwpos = GetMessagePos();
  HTREEITEM hItem= NULL;
  DWORD ToId;
  CDlgParams *pDlg;

	if (lpTreeView->action == 0) 
		hItem= lpTreeView->itemNew.hItem;
	else
	{
		ht.pt.x = GET_X_LPARAM(dwpos);
		ht.pt.y = GET_Y_LPARAM(dwpos);
		::MapWindowPoints(HWND_DESKTOP, lpTreeView->hdr.hwndFrom, &ht.pt, 1);

		TreeView_HitTest(lpTreeView->hdr.hwndFrom, &ht);
		

//	if (!ContactClicked)
//		return;
		hItem= ht.hItem; // lpTreeView->itemNew.hItem; 
	}
	if (hItem == NULL)
	{
	 return;
	}	
	pDlg= (CDlgParams *)m_TstMenu.GetItemData(hItem) ;
	if (pDlg != NULL)
	{
		pDlg->DoModal();
//		pDlg->
	}
}

void CDlgMain::OnNMDblclkTreeMenu2(NMHDR *pNMHDR, LRESULT *pResult)
{
		
	*pResult = 0;
}

BOOL CDlgMain::PreTranslateMessage(MSG* pMsg)
{
    if(WM_KEYDOWN == pMsg->message)
    {
//			if (m_ReadOnly)
//        return TRUE;        

        if(VK_RETURN == pMsg->wParam)
        {
					pMsg->wParam= 0;
					pMsg->lParam= 0;
//					MessageBeep(0);
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
    if (pMsg->message == WM_ACT_DLG)
    {
			MessageBeep(0);
    }

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgMain::OnBnClickedCheckSaveItems()
{
	SaveItens^= 1;
}

void CDlgMain::OnBnClickedCheckSaveData()
{
	SaveData^= 1;
}

void CDlgMain::OnBnClickedButtonSave()
{
	CWaitCursor WaitCursor; 
	int i, ItemId= 0, ParentId= 0, ChildId= 0, ItemCnt;
	HTREEITEM hItem, hNextItem, hChildItem;
	MENU_ITEM_DATA ItemData= {0};
	FILE *ItemF= NULL, *DataF= NULL;
		
	char *pFN, *pShortFN;
				
	ItemCnt= m_TreeMenu.GetCount();
	if (ItemCnt == 0)
	{
		::MessageBox(m_hWnd, _T("There are no menu items"), _T("Info"), MB_OK);
		return;
	}
	if (!SaveItens && !SaveData)
	{
		::MessageBox(m_hWnd, _T("Define a select option"), _T("Info"), MB_OK);
		return;
	}
	if (SaveItens)
	{
		MenuItemsSaved= true;
		pFN= SelectFileToSave(m_hWnd, _T("Enter a menu item file name"), _T("menu item files\0*.mif\0\0"), &pShortFN);
		if (pFN == NULL)
			return;
		i= strlen(pFN);
		if ((i < 4) || (pFN[i- 4] != '.'))
			strcpy(&pFN[i], ".mif");
		ItemF= fopen(pFN, "wb+");
		if (ItemF == 0)
			return;
	}
	if (SaveData)
	{
		DataSaved= true;
		pFN= SelectFileToSave(m_hWnd, _T("Enter a data file name"), _T("menu data files\0*.mdf\0\0"), &pShortFN);
		if (pFN == NULL)
			goto L_EXIT;
		i= strlen(pFN);
		if ((i < 4) || (pFN[i- 4] != '.'))
			strcpy(&pFN[i], ".mdf");
		DataF= fopen(pFN, "wb+");
		if (DataF == 0)
			return;
	}
	m_TreeMenu.Save(ItemF, DataF);
L_EXIT:	
	if (ItemF != NULL)
	fclose(ItemF);
	if (DataF != NULL)
	fclose(DataF);
}

void CDlgMain::OnBnClickedButtonLoadData()
{
	CWaitCursor WaitCursor; 
	MENU_ITEM_DATA ItemData= {0};
	HTREEITEM hItem;
	FILE *F;
	unsigned char *pData;
	int i, DataLen;
	size_t NumOfBytes;
	char *pFN, *pShortFN;
		
	pFN= SelectFileToOpen(m_hWnd, _T("Select a menu file"), _T("menu data files\0*.mdf\0\0"), &pShortFN);
	if (pFN == NULL)
		return;
	
	if (pFN == NULL)
		return;
		
	m_TreeMenu.LoadData(pFN);
//	m_TstMenu.Load(pFN, pShortFN);
	SetRadioBttnState(DST_UNDEFINED);
}

void CDlgMain::OnBnClickedButtonLoadExtMenu()
{
	// TODO: Add your control notification handler code here
}

void CDlgMain::OnBnClickedButtonImportItem()
{
	// TODO: Add your control notification handler code here
}

void CDlgMain::OnBnClickedButtonImportItemData()
{
	// TODO: Add your control notification handler code here
}

void CDlgMain::OnBnClickedButtonExportItem()
{
	// TODO: Add your control notification handler code here
}

void CDlgMain::OnBnClickedButtonImportItemData2()
{
	// TODO: Add your control notification handler code here
}
