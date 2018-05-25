// TreeMenu.cpp : implementation file
//

#include "stdafx.h"
#include "TreeMenu.h"
#include "DlgParamsBuilder.h"
#include "DlgParams.h"
#include "DlgTblBuilder.h"
#include "DlgGraph3DBuilder.h"
#include "DataDescriptorImport.h"

#pragma warning (disable: 4996)

// CTreeMenu

IMPLEMENT_DYNAMIC(CTreeMenu, CTreeCtrl)

CTreeMenu::CTreeMenu()
{
	m_pDlgPlaceHolder= NULL;
	m_pActDlg= NULL;
	OnActDlgChanged= NULL;
	
	m_hIconRoot= AfxGetApp()->LoadIcon(IDI_ICON_R);
	m_hIconParams= AfxGetApp()->LoadIcon(IDI_ICON_P);
	m_hIconTbl= AfxGetApp()->LoadIcon(IDI_ICON_T);
	m_hIcon2D= AfxGetApp()->LoadIcon(IDI_ICON_2D);	
	m_hIcon3D= AfxGetApp()->LoadIcon(IDI_ICON_3D);	
	
	m_ImgList.Create(GetSystemMetrics (SM_CXSMICON), GetSystemMetrics (SM_CYSMICON), (ILC_COLOR32|ILC_MASK), 5, 1 );
	m_ImgList.Add(m_hIconRoot);
	m_ImgList.Add(m_hIconParams); 
	m_ImgList.Add(m_hIconTbl);
	m_ImgList.Add(m_hIcon2D);
	m_ImgList.Add(m_hIcon3D);
}

CTreeMenu::~CTreeMenu()
{
//	ClearMenu();
  m_ImgList.DeleteImageList();
	DestroyIcon(m_hIconRoot);
	DestroyIcon(m_hIconParams); 
	DestroyIcon(m_hIconTbl);
	DestroyIcon(m_hIcon2D);
	DestroyIcon(m_hIcon3D);
}


BEGIN_MESSAGE_MAP(CTreeMenu, CTreeCtrl)
//	ON_NOTIFY_REFLECT(NM_RETURN, &CTreeMenu::OnNMReturn)
//	ON_NOTIFY_REFLECT(TVN_KEYDOWN, &CTreeMenu::OnTvnKeydown)
//	ON_WM_KEYUP()
//ON_NOTIFY_REFLECT(TVN_SELCHANGING, &CTreeMenu::OnTvnSelchanging)
//ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CTreeMenu::OnTvnSelchanged)
//ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CTreeMenu::OnTvnSelchanged)
ON_NOTIFY_REFLECT(NM_CLICK, &CTreeMenu::OnNMClick)
ON_WM_CREATE()
END_MESSAGE_MAP()



// CTreeMenu message handlers

BOOL CTreeMenu::DestroyWindow()
{
//	ClearMenu();
	return CTreeCtrl::DestroyWindow();
}

void CTreeMenu::WriteMenuKey(FILE *F)
{
	int i, l;
	unsigned __int8 DlgType;
	
	l= m_ItemList.size();
	if (l != CTreeCtrl::GetCount())
	{
		ArrangeAllItems();
		l= m_ItemList.size();
	}

	fwrite(&l, 1, 4, F);
	for (i= 0; i < l; i++)
	{
		DlgType= m_ItemList[i].ItemType;
		fwrite(&DlgType, 1, 1, F);
	}
	fflush(F);
}

bool CTreeMenu::CheckMenuKey(FILE *F)
{
	int i, l, ll;
	unsigned __int8 DlgType;
	
	l= m_ItemList.size();
	if (l != CTreeCtrl::GetCount())
	{
		ArrangeAllItems();
		l= m_ItemList.size();
	}

	fread(&ll, 1, 4, F);
	if (ll != l)
		return false;
	for (i= 0; i < l; i++)
	{
		fread(&DlgType, 1, 1, F);
		if (DlgType != m_ItemList[i].ItemType)
			return false;
	}
	return true;
}

