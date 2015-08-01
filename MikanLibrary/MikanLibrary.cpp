/*//メイン
#define _CRTDBG_MAP_ALLOC
//#define _CRTDBG_MAPALLOC
#include <stdlib.h>
#include <crtdbg.h>*/

#include "MikanLibrary.h"
#include <Winuser.h>

#include "../MikanMovie/MikanMovieManagement.h"
//#include <mmsystem.h>

//公開クラス
CLASSDEF_MIKAN_SYSTEM        *MikanSystem;
CLASSDEF_MIKAN_WINDOW        *MikanWindow;
CLASSDEF_MIKAN_DIRECTX_DRAW  *MikanDraw;
CLASSDEF_MIKAN_MIKANMOVIE    *MikanMovie;
CLASSDEF_MIKAN_DIRECTX_INPUT *MikanInput;
#ifndef MIKAN_NO_SOUND
CLASSDEF_MIKAN_DIRECTX_SOUND *MikanSound;
#endif
CLASSDEF_MIKAN_NETWORK       *MikanNet;
CLASSDEF_MIKAN_FILE          *MikanFile;
//非公開クラス
CLASSDEF_SYSTEM              *_MikanSystem;
CLASSDEF_WINDOW              *_MikanWindow;
CLASSDEF_DIRECTX_DRAW        *_MikanDraw;
CLASSDEF_MIKANMOVIE          *_MikanMovie;
CLASSDEF_DIRECTX_INPUT       *_MikanInput;
#ifndef MIKAN_NO_SOUND
CLASSDEF_DIRECTX_SOUND       *_MikanSound;
#endif
CLASSDEF_NETWORK             *_MikanNet;
//CLASSDEF_FILE
class FileIOManagement       *_MikanFile;

//crtdbg.h
/*#ifdef _DEBUG
 #define new                new(_NORMAL_BLOCK, __FILE__, __LINE__)
 #define malloc(s)          _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
 #define calloc(c, s)       _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
 #define realloc(p, s)      _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
 #define _recalloc(p, c, s) _recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
 #define _expand(p, s)      _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif*/

#ifdef _DEBUG
#define DEGUB_TIME( n ) { tm[ (n) ] = timeGetTime(); }
#else
#define DEGUB_TIME( n )
#endif

char CLASSNAME[ WBUFLEN ] = "UseMikanLibraryProject";

int( *WinProcMovie )( HWND, UINT, WPARAM, LPARAM );

LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_INPUT: // マウスなどの低レベル入力。
		_MikanInput->UpdateRawInputStatus( lParam );
		break;
		//case WM_TOUCH: // タッチの低レベル入力。
		//break;
	case WM_ACTIVATE:
		if ( LOWORD( wParam ) == WA_INACTIVE )
		{
			//非アクティブ化
			_MikanSystem->SetWindowState( 0 );
		} else
		{
			//アクティブ化
			_MikanSystem->SetWindowState( 1 );
		}
		break;
	case WM_SYSCOMMAND:
		if ( wParam == SC_SCREENSAVE && _MikanSystem->GetPermitScreenSaver() == 0 )
		{
			// スクリーンセーバーの起動を阻止。
			return 1;
		}
		break;
	case WM_GRAPH_NOTIFY: // MikanMoovie
		if ( _MikanMovie )
		{
			_MikanMovie->WinProcMovieFunc( hWnd, uMsg, wParam, lParam );
		}
		//if ( WinProcMovie ){ (*WinProcMovie)( hWnd, uMsg, wParam, lParam ); }
		break;
	case WM_CLOSE://ウィンドウが閉じた時
	case WM_DESTROY://ウィンドウが破棄された時
		//PostQuitMessage( 0 );
		_MikanSystem->SetGameLoop();
		break;
		//  case WM_PAINT://ウィンドウが再描画される時
		//    return 0;
	case WM_SIZE://ウィンドウのサイズが変わった時
		switch ( wParam )
		{
		case SIZE_RESTORED://サイズ変更
			break;
		case SIZE_MAXIMIZED://ウィンドウ最大化
			break;
		}
		break;
	case WM_CREATE://ウィンドウが作られた時
		//ここら辺でスクリーン選択画面を出すのもあり。
		//デバイスコンテキストの取得
		_MikanWindow->wd.Screen = CreateCompatibleDC( GetDC( hWnd ) );
		//_MikanWindow->wd.WindowParent = hWnd;
		_MikanWindow->wd.WindowHandle = hWnd;
		DragAcceptFiles( hWnd, TRUE );
		break;
	case WM_DROPFILES:
		_MikanWindow->ReceiveDragFiles( (HDROP)wParam );
		break;
	}
	//これがないとウィンドウ生成ができない。
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//スレッド実行用関数
unsigned long WINAPI MikanThreadFunc( void * param )
{
	int ret;
	struct ThreadData *td = ( struct ThreadData * )param;

	if ( td == NULL )
	{
		ExitThread( -1 );
	}

	if ( _MikanSystem )
	{
		ret = _MikanSystem->RunThread_( td );
	}

	td->handle = NULL;

	ExitThread( ret );
}

