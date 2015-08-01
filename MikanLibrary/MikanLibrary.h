#ifndef MikanVersion
#define MikanVersion "2.1.5"
#define MikanFirstFPS 60.0
#define LOCK_HANDLE_MAX 16
#define LOCK_NAME_MAX 16

#ifndef MIKAN_NO_SOUND
//#define MIKAN_NO_SOUND
#endif

//windowsアプリケーションを作るために必要
//#include <mmsystem.h>

#ifndef _MIKANDEVELOPER
#ifdef _MIKANDEV
#include "Dev/Window.h"
#include "Dev/DirectXDraw.h"
#include "Dev/Color.h"
#include "MikanMovie.h"
#include "Dev/DirectXInput.h"
#ifndef MIKAN_NO_SOUND
#include "Dev/MikanSound.h"
#endif
#include "Dev/MikanSocket.h"
#include "Dev/MikanNetwork.h"
#include "Dev/FileIO.h"
#else
#include "Window.h"
#include "DirectXDraw.h"
#include "Color.h"
#include "MikanMovie.h"
#include "DirectXInput.h"
#ifndef MIKAN_NO_SOUND
#include "MikanSound.h"
#endif
#include "MikanSocket.h"
#include "MikanNetwork.h"
#include "FileIO.h"
#endif
#else
#include "../Window/Window.h"
#include "../DirectXDraw/DirectXDraw.h"
#include "../DirectXDraw/Color.h"
#include "../MikanMovie/MikanMovie.h"
#include "../DirectXInput/DirectXInput.h"
#ifndef MIKAN_NO_SOUND
#include "../MikanSound/MikanSound.h"
#endif
#include "../MikanSocket/MikanSocket.h"
#include "../MikanNetwork/MikanNetwork.h"
#include "../FileIO/FileIO.h"
#endif

//メイン
#define _CRTDBG_MAP_ALLOC
//#define _CRTDBG_MAPALLOC
#include <stdlib.h>
#include <crtdbg.h>
//crtdbg.h
#ifdef _DEBUG
#define new                new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s)          _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define calloc(c, s)       _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define realloc(p, s)      _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _recalloc(p, c, s) _recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _expand(p, s)      _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//Mikanライブラリの本体の型宣言
typedef class MikanLibrary   CLASSDEF_SYSTEM;

//無用なものを隠蔽したクラスの型宣言
typedef class MikanSystemPub          CLASSDEF_MIKAN_SYSTEM;
typedef class MikanWindowPub          CLASSDEF_MIKAN_WINDOW;
typedef class MikanDrawPub            CLASSDEF_MIKAN_DIRECTX_DRAW;
typedef class MikanMovieManagementPub CLASSDEF_MIKAN_MIKANMOVIE;
typedef class MikanInputPub           CLASSDEF_MIKAN_DIRECTX_INPUT;
#ifndef MIKAN_NO_SOUND
typedef class MikanSoundPub           CLASSDEF_MIKAN_DIRECTX_SOUND;
#endif
typedef class MikanNetworkPub         CLASSDEF_MIKAN_NETWORK;
typedef class FileIOManagementPub     CLASSDEF_MIKAN_FILE;

//静的ライブラリのリンク
//VCのみ
#ifndef _UNLINK_LIBRARY
#  ifdef _MIKANDEV
#    ifdef _DEBUG
#      pragma comment( lib, "Dev/MikanLibraryD.lib" )
#    else
#      pragma comment( lib, "Dev/MikanLibrary.lib" )
#    endif
#  else
#    ifdef _DEBUG
#      pragma comment( lib, "MikanLibraryD.lib" )
#    else
#      pragma comment( lib, "MikanLibrary.lib" )
#    endif
#  endif
#endif

//スレッド制御用データ
struct ThreadData
{
	unsigned int num;
	HANDLE handle;
	void * thread_func;//int (*thread_func)( void * vparam );
	void * param;
	unsigned long threadid;
	int type;
};

//待ち関数
enum
{
	WAIT_BUSY = 0,
	WAIT_SLEEP,
	WAIT_NONE
};

//現在の処理位置
enum
{
	MIKAN_POSITION_BEGIN = 0,   // 実行開始
	MIKAN_POSITION_SYSTEMINIT,  // SystemInit実行中
	MIKAN_POSITION_LIBLARYINIT, // SystemInitからUserInitまでの間
	MIKAN_POSITION_USERINIT,    // UserInit実行中
	MIKAN_POSITION_MAINLOOP,    // MainLoop実行中
	MIKAN_POSITION_CLEANUP,     // CleanUp実行中
	MIKAN_POSITION_RELEASE,     // CleanUp終了後
	MIKAN_POSITION_END,         // 実行終了
};

//ウィンドウ生成前にユーザーが実行する
extern void SystemInit( void );
//初期化終了後にユーザーが実行する
extern void UserInit( void );
//1秒に60回ユーザーが実行する
extern int MainLoop( void );
//MainLoop終了後に1度だけユーザーが実行する
extern void CleanUp( void );

//コールバック関数
LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//WinMain関数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow );
//スレッド実行関数
unsigned long WINAPI ThreadFunc( void * param );

//MikanLibraryクラス
class MikanLibrary
{
protected:
	int gameloop;
	int windowactive;
	int devicelost;
	double fps;
	double getfps;
	//ウィンドウ描画フラグ
	int view;
	// ウィンドウの状態
	int active;
	//インスタンス
	HINSTANCE hInstance;
	//フレーム
	unsigned int frames;
	unsigned int beforetime;
	unsigned int process;
	//現在の処理位置
	int position;
	//垂直同期の値
	int vsync;
	//スレッド制御用データ
	unsigned int thread_max;
	struct ThreadData *tdata;
	//待ち関数
	//  int (*Wait)( unsigned int now, unsigned int ideal, unsigned int prog );
	int( *Wait )( double fps, double *getfps );
	int( *AfterDeviceLost )( void );
	int drawable, waitsleep;
	HANDLE lockhandle[ LOCK_HANDLE_MAX ];
	char locknames[ LOCK_HANDLE_MAX ][ LOCK_NAME_MAX ];
	int permitscreensaver;

	struct DLLLIST
	{
		HINSTANCE *instance;
		struct DLLLIST *next;
	} dlllist;
public:
#ifdef _DEBUG
	unsigned int tm[ 20 ];
#endif
	MikanLibrary( void );
	virtual ~MikanLibrary( void );
	virtual int Init( HINSTANCE hInstance );
	virtual int SecondInit( void );
	//ゲームループを設定
	virtual int SetGameLoop( int loop = 0 );
	//インスタンスを取得する
	virtual HINSTANCE GetInstance( void );
	//FPSを設定する
	virtual double SetFPS( double fps = MikanFirstFPS );
	//FPSを取得する
	virtual double GetFPS( void );
	//次のフレームまで待つ
	virtual int WaitNextFrame( int draw = 1 );
	//待ち関数の設定
	virtual int SetWaitFunction( int WaitFunc( double fps, double *getfps ) );
	virtual int SetWaitFunction( int type = WAIT_SLEEP );

	//デバイスロスト後実行する関数の設定
	virtual int SetAfterDeviceLostFunction( int( *func )( void ) );

