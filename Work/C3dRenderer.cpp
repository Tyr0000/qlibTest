#include "StdAfx.h"
#include "math.h"
#include "C3dRenderer.h"

#define M_PI						3.14

#define OFFSET_X				15
#define OFFSET_Y				15
#define STEP_X					0.5
#define STEP_Y					0.5
#define STEP_Z					0.5
#define COS_F						(sqrt(2.) / 2)		// for Z axis
#define SIN_F						(sqrt(2.) / 2)		// for Z axis

HDC hDC;
 void draw_3d_polygon(int [5][3],int [8][2],int [5][5]);
 void get_projected_point(int&,int&,int&);
 void Bezier_curve(const int [4][3]);

 void Line(const int,const int,const int,const int);

C3dRenderer::C3dRenderer(void)
{
	m_ScaleX= 0;
	m_ScaleY= 0;		
	m_ScaleZ= 0;		
	m_ScaleZx= 0;		
	m_ScaleZy= 0;		
	m_hWnd= NULL;
	m_hDcMem= NULL;
	m_hBm= NULL;
	m_MaxValX= 0;
	m_MaxValY= 0;
	m_MaxValZ= 0;
//	m_Zx= 0;
//	m_Zy= 0;
	m_Width= 0;
	m_Height= 0;
	m_NumOfGridMarkX= 10;
	m_NumOfGridMarkY= 10;
	m_NumOfGridMarkZ= 10;	
}

C3dRenderer::~C3dRenderer(void)
{
	if (m_hBm != NULL) DeleteObject(m_hBm);
	if (m_hDcWnd != NULL) ReleaseDC(m_hWnd, m_hDcWnd);
	if (m_hDcMem) DeleteDC(m_hDcMem);
}

HDC hDcWnd= NULL;
int Width= 0;
int Height= 0;
HDC hDcMem= NULL;

LRESULT CALLBACK RendererWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT PS= {0};
	HDC	hDc;

	if (uMsg == WM_PAINT)
	{
		hDc= BeginPaint(hwnd, &PS);
		::BitBlt(hDcWnd, 0, 0, Width, Height, hDcMem, 0, 0, SRCCOPY);
		EndPaint(hwnd, &PS);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); 
}

void C3dRenderer::AttachWnd(HWND hWnd)
{	
	RECT	WndRect;
	
	if (m_hBm != NULL) DeleteObject(m_hBm);
	if (m_hDcWnd != NULL) ReleaseDC(m_hWnd, m_hDcWnd);
	if (m_hDcMem) DeleteDC(m_hDcMem);

	m_hWnd= hWnd;
	m_hDcWnd= ::GetWindowDC(m_hWnd);
	::GetClientRect(m_hWnd, &WndRect);
	::AdjustWindowRect(&WndRect, ::GetWindowLongPtr(m_hWnd, GWL_STYLE), false);
	m_Width= WndRect.right- WndRect.left;
	m_Height= WndRect.bottom- WndRect.top;	
	m_hDcMem= ::CreateCompatibleDC(m_hDcWnd);
	m_hBm= ::CreateCompatibleBitmap(m_hDcWnd, m_Width, m_Height);
	SelectObject(m_hDcMem, m_hBm);
	hDcWnd= m_hDcWnd;
	Width=	m_Width;
	Height= m_Height; 
	hDcMem= m_hDcMem; 
	::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG)RendererWndProc);
	Init();
}

void C3dRenderer::Init()
{
	float Tmp;
/*	
// equal length of Y Z axises
	m_MaxValZy= (m_Height- (OFFSET_Y* 2)/ 2;
	ZLen= m_MaxValZy/ SIN_F;
	m_MaxValZx= ZLen* COS_F;
*/

// divide Y Z axises length according NumOfGridLines
	m_GridStepY= (m_Height- (OFFSET_Y* 2))/ (m_NumOfGridMarkY+ m_NumOfGridMarkZ); // avearage step for Y Z axises
	m_GridStepZy= m_GridStepY;
// !!!!!	Tmp= (SIN_F* COS_F);
	m_GridStepZx= m_GridStepZy; 

	m_Zx= m_GridStepZx* m_NumOfGridMarkZ;
	m_Zy= m_GridStepZy* m_NumOfGridMarkZ;
	m_ScaleX= (m_Width- (OFFSET_X* 2)- m_Zx)/ (m_MaxValX);
	m_ScaleY= (m_Height- (OFFSET_Y* 2)- m_Zy)/ (m_MaxValY);
//	m_ScaleZ= ZLen/ m_MaxValZ;
	if (m_MaxValZ != 0)
	{
		m_ScaleZx= m_Zx/ (m_MaxValZ); //* COS_F);
		m_ScaleZy= m_Zy/ (m_MaxValZ); //* SIN_F);
	}
	
	m_X0= OFFSET_X+ m_Zx;
	m_Y0= OFFSET_Y+ m_MaxValY* m_ScaleY;
//	m_Y0= m_Height- 1- OFFSET_Y- m_Zy;

	m_GridStepX= m_MaxValX* m_ScaleX / m_NumOfGridMarkX;
}