int CTreeMenu::ArrangeItemBranch(HTREEITEM hItemToArrange, bool DeleteData)
{
	int i= 0, ItemId= 0, ParentId= 0, ChildId= 0;
	HTREEITEM hItem, hNextItem, hChildItem;
	MENU_ITEM_DATA ItemData= {0};
//	FILE *F;
	
	ItemId= m_ItemList.size();
	i= ItemId;
	
	hItem= hItemToArrange;
	ItemData.hItem= hItem; 
	ItemData.ParentId= -1;
	ItemData.ItemId= ItemId;
	ItemId++;
	ItemData.pDlg= (CDlgData *)GetItemData(hItem);
	if (ItemData.pDlg != NULL)
	{
		ItemData.ItemType= ItemData.pDlg->m_DataType;
/*
		if (DeleteData)
		{
			ItemData.pDlg->CloseWindow();
			delete ItemData.pDlg;
			ItemData.pDlg= NULL;
		}
*/
	}
	else
		ItemData.ItemType= DST_UNDEFINED;
	m_ItemList.push_back(ItemData);
	
	do
	{	
//		ItemData= m_ItemList.back();
//		m_ItemList.pop_back();
		ItemData= m_ItemList[i];
		i++;
		hItem= ItemData.hItem;
		if (ItemHasChildren(hItem))
		{
			ParentId= ItemData.ItemId;
			hChildItem= GetChildItem(hItem);
			while (hChildItem != NULL)
			{
//				FillItemData(F, &ItemData, hChildItem, ItemId, ParentId);
				ItemData.hItem= hChildItem;
				ItemData.ParentId= ParentId;
				ItemData.ItemId= ItemId;
				ItemId++;				
				ItemData.pDlg= (CDlgData *)GetItemData(hChildItem);
				if (ItemData.pDlg != NULL)
				{
					ItemData.ItemType= ItemData.pDlg->m_DataType;
/*
					if (DeleteData)
					{
						ItemData.pDlg->CloseWindow();
						delete ItemData.pDlg;
						ItemData.pDlg= NULL;
					}
*/
				}
				else
					ItemData.ItemType= DST_UNDEFINED;
				m_ItemList.push_back(ItemData);
				hNextItem = GetNextItem(hChildItem, TVGN_NEXT); 
				hChildItem= hNextItem;
			}
		}
	}
	while (i < ItemId); // (!m_ItemList.empty());
	return m_ItemList.size();
}

void CTreeMenu::ArrangeAllItems(bool DeleteData)
{
	HTREEITEM hItem, hNextItem;

	m_ItemList.clear();
	
	hItem= GetRootItem();	
	while (hItem != NULL)
	{
		ArrangeItemBranch(hItem);
		hNextItem = GetNextItem(hItem, TVGN_NEXT); 
		hItem= hNextItem;
	}
}

void CTreeMenu::DeleteItem(HTREEITEM hItemToDelete, BOOL DeleteAll)
{
	MENU_ITEM_DATA ItemData= {0};
	HTREEITEM hItem;
	
  ArrangeItemBranch(hItemToDelete);
  while (!m_ItemList.empty())
  {
		ItemData= m_ItemList.back();
		m_ItemList.pop_back();
		hItem= ItemData.hItem;
		if (ItemData.pDlg != NULL) 
		{
			if (::IsWindow(ItemData.pDlg->m_hWnd))
			{
				ItemData.pDlg->CloseWindow();
				ItemData.pDlg->DestroyWindow();
			}
			delete ItemData.pDlg;
		}
		if (DeleteAll)
			CTreeCtrl::DeleteItem(hItem);
		else
			SetItemData(hItem, 0);
	}
}
/*
void CTreeMenu::ClearData()
{
	int i, ItemId= 0, ParentId= 0, ChildId= 0, ItemCnt;
	HTREEITEM hItem, hNextItem, hChildItem;
	MENU_ITEM_DATA ItemData= {0};
//	FILE *F;
				
	m_ItemList.clear();
	ItemCnt= GetCount();
	if (ItemCnt == 0)
	{
//		::MessageBoxA(m_hWnd, "There are no menu items", "Info", MB_OK);
		return;
	}
	hItem= GetRootItem();	
	do
	{
		hNextItem = GetNextItem(hItem, TVGN_NEXT); 
//		ArrangeItemBranch(hItem);
		DeleteItem(hItem);
		hItem= hNextItem;
	}
	while (hNextItem != NULL);
	
	UpdateWindow();
}
*/
void CTreeMenu::Clear(BOOL DeleteAll)
{
	int i, ItemId= 0, ParentId= 0, ChildId= 0, ItemCnt;
	HTREEITEM hItem, hNextItem, hChildItem;
	MENU_ITEM_DATA ItemData= {0};
//	FILE *F;
				
	m_ItemList.clear();
	ItemCnt= GetCount();
	if (ItemCnt == 0)
	{
//		::MessageBoxA(m_hWnd, "There are no menu items", "Info", MB_OK);
		return;
	}
	hItem= GetRootItem();	
	do
	{
		hNextItem = GetNextItem(hItem, TVGN_NEXT); 
//		ArrangeItemBranch(hItem);
		DeleteItem(hItem, DeleteAll);
		hItem= hNextItem;
	}
	while (hNextItem != NULL);
	
	UpdateWindow();
}

int CTreeMenu::AddItem(TCHAR *ItemName, HTREEITEM hItemParent)
{
	int Rslt= 0;
	HTREEITEM hItem, hNextItem, hChildItem;
	MENU_ITEM_DATA MenuItemData;
	CString cItemName;
	int i, l;
	
	hItem= GetRootItem();	
	while (hItem != NULL)
	{
		ArrangeItemBranch(hItem);
		l= m_ItemList.size();
		for (i= 0; i < l; i++)
		{
			cItemName= GetItemText(m_ItemList[i].hItem);
			if (_tcscmp(cItemName.GetBuffer(), ItemName) == 0)
			{
				Rslt= -1;  
				break;
			}
		}
		m_ItemList.clear();
		if (Rslt == -1)
			return -1;
		hNextItem = GetNextItem(hItem, TVGN_NEXT); 
		hItem= hNextItem;
	}
//	if (Rslt)
	hItem= InsertItem(ItemName, hItemParent);
	if (hItem == NULL)
		return 0;
	SetItemData(hItem, 0);
	if ((hItemParent == 0) || (hItemParent == TVI_ROOT))
	{
		SetItemImage(hItem, MII_ROOT, MII_ROOT);
		Select(hItem, TVGN_CARET);
	}
	else
		Expand(hItemParent, TVE_EXPAND);
	UpdateWindow();
	return 1;
}