	//MikanライブラリのMain関数
	virtual int WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow );

	//デバイスロストの発生を調べる
	virtual int IsDeviceLost( void );

	//現在の処理位置を調べる
	virtual int GetMikanPosition( void );

	//垂直同期待ちの設定
	virtual int SetVsyncWait( int vs = 0 );

	// ウィンドウモードの変更。
	virtual int SetFullScreen( int fullscreen_on = 1 );
	//ウィンドウ非アクティブ時の動作設定
	virtual int SetInactiveWindow( int flag = 1 );

	//ウィンドウ非アクティブ時の動作を取得
	virtual int GetInactiveWindow( void );

	//ウィンドウの状態を設定
	virtual int SetWindowState( int flag = 1 );

	//ウィンドウがアクティブかどうか調べる
	virtual int IsActive( void );

	// スクリーンセーバーの稼働を許すかどうか()。
	virtual int SetPermitScreenSaver( int permit = 1 );
	virtual int GetPermitScreenSaver( void );
	//スレッドを立てて関数を実行する
	virtual int RunThread( unsigned int tnum, int( *thread_func )( void * vparam ), void *param = NULL );
	//スレッドを立てて関数を実行する
	virtual int RunThread( unsigned int tnum, int( *thread_func )( void ) );
	//スレッドで実行している中身
	virtual int RunThread_( struct ThreadData *td );
	//立てたスレッドを破棄する
	virtual int StopThread( unsigned int tnum );
	// スレッドのハンドルを取得する。
	virtual void * GetThreadHandle( unsigned int tnum );
	// 指定スレッドが終了するまで待機する。
	virtual int Join( unsigned int tnum, unsigned long sleep = 5 );
	//virtual LRESULT WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual int LockMainLoop( void );
	virtual int UnlockMainLoop( void );
	virtual int WaitClearScreen( int value = 1 );
	virtual int CreateLock( unsigned int lnum );
	virtual unsigned long Lock( unsigned int lnum, unsigned long timeout = INFINITE );
	virtual int Unlock( unsigned int lnum );
	virtual int ReleaseLock( unsigned int lnum );
	virtual int DrawBegin( void );
	virtual int DrawEnd( void );

	// DLL操作。

	// DLLを読み込む。
	//virtual HINSTANCE * LoadDLL( const char *dllfile );
	// DLLを開放する。
	//virtual int UnloadDLL( HINSTANCE *dllhandle );
	// 取り込んだDLLを全て開放する。
	//virtual int UnloadDLLAll( void );
};

//隠蔽したシステムクラス
class MikanSystemPub
{
public:
	MikanSystemPub( void );
	virtual ~MikanSystemPub( void );
	// !FPSを設定する。
	/*!
	  \param fps 設定するFPSの値(デフォルト60.0)。
	  */
	virtual double SetFPS( double fps = MikanFirstFPS );
	// !FPSを取得する。
	/*!
	  \retval 値 現在のFPS。
	  */
	virtual double GetFPS( void );
	// !次のフレームまで待つ。
	/*!
	  \param draw 描画処理を行なってから待つ(デフォルト)。0で待つだけ。
	  */
	virtual int WaitNextFrame( int draw = 1 );
	// !待ち関数の設定。
	/*!
	  \param type 用意された待ち関数を指定する(WAIT_BUSY[デフォルト], WAIT_SLEEP, WAIT_NONE)。
	  */
	virtual int SetWaitFunction( int type = WAIT_BUSY );
	// !デバイスロストの発生を調べる。
	/*!
	  \retval 0 デバイスロストは発生していない。
	  \retval その他 デバイスロスト発生中。
	  */
	virtual int IsDeviceLost( void );
	// !垂直同期待ちの設定。
	/*!
	  \param vs 0=垂直同期の無効化, それ以外=有効化
	  */
	virtual int SetVsyncWait( int vs = 0 );
	// ウィンドウモードの変更。
	/*!
	  \param flag 1=フルスクリーンモードにする(デフォルト), 0=ウィンドウモードにする。
	  */
	virtual int SetFullScreen( int fullscreen_on = 1 );  // !ウィンドウ非アクティブ時の動作設定。
	// !ウィンドウが非アクティブ時の動作を設定する。
	/*!
	  \param flag 1=非アクティブ時に実行を止める(デフォルト), 0=非アクティブ時も実行。
	  */
	virtual int SetInactiveWindow( int flag = 1 );
	// !スレッドを立てて関数を実行する。
	/*!
	  \param tnum スレッド番号。
	  \param thread_func 実行する関数。int 関数名( void *param )という関数の関数名。
	  \param param 実行する関数に与えられるポインタ。与えるポインタをvoid *でキャストして使う。
	  */
	virtual int RunThread( unsigned int tnum, int( *thread_func )( void * vparam ), void *param = NULL );
	// !スレッドを立てて関数を実行する。
	/*!
	  \param tnum スレッド番号。
	  \param thread_func 実行する関数。int 関数名( void )という関数の関数名。
	  */
	virtual int RunThread( unsigned int tnum, int( *thread_func )( void ) );
	// !立てたスレッドを破棄する。
	/*!
	  \param tnum 強制停止するスレッドの番号。
	  */
	virtual int StopThread( unsigned int tnum );
	// !スレッドのハンドルを取得する。
	/*!
	  \param tnum スレッドハンドルを取得するスレッド番号。
	  \retval ハンドル スレッドのハンドル。
	  */
	virtual void * GetThreadHandle( unsigned int tnum );
	// !指定スレッドが終了するまで待機する。
	/*!
	  \param tnum 終了するのを待つスレッド番号。
	  \param sleep 1ループ辺りの待ち時間(ミリ秒。デフォルト5)。
	  */
	virtual int Join( unsigned int tnum, unsigned long sleep = 5 );
	// !MainLoopをロックする。
	virtual int LockMainLoop( void );
	// !MainLoopをアンロックする。
	virtual int UnlockMainLoop( void );
	// !ClearScreenの実行を待つ。
	/*!
	  \param value value回以上ClearScreenが実行されるまで待つ(デフォルト1)。
	  */
	virtual int WaitClearScreen( int value = 1 );
	// !ロックの作成。
	/*!
	  \param lnum ロック番号。
	  */
	virtual int CreateLock( unsigned int lnum );
	// !処理のロック。
	/*!
	  \param lnum ロック番号。
	  \param timeout タイムアウト時間(デフォルトは無制限に待つ)。
	  */
	virtual unsigned long Lock( unsigned int lnum, unsigned long timeout = INFINITE );
	// !処理のアンロック。
	/*!
	  \param lnum ロック番号。
	  */
	virtual int Unlock( unsigned int lnum );
};

//隠蔽したWindowクラス
class MikanWindowPub
{
public:
	MikanWindowPub();
	virtual ~MikanWindowPub();
	//ウィンドウの設定変更
	//  virtual BOOL SetWindow( int type = WT_RENEWAL );

	// !ウィンドウの名前の取得。
	/*!
	  \retval ウィンドウ名を格納したchar配列のポインタ。
	  */
	virtual char * GetWindowName( void );
	// !ウィンドウの名前の設定。
	/*!
	  \param name 設定するウィンドウ名。
	  */
	virtual int SetWindowName( char * name );
	// !ウィンドウのアイコン設定。
	/*!
	  \param iconnum アイコンリソースに付けられた番号(リソースタイプICON)。
	  */
	virtual int SetWindowIcon( int smalliconnum, int bigiconnum );
	// !ウィンドウのアイコン設定。
	/*!
	  \param iconnum アイコンリソースに付けられた番号(リソースタイプICON)。
	  */
	virtual int SetWindowIcon( int iconnum );
	// !ウィンドウのアイコン設定。
	/*!
	  \param iconnum アイコンリソースに付けられた名前(リソースタイプICON)。
	  */
	virtual int SetWindowIcon( char *smalliconname, char *bigiconname );
	// !ウィンドウのアイコン設定。
	/*!
	  \param iconnum アイコンリソースに付けられた番号(リソースタイプICON)。
	  */
	virtual int SetWindowIcon( char *iconname );

	// !現在のウィンドウの横幅を取得。
	/*!
	  \param type 0=設定値を返す(デフォルト), 1=現在のサイズを調べて返す。
	  \retval 値 ウィンドウの横幅。
	  */
	virtual unsigned int GetWindowWidth( int type = 0 );
	// !現在のウィンドウの高さを取得。
	/*!
	  \param type 0=設定値を返す(デフォルト), 1=現在のサイズを調べて返す。
	  \retval 値 ウィンドウの高さ。
	  */
	virtual unsigned int GetWindowHeight( int type = 0 );
	// !ウィンドウのサイズ設定。MikanWindow経由だとクライアント領域から計算される。
	/*!
	  \param width 設定するウィンドウの横幅。
	  \param height 設定するウィンドウの高さ。
	  \retval 0 成功
	  */
	virtual int SetWindowSize( unsigned int width, unsigned int height );