void C3dRenderer::DrawGrid()
{
//	RECT Rect= {0};
	HPEN hPenAxis;
	float x, y, z;
/*	
	Rect.top= OFFSET_Y;
	Rect.bottom= m_Height- 1- OFFSET_Y;
	Rect.left= OFFSET_X;
	Rect.right= m_Width- 1- OFFSET_X;
	FillRect(m_hDcMem, &Rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
*/
	SelectObject(m_hDcMem, (HBRUSH)GetStockObject(WHITE_BRUSH));
	SelectObject(m_hDcMem, (HBRUSH)GetStockObject(WHITE_PEN));
	Rectangle(m_hDcMem, 0, 0, m_Width, m_Height);
	
	MoveToEx(m_hDcMem, m_X0, m_Y0, NULL);
	LineTo(m_hDcMem, m_X0+ m_MaxValX* m_ScaleX, m_Y0);
	MoveToEx(m_hDcMem, m_X0, m_Y0, NULL);
	LineTo(m_hDcMem, m_X0, OFFSET_Y);
	MoveToEx(m_hDcMem, m_X0, m_Y0, NULL);
	LineTo(m_hDcMem, m_X0- m_Zx, m_Y0+ m_Zy);
	
	hPenAxis= CreatePen(PS_DASH, 2, 0x00F0F0F0);
	SelectObject(m_hDcMem, hPenAxis);
/////
	y= m_Y0- m_GridStepY;
	do
	{
		MoveToEx(m_hDcMem, m_X0,						  y, NULL);
		LineTo(m_hDcMem,	 m_Width- 1- OFFSET_X, y);
		MoveToEx(m_hDcMem, m_X0,							y, NULL);
		LineTo(m_hDcMem,	 OFFSET_X,					y+ m_Zy);
		y-= m_GridStepY;
	}
	while (y > OFFSET_Y);
	y= OFFSET_Y;
	MoveToEx(m_hDcMem, m_X0,						  y, NULL);
	LineTo(m_hDcMem,	 m_Width- OFFSET_X, y);
	MoveToEx(m_hDcMem, m_X0,							y, NULL);
	LineTo(m_hDcMem,	 OFFSET_X,					y+ m_Zy);
/////
	x= m_X0+ m_GridStepX;
	do
	{
		MoveToEx(m_hDcMem, x,										 m_Y0, NULL);
		LineTo(m_hDcMem,	 x,										 OFFSET_Y);
		MoveToEx(m_hDcMem, x,										 m_Y0, NULL);
		LineTo(m_hDcMem,	 x- m_Zx, m_Height- 1- OFFSET_Y);
		x+= m_GridStepX;
//		if (x > (m_Width- 1- OFFSET_X))
	}
	while (x < (m_Width- 1- OFFSET_X));
	x= m_Width- 1- OFFSET_X;
	MoveToEx(m_hDcMem, x,										 m_Y0, NULL);
	LineTo(m_hDcMem,	 x,										 OFFSET_Y);
	MoveToEx(m_hDcMem, x,										 m_Y0, NULL);
	LineTo(m_hDcMem,	 x- m_Zx, m_Height- 1- OFFSET_Y);
/////
	x= m_X0- m_GridStepZx;
	y= m_Y0+ m_GridStepZy;
	do
	{
		MoveToEx(m_hDcMem, x, y, NULL);
		LineTo(m_hDcMem,	 x+ m_MaxValX* m_ScaleX, y);
		MoveToEx(m_hDcMem, x,								    y, NULL);
		LineTo(m_hDcMem,	 x,										y- m_MaxValY* m_ScaleY);
		x-= m_GridStepZx;
		y+= m_GridStepZy;
	}
	while (x >= OFFSET_X);
	x= OFFSET_X;
	y= m_Height- 1- OFFSET_Y;
	MoveToEx(m_hDcMem, x, y, NULL);
	LineTo(m_hDcMem,	 x+ m_MaxValX* m_ScaleX, y);
	MoveToEx(m_hDcMem, x,								    y, NULL);
	LineTo(m_hDcMem,	 x,										y- m_MaxValY* m_ScaleY);
	
	DeleteObject(hPenAxis);
}

void C3dRenderer::Map3dTo2d(float X, float Y, float Z, int *pX, int *pY)
{
	int x, y, zx, zy;
	
	x= X* m_ScaleX;
	y= Y* m_ScaleY;
	zx= Z* COS_F* m_ScaleX;
	zy= Z* SIN_F* m_ScaleY;
	SetPixel(m_hDcMem, m_X0+ x- zx, m_Y0- (y- zy), 0x00FF0000);
}


void C3dRenderer::PlotXY(float X, float Y)
{
	int x, y;
	
	x= X* m_ScaleX;
	y= Y* m_ScaleY;
//	SetPixel(m_hDcMem, m_X0+ x, m_Y0- y, 0x00FF0000);
	Ellipse(m_hDcMem,  m_X0+ x- 3, m_Y0- y- 3,  m_X0+ x+ 3, m_Y0- y+ 3);
}

void C3dRenderer::PlotXZ(float X, float Z)
{
	int x, zx, zy;
	
	x= X* m_ScaleX;
	zx= Z* m_ScaleZx;
	zy= Z* m_ScaleZy;
//	SetPixel(m_hDcMem, m_X0+ zx, m_Y0+ zy, 0x00FF0000);
	Ellipse(m_hDcMem,  m_X0+ x- zx- 3, m_Y0+ zy- 3, m_X0+ x- zx+ 3, m_Y0+ zy+ 3);
}

void C3dRenderer::PlotYZ(float Y, float Z)
{
	int y, zx, zy;

	y= Y* m_ScaleY;
	zx= Z* m_ScaleZx;
	zy= Z* m_ScaleZy;
	Ellipse(m_hDcMem,  m_X0- zx- 3, m_Y0- y+ zy- 3, m_X0- zx+ 3, m_Y0- y+ zy+ 3);
}

void  C3dRenderer::PlotXYZ(float X, float Y, float Z, int *pX, int *pY)
{
	int x, y, zx, zy;

	x= X* m_ScaleX;
	y= Y* m_ScaleY;
	zx= Z* m_ScaleZx;
	zy= Z* m_ScaleZy;
	Ellipse(m_hDcMem,  m_X0+ x- zx- 3, m_Y0- y+ zy- 3, m_X0+ x- zx+ 3, m_Y0- y+ zy+ 3);
	if (pX != NULL) *pX= m_X0+ x- zx;
	if (pY != NULL) *pY= m_Y0- y+ zy;
}

void C3dRenderer::DrawSphere()
{
	float X= 0, Y= 0, Z= 0, XX[3]= {1, 2, 3};
	
	m_MaxValX= 10;
	m_MaxValY= 10;
	m_MaxValZ= 10;
	Init();
	DrawGrid();
	
	SelectObject(m_hDcMem, GetStockObject(GRAY_BRUSH));
//			PlotXZ(X, 0);
//return;

//	for (Y= 0; Y <= 6.; Y+= 1.)
	for (Z= 0; Z <= 5; Z+= 1.)
		for (X= 0; X <= 5; X+= 1.)
		{
//			Z= sqrt(100-((X* X)+ (Y* Y)));
			Y= sqrt(100-((X* X)+ (Z* Z)));
//			Map3dTo2d(X, Y, Z, NULL, NULL);
//			Z= sqrt(100-((X)* (X)));
//			Y= sqrt(100-((X)* (X)));
//			PlotXY(X, Z);
//			PlotXZ(X, Z);
//			PlotYZ(X, Z);
			PlotXYZ(X, Y, Z);
		}
}

