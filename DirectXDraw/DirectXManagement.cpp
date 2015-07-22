#include "DirectXDraw.h"

/*!
@brief コンストラクタ(非推奨)
@par   関数説明
変数を初期化する。
*/
DirectXDraw9::DirectXDraw9( void )
{
	Init();
}

/*!
@brief コンストラクタ(通常)
@par   関数説明
変数を初期化する。
*/
DirectXDraw9::DirectXDraw9( HWND wh, unsigned int tmax, unsigned int pmax, unsigned int fmax )
{
	Init( wh, tmax, pmax, fmax );
}

/*!
@brief コンストラクタ本体
@par   関数説明
変数を初期化する。
*/
int DirectXDraw9::Init( HWND wh, unsigned int tmax, unsigned int pmax, unsigned int fmax )
{
	unsigned int c;

	D3D = NULL;

	//バックバッファ
	backbuffer = NULL;
	backbufferz = NULL;
	line = NULL;
	//テクスチャの最大数
	SetTextureMax( tmax );
	//多角形の最大数
	SetPolygonMax( pmax );

	lastpvnum = 0;
	//テクスチャのポインタ配列をNULLにする
	dxtex = NULL;
	//多角形のポインタ配列をNULLにする。
	pv = NULL;
	rpv = NULL;
	rpvmax = 0;
	//ウィンドウハンドルの設定
	SetWindowHandle( wh );

	// 画面サイズを設定。
	// 0 だとウィンドウサイズになる。
	SetScreenSize( 0, 0 );

	//レンダーターゲットは画面。
	target[ 0 ] = 1;
	target[ 1 ] = 0;
	//テクスチャのバックアップを行う。
	SetAutoBackUpTexture( 0 );

	//直線の初期化
	line = NULL;

	fontmax = fmax;
	fontdata = ( struct FONTDATA * )calloc( fontmax, sizeof( struct FONTDATA ) );
	//フォントの初期化
	for ( c = 0; c < fontmax; ++c )
	{
		fontdata[ c ].D3DFont = NULL;
		fontdata[ c ].use = 0;
	}

	//垂直同期フラグ
	SetVsyncWait();

	swapchainnum = 0;
	swapchainmax = 1;
	/*  swapchain = (IDirect3DSwapChain9 **)calloc( sizeof(IDirect3DSwapChain9 *), swapchainmax );
	  for( c = 0 ; c < swapchainmax ; ++c )
	  {
	  swapchain[ c ] = NULL;
	  }*/
	SetDrawFunction();

	fullscreen = 0;

	// 描画中かどうか。
	drawing = 0;
	lock = 0;
	locksleeptime = 1;
	ismultithread = 0;

	D3D = Direct3DCreate9( D3D_SDK_VERSION );

	return 0;
}

/*!
@brief デストラクタ
@par   関数説明
何もしていない。
*/
DirectXDraw9::~DirectXDraw9( void )
{
	//  CleanupD3DObject();
	//  CleanupD3D();
}

int DirectXDraw9::SetFullScreenMode( int fullscreen_on )
{
	fullscreen = fullscreen_on;
	return 0;
}

/*!
@brief ウィンドウハンドルの設定
@par   関数説明
DirectXと関連付けるウィンドウハンドルを設定する。
@param wh ウィンドウハンドル。
@return 設定したウィンドウハンドルを返す。
*/
HWND DirectXDraw9::SetWindowHandle( HWND wh )
{
	return ( WindowHandle = wh );
}

/*!
@brief ウィンドウハンドルの取得
@par   関数説明
DirectXと関連付けられたウィンドウハンドルを取得する。
@return ウィンドウハンドル。
*/
HWND DirectXDraw9::GetWindowHandle( void )
{
	return WindowHandle;
}

/*!
@brief 垂直同期待ちの設定
@par   関数説明
垂直同期を待つかどうかの設定。InitD3Dより前に実行する必要がある。
@param vs 0で垂直同期を待たない。それ以外で垂直同期を待つ。
@return 設定した値を返す。
*/
int DirectXDraw9::SetVsyncWait( int vs )
{
	return ( vsync = vs );
}

//テクスチャの最大数の設定
unsigned int DirectXDraw9::SetTextureMax( unsigned int tmax )
{
	//if( dxtex == NULL )
	//{
	return ( texmax = tmax );
	//}
	//テクスチャ用の配列とかが出来ている場合には、初期化なり何なりの作業が必要。
	//とりあえず面倒なので実装は見送るが、どうするかな。
	//return 0;
}

unsigned int DirectXDraw9::GetTextureMax( void )
{
	return texmax;
}

int DirectXDraw9::IsEmptyTexture( unsigned int txnum )
{
	if ( txnum >= texmax )//if( txnum >= texmax || dxtex[ txnum ].ext == 0 )
	{
		//テクスチャ番号が不正
		return 2;
	}

	if ( dxtex[ txnum ].ext )
	{
		// テクスチャ使用中
		return 1;
	}
	// テクスチャ未使用
	return 0;
}

// 多角形頂点の最大数の設定
unsigned int DirectXDraw9::SetPolygonMax( unsigned int pmax )
{
	return ( pvmax = pmax );
}

// 描画サイズの設定。
int DirectXDraw9::SetScreenSize( int width, int height )
{
	swidth = width;
	sheight = height;

	if ( D3D )
	{
		//InitD3D( 0 );
		RecoverFromDeviceLost( 1 );
	}

	return 0;
}

int DirectXDraw9::GetScreenWidth( int now )
{
	return now ? D3DPP.BackBufferWidth : swidth;
}
int DirectXDraw9::GetScreenHeight( int now )
{
	return now ? D3DPP.BackBufferHeight : sheight;
}

/*!
@brief DirectXの初期化
@par   関数説明
DirectXの初期化を行う。
@return 0=成功。
@return 1=ウィンドウハンドルが見つからない。
@return 2=初期化失敗。
@return 3=IDirect3Device9の初期化に失敗。
*/
int DirectXDraw9::InitD3D( int create )
{
	//  unsigned int i;
	D3DDEVTYPE D3DDevType;
	//  D3DDISPLAYMODE d3ddm;
	//  tagRECT rect;

	if ( create )
	{
		if ( GetWindowHandle() == NULL )
		{
			//ウィンドウハンドルが見つからない
			return 1;
		}
	}

	//  D3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

	//D3PPの中身を0で埋める。
	ZeroMemory( &D3DPP, sizeof( D3DPP ) );
	//ウィンドウモードの指定
	D3DPP.Windowed = TRUE;

	if ( vsync )
	{
		D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	} else
	{
		D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	//    D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	if ( fullscreen )
	{
		// フルスクリーンモード
		D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
		D3DPP.BackBufferCount = 1;
		//D3DPP.SwapEffect = D3DSWAPEFFECT_COPY;
		//D3DPP.BackBufferFormat = d3ddm.Format;
		//D3DPP.EnableAutoDepthStencil = TRUE;
		//D3DPP.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		D3DPP.Windowed = FALSE;
		D3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;
		if ( GetScreenWidth() == 0 )
		{
			SetScreenSize( 640, 480 );
		}
	} else
	{
		// ウィンドウモード
		if ( vsync )
		{
			//スワップエフェクト
			D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
		} else
		{
			//スワップエフェクト
			D3DPP.SwapEffect = D3DSWAPEFFECT_COPY;
		}

		//バックバッファのフォーマット
		D3DPP.BackBufferCount = 1;//D3DFMT_UNKNOWN;;//1;//2//D3DFMT_UNKNOWN;

	}


	// ここでバックバッファのサイズ設定を行う。
	// 本来はアスペクト比維持の場合、ウィンドウサイズと比較してからバックバッファサイズを決める。
	// ついでに、描画をずらして描画が真ん中に来るようにする。

	D3DPP.BackBufferWidth = GetScreenWidth();
	D3DPP.BackBufferHeight = GetScreenHeight();
	// マルチサンプル時にぼやけないようにする。
	D3DPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	//  D3DPP.BackBufferFormat = d3ddm.Format;
	//  GetClientRect( GetWindowHandle(), &rect );
	//  D3DPP.BackBufferWidth = rect.right;
	//  D3DPP.BackBufferHeight = rect.bottom;
	//  D3DPP.Windowed = TRUE;

	//  D3DPP.EnableAutoDepthStencil = TRUE;
	//  D3DPP.AutoDepthStencilFormat = D3DFMT_D16;
	//  D3DPP.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	//D3Dデバイスを作る。
	//  hr = D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
	//    GetWindowHandle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING,
	//    &D3DPP, &D3DDev );
	ismultithread = 1;
	if ( create )
	{
		hr = D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetWindowHandle(), D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DPP, &D3DDev );
		// T&L, HAL
		if ( FAILED( hr ) )
		{
			hr = D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetWindowHandle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DPP, &D3DDev );
			// not T&L, HAL
			if ( FAILED( hr ) )
			{
				hr = D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, GetWindowHandle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DPP, &D3DDev );
				// REF
				if ( FAILED( hr ) )
				{
					//デバイス生成すべて失敗
					ismultithread = 0;
					return 3;
				} else
				{
					D3DDevType = D3DDEVTYPE_REF;
				}
			} else
			{
				D3DDevType = D3DDEVTYPE_HAL;
			}
		} else
		{
			D3DDevType = D3DDEVTYPE_HAL;
		}


		if ( FAILED( hr ) )
		{
			//IDirect3Device9インターフェイスの取得に失敗。
			return 3;
		}

		// テクスチャへの描画ができるかのチェック
		hr = D3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDevType, D3DPP.BackBufferFormat, D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8 );
		if ( FAILED( hr ) )
		{
			candrawtexture = 0;
		} else
		{
			candrawtexture = 1;
		}
		// サーフェイスへの描画ができるかのチェック
		hr = D3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDevType, D3DPP.BackBufferFormat, D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE, D3DFMT_A8R8G8B8 );
		if ( FAILED( hr ) )
		{
			candrawsurface = 0;
		} else
		{
			candrawsurface = 1;
		}
	} else
	{
		hr = D3DDev->Reset( &D3DPP );

		if ( FAILED( hr ) )
		{
			if ( hr == D3DERR_DEVICELOST )
			{
				//デバイスロスト
				return 4;
			}
			//復元に失敗
			return 5;
		}
	}


	// スワップチェーンの作成
	/*  for( i = 0 ; i < swapchainmax ; ++i )
	  {
	  D3DDev->CreateAdditionalSwapChain( &D3DPP, &(swapchain[ i ]) );
	  }*/

	InitRender();

	if ( create )
	{
		FontInit();
	}

	//IDirect3DSurface9 *surface;
	//swapchain[ swapchainnum ]->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &surface);
	//D3DDev->SetRenderTarget( 0, surface );


	return 0;
}

/*!
@brief DirectXの解放
@par   関数説明
DirectXのデバイスやオブジェクトの解放を行う。
@return 0を返す。
*/
int DirectXDraw9::CleanupD3D( void )
{
	//Direct3Dオブジェクトの解放
	DX_RELEASE( D3D );

	FontCleanUp();

	//デバイスの解放
	DX_RELEASE( D3DDev );

	return 0;
}

/*!
@brief DirectXのオブジェクトの初期化
@par   関数説明
DirectXのテクスチャ等の初期化を行う。
デバイスロスト後に実行すると、テクスチャの再生成を行う。
@return 0を返す。
*/
int DirectXDraw9::InitD3DObject( void )
{
	unsigned int t;

	//バックバッファの確保
	D3DDev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer );
	// Zバッファの取得
	D3DDev->GetDepthStencilSurface( &backbufferz );

	//領域の確保
	if ( dxtex == NULL )
	{
		dxtex = ( struct MYTEXTURE * )calloc( texmax, sizeof( struct MYTEXTURE ) );
	}
	if ( pv == NULL )
	{
		pv = ( struct POLYGON_VERTEX * )calloc( pvmax + 1, sizeof( struct POLYGON_VERTEX ) );
		pv[ pvmax ].type = 2;
	}

	for ( t = 0; t < texmax; ++t )
	{
		//    dxtex[ t ].tex = ( IDirect3DTexture9 * )calloc( 1, sizeof( IDirect3DTexture9 ) );

		//ファイル名がある場合にはテクスチャを作る。
		//バックアップがある場合はスレッドフラグを立てずに作る。
		if ( dxtex[ t ].ext )
		{
			if ( dxtex[ t ].filename )
			{
				if ( dxtex[ t ].module )
				{
					CreateTexture( t, dxtex[ t ].module, dxtex[ t ].filename, dxtex[ t ].color );//, (dxtex[ t ].backup == NULL) );
				} else
				{
					CreateTexture( t, dxtex[ t ].filename, dxtex[ t ].color );//, (dxtex[ t ].backup == NULL) );
				}
				//dxtex[ t ].ext = 1;
			} else
			{
				//空のテクスチャ
				CreateTexture( t, dxtex[ t ].width, dxtex[ t ].height );//, (dxtex[ t ].backup == NULL) );
			}

			//テクスチャのバックアップに成功したので書き戻す。
			if ( dxtex[ t ].backup )
			{
				//テクスチャをサーフェイスに
				DX_RELEASE( dxtex[ t ].sur[ 0 ] );
				dxtex[ t ].tex->GetSurfaceLevel( 0, &( dxtex[ t ].sur[ 0 ] ) );
				//テクスチャにサーフェイスを書き戻す
				hr = D3DDev->UpdateSurface( dxtex[ t ].backup, NULL, dxtex[ t ].sur[ 0 ], NULL );
			}

		}//end texture exist check
	}//end texture loop

	return 0;
}