	// !現在のウィンドウのX座標を取得。
	/*!
	  \param type 0=設定値を返す(デフォルト), 1=現在のサイズを調べて返す。
	  \retval 値 ウィンドウのX座標。
	  */
	virtual int GetPositionX( int type = 0 );
	// !現在のウィンドウのY座標を取得。
	/*!
	  \param type 0=設定値を返す(デフォルト), 1=現在のサイズを調べて返す。
	  \retval 値 ウィンドウのY座標。
	  */
	virtual int GetPositionY( int type = 0 );
	// !ウィンドウの座標設定。
	/*!
	  \param x 設定するウィンドウのX座標。
	  \param y 設定するウィンドウのY座標。
	  */
	virtual int SetPositionXY( int x, int y );

	// !フルスクリーンにする(SystemInitでの実行を推奨)。
	virtual int SetFullScreen( void );

	/*  //ディスプレイの数を返す
	virtual int Window::GetMonitorNumber(void);*/

	// デスクトップの横幅を取得する。
	/*!
	  \retval デスクトップの横幅。
	  */
	virtual int GetDesktopWidth( void );
	// デスクトップの高さを取得する。
	/*!
	  \retval デスクトップの高さ。
	  */
	virtual int GetDesktopHeight( void );
	// デスクトップの大きさを取得する。
	/*!
	  \param width 横幅を格納するint型変数へのポインタ。
	  \param height 高さを格納するint型変数へのポインタ。
	  */
	virtual int GetDesktopSize( int *width, int *height );

	//ウィンドウハンドルを返す
	/*!
	  \retval ウィンドウハンドル 現在のウィンドウハンドル。
	  */
	virtual HWND GetWindowHandle( void );

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
};

//隠蔽したDrawクラス
class MikanDrawPub
{
	friend class MikanMovieManagementPub;
protected:
	static int base_x, base_y;
public:

	MikanDrawPub();
	virtual ~MikanDrawPub();
	// !Mikanライブラリが初めから保持している色(原色大時点http://www.colordic.org/より)。
	struct WEBCOLOR ColorList;

	// !テクスチャの最大数の設定。
	/*!
	  \param tmax 使用するテクスチャの最大数(デフォルトDX_TEXTURE_MAX)。
	  */
	virtual unsigned int SetTextureMax( unsigned int tmax = DX_TEXTURE_MAX );

	// !描画サイズの設定
	/*!
	  \param color 画面を塗り潰す色(デフォルト0xFF000000[黒])。
	  */
	virtual int SetScreenSize( int width, int height );

	// !描画横幅を取得する。
	/*
	\param now 0で設定値を取得(デフォルト)。1で現在の有効値を取得。
	*/
	virtual int GetScreenWidth( int now = 0 );
	// !描画高さを取得する。
	/*
	\param now 0で設定値を取得(デフォルト)。1で現在の有効値を取得。
	*/
	virtual int GetScreenHeight( int now = 0 );

	// !画面を綺麗にする。
	/*!
	  \param color 画面を塗り潰す色(デフォルト0xFF000000[黒])。
	  */
	virtual int ClearScreen( unsigned long color = 0xFF000000 );

	// !描画基準x座標をセット。
	/*!
	  \param x 描画基準のX座標。
	  */
	virtual int SetBaseX( int x = 0 );
	// !描画基準y座標をセット。
	/*!
	  \param y 描画基準のY座標。
	  */
	virtual int SetBaseY( int y = 0 );

	// !テクスチャの合成方法の指定。
	/*!
	  \param kind 描画方法(DRAW_TRC=透過色有効[デフォルト], DRAW_ADD=加算合成, DRAW_SUB=減算合成, DRAW_MUL=乗算合成, DRAW_NON=透過色無効)。
	  */
	virtual int SetRender( int kind = DRAW_TRC );
	// !画面をレンダーターゲットにする。
	virtual int SetRenderTarget( void );
	// !テクスチャをレンダーターゲットにする。
	/*!
	  \param txnum 描画対象にするテクスチャ番号。
	  \retval 0 成功
	  */
	virtual int SetRenderTarget( unsigned int txnum );
	// !リソースを読み込んでテクスチャを作る。
	/*!
	  \param txnum テクスチャ番号。
	  \param module リソースを持つモジュールのハンドル。exeファイルの場合はNULLを指定する。
	  \param resource リソース名(リソース名 IMAGE "ファイルパス")。
	  */
	virtual int CreateTexture( unsigned int txnum, HMODULE module, const char *resource, unsigned long tcolor = 0x00000000, int thread = 0 );
	// !空のテクスチャを作る。
	/*!
	  \param txnum テクスチャ番号。
	  \param width テクスチャの横幅。
	  \param height テクスチャの高さ。
	  */
	virtual int CreateTexture( unsigned int txnum, unsigned int width, unsigned int height, int thread = 0 );
	// !画像を読み込んでテクスチャを作る。
	/*!
	  \param txnum テクスチャ番号。
	  \param filename ファイルパス。
	  \param tcolor 透過色の指定(TRC_NONE=透過色を指定しない[デフォルト], TRC_ZERO=左上の色を透過色にする, その他色=透過色にする色)。
	  */
	virtual int CreateTexture( unsigned int txnum, const char *filename, unsigned long tcolor = 0x00000000, int thread = 0 );
	// !テクスチャの解放。
	/*!
	  \param txnum 開放するテクスチャの番号。
	  */
	virtual int ReleaseTexture( unsigned int txnum );
	// !点を描画する。
	/*!
	  \param x 描画X座標。
	  \param y 描画Y座標。
	  \param color 線の色(デフォルト0xFFFFFFFF[白])
	  */
	virtual int DrawPixel( int x, int y, unsigned long color = 0xffffffff );
	// !直線を描画する。
	/*!
	  \param sx 描画開始X座標。
	  \param sy 描画開始Y座標。
	  \param ex 描画終了X座標。
	  \param ey 描画終了Y座標。
	  \param color 線の色(デフォルト0xFFFFFFFF[白])
	  \param width 線の太さ(デフォルト1.0)。
	  \param antialias アンチエイリアスの指定(0=無効[デフォルト], 1有効)。
	  */
	virtual int DrawLine( int sx, int sy, int ex, int ey, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );
	// !四角形の線を描画する。
	/*!
	  \param dx 描画開始X座標。
	  \param dy 描画開始Y座標。
	  \param w 箱の横幅。
	  \param h 箱の高さ。
	  \param color 箱の色。
	  \param width 線の太さ。
	  \param antialias アンチエイリアスの指定(0=無効[デフォルト], 1有効)。
	  */
	virtual int DrawBoxLine( int dx, int dy, int w, int h, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );
	// !四角形の線を描画する。
	/*!
	  \param dx 描画中央X座標。
	  \param dy 描画中央Y座標。
	  \param w 箱の横幅。
	  \param h 箱の高さ。
	  \param color 箱の色。
	  \param width 線の太さ。
	  \param antialias アンチエイリアスの指定(0=無効[デフォルト], 1有効)。
	  */
	virtual int DrawBoxLineC( int dx, int dy, int w, int h, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );

	// !四角形を描画する。
	/*!
	  \param dx 描画開始X座標。
	  \param dy 描画開始Y座標。
	  \param w 箱の横幅。
	  \param h 箱の高さ。
	  \param color 箱の色。
	  */
	virtual int DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color );
	// !四角形を描画する。
	/*!
	  \param x0 左上のX座標。
	  \param y0 左上のY座標。
	  \param x1 右上のX座標。
	  \param y1 右上のY座標。
	  \param x2 左下のX座標。
	  \param y2 左下のY座標。
	  \param x3 右上のX座標。
	  \param y3 右上のY座標。
	  \param color0 左上の色。
	  \param color1 右上の色。
	  \param color2 左下の色。
	  \param color3 右上の色。
	  */
	virtual int DrawBox( int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 );
	// !四角形を描画する。
	/*!
	  \param x0 左上のX座標。
	  \param y0 左上のY座標。
	  \param x1 右上のX座標。
	  \param y1 右上のY座標。
	  \param x2 左下のX座標。
	  \param y2 左下のY座標。
	  \param x3 右上のX座標。
	  \param y3 右上のY座標。
	  \param color 箱の色。
	  */
	virtual int DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 );
	// !四角形を描画する。
	/*!
	  \param dx 描画開始X座標。
	  \param dy 描画開始Y座標。
	  \param w 箱の横幅。
	  \param h 箱の高さ。
	  \param color 箱の色。
	  */
	virtual int DrawBox( int dx, int dy, int w, int h, unsigned long color );
	// !四角形を中心を基準に描画する。
	/*!
	  \param dx 描画中央X座標。
	  \param dy 描画中央Y座標。
	  \param w 箱の横幅。
	  \param h 箱の高さ。
	  \param color0 左上の色。
	  \param color1 右上の色。
	  \param color2 左下の色。
	  \param color3 右上の色。
	  */
	virtual int DrawBoxC( int dx, int dy, int w, int h, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 );
	// !四角形を中心を基準に描画する。
	/*!
	  \param dx 描画中央X座標。
	  \param dy 描画中央Y座標。
	  \param w 箱の横幅。
	  \param h 箱の高さ。
	  \param color 箱の色。
	  */
	virtual int DrawBoxC( int dx, int dy, int w, int h, unsigned long color );

	// !円描画(円の線のみ)。
	/*!
	  \param x 円の中心X座標。
	  \param y 円の中心Y座標。
	  \param r 円の半径。
	  \param color 円の色(デフォルト0xFFFFFFFF[白])。
	  */
	virtual int DrawCircleLineC( int x, int y, int r, unsigned long color = 0xFFFFFFFF, double width = 1.0, int antialias = 0 );
	// !円描画(箱内に収まる楕円の線)。
	/*!
	  \param x 円を収める箱の描画開始X座標。
	  \param y 円を収める箱の描画開始Y座標。
	  \param w 円を収める箱の横幅。
	  \param h 円を収める箱の高さ。
	  \param color 円の色(デフォルト0xFFFFFFFF[白])。
	  */
	virtual int DrawCircleLine( int x, int y, int w, int h, unsigned long color = 0xFFFFFFFF, double width = 1.0, int antialias = 0 );

	// !円描画(円のみ)。
	/*!
	  \param x 円の中心X座標。
	  \param y 円の中心Y座標。
	  \param r 円の半径。
	  \param color 円の色(デフォルト0xFFFFFFFF[白])。
	  */
	virtual int DrawCircleC( int x, int y, int r, unsigned long color = 0xFFFFFFFF );
	// !円描画(箱内に収まる楕円)。
	/*!
	  \param x 円を収める箱の描画開始X座標。
	  \param y 円を収める箱の描画開始Y座標。
	  \param w 円を収める箱の横幅。
	  \param h 円を収める箱の高さ。
	  \param color 円の色(デフォルト0xFFFFFFFF[白])。
	  */
	virtual int DrawCircle( int x, int y, int w, int h, unsigned long color = 0xFFFFFFFF );

	// !テクスチャを描画する。
	/*!
	  \param txnum テクスチャ番号。
	  \param dx 描画開始X座標。
	  \param dy 描画開始Y座標。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTexture( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	// !テクスチャを描画する。
	/*!
	  \param txnum テクスチャ番号。
	  \param dx 描画中央X座標。
	  \param dy 描画中央Y座標。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTextureC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	// !テクスチャを拡大縮小描画する。
	/*!
	  \param txnum テクスチャ番号。
	  \param dx 描画開始X座標。
	  \param dy 描画開始Y座標。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param dw 描画横幅。
	  \param dh 描画高さ。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	// !テクスチャを拡大縮小描画する。
	/*!
	  \param txnum テクスチャ番号。
	  \param dx 描画開始X座標。
	  \param dy 描画開始Y座標。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param scale 拡大率。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// !テクスチャを拡大縮小描画する。
	/*!
	  \param txnum テクスチャ番号。
	  \param dx 描画中央X座標。
	  \param dy 描画中央Y座標。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param dw 描画横幅。
	  \param dh 描画高さ。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	// !テクスチャを拡大縮小描画する。
	/*!
	  \param txnum テクスチャ番号。
	  \param dx 描画中央X座標。
	  \param dy 描画中央Y座標。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param scale 拡大率。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// !テクスチャを回転描画する(ラジアン角)。
	/*!
	  \param txnum テクスチャ番号。
	  \param dx 描画中央X座標。
	  \param dy 描画中央Y座標。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param rad 回転角度(ラジアン角度)。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTextureRotationC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 );
	// !テクスチャを回転描画する(ラジアン角ではない)。
	/*!
	  \param txnum テクスチャ番号。
	  \param dx 描画中央X座標。
	  \param dy 描画中央Y座標。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param angle 回転角度(度。ラジアン角度ではないので直角は90.0)。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTextureRotationAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 );
	// !テクスチャを回転拡大描画する(ラジアン角)。
	/*!
	  \param txnum テクスチャ番号。
	  \param dx 描画中央X座標。
	  \param dy 描画中央Y座標。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param sw 描画横幅。
	  \param sh 描画高さ。
	  \param rad 回転角度(ラジアン角度)。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag = 0 );
	// !テクスチャを回転拡大描画する(ラジアン角ではない)。
	/*!
	  \param txnum テクスチャ番号。
	  \param dx 描画中央X座標。
	  \param dy 描画中央Y座標。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param sw 描画横幅。
	  \param sh 描画高さ。
	  \param angle 回転角度(度。ラジアン角度ではないので直角は90.0)。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag = 0 );
	// !テクスチャを回転拡大描画する。
	/*!
	  \param txnum テクスチャ番号。
	  \param dx 描画中央X座標。
	  \param dy 描画中央Y座標。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param scale 拡大率。
	  \param rad 回転角度(ラジアン角度)。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 );
	// !テクスチャを回転拡大描画する。
	/*!
	  \param txnum テクスチャ番号。
	  \param dx 描画中央X座標。
	  \param dy 描画中央Y座標。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param scale 拡大率。
	  \param angle 回転角度(度。ラジアン角度ではないので直角は90.0)。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 );
	// !テクスチャの4点を自由に決めて描画する。
	/*!
	  \param txnum テクスチャ番号。
	  \param rx 読み込み開始X座標。
	  \param ry 読み込み開始Y座標。
	  \param w 切り取る横幅。
	  \param h 切り取る高さ。
	  \param x0 テクスチャの左上の描画先X座標。
	  \param y0 テクスチャの左上の描画先Y座標。
	  \param x1 テクスチャの右上の描画先X座標。
	  \param y1 テクスチャの右上の描画先Y座標。
	  \param x2 テクスチャの左下の描画先X座標。
	  \param y2 テクスチャの左下の描画先Y座標。
	  \param x3 テクスチャの右下の描画先X座標。
	  \param y3 テクスチャの右下の描画先Y座標。
	  \param drawflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	  */
	virtual int DrawTextureVertex( unsigned int txnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 );

	// !ポリゴンの作成。
	/*!
	  \param pnum ポリゴン番号。
	  \param max  ポリゴンの頂点数。
	  \param filltype ポリゴンの塗り方(D3DPT_TRIANGLESTRIP=連なった三角形とする(デフォルト),D3DPT_TRIANGLEFAN=最初の頂点とその他の頂点で三角形とする,などなど)。
	  */
	virtual int CreatePolygon( unsigned int pnum, unsigned int max, D3DPRIMITIVETYPE filltype = D3DPT_TRIANGLESTRIP );

	// !正多角形の作成(D3DPT_TRIANGLEFANで作成したポリゴン)。
	/*!
	  \param pnum ポリゴン番号。
	  \param cx 中央X座標。
	  \param cy 中央Y座標。
	  \param n 正多角形の角数。
	  \param r 半径。
	  \param rad 回転角度。
	  \param color 色。
	  */
	virtual int CreateRegularPolygon( unsigned int pnum, double cx, double cy, unsigned int n, double r, double rad = 0.0, unsigned long color = 0xffffffff );

	// !テクスチャの登録
	/*!
	  \param pnum ポリゴン番号。
	  \param tnum ポリゴンに設定するテクスチャ番号。
	  */
	virtual int SetTextureOnPolygon( unsigned int pnum, unsigned int tnum );

	// !テクスチャの登録解除
	/*!
	  \param pnum ポリゴン番号。
	  */
	virtual int SetTextureOnPolygon( unsigned int pnum );

	// !UV座標の登録。
	/*!
	  \param pnum ポリゴン番号。
	  \param vnum 頂点番号。
	  \param u UV展開のX座標。
	  \param v UV展開のY座標。
	  */
	virtual int SetPolygonUV( unsigned int pnum, unsigned int vnum, int u, int v );

	// !頂点の登録
	/*!
	  \param pnum ポリゴン番号。
	  \param vnum 頂点番号。
	  \param x 描画X座標。
	  \param y 描画Y座標。
	  \param color 頂点色。
	  */
	virtual int SetPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color = 0xFFFFFFFF );

	// !頂点情報の編集
	/*!
	  \param pnum ポリゴン番号。
	  \param vnum 頂点番号。
	  \param x 描画X座標。
	  \param y 描画Y座標。
	  \param color 頂点色。
	  \param u UV展開のX座標。
	  \param v UV展開のY座標。
	  \param edit どの編集を有効化するか(PVEDIT_X=X座標,PVEDIT_Y=Y座標,PVEDIT_XY=座標,PVEDIT_DEFAULT=座標(デフォルト),PVEDIT_COLOR=色,PVEDIT_U=U座標,PVEDIT_V=V座標,PVEDIT_UV=UV座標,PVEDIT_ALL=全て)。
	  */
	virtual int EditPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color = 0xFFFFFFFF, int u = 0, int v = 0, int edit = PVEDIT_DEFAULT );
	// !ポリゴンの移動
	/*!
	  \param pnum ポリゴン番号。
	  \param mx 移動X座標。
	  \param my 移動Y座標。
	  */
	virtual int TransformPolygonMoving( unsigned int pnum, double mx, double my );
	// !ポリゴンの拡大
	/*!
	  \param pnum ポリゴン番号。
	  \param scale 拡大率。
	  \param basenum 基準とする頂点番号(デフォルト=0)。
	  */
	virtual int TransformPolygonScaling( unsigned int pnum, double scale, unsigned int basenum = 0 );
	// !ポリゴンの描画
	/*!
	  \param pnum ポリゴン番号。
	  \param cx 描画X座標。
	  \param cy 描画Y座標。
	  \param begin 描画開始頂点番号。
	  \param end 描画終了頂点番号。
	  */
	virtual int DrawPolygon( unsigned int pnum, int cx, int cy, unsigned int begin, unsigned int end );
	// !ポリゴンの描画
	/*!
	  \param pnum ポリゴン番号。
	  \param cx 描画X座標。
	  \param cy 描画Y座標。
	  */
	virtual int DrawPolygon( unsigned int pnum, int cx, int cy );
	// !ポリゴンが保持する位置に多角形の描画
	/*!
	  \param pnum ポリゴン番号。
	  */
	virtual int DrawPolygon( unsigned int pnum );
	// !ポリゴンの拡大描画
	/*!
	  \param pnum ポリゴン番号。
	  \param cx 描画X座標。
	  \param cy 描画Y座標。
	  \param scale 拡大率
	  \param start 描画開始頂点番号。
	  \param end 描画終了頂点番号。
	  */
	virtual int DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale, unsigned int start, unsigned int end );
	// !ポリゴンの拡大描画
	/*!
	  \param pnum ポリゴン番号。
	  \param cx 描画X座標。
	  \param cy 描画Y座標。
	  \param scale 拡大率
	  */
	virtual int DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale );
	// !正多角形の描画
	/*!
	  \param cx 描画座標。
	  \param cy 描画座標。
	  \param n 正N角形のN。
	  \param r 半径。
	  \param rad 回転角度。
	  \param color 頂点色。
	  */
	virtual int DrawRegularPolygon( double cx, double cy, unsigned int n, double r, double rad = 0.0, unsigned long color = 0xffffffff );

	// !色の作成。
	/*!
	  \param alpha アルファ値(不透明度。0～255。255で不透明)。
	  \param red 赤(0～255)。
	  \param green 緑(0～255)。
	  \param blue 青(0～255)。
	  */
	virtual unsigned long Color( unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue );
	// !カラーの設定。
	/*!
	  \param txnum 色を変更するテクスチャ番号。
	  \param color テクスチャに設定する色(白で通常)。
	  */
	virtual int SetColor( unsigned int txnum, unsigned long color = 0xFFFFFFFF );
	// アルファ値の設定。
	/*!
	  \param txnum アルファ値を変更するテクスチャ番号。
	  \param alpha アルファ値(不透明度。0～255。255で不透明)。アルファ値以外の色は保持される。
	  */
	virtual int SetAlpha( unsigned int txnum, unsigned char alpha );
	// !頂点カラーの設定
	/*!
	  \param txnum 頂点の色を変更するテクスチャ番号。
	  \param color 左上に設定する色。過去の色は1つづつずれて右下の色が追い出される。
	  */
	virtual int SetColorVertex( unsigned int txnum, unsigned long color );

	// !テクスチャのデータを読めるようにする(色の検索や置換処理が複数ある場合に軽量化可能)。
	/*!
	  \param 読み書きロックを行うテクスチャ番号。
	  \param x 読み書き開始X座標(デフォルト0)。
	  \param y 読み書き開始Y座標(デフォルト0)。
	  \param w 横幅(デフォルト0[最大値まで])。
	  \param h 高さ(デフォルト0[最大値まで])。
	  */
	virtual int OpenTexture( unsigned int txnum, int x = 0, int y = 0, int w = 0, int h = 0 );
	// !テクスチャのデータを読める状態から戻す。
	/*!
	  \param txnum 読み書きアンロックを行うテクスチャ番号。
	  */
	virtual int CloseTexture( unsigned int txnum );
	// !テクスチャの指定座標の色を取得する。
	/*!
	  \param txnum 色の検索を行うテクスチャ番号。
	  \param x 読み込む点のX座標。
	  \param y 読み込む点のY座標。
	  */
	virtual unsigned long GetColorTexturePixel( unsigned int txnum, int x, int y );
	// !テクスチャの範囲内の色を置換する。
	/*!
	  \param txnum 色を置換するテクスチャ番号。
	  \param scolor 検索色。
	  \param rcolor 置換色。
	  \param x 読み込み開始X座標(デフォルト0)。
	  \param y 読み込み開始Y座標(デフォルト0)。
	  \param w 横幅(デフォルト0[最大値まで])。
	  \param h 高さ(デフォルト0[最大値まで])。
	  */
	virtual unsigned int TextureColorReplacement( unsigned int txnum, unsigned long scolor, unsigned long rcolor, int x = 0, int y = 0, int w = 0, int h = 0 );
	// !テクスチャの範囲内の色を検索する。
	/*!
	  \param txnum 色を検索するテクスチャ番号。
	  \param scolor 検索色。
	  \param x 読み込み開始X座標(デフォルト0)。
	  \param y 読み込み開始Y座標(デフォルト0)。
	  \param w 横幅(デフォルト0[最大値まで])。
	  \param h 高さ(デフォルト0[最大値まで])。
	  \retval 0 見つからなかった。
	  \retval それ以外 見つかるまでに調べたピクセル数(左上から1)。
	  */
	virtual unsigned int TextureColorSearch( unsigned int txnum, unsigned long scolor, int x = 0, int y = 0, int w = 0, int h = 0 );
	// !テクスチャの範囲内の色を検索し数を数える。
	/*!
	  \param txnum 色を検索するテクスチャ番号。
	  \param scolor 検索色。
	  \param x 読み込み開始X座標(デフォルト0)。
	  \param y 読み込み開始Y座標(デフォルト0)。
	  \param w 横幅(デフォルト0[最大値まで])。
	  \param h 高さ(デフォルト0[最大値まで])。
	  \retval 値 見つかった色の数。
	  */

	virtual unsigned int TextureColorCount( unsigned int txnum, unsigned long scolor, int x = 0, int y = 0, int w = 0, int h = 0 );
	// !テクスチャの横幅取得。
	/*!
	  \param txnum 横幅を取得するテクスチャ番号。
	  */
	virtual unsigned int GetTextureWidth( unsigned int txnum = 0 );
	// !テクスチャの高さ取得。
	/*!
	  \param txnum 高さを取得するテクスチャ番号。
	  */
	virtual unsigned int GetTextureHeight( unsigned int txnum = 0 );
	// !スクリーンショット。
	/*!
	  \param filename 出力ファイル名。
	  \param imagetype 出力画像形式(IMAGE_BMP=Bitmap[デフォルト], IMAGE_JPG=JPEG, IMAGE_PNG=PNG, ... etc)
	  */
	virtual int ScreenShot( const char *filename, unsigned int imagetype = IMAGE_BMP );

	//フォント
	// !フォントを読み込む(ファイルから。開放作業必要)。
	/*!
	  \param fontfile フォントファイルのパス。
	  */
	virtual int LoadFontFile( const char *fontfile );
	// !フォントを読み込む(リソースから。開放作業不要)。
	/*!
	  \param module リソースを持つモジュールのハンドル。exeファイルの場合はNULLを指定する。
	  \param resource リソース名(リソース名 FONT_FILE "ファイルパス")。
	  */
	virtual int LoadFontFile( HMODULE module, const char *resource );
	// !フォントを作る。
	/*!
	  \param fontnum 作成するフォント番号。
	  \param fontname フォント名(Windowsに登録されているものかLoadFontFileで読み込んだもの)。
	  \param weight 太さ(他の場合0を指定している)。
	  \param italic イタリック体の設定(1=イタリック体にする,0=イタリック体にしない[デフォルト])
	  \param size フォントサイズ(デフォルト10)。
	  \param color フォントの色(デフォルト0xFFFFFFFF[白])。
	  */
	virtual int CreateFont( unsigned int fontnum, const char *fontname, int size, unsigned long color, int weight, int italic = 0 );
	// !フォントを作る。
	/*!
	  \param fontnum 作成するフォント番号。
	  \param fontname フォント名(Windowsに登録されているものかLoadFontFileで読み込んだもの)。
	  \param size フォントサイズ(デフォルト10)。
	  \param color フォントの色(デフォルト0xFFFFFFFF[白])。
	  */
	virtual int CreateFont( unsigned int fontnum, const char *fontname, int size = 10, unsigned long color = 0xFFFFFFFF );
	// !フォントを作る。
	/*!
	  \param fontnum 作成するフォント番号。
	  \param size フォントサイズ(デフォルト10)。
	  \param color フォントの色(デフォルト0xFFFFFFFF[白])。
	  */
	virtual int CreateFont( unsigned int fontnum, int size = 10, unsigned long color = 0xFFFFFFFF );
	//フォントを再設定する

	// !文字列の描画。
	/*!
	  \param fontnum フォント番号(未生成の場合0が使われる)。
	  \param x 描画開始X座標。
	  \param y 描画開始Y座標。
	  \param color 文字の色。
	  \param format 変換指定子を含む文字列(C言語のprintfと同じ)。
	  \param それ以降 変換指定子が使う値。
	  */
	virtual int Printf( unsigned int fontnum, int x, int y, unsigned long color, const char *format, ... );
	// !文字列の描画。
	/*!
	  \param fontnum フォント番号(未生成の場合0が使われる)。
	  \param x 描画開始X座標。
	  \param y 描画開始Y座標。
	  \param format 変換指定子を含む文字列(C言語のprintfと同じ)。
	  \param それ以降 変換指定子が使う値。
	  */
	virtual int Printf( unsigned int fontnum, int x, int y, const char *format, ... );
};

