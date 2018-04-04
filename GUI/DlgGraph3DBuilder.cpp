// DlgGraph3DBuilder.cpp : implementation file
//

#include "stdafx.h"
#include "DymanicMenu.h"
#include "DlgGraph3DBuilder.h"
#include "u_File.h"


// CDlgGraph3D_Builder dialog

IMPLEMENT_DYNAMIC(CDlgGraph3D_Builder, CDlgData)

CDlgGraph3D_Builder::CDlgGraph3D_Builder(CWnd* pParent /*=NULL*/)
	: CDlgData(DST_1D_GRAPH, CDlgGraph3D_Builder::IDD, pParent)
{
}

CDlgGraph3D_Builder::~CDlgGraph3D_Builder()
{
	ClearData();
}

void CDlgGraph3D_Builder::DoDataExchange(CDataExchange* pDX)
{
	CDlgData::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgGraph3D_Builder, CDlgData)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CDlgGraph3D_Builder::OnBnClickedButtonLoad)
END_MESSAGE_MAP()


// CDlgGraph3D_Builder message handlers

BOOL CDlgGraph3D_Builder::OnInitDialog()
{
/*
	m_Renderer.AttachWnd(GetDlgItem(IDC_STATIC_PLACEHOLDER_DLG)->m_hWnd);
	
	if ((m_NumOfDataX != 0) && (m_NumOfDataY != 0) && (m_NumOfDataZ != NULL) && (m_Renderer.m_hWnd != NULL))
	{
	m_Renderer.m_MaxValY= 0x90;
	m_Renderer.m_MaxValX= 14; //0x9C;
	m_Renderer.m_MaxValZ= 21; 0x71;
	m_Renderer.m_NumOfGridMarkX= 14; 
	m_Renderer.m_NumOfGridMarkY= 14;
	m_Renderer.m_NumOfGridMarkZ= 21;
	m_Renderer.Init();
	m_Renderer.DrawGrid();
	
	HPEN hPen;
	
	hPen= CreatePen(PS_SOLID, 2, 0x00F00000);
	SelectObject(m_Renderer.m_hDcMem, hPen);

	int X= 0, Y= 0, Z= 0, x0=-1, y0= -1, x1= -1, y1= -1;
	for (Y= 0; Y < m_NumOfDataY; Y++)
//Z= 13;	
	{
		x1= -1; y1= -1;
		for (X= 0; X < m_NumOfDataX; X++)
//X= 0;
		{
//			PlotXYZ(DataX[X], DataY[(int)Z][(int)X], DataZ[Z], &x0, &y0);
			m_Renderer.PlotXYZ(Y, m_pDataZ[Z* m_NumOfDataX+ X], X, &x0, &y0);
			if (x1 == -1)
			{
				::MoveToEx(m_Renderer.m_hDcMem, x0, y0, NULL);
				x1= x0; y1= x0; 
			}
			else LineTo(m_Renderer.m_hDcMem, x0, y0);

//			PlotXYZ(DataZ[Z], DataY[(int)Z][(int)X], DataX[X]);
//			PlotXYZ(DataX[X], DataZ[Z], DataY[(int)Z][(int)X]);
//			PlotXYZ(DataZ[Z], DataX[X], DataY[(int)Z][(int)X]);
//			PlotXZ(X, DataZ[Z]);
		}
	}
		for (X= 0; X < m_NumOfDataX; X++)
//Z= 13;	
	{
		x1= -1; y1= -1;
	for (Y= 0; Y < m_NumOfDataY; Y+= 1)
//X= 0;
		{
//			PlotXYZ(DataX[X], DataY[(int)Z][(int)X], DataZ[Z], &x0, &y0);
			m_Renderer.PlotXYZ(Y, m_pDataZ[Z* m_NumOfDataX+ X], X, &x0, &y0);
			if (x1 == -1)
			{
				::MoveToEx(m_Renderer.m_hDcMem, x0, y0, NULL);
				x1= x0; y1= x0; 
			}
			else LineTo(m_Renderer.m_hDcMem, x0, y0);

//			PlotXYZ(DataZ[Z], DataY[(int)Z][(int)X], DataX[X]);
//			PlotXYZ(DataX[X], DataZ[Z], DataY[(int)Z][(int)X]);
//			PlotXYZ(DataZ[Z], DataX[X], DataY[(int)Z][(int)X]);
//			PlotXZ(X, DataZ[Z]);
		}
	}	
	DeleteObject(hPen);
	::BitBlt(m_Renderer.m_hDcWnd, 0, 0, m_Renderer.m_Width, m_Renderer.m_Height, m_Renderer.m_hDcMem, 0, 0, SRCCOPY);
	}
*/	
	return TRUE;
}

void CDlgGraph3D_Builder::ClearData()
{
}

void CDlgGraph3D_Builder::Save(FILE *F)
{
	int i;

	m_DataDescriptor.Save(F);

/*		
	for (i= 0; i < m_NumOfDataX; i++)
	{
		fwrite(&m_pDataX[i], 1, m_ptrLen, F);
	}
	for (i= 0; i < m_NumOfDataY; i++)
	{
		fwrite(&m_pDataY[i], 1, m_ptrLen, F);
	}
	for (i= 0; i < m_NumOfDataZ; i++)
	{
		fwrite(&m_pDataZ[i], 1, m_ptrLen, F);
	}
*/
}