/*!
@brief DirectXのオブジェクトの解放
@par   関数説明
テクスチャの解放を行う。ただしこれで解放するとテクスチャの再生成などはできない。
@return 0を返す。
*/
int DirectXDraw9::CleanupD3DObject( void )
{
	unsigned int i;
	//バックバッファの解放
	DX_RELEASE( backbuffer );
	DX_RELEASE( backbufferz );
	DX_RELEASE( line );

	//テクスチャの解放
	if ( dxtex )
	{
		for ( i = 0; i < texmax; ++i )
		{
			if ( dxtex[ i ].ext )
			{
				ReleaseTexture( i );
			}
		}
		DX_FREE( dxtex );
	}

	//頂点の解放
	if ( pv )
	{
		for ( i = 0; i <= pvmax; ++i )
		{
			//      if( pv[ i ] )
			//      {
			ReleasePolygon( i );
			//      }
		}
		DX_FREE( pv );
	}

	if ( rpv )
	{
		free( rpv );
		rpv = NULL;
		rpvmax = 0;
	}

	/*  for( i = 0 ; i < swapchainmax ; ++i )
	  {
	  DX_RELEASE( swapchain[ i ] );
	  }
	  DX_FREE( swapchain );*/

	return 0;
}

int DirectXDraw9::SetAutoBackUpTexture( int enable )
{
	return ( texturebackup = enable );
}

/*!
@brief テクスチャのバックアップ
@par   関数説明
テクスチャをデバイスロスト時にも自動復帰するサーフェイスとして保存する。
サーフェイスを転送する作業を持ってテクスチャを復元することが可能になる。
@param txnum テクスチャ番号
@return 0 = バックアップに成功。
@return -1 = バックアップ許可がない。
@return 1 = テクスチャ番号が不正。
@return 2 = バックアップに失敗。
*/
int DirectXDraw9::BackUpTexture( unsigned int txnum, int force )
{
	//サーフェイス
	IDirect3DSurface9 *bsur;
	//サーフェイスのフォーマット
	D3DSURFACE_DESC desc;

	if ( texturebackup == 0 && force == 0 )
	{
		return -1;
	}

	if ( CannotUseTexture( txnum ) )//if( txnum >= texmax || dxtex[ txnum ].ext == 0 )
	{
		//テクスチャ番号が不正
		return 1;
	}

	//レンダーテクスチャのフォーマットを取得
	hr = dxtex[ txnum ].tex->GetLevelDesc( 0, &desc );

	//バックアップサーフェイス
	bsur = dxtex[ txnum ].backup;
	hr = D3DDev->CreateOffscreenPlainSurface(
		desc.Width, desc.Height, desc.Format,
		D3DPOOL_SYSTEMMEM, &( dxtex[ txnum ].backup ), NULL );

	if ( dxtex[ txnum ].sur == NULL )
	{
		dxtex[ txnum ].tex->GetSurfaceLevel( 0, &( dxtex[ txnum ].sur[ 0 ] ) );
	}

	if ( SUCCEEDED( hr ) )
	{
		hr = D3DDev->GetRenderTargetData( dxtex[ txnum ].sur[ 0 ], dxtex[ txnum ].backup );
		if ( SUCCEEDED( hr ) )
		{
			DX_RELEASE( bsur );
		}
		return 0;
	}

	dxtex[ txnum ].backup = bsur;

	return 2;
}

/*!
@brief デバイスロストからの復帰準備
@par   関数説明
DirectXのオブジェクトの解放などを行い、Resetするための準備を行う。
ここで解放されたテクスチャは後に再生成可能。
@return 0を返す。
*/
int DirectXDraw9::BackUp( void )
{
	unsigned int i;
	IDirect3DTexture9 *release;

	//DirectX側で管理していないリソースの解放
	//バックバッファの解放
	DX_RELEASE( backbuffer );
	DX_RELEASE( backbufferz );
	DX_RELEASE( line );
	if ( dxtex )
	{
		for ( i = 0; i < texmax; ++i )
		{
			if ( dxtex[ i ].ext && dxtex[ i ].pool != D3DPOOL_MANAGED )
			{
				release = dxtex[ i ].tex;
				DX_RELEASE( release );
				DX_RELEASE( dxtex[ i ].sur[ 0 ] );
				DX_RELEASE( dxtex[ i ].sur[ 1 ] );
				DX_RELEASE( dxtex[ i ].zsur );
				dxtex[ i ].tex = NULL;
			}
		}
	}
	/*  for( i = 0 ; i < swapchainmax ; ++i )
	  {
	  DX_RELEASE( swapchain[ i ] );
	  }*/

	FontCleanUp();
	return 0;
}

/*!
@brief デバイスロストからの復帰作業
@par   関数説明
テクスチャやフォントなどを再生成する。
@return 0を返す。
*/
int DirectXDraw9::Recover( void )
{
	InitD3DObject();
	FontRecover();
	return 0;
}

/*!
@brief デバイスロストからの復帰
@par   関数説明
デバイスロストになったとき、テクスチャの解放やReset、復帰などの作業をまとめて行う。
@return 0=成功。
@return 1=デバイスロスト中なのでまだ復帰作業が出来ない。
@return 2=まだResetできない。
@return 3=Resetしたがデバイスロストだった。
@return 4=Resetに失敗。
*/
int DirectXDraw9::RecoverFromDeviceLost( int force )
{
	if ( GetWindowHandle() == NULL )
	{
		return 0;
	}
	if ( force == 0 )
	{
		hr = D3DDev->TestCooperativeLevel();
		if ( hr == D3DERR_DEVICELOST )
		{
			//デバイスロスト中なのでもう少し待つ。
			return 1;
		} else if ( hr != D3DERR_DEVICENOTRESET )
		{
			//予期せぬエラー
			return 2;
		}
	}

	//デバイスロスト終了。復帰作業開始。

	//Direct3Dで管理してないリソースの解放
	//Reset前にリソースの解放をしておかないといけない。
	BackUp();

	/*
	//D3PPの中身を0で埋める。
	ZeroMemory( &D3DPP, sizeof( D3DPP ) );
	//ウィンドウモードの指定
	D3DPP.Windowed = TRUE;
	//スワップエフェクト
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//バックバッファのフォーマット
	D3DPP.BackBufferCount = D3DFMT_UNKNOWN;*/

	//復元を行う

	if ( InitD3D( 0 ) == 5 )
	{
		InitD3D( 1 );
	}

	//透過色などの再設定
	//InitRender();

	//解放したDirect3Dで管理していないリソースを再取得
	Recover();

	//デバイスロストからの復帰
	return 0;
}

int DirectXDraw9::SetDrawFunction( int mode )
{
	switch ( mode )
	{
	case DRAW_SWAPCHAIN:
		drawfunc = &DirectXDraw9::Draw;
		break;
	default:
		drawfunc = &DirectXDraw9::DrawNull;
		break;
	}
	return 0;
}

int DirectXDraw9::Draw( int num )
{
	//swapchain[ num ]->Present();

	return 0;
}

/*!
@brief 画面をきれいにする。
@par   関数説明
画面を単一色で塗りつぶす。
@param color 塗りつぶす色。省略で0xFF000000(黒)。
@return 0を返す。
*/
int DirectXDraw9::ClearScreen( unsigned long color )
{
	D3DDev->Clear( 0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0 );
	++countcs;
	return 0;
}

int DirectXDraw9::CountClearScreen( void )
{
	return countcs;
}

/*!
@brief 描画開始
@par   関数説明
DirectXを用いたテクスチャやフォントの描画の開始。
@return 0=成功。
@return 1=失敗。
*/
int DirectXDraw9::Begin( void )
{
	drawing_ = 0;
	//描画開始
	if ( SUCCEEDED( D3DDev->BeginScene() ) )
	{
		//成功
		countcs = 0;
		drawing = 1;
		return 0;
	}
	//失敗
	return 1;
}

/*!
@brief 描画終了
@par   関数説明
DirectXを落ちいたテクスチャやフォントの描画の終了。
@return 0=成功。
@return その他=描画後のPresentの返り値。
*/
int DirectXDraw9::End( void )
{
	drawing_ = 1;
	while ( lock )
	{
		Sleep( locksleeptime );
	}
	D3DDev->EndScene();//描画終了
	drawing = 0;
	//描画更新
	//  if( swapchainnum >= 0 && FAILED( (hr = swapchain[ swapchainnum ]->Present( NULL, NULL, NULL, NULL, 0 )) ) )
	//  {
	if ( FAILED( ( hr = D3DDev->Present( NULL, NULL, NULL, NULL ) ) ) )
	{
		return hr;
	}
	//  }

	return 0;
}

int DirectXDraw9::LockDrawEnd( unsigned long sleeptime )
{
	locksleeptime = sleeptime;
	lock = 1;
	return 0;
}

int DirectXDraw9::UnlockDarawEnd( void )
{
	lock = 0;
	return 0;
}

int DirectXDraw9::WaitDrawBegin( unsigned long sleeptime, unsigned int timeout, int autounlock )
{
	unsigned int before;

	before = timeGetTime();

	while ( drawing == 0 )
	{
		Sleep( sleeptime );
		if ( autounlock && lock )
		{
			lock = 0;
		}
		if ( timeGetTime() - before > timeout )
		{
			return 1;
		}
	}
	return 0;
}

int DirectXDraw9::WaitDrawEnd( unsigned long sleeptime, unsigned int timeout )
{
	unsigned int before;

	before = timeGetTime();

	while ( drawing_ == 0 )
	{
		Sleep( sleeptime );
		if ( timeGetTime() - before > timeout )
		{
			return 1;
		}
	}
	return 0;
}

MYTEXTURE * DirectXDraw9::GetTextureData( unsigned int txnum )
{
	if ( CannotUseTexture( txnum ) )//if( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return NULL;
	}
	return dxtex + txnum;
}

/*!
@brief テクスチャの準備
@par   関数説明
テクスチャの合成などの初期設定を行う。
@return SetRenderの返り値。
*/
int DirectXDraw9::InitRender( void )
{

	//描画オプション等
	//テクスチャブレンドで色を乗算合成
	D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	//色のパラメータ2はディフューズ値
	D3DDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	//色のパラメータ1はテクセル
	D3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	//テクスチャブレンドでアルファを乗算合成
	D3DDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	//アルファのパラメータ2はディフューズ値
	D3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	//アルファのパラメータ1はテクセル
	D3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	//Zバッファ関連の設定
	D3DDev->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS );
	D3DDev->SetRenderState( D3DRS_ZENABLE, TRUE );
	D3DDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

	//カリングモードなし
	D3DDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	//ライティングなし
	D3DDev->SetRenderState( D3DRS_LIGHTING, FALSE );

	// ドットをぼやけないようにする。
	D3DDev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	D3DDev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	D3DDev->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT );
	return SetRender( DRAW_TRC );
}

/*!
@brief テクスチャの合成方法の指定
@par   関数説明
テクスチャの合成方法を指定する。
@param kind 以下の値のどれかを指定する。
DRAW_TRC=透過色を有効にする。
DRAW_ADD=加算合成。
DRAW_SUB=減算合成。
DRAW_MUL=乗算合成。
DRAW_NON=何もしない。透過色が無効。
@return 0を返す。
*/
int DirectXDraw9::SetRender( int kind )
{
	switch ( kind )
	{
	case DRAW_TRC://透過色有効描画
		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		//アルファ・ブレンディングを行う
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		//ブレンディング係数( 1 - As, 1 - As, 1 - As, 1 - As )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		//ブレンディング係数( As, As, As, As )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		break;
	case DRAW_ADD://加算描画
		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		//アルファ・ブレンディングを行う
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		//ブレンディング係数( 1, 1, 1, 1 )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		//ブレンディング係数( As, As, As, As )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		break;
	case DRAW_SUB://減算描画
		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		//アルファ・ブレンディングを行う
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		//ブレンディング係数( 1 - Rs, 1 - Gs, 1 - Bs, 1 - As )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		//ブレンディング係数( 0, 0, 0, 0 )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
		break;
	case DRAW_MUL://乗算描画
		//    D3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//    D3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		//    D3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR); 
		//D3DRS_BLENDOPでの演算を有効にするらしい
		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		//ブレンディング係数( Rs, Gs, Bs, As )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
		//ブレンディング係数( 0, 0, 0, 0 )と( As, As, As, As )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );// | D3DBLEND_SRCALPHA );
		//転送元 + 転送先
		D3DDev->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
		break;
	case DRAW_MUL_WHITE: // 白背景合成乗算描画
		D3DDev->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF );

		D3DDev->SetTexture( 1, NULL );

		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		D3DDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		D3DDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		D3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

		// テクスチャステージ1で白テクスチャと合成
		D3DDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TFACTOR ); // 定数テクスチャを使用
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_BLENDCURRENTALPHA );
		D3DDev->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT );
		D3DDev->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );

		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

		// 乗算ブレンド
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
		D3DDev->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
		break;
	case DRAW_FILL:

		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		//アルファ・ブレンディングを行う
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		//ブレンディング係数( 1 - As, 1 - As, 1 - As, 1 - As )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		//ブレンディング係数( As, As, As, As )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		break;
	case DRAW_HALFADD:
		//a = 0;
		break;
	case DRAW_CADD_ASUB:
		// 色
		/*    //ブレンディング係数( 1, 1, 1, 1 )
			D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			//ブレンディング係数( As, As, As, As )
			D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );

			// アルファ値
			D3DDev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
			//アルファ・ブレンディングを行う
			//    D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			//ブレンディング係数( 1 - Rs, 1 - Gs, 1 - Bs, 1 - As )
			D3DDev->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCCOLOR );
			//ブレンディング係数( 0, 0, 0, 0 )
			D3DDev->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO );*/

		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
		D3DDev->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVSRCCOLOR );
		D3DDev->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA );
		//    D3DDev->SetRenderState(D3DRS_BLENDOPALPHA,D3DBLENDOP_ADD);

		break;
	case DRAW_BLUR:

		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
		//アルファ・ブレンディングを行う
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		//ブレンディング係数( 1 - Rs, 1 - Gs, 1 - Bs, 1 - As )
		D3DDev->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCCOLOR );
		//ブレンディング係数( 0, 0, 0, 0 )
		D3DDev->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO );

		D3DDev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
		//ブレンディング係数( 1, 1, 1, 1 )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		//ブレンディング係数( As, As, As, As )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );

		break;
	case DRAW_CUT:
		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );//D3DBLEND_SRCCOLOR );
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA );
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA );
		break;
	default://描画
		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		//アルファ・ブレンディングを行わない
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		//ブレンディング係数( 0, 0, 0, 0 )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
		//ブレンディング係数( 1, 1, 1, 1 )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
		break;
	}
	return 0;
}

