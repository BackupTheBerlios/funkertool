// Funker.cpp : Definiert den Einsprungpunkt für die Anwendung.
//

#include "Funker.h"
#include <fstream>
#include "../FunctionParser/FunctionParser.h"

#pragma pack(push, 1)

struct	WaveHeader
{
	unsigned long	ChunkID;		// = 0x52494646 = "RIFF"
	unsigned long	ChunkDataSize;	// (file size) - 8
	unsigned long	RiffType;		// = 0x57415645 = "WAVE"
};

struct	RiffChunk
{
	unsigned long	ChunkID;		// 0x666D7420 = "fmt "
	unsigned long	ChunkDataSize;	// = 16
	unsigned short	CompressionCode;// 0x01 = none = PCM
	unsigned short	Channels;
	unsigned long	SampleRate;
	unsigned long	AverageBPS;
	unsigned short	BlockAlign;
	unsigned short	BitsPerSample;
};

struct	DataChunk
{
	unsigned long	ChunkID;		// 0x64617461 = "data"
	unsigned long	ChunkSize;
};


#pragma	pack(pop)


#define MAX_LOADSTRING 100
char		szClassName[]		= "DLGCLASS";
char		szDlgName[]			= "Main_Dlg_Wnd";
char		szMenuName[]		= "MAIN_MENU";
WndHandler*	WndHnd				= 0;

// Globale Variablen:
HINSTANCE	hInst;					// aktuelle Instanz

//============================================================================

FuncWindow::FuncWindow( )
{
	this->hFunc		= 0;
	this->hWindow	= 0;
	this->lpName	= 0;
	this->fScaleX	= 1.f;
	this->fScaleY	= 1.f;
	this->fMidX		= 250.f;
	this->fMidY		= 0.f;

	this->bDragMode	= false;
	this->bScaleMode= false;
}

FuncWindow::~FuncWindow()
{
	if ( this->hFunc )
		delete this->hFunc;
	if ( this->lpName )
		delete[] this->lpName;
}

WndHandler::WndHandler( HINSTANCE	hInst,
					    int			nCmdShow,
						char*		szDlgName,
						WNDPROC		WndProc,
						LPCTSTR		Icon,
						char*		szMenuName)
{
	this->hInst		= hInst;
	this->nCmdShow	= nCmdShow;
	this->szDlgName = szDlgName;
	for ( int i=0; i<MAX_WND_COUNT; i++)
		this->hWindows[i]	= 0;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= DLGWINDOWEXTRA;
	wcex.hInstance		= hInst;
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= szMenuName;
	wcex.lpszClassName  = szClassName;
	wcex.hIcon			= LoadIcon(hInst, (LPCTSTR)Icon);
	wcex.hIconSm		= LoadIcon(hInst, (LPCTSTR)Icon);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	RegisterClassEx(&wcex);
}

HWND WndHandler::AddWindow( )
{
	int i = 0;
	while ( (i<MAX_WND_COUNT) && (this->hWindows[i]) )
		i++;
	if ( i==MAX_WND_COUNT )
		return 0;

	this->hWindows[i] = new FuncWindow;
	this->hWindows[i]->hWindow = CreateDialogParam( this->hInst, this->szDlgName, NULL, NULL, NULL);

	if( !this->hWindows[i] )
	{
		return 0;
	}

	ShowWindow( this->hWindows[i]->hWindow, SW_SHOWNORMAL);//nCmdShow );
	UpdateWindow( this->hWindows[i]->hWindow );
	this->hWindows[i]->lpName = new char[100];
	sprintf( this->hWindows[i]->lpName, "Funker (#%d)", i+1 );
	SetWindowText( this->hWindows[i]->hWindow, this->hWindows[i]->lpName );
	return this->hWindows[i]->hWindow;
}

bool	WndHandler::HasWindows( )
{
	int i = 0;
	while ( (i<MAX_WND_COUNT) )
	{
		if ( this->hWindows[i]!=0 )
			return true;
		i++;
	}
	return false;
}

FuncWindow*	WndHandler::GetFuncWindow( HWND hWnd )
{
	int i = 0;
	while ( i<MAX_WND_COUNT )
	{
		if ( (this->hWindows[i]!=0) && (this->hWindows[i]->hWindow==hWnd) )
			return this->hWindows[i];
		i++;
	}
	return 0;
}

int	WndHandler::GetWindowNr( HWND hWnd )
{
	int i = 0;
	while ( i<MAX_WND_COUNT )
	{
		if ( (this->hWindows[i]!=0) && (this->hWindows[i]->hWindow==hWnd) )
			return i;
		i++;
	}
	return -1;
}