class MikanMovieManagementPub
{
public:
	// ! テクスチャを取得する(Lockしないと何が起こるか不明。2枚のテクスチャでダブルバッファリングしてる可能性が高い)。
	virtual struct IDirect3DTexture9 * GetTexture( unsigned int movnum );
	// ! テクスチャ処理をロックする。
	virtual int Lock( unsigned int movnum );
	// テクスチャ処理をアンロックする。
	virtual int Unlock( unsigned int movnum );

	// ! 動画を開く。
	/* !
	\param movnum 動画番号。
	\param moviefile 動画ファイル。
	*/
	virtual int Load( unsigned int movnum, char *moviefile );
	// ! 動画の再生を開始する。
	/* !
	\param movnum 動画番号。
	\param loopplay trueでループ再生。省略もしくはfalseで1度だけ再生。
	*/
	virtual int Play( unsigned int movnum, int loopplay = false );
	// ! 動画の再生を止める。
	/* !
	\param movnum 動画番号。
	*/
	virtual int Stop( unsigned int movnum );
	// ! 動画を中断する。
	/* !
	\param movnum 動画番号。
	*/
	virtual int Pause( unsigned int movnum );
	// ! 動画を再生しているか調べる。
	/* !
	\param movnum 動画番号。
	*/
	virtual int IsPlay( unsigned int movnum );
	// ! 動画の最後の場所を取得する(ミリ秒)。
	/* !
	\param movnum 動画番号。
	*/
	virtual long Size( unsigned int movnum );
	// ! 動画の現在の再生場所を取得する(ミリ秒)。
	/* !
	\param movnum 動画番号。
	*/
	virtual long Tell( unsigned int movnum );
	// ! 動画の再生位置をシークする(ミリ秒)。
	/* !
	\param movnum 動画番号。
	\param seek 再生位置。ミリ秒。
	*/
	virtual int Seek( unsigned int movnum, long seek );
	// ! 動画の再生速度の取得。通常は1.0。
	virtual double GetRate( unsigned int movnum );
	// ! 動画の再生速度の設定。通常は1.0。
	virtual int SetRate( unsigned int movnum, double rate );