void CTreeMenu::SaveItemBranch(FILE *ItemF, FILE *DataF)
{
	CString ItemName;
	MENU_ITEM_DATA ItemData= {0};
	int i, l, ItemDataLen= 0;
	HTREEITEM hItem;
	unsigned char *pData;
	
	ItemDataLen=  sizeof(MENU_ITEM_DATA)- (sizeof(HTREEITEM)+ sizeof(CDlgData *));
	pData= (unsigned char *)&ItemData; //+ sizeof(HTREEITEM)+ sizeof(CDlgData *);

//	hItem= GetRootItem();	
	l= m_ItemList.size();
	for (i= 0; i < l; i++)
//  while (!m_ItemList.empty())
  {
		ItemData= m_ItemList[i];
//		ItemData= m_ItemList.back();
//		m_ItemList.pop_back();
		hItem= ItemData.hItem;
		if (ItemF != NULL)
		{
//			if (ItemData.pDlg != NULL)
//				ItemData.ItemType= (DATA_SET_TYPE)(ItemData.ItemType | DT_NOT_NULL);
			ItemName= GetItemText(hItem);
			strcpy(ItemData.ItemName, ItemName);
			fwrite(pData, 1, ItemDataLen, ItemF);
		}
			if (ItemData.pDlg != NULL)
			{
				if (ItemF != NULL)
  					ItemData.pDlg->vSaveDataDescriptor(ItemF);
				if (DataF != NULL)
 					ItemData.pDlg->vSaveData(DataF);
			}
//			else
//				fwrite(pData, 1, 4, DataF);			
	}
	m_ItemList.clear();
}

void CTreeMenu::Save(FILE *ItemF, FILE *DataF)
{
	int i, ItemId= 0, ParentId= 0, ChildId= 0;
	HTREEITEM hItem, hNextItem, hChildItem;
	MENU_ITEM_DATA ItemData= {0};
//	FILE *F;
				
	m_ItemList.clear();
	if (GetCount() == 0)
	{
//		::MessageBoxA(m_hWnd, "There are no menu items", "Info", MB_OK);
		return;
	}
	ArrangeAllItems();
	if (DataF != NULL)
		WriteMenuKey(DataF);
	SaveItemBranch(ItemF, DataF);
}

void CTreeMenu::_Load(FILE *ItemF, FILE *DataF)
{
	MENU_ITEM_DATA ItemData= {0};
	HTREEITEM hItem, hRootItem= TVI_ROOT;
	unsigned char *pData;
	int i, j, ii, DataLen, ParentId;
	size_t NumOfBytes;
	FILE *F;
	
	Clear(); 

	if (F == 0)
		return;
	pData= (unsigned char *)&ItemData; // + sizeof(HTREEITEM)+ sizeof(CDlgData *);
	DataLen= sizeof(MENU_ITEM_DATA)- (sizeof(HTREEITEM)+ sizeof(CDlgData *));
	while (TRUE)
	{
		NumOfBytes= fread(pData, 1, DataLen, F);
		if (NumOfBytes != DataLen)
			break;
		ItemData.pDlg= NULL;
/*
		switch (m_MenuType)
		{
			case MT_BUILDER:
				if (ItemData.ItemType != DST_UNDEFINED)
				{
					switch (ItemData.ItemType) 
					{
						case DST_PARAMS:
							ItemData.pDlg=  new CDlgParamsBuilder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
						break;
						case DST_2D_TBL:
							ItemData.pDlg=  new CDlgTblBuilder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
						break;
						case DST_1D_GRAPH:
						case DST_2D_GRAPH:
						case DST_3D_GRAPH:
							ItemData.pDlg=  new CDlgGraph3D_Builder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
						break;			
					}
					ItemData.pDlg->Load(F);
				}
			break;
			case MT_WORK:
				switch (ItemData.ItemType) 
				{
					case DST_PARAMS:
						ItemData.pDlg=  new CDlgParams(ItemData.ItemType); // CreateDlg(DST_PARAMS);
						ItemData.pDlg->Load(F);
					break;
					case DST_2D_TBL:
						ItemData.pDlg=  new CDlgTblBuilder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
						ItemData.pDlg->Load(F);
					break;			
					case DST_1D_GRAPH:
					case DST_2D_GRAPH:
					case DST_3D_GRAPH:
						ItemData.pDlg=  new CDlgGraph3D_Builder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
						ItemData.pDlg->Load(F);
					break;				
				}
			break;
		}
*/		
		m_ItemList.push_back(ItemData);
	}
	fclose(F);
	DataLen= m_ItemList.size();
//	hRootItem= InsertItem(pMenuTitle, TVI_ROOT);
	for (i= 0; i < DataLen; i++)
	{
		ItemData= m_ItemList[i];
		if (ItemData.ParentId == -1)
		{
			m_ItemList[i].hItem= InsertItem(ItemData.ItemName, hRootItem);
			ParentId= -1;
			j= i;
		}
		else
		{
/*
			if (m_ItemList[i].ParentId != ParentId)
			do
			{
				if (m_ItemList[j].ItemId == m_ItemList[i].ParentId)
				{
					ParentId= m_ItemList[i].ParentId;
					break;
				}
			}
			while (j != i)
*/
			m_ItemList[i].hItem= InsertItem(ItemData.ItemName, m_ItemList[m_ItemList[i].ParentId].hItem);
		}
//		if (m_ItemList[i].pDlg != NULL)
//			SetItemData(m_ItemList[i].hItem, (DWORD_PTR)m_ItemList[i].pDlg);
	}
//	m_ItemList.clear();
	Expand(hRootItem, TVE_EXPAND);
}

