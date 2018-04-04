#include <windows.h>

#pragma once

class C3dRenderer
{
	public:
		C3dRenderer(void);
		~C3dRenderer(void);

private:
public:
	float m_ScaleX;
	float m_ScaleY;		
	float m_ScaleZ;		
	float m_ScaleZx;		
	float m_ScaleZy;		
	HWND	m_hWnd;
	HDC		m_hDcWnd;
	HDC		m_hDcMem;
	HBITMAP m_hBm;
	float m_MinValX;
	float m_MinValY;
	float m_MinValZ;
	float m_MaxValX;
	float m_MaxValY;
	float m_MaxValZ;
	float m_GridStepX;
	float m_GridStepY;
	float m_GridStepZ;
	float m_GridStepZx;
	float m_GridStepZy;
	int		m_NumOfGridMarkX;
	int		m_NumOfGridMarkY;
	int		m_NumOfGridMarkZ;
	int   m_Zx;
	int   m_Zy;
	int		m_X0;
	int		m_Y0;
	int		m_Width;
	int		m_Height;
	void	Init();
	void	Map3dTo2d(float X, float Y, float Z, int *pX, int *pY);
	void	PlotXY(float X, float Y);
	void	PlotXZ(float X, float Z);
	void	PlotYZ(float Y, float Z);
	void	PlotXYZ(float X, float Y, float Z, int *pX= NULL, int *pY= NULL);
public:
	void AttachWnd(HWND hWnd);
  void DrawGrid();			
  void DrawSphere();
  void DrawBezierPyramid();
  void DrawBezierCurve();
  void DrawData();
  void DrawData1();
  void DrawData2();
};
