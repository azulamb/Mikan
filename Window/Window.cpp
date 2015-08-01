/*!
@class   Window Window.h "Window.h"
@brief   Window生成などを行うクラス
@author  Hiroki
@version 1.0
*/
#include "Window.h"

/*!
@brief コンストラクタ(通常)
@par   関数説明
変数を初期化する。
*/
Window::Window( void )
{
	wd.wcx.cbSize = sizeof( WNDCLASSEX );
	wd.wcx.hIcon = wd.wcx.hIconSm = NULL;
	strcpy_s( CLASSNAME, WBUFLEN, MIKANCLASSNAME );
	//クラスネームの設定
	wd.wcx.lpszClassName = TEXT( MIKANCLASSNAME );//SetClassNameEx( "MikanWindow" );
	//ウィンドウの名前の設定
	SetWindowName( "Window" );//TEXT( __FILE__ )
	//wd.wc.lpszClassName = wd.ClassName;
	//ウィンドウスタイルの設定
	SetWindowStyle( WT_DEFAULT );
	//拡張ウィンドウスタイルの設定
	SetWindowStyleExDirect( NULL );
	//ウィンドウを標準の位置に設定
	SetPositionXY( 0, 0 );//CW_USEDEFAULT, CW_USEDEFAULT );
	//ウィンドウサイズを640*480に設定
	//  SetWindowSize( WF_WIDTH  + GetSystemMetrics(SM_CXEDGE) + GetSystemMetrics(SM_CXBORDER) + GetSystemMetrics(SM_CXDLGFRAME),
	//                 WF_HEIGHT + GetSystemMetrics(SM_CYEDGE) + GetSystemMetrics(SM_CYBORDER) + GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION));
	SetScreenSize( WF_WIDTH, WF_HEIGHT );
	//ウィンドウハンドルの設定
	SetWindowHandle( NULL );
	SetParentWindowHandle( NULL );
	//  SetWindowClass( NULL );
	wd.Screen = NULL;
	wd.hMenu = NULL;
	wd.hInstance = NULL;
	wd.lpParam = NULL;
	drophandle = NULL;
	dragfilesmax = 0;
	ReceiveDragFiles_ = NULL;
	dragfilepath[ 0 ] = '\0';
	fullscreen = 0;
}

/*!
@brief デストラクタ
@par   関数説明
何もしていない。
*/
Window::~Window( void )
{
}

int Window::InitWindowClassEx( HINSTANCE hInstance )
{
	wd.wcx.style = CS_HREDRAW | CS_VREDRAW;
	//wcx->lpfnWndProc   = ::WindowProc; //ウィンドウプロシージャ
	SetWindowProc( DefWindowProc );
	wd.wcx.cbClsExtra = 0;
	wd.wcx.cbWndExtra = 0;
	wd.wcx.hInstance = wd.hInstance = hInstance;
	wd.wcx.hIcon = LoadIcon( NULL, IDI_APPLICATION ); //アイコン
	//  wc->hIcon         = LoadIcon(hInstance,TEXT("ICON_16"));
	wd.wcx.hCursor = LoadCursor( NULL, IDC_ARROW ); //カーソル
	//  wc->hCursor       = LoadCursor(hInstance,TEXT("MYCURSOR"));//
	wd.wcx.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wd.wcx.lpszMenuName = NULL;
	wd.wcx.hIconSm = LoadIcon( NULL, IDI_APPLICATION ); //アイコン
	return 0;
}

int Window::RegistClassEx( void )
{
	unsigned long atom;
	//ウィンドウクラスの登録
	atom = RegisterClassEx( &( GetWindowClassEx() ) );
	if ( atom == NULL )
	{
		//登録失敗
		return 1;
	}
	return 0;
}