void CTreeMenu::LoadItems(char *FN, char *pMenuTitle)
{
	MENU_ITEM_DATA ItemData= {0};
	HTREEITEM hItem, hRootItem= TVI_ROOT;
	unsigned char *pData;
	int i, j, ii, DataLen, ParentId;
	size_t NumOfBytes;
	FILE *F;
	
	Clear(); 

	F= fopen(FN, "rb");
	if (F == 0)
		return;
	pData= (unsigned char *)&ItemData; // + sizeof(HTREEITEM)+ sizeof(CDlgData *);
	DataLen= sizeof(MENU_ITEM_DATA)- (sizeof(HTREEITEM)+ sizeof(CDlgData *));
	while (TRUE)
	{
		NumOfBytes= fread(pData, 1, DataLen, F);
		if (NumOfBytes != DataLen)
			break;
		ItemData.pDlg= NULL;
/*		switch (m_MenuType)
		{

			case MT_BUILDER:
				if (ItemData.ItemType != DST_UNDEFINED)
				{
					switch (ItemData.ItemType) 
					{
						case DST_PARAMS:
							ItemData.pDlg=  new CDlgParamsBuilder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
						break;
						case DST_2D_TBL:
							ItemData.pDlg=  new CDlgTblBuilder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
						break;
						case DST_1D_GRAPH:
						case DST_2D_GRAPH:
						case DST_3D_GRAPH:
							ItemData.pDlg=  new CDlgGraph3D_Builder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
						break;			
					}
					ItemData.pDlg->Load(F);
				}
			break;
			case MT_WORK:
				switch (ItemData.ItemType) 
				{
					case DST_PARAMS:
						ItemData.pDlg=  new CDlgParams(ItemData.ItemType); // CreateDlg(DST_PARAMS);
						ItemData.pDlg->Load(F);
					break;
					case DST_2D_TBL:
						ItemData.pDlg=  new CDlgTblBuilder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
						ItemData.pDlg->Load(F);
					break;			
					case DST_1D_GRAPH:
					case DST_2D_GRAPH:
					case DST_3D_GRAPH:
						ItemData.pDlg=  new CDlgGraph3D_Builder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
						ItemData.pDlg->Load(F);
					break;				
				}
			break;
		}		
*/		
		m_ItemList.push_back(ItemData);
	}
	fclose(F);
	DataLen= m_ItemList.size();
//	hRootItem= InsertItem(pMenuTitle, TVI_ROOT);
	for (i= 0; i < DataLen; i++)
	{
//		ItemData= m_ItemList[i];
		if (m_ItemList[i].ParentId == -1)
		{
			m_ItemList[i].hItem= InsertItem(m_ItemList[i].ItemName, hRootItem);
			ParentId= -1;
			j= i;
		}
		else
		{
			m_ItemList[i].hItem= InsertItem(m_ItemList[i].ItemName, m_ItemList[m_ItemList[i].ParentId].hItem);
		}
		if (m_ItemList[i].ItemType != DST_UNDEFINED)
		{
			m_ItemList[i].pDlg= CreateDlg(m_ItemList[i].ItemType);
			if (m_ItemList[i].pDlg != NULL)
				SetItemData(m_ItemList[i].hItem, (DWORD_PTR)m_ItemList[i].pDlg);
			else
				SetItemData(m_ItemList[i].hItem, (DWORD_PTR)0);
		}
	}
//	m_ItemList.clear();
	Expand(hRootItem, TVE_EXPAND);
	::SetWindowTextA(m_hWndMenuName, pMenuTitle);
}