	// ! 動画の横幅を取得する。
	virtual int GetWidth( unsigned int movnum );
	// ! 動画の高さを取得する。
	virtual int GetHeight( unsigned int movnum );

	// ! 動画の色合いを元に戻す。
	/* !
	\param movnum 動画番号。
	*/
	virtual int SetColor( unsigned int movnum );
	// ! 動画全体の色合いを決める。
	/* !
	\param movnum 動画番号。
	\param color 色。
	*/
	virtual int SetColor( unsigned int movnum, unsigned long color );
	// ! 動画の各頂点に色を設定する。
	/* !
	\param movnum 動画番号。
	\param color0 左上の色。
	\param color1 右上の色。
	\param color2 左下の色。
	\param color3 右下の色。
	*/
	virtual int SetColor( unsigned int movnum, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 );
	// ! 動画のアルファ値のみ変更する。
	/* !
	\param movnum 動画番号。
	\param alpha アルファ値。
	*/
	virtual int SetAlpha( unsigned int movnum, unsigned char alpha );

	// ! 動画を元サイズで指定位置に描画する。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovie( unsigned int movnum, int dx = 0, int dy = 0, int drawflag = 0 );
	// ! 動画を切り取って描画する。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovie( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	// ! 動画を切り取って中央描画する。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovieC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );

	// ! 動画を切り取って拡大縮小描画する。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param scale 拡大率。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// ! 動画を切り取って拡大縮小描画する。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param dw 描画横幅。
	\param dh 描画高さ。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	// ! 動画を切り取って中央に拡大縮小描画する。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param scale 拡大率。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// ! 動画を切り取って中央に拡大縮小描画する。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param dw 描画横幅。
	\param dh 描画高さ。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );

	// ! 動画を切り取って中央を基準に回転する(ラジアン角)。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param rad 回転角(ラジアン角)。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovieRotationC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 );
	// ! 動画を切り取って中央を基準に回転する(角度)。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param angle 回転角(角度)。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovieRotationAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 );

	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag = 0 ) = 0;
	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag = 0 ) = 0;

	// ! 動画を切り取って中央に回転拡大縮小描画する(ラジアン角)。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param scale 拡大率。
	\param rad 回転角(ラジアン角)。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 );
	// ! 動画を切り取って中央に回転拡大縮小描画する(角度)。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param scale 拡大率。
	\param angle 回転角(角度)。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 );
	// ! 動画を切り取って中央に回転拡大縮小描画する(ラジアン角)。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param dw 描画横幅。
	\param dh 描画高さ。
	\param rad 回転角(ラジアン角)。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, double rad, int drawflag = 0 );
	// ! 動画を切り取って中央に回転拡大縮小描画する(角度)。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param dw 描画横幅。
	\param dh 描画高さ。
	\param angle 角度。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, double angle, int drawflag = 0 );

	// ! 動画を切り取って4点指定して描画する。
	/* !
	\param movnum 動画番号。
	\param dx 描画開始X座標。
	\param dy 描画開始Y座標。
	\param rx 読み取り開始X座標。
	\param ry 読み取り開始Y座標。
	\param w 横幅。
	\param h 高さ。
	\param x0 描画左上X座標。
	\param y0 描画左上Y座標。
	\param x1 描画右上X座標。
	\param y1 描画右上Y座標。
	\param x2 描画左下X座標。
	\param y2 描画左下Y座標。
	\param x3 描画右下X座標。
	\param y3 描画右下Y座標。
	\param drafflag 描画反転( 0=反転なし(デフォルト), DRAW_LR=左右判定, DRAW_UD=上下反転, |演算子で両方指定も可能)。
	*/
	virtual int DrawMovieVertex( unsigned int movnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 );

};