void C3dRenderer::DrawData()
{
// первые 14 строк это значени€ Z
// верхн€€ строка начнающа€с€ с 34 это соответсвует нагрузке 0D
	int DataY[14][21]= 
	{ 
		{0x34, 0x36, 0x3A, 0x3C, 0x3E, 0x3A, 0x40, 0x40, 0x40, 0x3F, 0x3F, 0x42, 0x44, 0x48, 0x4B, 0x4D, 0x50, 0x54, 0x58, 0x59, 0x59}, 
		{0x3A, 0x3E, 0x40, 0x42, 0x44, 0x40, 0x46, 0x46, 0x46, 0x46, 0x46, 0x47, 0x49, 0x4A, 0x4D, 0x50, 0x54, 0x58, 0x5A, 0x5B, 0x5B}, 
		{0x40, 0x44, 0x49, 0x4B, 0x4A, 0x46, 0x4E, 0x50, 0x50, 0x50, 0x50, 0x50, 0x52, 0x54, 0x54, 0x55, 0x58, 0x5A, 0x5C, 0x5E, 0x5E}, 
		{0x44, 0x48, 0x4E, 0x50, 0x4E, 0x4C, 0x54, 0x58, 0x58, 0x5A, 0x59, 0x5A, 0x5B, 0x5B, 0x5B, 0x5B, 0x5B, 0x5E, 0x5F, 0x60, 0x60}, 
		{0x48, 0x4D, 0x51, 0x54, 0x52, 0x52, 0x58, 0x5E, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x62, 0x61, 0x60, 0x60, 0x60, 0x62, 0x62, 0x62},
		{0x4C, 0x50, 0x54, 0x56, 0x56, 0x58, 0x5E, 0x62, 0x64, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x64, 0x62, 0x62, 0x63, 0x64, 0x64},
		{0x4E, 0x52, 0x56, 0x58, 0x58, 0x5C, 0x64, 0x66, 0x69, 0x69, 0x6A, 0x6A, 0x6A, 0x69, 0x67, 0x66, 0x65, 0x64, 0x66, 0x66, 0x66},
		{0x50, 0x54, 0x58, 0x5A, 0x5A, 0x60, 0x68, 0x6C, 0x6D, 0x6E, 0x6F, 0x6F, 0x6D, 0x6C, 0x6A, 0x68, 0x67, 0x66, 0x68, 0x68, 0x68},
		{0x52, 0x56, 0x5A, 0x5C, 0x5C, 0x64, 0x6C, 0x70, 0x74, 0x75, 0x76, 0x75, 0x74, 0x70, 0x6E, 0x6C, 0x6B, 0x6A, 0x69, 0x69, 0x69},
		{0x69, 0x6C, 0x6E, 0x6E, 0x70, 0x74, 0x76, 0x78, 0x7E, 0x7F, 0x80, 0x7F, 0x7E, 0x7A, 0x77, 0x74, 0x71, 0x6F, 0x6B, 0x69, 0x69},
		{0x6C, 0x6E, 0x6E, 0x70, 0x74, 0x7A, 0x7C, 0x7C, 0x7E, 0x81, 0x83, 0x84, 0x84, 0x82, 0x80, 0x7B, 0x77, 0x71, 0x6D, 0x6B, 0x6A},
		{0x6E, 0x70, 0x70, 0x70, 0x74, 0x7C, 0x7E, 0x80, 0x80, 0x80, 0x84, 0x86, 0x84, 0x82, 0x81, 0x7D, 0x78, 0x71, 0x6D, 0x6B, 0x6A},
		{0x72, 0x72, 0x72, 0x72, 0x76, 0x7C, 0x80, 0x82, 0x80, 0x80, 0x84, 0x86, 0x85, 0x84, 0x82, 0x7D, 0x78, 0x71, 0x6D, 0x6B, 0x6A},
		{0x74, 0x74, 0x74, 0x74, 0x78, 0x7C, 0x82, 0x82, 0x80, 0x7F, 0x82, 0x83, 0x82, 0x82, 0x80, 0x7D, 0x78, 0x71, 0x6B, 0x69, 0x68} 
	};
//	оброты 
	int DataX[21]=
	{ 0x0C, 0x10, 0x16, 0x1C, 0x22, 0x28, 0x2E, 0x34, 0x3C, 0x44, 0x4C, 0x54, 0x5C, 0x64, 0x6C, 0x74, 0x7C, 0x84, 0x8C, 0x94, 0x9C };
//	нагрузка 
	int DataZ[14]=
	{ 0x0D, 0x11, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x3C, 0x4B, 0x55, 0x64, 0x71 };
	
	int X= 0, Y= 0, Z= 0, x0=-1, y0= -1, x1= -1, y1= -1;
	int MaxValY= 0, MinValY= 0;

	for (Z= 0; Z < 14; Z+= 1)
		for (X= 0; X < 21; X++)
		{
			if (MaxValY <  DataY[(int)Z][(int)X]) MaxValY=  DataY[(int)Z][(int)X];
			if (MinValY >  DataY[(int)Z][(int)X]) MinValY=  DataY[(int)Z][(int)X];
		}
	
	m_MaxValY= 0x90;
	m_MaxValX= 21; //0x9C;
	m_MaxValZ= 14; 0x71;
	m_NumOfGridMarkX= 21;
	m_NumOfGridMarkY= 5;
	m_NumOfGridMarkZ= 20;
	Init();
	DrawGrid();
	
	HPEN hPen;
	
	hPen= CreatePen(PS_SOLID, 2, 0x00F00000);
	SelectObject(m_hDcMem, hPen);
	
	for (Z= 0; Z < 14; Z+= 1)
//Z= 13;	
	{
		x1= -1; y1= -1;
		for (X= 0; X < 21; X++)
//X= 0;
		{
//			PlotXYZ(DataX[X], DataY[(int)Z][(int)X], DataZ[Z], &x0, &y0);
			PlotXYZ(X, DataY[(int)Z][(int)X], Z, &x0, &y0);
			if (x1 == -1)
			{
				::MoveToEx(m_hDcMem, x0, y0, NULL);
				x1= x0; y1= x0; 
			}
			else LineTo(m_hDcMem, x0, y0);

//			PlotXYZ(DataZ[Z], DataY[(int)Z][(int)X], DataX[X]);
//			PlotXYZ(DataX[X], DataZ[Z], DataY[(int)Z][(int)X]);
//			PlotXYZ(DataZ[Z], DataX[X], DataY[(int)Z][(int)X]);
//			PlotXZ(X, DataZ[Z]);
		}
	}
	DeleteObject(hPen);
	hPen= CreatePen(PS_SOLID, 1, 0x00F80000);
	SelectObject(m_hDcMem, hPen);
		for (X= 0; X < 21; X++)
//Z= 13;	
	{
		x1= -1; y1= -1;
	for (Z= 0; Z < 14; Z+= 1)
//X= 0;
		{
//			PlotXYZ(DataX[X], DataY[(int)Z][(int)X], DataZ[Z], &x0, &y0);
			PlotXYZ(X, DataY[(int)Z][(int)X], Z, &x0, &y0);
			if (x1 == -1)
			{
				::MoveToEx(m_hDcMem, x0, y0, NULL);
				x1= x0; y1= x0; 
			}
			else LineTo(m_hDcMem, x0, y0);

//			PlotXYZ(DataZ[Z], DataY[(int)Z][(int)X], DataX[X]);
//			PlotXYZ(DataX[X], DataZ[Z], DataY[(int)Z][(int)X]);
//			PlotXYZ(DataZ[Z], DataX[X], DataY[(int)Z][(int)X]);
//			PlotXZ(X, DataZ[Z]);
		}
	}
	DeleteObject(hPen);
}



