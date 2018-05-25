#include <windows.h>
#include <tchar.h>

static TCHAR FN[MAX_PATH+1];

TCHAR *SelectFileToSave(HWND hOwnerWnd, TCHAR *DlgTitle, TCHAR *FileExts, TCHAR **ppShortFN)
{
OPENFILENAMEA OSrcFNInfo;
DWORD ErrCode;
int i;

	FN[0]= 0;
  memset(&OSrcFNInfo,0,sizeof(OPENFILENAME));
  OSrcFNInfo.lStructSize = sizeof(OPENFILENAME);
  OSrcFNInfo.hwndOwner = hOwnerWnd;
  OSrcFNInfo.lpstrFile = FN;
  OSrcFNInfo.nMaxFile = MAX_PATH; //* sizeof(TCHAR);
//	OSrcFNInfo.lpstrDefExt= vFileExt;
//  OSrcFNInfo.lpstrFilter = ("bin, hex, flv files\0*.bin;*.hex;*.flv\0bin files\0*.bin\0hex files\0*.hex\0flv files\0*.flv\0\0");				
  OSrcFNInfo.lpstrFilter = FileExts;				
//  OSrcFNInfo.nFilterIndex = FilterCnt;
  OSrcFNInfo.lpstrTitle = DlgTitle;					
  OSrcFNInfo.lpstrInitialDir = NULL; //SrcFN;
  OSrcFNInfo.Flags = /*OFN_ALLOWMULTISELECT |*/ OFN_EXPLORER | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;

  if (!GetSaveFileNameA(&OSrcFNInfo))
	{
		ErrCode= CommDlgExtendedError();
    FN[0]= 0;
		switch (ErrCode)
		{
			case 0:
			break;
			case CDERR_DIALOGFAILURE: 
			break;
			case CDERR_FINDRESFAILURE: 
			break;
			case CDERR_INITIALIZATION: 
			break;
			case CDERR_LOADRESFAILURE: 
			break;
			case CDERR_LOADSTRFAILURE: 
			break;
			case CDERR_LOCKRESFAILURE: 
			break;
			case CDERR_MEMALLOCFAILURE: 
			break;
			case CDERR_MEMLOCKFAILURE: 
			break;
			case CDERR_NOHINSTANCE: 
			break;
			case CDERR_NOHOOK: 
			break;
			case CDERR_NOTEMPLATE:
			break;
			case CDERR_STRUCTSIZE: 
			break;
			case FNERR_BUFFERTOOSMALL:
        ::MessageBox(hOwnerWnd, _T("Too many files selected.\r\nTry again"), _T("Error"), MB_OK);
			break;
			case FNERR_INVALIDFILENAME: 
			break;
			case FNERR_SUBCLASSFAILURE:
			break;
			default:	
        ::MessageBox(hOwnerWnd, _T("GetOpenFileName failed"), _T("Unknown Error Code"), MB_OK);
		}
		return NULL;
	}
	else
	{
	}
	if (ppShortFN != NULL)
	{
		i= _tcslen(FN);
		if (i > 0)
		{
			while (i > 0)
			{
				if (FN[i] == '\\') 
				{ 
					i++; 
					*ppShortFN= &FN[i];
					break; 
				}
				i--;
			}
		} 
		else
			return NULL; // strange
	}
}

TCHAR *SelectFileToOpen(HWND hOwnerWnd, TCHAR *DlgTitle, TCHAR *FileExts, TCHAR **ppShortFN)
{
OPENFILENAME OSrcFNInfo;
DWORD ErrCode;
int i;

	FN[0]= 0;
  memset(&OSrcFNInfo,0,sizeof(OPENFILENAME));
  OSrcFNInfo.lStructSize = sizeof(OPENFILENAME);
  OSrcFNInfo.hwndOwner = hOwnerWnd;
  OSrcFNInfo.lpstrFile = FN;
  OSrcFNInfo.nMaxFile = MAX_PATH; //* sizeof(TCHAR);
//	OSrcFNInfo.lpstrDefExt= vFileExt;
//  OSrcFNInfo.lpstrFilter = ("bin, hex, flv files\0*.bin;*.hex;*.flv\0bin files\0*.bin\0hex files\0*.hex\0flv files\0*.flv\0\0");				
  OSrcFNInfo.lpstrFilter = FileExts;				
//  OSrcFNInfo.nFilterIndex = FilterCnt;
  OSrcFNInfo.lpstrTitle = DlgTitle;					
  OSrcFNInfo.lpstrInitialDir = NULL; //SrcFN;
  OSrcFNInfo.Flags = /*OFN_ALLOWMULTISELECT |*/ OFN_EXPLORER | OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;

  if (!GetOpenFileNameA(&OSrcFNInfo))
	{
		ErrCode= CommDlgExtendedError();
    FN[0]= 0;
		switch (ErrCode)
		{
			case 0:
			break;
			case CDERR_DIALOGFAILURE: 
			break;
			case CDERR_FINDRESFAILURE: 
			break;
			case CDERR_INITIALIZATION: 
			break;
			case CDERR_LOADRESFAILURE: 
			break;
			case CDERR_LOADSTRFAILURE: 
			break;
			case CDERR_LOCKRESFAILURE: 
			break;
			case CDERR_MEMALLOCFAILURE: 
			break;
			case CDERR_MEMLOCKFAILURE: 
			break;
			case CDERR_NOHINSTANCE: 
			break;
			case CDERR_NOHOOK: 
			break;
			case CDERR_NOTEMPLATE:
			break;
			case CDERR_STRUCTSIZE: 
			break;
			case FNERR_BUFFERTOOSMALL:
        ::MessageBox(hOwnerWnd, _T("Too many files selected.\r\nTry again"), _T("Error"), MB_OK);
			break;
			case FNERR_INVALIDFILENAME: 
			break;
			case FNERR_SUBCLASSFAILURE:
			break;
			default:	
        ::MessageBox(hOwnerWnd, _T("GetOpenFileName failed"), _T("Unknown Error Code"), MB_OK);
		}
		return NULL;
	}
	else
	{
	}
	if (ppShortFN != NULL)
	{
		i= _tcslen(FN);
		if (i > 0)
		{
			while (i > 0)
			{
				if (FN[i] == '\\') 
				{ 
					i++; 
					*ppShortFN= &FN[i];
					break; 
				}
				i--;
			}
		} 
		else
			return NULL; // strange
	}
	return FN;
}

