#ifndef WT_DEFAULT

#ifndef UNUSE_RAW_INPUT
// 低レベル入力を使う
#ifndef WINVER
#define WINVER (0x500)        // RawInput用
#define _WIN32_WINNT 0x0501 // RawInput用
#endif
#endif
#include <winsock2.h>
#include <windows.h>

#ifndef _UNLINK_LIBRARY
#  pragma comment( lib, "winmm.lib" )
#  ifdef _MIKANDEV
#    ifdef _DEBUG
#      pragma comment( lib, "Dev/WindowD.lib" )
#    else
#      pragma comment( lib, "Dev/Window.lib" )
#    endif
#  else
#    ifdef _DEBUG
#      pragma comment( lib, "WindowD.lib" )
#    else
#      pragma comment( lib, "Window.lib" )
#    endif
#  endif
#endif

//ウィンドウタイプの設定
enum WINDOWTYPE
{
	WT_RENEWAL            = 0, // 現在の設定を維持
	WT_DEFAULT            = 1, // リサイズ不可のウィンドウ
	WT_NORMAL             = 2, // リサイズなども可能なウィンドウ
	WT_NORESIZEFULLSCREEN = 3, // 画面設定を変更しないフルスクリーン
	WT_NOFRAME            = 4, // 枠のないウィンドウ
	WT_FULLSCREEN         = 5, // フルスクリーン
};

//ウィンドウスタイルの定義
//#define WS_DEFAULT WS_OVERLAPPED|WS_VISIBLE |WS_BORDER | WS_CAPTION
//#define WS_DEFAULT WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
//#define WS_DEFAULT WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
#define WS_DEFAULT            WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
#define WS_NORMAL             WS_OVERLAPPEDWINDOW | WS_VISIBLE
#define WS_NORESIZEFULLSCREEN WS_POPUP | WS_VISIBLE
#define WS_NOFRAME            WS_NORESIZEFULLSCREEN
#define WS_FULLSCREEN         WS_NORESIZEFULLSCREEN

//ウィンドウの初期値
#define WF_WIDTH 640
#define WF_HEIGHT 480

//型を作る
typedef struct WINDOWDATA_ WINDOWDATA;
typedef WINDOWDATA * WindowData;

#define WBUFLEN 256
#define MIKANCLASSNAME "MIKANWINDOW"
/*!
@struct  WINDOWDATA_
@brief   ウィンドウ生成などの変数を集めた構造体
@author  Hiroki
@version 1.0
*/
struct WINDOWDATA_
{
	//char          *ClassName;//[ WBUFLEN ]; //
	//char          ClassNameW[ WBUFLEN ]; //
	char          WindowName[ WBUFLEN ];    //
	unsigned long WindowStyle;    //ウィンドウスタイル
	unsigned long WindowStyleEx;  //拡張ウィンドウスタイル
	int           PositionX;      //ウィンドウx座標
	int           PositionY;      //ウィンドウy座標
	unsigned int  Width;          //描写領域の横幅
	unsigned int  Height;         //描写領域の高さ
	//unsigned int WindowWidth;   //ウィンドウ全体の横幅
	//unsigned int WindowHeight;  //ウィンドウ全体の高さ
	HWND          WindowHandle;   //ウィンドウハンドル
	HWND          WindowParent;   //親ウィンドウハンドル
	HDC           Screen;         //仮想画面(メモリデバイス)
	WNDCLASSEX    wcx;            //拡張ウィンドウクラス
	HMENU         hMenu;          //
	HINSTANCE     hInstance;      //
	LPVOID        lpParam;        //
	UINT          flags;
	HWND          winpos;
};

#define CLASSDEF_WINDOW class Window

//ウィンドウクラス
class Window
{
protected:
	HDROP drophandle;
	int( *ReceiveDragFiles_ )( int files );
	int dragfilesmax;
	char dragfilepath[ MAX_PATH ];
	int fullscreen;
public:
	char CLASSNAME[ WBUFLEN ];
	//構造体
	WINDOWDATA wd, backup;

	//コンストラクタ
	Window( void );
	//デストラクタ
	virtual ~Window( void );
	//
	virtual int InitWindowClassEx( HINSTANCE hInstance );
	virtual int RegistClassEx( void );
	//ウィンドウの生成
	virtual unsigned long MakeWindow( int nCmdShow );

	// ウィンドウ設定のバックアップ。
	virtual int BackUpWindow();
	// ウィンドウ設定の復帰。
	virtual int RepairWindow();
	//ウィンドウの設定変更
	virtual BOOL SetWindow( int type = WT_RENEWAL, int display = 0 );

