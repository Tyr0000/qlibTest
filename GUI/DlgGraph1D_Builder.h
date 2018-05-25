#include "DlgGraph1D_Descriptor.h"
#include "FListCtrl.h"

#pragma once


// CDlgGraph1D_Builder dialog

class CDlgGraph1D_Builder : public CDlgGraph1D_Descriptor
{
	DECLARE_DYNAMIC(CDlgGraph1D_Builder)

public:
	CDlgGraph1D_Builder(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGraph1D_Builder();

// Dialog Data
	enum { IDD = IDD_DLG_GRAPH_1D_BUILDER };

protected:
	CFListCtrl m_ListPointNames;
	bool _UpdateDataDescriptorFromGUI();
	void _DsplDataDescriptor();

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