/*!
@brief ウィンドウの生成
@par   関数説明
ウィンドウを生成する。
決まったウィンドウサイズやスタイルがあるのであればあらかじめSet系関数で設定しておく。
@param nCmdShow WinMain関数で取得できる第三引数。
@return 0=成功。
@return 1=生成に失敗。
*/
unsigned long Window::MakeWindow( int nCmdShow )
{
	//int w = 0, h = 0;
	unsigned long err;

	if ( wd.WindowParent )
	{
		SetWindowStyleDirect( GetWindowStyle() | WS_CHILD );
	} else
	{
		SetWindowStyleDirect( GetWindowStyle() & ( ~WS_CHILD ) );
	}

	// 判定。
	//if ( GetPositionX() < 0 ){ SetPositionX( 0 ); w = GetWindowWidth();  SetWindowWidth( 640 );  }
	//if ( GetPositionY() < 0 ){ SetPositionY( 0 ); h = GetWindowHeight(); SetWindowHeight( 480 ); }

	//ウィンドウの生成
	wd.WindowHandle = CreateWindowEx(
		GetWindowStyleEx(),
		wd.wcx.lpszClassName,//MIKANCLASSNAME,//GetClassNameEx(),
		GetWindowName(),
		GetWindowStyle(),
		GetPositionX(), GetPositionY(),
		GetWindowWidth(), GetWindowHeight(),
		GetParentWindowHandle(),
		GetMenuHandle(),
		GetInstanceHandle(),
		GetParameter()
		);
	/*if ( w > 0 || h > 0 )
	{
	SetWindowSize( w, h );
	SetWindow();
	}*/

	//生成に失敗したら1を返して終了
	if ( wd.WindowHandle == NULL )
	{
		err = GetLastError();
		return err;
	}

	ShowWindow( wd.WindowHandle, nCmdShow );
	UpdateWindow( wd.WindowHandle );
	//  SetWindowText( wd.WindowParent, GetWindowName() );

	return 0;
}

// ウィンドウ設定のバックアップ。
int Window::BackUpWindow()
{
	backup = wd;
	return 0;
}

// ウィンドウ設定の復帰。
int Window::RepairWindow()
{
	wd = backup;
	return SetWindowPos(
		GetWindowHandle(), wd.winpos,
		GetPositionX(), GetPositionY(),
		GetWindowWidth(), GetWindowHeight(),
		wd.flags
		);
}