void C3dRenderer::DrawData1()
{
// первые 14 строк это значени€ Z
// верхн€€ строка начнающа€с€ с 34 это соответсвует нагрузке 0D
	int DataY[14][21]= 
	{ 
		{0x34, 0x36, 0x3A, 0x3C, 0x3E, 0x3A, 0x40, 0x40, 0x40, 0x3F, 0x3F, 0x42, 0x44, 0x48, 0x4B, 0x4D, 0x50, 0x54, 0x58, 0x59, 0x59}, 
		{0x3A, 0x3E, 0x40, 0x42, 0x44, 0x40, 0x46, 0x46, 0x46, 0x46, 0x46, 0x47, 0x49, 0x4A, 0x4D, 0x50, 0x54, 0x58, 0x5A, 0x5B, 0x5B}, 
		{0x40, 0x44, 0x49, 0x4B, 0x4A, 0x46, 0x4E, 0x50, 0x50, 0x50, 0x50, 0x50, 0x52, 0x54, 0x54, 0x55, 0x58, 0x5A, 0x5C, 0x5E, 0x5E}, 
		{0x44, 0x48, 0x4E, 0x50, 0x4E, 0x4C, 0x54, 0x58, 0x58, 0x5A, 0x59, 0x5A, 0x5B, 0x5B, 0x5B, 0x5B, 0x5B, 0x5E, 0x5F, 0x60, 0x60}, 
		{0x48, 0x4D, 0x51, 0x54, 0x52, 0x52, 0x58, 0x5E, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x62, 0x61, 0x60, 0x60, 0x60, 0x62, 0x62, 0x62},
		{0x4C, 0x50, 0x54, 0x56, 0x56, 0x58, 0x5E, 0x62, 0x64, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x64, 0x62, 0x62, 0x63, 0x64, 0x64},
		{0x4E, 0x52, 0x56, 0x58, 0x58, 0x5C, 0x64, 0x66, 0x69, 0x69, 0x6A, 0x6A, 0x6A, 0x69, 0x67, 0x66, 0x65, 0x64, 0x66, 0x66, 0x66},
		{0x50, 0x54, 0x58, 0x5A, 0x5A, 0x60, 0x68, 0x6C, 0x6D, 0x6E, 0x6F, 0x6F, 0x6D, 0x6C, 0x6A, 0x68, 0x67, 0x66, 0x68, 0x68, 0x68},
		{0x52, 0x56, 0x5A, 0x5C, 0x5C, 0x64, 0x6C, 0x70, 0x74, 0x75, 0x76, 0x75, 0x74, 0x70, 0x6E, 0x6C, 0x6B, 0x6A, 0x69, 0x69, 0x69},
		{0x69, 0x6C, 0x6E, 0x6E, 0x70, 0x74, 0x76, 0x78, 0x7E, 0x7F, 0x80, 0x7F, 0x7E, 0x7A, 0x77, 0x74, 0x71, 0x6F, 0x6B, 0x69, 0x69},
		{0x6C, 0x6E, 0x6E, 0x70, 0x74, 0x7A, 0x7C, 0x7C, 0x7E, 0x81, 0x83, 0x84, 0x84, 0x82, 0x80, 0x7B, 0x77, 0x71, 0x6D, 0x6B, 0x6A},
		{0x6E, 0x70, 0x70, 0x70, 0x74, 0x7C, 0x7E, 0x80, 0x80, 0x80, 0x84, 0x86, 0x84, 0x82, 0x81, 0x7D, 0x78, 0x71, 0x6D, 0x6B, 0x6A},
		{0x72, 0x72, 0x72, 0x72, 0x76, 0x7C, 0x80, 0x82, 0x80, 0x80, 0x84, 0x86, 0x85, 0x84, 0x82, 0x7D, 0x78, 0x71, 0x6D, 0x6B, 0x6A},
		{0x74, 0x74, 0x74, 0x74, 0x78, 0x7C, 0x82, 0x82, 0x80, 0x7F, 0x82, 0x83, 0x82, 0x82, 0x80, 0x7D, 0x78, 0x71, 0x6B, 0x69, 0x68} 
	};
//	оброты 
	int DataX[21]=
	{ 0x0C, 0x10, 0x16, 0x1C, 0x22, 0x28, 0x2E, 0x34, 0x3C, 0x44, 0x4C, 0x54, 0x5C, 0x64, 0x6C, 0x74, 0x7C, 0x84, 0x8C, 0x94, 0x9C };
//	нагрузка 
	int DataZ[14]=
	{ 0x0D, 0x11, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x3C, 0x4B, 0x55, 0x64, 0x71 };
	
	int X= 0, Y= 0, Z= 0, x0=-1, y0= -1, x1= -1, y1= -1;
	
	m_MaxValY= 0x90;
	m_MaxValX= 21; //0x9C;
	m_MaxValZ= 14; 0x71;
	m_NumOfGridMarkX= 21;
	m_NumOfGridMarkY= 5;
	m_NumOfGridMarkZ= 20;
	Init();
	DrawGrid();
	
	HPEN hPen;
	
	hPen= CreatePen(PS_SOLID, 2, 0x00F00000);
	SelectObject(m_hDcMem, hPen);
	
	for (Z= 0; Z < 14; Z+= 1)
//Z= 13;	
	{
		x1= -1; y1= -1;
		for (X= 0; X < 21; X++)
//X= 0;
		{
//			PlotXYZ(DataX[X], DataY[(int)Z][(int)X], DataZ[Z], &x0, &y0);
			PlotXYZ(X, DataY[(int)Z][(int)X], Z, &x0, &y0);
			if (x1 == -1)
			{
				::MoveToEx(m_hDcMem, x0, y0, NULL);
				x1= x0; y1= x0; 
			}
			else LineTo(m_hDcMem, x0, y0);

//			PlotXYZ(DataZ[Z], DataY[(int)Z][(int)X], DataX[X]);
//			PlotXYZ(DataX[X], DataZ[Z], DataY[(int)Z][(int)X]);
//			PlotXYZ(DataZ[Z], DataX[X], DataY[(int)Z][(int)X]);
//			PlotXZ(X, DataZ[Z]);
		}
	}
	DeleteObject(hPen);
	hPen= CreatePen(PS_SOLID, 1, 0x00F80000);
	SelectObject(m_hDcMem, hPen);
		for (X= 0; X < 21; X++)
//Z= 13;	
	{
		x1= -1; y1= -1;
	for (Z= 0; Z < 14; Z+= 1)
//X= 0;
		{
//			PlotXYZ(DataX[X], DataY[(int)Z][(int)X], DataZ[Z], &x0, &y0);
			PlotXYZ(X, DataY[(int)Z][(int)X], Z, &x0, &y0);
			if (x1 == -1)
			{
				::MoveToEx(m_hDcMem, x0, y0, NULL);
				x1= x0; y1= x0; 
			}
			else LineTo(m_hDcMem, x0, y0);

//			PlotXYZ(DataZ[Z], DataY[(int)Z][(int)X], DataX[X]);
//			PlotXYZ(DataX[X], DataZ[Z], DataY[(int)Z][(int)X]);
//			PlotXYZ(DataZ[Z], DataX[X], DataY[(int)Z][(int)X]);
//			PlotXZ(X, DataZ[Z]);
		}
	}
	DeleteObject(hPen);
}