//フレーム待ち関数の実体
int WaitSleep( double fps, double *getfps )
{
	static unsigned int beforetime = timeGetTime(), frames = 0;
	unsigned int nowtime, progress, idealtime;
	Sleep( 0 );
	//windowsが起動してからのミリ秒を取得
	nowtime = timeGetTime();
	//進行時間
	progress = nowtime - beforetime;
	//FPSの計算
	*getfps = (double)( frames ) / (double)(progress)* 1000.0;
	//前の時間から1秒以上経った
	if ( progress >= 1000 )
	{
		//1秒引く
		progress = 0;//-= 1000;
		//前の時間の更新
		beforetime = nowtime;
		//フレーム数の初期化
		frames = 0;
	}

	++frames;

	//フレーム数から前の時間からどれだけ待つかを計算する
	idealtime = (int)( frames * ( 1000.0 / fps ) );
	if ( idealtime > progress )
	{
		Sleep( idealtime - progress );
		return ( idealtime - progress );
	}
	return 0;
}

int WaitBusy( double fps, double *getfps )
{
	static unsigned int beforetime = timeGetTime(), frames = 0;
	int ct = 0;
	unsigned int nowtime, progress, idealtime;
	Sleep( 0 );
	//windowsが起動してからのミリ秒を取得
	nowtime = timeGetTime();
	//進行時間
	progress = nowtime - beforetime;
	//FPSの計算
	*getfps = (double)( frames ) / (double)(progress)* 1000.0;
	//前の時間から1秒以上経った
	if ( progress >= 1000 )
	{
		//1秒引く
		progress = 0;//-= 1000;
		//前の時間の更新
		beforetime = nowtime;
		//フレーム数の初期化
		frames = 0;
	}

	++frames;

	//フレーム数から前の時間からどれだけ待つかを計算する
	idealtime = (int)( frames * ( 1000.0 / fps ) );
	nowtime += idealtime - progress - 1;
	//進行時間が前の時間から待つ時間より大きい場合スリープする
	while ( nowtime > timeGetTime() )
	{
		++ct;
		Sleep( 1 );
	}
	return ct;
}

int WaitNone( double fps, double *getfps )
{
	return 0;//now - ideal - prog;
}

/*!
@class   MikanLibrary MikanLibrary.h "MikanLibrary.h"
@brief   _MikanSystemクラス。ゲームに必要な処理を行う。
@author  Hiroki
@version 1.0
*/

/*!
@brief _MikanSystem(Mikanライブラリ)のコンストラクタ
@par   関数説明
各公開・非公開クラスの生成や変数初期化などを行う。
*/
MikanLibrary::MikanLibrary( void )
{
	//ポジションの設定
	position = MIKAN_POSITION_BEGIN;
	//非公開クラス
	_MikanWindow = new Window();
	_MikanDraw = new DirectXDraw9();
	_MikanMovie = new MikanMovieManagement_( 4 );
	_MikanInput = new DirectXInput();
#ifndef MIKAN_NO_SOUND
	_MikanSound = new ThaleiaSound();
#endif
	_MikanNet = new MikanNetwork();
	_MikanFile = new FileIOManagement();

	//公開クラス
	MikanSystem = new MikanSystemPub();
	MikanWindow = new MikanWindowPub();
	MikanDraw = new MikanDrawPub();
	MikanMovie = new MikanMovieManagementPub();
	MikanInput = new MikanInputPub();
#ifndef MIKAN_NO_SOUND
	MikanSound = new MikanSoundPub();
#endif
	MikanNet = new MikanNetworkPub();
	MikanFile = new FileIOManagementPub();

	// ゲームループ開始。
	SetGameLoop( 1 );
}