/*!
@brief 画面をレンダーターゲットにする
@par   関数説明
描画対象を画面にする。
@return 0を返す。
*/
int DirectXDraw9::SetRenderTarget( void )
{
	D3DDev->SetRenderTarget( 0, backbuffer );
	D3DDev->SetDepthStencilSurface( backbuffer );
	if ( target[ 0 ] == 0 )
	{
		//前回はテクスチャに書き込んでいた。
		BackUpTexture( target[ 1 ] );
	}
	target[ 0 ] = 1;
	return 0;
}

/*!
@brief テクスチャをレンダーターゲットにする
@par   関数説明
描画対象をテクスチャにする。
@param txnum 描画対象にするテクスチャ番号。
@return 0を返す。
*/
int DirectXDraw9::SetRenderTarget( unsigned int txnum )
{

	if ( CannotUseTexture( txnum ) )//if( txnum >= texmax || dxtex[ txnum ].ext == 0 )
	{
		//テクスチャ番号が不正
		return 1;
	}

	//サーフェイスの解放
	DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
	//サーフェイスの取得
	hr = dxtex[ txnum ].tex->GetSurfaceLevel( 0, &( dxtex[ txnum ].sur[ 0 ] ) );

	//Depth用サーフェイスの解放
	DX_RELEASE( dxtex[ txnum ].zsur );
	//Depth用サーフェイスの作成
	hr = D3DDev->CreateDepthStencilSurface(
		dxtex[ txnum ].width, dxtex[ txnum ].height,
		D3DPP.AutoDepthStencilFormat,
		D3DPP.MultiSampleType,
		D3DPP.MultiSampleQuality,
		FALSE, &( dxtex[ txnum ].zsur ), NULL );

	//ターゲットをテクスチャにする
	hr = D3DDev->SetRenderTarget( 0, dxtex[ txnum ].sur[ 0 ] );
	//Depth用サーフェイスを指定する。
	hr = D3DDev->SetDepthStencilSurface( dxtex[ txnum ].zsur );

	if ( target[ 0 ] == 0 )
	{
		//前回はテクスチャに描画した。
		BackUpTexture( target[ 1 ] );
	}

	target[ 0 ] = 0;
	target[ 1 ] = txnum;

	return 0;
}

int DirectXDraw9::ResetRenderTarget( void )
{
	if ( target[ 0 ] )
	{
		// 画面。
		return SetRenderTarget();
	}
	// テクスチャ。
	return SetRenderTarget( target[ 1 ] );
}

/*!
@brief 画像を読み込んでテクスチャを作る
@par   関数説明
画像ファイルを指定してテクスチャを作る。
@param txnum テクスチャ番号。
@param filename ファイル名。
@param tcolor 透過色。TRC_NONEで何もしない。TRC_ZEROで左上の色を透過色に指定する。省略した場合TRC_NONE(何もしない)。
@param thread スレッド生成フラグ。0でテクスチャ生成時にスレッドを作らない。省略すると1で、スレッド生成時にスレッドを作る。
@return 0=テクスチャ生成スレッドの生成。
@return 1=テクスチャ番号が不正。
@return 2=ファイル名がない。
@return 3=ファイル情報が読み取れなかった。
@return 4=領域確保に失敗。
*/
int DirectXDraw9::CreateTexture( unsigned int txnum, const char *filename, unsigned long tcolor, int thread )
{
	HRESULT hr;
	D3DXIMAGE_INFO image_info;
	struct THREADIN_TEX * in;
	unsigned long tid;

	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	if ( filename == NULL )
	{
		//ファイル名がない
		return 2;
	}

	hr = D3DXGetImageInfoFromFile( filename, &image_info );
	if ( FAILED( hr ) )
	{
		//ファイル情報が読み取れなかった
		return 3;
	}
	//テクスチャを作るので、使用不可にする
	dxtex[ txnum ].ext = 0;
	//書き込みフラグをなしにする。
	dxtex[ txnum ].writed = 0;

	//テクスチャサイズの取得
	dxtex[ txnum ].width = image_info.Width;
	dxtex[ txnum ].height = image_info.Height;
	//リソースの設定
	dxtex[ txnum ].module = NULL;
	dxtex[ txnum ].resource = 0;

	//スレッドに渡す構造体の領域確保
	in = ( struct THREADIN_TEX * )calloc( 1, sizeof( struct THREADIN_TEX ) );

	if ( in == NULL )
	{
		//領域確保に失敗
		return 4;
	}

	if ( dxtex[ txnum ].filename == NULL )
	{
		//ファイル名を入れる文字列領域を確保する
		dxtex[ txnum ].filename = (char *)calloc( strlen( filename ) + 1, sizeof( char ) );
		//ファイル名をコピーする
		memcpy( dxtex[ txnum ].filename, filename, strlen( filename ) );
	}

	//テクスチャの設定を行う。

	//ミップレベルの数
	dxtex[ txnum ].miplevel = 1;//D3DX_DEFAULT;
	//テクスチャの使用目的
	dxtex[ txnum ].usage = D3DUSAGE_RENDERTARGET;//0;
	//テクスチャのフォーマット
	dxtex[ txnum ].format = D3DFMT_A8R8G8B8;//D3DFMT_UNKNOWN;
	//メモリ管理方法
	dxtex[ txnum ].pool = D3DPOOL_DEFAULT;//D3DPOOL_MANAGED;
	//フィルタリングの方法
	dxtex[ txnum ].filter = D3DX_FILTER_NONE;//D3DX_DEFAULT;
	//ミップフィルタリングの方法
	dxtex[ txnum ].mipfilter = D3DX_FILTER_LINEAR;//D3DX_DEFAULT;
	//透過色
	dxtex[ txnum ].color = tcolor;
	//D3DXIMAGE_INFO構造体へのポインタ
	dxtex[ txnum ].imgdata = NULL;
	//PALETTEENTRY構造体へのポインタ
	dxtex[ txnum ].pallete = NULL;
	//頂点の色
	dxtex[ txnum ].vcolor[ 0 ] = dxtex[ txnum ].vcolor[ 1 ] = dxtex[ txnum ].vcolor[ 2 ] = dxtex[ txnum ].vcolor[ 3 ] = 0xFFFFFFFF;

	//DirectXDrawクラスへのポインタ
	in->dxd = this;
	//テクスチャ番号
	in->num = txnum;

	if ( thread )
	{
		in->mode = 1;
		//テクスチャを作るスレッドに残り作業を任せる
		CreateThread( NULL, 0, ThreadCreateTexture, (void *)in, 0, &tid );
	} else
	{
		in->mode = 0;
		//スレッドは作らず続けて処理を行う。
		return ThreadCreateTexture( (void *)in );
	}
	return 0;
}

/*!
@brief リソースを読み込んでテクスチャを作る
@par   関数説明
画像ファイルを指定してテクスチャを作る。
@param txnum テクスチャ番号。
@param module リソースのあるモジュール。同じファイル内ならばNULL。
@param filename リソース名。
@param tcolor 透過色。TRC_NONEで何もしない。TRC_ZEROで左上の色を透過色に指定する。省略した場合TRC_NONE(何もしない)。
@param thread スレッド生成フラグ。0でテクスチャ生成時にスレッドを作らない。省略すると1で、スレッド生成時にスレッドを作る。
@return 0=テクスチャ生成スレッドの生成。
@return 1=テクスチャ番号が不正。
@return 2=リソース名がない。
@return 3=ファイル情報が読み取れなかった。
@return 4=領域確保に失敗。
*/
int DirectXDraw9::CreateTexture( unsigned int txnum, HMODULE module, const char *resource, unsigned long tcolor, int thread )
{
	HRSRC hres;
	void *ptr;

	HRESULT hr;
	D3DXIMAGE_INFO image_info;
	struct THREADIN_TEX * in;
	unsigned long tid;

	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	if ( resource == NULL )
	{
		//リソース名がない
		return 2;
	}

	hres = FindResource( module/*NULL*/, resource, "IMAGE" );
	ptr = LockResource( LoadResource( module/*NULL*/, hres ) );

	//  hr = D3DXGetImageInfoFromResource( module, resource, &image_info );
	hr = D3DXGetImageInfoFromFileInMemory( ptr, SizeofResource( module/*NULL*/, hres ), &image_info );
	if ( FAILED( hr ) )
	{
		//ファイル情報が読み取れなかった
		return 3;
	}
	//テクスチャを作るので、使用不可にする
	dxtex[ txnum ].ext = 0;

	//テクスチャサイズの取得
	dxtex[ txnum ].width = image_info.Width;
	dxtex[ txnum ].height = image_info.Height;
	//リソースの設定
	dxtex[ txnum ].module = module;
	dxtex[ txnum ].resource = 1;

	//スレッドに渡す構造体の領域確保
	in = ( struct THREADIN_TEX * )calloc( 1, sizeof( struct THREADIN_TEX ) );
	in->resouce = ptr;
	in->resoucesize = SizeofResource( module/*NULL*/, hres );

	if ( in == NULL )
	{
		//領域確保に失敗
		return 4;
	}

	if ( dxtex[ txnum ].filename == NULL )
	{
		//ファイル名を入れる文字列領域を確保する
		dxtex[ txnum ].filename = (char *)calloc( strlen( resource ) + 1, sizeof( char ) );
		//ファイル名をコピーする
		memcpy( dxtex[ txnum ].filename, resource, strlen( resource ) );
	}

	//テクスチャの設定を行う。

	//ミップレベルの数
	dxtex[ txnum ].miplevel = 1;//D3DX_DEFAULT;
	//テクスチャの使用目的
	dxtex[ txnum ].usage = D3DUSAGE_RENDERTARGET;//0;
	//テクスチャのフォーマット
	dxtex[ txnum ].format = D3DFMT_A8R8G8B8;//D3DFMT_UNKNOWN;
	//メモリ管理方法
	dxtex[ txnum ].pool = D3DPOOL_DEFAULT;//D3DPOOL_MANAGED;
	//フィルタリングの方法
	dxtex[ txnum ].filter = D3DX_FILTER_NONE;//D3DX_DEFAULT;
	//ミップフィルタリングの方法
	dxtex[ txnum ].mipfilter = D3DX_FILTER_LINEAR;//D3DX_DEFAULT;
	//透過色
	dxtex[ txnum ].color = tcolor;
	//D3DXIMAGE_INFO構造体へのポインタ
	dxtex[ txnum ].imgdata = NULL;
	//PALETTEENTRY構造体へのポインタ
	dxtex[ txnum ].pallete = NULL;
	//頂点の色
	dxtex[ txnum ].vcolor[ 0 ] = dxtex[ txnum ].vcolor[ 1 ] = dxtex[ txnum ].vcolor[ 2 ] = dxtex[ txnum ].vcolor[ 3 ] = 0xFFFFFFFF;

	//DirectXDrawクラスへのポインタ
	in->dxd = this;
	//テクスチャ番号
	in->num = txnum;

	if ( thread )
	{
		in->mode = 1;
		//テクスチャを作るスレッドに残り作業を任せる
		CreateThread( NULL, 0, ThreadCreateTextureFromResorce, (void *)in, 0, &tid );
	} else
	{
		in->mode = 0;
		//スレッドは作らず続けて処理を行う。
		return ThreadCreateTextureFromResorce( (void *)in );
	}
	return 0;
}

