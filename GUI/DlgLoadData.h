#include "resource.h"		// main symbols

#pragma once


// CDlgLoadData dialog

class CDlgLoadData : public CDialog
{
	DECLARE_DYNAMIC(CDlgLoadData)

public:
	CDlgLoadData(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLoadData();

// Dialog Data
	enum { IDD = IDD_DLG_LOAD_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoad();
	virtual BOOL OnInitDialog();
	};
