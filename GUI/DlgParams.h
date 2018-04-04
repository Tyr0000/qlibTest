#include "DlgParamsBuilder.h"

#pragma once


// CDlgParams dialog

class CDlgParams : public CDlgParamsBuilder
{
	DECLARE_DYNAMIC(CDlgParams)

public:
	CDlgParams(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgParams();

// Dialog Data
	enum { IDD = IDD_DLG_DUMMY};

public:
// void Load(FILE *F);
	std::vector<CWnd *> ListCntrl; 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	};