/*!
@brief 空のテクスチャを作る
@par   関数説明
空のテクスチャを作る。
@param txnum テクスチャ番号。
@param width テクスチャの横幅。
@param height テクスチャの高さ。
@param thread スレッド生成フラグ。0でテクスチャ生成時にスレッドを作らない。省略すると1で、スレッド生成時にスレッドを作る。
@return 0=テクスチャ生成スレッドの生成。
@return 1=テクスチャ番号が不正。
@return 2=どちらかの大きさが0。
@return 3=領域確保に失敗。
*/
int DirectXDraw9::CreateTexture( unsigned int txnum, unsigned int width, unsigned int height, int thread )
{
	struct THREADIN_TEX * in;
	unsigned long tid;

	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	if ( width == 0 || height == 0 )
	{
		//サイズが不正
		return 2;
	}

	//テクスチャを作るので、使用不可にする
	dxtex[ txnum ].ext = 0;

	//テクスチャサイズの取得
	dxtex[ txnum ].width = width;
	dxtex[ txnum ].height = height;
	//リソースの設定
	dxtex[ txnum ].module = NULL;
	dxtex[ txnum ].resource = 0;

	//スレッドに渡す構造体の領域確保
	in = ( struct THREADIN_TEX * )calloc( 1, sizeof( struct THREADIN_TEX ) );

	if ( in == NULL )
	{
		//領域確保に失敗
		return 3;
	}

	//テクスチャの設定を行う。

	//ミップレベルの数
	dxtex[ txnum ].miplevel = 1;//D3DX_DEFAULT;
	//テクスチャの使用目的
	dxtex[ txnum ].usage = D3DUSAGE_RENDERTARGET;//0;
	//テクスチャのフォーマット
	dxtex[ txnum ].format = D3DFMT_A8R8G8B8;//D3DFMT_UNKNOWN;
	//メモリ管理方法
	dxtex[ txnum ].pool = D3DPOOL_DEFAULT;
	//頂点の色
	dxtex[ txnum ].vcolor[ 0 ] = dxtex[ txnum ].vcolor[ 1 ] = dxtex[ txnum ].vcolor[ 2 ] = dxtex[ txnum ].vcolor[ 3 ] = 0xffffffff;

	//以下4つの値は使わない。
	//フィルタリングの方法
	//ある人より要望。テクスチャの範囲外を描画したときに今は繰り返しだが、例えばX軸方向だけ繰り返しとか、別の色で塗りつぶすオプションが欲しいらしい。
	//ここのフィルタリングを変えれば、その願望が叶うかも。
	dxtex[ txnum ].filter = D3DX_FILTER_NONE;//NULL;
	//ミップフィルタリングの方法
	dxtex[ txnum ].mipfilter = D3DX_FILTER_LINEAR;//NULL;
	//D3DXIMAGE_INFO構造体へのポインタ
	dxtex[ txnum ].imgdata = NULL;
	//PALETTEENTRY構造体へのポインタ
	dxtex[ txnum ].pallete = NULL;

	//DirectXDrawクラスへのポインタ
	in->dxd = this;
	//テクスチャ番号
	in->num = txnum;

	if ( thread )
	{
		in->mode = 1;
		//テクスチャを作るスレッドに残り作業を任せる
		CreateThread( NULL, 0, ThreadCreateTextureEmpty, (void *)in, 0, &tid );
	} else
	{
		in->mode = 0;
		return ThreadCreateTextureEmpty( (void *)in );
	}

	return 0;
}

/*!
@brief 裏でテクスチャを作る。
@par   関数説明
テクスチャを作るスレッド。
@param threadin スレッドに渡す構造体へのポインタ。
@return 0=テクスチャの生成に成功。
@return 1=引数読み込みに失敗。
@return 2=サーフェイスの作成に失敗。
@return 3=ファイルのサーフェイスへの読み込みに失敗。
@return 4=サーフェイスのロックに失敗。
@return 5=テクスチャ生成に失敗。
*/
unsigned long WINAPI DirectXDraw9::ThreadCreateTexture( void * threadin )
{
	HRESULT hr;
	//点取得のためのサーフェイス
	IDirect3DSurface9 *surface;
	//矩形領域
	RECT cutrect;
	//ロックする矩形領域
	D3DLOCKED_RECT lockrect;
	//DirectXDrawクラスへのポインタ
	CLASSDEF_DIRECTX_DRAW * dxd = NULL;
	//テクスチャ番号
	int num = 0;
	int ret = 0;
	int mode = 0;

	if ( threadin == NULL )
	{
		//引数読み込みに失敗
		return 1;
	}

	dxd = ( ( struct THREADIN_TEX * )( threadin ) )->dxd;
	num = ( ( struct THREADIN_TEX * )( threadin ) )->num;
	mode = ( ( struct THREADIN_TEX * )( threadin ) )->mode;

	if ( dxd->dxtex[ num ].color == TRC_ZERO )
	{

		//画像取得領域の作成
		cutrect.left = cutrect.top = 0;
		cutrect.right = cutrect.bottom = 1;
		//空のサーフェイスの作成
		if ( SUCCEEDED( dxd->D3DDev->CreateOffscreenPlainSurface( 1, 1, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL ) ) )
		{
			//画像の左上1pxをサーフェイスに読み込む
			if ( SUCCEEDED( D3DXLoadSurfaceFromFile( surface, NULL, NULL, dxd->dxtex[ num ].filename, &cutrect, D3DX_FILTER_NONE, 0, NULL ) ) )
			{
				//サーフェイスのロック
				if ( SUCCEEDED( surface->LockRect( &lockrect, &cutrect, D3DLOCK_READONLY ) ) )
				{
					//左上の色を取得
					dxd->dxtex[ num ].color = *( (unsigned long *)lockrect.pBits );
					//サーフェイスのアンロック
					surface->UnlockRect();
				} else
				{
					//サーフェイスのロックに失敗
					ret = 4;
				}
				//サーフェイスの解放
				DX_RELEASE( surface );
			} else
			{
				//ファイルのサーフェイスへの読み込みに失敗
				ret = 3;
			}
		} else
		{
			//サーフェイスの作成に失敗
			ret = 2;
		}
	}

	hr = D3DXCreateTextureFromFileEx(
		dxd->D3DDev,                 //D3Dデバイス
		dxd->dxtex[ num ].filename,  //ファイル名
		dxd->dxtex[ num ].width,     //テクスチャサイズ
		dxd->dxtex[ num ].height,    //テクスチャサイズ
		dxd->dxtex[ num ].miplevel,  //ミップレベルの数
		dxd->dxtex[ num ].usage,     //テクスチャの使用目的
		dxd->dxtex[ num ].format,    //テクスチャのフォーマット
		dxd->dxtex[ num ].pool,      //メモリ管理方法
		dxd->dxtex[ num ].filter,    //フィルタリングの方法
		dxd->dxtex[ num ].mipfilter, //ミップフィルタリングの方法
		dxd->dxtex[ num ].color,     //透過色
		dxd->dxtex[ num ].imgdata,   //D3DXIMAGE_INFO構造体へのポインタ
		dxd->dxtex[ num ].pallete,   //PALETTEENTRY構造体へのポインタ
		&( dxd->dxtex[ num ].tex )     //テクスチャを受け取るIDirect3DTexture9へのポインタ
		);

	//構造体の解放
	DX_FREE( threadin );

	if ( FAILED( hr ) )
	{
		//テクスチャ生成に失敗
		return 5;
	}

	dxd->dxtex[ num ].ext = 1;
	dxd->dxtex[ num ].lock = 0;


	if ( mode )
	{
		ExitThread( 0 );
	}
	return ret;
}

/*!
@brief 裏でテクスチャを作る。
@par   関数説明
テクスチャを作るスレッド。
@param threadin スレッドに渡す構造体へのポインタ。
@return 0=テクスチャの生成に成功。
@return 1=引数読み込みに失敗。
@return 2=サーフェイスの作成に失敗。
@return 3=ファイルのサーフェイスへの読み込みに失敗。
@return 4=サーフェイスのロックに失敗。
@return 5=テクスチャ生成に失敗。
*/
unsigned long WINAPI DirectXDraw9::ThreadCreateTextureFromResorce( void * threadin )
{
	HRESULT hr;
	//点取得のためのサーフェイス
	IDirect3DSurface9 *surface;
	//矩形領域
	RECT cutrect;
	//ロックする矩形領域
	D3DLOCKED_RECT lockrect;
	//DirectXDrawクラスへのポインタ
	CLASSDEF_DIRECTX_DRAW * dxd = NULL;
	void *ptr;
	unsigned int size;
	//テクスチャ番号
	int num = 0;
	int ret = 0;
	int mode = 0;

	if ( threadin == NULL )
	{
		//引数読み込みに失敗
		return 1;
	}

	dxd = ( ( struct THREADIN_TEX * )( threadin ) )->dxd;
	num = ( ( struct THREADIN_TEX * )( threadin ) )->num;
	mode = ( ( struct THREADIN_TEX * )( threadin ) )->mode;
	ptr = ( ( struct THREADIN_TEX * )( threadin ) )->resouce;
	size = ( ( struct THREADIN_TEX * )( threadin ) )->resoucesize;

	if ( dxd->dxtex[ num ].color == TRC_ZERO )
	{

		//画像取得領域の作成
		cutrect.left = cutrect.top = 0;
		cutrect.right = cutrect.bottom = 1;
		//空のサーフェイスの作成
		if ( SUCCEEDED( dxd->D3DDev->CreateOffscreenPlainSurface( 1, 1, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL ) ) )
		{
			//画像の左上1pxをサーフェイスに読み込む
			//      if( SUCCEEDED( D3DXLoadSurfaceFromFileInMemory( surface, NULL, NULL, dxd->dxtex[ num ].module, dxd->dxtex[ num ].filename, &cutrect, D3DX_FILTER_NONE, 0, NULL ) ) )
			if ( SUCCEEDED( D3DXLoadSurfaceFromFileInMemory( surface, NULL, NULL, ptr, size, &cutrect, D3DX_FILTER_NONE, 0, NULL ) ) )
			{
				//サーフェイスのロック
				if ( SUCCEEDED( surface->LockRect( &lockrect, &cutrect, D3DLOCK_READONLY ) ) )
				{
					//左上の色を取得
					dxd->dxtex[ num ].color = *( (unsigned long *)lockrect.pBits );
					//サーフェイスのアンロック
					surface->UnlockRect();
				} else
				{
					//サーフェイスのロックに失敗
					ret = 4;
				}
				//サーフェイスの解放
				DX_RELEASE( surface );
			} else
			{
				//ファイルのサーフェイスへの読み込みに失敗
				ret = 3;
			}
		} else
		{
			//サーフェイスの作成に失敗
			ret = 2;
		}
	}

	//  hr = D3DXCreateTextureFromResourceEx(
	//    dxd->D3DDev,                 //D3Dデバイス
	//    dxd->dxtex[ num ].module,    //モジュール
	//    dxd->dxtex[ num ].filename,  //リソース名
	//    dxd->dxtex[ num ].width,     //テクスチャサイズ
	//    dxd->dxtex[ num ].height,    //テクスチャサイズ
	//    dxd->dxtex[ num ].miplevel,  //ミップレベルの数
	//    dxd->dxtex[ num ].usage,     //テクスチャの使用目的
	//    dxd->dxtex[ num ].format,    //テクスチャのフォーマット
	//    dxd->dxtex[ num ].pool,      //メモリ管理方法
	//    dxd->dxtex[ num ].filter,    //フィルタリングの方法
	//    dxd->dxtex[ num ].mipfilter, //ミップフィルタリングの方法
	//    dxd->dxtex[ num ].color,     //透過色
	//    dxd->dxtex[ num ].imgdata,   //D3DXIMAGE_INFO構造体へのポインタ
	//    dxd->dxtex[ num ].pallete,   //PALETTEENTRY構造体へのポインタ
	//    &(dxd->dxtex[ num ].tex)     //テクスチャを受け取るIDirect3DTexture9へのポインタ
	//    );
	hr = D3DXCreateTextureFromFileInMemoryEx(
		dxd->D3DDev,                 //D3Dデバイス
		ptr,                         //リソースへのポインタ
		size,                        //リソースのサイズ
		dxd->dxtex[ num ].width,     //テクスチャサイズ
		dxd->dxtex[ num ].height,    //テクスチャサイズ
		dxd->dxtex[ num ].miplevel,  //ミップレベルの数
		dxd->dxtex[ num ].usage,     //テクスチャの使用目的
		dxd->dxtex[ num ].format,    //テクスチャのフォーマット
		dxd->dxtex[ num ].pool,      //メモリ管理方法
		dxd->dxtex[ num ].filter,    //フィルタリングの方法
		dxd->dxtex[ num ].mipfilter, //ミップフィルタリングの方法
		dxd->dxtex[ num ].color,     //透過色
		dxd->dxtex[ num ].imgdata,   //D3DXIMAGE_INFO構造体へのポインタ
		dxd->dxtex[ num ].pallete,   //PALETTEENTRY構造体へのポインタ
		&( dxd->dxtex[ num ].tex )     //テクスチャを受け取るIDirect3DTexture9へのポインタ
		);
	//構造体の解放
	DX_FREE( threadin );

	if ( FAILED( hr ) )
	{
		//テクスチャ生成に失敗
		return 5;
	}

	dxd->dxtex[ num ].ext = 1;
	dxd->dxtex[ num ].lock = 0;

	if ( mode )
	{
		ExitThread( 0 );
	}
	return ret;
}