void CTreeMenu::Import(TCHAR *pFN)
{
	MENU_ITEM_DATA ItemData= {0};
	long l, i= 0;
	char *pSym, *pItemStr, *pData, *pValStr, *pVal;
	FILE *F;
 
#ifdef _UNICODE	
	F= _wfopen(pFN, L"rb");
#else
	F= fopen(pFN, "rb");
#endif	
 fseek(F, 0, SEEK_END);
 l= ftell(F);
 fseek(F, 0, SEEK_SET);
 pData= (char *)malloc(l+ 1);
 fread(pData, 1, l, F);
 pData[l]= 0;
 
	bool IsEOL= false;
	bool IsComment= false;
	bool IsFound= false;
	HTREEITEM hRootItem= NULL, hItem;
    CDlgParamsBuilder *pDlgParams= NULL;
    PARAM_DATA ParamData= {0};
    CDlgTblBuilder *pDlgTbl= NULL;
 
	char *pTableEditor= NULL;
	char *pUserDefined= NULL;
	char *pCurveEditor= NULL;
	char *pConstants= NULL;
	char *pMenu= NULL;
	pTableEditor= strstr(pData, "[TableEditor]");
	pUserDefined= strstr(pData, "[UserDefined]");
	pCurveEditor= strstr(pData, "[CurveEditor]");
	pConstants=		strstr(pData, "[Constants]");
	pMenu= strstr(pData, "[Menu]");
	pSym= pMenu;
	if (pSym == NULL)
		goto L_EXIT;
        
        
	m_ItemList.clear();    
#define pStr pSym
// find menu items
	do
	{
		GOTO_NEXT_LINE(pSym)
		if (strncmp(pSym, "subMenu", 7) == 0)
		{
			SKIP_TAG(pSym, 7)
			GET_VAL(pSym)
			GET_STR_VAL(pSym)
			if (pValStr == NULL)
					continue;
			hItem= InsertItem(pValStr, hRootItem);
			ItemData.hItem= hItem;
			ItemData.pDlg= (CDlgData *)pVal;
			m_ItemList.push_back(ItemData);
//           SetItemData(hItem, (DWORD_PTR)pVal);
		}
		else
			if (strncmp(pSym, "menu", 4) == 0)
				{
						SKIP_TAG(pSym, 7)
						GET_STR_VAL(pSym)
						hRootItem= InsertItem(pValStr, TVI_ROOT);
					}
					else 
						break;        
	}
	while (*pSym != 0);   
//    goto L_EXIT;
// find param dialog items   

	l= m_ItemList.size();
//	pSym= strstr(pData, "[UserDefined]");
	if (pUserDefined != NULL) 
	{
		pSym= pUserDefined;
	do
	{
		GOTO_NEXT_LINE(pSym)
		if (strncmp(pSym, "dialog", 6) == 0)
		{
			pDlgParams= NULL;
			SKIP_TAG(pSym, 7)
			GET_VAL(pSym)
			GET_STR_VAL(pSym)
			for (i= 0; i < l; i++)
			{
//				pValStr= (char *)m_ItemList[i].pDlg;
				if (strcmp(pVal, (char *)m_ItemList[i].pDlg) == 0)
				{
					pDlgParams= new CDlgParamsBuilder;
					strcpy(pDlgParams->m_Title, pValStr);
					SetItemData(m_ItemList[i].hItem, (DWORD_PTR)pDlgParams);
					break;
				}
			}
		}
		else
			if (strncmp(pSym, "field", 5) == 0)
			{
			    memset(&ParamData, 0, sizeof(PARAM_DATA));
				SKIP_TAG(pSym, 5)
				GET_STR_VAL(pSym)
				SKIP_WHITE_SPACE(pSym)
				GET_VAL(pSym)
				if (pValStr != NULL)
					strcpy(ParamData.Name, pValStr);
				if (pVal == NULL)
					ParamData.Type= FVT_UNDEFINED;
				else
				{
//					pItemStr= strstr(pConstants, pVal);
					FIND_SUB_STR(pConstants, pMenu, pVal, strlen(pVal), pItemStr)
					if ((pItemStr != NULL) && (pItemStr < pMenu))
					{
						SKIP_TAG(pItemStr, strlen(pVal))
//						SKIP_WHITE_SPACE(pItemStr)
						GET_VAL(pItemStr)
						if (pVal != NULL)
						{
							if (strcmp(pVal, "scalar") == 0)
							{
								GET_VAL(pItemStr)
								if (*pVal == 'U')
								{
								if (strcmp(ParamData.Name, "MAP Sensor Offset") == 0)
									MessageBeep(0);
									*(WORD *)(pVal+ 1)-= 0x3030;
									ParamData.Type= (FIELD_VAL_TYPE)(*(pVal+ 1)* 10+ *(pVal+ 2));
									GET_NUM_VAL(pItemStr)
									ParamData.Offset= atol(pVal);
									GET_STR_VAL(pItemStr)
									if (pValStr != NULL)
										strcpy(ParamData.Units, pValStr);
									GET_NUM_VAL(pItemStr)
									ParamData.Multiplier= atof(pVal);
									GET_NUM_VAL(pItemStr)
									if (pVal != NULL)
										ParamData.Addition= atof(pVal);
									GET_NUM_VAL(pItemStr)
									ParamData.MinValue= atof(pVal);
									GET_NUM_VAL(pItemStr)
									ParamData.MaxValue= atof(pVal);
									GET_NUM_VAL(pItemStr)
									ParamData.NumOfDecimalPlaces= atoi(pVal);
								}
							}
							else
								if (strcmp(pVal, "bits") == 0)
								{
									ParamData.Type= FVT_VAL_BITS;
									GET_VAL(pItemStr)
									if (*pVal == 'U')
									{
										*(WORD *)(pVal+ 1)-= 0x3030;
										ParamData.Type= (FIELD_VAL_TYPE)(*(pVal+ 1)* 10+ *(pVal+ 2));
										GET_NUM_VAL(pItemStr)
										ParamData.Offset= atol(pVal);
//				                        GET_VAL(pItemStr)
										SKIP_WHITE_SPACE(pItemStr)
										if (*pItemStr == '[')
										{
											pItemStr++;
											GET_NUM_VAL(pItemStr)
											ParamData.MinValue= atoi(pVal);
											GET_NUM_VAL(pItemStr)
											ParamData.MaxValue= atoi(pVal);
											for (i= 0; i < 16; i++)
											{
												GET_STR_VAL(pItemStr)
												if (pValStr == NULL)
												break;
												strcpy(ParamData.BitStrList[i], pValStr);				                            }
											}
								   }
								}
						}
					}
				}
				if (pDlgParams != NULL)
					pDlgParams->ListParams.push_back(ParamData);
			}
			else break;
	}
	while (*pSym != 0);   
	} 
// tables

//	pSym= strstr(pData, "[TableEditor]");
	if (pTableEditor != NULL) 
	{
		pSym= pTableEditor;
	do
	{
		GOTO_NEXT_LINE(pSym)
		if (strncmp(pSym, "table", 5) == 0)
		{
			pDlgTbl= NULL;
			SKIP_TAG(pSym, 7)
			GET_VAL(pSym)
			SKIP_UNTIL_SYM(pSym, '"')
			GET_STR_VAL(pSym)
			for (i= 0; i < l; i++)
			{
//				pValStr= (char *)m_ItemList[i].pDlg;
				if (strcmp(pVal, (char *)m_ItemList[i].pDlg) == 0)
				{
					pDlgTbl= new CDlgTblBuilder; 
					strcpy(pDlgTbl->m_Title, pValStr);
					SetItemData(m_ItemList[i].hItem, (DWORD_PTR)pDlgTbl);
					break;
				}
			}
		}
	}		
	while (*pSym != 0);    
	}
//	pSym= strstr(pData, "[CurveEditor]");
	if (pCurveEditor != NULL) 
	{
		pSym= pCurveEditor;
	do
	{
		GOTO_NEXT_LINE(pSym)
		if (strncmp(pSym, "curve", 5) == 0)
		{
			pDlgTbl= NULL;
			SKIP_TAG(pSym, 7)
			GET_VAL(pSym)
			SKIP_UNTIL_SYM(pSym, '"')
			GET_STR_VAL(pSym)
			for (i= 0; i < l; i++)
			{
//				pValStr= (char *)m_ItemList[i].pDlg;
				if (strcmp(pVal, (char *)m_ItemList[i].pDlg) == 0)
				{
					pDlgTbl= new CDlgTblBuilder; 
					strcpy(pDlgTbl->m_Title, pValStr);
					SetItemData(m_ItemList[i].hItem, (DWORD_PTR)pDlgTbl);
					break;
				}
			}
		}
	}		
	while (*pSym != 0);    
	}
L_EXIT:
 //   GlobalUnlock(hClBrData); 
 //   CloseClipboard();
    free(pData);
}