/*!
@brief _MikanSystem(Mikanライブラリ)のデストラクタ
@par   関数説明
生成した各クラスを破棄する。
*/
MikanLibrary::~MikanLibrary( void )
{
	//ポジションの設定
	position = MIKAN_POSITION_END;



	delete( _MikanWindow );
	delete( _MikanDraw );
	delete( _MikanMovie );
	delete( _MikanInput );
#ifndef MIKAN_NO_SOUND
	delete( _MikanSound );
#endif
	delete( _MikanNet );
	delete( _MikanFile );
	delete( MikanSystem );
	delete( MikanWindow );
	delete( MikanDraw );
	delete( MikanMovie );
	delete( MikanInput );
#ifndef MIKAN_NO_SOUND
	delete( MikanSound );
#endif
	delete( MikanNet );
	delete( MikanFile );
}

/*!
@brief 初期化関数
@par   関数説明
ウィンドウの設定などの初期化を行う。
@param hInstance WinMain関数に渡される第1引数。
@return 0を返す。
*/
int MikanLibrary::Init( HINSTANCE hInstance )
{
	int i;
	WNDCLASSEX *wcx;

	//垂直同期待ちの設定
	vsync = 0;

	//待ち関数の設定
	SetWaitFunction();

	SetAfterDeviceLostFunction( NULL );

	wcx = &( _MikanWindow->wd.wcx );

	this->hInstance = hInstance;

	_MikanWindow->InitWindowClassEx( hInstance );
	_MikanWindow->SetWindowProc( ::WindowProc );

	_MikanWindow->SetScreenSize( 640, 480 );

	//fps初期値
	fps = MikanFirstFPS;
	SetInactiveWindow();
	SetWindowState();
	//view = 1;
	devicelost = 0;

	thread_max = 16;

	for ( i = 0 ; i < LOCK_HANDLE_MAX ; ++i )
	{
		lockhandle[ i ] = NULL;
	}

	_MikanSystem->SetPermitScreenSaver( 0 );

	return 0;
}

int MikanLibrary::SecondInit( void )
{
	//スレッド
	tdata = ( struct ThreadData * )calloc( thread_max, sizeof( struct ThreadData ) );

	return 0;
}

//ゲームループを設定
int MikanLibrary::SetGameLoop( int loop )
{
	return ( gameloop = loop );
}

/*!
@brief 初期化関数
@par   関数説明
ウィンドウの設定などの初期化を行う。
@param hInstance WinMain関数に渡される第1引数。
@return 0を返す。
*/
HINSTANCE MikanLibrary::GetInstance( void )
{
	return hInstance;
}