void C3dRenderer::DrawData2()
{
// первые 14 строк это значени€ Z
// верхн€€ строка начнающа€с€ с 34 это соответсвует нагрузке 0D
	int DataY[14][21]= 
	{ 
		{0x34, 0x36, 0x3A, 0x3C, 0x3E, 0x3A, 0x40, 0x40, 0x40, 0x3F, 0x3F, 0x42, 0x44, 0x48, 0x4B, 0x4D, 0x50, 0x54, 0x58, 0x59, 0x59}, 
		{0x3A, 0x3E, 0x40, 0x42, 0x44, 0x40, 0x46, 0x46, 0x46, 0x46, 0x46, 0x47, 0x49, 0x4A, 0x4D, 0x50, 0x54, 0x58, 0x5A, 0x5B, 0x5B}, 
		{0x40, 0x44, 0x49, 0x4B, 0x4A, 0x46, 0x4E, 0x50, 0x50, 0x50, 0x50, 0x50, 0x52, 0x54, 0x54, 0x55, 0x58, 0x5A, 0x5C, 0x5E, 0x5E}, 
		{0x44, 0x48, 0x4E, 0x50, 0x4E, 0x4C, 0x54, 0x58, 0x58, 0x5A, 0x59, 0x5A, 0x5B, 0x5B, 0x5B, 0x5B, 0x5B, 0x5E, 0x5F, 0x60, 0x60}, 
		{0x48, 0x4D, 0x51, 0x54, 0x52, 0x52, 0x58, 0x5E, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x62, 0x61, 0x60, 0x60, 0x60, 0x62, 0x62, 0x62},
		{0x4C, 0x50, 0x54, 0x56, 0x56, 0x58, 0x5E, 0x62, 0x64, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x64, 0x62, 0x62, 0x63, 0x64, 0x64},
		{0x4E, 0x52, 0x56, 0x58, 0x58, 0x5C, 0x64, 0x66, 0x69, 0x69, 0x6A, 0x6A, 0x6A, 0x69, 0x67, 0x66, 0x65, 0x64, 0x66, 0x66, 0x66},
		{0x50, 0x54, 0x58, 0x5A, 0x5A, 0x60, 0x68, 0x6C, 0x6D, 0x6E, 0x6F, 0x6F, 0x6D, 0x6C, 0x6A, 0x68, 0x67, 0x66, 0x68, 0x68, 0x68},
		{0x52, 0x56, 0x5A, 0x5C, 0x5C, 0x64, 0x6C, 0x70, 0x74, 0x75, 0x76, 0x75, 0x74, 0x70, 0x6E, 0x6C, 0x6B, 0x6A, 0x69, 0x69, 0x69},
		{0x69, 0x6C, 0x6E, 0x6E, 0x70, 0x74, 0x76, 0x78, 0x7E, 0x7F, 0x80, 0x7F, 0x7E, 0x7A, 0x77, 0x74, 0x71, 0x6F, 0x6B, 0x69, 0x69},
		{0x6C, 0x6E, 0x6E, 0x70, 0x74, 0x7A, 0x7C, 0x7C, 0x7E, 0x81, 0x83, 0x84, 0x84, 0x82, 0x80, 0x7B, 0x77, 0x71, 0x6D, 0x6B, 0x6A},
		{0x6E, 0x70, 0x70, 0x70, 0x74, 0x7C, 0x7E, 0x80, 0x80, 0x80, 0x84, 0x86, 0x84, 0x82, 0x81, 0x7D, 0x78, 0x71, 0x6D, 0x6B, 0x6A},
		{0x72, 0x72, 0x72, 0x72, 0x76, 0x7C, 0x80, 0x82, 0x80, 0x80, 0x84, 0x86, 0x85, 0x84, 0x82, 0x7D, 0x78, 0x71, 0x6D, 0x6B, 0x6A},
		{0x74, 0x74, 0x74, 0x74, 0x78, 0x7C, 0x82, 0x82, 0x80, 0x7F, 0x82, 0x83, 0x82, 0x82, 0x80, 0x7D, 0x78, 0x71, 0x6B, 0x69, 0x68} 
	};
//	оброты 
	int DataX[21]=
	{ 0x0C, 0x10, 0x16, 0x1C, 0x22, 0x28, 0x2E, 0x34, 0x3C, 0x44, 0x4C, 0x54, 0x5C, 0x64, 0x6C, 0x74, 0x7C, 0x84, 0x8C, 0x94, 0x9C };
//	нагрузка 
	int DataZ[14]=
	{ 0x0D, 0x11, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x3C, 0x4B, 0x55, 0x64, 0x71 };
	
	int X= 0, Y= 0, Z= 0, x0=-1, y0= -1, x1= -1, y1= -1;
	
	m_MaxValY= 0x90;
	m_MaxValX= 14; //0x9C;
	m_MaxValZ= 21; 0x71;
	m_NumOfGridMarkX= 14;
	m_NumOfGridMarkY= 14;
	m_NumOfGridMarkZ= 21;
	Init();
	DrawGrid();
	
	HPEN hPen;
	
	hPen= CreatePen(PS_SOLID, 2, 0x00F00000);
	SelectObject(m_hDcMem, hPen);
	
	for (Z= 0; Z < 14; Z+= 1)
//Z= 13;	
	{
		x1= -1; y1= -1;
		for (X= 0; X < 21; X++)
//X= 0;
		{
//			PlotXYZ(DataX[X], DataY[(int)Z][(int)X], DataZ[Z], &x0, &y0);
			PlotXYZ(Z, DataY[(int)Z][(int)X], X, &x0, &y0);
			if (x1 == -1)
			{
				::MoveToEx(m_hDcMem, x0, y0, NULL);
				x1= x0; y1= x0; 
			}
			else LineTo(m_hDcMem, x0, y0);

//			PlotXYZ(DataZ[Z], DataY[(int)Z][(int)X], DataX[X]);
//			PlotXYZ(DataX[X], DataZ[Z], DataY[(int)Z][(int)X]);
//			PlotXYZ(DataZ[Z], DataX[X], DataY[(int)Z][(int)X]);
//			PlotXZ(X, DataZ[Z]);
		}
	}
	DeleteObject(hPen);
	hPen= CreatePen(PS_SOLID, 1, 0x00F80000);
	SelectObject(m_hDcMem, hPen);
		for (X= 0; X < 21; X++)
//Z= 13;	
	{
		x1= -1; y1= -1;
	for (Z= 0; Z < 14; Z+= 1)
//X= 0;
		{
//			PlotXYZ(DataX[X], DataY[(int)Z][(int)X], DataZ[Z], &x0, &y0);
			PlotXYZ(Z, DataY[(int)Z][(int)X], X, &x0, &y0);
			if (x1 == -1)
			{
				::MoveToEx(m_hDcMem, x0, y0, NULL);
				x1= x0; y1= x0; 
			}
			else LineTo(m_hDcMem, x0, y0);

//			PlotXYZ(DataZ[Z], DataY[(int)Z][(int)X], DataX[X]);
//			PlotXYZ(DataX[X], DataZ[Z], DataY[(int)Z][(int)X]);
//			PlotXYZ(DataZ[Z], DataX[X], DataY[(int)Z][(int)X]);
//			PlotXZ(X, DataZ[Z]);
		}
	}
	DeleteObject(hPen);
}