/*!
@brief 裏で空のテクスチャを作る。
@par   関数説明
テクスチャを作るスレッド。
@param threadin スレッドに渡す構造体へのポインタ。
@return 0=テクスチャの生成に成功。
@return 1=引数読み込みに失敗。
@return 5=テクスチャ生成に失敗。
*/
unsigned long WINAPI DirectXDraw9::ThreadCreateTextureEmpty( void * threadin )
{
	HRESULT hr;
	//DirectXDrawクラスへのポインタ
	CLASSDEF_DIRECTX_DRAW * dxd = NULL;
	//テクスチャ番号
	int num = 0;
	int ret = 0;
	int mode = 0;

	if ( threadin == NULL )
	{
		//引数読み込みに失敗
		return 1;
	}

	dxd = ( ( struct THREADIN_TEX * )( threadin ) )->dxd;
	num = ( ( struct THREADIN_TEX * )( threadin ) )->num;
	mode = ( ( struct THREADIN_TEX * )( threadin ) )->mode;

	hr = D3DXCreateTexture(
		dxd->D3DDev,                 //D3Dデバイス
		dxd->dxtex[ num ].width,     //テクスチャサイズ
		dxd->dxtex[ num ].height,    //テクスチャサイズ
		dxd->dxtex[ num ].miplevel,  //ミップレベルの数
		dxd->dxtex[ num ].usage,     //テクスチャの使用目的
		dxd->dxtex[ num ].format,    //テクスチャのフォーマット
		dxd->dxtex[ num ].pool,      //メモリ管理方法
		&( dxd->dxtex[ num ].tex )     //テクスチャを受け取るIDirect3DTexture9へのポインタ
		);

	//構造体の解放
	DX_FREE( threadin );

	if ( FAILED( hr ) )
	{
		//テクスチャ生成に失敗
		return 5;
	}

	dxd->dxtex[ num ].ext = 1;
	dxd->dxtex[ num ].lock = 0;

	if ( mode )
	{
		ExitThread( 0 );
	}
	return ret;
}

/*!
@brief テクスチャの解放
@par   関数説明
テクスチャを解放する。
@param txnum テクスチャ番号。
@return 0を返す。
*/
int DirectXDraw9::ReleaseTexture( unsigned int txnum )
{
	if ( CannotUseTexture( txnum ) )
	{
		//テクスチャ番号が不正
		return 1;
	}

	if ( txnum < texmax )
	{
		dxtex[ txnum ].ext = 0;
		dxtex[ txnum ].lock = 0;
		DX_FREE( dxtex[ txnum ].filename );
		DX_FREE( dxtex[ txnum ].pallete );
		DX_FREE( dxtex[ txnum ].imgdata );
		DX_RELEASE( dxtex[ txnum ].tex );
		DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
		DX_RELEASE( dxtex[ txnum ].sur[ 1 ] );
		DX_RELEASE( dxtex[ txnum ].zsur );
		DX_RELEASE( dxtex[ txnum ].backup );
	}
	return 0;
}

/*!
@brief テクスチャの解放
@par   関数説明
全テクスチャを解放する。
@return 開放したテクスチャ数を返す。
*/
int DirectXDraw9::ReleaseTexture()
{
	int ret = 0;
	unsigned int i;

	for ( i = 0; i < texmax; ++i )
	{
		if ( ReleaseTexture( i ) == 0 )
		{
			++ret;
		}
	}

	return ret;
}

//多角形の解放
int DirectXDraw9::ReleasePolygon( unsigned int pvnum )
{
	struct CUSTOMVERTEX_LIST *tmp, *del;
	if ( pvnum <= pvmax )
	{
		if ( pv[ pvnum ].type == 1 )
		{
			// 配列
			DX_FREE( pv[ pvnum ].vertex.cv );
		} else if ( pv[ pvnum ].type == 2 )
		{
			// リスト
			tmp = pv[ pvnum ].vertex.cvl;
			while ( tmp )
			{
				del = tmp;
				tmp = tmp->next;
				DX_FREE( del );
			}
		}
		DX_FREE( pv[ pvnum ].cv );
		pv[ pvnum ].cvmax = 0;
		pv[ pvnum ].max = 0;
		pv[ pvnum ].read.num = 0;
		pv[ pvnum ].read.pointer = NULL;
		pv[ pvnum ].type = 0;

	}
	return 0;
}


//多角形の登録(リスト)
int DirectXDraw9::CreatePolygon( unsigned int pnum, D3DPRIMITIVETYPE filltype )
{
	if ( pnum > pvmax )
	{
		// 番号が不正。
		return 1;
	}

	lastpvnum = pnum;

	// すでに前のがありそうなので開放作業。
	ReleasePolygon( pnum );

	// リストとして多角形を登録していく。
	pv[ pnum ].type = 2;

	pv[ pnum ].max = 0;

	pv[ pnum ].vertex.cvl = NULL;

	pv[ pnum ].txnum = texmax;

	switch ( filltype )
	{
	case D3DPT_POINTLIST:
		pv[ pnum ].filltype = D3DPT_POINTLIST;
		break;
	case D3DPT_LINELIST:
		pv[ pnum ].filltype = D3DPT_LINELIST;
		break;
	case D3DPT_LINESTRIP:
		pv[ pnum ].filltype = D3DPT_LINESTRIP;
		break;
	case D3DPT_TRIANGLELIST:
		pv[ pnum ].filltype = D3DPT_TRIANGLELIST;
		break;
	case D3DPT_TRIANGLESTRIP:
		pv[ pnum ].filltype = D3DPT_TRIANGLESTRIP;
		break;
	case D3DPT_TRIANGLEFAN:
		pv[ pnum ].filltype = D3DPT_TRIANGLEFAN;
		break;
	}
	return 0;
}

//多角形の登録(配列)
int DirectXDraw9::CreatePolygon( unsigned int pnum, unsigned int max, D3DPRIMITIVETYPE filltype )
{
	if ( pnum > pvmax + 1 )
	{
		// 番号が不正。
		return 1;
	}

	lastpvnum = pnum;

	// 配列として多角形を登録していく。
	pv[ pnum ].type = 1;

	pv[ pnum ].max = max;

	pv[ pnum ].txnum = texmax;

	// 領域をさくっと確保。
	pv[ pnum ].vertex.cv = ( struct CUSTOMVERTEX * )calloc( max, sizeof( struct CUSTOMVERTEX ) );

	switch ( filltype )
	{
	case D3DPT_POINTLIST:
		pv[ pnum ].filltype = D3DPT_POINTLIST;
		break;
	case D3DPT_LINELIST:
		pv[ pnum ].filltype = D3DPT_LINELIST;
		break;
	case D3DPT_LINESTRIP:
		pv[ pnum ].filltype = D3DPT_LINESTRIP;
		break;
	case D3DPT_TRIANGLELIST:
		pv[ pnum ].filltype = D3DPT_TRIANGLELIST;
		break;
	case D3DPT_TRIANGLESTRIP:
		pv[ pnum ].filltype = D3DPT_TRIANGLESTRIP;
		break;
	case D3DPT_TRIANGLEFAN:
		pv[ pnum ].filltype = D3DPT_TRIANGLEFAN;
		break;
	}

	return 0;
}

//正多角形の登録
int DirectXDraw9::CreateRegularPolygon( unsigned int pnum, double cx, double cy, unsigned int n, double r, double rad, unsigned long color )
{
	double arad;
	unsigned i;
	CreatePolygon( pnum, n + 2, D3DPT_TRIANGLEFAN );

	// 中央座標
	SetPolygonVertex( pnum, cx, cy, color );

	arad = AngleToRadian( 360.0 / (double)n );

	for ( i = 0; i <= n; ++i )
	{
		SetPolygonVertex( pnum,
			cx + cos( rad + arad * i ) * r - 0.5,
			cy + sin( rad + arad * i ) * r - 0.5,
			color );
	}


	return 0;
}

// テクスチャの登録
int DirectXDraw9::SetTextureOnPolygon( unsigned int pnum, unsigned int tnum )
{
	if ( pnum > pvmax )
	{
		// ポリゴン番号が不正
		return 1;
	}

	if ( CannotUseTexture( tnum ) )//if( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 2;
	}

	pv[ pnum ].txnum = tnum;

	return 0;
}

// テクスチャの登録解除
int DirectXDraw9::SetTextureOnPolygon( unsigned int pnum )
{
	if ( pnum > pvmax )
	{
		// ポリゴン番号が不正
		return 1;
	}

	pv[ pnum ].txnum = texmax;

	return 0;
}

// テクスチャ番号の取得。
unsigned int DirectXDraw9::GetTextureNumOnPolygon( unsigned int pnum )
{
	if ( pnum > pvmax )
	{
		// ポリゴン番号が不正
		return texmax;
	}
	return pv[ pnum ].txnum;
}

// UV座標の登録。
int DirectXDraw9::SetPolygonUV( unsigned int pnum, unsigned int vnum, int u, int v )
{
	struct CUSTOMVERTEX_LIST *next;
	double w, h;

	if ( pnum > pvmax )
	{
		// 番号が不正なので最後の領域を使う。
		pnum = pvmax;
	}

	if ( vnum >= pv[ pnum ].max )
	{
		// 頂点数が不正。
		return 1;
	}

	if ( pv[ pnum ].txnum >= texmax )
	{
		return 2;
	}
	w = (double)GetTextureWidth( pv[ pnum ].txnum );
	h = (double)GetTextureHeight( pv[ pnum ].txnum );

	if ( pv[ pnum ].type == 1 )
	{
		// 配列
		pv[ pnum ].vertex.cv[ vnum ].u = (float)( (double)u / w );
		pv[ pnum ].vertex.cv[ vnum ].v = (float)( (double)v / h );

	} else if ( pv[ pnum ].type == 2 )
	{
		// リスト
		next = pv[ pnum ].vertex.cvl;
		while ( vnum-- > 0 )
		{
			next = next->next;
		}

		next->u = (float)( (double)u / w );
		next->v = (float)( (double)v / h );
	}

	return 0;
}

//頂点の登録(配列限定)
int DirectXDraw9::SetPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color )
{
	if ( pnum > pvmax )
	{
		// 番号が不正なので最後の領域を使う。
		return -1;
	}

	if ( pv[ pnum ].type != 1 )
	{
		// 配列じゃないので不正。
		return -2;
	}

	if ( pv[ pnum ].max <= vnum )
	{
		// 頂点番号が配列サイズを超えてる。
		return -3;
	}

	lastpvnum = vnum;

	// 配列。
	if ( pv[ pnum ].read.num == 0 )
	{
		pv[ pnum ].space[ 0 ] = x;
		pv[ pnum ].space[ 1 ] = x;
		pv[ pnum ].space[ 2 ] = y;
		pv[ pnum ].space[ 3 ] = y;
		pv[ pnum ].space[ 4 ] = 0.5;
		pv[ pnum ].space[ 5 ] = 0.5;
	} else
	{
		if ( x < pv[ pnum ].space[ 0 ] )
		{
			pv[ pnum ].space[ 0 ] = x;
		}
		if ( pv[ pnum ].space[ 1 ] < x )
		{
			pv[ pnum ].space[ 1 ] = x;
		}
		if ( y < pv[ pnum ].space[ 2 ] )
		{
			pv[ pnum ].space[ 2 ] = y;
		}
		if ( pv[ pnum ].space[ 3 ] < y )
		{
			pv[ pnum ].space[ 3 ] = y;
		}
	}

	if ( pv[ pnum ].read.num < pv[ pnum ].max )
	{
		//n = pv[ pnum ].read.num;
		pv[ pnum ].vertex.cv[ vnum ].x = (float)x - 0.5f;
		pv[ pnum ].vertex.cv[ vnum ].y = (float)y - 0.5f;
		pv[ pnum ].vertex.cv[ vnum ].z = ( float )0.5;
		pv[ pnum ].vertex.cv[ vnum ].color = color;
#ifdef USERHW
		pv[ pnum ].vertex.cv[ vnum ].rhw = 1.0f;
#endif
		pv[ pnum ].vertex.cv[ vnum ].u = 0.0f;
		pv[ pnum ].vertex.cv[ vnum ].v = 0.0f;
		//++pv[ pnum ].read.num;
		if ( pv[ pnum ].read.num < vnum )
		{
			pv[ pnum ].read.num = vnum;
		}
	}

	return 0;
}