/*!
@brief Mikanライブラリの主な処理
@par   関数説明
Mikanライブラリでの処理が行われる。WinMain関数からこれが呼ばれ、実質これがWinMain関数となって動作している。
@param hInstance WinMain関数に渡される第1引数。
@param hPrevInstance WinMain関数に渡される第2引数。
@param lpCmdLine WinMain関数に渡される第3引数。
@param nCmdShow WinMain関数に渡される第4引数。
@return 1=ウィンドウクラスの登録失敗
@return 2=ウィンドウ生成に失敗。
@return 3=DirectXの初期化が出来ずに失敗。
@return その他=メッセージのパラメーターを返す。
*/
int MikanLibrary::WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow )
{
	unsigned int i;
	MSG msg = {};
	//ウィンドウハンドル
	HWND myhwnd = NULL;
	//RECT構造体
	RECT myrect;
	//  unsigned long atom;

	if ( Init( hInstance ) )
	{//初期化に失敗したので終了
		return 1;
	}

	//ポジションの設定
	position = MIKAN_POSITION_SYSTEMINIT;
	//ユーザー
	SystemInit();
	//ポジションの設定
	position = MIKAN_POSITION_LIBLARYINIT;

	SecondInit();

	//垂直同期待ちの設定
	_MikanDraw->SetVsyncWait( vsync );

	// ネットワークの初期化
	_MikanNet->SecondInit();

	//ウィンドウクラスの登録
	//  atom = RegisterClassEx( &( _MikanWindow->GetWindowClassEx() ) );
	//  if( atom == NULL )
	if ( _MikanWindow->RegistClassEx() )
	{
		//登録失敗
		return 1;
	}

	if ( _MikanWindow->MakeWindow( nCmdShow ) || _MikanWindow->wd.Screen == NULL )
	{
		//ウィンドウが作れなかったので終了
		return 2;
	}

	//デスクトップのウィンドウハンドル取得
	myhwnd = GetDesktopWindow();
	if ( myhwnd && GetClientRect( myhwnd, &myrect ) )
	{
		//ウィンドウポジションの変更
		MikanWindow->SetPositionXY( ( myrect.right - MikanWindow->GetWindowWidth() ) / 2,
			( myrect.bottom - MikanWindow->GetWindowHeight() ) / 2 );
	}

	//Direct3D
	//ウィンドウハンドルのセット
	_MikanDraw->SetWindowHandle( _MikanWindow->GetWindowHandle() );
	//DirectXの初期化
	if ( _MikanDraw->InitD3D() )
	{
		//DirectXの初期化ができなかったので終了
		return 3;
	}
	_MikanDraw->InitD3DObject();

	( ( class MikanMovieManagement_ * )_MikanMovie )->SecondInit( _MikanDraw );
	//DirectInput
	//入力のセットアップ
	_MikanInput->SetUp( _MikanWindow->GetWindowHandle(), hInstance );

#ifndef MIKAN_NO_SOUND
	//DirectSound
	//初期化
	_MikanSound->Initialize( _MikanWindow->GetWindowHandle() );
#endif
	//ポジションの設定
	position = MIKAN_POSITION_USERINIT;

	waitsleep = 0;
	DrawBegin();

	//ユーザー
	UserInit();

	WaitNextFrame( 1 );

	//windowsが起動してからのミリ秒を取得
	beforetime = timeGetTime();
	//フレーム数の初期化
	frames = 0;

	//ポジションの設定
	position = MIKAN_POSITION_MAINLOOP;
	while ( gameloop )
	{
		DEGUB_TIME( 0 );
		while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			/*      if (msg.message == WM_QUIT)
				  {
				  return EXIT_SUCCESS;
				  }*/
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		DEGUB_TIME( 1 );

		//ゲーム部分
		if ( devicelost || ( _MikanSystem->IsActive() == 0 && GetInactiveWindow() == 1 ) )
		{
			//何らかの原因で動作ができない。

			if ( devicelost )
			{
				//デバイスロスト
				devicelost = _MikanDraw->RecoverFromDeviceLost();
				if ( devicelost == 0 && AfterDeviceLost )
				{
					( *AfterDeviceLost )( );
				}
			} else// if( GetInactiveWindow() == 0 )
			{
				//ウィンドウが非アクティブ
				//ただし非アクティブ時に動作を止めない設定だと、ここにはこない。
				Sleep( 10 );
			}
		} else
		{
			//正常に動作している。

			DEGUB_TIME( 2 );
			//ゲームパッド入力の更新
			_MikanInput->UpdatePadInput();
			// ゲームパッドのPad2Keyの更新。
			_MikanInput->UpdatePad2KeyInput();
			//キー入力の更新
			_MikanInput->UpdateKeyInput();
			//マウス入力の更新
			_MikanInput->UpdateMouseInput();
			// 低レベル入力の更新。
			_MikanInput->UpdateRawInput();
			DEGUB_TIME( 3 );
			//描画開始
			DrawBegin();
			// MikanMovieからDirectXを奪い返す。
			_MikanMovie->Reset();
			//ユーザー
			if ( MainLoop() )
			{
				break;
			}
			DEGUB_TIME( 4 );
			//描画終了
			//次のフレームまで待機
			WaitNextFrame( 1 );
			DEGUB_TIME( 6 );
#ifdef _DEBUG
			tm[ 10 ] = tm[ 4 ] - tm[ 3 ];
			tm[ 11 ] = tm[ 5 ] - tm[ 4 ];
			tm[ 12 ] = tm[ 6 ] - tm[ 5 ];
#endif
		}
	}

	//ポジションの設定
	position = MIKAN_POSITION_CLEANUP;
	//ユーザー
	CleanUp();

	position = MIKAN_POSITION_RELEASE;

	// スレッド全停止
	for ( i = 0 ; i < thread_max ; ++i )
	{
		StopThread( i );
	}
	for ( i = 0 ; i < thread_max ; ++i )
	{
		while ( _MikanSystem->GetThreadHandle( i ) )
		{
			Sleep( 1 );
		}
	}
	// ロック全停止
	for ( i = 0 ; i < LOCK_HANDLE_MAX ; ++i )
	{
		ReleaseLock( i );
	}
	if ( tdata )
	{
		free( tdata ); tdata = NULL;
	}

	//DirectXの掃除
	_MikanDraw->CleanupD3DObject();
	_MikanDraw->CleanupD3D();
	_MikanDraw->CleanupFont();
#ifndef MIKAN_NO_SOUND
	MikanSound->UnloadAll();
#endif

	return msg.wParam;
}

