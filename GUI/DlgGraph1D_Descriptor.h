#include "DlgData.h"
#include <vector>

#pragma once

typedef struct _POINT_NAME
{
	unsigned __int16 StrLen;
	MENU_STR	tStr;
}
POINT_NAME;


// DlgGraph1D_Descriptor dialog

class CDlgGraph1D_Descriptor : public CDlgData
{
	DECLARE_DYNAMIC(CDlgGraph1D_Descriptor)

public:
	CDlgGraph1D_Descriptor(UINT ResId, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGraph1D_Descriptor();

// Dialog Data
	enum { IDD = IDD_DLG_DUMMY };

protected:
	unsigned int m_NumOfPoints;
	std::vector<POINT_NAME> m_PointNamesList;
	void LoadDataDescriptor(FILE *F);

protected:


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