/*!
@brief ウィンドウの設定を変更する
@par   関数説明
ウィンドウスタイルを設定する。
@param Type WT_で始まる定数。
@return false=設定反映かSetWindowPosでの設定失敗。
@return true=SetWindowPosでの設定成功。
*/
BOOL Window::SetWindow( int type, int display )
{
	HWND myhwnd = NULL;//ウィンドウハンドル
	RECT myrect;//RECT構造体
	BOOL ret;

	// ディスプレイ番号から領域を割り出して移動してからサイズ変更とかを行うように書き換え

	switch ( type )
	{
	case WT_DEFAULT://サイズ固定のウィンドウ
		//ウィンドウスタイルの設定
		SetWindowStyle( WT_DEFAULT );
		//ウィンドウスタイルの変更
		SetWindowLong( GetWindowHandle(), GWL_STYLE, GetWindowStyle() );
		wd.flags = SWP_SHOWWINDOW;
		wd.winpos = HWND_TOP;
		//ウィンドウのサイズや位置を変更
		ret = SetWindowPos(
			GetWindowHandle(), wd.winpos,
			GetPositionX(), GetPositionY(),//CW_USEDEFAULT, CW_USEDEFAULT,
			GetWindowWidth(), GetWindowHeight(),
			wd.flags
			);
		SetPositionX( GetPositionX( 1 ) );
		SetPositionY( GetPositionY( 1 ) );
		return ret;
	case WT_NORMAL://通常のリサイズ可能ウィンドウ
		//ウィンドウスタイルの設定
		SetWindowStyle( WT_NORMAL );
		//ウィンドウスタイルの変更
		SetWindowLong( GetWindowHandle(), GWL_STYLE, GetWindowStyle() );
		wd.flags = SWP_SHOWWINDOW;
		//ウィンドウのサイズや位置を変更
		wd.winpos = HWND_TOP;
		ret = SetWindowPos(
			GetWindowHandle(), wd.winpos,
			GetPositionX(), GetPositionY(),//CW_USEDEFAULT, CW_USEDEFAULT,
			GetWindowWidth(), GetWindowHeight(),
			wd.flags
			);
		SetPositionX( GetPositionX( 1 ) );
		SetPositionY( GetPositionY( 1 ) );
		return ret;
	case WT_NOFRAME:
	case WT_FULLSCREEN:
		//ウィンドウスタイルの設定
		SetWindowStyle( WT_NOFRAME );

		//ウィンドウスタイルの変更
		SetWindowLong( GetWindowHandle(), GWL_STYLE, GetWindowStyle() );
		wd.flags = SWP_SHOWWINDOW;
		wd.winpos = HWND_TOP;
		//ウィンドウのサイズや位置を変更
		return SetWindowPos(
			GetWindowHandle(), wd.winpos,
			GetPositionX(), GetPositionY(),
			GetWindowWidth(), GetWindowHeight(),
			wd.flags
			);
	case WT_NORESIZEFULLSCREEN://画面設定を変えないフルスクリーン
		//プログラムを起動したデスクトップサイズのウィンドウで
		//描写優先度が一番上のウィンドウを生成する

		//ウィンドウスタイルの設定
		SetWindowStyle( WT_NORESIZEFULLSCREEN );

		// 対象ディスプレイのサイズなどを取得。
		if ( GetDisplayRect( display, &myrect ) == 0 )
		{
			// 成功したので、うまい具合に設定。
			// 位置はディスプレイの左上。
			SetPositionXY( myrect.left, myrect.top );
			// サイズはディスプレイに合わせる。
			SetWindowSize( myrect.right - myrect.left, myrect.bottom - myrect.top );
		} else
		{
			// 失敗したので、デスクトップから取得。

			//デスクトップのウィンドウハンドル取得
			myhwnd = GetDesktopWindow();
			//デスクトップのサイズなどを取得
			if ( myhwnd && GetClientRect( myhwnd, &myrect ) )
			{
				//位置は原点に設定
				SetPositionXY( 0, 0 );
				//サイズはデスクトップと同じサイズに設定
				SetWindowSize( myrect.right, myrect.bottom );
			}
		}

		//ウィンドウスタイルの変更
		SetWindowLong( GetWindowHandle(), GWL_STYLE, GetWindowStyle() );
		wd.flags = SWP_SHOWWINDOW;
		wd.winpos = HWND_TOPMOST;
		//ウィンドウのサイズや位置を変更
		return SetWindowPos(
			GetWindowHandle(), wd.winpos,
			GetPositionX(), GetPositionY(),
			GetWindowWidth(), GetWindowHeight(),
			wd.flags
			);

	default:
		//後で考える
		//リサイズ程度
		MoveWindow(
			GetWindowHandle(),
			GetPositionX(), GetPositionY(),
			GetWindowWidth(), GetWindowHeight(),
			TRUE
			);
		break;
	}
	return 0;
}

/*!
@brief 拡張ウィンドウクラス名の取得
@par   関数説明
拡張ウィンドウクラス名を返す。
@return 拡張ウィンドウクラス名(char*型)。
*/
const char * Window::GetClassNameEx( void )
{
	//ウィンドウの設定がウィンドウクラス。
	//それへの個別の名前がこのウィンドウクラスの名前となる。
	return CLASSNAME;//wd.ClassName;//wd.ClassName;
}

/*!
@brief ウィンドウクラスの名前の設定
@par   関数説明
ウィンドウクラス名の設定を行う。設定反映はSetWindow()関数を使う。
@param LPCTSTR name TCHAR型文字配列のウィンドウクラス名
*/
/*int Window::SetClassNameEx( char * name )
{
wd.ClassName = name;
return 0;
}*/