/*!
@brief FPSを設定する。
@par   関数説明
FPSを設定する。
@param fps 設定するFPS。1秒間に何回実行するか。省略した場合60.0。
@return 設定した値。
*/
double MikanLibrary::SetFPS( double fps )
{
	return ( this->fps = fps );
}

/*!
@brief FPSを取得する。
@par   関数説明
今のFPSを計測して返す。
@return 現在のFPS。計測した値が出る。ずっと低い値が出た場合は処理落ちを起こしている。
*/
double MikanLibrary::GetFPS( void )
{
	return getfps;//frames > 0 ? (double)(start-beforetime) / frames : 0.0;//( process < (1000.0 / fps) ) ? 60 : process / ( 1000.0 / fps );
}

/*!
@brief 次のフレームまで待つ
@par   関数説明
実行すると前の時間などと比較し、どれだけ休めばよいかを計算し、その時間の分だけSleepする。
これで待つことによって、FPS制御を行なっている。
@return Sleepした時間(ミリ秒)。
*/
int MikanLibrary::WaitNextFrame( int draw )
{
	// 画面に描画
	if ( draw )
	{
		DrawEnd();
	}
	DEGUB_TIME( 5 );
	// 待ち関数に処理を投げる
	( *Wait )( fps, &getfps );

	return 0;
}

//待ち関数の設定
int MikanLibrary::SetWaitFunction( int WaitFunc( double fps, double *getfps ) )
{
	Wait = WaitFunc;
	return 0;
}

int MikanLibrary::SetWaitFunction( int type )
{
	switch ( type )
	{
	case WAIT_BUSY:
		Wait = WaitBusy;
		break;
	case WAIT_NONE:
		Wait = WaitNone;
		break;
	case WAIT_SLEEP:
	default:
		Wait = WaitSleep;
		break;
	}
	return 0;
}

int MikanLibrary::SetAfterDeviceLostFunction( int( *func )( void ) )
{
	AfterDeviceLost = func;
	return 0;
}

/*!
@brief デバイスロストを起こしているかどうか調べる
@par   関数説明
今現在デバイスロストを起こしているかどうかを調べる。
ただし、実行中にMainLoop内のプログラムは実行されないため、別手段を用いなければ意味が無い。
@return 0=正常。
@return その他=デバイスロスト中
*/
int MikanLibrary::IsDeviceLost( void )
{
	return devicelost;
}

/*!
@brief 現在の処理位置を調べる
@par   関数説明
現在の処理位置を調べる。処理位置はMIKAN_POSITION_で始まる。
@return MIKAN_POSITION_BEGIN=開始
@return MIKAN_POSITION_SYSTEMINIT=SystemInit実行中
@return MIKAN_POSITION_LIBLARYINIT=ライブラリの初期化中
@return MIKAN_POSITION_USERINIT=UserInit実行中
@return MIKAN_POSITION_MAINLOOP=MainLoop実行中
@return MIKAN_POSITION_CLEANUP=CleanUp実行中
@return MIKAN_POSITION_RELEASE=ライブラリのリソース解放作業など
@return MIKAN_POSITION_END=最後のクラス開放中
*/
int MikanLibrary::GetMikanPosition( void )
{
	return position;
}

/*!
@brief 垂直同期待ちの設定
@par   関数説明
垂直同期を待つかどうかの設定。
@param vs 0以外で垂直同期待ちし、0で待たない。-1は特殊な返り値なので使わないことをすすめる。
@return -1 失敗。
@return その他 設定した値が返ってくる。
*/
int MikanLibrary::SetVsyncWait( int vs )
{
	vsync = vs;
	if ( _MikanDraw )
	{
		SetWaitFunction( WAIT_NONE );
		return vsync;
	}
	return -1;
}

