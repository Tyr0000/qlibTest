#include "resource.h"
#include <tchar.h>
#include <vector>
#include "DataDescriptor.h"

#pragma once

#define MAX_CAPTION_LEN			50

typedef TCHAR			MENU_STR[MAX_CAPTION_LEN+ 1];
/*
typedef enum 
{
	DST_UNDEFINED= 0,
	DST_PARAMS= 1,
	DST_1D= 2,
	DST_1D_GRAPH= 3,
	DST_2D= 4,
	DST_2D_TBL,
	DST_2D_GRAPH,
	DST_3D= 8,	
	DST_3D_GRAPH
}
DATA_SET_TYPE;

typedef enum 
{
	DPL_UNDEFINED= 0,
	DPL_8= 1,
	DPL_16= 2,
	DPL_32= 4
}
DATA_PTR_LEN;
*/

// CDlgData dialog

class CDlgData : public CDialog
{
	DECLARE_DYNAMIC(CDlgData)

public:
	CDlgData(DATA_SET_TYPE DataType, UINT ResId, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgData();

// Dialog Data
	enum { IDD = IDD_DLG_DUMMY };

public:
// data descriptor
	DATA_SET_TYPE m_DataType;
	DATA_PTR_LEN m_DataPtrLen;
protected:
//	unsigned __int32 m_DataOffset; 
	unsigned __int32 m_MinVal;
	unsigned __int32 m_MaxVal;
	std::vector<unsigned __int32> DataOffsetList;
//	unsigned __int32 m_DataLen;
//	unsigned __int8 *m_pData;
//	unsigned __int32 *m_pData32;
	bool _UpdateDataDescriptorFromGUI();
	void _DsplDataDescriptor();
	void _SaveDataDescriptor(FILE *F);
	void _LoadDataDescriptor(FILE *F);
public:
	virtual void vSaveDataDescriptor(FILE *F);
	virtual void vLoadDataDescriptor(FILE *F);
//	virtual void vUpdateDescriptorGUI();
	virtual void vDsplDataDescriptor();

	virtual void vSaveData(FILE *F);
	virtual void vLoadData(FILE *F);
//	void SaveData(FILE *F);
//	void LoadData(FILE *F);
	bool LoadDataFromExtFile();
	bool LoadDataFromExtFile(FILE *F, unsigned int FileOffset);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
};