void CTreeMenu::LoadData(char *FN)
{
	MENU_ITEM_DATA ItemData= {0};
	HTREEITEM hItem, hRootItem= TVI_ROOT;
	unsigned char *pData;
	int i, l, DataLen;
	size_t NumOfBytes;
	FILE *F;
	CDlgData *pDlg;
	
	F= fopen(FN, "rb");
	if (F == 0)
		return;
	pData= (unsigned char *)&ItemData; // + sizeof(HTREEITEM)+ sizeof(CDlgData *);
	DataLen= sizeof(MENU_ITEM_DATA)- (sizeof(HTREEITEM)+ sizeof(CDlgData *));

//	hRootItem= InsertItem(pMenuTitle, TVI_ROOT);

	ArrangeAllItems();
	l= m_ItemList.size();
  
  if (!CheckMenuKey(F))
  {
		::MessageBox(m_pDlgPlaceHolder->m_hWnd, _T("Wrong data file"), _T("Data Error"), MB_ICONEXCLAMATION | MB_OK);
		goto L_EXIT;
  }
	for (i= 0; i < l; i++)
	{
		pDlg= m_ItemList[i].pDlg;
		if (pDlg == NULL)	continue;
		if (::IsWindow(pDlg->m_hWnd))
		{
			pDlg->CloseWindow();
			pDlg->DestroyWindow();
		}
		delete pDlg;
		pDlg= CreateDlg(m_ItemList[i].ItemType);
		SetItemData(m_ItemList[i].hItem, (DWORD_PTR)pDlg);
		pDlg->vLoadData(F);
	}
L_EXIT:	
	fclose(F);
	return;
		
	DataLen= m_ItemList.size();
	for (i= 0; i < DataLen; i++)
	{
		ItemData= m_ItemList[i];
//		if ((ItemData.ItemType & DT_NOT_NULL) != 0)
		{
//			ItemData.ItemType= (DATA_SET_TYPE)(ItemData.ItemType & (~DT_NOT_NULL));
/*
			switch (m_MenuType)
			{
				case MT_BUILDER:
					if (ItemData.ItemType != DST_UNDEFINED)
					{
						switch (ItemData.ItemType) 
						{
							case DST_PARAMS:
								ItemData.pDlg=  new CDlgParamsBuilder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
							break;
							case DST_2D_TBL:
								ItemData.pDlg=  new CDlgTblBuilder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
							break;
							case DST_1D_GRAPH:
							case DST_2D_GRAPH:
							case DST_3D_GRAPH:
								ItemData.pDlg=  new CDlgGraph3D_Builder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
							break;			
						}
						ItemData.pDlg->Load(F);
					}
				break;
				case MT_WORK:
					switch (ItemData.ItemType) 
					{
						case DST_PARAMS:
							ItemData.pDlg=  new CDlgParams(ItemData.ItemType); // CreateDlg(DST_PARAMS);
							ItemData.pDlg->Load(F);
						break;
						case DST_2D_TBL:
							ItemData.pDlg=  new CDlgTblBuilder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
							ItemData.pDlg->Load(F);
						break;			
						case DST_1D_GRAPH:
						case DST_2D_GRAPH:
						case DST_3D_GRAPH:
							ItemData.pDlg=  new CDlgGraph3D_Builder(ItemData.ItemType); // CreateDlg(DST_PARAMS);
							ItemData.pDlg->Load(F);
						break;				
					}
				break;
			}
*/
		}
		if (ItemData.pDlg != NULL)
		{
//			ItemData.pDlg->Load(F);
			SetItemData(m_ItemList[i].hItem, (DWORD_PTR)m_ItemList[i].pDlg);
		}
	}
}