/*!
@brief ウィンドウモードの変更
@par   関数説明
ウィンドウをフルスクリーンorウィンドウで切り替える。
@param fullscreen_on 1=フルスクリーンモード、0=ウィンドウモード。
*/
int MikanLibrary::SetFullScreen( int fullscreen_on )
{
	if ( position == MIKAN_POSITION_SYSTEMINIT )
	{
		// SystemInit
		_MikanWindow->SetWindowStyle( WT_FULLSCREEN );
		_MikanDraw->SetFullScreenMode( fullscreen_on );
	} else
	{
		// それ以外。
		if ( fullscreen_on )
		{
			// ウィンドウ設定のバックアップ。
			_MikanWindow->BackUpWindow();
		} else
		{
			// ウィンドウ設定の復帰。
			_MikanWindow->RepairWindow();
		}
		// フルスクリーンモードにする。
		_MikanWindow->SetWindowStyle( WT_FULLSCREEN );
		_MikanDraw->SetFullScreenMode( fullscreen_on );

		// 復帰作業。
		_MikanDraw->RecoverFromDeviceLost( 1 );
	}

	return 0;
}

/*!
@brief ウィンドウ非アクティブ時の動作設定
@par   関数説明
ウィンドウが非アクティブになったときの挙動について設定する。
@param flag 0で非アクティブ時も動作を続け、それ以外の値で非アクティブ時に動作を止める。
@return 0 非アクティブ時にゲームを動作をさせる。
@return 1 非アクティブ時にゲームの動作を止める。
*/
int MikanLibrary::SetInactiveWindow( int flag )
{
	return ( view = flag );
}

/*!
@brief ウィンドウ非アクティブ時の動作を取得
@par   関数説明
ウィンドウが非アクティブになったときの挙動について返す。
@return 0 非アクティブ時にゲームの動作をさせる。
@return 1 非アクティブ時にゲームの動作を止める。
*/
int MikanLibrary::GetInactiveWindow( void )
{
	return view;
}

/*!
@brief ウィンドウの状態を設定
@par   関数説明
ウィンドウがアクティブか非アクティブかの設定を行う。
@param flag 0で非アクティブ化。それ以外でアクティブ化。
@return 0 非アクティブ化
@return 1 アクティブ化
*/
int MikanLibrary::SetWindowState( int flag )
{
	return ( active = flag );
}

/*!
@brief ウィンドウがアクティブかどうか調べる
@par   関数説明
ウィンドウが現在アクティブかそうでないかを調べる。
@return 0 非アクティブ
@return 1 アクティブ
*/
int MikanLibrary::IsActive( void )
{
	return active;
}

int MikanLibrary::SetPermitScreenSaver( int cannotpermit )
{
	return ( permitscreensaver = cannotpermit );
}

int MikanLibrary::GetPermitScreenSaver( void )
{
	return permitscreensaver;
}

//スレッドを立てて関数を実行する
int MikanLibrary::RunThread( unsigned int tnum, int( *thread_func )( void * vparam ), void *param )
{
	if ( tnum > thread_max || tdata == NULL )
	{
		return 1;
	}

	if ( tdata[ tnum ].handle != NULL )
	{
		StopThread( tnum );
	}

	tdata[ tnum ].num = tnum;
	tdata[ tnum ].thread_func = thread_func;
	tdata[ tnum ].param = param;
	tdata[ tnum ].type = 1;
	tdata[ tnum ].handle = CreateThread( NULL, 0, MikanThreadFunc, (void *)&( tdata[ tnum ] ), 0, &( tdata[ tnum ].threadid ) );

	return 0;
}

int MikanLibrary::RunThread( unsigned int tnum, int( *thread_func )( void ) )
{
	if ( tnum > thread_max || tdata == NULL )
	{
		return 1;
	}

	if ( tdata[ tnum ].handle != NULL )
	{
		StopThread( tnum );
	}

	tdata[ tnum ].num = tnum;
	tdata[ tnum ].thread_func = thread_func;
	tdata[ tnum ].param = NULL;
	tdata[ tnum ].type = 0;
	tdata[ tnum ].handle = CreateThread( NULL, 0, MikanThreadFunc, (void *)&( tdata[ tnum ] ), 0, &( tdata[ tnum ].threadid ) );

	return 0;
}