/*!
@brief ウィンドウの名前の取得
@par   関数説明
ウィンドウの名前を返す。
@return 現在のウィンドウ名(char*型)。
*/
char * Window::GetWindowName( void )
{
	//charで返すのはMikanに任せる
	return wd.WindowName;
}

int Window::SetWindowIcon( int smalliconnum, int bigiconnum )
{
	if ( wd.wcx.hIcon == NULL )
	{
		wd.wcx.hIcon = LoadIcon( NULL, MAKEINTRESOURCE( bigiconnum ) );
		wd.wcx.hIconSm = LoadIcon( NULL, MAKEINTRESOURCE( smalliconnum ) );
	} else
	{
		wd.wcx.hIcon = LoadIcon( GetInstanceHandle(), MAKEINTRESOURCE( bigiconnum ) );
		wd.wcx.hIconSm = LoadIcon( GetInstanceHandle(), MAKEINTRESOURCE( smalliconnum ) );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_BIG, (WPARAM)( wd.wcx.hIcon ) );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_SMALL, (WPARAM)( wd.wcx.hIconSm ) );
	}
	return 0;
}

int Window::SetWindowIcon( char *smalliconname, char *bigiconname )
{
	if ( wd.wcx.hIcon == NULL )
	{
		wd.wcx.hIcon = LoadIcon( NULL, bigiconname );
		wd.wcx.hIconSm = LoadIcon( NULL, smalliconname );
	} else
	{
		wd.wcx.hIcon = LoadIcon( GetInstanceHandle(), bigiconname );
		wd.wcx.hIconSm = LoadIcon( GetInstanceHandle(), smalliconname );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_BIG, (WPARAM)( wd.wcx.hIcon ) );
		SendMessage( GetWindowHandle(), WM_SETICON, ICON_SMALL, (WPARAM)( wd.wcx.hIconSm ) );
	}
	return 0;
}

/*!
@brief ウィンドウの名前の設定
@par   関数説明
ウィンドウの名前を設定する。反設定映はSetWindow()関数を使う。
@param name TCHAR型文字配列のウィンドウクラス名
@return 0=成功。
@return 1=与えた文字列へのポインタがNULL。
*/
int Window::SetWindowName( char * name )
{
	int length;
	if ( name )
	{
		length = strlen( name );
		if ( length < WBUFLEN )
		{
			strcpy_s( wd.WindowName, WBUFLEN, name );
			if ( GetWindowHandle() )
			{
				SetWindowText( GetWindowHandle(), wd.WindowName );
			}
			return 0;
		}
	}
	return 1;
}

/*!
@brief ウィンドウスタイルの取得
@par   関数説明
ウィンドウスタイルを取得する。
@return 現在のウィンドウスタイル。
*/
unsigned long Window::GetWindowStyle( void )
{
	return wd.WindowStyle;
}

/*!
@brief ウィンドウスタイルの設定
@par   関数説明
ウィンドウクスタイルの設定を行う。
ウィンドウスタイルを直設定したい場合はSetWindowStyleDirect()関数を使う。
設定反映はSetWindow()関数を使う。
@param wt WT_から始まる定数。
@return NULL=wtが不正。もしくはSetWindowStyleDirectの返り値。
@return その他=SetWindowStyleDirectの返り値。
*/
unsigned long Window::SetWindowStyle( enum WINDOWTYPE wt )
{
	fullscreen = 0;
	//WINDOWTYPEで分岐
	switch ( wt )
	{
	case WT_DEFAULT://リサイズ不可ウィンドウ
		return SetWindowStyleDirect( WS_DEFAULT );
	case WT_NORMAL://リサイズ可能ウィンドウ
		return SetWindowStyleDirect( WS_NORMAL );
	case WT_NORESIZEFULLSCREEN://画面設定を変更しないフルスクリーン
		return SetWindowStyleDirect( WS_NORESIZEFULLSCREEN );
	case WT_NOFRAME:
		return SetWindowStyleDirect( WS_NOFRAME );
	case WT_FULLSCREEN:
		fullscreen = 1;
		return SetWindowStyleDirect( WS_FULLSCREEN );
	}
	return NULL;
}