HWND	WndHandler::GetWindow( int Nr )
{
	if ( (Nr<MAX_WND_COUNT) && (this->hWindows[Nr]) )
		return this->hWindows[Nr]->hWindow;
	return 0;
}

bool	WndHandler::KillWindow( HWND hWnd )
{
	int i = 0;
	while ( i<MAX_WND_COUNT )
	{
		if ( (this->hWindows[i]!=0) && (this->hWindows[i]->hWindow==hWnd) )
			break;
		i++;
	}
	if ( i==MAX_WND_COUNT )
		return false;
	DestroyWindow( this->hWindows[i]->hWindow );
	delete this->hWindows[i];
	this->hWindows[i] = 0;
	return true;
}

//============================================================================

// Vorausdeklarationen von Funktionen, die in diesem Code-Modul enthalten sind:
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );

//============================================================================

void	SetAllDropDownEntries( )
{
	// find out all windows
	FuncWindow*	fws[MAX_WND_COUNT];
	int i;
	for ( i=0; i<MAX_WND_COUNT; i++)
	{
		fws[i] = 0;
		HWND hwnd = WndHnd->GetWindow( i );
		if (!hwnd)
			continue;
		FuncWindow* fw = WndHnd->GetFuncWindow( hwnd );
		if (!fw)
			continue;
		fws[i] = fw;
	}

	// trace all windows
	for ( i=0; i<MAX_WND_COUNT; i++)
	{
		if ( !fws[i] )
			continue;
		HWND dd = GetDlgItem(fws[i]->hWindow,IDC_GRAB_FROM);

		// find the actual selected entry
		int cursel = SendMessage( dd, CB_GETCURSEL, 0, 0 );
		int func;
		if ( cursel!=-1)
			func = *(int*)SendMessage( dd, CB_GETITEMDATA, cursel, 0 );
		else
			func = -1;

		// remove all entries in the drop-down
		int j, count = SendMessage( dd, CB_GETCOUNT, 0, 0 );
		for ( j=0; j<count; j++)
		{
			int* rem = (int*)SendMessage( dd, CB_GETITEMDATA, 0, 0);
			delete rem;
			SendMessage( dd, CB_DELETESTRING, 0, 0);
		}
		// put standard-entries
		SendMessage( dd, CB_INSERTSTRING, -1, (long)("x"));
		SendMessage( dd, CB_INSERTSTRING, -1, (long)("x²"));
		SendMessage( dd, CB_INSERTSTRING, -1, (long)("keine"));
		int* ip = new int;
		*ip = -1;
		SendMessage( dd, CB_SETITEMDATA, 0, (long)ip);
		ip = new int;
		*ip = -2;
		SendMessage( dd, CB_SETITEMDATA, 1, (long)ip);
		ip = new int;
		*ip = -3;
		SendMessage( dd, CB_SETITEMDATA, 2, (long)ip);

		// put all dynamic entries
		int cs = 0;
		for ( j=0; j<MAX_WND_COUNT; j++)
		{
			if ( (!fws[j]) || (i==j) )
				continue;
			SendMessage( dd, CB_INSERTSTRING, -1, (long)(fws[j]->lpName));
			ip = new int;
			*ip = j;
			SendMessage( dd, CB_SETITEMDATA, 3+cs++, (long)ip);
		}

		// create the selection
		count = SendMessage( dd, CB_GETCOUNT, 0, 0 );
		if ( func<0 )
			SendMessage( dd, CB_SETCURSEL, -func-1, 0 );
		else
			SendMessage( dd, CB_SETCURSEL, 3+func, 0 );
	}
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
 	// ZU ERLEDIGEN: Fügen Sie hier den Code ein.
	MSG msg;
	HACCEL hAccelTable;

	WndHnd = new WndHandler( hInstance, nCmdShow, szDlgName, (WNDPROC)WndProc, (LPCSTR)IDI_FUNKER, szMenuName );
	// Initialisierung der Anwendung durchführen:
	HWND hwnd;
	if( !(hwnd=WndHnd->AddWindow( )) )
	{
		return FALSE;
	}
	SetTimer( hwnd, 0, 50, NULL);

	InitCommonControls();
	SetAllDropDownEntries();
	SendMessage( GetDlgItem(hwnd,IDC_GRAB_FROM), CB_SETCURSEL, 2, 0 );

	hAccelTable = LoadAccelerators(hInstance,  MAKEINTRESOURCE(IDR_FUNKER) );

	// Hauptnachrichtenschleife:
	while (WndHnd->HasWindows())
	{
		while( GetMessage(&msg, NULL, 0, 0) )
		{
			if( !TranslateAccelerator (msg.hwnd, hAccelTable, &msg) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
	}
	return msg.wParam;
}


void	Draw( HWND hWnd )
{
	RECT	rc;

	HWND hwnd = GetDlgItem( hWnd, IDC_GRAPH );
	HDC wdc	= GetWindowDC(hwnd);    //wie GetDC nur fürs ganze Fenster
	GetWindowRect(hwnd,&rc);
	FuncWindow* fw = WndHnd->GetFuncWindow( hWnd );
	if ( ! (fw && (fw->hFunc)) )
		return;

	// Fenster auf weiß
	BitBlt( wdc,1,1,rc.right-rc.left-2,rc.bottom-rc.top-2,NULL,0,0,WHITENESS);
	HPEN OutaPen = CreatePen(PS_SOLID,1,RGB(100,150,250));
	HPEN BlackPen = CreatePen(PS_SOLID,1, RGB(0,0,0) );

	// draw coordinate-system
	HGDIOBJ oldObj = SelectObject( wdc, BlackPen );
	// draw the y-achse
	int width  = rc.right-rc.left;
	float midx = 0.5f*width - fw->fMidX/fw->fScaleX;
	if ( (midx>0) && (midx<width-1) )
	{
		MoveToEx( wdc, midx, 1, NULL);
		LineTo( wdc, midx, rc.bottom-rc.top-1);
	}

	// draw horizontal in the middle
	MoveToEx(wdc,1,(rc.bottom-rc.top)/2, NULL);
	LineTo(wdc,rc.right-rc.left-1,(rc.bottom-rc.top)/2);

	// Draw the function itself
	SelectObject( wdc,OutaPen);
	int ih = (rc.bottom-rc.top-2)/2+1;
	MoveToEx( wdc, 1, MAX(ih-(int)((1/fw->fScaleY)*fw->hValues[0]),1), 0 );

	for ( int i=1; (i<1000) && (i<rc.right-rc.left-1); i++)
		LineTo( wdc, i+1, MAX(ih-(int)((1/fw->fScaleY)*fw->hValues[i]),1) );

	// release all
	SelectObject( wdc,oldObj);
	if (DeleteObject( OutaPen) && DeleteObject(BlackPen) )
	{
		ReleaseDC(hwnd,wdc);
	}
}

float	Calculate( HWND hWnd, float x )
{
	HWND dd		= GetDlgItem( hWnd, IDC_GRAB_FROM );
	int cursel	= (int)SendMessage( dd, CB_GETCURSEL, 0, 0 );
	int func	= *(int*)SendMessage( dd, CB_GETITEMDATA, cursel, 0 );
	FuncWindow*	wf = WndHnd->GetFuncWindow( hWnd );
	if ( wf->hFunc )
	{
		if ( func==-1 )
		{
			float y = wf->hFunc->Evaluate(x);
			return y;
		}
		if ( func==-2 )
		{
			float y = wf->hFunc->Evaluate(x*x);
			return y;
		}
		if ( func>=0 )
		{
			float y = Calculate( WndHnd->GetWindow( func), x );
			return y;
		}
	}
	return 0;
}

//===============================================================

char*	GetRecursiveFunc( int WndNr )
{
	// grab the window to the given Nr
	HWND hWnd  = WndHnd->GetWindow( WndNr );

	// grab the string for the function of this window
	char* text = new char[1000];
	GetWindowText( GetDlgItem( hWnd, IDC_SOURCE ), text, 999);

	// get the selected type-info
	HWND dd		= GetDlgItem( hWnd, IDC_GRAB_FROM );
	int cursel	= (int)SendMessage( dd, CB_GETCURSEL, 0, 0 );
	int func	= *(int*)SendMessage( dd, CB_GETITEMDATA, cursel, 0 );
	char* varx[4]	= { "",
						"defsub(x,%s)",
						"defsub(x*x,%s)",
						"%s" };
	char* funcstr = 0;
	if ( func<0 )
	{
		funcstr	= new char[strlen(varx[-func])-2+strlen(text)+1];
		sprintf( funcstr, varx[-func], text );
	}
	else
	{
		char*	printer = "defsub(%s,%s)";
		char*	defsub	= GetRecursiveFunc( func );
		funcstr	= new char[strlen(printer)-4+strlen(defsub)+strlen(text)+1];
		sprintf( funcstr, printer, defsub, text );
		delete[] defsub;
	}
	delete[] text;
	return funcstr;
}

//===============================================================

void	GrabTheFunction( HWND hWnd )
{
	int func		= WndHnd->GetWindowNr( hWnd );
	char* funcstr	= GetRecursiveFunc( func );
	FuncWindow* fw	= WndHnd->GetFuncWindow( hWnd );
	if ( !fw )
		return;
	if ( fw->hFunc )
		delete fw->hFunc;
	FunctionParser  s2f;
	fw->hFunc = s2f.Parse(funcstr);
	delete[] funcstr;
}

void	CalculateAll( HWND hWnd, int Vals )
{
	FuncWindow*	wf = WndHnd->GetFuncWindow( hWnd );
	if ( !wf )
		return;
	float left	= wf->fMidX - wf->fScaleX*(0.5f*Vals);
	float right	= wf->fMidX + wf->fScaleX*(0.5f*Vals);
	if ( !wf->hFunc )
	{
		for (int i=0; i<Vals; i++)
			wf->hValues[i] = 0;
	}
	else
	{
		for (int i=0; i<Vals; i++)
		{
			float x = left + i*(right-left)/Vals;
			wf->hValues[i] = wf->hFunc->Evaluate(x);
		}
	}
}

//
//  FUNKTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  AUFGABE:  Verarbeitet Nachrichten für das Hauptfenster.
//
//  WM_COMMAND	- Anwendungsmenü verarbeiten
//  WM_PAINT	- Hauptfenster darstellen
//  WM_DESTROY	- Beendigungsnachricht ausgeben und zurückkehren
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	static DWORD Time = GetTickCount();
	DWORD aTime = GetTickCount();

	static char* warnings = NULL;
	switch( message )
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint( hWnd ,&ps);
			EndPaint( hWnd, &ps);
			DeleteDC( hdc );
			if ( Time+20<aTime )
			{
				Time = aTime;
				Draw( hWnd );
			}
			break;
		}/**/

		case WM_LBUTTONDOWN:
			{
				FuncWindow* fw = WndHnd->GetFuncWindow( hWnd );
				GetCursorPos( &fw->pMouseStart );
				RECT rc;
				GetWindowRect( GetDlgItem(hWnd, IDC_GRAPH), &rc);
				if ( (rc.left<=fw->pMouseStart.x)
					&& (rc.right>=fw->pMouseStart.x)
					&& (rc.top<=fw->pMouseStart.y)
					&& (rc.bottom>=fw->pMouseStart.y) )
					fw->bScaleMode = true;
			}
			break;

		case WM_LBUTTONUP:
			{
				FuncWindow* fw = WndHnd->GetFuncWindow( hWnd );
				fw->bScaleMode = false;
			}
			break;

		case WM_RBUTTONDOWN:
			{
				FuncWindow* fw = WndHnd->GetFuncWindow( hWnd );
				GetCursorPos( &fw->pMouseStart );
				RECT rc;
				GetWindowRect( GetDlgItem(hWnd, IDC_GRAPH), &rc);
				if ( (rc.left<=fw->pMouseStart.x)
					&& (rc.right>=fw->pMouseStart.x)
					&& (rc.top<=fw->pMouseStart.y)
					&& (rc.bottom>=fw->pMouseStart.y) )
					fw->bDragMode = true;
			}
			break;

		case WM_RBUTTONUP:
			{
				FuncWindow* fw = WndHnd->GetFuncWindow( hWnd );
				fw->bDragMode = false;
			}
			break;

		case WM_MOUSEMOVE:
			{
				FuncWindow* fw = WndHnd->GetFuncWindow( hWnd );
				POINT cur;
				static POINT old;
				RECT rc;
				GetWindowRect( GetDlgItem(hWnd, IDC_GRAPH), &rc);
				GetCursorPos( &cur );
				if ( (rc.left<=cur.x)
					&& (rc.right>=cur.x)
					&& (rc.top<=cur.y)
					&& (rc.bottom>=cur.y) )
				{
					float midx = fw->fMidX + (cur.x-0.5f*(rc.right+rc.left))*fw->fScaleX;
					float midy = fw->fMidY + (-cur.y+0.5f*(rc.top+rc.bottom))*fw->fScaleY;
					char text[100];
					sprintf(text,"(%.4f/%.4f)", midx, midy );
					SetWindowText( GetDlgItem(hWnd,IDC_POSITION),text);
					sprintf(text,"(%.4f/%.4f)", 1/fw->fScaleX, 1/fw->fScaleY );
					SetWindowText( GetDlgItem(hWnd,IDC_SCALE),text);
					sprintf(text,"%.5f", fw->hValues[cur.x+1-rc.left] );
					SetWindowText( GetDlgItem(hWnd,IDC_VALUE),text);
				}
				if ( (!fw->bScaleMode) && (!fw->bDragMode) )
				{
					break;
				}
				GetCursorPos( &cur );
				if ( (old.x==cur.x) && (old.y==cur.y) )
					break;
				HWND gw = GetDlgItem( hWnd, IDC_GRAPH );
				GetWindowRect( gw, &rc );
				if ( fw->bScaleMode )
				{
					fw->fScaleX += 0.001f*fw->fScaleX*(fw->pMouseStart.x-cur.x);
					fw->fScaleY += 0.001f*fw->fScaleY*(cur.y-fw->pMouseStart.y);
				}
				if ( fw->bDragMode )
				{
					fw->fMidX += fw->fScaleX*(cur.x-fw->pMouseStart.x);
				}
				if ( Time+20<aTime )
				{
					Time = aTime;
					CalculateAll( hWnd, rc.right-rc.left-1 );
					Draw( hWnd );
				}
				SetCursorPos( fw->pMouseStart.x, fw->pMouseStart.y );
				old = cur;
				break;
			}
			break;

		case WM_TIMER:
			Draw( hWnd );
			KillTimer( hWnd, 0);
			break;

		case WM_ACTIVATE:
		{
			SetTimer( hWnd, 0, 20, 0 );
			if ( Time+20<aTime )
			{
				Time = aTime;
				Draw( hWnd );
			}
			break;
		}/**/

		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			if (wmEvent==BN_CLICKED)
			{
				switch( wmId )
				{
					case IDC_CALCULATE:
						{
							HWND gw = GetDlgItem( hWnd, IDC_GRAPH );
							RECT rc;
							GetWindowRect( gw, &rc );
							// calculate all values
							GrabTheFunction( hWnd );
							CalculateAll( hWnd, rc.right-rc.left-1 );
							Draw( hWnd );
						}
						break;

					case IDC_NEW_WINDOW:
						{
						HWND hwnd = WndHnd->AddWindow( );
						if (hwnd)
						{
							SetTimer( hwnd, 0, 50, NULL);
							SetAllDropDownEntries();
							SendMessage( GetDlgItem(hwnd,IDC_GRAB_FROM), CB_SETCURSEL, 2, 0 );
						}
						}
						break;

					case IDC_QUIT:
						{
							for ( int i=0; i<MAX_WND_COUNT; i++)
								WndHnd->KillWindow( WndHnd->GetWindow( i ) );
						}

					case IDM_CLOSE_WINDOW:
						WndHnd->KillWindow( hWnd );
						break;

					case IDM_EXPORT_FUNCTION:
						{
							std::ofstream out( "text.fet", std::ios::out|std::ios::binary);
							WndHnd->GetFuncWindow( hWnd )->hFunc->Stream( out );
							out.close();
						}

					case IDM_EXPORT_AS_WAV:
						{
							int length  = 100000;
							RiffChunk rc;
							rc.ChunkID = ' tmf';		// 0x666D7420 = "fmt "
							rc.ChunkDataSize = 16;	// = 16
							rc.CompressionCode = 0x01;// 0x01 = none = PCM
							rc.Channels = 1;
							rc.SampleRate = 44100;
							rc.BitsPerSample = 16;
							rc.AverageBPS = rc.SampleRate*rc.BitsPerSample*rc.Channels;
							rc.BlockAlign = rc.BitsPerSample/8*rc.Channels;
							DataChunk dc;
							dc.ChunkID = 'atad';
							dc.ChunkSize	= length*rc.BitsPerSample/8*rc.Channels;
							WaveHeader wh;
							wh.ChunkID			= 'FFIR';
							wh.RiffType			= 'EVAW';
							wh.ChunkDataSize	= sizeof(rc)+sizeof(wh)+sizeof(dc)+dc.ChunkSize-8;
							short* datas = new short[length];
							ScriptFunction* sf = WndHnd->GetFuncWindow( hWnd )->hFunc;
							for ( int i=0; i<length; i++)
								datas[i] = static_cast<short>(sf->Evaluate(i));
							std::ofstream out( "test.wav", std::ios::out|std::ios::binary );
							out.write( reinterpret_cast<char*>(&wh), sizeof(wh) );
							out.write( reinterpret_cast<char*>(&rc), sizeof(rc) );
							out.write( reinterpret_cast<char*>(&dc), sizeof(dc) );
							out.write( reinterpret_cast<char*>(datas), dc.ChunkSize );
							out.flush();
							out.close();
						}
						break;
				}
			}
		//	SetFocus(hDlg);
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		case WM_CLOSE:
			WndHnd->KillWindow( hWnd );
			return DefWindowProc( hWnd, message, wParam, lParam );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
   }
   return 0;
}
