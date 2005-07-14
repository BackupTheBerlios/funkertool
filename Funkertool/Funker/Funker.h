
#if !defined(AFX_FUNKER_H__3E9798C7_B791_4C88_B0AE_1A608501AA94__INCLUDED_)
#define AFX_FUNKER_H__3E9798C7_B791_4C88_B0AE_1A608501AA94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include <windows.h>
#include "../ScriptFunctions/ScriptFunctions.h"
#include <COMMCTRL.h>

class	FuncWindow
{
public:
	FuncWindow			();
	~FuncWindow			();
	HWND				hWindow;
	char*				lpName;
	ScriptFunction*		hFunc;
	float				hValues[1000];
	float				fMidX;
	float				fMidY;
	float				fScaleX;
	float				fScaleY;

	bool				bDragMode;
	bool				bScaleMode;
	POINT				pMouseStart;
};

const	char	MAX_WND_COUNT	= 15;
class	WndHandler
{
public:
	WndHandler ( HINSTANCE	hInst,
				 int		nCmdShow,
				 char*		szDlgName,
				 WNDPROC	WndProc,
				 LPCTSTR	Icon,
				 char*		szMenuName);

	bool		HasWindows		( );
	HWND		AddWindow		( );
	bool		KillWindow		( HWND hWnd );
	FuncWindow*	GetFuncWindow	( HWND hWnd );
	int			GetWindowNr		( HWND hWnd );
	HWND		GetWindow		( int Nr );
private:
	char*		szDlgName;
	HINSTANCE	hInst;
	int			nCmdShow;
	FuncWindow*	hWindows[MAX_WND_COUNT];
};

#endif // !defined(AFX_FUNKER_H__3E9798C7_B791_4C88_B0AE_1A608501AA94__INCLUDED_)