//隠蔽したInputクラス
class MikanInputPub
{
public:
	MikanInputPub( void );
	virtual ~MikanInputPub( void );
	// !ゲームパッドの再認識。
	/*!
	  \param resume 追加認識するかどうか(1=デフォルト, 0=初めから)。
	  */
	virtual int RecognitionGamepad( int resume = 1 );
	// !キーの取得(状態の更新)。
	virtual int UpdateKeyInput( void );
	// !ゲームパッドの更新(状態の更新)。
	virtual int UpdatePadInput( void );
	// !キー入力の取得。
	/*!
	  \param keynum キーボード定数(K_キー名。K_ZやK_ESCなど)。
	  \retval 正の値 押されているフレーム数。
	  \retval 0 押されていない。
	  \retval -1 離れた次の1フレーム。
	  */
	virtual int GetKeyNum( int keynum );
	// !キー入力の取得(interval以上押して、かつthresholdフレームごとに1を返す)。
	/*!
	  \param keynum キーボード定数(K_キー名。K_ZやK_ESC、K_UPなど)。
	  \param threshold 1を返す間隔。
	  \param interval 初めの常に0を返す時間(デフォルト0)。
	  \retval 0 条件を満たしていない。
	  \retval 1 条件を満たした。
	  */
	virtual int GetKeyNumRapid( int keynum, int threshold, unsigned int interval = 0 );
	// !何のキーが押されているか返す。
	/*!
	  \retval キー定数 押されているキー定数を返す。複数押されている場合最も最後に押されたキー。
	  \reval -1 何もキーは押されていない。
	  */
	virtual int GetKeyWhichButton( void );

	// !ゲームパッド入力の取得。
	/*!
	  \param padnum パッド番号。
	  \param button パッドのボタン定数(PAD_UPやPAD_Aなど)。
	  */
	virtual int GetPadNum( int padnum, int button );
	// !ゲームパッドのスティック入力の取得(アナログ値の入力)。
	/*!
	  \param padnum パッド番号。
	  \param button アナログスティックのボタン定数(PAD_LS_LRやPAD_X_AXIS等)。
	  */
	virtual int GetPadStick( unsigned int padnum, int button );
	// ゲームパッドがフォースフィードバックに対応しているかどうか調べる。
	virtual int CanForceFeedback( unsigned int padnum );
	// !POVモードの切替。
	/*!
	  \param padnum POVモード(上下左右キー入力とスティック入力の分離)を切り替えるパッド番号。
	  \param povmode (0=上下左右キー入力とスティック入力を同一視, 1=上下左右とスティック入力を別に見る)。
	  */
	virtual int SetPOVMode( unsigned int padnum, int povmode );
	// !挿さっているゲームパッドの数を取得する。
	/*!
	  \retval 値 現在認識しているゲームパッドの数を返す。
	  */
	virtual int GetPadMount( void );
	// !パッドのどのボタンを押したか。
	/*!
	  \retval padnum ボタン押下を調べるパッド番号。
	  \retval パッドのボタン定数 同時に押されている場合最も新しく押された番号の小さいパッドのボタンを優先する。
	  */
	virtual int GetPadWhichButton( unsigned int padnum );

	// !マウスのX座標を取得する。
	/*!
	  \retval X座標 ただしウィンドウ内での座標。
	  */
	virtual int GetMousePosX( void );
	// !マウスのY座標を取得する。
	/*!
	  \param Y座標 ただしウィンドウ内での座標。
	  */
	virtual int GetMousePosY( void );
	// !マウスのボタン入力を取得する。
	/*!
	  \param num マウスのボタン定数(0=左クリック, 1=右クリック, ... etc)。
	  \retval TODO
	  */
	virtual int GetMouseNum( unsigned int num );
	// !マウスカーソルの座標を変更する。
	/*!
	  \param x 設定するX座標(ただしウィンドウ内)。
	  \param y 設定するY座標(ただしウィンドウ内)。
	  */
	virtual int SetMousePos( int x, int y );
	// !マウスカのホイール回転を取得する。
	/*!
	\retval ホイールが回転した瞬間に正負の値が取得できる。0は回転なし。
	*/
	virtual int GetMouseWheel( void );
	// !マウス入力の更新(状態の更新)。
	virtual int UpdateMouseInput( void );

	// !Pad2Keyの機能を有効化する。
	/*!
	  \param enable (0=無効化[デフォルト], 1=有効化)。
	  */
	virtual int SetPad2KeyInput( int enable = 0 );
	// !Pad2Keyのキー組み合わせを設定する。
	/*!
	  \param padnum Pad2Keyを設定するパッド番号。
	  \param padbutton 変換対象のパッドボタン定数。
	  \param keynum 変換後のキーボードのキー定数。
	  */
	virtual int SetPad2KeyCode( unsigned int padnum, int padbutton, int keynum = -1 );
	// !Pad2Keyの状態を更新する。
	virtual int UpdatePad2KeyInput( void );
};

//隠蔽したNetqorkクラス
class MikanNetworkPub
{
public:

	MikanNetworkPub( void );
	~MikanNetworkPub( void );