/*!
@brief ウィンドウスタイルの直設定
@par   関数説明
ウィンドウクスタイルの設定を行う。
設定反映はSetWindow()関数を使う。
@param ws WS_から始まる定数の組み合わせ。詳しくはMSDN等で。
@return 設定したウィンドウスタイル。
*/
unsigned long Window::SetWindowStyleDirect( unsigned long ws )
{
	wd.WindowStyle = ws;
	return ws;
}

/*!
@brief 拡張ウィンドウスタイルの取得
@par   関数説明
拡張ウィンドウクスタイルの設定を取得する。
@return 拡張ウィンドウスタイル。
*/
unsigned long Window::GetWindowStyleEx( void )
{
	return wd.WindowStyleEx;
}

/*!
@brief 拡張ウィンドウスタイルの直設定
@par   関数説明
拡張ウィンドウクスタイルの設定を行う。
設定反映はSetWindow()関数を使う。
@param unsigned long ws 拡張ウィンドウスタイルの定数の組み合わせ。
@return 設定した拡張ウィンドウスタイル。
*/
unsigned long Window::SetWindowStyleExDirect( unsigned long wse )
{
	wd.WindowStyleEx = wse;
	return wse;
}

/*!
@brief ウィンドウの横幅を取得
@par   関数説明
ウィンドウの横幅を取得する。
@param int type 初期値=0。0で設定値を返し、1で現在値を調べて返す。
@return ウィンドウの横幅。
*/
unsigned int Window::GetWindowWidth( int type )
{
	//type=0:設定値,1=現在値
	RECT myrect;//RECT構造体

	if ( type )
	{
		GetClientRect( GetWindowHandle(), &myrect );
		return myrect.right - myrect.left;
	}

	return wd.Width;
}

/*!
@brief ウィンドウの高さを取得
@par   関数説明
ウィンドウの高さを取得する。
@param int type 初期値=0。0で設定値を返し、1で現在値を調べて返す。
@return ウィンドウの高さ
*/
unsigned int Window::GetWindowHeight( int type )
{
	//type=0:設定値,1=現在値
	RECT myrect;//RECT構造体

	if ( type )
	{
		GetClientRect( GetWindowHandle(), &myrect );
		return myrect.bottom - myrect.top;
	}

	return wd.Height;
}

/*!
@brief ウィンドウの横幅設定
@par   関数説明
ウィンドウの横幅を設定する。設定反映はSetWindow()関数を使う。
@param unsigned int width 横幅を与える。極端な値だと別の値になる。
@return 設定した横幅。
*/
unsigned int Window::SetWindowWidth( unsigned int width )
{
	int w = 0;
	//unsignedなので引数が0なら値を書き変える
	if ( width <= 0 )
	{
		width = WF_WIDTH;
	}
	wd.Width = width;
	/*
	if( wd.WindowStyle & WS_BORDER )
	{
	w += GetSystemMetrics(SM_CXBORDER);
	}

	//if( wd.WindowStyle & WS_BORDER )
	//{
	w += GetSystemMetrics(SM_CXEDGE);
	//}
	*/

	//  wd.Width = width + w * 2;// + GetSystemMetrics(SM_CXEDGE) + GetSystemMetrics(SM_CXBORDER) + GetSystemMetrics(SM_CXDLGFRAME);// + GetSystemMetrics(SM_CXFIXEDFRAME);
	//実際に代入した値を返す
	return width;
}

/*!
@brief ウィンドウの高さ設定
@par   関数説明
ウィンドウの高さを設定する。設定反映はSetWindow()関数を使う。
@param unsigned int height 高さを与える。極端な値だと別の値になる。
@return 設定した高さ。
*/
unsigned int Window::SetWindowHeight( unsigned int height )
{
	//unsignedなので引数が0なら値を書き変える
	if ( height <= 0 )
	{
		height = WF_HEIGHT;
	}
	wd.Height = height;
	return height;
}