void  C3dRenderer::DrawBezierPyramid()
{
hDC= m_hDcMem;
	m_MaxValX= 300;
	m_MaxValY= 400;
	m_MaxValZ= 100;
	Init();
	DrawGrid();

       int vertices[5][3]={
			    {245,325,75},      //  base front left
			    {395,325,75},      //  base front right
			    {395,325,-75},     //  base back right
			    {245,325,-75},     //  base back left
			    {320,125,0}        //  top
			  };

       int edges[8][2]={
			 // first is starting point vertex number
			 // second is ending point vertex number
			 {0,3} , {3,2} , {2,1} , {1,0} , {0,4} ,
			 {3,4} , {2,4} , {1,4}
		       };

       int surfaces[5][5]={
			     // First entry is number of edges
			     // Rest entries are edge numbers
			     {4,0,1,2,3}, {3,0,4,5} , {3,1,5,6} ,
			     {3,2,6,7} , {3,3,4,7}
			  };

       for(int i=0;i<5;i++)
	  get_projected_point(vertices[i][0],vertices[i][1],vertices[i][2]);

	 draw_3d_polygon(vertices,edges,surfaces);
}

void C3dRenderer::DrawBezierCurve()
{
	m_MaxValX= 10;
	m_MaxValY= 10;
	m_MaxValZ= 10;
	Init();
	DrawGrid();

  int control_points[4][3]={0};
	int i= 0;
	float X= 0., Y= 0., Z= 0.;
	do
	{
		for (int count=0;count<=3;count++)
		{
			control_points[count][0]= X;  // x
			control_points[count][1]= Y;	// y
			control_points[count][2]= sqrt(100-((X+ 10)* (X+ 10)+ (Y+ 10)* (Y+ 10)));;	// z
			X++;
			Y++;
	  }

	  Bezier_curve(control_points);
		i++;
  }
  while(i < 10);
}
 /*************************************************************************/
 /*************************************************************************

 A C++ program to show a 3D object using Polygon-Mesh Representation.

  *************************************************************************/
 /*************************************************************************/

  /*************************************************************************

	  By :
		Muhammad Tahir Shahzad  [ MTS ]
		B.C.S Honours  [ 2000-04 ]
		Government College University Lahore
		Pakistan

      E-mail :  mtshome@wol.net.pk

    Web-Site :  www.mts-home.cjb.net  [ www.wol.net.pk/mtshome ]
		www.mtshome.cjb.net   [ www.geocities.com/mtahirshahzad ]

  *************************************************************************/

 /*************************************************************************/
 /*************************************************************************/
 //---------------------------  Header Files  ----------------------------//
 /*************************************************************************/
 /*************************************************************************/