void CDlgGraph3D_Builder::Load(FILE *F)
{
	switch (m_DataType)
	{
		case DST_1D_GRAPH:
		break;
		case DST_2D_GRAPH:
		break;
		case DST_3D_GRAPH:
			Load3D(F);
		break;
	}	
}

void CDlgGraph3D_Builder::Load3D(FILE *F)
{
	int i;
	
	m_DataDescriptor.Load(F);
/*									
	m_pDataX= new unsigned __int32 [m_NumOfDataX];
	m_pDataY= new unsigned __int32 [m_NumOfDataY];
	m_pDataZ= new unsigned __int32 [m_NumOfDataZ];
	
	for (i= 0; i < m_NumOfDataX; i++)
	{
		m_pDataX[i]= 0;
		fread(&m_pDataX[i], 1, m_ptrLen, F);
	}
	for (i= 0; i < m_NumOfDataY; i++)
	{
		m_pDataY[i]= 0;
		fread(&m_pDataY[i], 1, m_ptrLen, F);
	}
	for (i= 0; i < m_NumOfDataZ; i++)
	{
		m_pDataZ[i]= 0;
		fread(&m_pDataZ[i], 1, m_ptrLen, F);
	}
*/	
/*	
	if ((m_NumOfDataX != 0) && (m_NumOfDataY != 0) && (m_NumOfDataZ != NULL) && (m_Renderer.m_hWnd != NULL))
	{
	m_Renderer.m_MaxValY= 0x90;
	m_Renderer.m_MaxValX= 14; //0x9C;
	m_Renderer.m_MaxValZ= 21; 0x71;
	m_Renderer.m_NumOfGridMarkX= 14;
	m_Renderer.m_NumOfGridMarkY= 14;
	m_Renderer.m_NumOfGridMarkZ= 21;
	m_Renderer.Init();
	m_Renderer.DrawGrid();
	
	HPEN hPen;
	
	hPen= CreatePen(PS_SOLID, 2, 0x00F00000);
	SelectObject(m_Renderer.m_hDcMem, hPen);

	int X= 0, Y= 0, Z= 0, x0=-1, y0= -1, x1= -1, y1= -1;
	for (Y= 0; Y < m_NumOfDataY; Y++)
//Z= 13;	
	{
		x1= -1; y1= -1;
		for (X= 0; X < m_NumOfDataX; X++)
//X= 0;
		{
//			PlotXYZ(DataX[X], DataY[(int)Z][(int)X], DataZ[Z], &x0, &y0);
			m_Renderer.PlotXYZ(Y, m_pDataZ[Z* m_NumOfDataX+ X], X, &x0, &y0);
			if (x1 == -1)
			{
				::MoveToEx(m_Renderer.m_hDcMem, x0, y0, NULL);
				x1= x0; y1= x0; 
			}
			else LineTo(m_Renderer.m_hDcMem, x0, y0);

//			PlotXYZ(DataZ[Z], DataY[(int)Z][(int)X], DataX[X]);
//			PlotXYZ(DataX[X], DataZ[Z], DataY[(int)Z][(int)X]);
//			PlotXYZ(DataZ[Z], DataX[X], DataY[(int)Z][(int)X]);
//			PlotXZ(X, DataZ[Z]);
		}
	}
		for (X= 0; X < m_NumOfDataX; X++)
//Z= 13;	
	{
		x1= -1; y1= -1;
	for (Y= 0; Y < m_NumOfDataY; Y+= 1)
//X= 0;
		{
//			PlotXYZ(DataX[X], DataY[(int)Z][(int)X], DataZ[Z], &x0, &y0);
			m_Renderer.PlotXYZ(Y, m_pDataZ[Z* m_NumOfDataX+ X], X, &x0, &y0);
			if (x1 == -1)
			{
				::MoveToEx(m_Renderer.m_hDcMem, x0, y0, NULL);
				x1= x0; y1= x0; 
			}
			else LineTo(m_Renderer.m_hDcMem, x0, y0);

//			PlotXYZ(DataZ[Z], DataY[(int)Z][(int)X], DataX[X]);
//			PlotXYZ(DataX[X], DataZ[Z], DataY[(int)Z][(int)X]);
//			PlotXYZ(DataZ[Z], DataX[X], DataY[(int)Z][(int)X]);
//			PlotXZ(X, DataZ[Z]);
		}
	}	
	DeleteObject(hPen);
	::BitBlt(m_Renderer.m_hDcWnd, 0, 0, m_Renderer.m_Width, m_Renderer.m_Height, m_Renderer.m_hDcMem, 0, 0, SRCCOPY);
	}
*/	
}

void CDlgGraph3D_Builder::OnBnClickedButtonLoad()
{
	char *pFN, *pShortFN;
	TCHAR StrVal[128]; 
	FILE *F;
	int CntCol;
	
	pFN= SelectFileToOpen(m_hWnd, "Select a table data file", "data files\0*.*\0\0", &pShortFN);
	if (pFN == NULL)
		return;
	
	F= fopen(pFN, "rb+");
	if (F == NULL)
		return;
	Load(F);
	fclose(F);
}

void CDlgGraph3D_Builder::vSaveDataDescriptor(FILE *F)
{
	CDlgData::_SaveDataDescriptor(F);
	
	m_DataDescriptor.Save(F);
}

void CDlgGraph3D_Builder::vLoadDataDescriptor(FILE *F)
{
	CDlgData::_LoadDataDescriptor(F);
	m_DataDescriptor.Load(F);
}

void CDlgGraph3D_Builder::vDsplDataDescriptor()
{
	CDlgData::_DsplDataDescriptor();
}