	// !TCPサーバーを作成する。
	/*!
	  \param nnum ネットワーク番号。
	  \param port 待ち受けポート番号。
	  \param connect コネクト数。
	  */
	virtual SOCKET CreateTCPServer( unsigned int nnum, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	// !UDPサーバーを作成する。
	/*!
	  \param nnum ネットワーク番号。
	  \param port 待ち受けポート番号。
	  \param connect コネクト数。
	  */
	virtual SOCKET CreateUDPServer( unsigned int nnum, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	// !ブロードキャストサーバーを作成する。
	/*!
	  \param nnum ネットワーク番号。
	  \param port 待ち受けポート番号。
	  \param connect コネクト数。
	  */
	virtual SOCKET CreateBloadcastServer( unsigned int nnum, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	virtual SOCKET CreateMulticastServer( unsigned int nnum, const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	virtual SOCKET CreateMulticastServer( unsigned int nnum, unsigned long address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );

	// !TCPクライアントを作成する。
	/*!
	  \param nnum ネットワーク番号。
	  \param address 接続先IPアドレスまたはホスト名。
	  \param port 接続ポート番号。
	  */
	virtual SOCKET CreateTCPClient( unsigned int nnum, const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	// !UDPクライアントを作成する。
	/*!
	  \param nnum ネットワーク番号。
	  \param address 接続先IPアドレスまたはホスト名。
	  \param port 接続ポート番号。
	  */
	virtual SOCKET CreateUDPClient( unsigned int nnum, const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	// !ブロードキャストクライアントを作成する。
	/*!
	  \param nnum ネットワーク番号。
	  \param address 接続先IPアドレスまたはホスト名(デフォルト255.255.255)。
	  \param port 接続ポート番号。
	  */
	virtual SOCKET CreateBloadcastClient( unsigned int nnum, const char *address = "255.255.255.255", unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	virtual SOCKET CreateMulticastClient( unsigned int nnum, const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );

	// !ネットワーク接続を終了する。
	/*!
	  \param nnum 終了するネットワーク番号。
	  */
	virtual int Close( unsigned int nnum );

	// !接続先にデータを送る。
	/*!
	  \param nnum ネットワーク番号。
	  \param str 送る文字列。
	  */
	virtual int Send( unsigned int nnum, const char *str );
	// !接続先にデータを送る。
	/*!
	  \param nnum ネットワーク番号。
	  \param str 送る文字列。
	  \param size 送るデータサイズ。
	  */
	virtual int Send( unsigned int nnum, const char *str, unsigned int size );

	// !接続先からデータを受け取る。
	/*!
	  \param nnum ネットワーク番号。
	  \param getsize 受け取ったデータサイズを格納する変数へのポインタ。
	  \param from 接続相手の情報を格納する変数へのポインタ(UDP系の通信に限る)。
	  \retval 文字列ポインタ 受け取ったデータ(解放必須)。
	  */
	virtual char * Receive( unsigned int nnum, int *getsize, struct sockaddr *from = NULL );
	// !接続先からデータを受け取る。
	/*!
	  \param nnum ネットワーク番号。
	  \param buf 受け取ったデータを格納する変数へのポインタ。
	  \param size 受け取りの最大サイズ。
	  \param from 接続相手の情報を格納する変数へのポインタ(UDP系の通信に限る)。
	  */
	virtual int Receive( unsigned int nnum, char *buf, int size, struct sockaddr *from = NULL );

	// !TCPでの接続を受け入れる(TCPサーバーのみ)。
	/*!
	  \param nnum ネットワーク番号。
	  \param client 接続相手の情報を格納する変数へのポインタ。
	  */
	virtual SOCKET Accept( unsigned int nnum, struct sockaddr_in *client = NULL );

	// !HTTP経由でファイルをダウンロードする(GET)。
	/*!
	  \param address ダウンロード先アドレス。
	  \param filepath ダウンロードしたデータを保存するファイル名(NULL=ダウンロード先のファイル名[デフォルト])。
	  \param getsize 受け取ったデータサイズを格納する変数へのポインタ(HTTPヘッダを調査)。
	  \param loadsize 現在ロードしているデータサイズを格納する変数へのポインタ(マルチスレッド時に有効)。
	  \retval 値 HTTPのリターンコード。
	  */
	static int HttpGet( const char *address, const char *filepath = NULL, int *getsize = NULL, int *loadsize = NULL );
	//  static int HttpGet( const char *address, const char *filepath = NULL, const char *proxy = NULL, const char *user = NULL, const char *password = NULL, int *getsize = NULL, int *loadsize = NULL );

	// !HTTP経由でファイルをダウンロードする(POST)。
	/*!
	  \param address ダウンロード先アドレス。
	  \param data サーバーにPOSTとして送るデータ。
	  \param datasize データサイズ(省略もしくは0にすると文字列の長さを自動的に計算する)。
	  \param filepath ダウンロードしたデータを保存するファイル名(NULL=ダウンロード先のファイル名[デフォルト])。
	  \param getsize 受け取ったデータサイズを格納する変数へのポインタ(HTTPヘッダを調査)。
	  \param loadsize 現在ロードしているデータサイズを格納する変数へのポインタ(マルチスレッド時に有効)。
	  \retval 値 HTTPのリターンコード。
	  */  static int HttpPost( const char *address, const char *data = NULL, int datasize = 0, const char *filepath = NULL, int *getsize = NULL, int *loadsize = NULL );

	// MikanIOの取得。
	/*!
	  \param ionum ファイル番号。
	  \retval ポインタ MikanIOクラスへのポインタ。
	  */
	virtual class MikanIO * GetMikanIO( unsigned int ionum );
};

class FileIOManagementPub
{
public:
	// !開く。
	/*!
	  \param ionum ファイル番号。
	  \param io 開くファイル名。
	  \param mode 開くファイルモード("r"=読み込み[デフォルト], "w"=書き込み, "rw"=読み書き, ... 等。C言語と同じ)。
	  \retval -1 失敗。
	  */
	virtual int Open( unsigned int ionum, const char *io, const char *mode = "r" );
	// !1行読み込む。
	/*!
	  \param ionum ファイル番号。
	  \param read_size 読み込み最大サイズ。
	  \retval ポインタ 受け取ったデータ(解放必須)。
	  */
	virtual char * ReadLine( unsigned int ionum, unsigned int read_size = 0 );
	// !1行読み込む。
	/*!
	  \param ionum ファイル番号。
	  \param buf データを格納するポインタ。
	  \param max_size bufの最大サイズ。
	  \param read_size 読み込んだサイズを格納するポインタ。
	  */
	virtual int ReadLine( unsigned int ionum, char *buf, unsigned int max_size, unsigned int *read_size = NULL );
	// !書き込む。
	/*!
	  \param ionum ファイル番号。
	  \param write_data 書き込むデータ。
	  \param weite_size 書き込みサイズ(-1=NULL文字までのサイズを計算して書き込む[デフォルト])。
	  */
	virtual int Write( unsigned int ionum, const char *write_data, int write_size = -1 );

	// !読み込む。
	/*!
	  \param ionum ファイル番号。
	  \param read_size 読み込み最大サイズ(0=無制限[デフォルト])。
	  \param readed_size 読み込んだデータサイズ(マルチスレッド環境下で有効)。
	  \retval NULL 失敗。
	  */
	virtual void * Read( unsigned int ionum, unsigned int read_size = 0, int *readed_size = NULL );
	// !読み込む。
	/*!
	  \param ionum ファイル番号。
	  \param data 読み込みバッファへのポインタ。
	  \param read_size 読み込み最大サイズ。
	  \param readed_size 読み込んだデータサイズ(マルチスレッド環境下で有効)。
	  \retval -1 失敗。
	  \retval 値 読み込んだデータサイズ
	  */
	virtual int Read( unsigned int ionum, void *data, unsigned int read_size, int *readed_size = NULL );
	// シークする。
	/*!
	  \param ionum ファイル番号。
	  \param offset オフセット。
	  \param seekmode シーク開始場所(SEEK_CUR=現在値から[デフォルト], SEEK_SET=ファイルの先頭, SEEK_END=ファイルの末尾。C言語と同じ)。
	  \retval -1 失敗。
	  */
	virtual int Seek( unsigned int ionum, int offset, int seekmode = SEEK_CUR );
	// 書き込む。
	/*!
	  \param ionum ファイル番号。
	  \param write_data 書き込むデータ。
	  \param write_size 書き込むサイズ(-1=NULL文字までのサイズを計算して書き込む[デフォルト])。
	  \retval -1 失敗。
	  \retval 値 書き込んだサイズ。
	  */
	virtual int Write( unsigned int ionum, const void *write_data, int write_size = -1 );
	// 整形して書き込む(1024バイトまで。足りない場合は_MikanFileのPrintfを使うこと)。
	/*!
	  \param ionum ファイル番号。
	  \param format 整形文字列
	  \param ... 整形文字列で出力する引数。
	  \retval -1 失敗。
	  \retval 値 書き込んだサイズ。
	  */
	virtual int Printf( unsigned int ionum, const char *format, ... );
	// !今まで読んだバイト数を返す。
	/*!
	  \param ionum ファイル番号。
	  \retval -1 失敗
	  \retval 値 今まで読んだバイト数を返す。
	  */
	virtual int Tell( unsigned int ionum );
	// !サイズを返す。
	/*!
	  \param ionum ファイル番号。
	  \retval -1 失敗。
	  */
	virtual int Size( unsigned int ionum );
	// !閉じる。
	/*!
	  \param ionum ファイル番号。
	  \retval -1 失敗。
	  */
	virtual int Close( unsigned int ionum );

	// !IOを破棄する。
	/*!
	  \param ionum ファイル番号。
	  */
	virtual int Destroy( unsigned int ionum );
	// !すべて閉じる。
	/*!
	  \retval 値 閉じることに成功したIOの数を返す。
	  */
	virtual int CloseAll( void );

	static int Remove( const char *file );

	static int Move( const char *oldfile, const char *newfile );

	// FileIOの取得。
	/*!
	  \param ionum ファイル番号。
	  \retval ポインタ FileIOクラスへのポインタ。
	  */
	virtual class FileIO * GetIO( unsigned int ionum );

	// MikanIOの取得。
	/*!
	  \param ionum ファイル番号。
	  \retval ポインタ MikanIOクラスへのポインタ。
	  */
	virtual class MikanIO * GetMikanIO( unsigned int ionum );
};

#endif