// # include <iostream.h>
// # include <graphics.h>
// # include    <conio.h>
// # include     <math.h>

 /*************************************************************************/
 /*************************************************************************/
 //-----------------------  Global Declarations  -------------------------//
 /*************************************************************************/
 /*************************************************************************/

 # define Xc -0.3
 # define Yc  0.3
 # define Zc -1.0

 /*************************************************************************/
 /*************************************************************************/
 //-----------------------  Function Prototypes  -------------------------//
 /*************************************************************************/
 /*************************************************************************/


 void show_screen( );


 /*************************************************************************/
 /*************************************************************************/
 //------------------------------  main( )  ------------------------------//
 /*************************************************************************/
 /*************************************************************************/

 int Test( )
{

       int vertices[5][3]={
			    {245,325,75},      //  base front left
			    {395,325,75},      //  base front right
			    {395,325,-75},     //  base back right
			    {245,325,-75},     //  base back left
			    {320,125,0}        //  top
			  };

       int edges[8][2]={
			 // first is starting point vertex number
			 // second is ending point vertex number
			 {0,3} , {3,2} , {2,1} , {1,0} , {0,4} ,
			 {3,4} , {2,4} , {1,4}
		       };

       int surfaces[5][5]={
			     // First entry is number of edges
			     // Rest entries are edge numbers
			     {4,0,1,2,3}, {3,0,4,5} , {3,1,5,6} ,
			     {3,2,6,7} , {3,3,4,7}
			  };

       for(int i=0;i<5;i++)
	  get_projected_point(vertices[i][0],vertices[i][1],vertices[i][2]);

	 draw_3d_polygon(vertices,edges,surfaces);
/*
	   outtextxy(300,355,"S1");
	   outtextxy(260,270,"S2");
	   outtextxy(310,250,"S3");
	   outtextxy(370,280,"S4");
	   outtextxy(310,317,"S5");

	   outtextxy(255,315,"E1");
	   outtextxy(320,291,"E2");
	   outtextxy(403,315,"E3");
	   outtextxy(310,337,"E4");
	   outtextxy(250,250,"E5");
	   outtextxy(288,240,"E6");
	   outtextxy(390,240,"E7");
	   outtextxy(356,260,"E8");

	   outtextxy(212,344,"V1");
	   outtextxy(255,293,"V2");
	   outtextxy(420,293,"V3");
	   outtextxy(370,350,"V4");
	   outtextxy(320,115,"V5");
*/
//       getch( );
       return 0;
    }

 /*************************************************************************/
 /*************************************************************************/
 //------------------------  Funcion Definitions  ------------------------//
 /*************************************************************************/
 /*************************************************************************/

 /************************************************************************/
 //-------------------------  draw_3d_polygon( )  -----------------------//
 /************************************************************************/

 void draw_3d_polygon(int vertices[5][3],int edges[8][2],int surfaces[5][5])
    {
       for(int i=0;i<5;i++)
	  {
	     for(int j=1;j<=surfaces[i][0];j++)
		{
		   int x_1=vertices[edges[surfaces[i][j]][0]][0];
		   int y_1=vertices[edges[surfaces[i][j]][0]][1];
		   int x_2=vertices[edges[surfaces[i][j]][1]][0];
		   int y_2=vertices[edges[surfaces[i][j]][1]][1];

		   Line(x_1,y_1,x_2,y_2);
		}
	  }
    }

 /************************************************************************/
 //---------------------  get_projected_point( )  -----------------------//
 /************************************************************************/

 void get_projected_point(int& x,int& y,int& z)
    {
       float projection_matrix[4][4]={
				      { -Zc,0,0,0 },
				      { 0,-Zc,0,0 },
				      { Xc,Yc,0,1 },
				      { 0,0,0,-Zc }
				   };

       float matrix3d[4]={x,y,z,1};
       float matrix2d[4]={0,0,0,0};

       for(int count_1=0;count_1<4;count_1++)
	  {
	     for(int count_2=0;count_2<4;count_2++)
		matrix2d[count_1]+=
			   (matrix3d[count_2]*projection_matrix[count_2][count_1]);
	  }

       x=(int)(matrix2d[0]+0.5);
       y=(int)(matrix2d[1]+0.5);
       z=(int)(matrix2d[2]+0.5);
    }

 /*************************************************************************/
 //--------------------------  Line( )  ------------------------//
 /*************************************************************************/

 void Line(const int x_1, const int y_1,const int x_2,const int y_2)
    {
       int color= 0x00FF0000; //getcolor( );

       int x1=x_1;
       int y1=y_1;

       int x2=x_2;
       int y2=y_2;

       if(x_1>x_2)
	  {
	     x1=x_2;
	     y1=y_2;

	     x2=x_1;
	     y2=y_1;
	  }

       int dx=abs(x2-x1);
       int dy=abs(y2-y1);
       int inc_dec=((y2>=y1)?1:-1);

       if(dx>dy)
	  {
	     int two_dy=(2*dy);
	     int two_dy_dx=(2*(dy-dx));
	     int p=((2*dy)-dx);

	     int x=x1;
	     int y=y1;

	     SetPixel(hDC, x,y, color);

	     while(x<x2)
		{
		   x++;

		   if(p<0)
		      p+=two_dy;

		   else
		      {
			 y+=inc_dec;
			 p+=two_dy_dx;
		      }

		   SetPixel(hDC, x,y,color);
		}
	  }

       else
	  {
	     int two_dx=(2*dx);
	     int two_dx_dy=(2*(dx-dy));
	     int p=((2*dx)-dy);

	     int x=x1;
	     int y=y1;

	     SetPixel(hDC, x,y,color);

	     while(y!=y2)
		{
		   y+=inc_dec;

		   if(p<0)
		      p+=two_dx;

		   else
		      {
			 x++;
			 p+=two_dx_dy;
		      }

		   SetPixel(hDC, x,y,color);
		}
	  }
    }

 /*************************************************************************/
 //--------------------------  show_screen( )  ---------------------------//
 /*************************************************************************/


 void show_screen( )
 {
/*
       setfillstyle(1,1);
	 bar(205,26,438,38);

       settextstyle(0,0,1);
	 setcolor(15);
	   outtextxy(5,5,"******************************************************************************");
	   outtextxy(5,17,"*-**************************************************************************-*");
	   outtextxy(5,29,"*-----------------------                               ----------------------*");
	   outtextxy(5,41,"*-**************************************************************************-*");
	   outtextxy(5,53,"*-**************************************************************************-*");

	 setcolor(11);
	   outtextxy(214,29,"Polygon-Mesh Representation");

	 setcolor(15);

	   for(int count=0;count<=30;count++)
	      outtextxy(5,(65+(count*12)),"*-*                                                                        *-*");

	   outtextxy(5,438,"*-**************************************************************************-*");
	   outtextxy(5,450,"*---------------------------                         ------------------------*");
	   outtextxy(5,462,"******************************************************************************");

	 setcolor(12);
	   outtextxy(227,450,"  Press any key to exit ");
*/
}

 /*************************************************************************/
 /*************************************************************************/
 //-----------------------------  THE END  -------------------------------//
 /*************************************************************************/
 /*************************************************************************/



 /*************************************************************************/
 /*************************************************************************

 A C++ program to draw a 3D Cubic Bezier Curve.

  *************************************************************************/
 /*************************************************************************/

  /*************************************************************************

	  By :
		Muhammad Tahir Shahzad  [ MTS ]
		B.C.S Honours  [ 2000-04 ]
		Government College University Lahore
		Pakistan

      E-mail :  mtshome@wol.net.pk

    Web-Site :  www.mts-home.cjb.net  [ www.wol.net.pk/mtshome ]
		www.mtshome.cjb.net   [ www.geocities.com/mtahirshahzad ]

  *************************************************************************/

 /*************************************************************************/
 /*************************************************************************/
 //---------------------------  Header Files  ----------------------------//
 /*************************************************************************/
 /*************************************************************************/


 /*************************************************************************/
 /*************************************************************************/
 //-----------------------  Global Declarations  -------------------------//
 /*************************************************************************/
 /*************************************************************************/

 # define  f                 0.3
 # define  projection_angle   45

 /*************************************************************************/
 /*************************************************************************/
 //-----------------------  Function Prototypes  -------------------------//
 /*************************************************************************/
 /*************************************************************************/

 void show_screen( );

 double nCr(int,int);
 double factorial(int);

 void get_projected_point(float&,float&,float&);
 void multiply_matrices(const float[4],const float[4][4],float[4]);

 void Dashed_line(const int,const int,const int,const int,const int=0);

 /*************************************************************************/
 /*************************************************************************/
 //------------------------------  main( )  ------------------------------//
 /*************************************************************************/
 /*************************************************************************/

 int main( )
{

       int control_points[4][3]={0};
/*
    do
	  {

	     for(int count=0;count<=3;count++)
		{
		   gotoxy(8,10);
		   cout<<"Coordinates of Point-"<<count<<" (x"<<count<<",y"<<count<<",z"<<count<<") :";

		   gotoxy(8,11);
		   cout<<"ЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌЌ";

		   gotoxy(12,13);
		   cout<<"Enter the value of x"<<count<<" = ";
		   cin>>control_points[count][0];

		   gotoxy(12,15);
		   cout<<"Enter the value of y"<<count<<" = ";
		   cin>>control_points[count][1];

		   gotoxy(12,17);
		   cout<<"Enter the value of z"<<count<<" = ";
		   cin>>control_points[count][2];

		   gotoxy(8,10);
		   cout<<"                                            ";

		   gotoxy(12,13);
		   cout<<"                                            ";

		   gotoxy(12,15);
		   cout<<"                                            ";

		   gotoxy(12,17);
		   cout<<"                                            ";
	     }

	     initgraph(&driver,&mode,"..\\Bgi");

	     setcolor(15);
	       Bezier_curve(control_points);

	     setcolor(15);
	       outtextxy(110,460,"Press <Enter> to continue or any other key to exit.");

	     int key=int(getch( ));

	     if(key!=13)
		break;
	  }
       while(1);
*/
       return 0;
 }

 /*************************************************************************/
 /*************************************************************************/
 //------------------------  Funcion Definitions  ------------------------//
 /*************************************************************************/
 /*************************************************************************/

 /*************************************************************************/
 //--------------------------  Bezier_curve( )  --------------------------//
 /*************************************************************************/

 void Bezier_curve(const int cp[4][3])
    {
       int color= 0x00FF0000; //getcolor( );

       float x_1;
       float y_1;
       float z_1;
       float x_2;
       float y_2;
       float z_2;

       for(int count=0;count<3;count++)
	  {
	     x_1=cp[count][0];
	     y_1=cp[count][1];
	     z_1=cp[count][2];
	     x_2=cp[(count+1)][0];
	     y_2=cp[(count+1)][1];
	     z_2=cp[(count+1)][2];

	     get_projected_point(x_1,y_1,z_1);
	     get_projected_point(x_2,y_2,z_2);

	     Dashed_line((int)(x_1+0.5),(int)(y_1+0.5),
					     (int)(x_2+0.5),(int)(y_2+0.5));
	  }

       float x;
       float y;
       float z;

       for(float u=0.0005;u<=1;u+=0.0005)
	  {
	     x=0;
	     y=0;
	     z=0;

	     for(int k=0;k<=3;k++)
		{
		   x+=(cp[k][0]*nCr(3,k)*pow(u,k)*powl((1-u),(3-k)));
		   y+=(cp[k][1]*nCr(3,k)*pow(u,k)*powl((1-u),(3-k)));
		   z+=(cp[k][2]*nCr(3,k)*pow(u,k)*powl((1-u),(3-k)));
		}

	     get_projected_point(x,y,z);

	     SetPixel(hDC, (int)(x+0.5),(int)(y+0.5),color);
	  }
    }

 /*************************************************************************/
 //------------------------------  nCr( )  -------------------------------//
 /*************************************************************************/

 double nCr(int n,int r)
    {
       double nf;
       double rf;
       double nrf;
       double ncr;

       nf=factorial(n);
       rf=factorial(r);
       nrf=factorial((n-r));

       ncr=(nf/(rf*nrf));

       return ncr;
    }

 /*************************************************************************/
 //---------------------------  factorial( )  ----------------------------//
 /*************************************************************************/

 double factorial(int number)
    {
       double factorial=1;

       if(number==0 || number==1);

       else
	  {
	     for(int count=1;count<=number;count++)
		factorial=factorial*count;
	  }

       return factorial;
    }

 /************************************************************************/
 //---------------------  get_projected_point( )  -----------------------//
 /************************************************************************/

 void get_projected_point(float& x,float& y,float& z)
    {
       float fcos0=(f*cos(projection_angle*(M_PI/180)));
       float fsin0=(f*sin(projection_angle*(M_PI/180)));

       float Par_v[4][4]={
			    {1,0,0,0},
			    {0,1,0,0},
			    {fcos0,fsin0,0,0},
			    {0,0,0,1}
			 };

       float xy[4]={x,y,z,1};
       float new_xy[4]={0};

       multiply_matrices(xy,Par_v,new_xy);

       x=new_xy[0];
       y=new_xy[1];
       z=new_xy[2];
    }

 /************************************************************************/
 //----------------------  multiply_matrices( )  ------------------------//
 /************************************************************************/

 void multiply_matrices(const float matrix_1[4],
			      const float matrix_2[4][4],float matrix_3[4])
    {
       for(int count_1=0;count_1<4;count_1++)
	  {
	     for(int count_2=0;count_2<4;count_2++)
		matrix_3[count_1]+=
			   (matrix_1[count_2]*matrix_2[count_2][count_1]);
	  }
    }

 /*************************************************************************/
 //---------------------------  Dashed_line( )  --------------------------//
 /*************************************************************************/

 void Dashed_line(const int x_1,const int y_1,const int x_2,
					  const int y_2,const int line_type)
    {
       int count=0;
       int color= 0x00F0F0F0; // getcolor( );

       int x1=x_1;
       int y1=y_1;

       int x2=x_2;
       int y2=y_2;

       if(x_1>x_2)
	  {
	     x1=x_2;
	     y1=y_2;

	     x2=x_1;
	     y2=y_1;
	  }

       int dx=abs(x2-x1);
       int dy=abs(y2-y1);
       int inc_dec=((y2>=y1)?1:-1);

       if(dx>dy)
	  {
	     int two_dy=(2*dy);
	     int two_dy_dx=(2*(dy-dx));
	     int p=((2*dy)-dx);

	     int x=x1;
	     int y=y1;

	     SetPixel(hDC, x,y,color);

	     while(x<x2)
		{
		   x++;

		   if(p<0)
		      p+=two_dy;

		   else
		      {
			 y+=inc_dec;
			 p+=two_dy_dx;
		      }

		   if((count%2)!=0 && line_type==0)
		      SetPixel(hDC, x,y,color);

		   else if((count%5)!=4 && line_type==1)
		      SetPixel(hDC, x,y,color);

		   else if((count%10)!=8 && (count%10)!=9 && line_type==2)
		      SetPixel(hDC, x,y,color);

		   else if((count%20)!=18 && (count%20)!=19 && line_type==3)
		      SetPixel(hDC, x,y,color);

		   else if((count%12)!=7 && (count%12)!=8 &&
			    (count%12)!=10 && (count%12)!=11 && line_type==4)
		      SetPixel(hDC, x,y,color);

		   count++;
		}
	  }

       else
	  {
	     int two_dx=(2*dx);
	     int two_dx_dy=(2*(dx-dy));
	     int p=((2*dx)-dy);

	     int x=x1;
	     int y=y1;

	     SetPixel(hDC, x,y,color);

	     while(y!=y2)
		{
		   y+=inc_dec;

		   if(p<0)
		      p+=two_dx;

		   else
		      {
			 x++;
			 p+=two_dx_dy;
		      }

		   if((count%2)!=0 && line_type==0)
		      SetPixel(hDC, x,y,color);

		   else if((count%5)!=4 && line_type==1)
		      SetPixel(hDC, x,y,color);

		   else if((count%10)!=8 && (count%10)!=9 && line_type==2)
		      SetPixel(hDC, x,y,color);

		   else if((count%20)!=18 && (count%20)!=19 && line_type==3)
		      SetPixel(hDC, x,y,color);

		   else if((count%12)!=7 && (count%12)!=8 &&
			    (count%12)!=10 && (count%12)!=11 && line_type==4)
		      SetPixel(hDC, x,y,color);

		   count++;
		}
	  }
    }

 /*************************************************************************/
 //--------------------------  show_screen( )  ---------------------------//
 /*************************************************************************/

 /*************************************************************************/
 /*************************************************************************/
 //-----------------------------  THE END  -------------------------------//
 /*************************************************************************/
 /*************************************************************************/