void CTreeMenu::ShowDlg()
{
/*
		if (pActDlg->m_hWnd == NULL)
//			SetActBuildDlg(pActDlg->m_DataType);
			CreateBuildDlg(pActDlg->m_DataType);
		else
		{
			SetRadioBttnState(pActDlg->m_DataType);
			pActDlg->ShowWindow(SW_RESTORE);
*/
}

void CTreeMenu::HideDlg()
{
}

CDlgData *CTreeMenu::CreateDlg(DATA_SET_TYPE DlgType) // , bool ShowDlg)
{
	CDlgData *pDlg;
	int ImgInd;
	
  switch (DlgType)
  {
		case DST_PARAMS:
			pDlg= new CDlgParamsBuilder;
			ImgInd= MII_PARAMS;
//			if (ShowDlg)
//				pDlg->Create(CDlgParamsBuilder::IDD, m_pDlgPlaceHolder);
		break;
		case DST_2D_TBL:
			pDlg= new CDlgTblBuilder;
			ImgInd= MII_TBL;
//			if (ShowDlg)
//				pDlg->Create(CDlgTblBuilder::IDD, m_pDlgPlaceHolder);
		break;
		case DST_1D_GRAPH:
			pDlg= new CDlgGraph3D_Builder;
			ImgInd= MII_2D;
//			if (ShowDlg)
//				pDlg->Create(CDlgGraph3D_Builder::IDD, m_pDlgPlaceHolder);
		break;
		case DST_2D_GRAPH:
			pDlg= new CDlgGraph3D_Builder;
			ImgInd= MII_2D;
//			if (ShowDlg)
//				pDlg->Create(CDlgGraph3D_Builder::IDD, m_pDlgPlaceHolder);
		break;
		case DST_3D_GRAPH:
			pDlg= new CDlgGraph3D_Builder;
			ImgInd= MII_3D;
//			if (ShowDlg)
//				pDlg->Create(CDlgGraph3D_Builder::IDD, m_pDlgPlaceHolder);
		break;
		default:
			return NULL;
	}
//	m_pActDlg->Create(m_pActDlg->IDD, m_pDlgPlaceHolder);
//	m_pActDlg->ShowWindow(SW_SHOW);
	SetItemImage(m_hSelItem, ImgInd, ImgInd);
	return pDlg;
}

void CTreeMenu::ChangeActDlgType(DATA_SET_TYPE DlgType)
{
//	bool Rslt= false;
	
	if (m_pActDlg != NULL)
	{
		if (::IsWindow(m_pActDlg->m_hWnd))
		{
			m_pActDlg->CloseWindow();
			m_pActDlg->DestroyWindow();
		}
		delete m_pActDlg;
//		SetItemData(m_hSelItem, (DWORD_PTR)NULL);
	}
	m_pActDlg= CreateDlg(DlgType);
	SetItemData(m_hSelItem, (DWORD_PTR)m_pActDlg);
	ActivateDlg();
}