//頂点の登録
int DirectXDraw9::SetPolygonVertex( unsigned int pnum, double x, double y, unsigned long color )
{
	int n;
	if ( pnum > pvmax )
	{
		// 番号が不正なので最後の領域を使う。
		pnum = pvmax;
	}

	lastpvnum = pnum;

	if ( pv[ pnum ].type == 1 )
	{
		// 配列。
		if ( pv[ pnum ].read.num == 0 )
		{
			pv[ pnum ].space[ 0 ] = x;
			pv[ pnum ].space[ 1 ] = x;
			pv[ pnum ].space[ 2 ] = y;
			pv[ pnum ].space[ 3 ] = y;
			pv[ pnum ].space[ 4 ] = 0.5;
			pv[ pnum ].space[ 5 ] = 0.5;
		} else
		{
			if ( x < pv[ pnum ].space[ 0 ] )
			{
				pv[ pnum ].space[ 0 ] = x;
			}
			if ( pv[ pnum ].space[ 1 ] < x )
			{
				pv[ pnum ].space[ 1 ] = x;
			}
			if ( y < pv[ pnum ].space[ 2 ] )
			{
				pv[ pnum ].space[ 2 ] = y;
			}
			if ( pv[ pnum ].space[ 3 ] < y )
			{
				pv[ pnum ].space[ 3 ] = y;
			}
		}

		if ( pv[ pnum ].read.num < pv[ pnum ].max )
		{
			n = pv[ pnum ].read.num;
			pv[ pnum ].vertex.cv[ n ].x = (float)x - 0.5f;
			pv[ pnum ].vertex.cv[ n ].y = (float)y - 0.5f;
			pv[ pnum ].vertex.cv[ n ].z = ( float )0.5;
			pv[ pnum ].vertex.cv[ n ].color = color;
#ifdef USERHW
			pv[ pnum ].vertex.cv[ n ].rhw = 1.0f;
#endif
			pv[ pnum ].vertex.cv[ n ].u = 0.0f;
			pv[ pnum ].vertex.cv[ n ].v = 0.0f;
			++pv[ pnum ].read.num;
		}
	} else if ( pv[ pnum ].type == 2 )
	{
		// リスト。
		if ( pv[ pnum ].read.pointer )
		{
			if ( x < pv[ pnum ].space[ 0 ] )
			{
				pv[ pnum ].space[ 0 ] = x;
			}
			if ( pv[ pnum ].space[ 1 ] < x )
			{
				pv[ pnum ].space[ 1 ] = x;
			}
			if ( y < pv[ pnum ].space[ 2 ] )
			{
				pv[ pnum ].space[ 2 ] = y;
			}
			if ( pv[ pnum ].space[ 3 ] < y )
			{
				pv[ pnum ].space[ 3 ] = y;
			}

			pv[ pnum ].read.pointer->next = ( struct CUSTOMVERTEX_LIST * )calloc( 1, sizeof( struct CUSTOMVERTEX_LIST ) );
			pv[ pnum ].read.pointer = pv[ pnum ].read.pointer->next;
		} else
		{
			pv[ pnum ].space[ 0 ] = x;
			pv[ pnum ].space[ 1 ] = x;
			pv[ pnum ].space[ 2 ] = y;
			pv[ pnum ].space[ 3 ] = y;

			pv[ pnum ].vertex.cvl = ( struct CUSTOMVERTEX_LIST * )calloc( 1, sizeof( struct CUSTOMVERTEX_LIST ) );
			pv[ pnum ].read.pointer = pv[ pnum ].vertex.cvl;

		}
		pv[ pnum ].read.pointer->x = (float)x - 0.5f;
		pv[ pnum ].read.pointer->y = (float)y - 0.5f;
		pv[ pnum ].read.pointer->z = ( float )0.5;
		pv[ pnum ].read.pointer->color = color;
#ifdef USERHW
		pv[ pnum ].read.pointer->rhw = 1.0f;
#endif
		pv[ pnum ].read.pointer->u = 0.0f;
		pv[ pnum ].read.pointer->v = 0.0f;
		++pv[ pnum ].max;
	}

	return 0;
}

//頂点情報の取得
struct CUSTOMVERTEX * DirectXDraw9::GetPolygonVertex( unsigned int pnum, unsigned int vnum )
{
	struct CUSTOMVERTEX_LIST *next;
	if ( pnum > pvmax )
	{
		// 番号が不正なので最後の領域を使う。
		pnum = pvmax;
	}

	if ( vnum >= pv[ pnum ].max )
	{
		// 頂点数が不正なのでNULLを返す。
		return NULL;
	}

	if ( pv[ pnum ].type == 1 )
	{
		return &( pv[ pnum ].vertex.cv[ vnum ] );
	} else if ( pv[ pnum ].type == 2 )
	{
		next = pv[ pnum ].vertex.cvl;
		while ( vnum-- > 0 )
		{
			next = next->next;
		}
		return ( struct CUSTOMVERTEX * )next;
	}

	return 0;
}

// 頂点要素数を返す
unsigned int DirectXDraw9::GetPolygonVertexLength( unsigned int pnum )
{
	if ( pnum > pvmax )
	{
		// 番号が不正
		return 0;
	}
	// 配列を使っている場合は最大数ではなく使った要素数を返す。
	return pv[ pnum ].type == 1 ? pv[ pnum ].read.num : pv[ pnum ].max;
}

//頂点情報の編集
int DirectXDraw9::EditPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color, double u, double v, int edit )
{
	struct CUSTOMVERTEX_LIST *next;
	if ( pnum > pvmax )
	{
		// 番号が不正なので最後の領域を使う。
		pnum = pvmax;
	}

	if ( vnum >= pv[ pnum ].max )
	{
		// 頂点数が不正なのでNULLを返す。
		return NULL;
	}

	if ( pv[ pnum ].type == 1 )
	{
		// 配列
		if ( edit & PVEDIT_X )
		{
			pv[ pnum ].vertex.cv[ vnum ].x = (float)x - 0.5f;
		}
		if ( edit & PVEDIT_Y )
		{
			pv[ pnum ].vertex.cv[ vnum ].y = (float)y - 0.5f;
		}
		if ( edit & PVEDIT_Z )
		{
			pv[ pnum ].vertex.cv[ vnum ].z = ( float )0.5;
		}
		if ( edit & PVEDIT_COLOR )
		{
			pv[ pnum ].vertex.cv[ vnum ].color = color;
		}
		if ( edit & PVEDIT_U )
		{
			if ( pv[ pnum ].txnum >= texmax )
			{
				pv[ pnum ].vertex.cv[ vnum ].u = (float)u / (float)GetTextureWidth( pv[ pnum ].txnum );
			} else
			{
				pv[ pnum ].vertex.cv[ vnum ].u = (float)u;
			}
		}
		if ( edit & PVEDIT_V )
		{
			if ( pv[ pnum ].txnum >= texmax )
			{
				pv[ pnum ].vertex.cv[ vnum ].v = (float)v / (float)GetTextureHeight( pv[ pnum ].txnum );
			} else
			{
				pv[ pnum ].vertex.cv[ vnum ].v = (float)v;
			}
		}

	} else if ( pv[ pnum ].type == 2 )
	{
		// リスト
		next = pv[ pnum ].vertex.cvl;
		while ( vnum-- > 0 )
		{
			next = next->next;
		}

		if ( edit & PVEDIT_X )
		{
			next->x = (float)x - 0.5f;
		}
		if ( edit & PVEDIT_Y )
		{
			next->y = (float)y - 0.5f;
		}
		if ( edit & PVEDIT_Z )
		{
			next->z = ( float )0.5;
		}
		if ( edit & PVEDIT_COLOR )
		{
			next->color = color;
		}
		if ( edit & PVEDIT_U )
		{
			next->u = (float)u;
		}
		if ( edit & PVEDIT_V )
		{
			next->v = (float)v;
		}
	}

	return 0;
}

/*int DirectXDraw9::SetPolygonVertex( double x, double y, unsigned long color )
{
return SetPolygonVertex( lastpvnum, x, y, color );
}*/

// 多角形の変形

// 移動
int DirectXDraw9::TransformPolygonMoving( unsigned int pnum, double mx, double my )
{
	unsigned int i;
	struct CUSTOMVERTEX_LIST *next;

	if ( pnum > pvmax )
	{
		// 番号が不正なので最後の番号を使う。
		pnum = pvmax;
	}

	if ( pv[ pnum ].type == 1 )
	{
		// 配列

		// 座標移動。
		for ( i = 0; i < pv[ pnum ].read.num; ++i )
		{
			pv[ pnum ].vertex.cv[ i ].x += (float)mx;
			pv[ pnum ].vertex.cv[ i ].y += (float)my;
			//pv[ pnum ].vertex.cv[ i ].z += (float)mz;
		}
	} else if ( pv[ pnum ].type == 2 )
	{
		// リスト

		// コピーしつつ座標移動。
		next = pv[ pnum ].vertex.cvl;
		for ( i = 0; i < pv[ pnum ].max; ++i )
		{
			next->x += (float)mx;
			next->y += (float)my;
			//next->z += (float)mz;
			next = next->next;
		}
	}

	return 0;
}

// 拡大
int DirectXDraw9::TransformPolygonScaling( unsigned int pnum, double scale, unsigned int basenum )
{
	unsigned int i;
	double mx, my;
	struct CUSTOMVERTEX_LIST *next;
	D3DXMATRIX posmatrix, zoommatrix;

	// 行列の初期化。
	D3DXMatrixIdentity( &posmatrix );
	D3DXMatrixIdentity( &zoommatrix );

	// 拡大率の値をzoommatrix行列に代入する。
	D3DXMatrixScaling( &zoommatrix, (float)scale, (float)scale, 0.0 );

	if ( pnum > pvmax )
	{
		// 番号が不正なので最後の番号を使う。
		pnum = pvmax;
	}

	if ( pv[ pnum ].type == 1 )
	{
		// 配列

		// 基準座標への移動距離。
		mx = pv[ pnum ].vertex.cv[ basenum ].x;
		my = pv[ pnum ].vertex.cv[ basenum ].y;

		// 座標を拡大・移動
		for ( i = 0; i < pv[ pnum ].read.num; ++i )
		{
			// 基準点を原点とした時の頂点の位置を算出。
			D3DXMatrixTranslation( &posmatrix, (float)( pv[ pnum ].vertex.cv[ i ].x - mx ), (float)( pv[ pnum ].vertex.cv[ i ].y - my ), (float)( pv[ pnum ].vertex.cv[ i ].z ) );
			// 行列演算。
			posmatrix *= zoommatrix;

			pv[ pnum ].vertex.cv[ i ].x = posmatrix._41 - (float)mx;
			pv[ pnum ].vertex.cv[ i ].y = posmatrix._42 - (float)my;
			pv[ pnum ].vertex.cv[ i ].z = posmatrix._43;
		}
	} else if ( pv[ pnum ].type == 2 )
	{
		// リスト

		// 基準座標を調べる。
		next = pv[ pnum ].vertex.cvl;
		for ( i = 0; i < basenum; ++i )
		{
			next = next->next;
		}
		// 基準座標への移動距離。
		mx = next->x;
		my = next->y;

		// 新配列に座標を拡大・移動しつつコピー。
		next = pv[ pnum ].vertex.cvl;
		for ( i = 0; i < pv[ pnum ].max; ++i )
		{
			// 基準点を原点とした時の頂点の位置を算出。
			D3DXMatrixTranslation( &posmatrix, (float)( pv[ pnum ].vertex.cv[ i ].x - mx ), (float)( pv[ pnum ].vertex.cv[ i ].y - my ), (float)( pv[ pnum ].vertex.cv[ i ].z ) );
			// 行列演算。
			posmatrix *= zoommatrix;

			next->x = posmatrix._41 - (float)mx;
			next->y = posmatrix._42 - (float)my;
			next->z = posmatrix._43;
			next = next->next;
		}
	}

	return 0;
}

/*!
@brief 色の作成
@par   関数説明
Mikan(DirectX)で使われる色を作る。
@param alpha アルファ値。
@param red 赤。
@param green 緑。
@param blue 青。
@return 色。0xAARRGGBB。
*/
unsigned long DirectXDraw9::Color( unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue )
{
	return D3DCOLOR_ARGB( alpha, red, green, blue );
}

unsigned long DirectXDraw9::ColorHSV( unsigned char alpha, unsigned int hue, unsigned char sat, unsigned char vob )
{
	// TODO
	// 未実装
	return 0;
}

/*!
@brief カラーの設定
@par   関数説明
テクスチャを染める色を設定する。4色同時設定。
@param txnum テクスチャ番号。
@param color 色。
@return 0=成功。
@return 1=テクスチャ番号が不正。
*/
int DirectXDraw9::SetColor( unsigned int txnum, unsigned long color )
{
	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	dxtex[ txnum ].vcolor[ 3 ] = dxtex[ txnum ].vcolor[ 2 ] = dxtex[ txnum ].vcolor[ 1 ] = dxtex[ txnum ].vcolor[ 0 ] = color;
	return 0;
}

int DirectXDraw9::SetAlpha( unsigned int txnum, unsigned char alpha )
{
	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	dxtex[ txnum ].vcolor[ 0 ] = ( dxtex[ txnum ].vcolor[ 0 ] & 0x00ffffff ) | alpha << 24;
	dxtex[ txnum ].vcolor[ 1 ] = ( dxtex[ txnum ].vcolor[ 1 ] & 0x00ffffff ) | alpha << 24;
	dxtex[ txnum ].vcolor[ 2 ] = ( dxtex[ txnum ].vcolor[ 2 ] & 0x00ffffff ) | alpha << 24;
	dxtex[ txnum ].vcolor[ 3 ] = ( dxtex[ txnum ].vcolor[ 3 ] & 0x00ffffff ) | alpha << 24;
	return 0;
}

