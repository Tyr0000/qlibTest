#include "stdio.h"

#pragma once

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

/*
typedef struct _DATA_SET
{
	DATA_SET_TYPE DataSetType;
	DATA_PTR_LEN DataPtrLen;
	unsigned __int32 DataOffset; 
	unsigned __int32 DataLen;
	unsigned __int8 *pData;
	_DATA_SET()
	{
		DataSetType= DST_UNDEFINED;
		DataPtrLen= DPL_UNDEFINED;
		DataOffset= 0;
		DataLen= 0;
		pData= NULL;
	}
	~_DATA_SET()
	{
		if (pData != NULL)
			free(pData);
	}
}
DATA_SET;

*/

typedef struct _DATA_DESCRIPTOR
{
	DATA_SET_TYPE m_DataType;
	DATA_PTR_LEN m_DataPtrLen;
	unsigned __int32 m_MinVal;
	unsigned __int32 m_MaxVal;
//	unsigned __int32 DataOffset; 
//	unsigned __int32 DataLen;
//	unsigned __int8 *pData;
	_DATA_DESCRIPTOR()
	{
		m_DataType= DST_UNDEFINED;
		m_DataPtrLen= DPL_UNDEFINED;
//		DataOffset= 0;
//		DataLen= 0;
//		pData= NULL;
	}
	~_DATA_DESCRIPTOR()
	{
//		if (pData != NULL)
//			free(pData);
	}
	void Save(FILE *F)
	{
		fwrite(&m_DataType, 1, 1, F);
		fwrite(&m_DataPtrLen, 1, 1, F);

//		fwrite(&m_DataOffset, 1, 4, F);
		fwrite(&m_MinVal, 1, 4, F);
		fwrite(&m_MaxVal, 1, 4, F);
//		fwrite(&m_DataLen, 1, 4, F);	
	}
	void Load(FILE *F)
	{
		fread(&m_DataType, 1, 1, F);
		fread(&m_DataPtrLen, 1, 1, F);

//		fread(&m_DataOffset, 1, 4, F);
		fread(&m_MinVal, 1, 4, F);
		fread(&m_MaxVal, 1, 4, F);
//		fread(&m_DataLen, 1, 4, F);
	}
}
DATA_DESCRIPTOR;

typedef struct _DATA_2D__DESCRIPTOR // : public DATA_DESCRIPTOR
{
	unsigned int m_NumOfDataX;
	unsigned int m_NumOfDataY;
	unsigned __int32 m_OffsetOfDataX;
	unsigned __int32 m_OffsetOfDataY;
	unsigned __int32 *m_pDataX;
	unsigned __int32 *m_pDataY;
	_DATA_2D__DESCRIPTOR() // : DATA_DESCRIPTOR()
	{
		m_NumOfDataX= 0;
		m_NumOfDataY= 0;
		m_OffsetOfDataX= 0;
		m_OffsetOfDataY= 0;
		m_pDataX= NULL;
		m_pDataY= NULL;
	}
	void UpdateDataDescriptorFromGUI(HWND hDlg)
	{
		TCHAR tStr[32];

		GetDlgItemText(hDlg, IDC_EDIT_NUM_OF_X, tStr, 32);
		m_NumOfDataX= _tstoi(tStr);
		GetDlgItemText(hDlg, IDC_EDIT_NUM_OF_Y, tStr, 32);
		m_NumOfDataY= _tstoi(tStr);
		
		GetDlgItemText(hDlg, IDC_EDIT_DATA_OFFSET_X, tStr, 32);
		m_OffsetOfDataX= _tstoi(tStr);
		GetDlgItemText(hDlg, IDC_EDIT_DATA_OFFSET_Y, tStr, 32);
		m_OffsetOfDataY= _tstoi(tStr);
	}
	void Load(FILE *F)
	{
//		DATA_DESCRIPTOR::Load(F);
		
		fread(&m_NumOfDataX, 1, 4, F);
		fread(&m_NumOfDataY, 1, 4, F);
		fread(&m_OffsetOfDataX, 1, 4, F);
		fread(&m_OffsetOfDataY, 1, 4, F);
	}
	void Save(FILE *F)
	{
//		DATA_DESCRIPTOR::Save(F);
		
		fwrite(&m_NumOfDataX, 1, 4, F);
		fwrite(&m_NumOfDataY, 1, 4, F);
		fwrite(&m_OffsetOfDataX, 1, 4, F);
		fwrite(&m_OffsetOfDataY, 1, 4, F);
	}
}
DATA_2D_DESCRIPTOR;

typedef struct _DATA_3D_DESCRIPTOR: public DATA_2D_DESCRIPTOR // DATA_DESCRIPTOR
{
//	int m_NumOfDataX;
//	int m_NumOfDataY;
	int m_NumOfDataZ;
//	unsigned __int32 m_OffsetOfDataX;
//	unsigned __int32 m_OffsetOfDataY;
	unsigned __int32 m_OffsetOfDataZ;
//	unsigned __int32 *m_pDataX;
//	unsigned __int32 *m_pDataY;
	unsigned __int32 *m_pDataZ;
	
	_DATA_3D_DESCRIPTOR(): DATA_2D_DESCRIPTOR()
	{
//		m_NumOfDataX= 0;
//		m_NumOfDataY= 0;
		m_NumOfDataZ= 0;
//		m_OffsetOfDataX= 0;
//		m_OffsetOfDataY= 0;
		m_OffsetOfDataZ= 0;
//		m_pDataX= NULL;
//		m_pDataY= NULL;
		m_pDataZ= NULL;		
	}
	void Load(FILE *F)
	{
		DATA_2D_DESCRIPTOR::Load(F);
		
//		fread(&m_NumOfDataX, 1, 4, F);
//		fread(&m_NumOfDataY, 1, 4, F);
		fread(&m_NumOfDataZ, 1, 4, F);
//		fread(&m_OffsetOfDataX, 1, 4, F);
//		fread(&m_OffsetOfDataY, 1, 4, F);
		fread(&m_OffsetOfDataZ, 1, 4, F);
	}
	void Save(FILE *F)
	{
		DATA_2D_DESCRIPTOR::Save(F);
		
//		fwrite(&m_NumOfDataX, 1, 4, F);
//		fwrite(&m_NumOfDataY, 1, 4, F);
		fwrite(&m_NumOfDataZ, 1, 4, F);
//		fwrite(&m_OffsetOfDataX, 1, 4, F);
///		fwrite(&m_OffsetOfDataY, 1, 4, F);
		fwrite(&m_OffsetOfDataZ, 1, 4, F);
	}
}
DATA_3D_DESCRIPTOR;