void CTreeMenu::ActivateDlg()
{
	DATA_SET_TYPE DlgType= DST_UNDEFINED;
	
	if ((m_pActDlg != NULL) && ::IsWindow(m_pActDlg->m_hWnd))
		m_pActDlg->ShowWindow(SW_HIDE);	

	m_pActDlg= (CDlgData *)GetItemData(m_hSelItem);
	if (m_pActDlg != NULL)
	{
		DlgType= m_pActDlg->m_DataType;
		if (::IsWindow(m_pActDlg->m_hWnd))
			m_pActDlg->ShowWindow(SW_RESTORE);
		else
		{
		  switch (DlgType)
			{
				case DST_PARAMS:
//					pDlg= new CDlgParamsBuilder(DlgType);
					m_pActDlg->Create(CDlgParamsBuilder::IDD, m_pDlgPlaceHolder);
				break;
				case DST_2D_TBL:
//					pDlg= new CDlgTblBuilder(DlgType);
					m_pActDlg->Create(CDlgTblBuilder::IDD, m_pDlgPlaceHolder);
				break;
				case DST_1D_GRAPH:
//					pDlg= new CDlgGraph3D_Builder(DlgType);
					m_pActDlg->Create(CDlgGraph3D_Builder::IDD, m_pDlgPlaceHolder);
				break;
				case DST_2D_GRAPH:
//					pDlg= new CDlgGraph3D_Builder(DlgType);
					m_pActDlg->Create(CDlgGraph3D_Builder::IDD, m_pDlgPlaceHolder);
				break;
				case DST_3D_GRAPH:
//					pDlg= new CDlgGraph3D_Builder(DlgType);
					m_pActDlg->Create(CDlgGraph3D_Builder::IDD, m_pDlgPlaceHolder);
				break;
			}
		}
	}
	else
	{
// no item dlg	
		MessageBeep(0);
//		CreateActDlg(m_pActDlg->m_DataType);
	}
	if (OnActDlgChanged != NULL)
		OnActDlgChanged(DlgType);
//	::SendMessage(::GetParent(m_hWnd), WM_ACT_DLG, DlgType, 0);	
}

BOOL CTreeMenu::PreTranslateMessage(MSG* pMsg)
{
NMTREEVIEW TreeView;
HTREEITEM hItem;

    if (WM_KEYDOWN == pMsg->message)
    {
//			if (m_ReadOnly)
//        return TRUE;        

        if (VK_RETURN == pMsg->wParam) 
        {
//					pMsg->wParam= 0;
//					pMsg->lParam= 0;
//					MessageBeep(0);
					if (m_MenuType == MT_WORK)
					{
/*
						memset(&TreeView, 0, sizeof(NMTREEVIEW));
						TreeView.hdr.idFrom= ::GetDlgCtrlID(m_hWnd);
						TreeView.hdr.hwndFrom= m_hWnd;
						TreeView.hdr.code= NM_CLICK;
	//					TreeView.action= MT_WORK; // TVN_SELCHANGED 
						TreeView.itemNew.hItem= this->GetSelectedItem();
					::SendMessage(::GetParent(m_hWnd), WM_SHOW_DLG, m_MenuType, 0);
					::SendMessage(::GetParent(m_hWnd), WM_NOTIFY, TreeView.hdr.idFrom, (LPARAM)&TreeView);
//						::PostMessage(::GetParent(m_hWnd), WM_NOTIFY, TreeView.hdr.idFrom, (LPARAM)&TreeView);
*/
						hItem= GetSelectedItem();
						if (hItem != NULL)
						{
							m_hSelItem= hItem;
							ActivateDlg();
						}
					}
					return 0;
        } // if

        if(VK_ESCAPE == pMsg->wParam)
        {
					pMsg->wParam= 0;
					pMsg->lParam= 0;
 //           EndEdit();
        } // if
    } // if
    
    if (pMsg->message == WM_COMMAND)
    {
			MessageBeep(0);
    }

	return CTreeCtrl::PreTranslateMessage(pMsg);
}
/*
BOOL CTreeMenu::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	*pResult = 1;
	
	return CTreeCtrl::OnNotify(wParam, lParam, pResult);
}
*/
/*
void CTreeMenu::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	m_hSelItem= pNMTreeView->itemNew.hItem;
	*pResult = 0;
}
*/

//void CTreeMenu::OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
//	{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//	}


void CTreeMenu::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	
//	MessageBox("gfdsj", "gjg");
	*pResult = 0;
	
//	HTREEITEM hItem= pNMTreeView->itemNew;

//	::SendMessage(::GetParent(m_hWnd), WM_SHOW_DLG, m_MenuType, 0);

}

void CTreeMenu::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
  HTREEITEM hItem; 
	UINT unFlags = 0;
  POINT P;
  
	::GetCursorPos(&P);
	ScreenToClient(&P);
	hItem = HitTest(P, &unFlags);

	if (hItem == NULL)
		return;

	if (unFlags & TVHT_ONITEMLABEL) //TVHT_ONITEMBUTTON)
	{	
		m_hSelItem= hItem;
		ActivateDlg();
/*
		if (pNMTreeView->action & TVN_ITEMEXPANDING) // 6044
		{
			if ((GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED) == 0)
			{
			}
		} 
*/
	}

	*pResult = 0;
}

int CTreeMenu::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CTreeMenu::AttachImageList()
{
		SetImageList(&m_ImgList, LVSIL_NORMAL);

}

