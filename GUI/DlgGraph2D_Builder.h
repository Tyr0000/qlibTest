#include "DlgData.h"
// #include "DataDescriptor.h"

#pragma once


// CDlgGraph2D_Builder dialog

class CDlgGraph2D_Builder : public CDlgData
{
	DECLARE_DYNAMIC(CDlgGraph2D_Builder)

public:
	CDlgGraph2D_Builder(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGraph2D_Builder();

// Dialog Data
	enum { IDD = IDD_DLG_GRAPH_1D_BUILDER };

protected:
	DATA_2D_DESCRIPTOR m_DataDescriptor;

public:

	virtual void vSaveDataDescriptor(FILE *F);
	virtual void vLoadDataDescriptor(FILE *F);
	virtual void vSaveData(FILE *F);
	virtual void vLoadData(FILE *F);
	virtual void vDsplDataDescriptor();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
