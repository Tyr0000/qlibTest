#pragma once


// CDlgData1DGraph dialog

class CDlgData1DGraph : public CDialog
{
	DECLARE_DYNAMIC(CDlgData1DGraph)

public:
	CDlgData1DGraph(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgData1DGraph();

// Dialog Data
	enum { IDD = IDD_DLGDATA1DGRAPH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