/*!
@brief 頂点カラーの設定
@par   関数説明
テクスチャを染める色を設定する。1色だけの設定なので、4回使えば4頂点に別々の色を設定する事ができる。
@param txnum テクスチャ番号。
@param color 色。
@return 0=成功。
@return 1=テクスチャ番号が不正。
*/
int DirectXDraw9::SetColorVertex( unsigned int txnum, unsigned long color )
{
	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	dxtex[ txnum ].vcolor[ 3 ] = dxtex[ txnum ].vcolor[ 2 ];
	dxtex[ txnum ].vcolor[ 2 ] = dxtex[ txnum ].vcolor[ 1 ];
	dxtex[ txnum ].vcolor[ 1 ] = dxtex[ txnum ].vcolor[ 0 ];
	dxtex[ txnum ].vcolor[ 0 ] = color;
	return 0;
}

//テクスチャのデータを読めるようにする。
int DirectXDraw9::OpenTexture( unsigned int txnum, int x, int y, int w, int h )
{
	//テクスチャは直に値を読み書き出来ないので、次のような手順で読み書きする。
	//1.テクスチャをサーフェイスにする(解放すべきサーフェイスの生成)。
	//2.サーフェイスに対しロックを掛ける。(OpenTextureの作業ここまで)
	//3.サーフェイスに対して作業を行う。
	//4.サーフェイスをアンロックする。(CloseTextureの作業)
	//5.サーフェイスをテクスチャに書き戻す。(CloseTextureの作業)
	//非常に面倒かつグラボが古いと対応していない場合もある。
	//さらに、ロックをとる作業に非常に時間がかかるので、テクスチャ操作系の関数は以下のように作業する。
	//パターン1
	// 予めOpenTextureが実行されない場合には、開始時に実行して終了時に閉じる。
	//パターン2
	// 予めOpenTextureが実行された場合には開始時と終了時に何もせず、
	// CloseTexture使用時にデータが改変されていたらテクスチャに書き戻す。

	//画像取得領域の作成
	//  RECT cutrect;
	//サーフェイスのフォーマット
	D3DSURFACE_DESC desc;
	unsigned int size;

	//テクスチャチェック
	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}
	if ( w <= 0 )
	{
		w = dxtex[ txnum ].width;
	}
	if ( h <= 0 )
	{
		h = dxtex[ txnum ].height;
	}

	//置換範囲の設定
	dxtex[ txnum ].cutrect.left = x < 0 ? ( x = 0 ) : x;
	dxtex[ txnum ].cutrect.right = ( ( size = x + w ) > dxtex[ txnum ].width ) ? dxtex[ txnum ].width : size;//w <= 0 ? dxtex[ txnum ].width : x + w;
	dxtex[ txnum ].cutrect.top = y < 0 ? ( y = 0 ) : y;
	dxtex[ txnum ].cutrect.bottom = ( ( size = y + h ) > dxtex[ txnum ].height ) ? dxtex[ txnum ].height : size;//h <= 0 ? dxtex[ txnum ].height : y + h;

	//テクスチャをサーフェイスに
	DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
	DX_RELEASE( dxtex[ txnum ].sur[ 1 ] );
	dxtex[ txnum ].tex->GetSurfaceLevel( 0, &( dxtex[ txnum ].sur[ 0 ] ) );

	//レンダーテクスチャのフォーマットを取得
	hr = dxtex[ txnum ].tex->GetLevelDesc( 0, &desc );
	//レンダーテクスチャと同じフォーマットのサーフェイスをシステムメモリに確保
	hr = D3DDev->CreateOffscreenPlainSurface(
		desc.Width, desc.Height, desc.Format,
		D3DPOOL_SYSTEMMEM, &( dxtex[ txnum ].sur[ 1 ] ), NULL );

	hr = D3DDev->GetRenderTargetData( dxtex[ txnum ].sur[ 0 ], dxtex[ txnum ].sur[ 1 ] );

	if ( SUCCEEDED( hr ) && dxtex[ txnum ].sur[ 1 ] )
	{
		hr = dxtex[ txnum ].sur[ 1 ]->LockRect( &( dxtex[ txnum ].lockrect ), &( dxtex[ txnum ].cutrect ), D3DLOCK_DISCARD );
		//テクスチャのロック
		if ( SUCCEEDED( hr ) )
		{
			dxtex[ txnum ].lock = 1;
			return 0;
		}
	}
	return 2;
}

//テクスチャのデータを読める状態から戻す。
int DirectXDraw9::CloseTexture( unsigned int txnum )
{
	//テクスチャチェック
	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}

	dxtex[ txnum ].sur[ 1 ]->UnlockRect();

	//テクスチャにサーフェイスを書き戻す
	if ( dxtex[ txnum ].writed )
	{
		hr = D3DDev->UpdateSurface( dxtex[ txnum ].sur[ 1 ], NULL, dxtex[ txnum ].sur[ 0 ], NULL );
		dxtex[ txnum ].writed = 0;
		BackUpTexture( txnum );
	}

	//サーフェイスの解放(いるかなぁ？)
	DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
	DX_RELEASE( dxtex[ txnum ].sur[ 1 ] );
	dxtex[ txnum ].lock = 0;
	return 0;
}

/*!
@brief テクスチャの指定座標の色を取得する。
@par   関数説明
テクスチャの指定座標の色を取得する。
@param txnum テクスチャ番号。
@param x x座標。
@param y y座標。
@return 0=成功。
@return 1=テクスチャ番号が不正。
*/
unsigned long DirectXDraw9::GetColorTexturePixel( unsigned int txnum, int x, int y )
{
	//画像取得領域の作成
	//  RECT cutrect;
	unsigned long color = 0x00000000;
	//ロックする矩形領域
	//  D3DLOCKED_RECT lockrect;
	//サーフェイスのフォーマット
	//  D3DSURFACE_DESC desc;
	//サーフェイス
	//  LPDIRECT3DSURFACE9 surface[2] = {};

	//  cutrect.left   = x;
	//  cutrect.right  = x + 1;
	//  cutrect.top    = y;
	//  cutrect.bottom = y + 1;

	//テクスチャチェック
	//  if( CanUseTexture( txnum ) )//if( txnum < texmax && dxtex[ txnum ].ext )
	//  {
	//テクスチャをサーフェイスに
	//    dxtex[ txnum ].tex->GetSurfaceLevel( 0, &(surface[0]) );

	//レンダーテクスチャのフォーマットを取得
	//    hr = dxtex[ txnum ].tex->GetLevelDesc( 0, &desc );
	//レンダーテクスチャと同じフォーマットのサーフェイスをシステムメモリに確保
	//    hr = D3DDev->CreateOffscreenPlainSurface(
	//      desc.Width, desc.Height, desc.Format,
	//      D3DPOOL_SYSTEMMEM, &(surface[1]), NULL );

	//    hr = D3DDev->GetRenderTargetData( surface[0], surface[1] );

	//    if( SUCCEEDED( hr ) && surface[1] )
	//    {
	//テクスチャのロック
	//      if( SUCCEEDED( surface[1]->LockRect( &lockrect, &cutrect, D3DLOCK_READONLY ) ) )//if( SUCCEEDED( dxtex[ txnum ].tex->LockRect( 0, &lockrect, &cutrect, D3DLOCK_READONLY ) ) )
	//      {
	//左上の色を取得
	//        color = *( (unsigned long *)lockrect.pBits );
	//サーフェイスのアンロック
	//        surface[1]->UnlockRect();//dxtex[ txnum ].tex->UnlockRect( 0 );
	//      }
	//    }
	//サーフェイスの解放
	//    DX_RELEASE( surface[0] );
	//    DX_RELEASE( surface[1] );
	//  }

	int locked;

	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}

	locked = dxtex[ txnum ].lock;
	if ( locked == 0 )
	{
		// テクスチャロックを行なっていないので、さくっとロック。
		OpenTexture( txnum, x, y, 1, 1 );
		color = *( (unsigned long *)( dxtex[ txnum ].lockrect.pBits ) );
		CloseTexture( txnum );
		return color;
	}

	//memcpy((BYTE*)lockRect.pBits + lockRect.Pitch*y + 4*x ), &color, sizeof(DWORD));
	//  color = *((unsigned long*)dxtex[ txnum ].lockrect.pBits + dxtex[ txnum ].lockrect.Pitch * y + 4 * x );
	color = *( (unsigned long*)dxtex[ txnum ].lockrect.pBits + dxtex[ txnum ].lockrect.Pitch * y / 4 + x );

	return color;
}

/*!
@brief テクスチャの色を別の色に置換する
@par   関数説明
テクスチャの範囲内の色を置換する。
@param txnum テクスチャ番号。
@param scolor 検索する色。
@param rcolor 置換する色。
@param x 検索開始x座標。初期値は0。
@param y 検索開始y座標。初期値は0。
@param w 横幅。初期値は0で、テクスチャの横幅になる。
@param h 高さ。初期値は0で、テクスチャの高さになる。
@return 置換した回数を返す。一度も置換しなければ0。
*/
unsigned int DirectXDraw9::TextureColorReplacement( unsigned int txnum, unsigned long scolor, unsigned long rcolor, int x, int y, int w, int h )
{
	int count = 0;
	int rx, ry, locked;
	//  unsigned int size;
	unsigned char *line;
	unsigned long *pixel;
	//画像取得領域の作成
	//  RECT cutrect;
	//ロックする矩形領域
	//  D3DLOCKED_RECT lockrect;
	//サーフェイスのフォーマット
	//  D3DSURFACE_DESC desc;
	//サーフェイス
	//  IDirect3DSurface9 *surface= NULL;

	//テクスチャチェック
	//  if( CanUseTexture( txnum ) )//if( txnum < texmax && dxtex[ txnum ].ext )
	//  {

	//    if( w <= 0 )
	//    {
	//      w = dxtex[ txnum ].width - x;
	//    }
	//    if( h <= 0 )
	//    {
	//      h = dxtex[ txnum ].height - y;
	//    }

	//置換範囲の設定
	//    cutrect.left   = x;
	//    cutrect.right  = ( (size = x + w) > dxtex[ txnum ].width ) ? dxtex[ txnum ].width : size;
	//    cutrect.top    = y;
	//    cutrect.bottom = ( (size = y + h) > dxtex[ txnum ].height ) ? dxtex[ txnum ].height : size;

	//テクスチャをサーフェイスに
	//    DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
	//    dxtex[ txnum ].tex->GetSurfaceLevel( 0, &(dxtex[ txnum ].sur[ 0 ]) );

	//レンダーテクスチャのフォーマットを取得
	//    hr = dxtex[ txnum ].tex->GetLevelDesc( 0, &desc );
	//レンダーテクスチャと同じフォーマットのサーフェイスをシステムメモリに確保
	//    hr = D3DDev->CreateOffscreenPlainSurface(
	//      desc.Width, desc.Height, desc.Format,
	//      D3DPOOL_SYSTEMMEM, &(surface), NULL );

	//    hr = D3DDev->GetRenderTargetData( dxtex[ txnum ].sur[ 0 ], surface );

	//    if( SUCCEEDED( hr ) && surface )
	//    {
	//テクスチャのロック
	//      if( SUCCEEDED( surface->LockRect( &lockrect, &cutrect, D3DLOCK_DISCARD ) ) )//D3DLOCK_READONLY ) ) )//if( SUCCEEDED( dxtex[ txnum ].tex->LockRect( 0, &lockrect, &cutrect, D3DLOCK_READONLY ) ) )
	//      {
	//色の検索
	//        line = (unsigned char *)lockrect.pBits;
	//        for( ry = 0; ry < h ; ++ry )
	//        {
	//          pixel = (unsigned long*)line;
	//          for( rx = 0; rx < w ; rx++ )
	//          {
	//色の判定
	//            if( *pixel == scolor )
	//            {
	//色の書き換え
	//              *pixel = rcolor;
	//              ++count;
	//              dxtex[ txnum ].writed = 1;
	//            }
	//次のドットへ
	//            ++pixel;
	//          }
	//          line += lockrect.Pitch;
	//        }
	//サーフェイスのアンロック
	//        surface->UnlockRect();
	//      }
	//    }
	//テクスチャにサーフェイスを書き戻す
	//    hr = D3DDev->UpdateSurface( surface, NULL, dxtex[ txnum ].sur[ 0 ], NULL);

	//    BackUpTexture( txnum );

	//サーフェイスの解放
	//    DX_RELEASE( surface );
	//  }

	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}

	locked = dxtex[ txnum ].lock;
	if ( locked == 0 )
	{
		// テクスチャをロックしてないのでロック開始。
		OpenTexture( txnum, x, y, w, h );
	}

	// 色の検索
	line = (unsigned char *)dxtex[ txnum ].lockrect.pBits;
	for ( ry = 0; ry < h; ++ry )
	{
		pixel = (unsigned long*)line;
		for ( rx = 0; rx < w; rx++ )
		{
			//色の判定
			if ( *pixel == scolor )
			{
				//色の書き換え
				*pixel = rcolor;
				++count;
				dxtex[ txnum ].writed = 1;
			}
			//次のドットへ
			++pixel;
		}
		line += dxtex[ txnum ].lockrect.Pitch;
	}

	if ( locked == 0 )
	{
		// テクスチャをロックしてなかったので元に戻すべくアンロック。
		CloseTexture( txnum );
	}

	return count;
}