/*!
@brief ウィンドウのサイズ設定
@par   関数説明
ウィンドウのサイズを設定する。設定反映はSetWindow()関数を使う。
@param unsigned int width 横幅を与える。
@param unsigned int height 高さを与える。
@return 0=設定値が両方引数と異なる
@return 1=横幅は設定値が異なるが高さの設定値は引数通り
@return 2=横幅は設定値が引数通りだが高さの設定値は異なる
@return 3=設定値が両方引数と同じ
*/
int Window::SetWindowSize( unsigned int width, unsigned int height )
{
	return ( SetWindowWidth( width ) == width ) * 2 +
		( SetWindowHeight( height ) == height );
}

/*!
@brief 描画領域からウィンドウのサイズ設定
@par   関数説明
ウィンドウのサイズを描画領域を基準にして設定する。設定反映はSetWindow()関数を使う。
@param unsigned int width 横幅を与える。
@param unsigned int height 高さを与える。
@return 0=設定値が両方引数と異なる
@return 1=横幅は設定値が異なるが高さの設定値は引数通り
@return 2=横幅は設定値が引数通りだが高さの設定値は異なる
@return 3=設定値が両方引数と同じ
*/
int Window::SetScreenSize( unsigned int width, unsigned int height )
{
	RECT rect;
	rect.left = 0;
	rect.right = width;
	rect.top = 0;
	rect.bottom = height;

	AdjustWindowRectEx( &rect, GetWindowStyle(), false, GetWindowStyleEx() );
	return SetWindowSize( rect.right - rect.left, rect.bottom - rect.top );
	/*  return  SetWindowSize(
		width  + GetSystemMetrics(SM_CXEDGE) + GetSystemMetrics(SM_CXBORDER) + GetSystemMetrics(SM_CXDLGFRAME),
		height + GetSystemMetrics(SM_CYEDGE) + GetSystemMetrics(SM_CYBORDER) + GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION));*/
}

/*!
@brief 現在のウィンドウのX座標を取得
@par   関数説明
ウィンドウのX座標を取得する。
@param int type 初期値=0。0で設定値を返し、1で現在値を調べて返す。
@return 左からの位置。
*/
int Window::GetPositionX( int type )
{
	//type=0:設定値,1=現在値
	RECT myrect;//RECT構造体

	if ( type )
	{
		GetClientRect( GetWindowHandle(), &myrect );
		return myrect.left;
	}

	return wd.PositionX;
}

/*!
@brief 現在のウィンドウのY座標を取得
@par   関数説明
ウィンドウのY座標を取得する。
@param int type 初期値=0。0で設定値を返し、1で現在値を調べて返す。
@return 上からの位置。
*/
int Window::GetPositionY( int type )
{
	//type=0:設定値,1=現在値
	RECT myrect;//RECT構造体

	if ( type )
	{
		GetClientRect( GetWindowHandle(), &myrect );
		return myrect.top;
	}

	return wd.PositionY;
}

/*!
@brief ウィンドウのX座標設定
@par   関数説明
ウィンドウのX座標を設定する。設定反映はSetWindow()関数を使う。
@param unsigned int x X座標を与える。
@return 設定した値。
*/
int Window::SetPositionX( int x )
{
	wd.PositionX = x;
	//実際に代入した値を返す
	return x;
}

/*!
@brief ウィンドウのY座標設定
@par   関数説明
ウィンドウのY座標を設定する。設定反映はSetWindow()関数を使う。
@param unsigned int y Y座標を与える。
@return 設定した値。
*/
int Window::SetPositionY( int y )
{
	wd.PositionY = y;
	return y;
}

/*!
@brief 設定ウィンドウの座標設定
@par   関数説明
ウィンドウの座標を設定する。設定反映はSetWindow()関数を使う。
@param unsigned int x X座標を与える。
@param unsigned int y Y座標を与える。
@return 0を返す。
*/
int Window::SetPositionXY( int x, int y )
{
	SetPositionX( x );
	SetPositionY( y );
	return 0;
}