int MikanLibrary::RunThread_( struct ThreadData *td )
{
	int ret = -1;
	//パラメーターの有無で実行関数を変化させる。
	//何というか、キャストとかいろいろ壮絶。

	if ( td )
	{
		if ( td->type )
		{
			//int 関数名( void * vparam ); という関数を受け取って実行
			ret = ( ( int( *)( void * vparam ) )( td->thread_func ) )( td->param );
		} else
		{
			//int 関数名( void ); という関数を受け取って実行
			ret = ( ( int( *)( void ) )( td->thread_func ) )( );
		}
	}

	return ret;
}

//立てたスレッドを破棄する
int MikanLibrary::StopThread( unsigned int tnum )
{
	if ( tnum > thread_max || tdata == NULL || tdata[ tnum ].handle == NULL )
	{
		return 1;
	}

	TerminateThread( tdata[ tnum ].handle, 1 );
	tdata[ tnum ].handle = NULL;

	return 0;
}

// スレッドのハンドルを取得する。
void * MikanLibrary::GetThreadHandle( unsigned int tnum )
{
	return tdata[ tnum ].handle;
}

int MikanLibrary::Join( unsigned int tnum, unsigned long sleep )
{
	if ( tnum > thread_max || tdata == NULL || tdata[ tnum ].handle == NULL )
	{
		return 1;
	}
	if ( tnum <= 0 )
	{
		tnum = 1;
	}

	while ( GetThreadHandle( tnum ) )
	{
		Sleep( sleep );
	}

	return 0;
}

int MikanLibrary::LockMainLoop( void )
{
	// 前回MainLoop終了間際にロックされ、そのまますぐ再ロックされた場合にMainLoopでの処理が遅れる。結構多発する。
	// それの回避のため、描画を終わる際にロックされているとdrawableが2以上になる。
	// 積み重なった終了したいという気持ち(drawable)が2以上の場合はその気持ちを尊重し終了させる。
	while ( drawable > 1 )
	{
		Sleep( 1 );
	}
	waitsleep = 1;
	while ( drawable == 0 )
	{
		Sleep( 1 );
	}
	return 0;
}

int MikanLibrary::UnlockMainLoop( void )
{
	waitsleep = 0;
	return 0;
}

int MikanLibrary::WaitClearScreen( int value )
{
	// ClearScreenは描画開始から終了までの間実行回数を足し続ける。
	// ClearScreenの実行場所と回数は大体同じ箇所になるので、本人は何回実行したかでタイミングを知ることができる。よね？
	// それの簡易版。
	while ( _MikanDraw->CountClearScreen() < value )
	{
		Sleep( 1 );
	}
	return 0;
}

int MikanLibrary::CreateLock( unsigned int lnum )
{
	if ( LOCK_HANDLE_MAX <= lnum )
	{
		return 1;
	}
	if ( lockhandle[ lnum ] )
	{
		ReleaseLock( lnum );
	} else
	{
		sprintf_s( locknames[ lnum ], "MIKANMUTEX_%d", lnum );
	}
	lockhandle[ lnum ] = CreateMutex( 0, FALSE, locknames[ lnum ] );
	return 0;
}

unsigned long MikanLibrary::Lock( unsigned int lnum, unsigned long timeout )
{
	if ( LOCK_HANDLE_MAX <= lnum || lockhandle[ lnum ] == NULL )
	{
		return 1;
	}
	return WaitForSingleObjectEx( lockhandle[ lnum ], timeout, false );
}

int MikanLibrary::Unlock( unsigned int lnum )
{
	if ( LOCK_HANDLE_MAX <= lnum || lockhandle[ lnum ] == NULL )
	{
		return 1;
	}
	return ReleaseMutex( lockhandle[ lnum ] );
}

int MikanLibrary::ReleaseLock( unsigned int lnum )
{
	if ( LOCK_HANDLE_MAX <= lnum || lockhandle[ lnum ] == NULL )
	{
		return 1;
	}
	CloseHandle( lockhandle[ lnum ] );
	lockhandle[ lnum ] = NULL;
	return 0;
}

int MikanLibrary::DrawBegin( void )
{
	_MikanDraw->Begin();
	drawable = 1;

	return 0;
}

int MikanLibrary::DrawEnd( void )
{
	// 描画ロック待ち
	// 終了したい気持ちを伝えるため、drawableを足し続ける。
	while ( waitsleep && drawable++ )
	{
		Sleep( 1 );
	}
	drawable = 0;
	devicelost = _MikanDraw->End();

	return 0;
}




