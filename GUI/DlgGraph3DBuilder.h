#include "DlgData.h"
//#include "DataDescriptor.h"
#include "C3dRenderer.h"

#pragma once


// CDlgGraph3D_Builder dialog

class CDlgGraph3D_Builder : public CDlgData
{
	DECLARE_DYNAMIC(CDlgGraph3D_Builder)

public:
	CDlgGraph3D_Builder(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGraph3D_Builder();

// Dialog Data
	enum { IDD = IDD_DLG_GRAPH_1D_BUILDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();

protected:
	DATA_3D_DESCRIPTOR m_DataDescriptor;

	C3dRenderer m_Renderer;
//#define X_POINT(IndX)	pDataX[IndX]
public:

	void Save(FILE *F);
	void Load(FILE *F);
	void Load3D(FILE *F);
	void ClearData();
	virtual void vSaveDataDescriptor(FILE *F);
	virtual void vLoadDataDescriptor(FILE *F);
//	virtual void vSaveData(FILE *F);
//	virtual void vLoadData(FILE *F);
	virtual void vDsplDataDescriptor();

public:
	afx_msg void OnBnClickedButtonLoad();
};