//ディスプレイの数を返す
int Window::GetDisplayMax( void )
{
	return GetSystemMetrics( SM_CMONITORS );
}

// ディスプレイの番号を返す
int Window::GetDisplayNumber( HWND window )
{
	int max, i, ret = 0, area[ 2 ] = { 0, 0 };
	RECT win, disp;

	if ( window == NULL )
	{
		return 0;
	}

	// ウィンドウの位置とサイズをRECTで取得
	GetWindowRect( window, &win );

	// ディスプレイの数
	max = GetDisplayMax();

	for ( i = 0; i < max; ++i )
	{
		GetDisplayRect( i, &disp );
		// そもそも当たってない。
		if ( win.right < disp.left ||
			disp.right < win.left ||
			win.bottom < disp.top ||
			disp.bottom < win.top )
		{
			continue;
		}

		// ウィンドウはディスプレイの中 or 一部分が重なっている。
		// ディスプレイのRECTをウィンドウと重なっている部分の大きさに置換。
		if ( disp.left < win.left )
		{
			disp.left = win.left;
		}
		if ( win.right < disp.right )
		{
			disp.right = win.right;
		}
		if ( win.top < disp.top )
		{
			disp.top = win.top;
		}
		if ( win.bottom < disp.bottom )
		{
			disp.bottom = win.bottom;
		}

		// RECTの面積を計算。
		area[ 1 ] = ( disp.right - disp.left ) * ( disp.bottom - disp.top );
		if ( area[ 0 ] < area[ 1 ] )
		{
			area[ 0 ] = area[ 1 ];
			ret = i;
		}
	}

	return ret;
}

// ディスプレイのサイズを返す
int Window::GetDisplaySize( long displaynum, int *width, int *height )
{
	return GetDisplayRect( displaynum, NULL, NULL, width, height );
}

// ディスプレイの位置とサイズを返す
int Window::GetDisplayRect( long displaynum, int *x, int *y, int *width, int *height )
{
	DISPLAY_DEVICE disdev;
	DEVMODE devmode;

	if ( x )
	{
		*x = 0;
	}
	if ( y )
	{
		*y = 0;
	}
	if ( width )
	{
		*width = 0;
	}
	if ( height )
	{
		*height = 0;
	}

	memset( &disdev, 0, sizeof( DISPLAY_DEVICE ) );
	disdev.cb = sizeof( DISPLAY_DEVICE );
	if ( EnumDisplayDevices( NULL, displaynum, &disdev, 0 ) == 0 )
	{
		// 不正なディスプレイ番号
		return 1;
	}

	memset( &devmode, 0, sizeof( DEVMODE ) );
	devmode.dmSize = sizeof( DEVMODE );
	if ( EnumDisplaySettingsEx( disdev.DeviceName, /*ENUM_CURRENT_SETTINGS*/ENUM_REGISTRY_SETTINGS, &devmode, EDS_RAWMODE ) == 0 )
	{
		// 設定を取得できなかった
		return 2;
	}
	if ( x )
	{
		*x = devmode.dmPosition.x;
	}
	if ( y )
	{
		*y = devmode.dmPosition.y;
	}
	if ( width )
	{
		*width = devmode.dmPelsWidth;
	}
	if ( height )
	{
		*height = devmode.dmPelsHeight;
	}

	return 0;
}

int Window::GetDisplayRect( long displaynum, RECT *rect )
{
	int ret, x, y, w, h;
	if ( rect == NULL )
	{
		return 3;
	}
	if ( ( ret = GetDisplayRect( displaynum, &x, &y, &w, &h ) ) )
	{
		return ret;
	}
	rect->left = x;
	rect->top = y;
	rect->right = x + w;
	rect->bottom = y + h;
	return 0;
}

int Window::GetDesktopWidth( void )
{
	int w;
	Window::GetDesktopSize( &w, NULL );
	return w;
}