/*!
@brief テクスチャの色を検索する
@par   関数説明
テクスチャの範囲内の色を検索する。
@param txnum テクスチャ番号。
@param scolor 検索する色。
@param x 検索開始x座標。初期値は0。
@param y 検索開始y座標。初期値は0。
@param w 横幅。初期値は0で、テクスチャの横幅になる。
@param h 高さ。初期値は0で、テクスチャの高さになる。
@return 0 見つからなかった。
@return 初めてヒットするまでの検索済みピクセル数。左上から右下に向かう順番。
*/
unsigned int DirectXDraw9::TextureColorSearch( unsigned int txnum, unsigned long scolor, int x, int y, int w, int h )
{
	int count = 0;
	int rx, ry, locked;
	//  unsigned int size;
	unsigned char *line;
	unsigned long *pixel;
	//画像取得領域の作成
	//  RECT cutrect;
	//ロックする矩形領域
	//  D3DLOCKED_RECT lockrect;
	//サーフェイスのフォーマット
	//  D3DSURFACE_DESC desc;
	//サーフェイス
	//  IDirect3DSurface9 * surface= NULL;

	//テクスチャチェック
	//  if( CanUseTexture( txnum ) )//if( txnum < texmax && dxtex[ txnum ].ext )
	//  {

	if ( w <= 0 )
	{
		w = dxtex[ txnum ].width - x;
	}
	if ( h <= 0 )
	{
		h = dxtex[ txnum ].height - y;
	}

	//検索範囲
	//    cutrect.left   = x;
	//    cutrect.right  = ( (size = x + w) > dxtex[ txnum ].width ) ? dxtex[ txnum ].width : size;
	//    cutrect.top    = y;
	//    cutrect.bottom = ( (size = y + h) > dxtex[ txnum ].height ) ? dxtex[ txnum ].height : size;

	//テクスチャをサーフェイスに
	//    DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
	//    dxtex[ txnum ].tex->GetSurfaceLevel( 0, &(dxtex[ txnum ].sur[ 0 ]) );

	//レンダーテクスチャのフォーマットを取得
	//    hr = dxtex[ txnum ].tex->GetLevelDesc( 0, &desc );
	//レンダーテクスチャと同じフォーマットのサーフェイスをシステムメモリに確保
	//    hr = D3DDev->CreateOffscreenPlainSurface(
	//      desc.Width, desc.Height, desc.Format,
	//      D3DPOOL_SYSTEMMEM, &(surface), NULL );

	//    hr = D3DDev->GetRenderTargetData( dxtex[ txnum ].sur[ 0 ], surface );

	//    if( SUCCEEDED( hr ) && surface )
	//    {
	//テクスチャのロック
	//      if( SUCCEEDED( surface->LockRect( &lockrect, &cutrect, D3DLOCK_READONLY ) ) )
	//      {
	//色の検索
	//        line = (unsigned char *)lockrect.pBits;
	//        for( ry = 0; ry < h ; ++ry )
	//        {
	//          pixel = (unsigned long*)line;
	//          for( rx = 0; rx < w ; rx++ )
	//          {
	//            ++count;
	//色の判定
	//            if( *pixel == scolor )
	//            {
	//サーフェイスのアンロック
	//              surface->UnlockRect();
	//サーフェイスの解放
	//              DX_RELEASE( surface );
	//              return count;
	//            }
	//次のドットへ
	//            ++pixel;
	//          }
	//          line += lockrect.Pitch;
	//        }
	//サーフェイスのアンロック
	//        surface->UnlockRect();
	//      }
	//    }
	//サーフェイスの解放
	//    DX_RELEASE( surface );
	//  }

	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}

	locked = dxtex[ txnum ].lock;
	if ( locked == 0 )
	{
		// テクスチャをロックしてないのでロック開始。
		OpenTexture( txnum, x, y, w, h );
	}

	// 色の検索
	line = (unsigned char *)dxtex[ txnum ].lockrect.pBits;
	for ( ry = 0; ry < h; ++ry )
	{
		pixel = (unsigned long*)line;
		for ( rx = 0; rx < w; rx++ )
		{
			++count;
			//色の判定
			if ( *pixel == scolor )
			{
				ry = h;
				goto out;
			}
			//次のドットへ
			++pixel;
		}
		line += dxtex[ txnum ].lockrect.Pitch;
	}

	count = 0;

out:
	if ( locked == 0 )
	{
		// テクスチャをロックしてなかったので元に戻すべくアンロック。
		CloseTexture( txnum );
	}

	return count;
}

/*!
@brief テクスチャの色を検索し数を数える
@par   関数説明
テクスチャの範囲内の色を検索する。
@param txnum テクスチャ番号。
@param scolor 検索する色。
@param x 検索開始x座標。初期値は0。
@param y 検索開始y座標。初期値は0。
@param w 横幅。初期値は0で、テクスチャの横幅になる。
@param h 高さ。初期値は0で、テクスチャの高さになる。
@return 0 見つからなかった。
@return 範囲内で見つかった検索色のピクセル数。
*/
unsigned int DirectXDraw9::TextureColorCount( unsigned int txnum, unsigned long scolor, int x, int y, int w, int h )
{
	int count = 0;
	int rx, ry, locked;
	//  unsigned int size;
	unsigned char *line;
	unsigned long *pixel;
	//画像取得領域の作成
	//  RECT cutrect;
	//ロックする矩形領域
	//  D3DLOCKED_RECT lockrect;
	//サーフェイスのフォーマット
	//  D3DSURFACE_DESC desc;
	//サーフェイス
	//  IDirect3DSurface9 * surface= NULL;

	//テクスチャチェック
	//  if( CanUseTexture( txnum ) )//if( txnum < texmax && dxtex[ txnum ].ext )
	//  {

	//    if( w <= 0 )
	//    {
	//      w = dxtex[ txnum ].width - x;
	//    }
	//    if( h <= 0 )
	//    {
	//      h = dxtex[ txnum ].height - y;
	//    }

	//検索範囲
	//    cutrect.left   = x;
	//    cutrect.right  = ( (size = x + w) > dxtex[ txnum ].width ) ? dxtex[ txnum ].width : size;
	//    cutrect.top    = y;
	//    cutrect.bottom = ( (size = y + h) > dxtex[ txnum ].height ) ? dxtex[ txnum ].height : size;

	//テクスチャをサーフェイスに
	//    DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
	//    dxtex[ txnum ].tex->GetSurfaceLevel( 0, &(dxtex[ txnum ].sur[ 0 ]) );

	//レンダーテクスチャのフォーマットを取得
	//    hr = dxtex[ txnum ].tex->GetLevelDesc( 0, &desc );
	//レンダーテクスチャと同じフォーマットのサーフェイスをシステムメモリに確保
	//    hr = D3DDev->CreateOffscreenPlainSurface(
	//      desc.Width, desc.Height, desc.Format,
	//      D3DPOOL_SYSTEMMEM, &(surface), NULL );

	//    hr = D3DDev->GetRenderTargetData( dxtex[ txnum ].sur[ 0 ], surface );

	//    if( SUCCEEDED( hr ) && surface )
	//    {
	//テクスチャのロック
	//      if( SUCCEEDED( surface->LockRect( &lockrect, &cutrect, D3DLOCK_READONLY ) ) )
	//      {
	//色の検索
	//        line = (unsigned char *)lockrect.pBits;
	//        for( ry = 0; ry < h ; ++ry )
	//        {
	//          pixel = (unsigned long*)line;
	//          for( rx = 0; rx < w ; rx++ )
	//          {
	//色の判定
	//            if( *pixel == scolor )
	//            {
	//サーフェイスのアンロック
	//              surface->UnlockRect();
	//サーフェイスの解放
	//              DX_RELEASE( surface );
	//              ++count;
	//            }
	//次のドットへ
	//            ++pixel;
	//          }
	//          line += lockrect.Pitch;
	//        }
	//サーフェイスのアンロック
	//        surface->UnlockRect();
	//      }
	//    }
	//サーフェイスの解放
	//    DX_RELEASE( surface );
	//  }

	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}

	locked = dxtex[ txnum ].lock;
	if ( locked == 0 )
	{
		// テクスチャをロックしてないのでロック開始。
		OpenTexture( txnum, x, y, w, h );
	}

	// 色の検索
	line = (unsigned char *)dxtex[ txnum ].lockrect.pBits;
	for ( ry = 0; ry < h; ++ry )
	{
		pixel = (unsigned long*)line;
		for ( rx = 0; rx < w; rx++ )
		{
			//色の判定
			if ( *pixel == scolor )
			{
				++count;
			}
			//次のドットへ
			++pixel;
		}
		line += dxtex[ txnum ].lockrect.Pitch;
	}

	if ( locked == 0 )
	{
		// テクスチャをロックしてなかったので元に戻すべくアンロック。
		CloseTexture( txnum );
	}

	return count;
}

/*!
@brief テクスチャの横幅取得
@par   関数説明
テクスチャの横幅を取得。
@param txnum テクスチャ番号。
@return 0=失敗。
@return その他=テクスチャの横幅。
*/
unsigned int DirectXDraw9::GetTextureWidth( unsigned int txnum )
{
	if ( CanUseTexture( txnum ) )//if( txnum < texmax && dxtex[ txnum ].ext )
	{
		return dxtex[ txnum ].width;
	}
	return 0;
}

/*!
@brief テクスチャの高さ取得
@par   関数説明
テクスチャの高さを取得。
@param txnum テクスチャ番号。
@return 0=失敗。
@return その他=テクスチャの高さ。
*/
unsigned int DirectXDraw9::GetTextureHeight( unsigned int txnum )
{
	if ( CanUseTexture( txnum ) )//if( txnum < texmax && dxtex[txnum].ext )
	{
		return dxtex[ txnum ].height;
	}
	return 0;
}

/*!
@brief スクリーンショットを撮る
@par   関数説明
現在のスクリーンショット
@param filename ファイル名。
@param imagetype 画像のタイプ。
@return 常に0を返す。
*/
int DirectXDraw9::ScreenShot( const char *filename, unsigned int imagetype )
{
	if ( imagetype >= IMAGE_MAX )
	{
		imagetype = IMAGE_BMP;
	}
	D3DXSaveSurfaceToFile( filename, (D3DXIMAGE_FILEFORMAT)imagetype, backbuffer, NULL, NULL );
	return 0;
}

// テクスチャを画像へ出力
int DirectXDraw9::TextureToImage( const char *filename, unsigned int txnum, unsigned int imagetype )
{
	int locked;
	RECT cutrect;

	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}

	if ( imagetype >= IMAGE_MAX )
	{
		imagetype = IMAGE_BMP;
	}

	locked = dxtex[ txnum ].lock;
	if ( locked == 0 )
	{
		// テクスチャをロックしてないのでロック開始。
		OpenTexture( txnum );

		D3DXSaveSurfaceToFile( filename, (D3DXIMAGE_FILEFORMAT)imagetype, dxtex[ txnum ].sur[ 0 ], NULL, NULL );

		// テクスチャをロックしてなかったので元に戻すべくアンロック。
		CloseTexture( txnum );
	} else
	{
		cutrect = dxtex[ txnum ].cutrect;
		D3DXSaveSurfaceToFile( filename, (D3DXIMAGE_FILEFORMAT)imagetype, dxtex[ txnum ].sur[ 0 ], NULL, &cutrect );
	}

	return 0;
}



/*!
@brief 値を調整する。
@par   関数説明
値を2のn上に調整する。
@param size 調整する値。
@return 調整後の値。
*/
int DirectXDraw9::ControlSize( int size )
{
	//これlog2をとってpowした方がいい気がする。
	if ( size <= 2 )
	{
		return 2;
	} else if ( size <= 4 )
	{
		return 4;
	} else if ( size <= 8 )
	{
		return 8;
	} else if ( size <= 16 )
	{
		return 16;
	} else if ( size <= 32 )
	{
		return 32;
	} else if ( size <= 64 )
	{
		return 64;
	} else if ( size <= 128 )
	{
		return 128;
	} else if ( size <= 256 )
	{
		return 256;
	} else if ( size <= 512 )
	{
		return 512;
	} else if ( size <= 1024 )
	{
		return 1024;
	} else if ( size <= 2048 )
	{
		return 2048;
	} else if ( size <= 4096 )
	{
		return 4096;
	} else if ( size <= 8192 )
	{
		return 8192;
	}
	return size;
}

IDirect3DDevice9 * DirectXDraw9::GetDirect3DDevice( int force, unsigned long sleeptime )
{
	if ( force == 0 )
	{
		while ( drawing == 0 )
		{
			Sleep( sleeptime );
		}
	}
	return D3DDev;
}
HRESULT DirectXDraw9::SetRenderState( D3DRENDERSTATETYPE State, DWORD Value )
{
	return D3DDev->SetRenderState( State, Value );
}
HRESULT DirectXDraw9::SetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value )
{
	return D3DDev->SetTextureStageState( Stage, Type, Value );
}