	//ウィンドウクラスの名前の取得
	virtual const char * GetClassNameEx( void );
	//ウィンドウクラスの名前の設定(反映はSetWindow()関数)
	//virtual int SetClassNameEx( char * name );

	//ウィンドウの名前の取得
	virtual char * GetWindowName( void );
	//ウィンドウの名前の設定(反映はSetWindow()関数)
	virtual int SetWindowName( char * name );
	//ウィンドウのアイコン設定
	virtual int SetWindowIcon( int smalliconnum, int bigiconnum );
	virtual int SetWindowIcon( char *smalliconname, char *bigiconname );

	//ウィンドウスタイルの取得
	virtual unsigned long GetWindowStyle( void );
	//ウィンドウスタイルの設定(反映はSetWindow()関数)
	virtual unsigned long SetWindowStyle( enum WINDOWTYPE wt );
	//ウィンドウスタイルの直設定(反映はSetWindow()関数)
	virtual unsigned long SetWindowStyleDirect( unsigned long ws );

	//拡張ウィンドウスタイルの取得
	virtual unsigned long GetWindowStyleEx( void );
	//拡張ウィンドウスタイルの直設定(反映はSetWindow()関数)
	virtual unsigned long SetWindowStyleExDirect( unsigned long wse );

	//現在のウィンドウの横幅を取得
	virtual unsigned int GetWindowWidth( int type = 0 );
	//現在のウィンドウの高さを取得
	virtual unsigned int GetWindowHeight( int type = 0 );
	//ウィンドウの横幅設定(反映はSetWindow()関数)
	virtual unsigned int SetWindowWidth( unsigned int width );
	//ウィンドウの高さ設定(反映はSetWindow()関数)
	virtual unsigned int SetWindowHeight( unsigned int height );
	//ウィンドウのサイズ設定
	virtual int SetWindowSize( unsigned int width, unsigned int height );
	//ウィンドウの描画領域からウィンドウのサイズ設定
	virtual int SetScreenSize( unsigned int width, unsigned int height );

	//現在のウィンドウのX座標を取得
	virtual int GetPositionX( int type = 0 );
	//現在のウィンドウのY座標を取得
	virtual int GetPositionY( int type = 0 );
	//ウィンドウのX座標設定(反映はSetWindow()関数)
	virtual int SetPositionX( int x );
	//ウィンドウのY座標設定(反映はSetWindow()関数)
	virtual int SetPositionY( int y );
	//ウィンドウの座標設定
	virtual int SetPositionXY( int x, int y );

	//ディスプレイの数を返す
	static int GetDisplayMax( void );
	// そのウィンドウが存在しているディスプレイの番号を返す。
	// 複数ディスプレイにいる場合はウィンドウが一番大きく入っているディスプレイになる。
	static int GetDisplayNumber( HWND window = NULL );
	// ディスプレイのサイズを返す
	static int GetDisplaySize( long displaynum, int *width = NULL, int *height = NULL );
	// ディスプレイの位置とサイズを返す
	static int GetDisplayRect( long displaynum, int *x = NULL, int *y = NULL, int *width = NULL, int *height = NULL );
	static int GetDisplayRect( long displaynum, RECT *rect );
	static int GetDesktopWidth( void );
	static int GetDesktopHeight( void );
	static int GetDesktopSize( int *width, int *height );
	//ウィンドウハンドルを返す
	virtual HWND GetWindowHandle( void );
	//ウィンドウハンドルの設定
	virtual HWND SetWindowHandle( HWND wh );
	//親ウィンドウハンドルを返す
	virtual HWND GetParentWindowHandle( void );
	//親ウィンドウハンドルの設定
	virtual HWND SetParentWindowHandle( HWND wh );

	//ウィンドウクラスを返す
	virtual WNDCLASSEX GetWindowClassEx( void );
	//ウィンドウクラスの設定
	virtual WNDCLASSEX SetWindowClassEx( WNDCLASSEX wc );

	//
	virtual HMENU GetMenuHandle( void );
	//
	virtual HINSTANCE GetInstanceHandle( void );
	// ウィンドウプロシージャの設定
	virtual int SetWindowProc( WNDPROC winproc );
	//
	virtual LPVOID GetParameter( void );

	// !ドラッグ&ドロップされた時に受け取る関数を設定する。
	/*!
	  \param func int Function( int files )という関数の名前を与える。引数は受け取ったファイル数。
	  */
	virtual int SetReceiveDragFileFunction( int( *func )( int files ) );
	// !ドラッグ&ドロップされたファイル名を取得
	/*!
	  \param num ドラッグ&ドロップされたファイル番号
	  \retval 文字列のポインタ。早くコピーすること。
	  */
	virtual char * GetReceiveDragFilePath( int num );
	virtual int ReceiveDragFiles( HDROP hdrop );
};

#endif