int Window::GetDesktopHeight( void )
{
	int h;
	Window::GetDesktopSize( NULL, &h );
	return h;
}

int Window::GetDesktopSize( int *width, int *height )
{
	HWND myhwnd; //ウィンドウハンドル
	RECT myrect; //RECT構造体

	//デスクトップのウィンドウハンドル取得
	myhwnd = GetDesktopWindow();

	//デスクトップのサイズなどを取得
	if ( myhwnd && GetClientRect( myhwnd, &myrect ) )
	{
		if ( width )
		{
			*width = myrect.right;
		}
		if ( height )
		{
			*height = myrect.bottom;
		}
	}
	return 0;
}

/*!
@brief ウィンドウハンドルを返す
@par   関数説明
ウィンドウハンドルを取得する。
@return 現在のウィンドウハンドル。
*/
HWND Window::GetWindowHandle( void )
{
	return wd.WindowHandle;
}

/*!
@brief ウィンドウハンドルの設定
@par   関数説明
ウィンドウハンドルを設定する。
@param HWND wh ウィンドウハンドル
@return 設定したウィンドウハンドル。
*/
HWND Window::SetWindowHandle( HWND wh )
{
	wd.WindowHandle = wh;
	return wd.WindowHandle;
}

HWND Window::GetParentWindowHandle( void )
{
	return wd.WindowParent;
}

/*!
@brief ウィンドウハンドルの設定
@par   関数説明
ウィンドウハンドルを設定する。
@param HWND wh ウィンドウハンドル
@return 設定したウィンドウハンドル。
*/
HWND Window::SetParentWindowHandle( HWND wh )
{
	wd.WindowParent = wh;
	return wd.WindowParent;
}

/*!
@brief ウィンドウクラスを返す
@par   関数説明
ウィンドウクラスを取得する。
@return ウィンドウクラス。
*/
WNDCLASSEX Window::GetWindowClassEx( void )
{
	return wd.wcx;
}

/*!
@brief ウィンドウクラスの設定
@par   関数説明
ウィンドウクラスを設定する。
@param WNDCLASS wc ウィンドウクラス
@return 設定したウィンドウクラス。
*/
WNDCLASSEX Window::SetWindowClassEx( WNDCLASSEX wcx )
{
	wd.wcx = wcx;
	return wd.wcx;
}

/*!
@brief メニューハンドルの取得
@par   関数説明
メニューハンドルを取得する。
@return メニューハンドル。
*/
HMENU Window::GetMenuHandle( void )
{
	return wd.hMenu;
}

/*!
@brief インスタンスハンドルの取得
@par   関数説明
インスタンスハンドルを取得する。
@return インスタンスハンドル。
*/
HINSTANCE Window::GetInstanceHandle( void )
{
	return wd.hInstance;
}

// ウィンドウプロシージャの設定
int Window::SetWindowProc( WNDPROC winproc )
{
	wd.wcx.lpfnWndProc = winproc;
	return 0;
}

/*!
@brief パラメーターの取得
@par   関数説明
パラメーターを取得する。
@return パラメーター。
*/
LPVOID Window::GetParameter( void )
{
	return wd.lpParam;
}
int Window::SetReceiveDragFileFunction( int( *func )( int files ) )
{
	ReceiveDragFiles_ = func;
	return 0;
}
char * Window::GetReceiveDragFilePath( int num )
{
	HDROP hdrop = drophandle;
	if ( hdrop && num < dragfilesmax )
	{
		DragQueryFile( hdrop, num, dragfilepath, MAX_PATH );
		return dragfilepath;
	}
	return NULL;
}

int Window::ReceiveDragFiles( HDROP hdrop )
{
	drophandle = hdrop;
	dragfilesmax = 0;
	if ( ReceiveDragFiles_ )
	{
		dragfilesmax = DragQueryFile( hdrop, -1, NULL, 0 );
		ReceiveDragFiles_( dragfilesmax );
	}
	DragFinish( hdrop );
	drophandle = NULL;
	dragfilesmax = 0;
	return 0;
}
