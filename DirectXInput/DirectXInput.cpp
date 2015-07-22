#include "DirectXInput.h"

// 振動機能搭載したいが、何か失敗する。

/*!
@class   DirectXInput DirectXInput.h "DirectXInput.h"
@brief   _MikanInputクラス。DirectXを用いた入力を行う。
@author  Hiroki
@version 1.0
*/

int CALLBACK JoystickCallback( const DIDEVICEINSTANCE* lpdi, void *obj )
{
	CLASSDEF_DIRECTX_INPUT *dic = (CLASSDEF_DIRECTX_INPUT*)obj;
	HRESULT hr;

	//ジョイスティック用にデバイスオブジェクトを作成
	hr = dic->DIPad->CreateDevice( lpdi->guidInstance, &( dic->DIDevPad[ dic->CountPadNum ] ), NULL );

	if ( hr != DI_OK )
	{
		return DIENUM_CONTINUE;
	}

	++dic->CountPadNum;
	if ( dic->CountPadNum == PAD_MAX )
	{
		return DIENUM_STOP;
	}
	//  pDev->QueryInterface( IID_IDirectInputDevice2, (LPVOID *)&pDInputDevice );

	return DIENUM_CONTINUE;
}

/*!
@brief コンストラクタ
@par   関数説明
変数の初期化を行う
*/
DirectXInput::DirectXInput( void )
{
	int p, i;
	// Pad2Key無効化。
	SetPad2KeyInput();

	attached = 0;
	for ( p = 0; p < PAD_MAX; ++p )
	{
		//全てのパッドを初期化する
		//g_pDIDevPad[ i ] = NULL;
		Pad[ p ].use = 0;
		//どのゲームパッドが押されたかのフラグを初期化する
		Pad[ p ].push = 0;
		// Pad2Keyの初期化
		InitPad2Key( p );
		//PadCFF[ p ] = 0;
		for ( i = 0; i < PAD_KEYMOUNT; ++i )
		{
			Pad[ p ].value[ i ] = Pad[ p ].buf[ i ] = 0;
			Pad[ p ].pov_mode = 0;
		}
	}
	//バッファの初期化
	for ( i = 0; i < 256; ++i )
	{
		Key[ i ].value = Key[ i ].buf = 0;
	}
	//マウスボタンの初期化
	for ( i = 0; i < MOUSE_BUTTON_MAX; ++i )
	{
		Mouse[ i ] = MouseBuf[ i ] = 0;
	}

	//パッドの数の初期化
	CountPadNum = PadNum = 0;
	//マウスの変数の初期化
	MouseX = MouseY = MouseZ = MouseWheel = 0;
	//ウィンドウハンドルの初期化
	WindowHandle = NULL;

	// マウスの数(RawInput)
	mousenum = 0;
	rawmouse = NULL;
	SetMouseArea( 0, 0, 3000, 3000 );
	rawsize = 0;
	rawdata = NULL;

	//その他DirectXInput絡みの初期化
	DIKey = NULL;
	DIPad = NULL;
	DIMouse = NULL;

	init[ 0 ] = init[ 1 ] = init[ 2 ] = 0;
	SetForceFeedBackMode( 0 );
}

/*!
@brief デストラクタ
@par   関数説明
オブジェクトの解放など。
*/
DirectXInput::~DirectXInput( void )
{
	unsigned int p;//, i;

	DX_RELEASE( DIKey );

	/*  for( i = 0 ; i < PAD_EFFECT_MAX ; ++i )
	  {
	  DX_FREE( PadEffect[ i ].cf );
	  DX_FREE( PadEffect[ i ].eff );
	  for( p = 0 ; p < PAD_MAX ; ++p )
	  {
	  DX_RELEASE( PadEffect[ i ].die[ p ] );
	  }
	  }*/

	for ( p = 0; p < PadNum; ++p )
	{
		if ( DIDevPad[ p ] )
		{
			DIDevPad[ p ]->Unacquire();
			//一部ゲームパッドの開放時に妙なエラーが出るため不本意ながらコメントアウト。
			//実際には解放すべき。
			//エラー文面のサンプル。
			//MikanSample2010.exe の 0x009c3635 でハンドルされていない例外が発生しました: 0xC0000005: 場所 0xfeeeff6a を読み込み中にアクセス違反が発生しました。
			//RFVibraPS02.dllのある場所で落ちてるらしい。
			//DX_RELEASE( DIDevPad[ p ] );
		}
	}

	DX_RELEASE( DIPad );

	DX_FREE( rawmouse );
	DX_FREE( rawdata );
}

HWND DirectXInput::GetWindowHandle( void )
{
	return WindowHandle;
}

int DirectXInput::GetForceFeedBackMode( void )
{
	return forcefeedback;
}

int DirectXInput::SetForceFeedBackMode( int enable )
{
	// ゲームパッドの初期化前。
	// 初期化後だったらもう一回初期化する？
	if ( init[ 1 ] == 0 )
	{
		forcefeedback = enable;
	}

	return 0;
}

/*!
@brief キーボード入力の初期化
@par   関数説明
キーボード入力のためのデバイス初期などを行う。
@param wh ウィンドウハンドル。
@param ih インスタンスハンドル。
@return 0=成功。
@return 1=DirectInputの作成に失敗。
@return 2=デバイスオブジェクトの作成に失敗。
@return 3=データ・フォーマットの設定に失敗。
@return 4=強調モードの設定に失敗。
*/
int DirectXInput::InitKeyboard( HWND wh, HINSTANCE ih )
{
	// DirectInputの作成
	hr = DirectInput8Create( ih, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DIKey, NULL );
	if ( FAILED( hr ) )
	{
		return 1;
	}

	// デバイス・オブジェクトを作成
	hr = DIKey->CreateDevice( GUID_SysKeyboard, &DIDevKey, NULL );
	if ( FAILED( hr ) )
	{
		return 2;
	}

	// データ・フォーマットを設定
	hr = DIDevKey->SetDataFormat( &c_dfDIKeyboard );
	if ( FAILED( hr ) )
	{
		return 3;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = DIDevKey->SetCooperativeLevel( wh, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	if ( FAILED( hr ) )
	{
		return 4;
	}
	// 入力制御開始
	DIDevKey->Acquire();
	init[ 0 ] = 1;
	return 0;
}

/*!
@brief パッドの初期化
@par   関数説明
パッドの初期化を行う。
@param wh ウィンドウハンドル。
@param ih インスタンスハンドル。
@return 0=成功。
@return 1=DirectInput8の生成に失敗。
@return 2=パッドを探すためのCallback関数の設定に失敗。
*/
int DirectXInput::InitGamepad( HWND wh, HINSTANCE ih )
{
	HRESULT hr;
	void *pt;
	int i, p;

	this->WindowHandle = wh;

	hr = DirectInput8Create( ih, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DIPad, NULL );
	if ( hr != DI_OK )
	{
		return 1;
	}

#ifdef __USEFORCEFEEDBACK
	if ( GetForceFeedBackMode() )
	{
		//フォースフィードバック関係の初期化
		//エフェクトの軸
		PadEffectAxes[ 0 ] = DIJOFS_X;
		PadEffectAxes[ 1 ] = DIJOFS_Y;
		//エフェクトの対象初期化
		//左右揺れなし。
		PadEffectDirection[ 0 ][ 0 ] = 0;
		PadEffectDirection[ 0 ][ 1 ] = 0;
		//右だけ揺れる。
		PadEffectDirection[ 1 ][ 0 ] = 0;
		PadEffectDirection[ 1 ][ 1 ] = 1;
		//左だけ揺れる。
		PadEffectDirection[ 2 ][ 0 ] = 1;
		PadEffectDirection[ 2 ][ 1 ] = 0;
		//左右が揺れる。
		PadEffectDirection[ 3 ][ 0 ] = 1;
		PadEffectDirection[ 3 ][ 1 ] = 1;

		for ( i = 0; i < PAD_EFFECT_MAX; ++i )
		{
			PadEffect[ i ].cf = NULL;
			PadEffect[ i ].eff = NULL;
			for ( p = 0; p < PAD_MAX; ++p )
			{
				PadEffect[ i ].die[ p ] = NULL;
			}
		}

		CreateForceFeedback( 0 );
	}
#endif

	//ジョイスティックを探す
	pt = this;

	if ( GetForceFeedBackMode() )
	{
		hr = DIPad->EnumDevices( DI8DEVCLASS_GAMECTRL, JoystickCallback, ( void* )this, DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK );
	} else
	{
		hr = DIPad->EnumDevices( DI8DEVCLASS_GAMECTRL, JoystickCallback, ( void* )this, DIEDFL_ATTACHEDONLY );
	}
	if ( FAILED( hr ) )
	{
		DX_RELEASE( DIPad );
		return 2;
	}
	//ゲームパッドの認識
	RecognitionGamepad( 0 );

	//RecognitionGamepad();

	init[ 1 ] = 1;
	return 0;
}

/*!
@brief マウスの初期化
@par   関数説明
マウスの初期化を行う。
@param wh ウィンドウハンドル。
@param ih インスタンスハンドル。
@return 0=成功。
@return 1=DirectInput8の生成に失敗。
@return 2=パッドを探すためのCallback関数の設定に失敗。
*/
int DirectXInput::InitMouse( HWND wh, HINSTANCE ih )
{
	DIPROPDWORD dipdw;

	//DIrectInputオブジェクトの作成
	hr = DirectInput8Create( ih, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DIMouse, NULL );
	if ( FAILED( hr ) )
	{
		return 1;
	}

	//デバイス・オブジェクトを作成
	hr = DIMouse->CreateDevice( GUID_SysMouse, &DIDevMouse, NULL );
	if ( FAILED( hr ) )
	{
		return 2;
	}

	//データ形式の設定
	hr = DIDevMouse->SetDataFormat( &c_dfDIMouse2 );
	if ( FAILED( hr ) )
	{
		return 3;
	}

	//マウスの動作の設定
	hr = DIDevMouse->SetCooperativeLevel( wh, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	if ( FAILED( hr ) )
	{
		return 4;
	}

	ZeroMemory( &dipdw, sizeof( dipdw ) );
	dipdw.diph.dwSize = sizeof( DIPROPDWORD );
	dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;

	// データ
	//dipdw.dwData            = DIPROPAXISMODE_REL; //相対値軸モード
	//dipdw.dwData            = DIPROPAXISMODE_ABS; //絶対値軸モード
	//軸基準
	//hr = DIDevMouse->SetProperty( DIPROP_AXISMODE, &dipdw.diph );

	//バッファサイズ
	dipdw.dwData = 1000;
	//変異基準
	DIDevMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
	if ( FAILED( hr ) )
	{
		return 4;
	}

	//有効化
	DIDevMouse->Acquire();
	init[ 2 ] = 1;
	return 0;
}

/*!
@brief キーボードとパッドの初期化
@par   関数説明
内部でInitKeyboardとInitGamepadを呼び出している。
@param wh ウィンドウハンドル。
@param ih インスタンス。
@return 0を返す。
*/
int DirectXInput::SetUp( HWND wh, HINSTANCE ih )
{

	InitKeyboard( wh, ih );
	InitGamepad( wh, ih );
	InitMouse( wh, ih );

	return 0;
}

int DirectXInput::SetStickCenter( unsigned int padnum )
{
	struct DIJOYSTATE js;
	int center, max;

	if ( padnum >= PAD_MAX || Pad[ padnum ].use == 0 )
	{
		return 1;
	}

	hr = DIDevPad[ padnum ]->Poll();

	while ( FAILED( hr ) )
	{
		hr = DIDevPad[ padnum ]->Acquire();
		while ( hr == DIERR_INPUTLOST )
		{
			hr = DIDevPad[ padnum ]->Acquire();
		}
		hr = DIDevPad[ padnum ]->Poll();
	}

	hr = DIDevPad[ padnum ]->GetDeviceState( sizeof( DIJOYSTATE ), &js );

	//アナログ(スティックやトリガー)の値域を-1000～1000にするための前処理。
	//正直言ってここの処理はどうするか微妙。
	//今現在左スティックの初期値が0で値域が-1000～1000のコントローラーと
	//初期値が32767で値域が0～65535のコントローラー(XBOX)が確認されている。

	//左アナログスティックの上下方向の初期値を真ん中と仮定。
	//何かいい方法ない？
	center = js.lY;

	if ( center == 0 )
	{
		//中央が0の時は最大値が1000のコントローラーを見たのでこれに設定。
		max = 1000;
	} else
	{
		max = center * 2;
		if ( max + 2 > 0xffff )
		{
			//中央が65535/2くらいの値だったらだいたい最大値は65535なのでこれにする。
			max = 0xffff;
		}
	}

	return SetStickCenter( padnum, center, max );
}

int DirectXInput::SetStickCenter( unsigned int padnum, int center, int max )
{
	Pad[ padnum ].stickcenter[ 0 ] = center;
	Pad[ padnum ].stickcenter[ 1 ] = max;

	return 0;
}

/*!
@brief パッドの認識
@par   関数説明
パッドの再認識を行う。
@param resume 0で初めから認識をし直す。省略した場合は1と同じで追加認識を行う。
@return 0=成功。
@return 1=データフォーマットの設定に失敗。
@return 2=モード設定に失敗。
*/
int DirectXInput::RecognitionGamepad( int resume )
{
	int begin = 0, end, i;
	struct DIPROPRANGE diprg;


	if ( resume )
	{
		begin = PadNum;
	}

	end = CountPadNum;
	for ( i = begin; i < end; ++i )
	{
		//DX_RELEASE( g_pDIDevPad[ i ] );
		//データフォーマットを設定
		hr = DIDevPad[ i ]->SetDataFormat( &c_dfDIJoystick );
		if ( hr != DI_OK )
		{
			//DX_RELEASE(pDInputDevice);
			//DX_RELEASE( DIPad );
			DIDevPad[ i ]->Unacquire();
			--i;
			--PadNum;
			continue;
			//return 1;
		}

		//モードを設定
		//ret = pDInputDevice->SetCooperativeLevel(hwnd,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		//    hr = g_pDIDevPad[ i ]->SetCooperativeLevel( WindowHandle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND );

		if ( GetForceFeedBackMode() )
		{
			// フォースフィードバックに必要な排他協調モードだっけ？の設定。
			hr = DIDevPad[ i ]->SetCooperativeLevel( WindowHandle, DISCL_EXCLUSIVE | DISCL_FOREGROUND );
		} else
		{
			hr = DIDevPad[ i ]->SetCooperativeLevel( WindowHandle, DISCL_NONEXCLUSIVE | ( pad2key ? DISCL_BACKGROUND : DISCL_FOREGROUND ) );
		}
		if ( hr != DI_OK )
		{
			DX_RELEASE( DIPad );
			return 2;
		}

		Pad[ i ].devcaps.dwSize = sizeof( DIDEVCAPS );
		hr = DIDevPad[ i ]->GetCapabilities( &Pad[ i ].devcaps );
		if ( FAILED( hr ) )
		{
			continue;
		}
		//    Pad[ i ].devcaps.dwFlags;//DIDC_FORCEFEEDBACK=フォースフィードバック可能かどうか

		// 軸の値の範囲を設定
		ZeroMemory( &diprg, sizeof( diprg ) );
		diprg.diph.dwSize = sizeof( diprg );
		diprg.diph.dwHeaderSize = sizeof( diprg.diph );
		diprg.diph.dwObj = DIJOFS_X;
		//diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.diph.dwHow = DIPH_BYID;
		//    diprop.diph.dwObj = 0;
		//    diprg.diph.dwHow = DIPH_DEVICE;
		diprg.lMin = -1000;
		diprg.lMax = +1000;
		//    DIDevPad[ i ]->SetProperty( DIPROP_RANGE, &diprg.diph );
		diprg.diph.dwObj = DIJOFS_Y;//DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;//
		DIDevPad[ i ]->SetProperty( DIPROP_RANGE, &diprg.diph );

		DIDevPad[ i ]->Acquire();
		Pad[ i ].use = 1;
		++PadNum;
		if ( Pad[ i ].devcaps.dwAxes > 0 )
		{
			SetStickCenter( i );
			//      SetStickCenter( i, 65535/2+1,65535 );
		}

		if ( GetForceFeedBackMode() )
		{
			DIPROPDWORD DIPropAutoCenter;

			DIPropAutoCenter.diph.dwSize = sizeof( DIPropAutoCenter );
			DIPropAutoCenter.diph.dwHeaderSize = sizeof( DIPROPHEADER );
			DIPropAutoCenter.diph.dwObj = 0;
			DIPropAutoCenter.diph.dwHow = DIPH_DEVICE;
			DIPropAutoCenter.dwData = DIPROPAUTOCENTER_OFF;

			hr = DIDevPad[ i ]->SetProperty( DIPROP_AUTOCENTER,
				&DIPropAutoCenter.diph );
			if ( FAILED( hr ) )
			{
				// Handle the failure as appropriate
			}
		}

	}

	return 0;
}

/*!
@brief 特定のキー入力の状態を取得する。
@par   関数説明
特定のキーに対する入力がどうなっているかを調べて返す。
@param keynum 調べたいキーの値。詳しくはkeymap.hに記されたキーマップ参照。
@return -1=離れた。
@return 0=押していない。
@return 0以上=押している。
*/
int DirectXInput::GetKeyNum( int keynum )
{
	if ( 0 <= keynum && keynum < KB_KEYMOUNT )
	{
		return Key[ keynum ].value;
	}
	return 0;
}

int DirectXInput::GetKeyNumRapid( int keynum, int threshold, unsigned int interval )
{
	int i;
	i = GetKeyNum( keynum );
	if ( i >= threshold && ++interval > 0 )
	{
		return ( ( i - threshold ) % interval == 0 );
	}
	return 0;
}

int DirectXInput::GetKeyNumRapidF( int keynum, int threshold, unsigned int interval )
{
	int i;
	i = GetKeyNum( keynum );
	if ( i == 1 )
	{
		return 1;
	}
	if ( i >= threshold && ++interval > 0 )
	{
		return ( ( i - threshold ) % interval == 0 );
	}
	return 0;
}

int DirectXInput::GetKeyWhichButton( void )
{
	int b, val = 0x7fffffff, ret = -1;

	for ( b = 0; b <= 256; ++b )
	{
		if ( Key[ b ].value > 0 && Key[ b ].value < val )
		{
			val = Key[ b ].value;
			ret = b;
		}
	}
	return ret;
}

/*!
@brief 関数実行時に押されているキーの状態を調べる。
@par   関数説明
関数を実行したときのキーボードの状態を取得し、押されている場合はバッファに状態を保存する。
このままでは入力の更新は行われない。
@return 0=成功。
@return 1=キー状態の取得が出来なかったので、入力制御を再度開始した。
*/
int DirectXInput::UpdateKeyStatus( void )
{
	int i;
	char KeyState[ 256 ] = {};

	//キーの状態を調べる。
	hr = DIDevKey->GetDeviceState( sizeof( KeyState ), KeyState );
	if ( SUCCEEDED( hr ) )
	{
		for ( i = 0; i < KB_KEYMOUNT; ++i )
		{
			if ( KeyState[ i ] & 0x80 )
			{
				//キーが押されている。
				Key[ i ].buf = 1;
			}
		}
		return 0;
	} else
	{
		// 入力制御開始
		if ( DIDevKey )
		{
			DIDevKey->Acquire();
		}
	}
	return 1;
}

/*!
@brief 実行時までに押されたキーに対し、値を更新する。
@par   関数説明
UpdateKeyStatusによって取得されたバッファを元に、キー入力を更新する。
この関数はある区間内(フレームなど)で一度だけ実行するのが好ましい。
この関数は単体でUpdateKeyStatusを行うので、事前に使う必要はない。
@return 0=成功。
@return 1=UpdateKeyStateが失敗。
*/
int DirectXInput::UpdateKeyInput( void )
{
	int i;
	//キーの状態を調べる。
	//UpdateKeyStatus();
	//if ( SUCCEEDED( hr ) )
	if ( UpdateKeyStatus() == 0 )
	{
		for ( i = 0; i < KB_KEYMOUNT; ++i )
		{
			if ( Key[ i ].buf > 0 )
			{
				//キーが押されている。
				if ( Key[ i ].value < KB_MAXINPUT )
				{
					++Key[ i ].value;
				}
			} else if ( Key[ i ].value > 0 )
			{
				//キーが放された。
				Key[ i ].value = -1;
			} else
			{
				Key[ i ].value = 0;
			}
			Key[ i ].buf = 0;
		}
		return 0;
	}
	return 1;
}

/*!
@brief 指定されたパッドの指定ボタンの入力状態を取得する。
@par   関数説明
認識されたあるパッドの指定ボタンの入力状態を調べて返す。
@param padnum パッド番号。
@param button 上下左右はPAD_UP,PAD_DOWN,PAD_LEFT,PAD_RIGHTとなり、その他ボタンは1番からPAD_A,PAD_Bと
名付けられ、PAD_Lまで存在する。
@return -1=離れた。
@return 0=押していない。
@return 0以上=押している。
*/
int DirectXInput::GetPadNum( unsigned int padnum, int button )
{
	if ( padnum < PadNum && 0 <= button && button < PAD_KEYMOUNT )
	{
		return Pad[ padnum ].value[ button ];
	}
	return 0;
}

/*!
@brief 指定されたパッドのスティック入力を取得する。
@par   関数説明
認識されたあるパッドのアナログスティックの入力状態を調べて返す。
値はアナログの生の値で方向はDirectXの座標系と同じで、PAD_UDは下方向が正、上方向が負。PAD_LRは右方向が正、左方向が負。
@param padnum パッド番号。
@param button アナログスティックの上下方向はPAD_UDで、左右方向はPAD_LRで取得できる。
@return 0=押していない。もしくは無効。
@return それ以外=スティックの入力。傾き具合。
*/
int DirectXInput::GetPadStick( unsigned int padnum, int button )
{
	if ( padnum < PadNum && 0 <= button && button < PAD_KEYMOUNT )
	{
		return Pad[ padnum ].buf[ button ];
	}
	return 0;
}

/*!
@brief パッドの認識数
@par   関数説明
現在認識されているパッドの数を返す。
@return 現在に認識しているパッドの数。
*/
int DirectXInput::GetPadMount( void )
{
	return PadNum;
}

int DirectXInput::GetPadWhichButton( unsigned int padnum )
{
	int b, val = 0x7fffffff, ret = -1;

	for ( b = 0; b <= PAD_L; ++b )
	{
		if ( Pad[ padnum ].value[ b ] > 0 && Pad[ padnum ].value[ b ] < val )
		{
			val = Pad[ padnum ].value[ b ];
			ret = b;
		}
	}

	return ret;
}

/*!
@brief 指定パッドの指定ボタンのバッファを更新する。
@par   関数説明
UpdatePadStateにてボタンを押していることを感知した時に呼び出される。
バッファに1を入れる仕事とどのパッドが押されているかのフラグを立てる仕事を行う。
仮想入力などには使えるが、これをむやみに使うことを禁止する。
@param padnum パッド番号。
@param button GetPadNum参照。
@return バッファに入れた値(1)を返す。
*/
int DirectXInput::PadPush( unsigned int padnum, int button )
{
	Pad[ padnum ].push = 1;
	return ( Pad[ padnum ].buf[ button ] = 1 );
}

/*!
@brief 現在指定パッドの状態を取得する。
@par   関数説明
現在どのパッドのどのボタンが押されているかを判断し、バッファにフラグを立てる。
@param padnum パッド番号
@return 0=成功。
@return 1=パッド番号が不正か、パッドが使われていない。
@return 2=パッドの入力準備が出来ていない。
@return 3=ステータスの取得に失敗。
*/
int DirectXInput::UpdatePadStatus( unsigned int padnum )
{
	//この関数は、入力バッファに値を入れる。
	//入力バッファを入力値に入れるのはUpdatePadInput。
	HRESULT hr;
	struct DIJOYSTATE js;

	if ( padnum >= PAD_MAX || Pad[ padnum ].use == 0 )
	{
		return 1;
	}

	// 取得準備
	hr = DIDevPad[ padnum ]->Poll();
	if ( FAILED( hr ) )
	{
		hr = DIDevPad[ padnum ]->Acquire();
		while ( hr == DIERR_INPUTLOST )
		{
			hr = DIDevPad[ padnum ]->Acquire();
		}
		return 2;
	}
	// 取得
	hr = DIDevPad[ padnum ]->GetDeviceState( sizeof( DIJOYSTATE ), &js );
	if ( FAILED( hr ) )
	{
		return 3;
	}

	// 取得情報

	//これらはアナログ値としてバッファにためる。
	//左スティック
	Pad[ padnum ].buf[ PAD_LS_UD ] = PAD_STICK_VALUE( js.lY, Pad[ padnum ].stickcenter[ 0 ], Pad[ padnum ].stickcenter[ 1 ] );
	Pad[ padnum ].buf[ PAD_LS_LR ] = PAD_STICK_VALUE( js.lX, Pad[ padnum ].stickcenter[ 0 ], Pad[ padnum ].stickcenter[ 1 ] );
	//右スティック。XY軸
	//コントローラーによって色々。
	Pad[ padnum ].buf[ PAD_X_AXIS ] = PAD_STICK_VALUE( js.lRx, Pad[ padnum ].stickcenter[ 0 ], Pad[ padnum ].stickcenter[ 1 ] );
	Pad[ padnum ].buf[ PAD_Y_AXIS ] = PAD_STICK_VALUE( js.lRy, Pad[ padnum ].stickcenter[ 0 ], Pad[ padnum ].stickcenter[ 1 ] );
	//Z軸。
	//コントローラーによって色々。
	//XBOXコンだとLトリガー(負)とRトリガー(正)の合計値。つまり両方同じ強さで押すと0。
	Pad[ padnum ].buf[ PAD_Z_AXIS ] = PAD_STICK_VALUE( js.lZ, Pad[ padnum ].stickcenter[ 0 ], Pad[ padnum ].stickcenter[ 1 ] );
	Pad[ padnum ].buf[ PAD_RZ_AXIS ] = PAD_STICK_VALUE( js.lRz, Pad[ padnum ].stickcenter[ 0 ], Pad[ padnum ].stickcenter[ 1 ] );

	if ( Pad[ padnum ].pov_mode )
	{
		//POV有効と考えて、十字キーの入力のみ受け付ける。
		if ( js.rgdwPOV[ 0 ] == 0 || js.rgdwPOV[ 0 ] == 4500 || js.rgdwPOV[ 0 ] == 31500 )
		{
			PadPush( padnum, PAD_UP );
		}
		if ( js.rgdwPOV[ 0 ] == 13500 || js.rgdwPOV[ 0 ] == 18000 || js.rgdwPOV[ 0 ] == 22500 )
		{
			PadPush( padnum, PAD_DOWN );
		}
		if ( js.rgdwPOV[ 0 ] == 22500 || js.rgdwPOV[ 0 ] == 27000 || js.rgdwPOV[ 0 ] == 31500 )
		{
			PadPush( padnum, PAD_LEFT );
		}
		if ( js.rgdwPOV[ 0 ] == 4500 || js.rgdwPOV[ 0 ] == 9000 || js.rgdwPOV[ 0 ] == 13500 )
		{
			PadPush( padnum, PAD_RIGHT );
		}
	} else
	{
		//十字キーの入力を受け付ける。
		if ( js.rgdwPOV[ 0 ] == 0 || js.rgdwPOV[ 0 ] == 4500 || js.rgdwPOV[ 0 ] == 31500 )
		{
			PadPush( padnum, PAD_UP );
		}
		if ( js.rgdwPOV[ 0 ] == 13500 || js.rgdwPOV[ 0 ] == 18000 || js.rgdwPOV[ 0 ] == 22500 )
		{
			PadPush( padnum, PAD_DOWN );
		}
		if ( js.rgdwPOV[ 0 ] == 22500 || js.rgdwPOV[ 0 ] == 27000 || js.rgdwPOV[ 0 ] == 31500 )
		{
			PadPush( padnum, PAD_LEFT );
		}
		if ( js.rgdwPOV[ 0 ] == 4500 || js.rgdwPOV[ 0 ] == 9000 || js.rgdwPOV[ 0 ] == 13500 )
		{
			PadPush( padnum, PAD_RIGHT );
		}
		//アナログスティックの入力も上下左右として認識して受け付ける。
		if ( Pad[ padnum ].buf[ PAD_LS_UD ] < -300 )
		{
			PadPush( padnum, PAD_UP );
		}
		if ( Pad[ padnum ].buf[ PAD_LS_UD ] > 300 )
		{
			PadPush( padnum, PAD_DOWN );
		}
		if ( Pad[ padnum ].buf[ PAD_LS_LR ] < -300 )
		{
			PadPush( padnum, PAD_LEFT );
		}
		if ( Pad[ padnum ].buf[ PAD_LS_LR ] > 300 )
		{
			PadPush( padnum, PAD_RIGHT );
		}
	}

	//ゲームパッドのボタン
	if ( js.rgbButtons[ 0 ] > 0 )
	{
		PadPush( padnum, PAD_A );
	}
	if ( js.rgbButtons[ 1 ] > 0 )
	{
		PadPush( padnum, PAD_B );
	}
	if ( js.rgbButtons[ 2 ] > 0 )
	{
		PadPush( padnum, PAD_C );
	}
	if ( js.rgbButtons[ 3 ] > 0 )
	{
		PadPush( padnum, PAD_D );
	}
	if ( js.rgbButtons[ 4 ] > 0 )
	{
		PadPush( padnum, PAD_E );
	}
	if ( js.rgbButtons[ 5 ] > 0 )
	{
		PadPush( padnum, PAD_F );
	}
	if ( js.rgbButtons[ 6 ] > 0 )
	{
		PadPush( padnum, PAD_G );
	}
	if ( js.rgbButtons[ 7 ] > 0 )
	{
		PadPush( padnum, PAD_H );
	}
	if ( js.rgbButtons[ 8 ] > 0 )
	{
		PadPush( padnum, PAD_I );
	}
	if ( js.rgbButtons[ 9 ] > 0 )
	{
		PadPush( padnum, PAD_J );
	}
	if ( js.rgbButtons[ 10 ] > 0 )
	{
		PadPush( padnum, PAD_K );
	}
	if ( js.rgbButtons[ 11 ] > 0 )
	{
		PadPush( padnum, PAD_L );
	}

	//アナログスティックのフレーム単位で加算する値。
	//スティックのアナログ値はGetPadStickで取得できるが、GetPadNumで取得できないので
	//ここで取得できるようにする。
	//つまりスティックのアナログ値ではなく、スティックが何フレームどの方向に傾いているかを示す。
	if ( Pad[ padnum ].buf[ PAD_LS_UD ] < -300 )
	{
		PadPush( padnum, PAD_LS_UP );
	}
	if ( Pad[ padnum ].buf[ PAD_LS_UD ] > 300 )
	{
		PadPush( padnum, PAD_LS_DOWN );
	}
	if ( Pad[ padnum ].buf[ PAD_LS_LR ] < -300 )
	{
		PadPush( padnum, PAD_LS_LEFT );
	}
	if ( Pad[ padnum ].buf[ PAD_LS_LR ] > 300 )
	{
		PadPush( padnum, PAD_LS_RIGHT );
	}

	return 0;
}

/*!
@brief 全パッドのキー入力の値を更新する。
@par   関数説明
全てのパッドの全てのボタンのキー入力の値を更新する。
@return 入力されたパッドの数。
*/
int DirectXInput::UpdatePadInput( void )
{
	//実行時までに押されたキーに対し、値を更新する。
	unsigned int i, p;
	int f = 0;
	//キーの状態を調べる。
	for ( p = 0; p < PadNum; ++p )
	{
		Pad[ p ].push = 1;
		//if( UpdatePadStatus( p ) ){ continue; }
		//if ( SUCCEEDED( hr ) )
		if ( UpdatePadStatus( p ) == 0 )
		{
			for ( i = 0; i < PAD_LS_LR; ++i )
			{
				if ( Pad[ p ].buf[ i ] > 0 )
				{
					//キーが押されている。
					if ( Pad[ p ].value[ i ] < PAD_MAXINPUT )
					{
						++Pad[ p ].value[ i ];
					}
				} else if ( Pad[ p ].value[ i ] > 0 )
				{
					//キーが放された。
					Pad[ p ].value[ i ] = -1;
				} else
				{
					Pad[ p ].value[ i ] = 0;
				}
				Pad[ p ].buf[ i ] = 0;
			}
			for ( ; i < PAD_KEY_MAX; ++i )
			{
				if ( Pad[ p ].buf )
				{
				}
			}
		} else
		{
			++f;
		}
		// Pad2Keyの状態更新。入力更新は別。
		UpdatePad2KeyStatus( p );
	}

	return f;
}

int DirectXInput::CanForceFeedback( unsigned int padnum )
{
	if ( padnum >= PAD_MAX || Pad[ padnum ].use == 0 )
	{
		return 0;
	}

	return ( Pad[ padnum ].devcaps.dwFlags & DIDC_FORCEFEEDBACK ) != 0;
}

int DirectXInput::SetPOVMode( unsigned int padnum, int povmode )
{
	if ( padnum >= PAD_MAX || Pad[ padnum ].use == 0 )
	{
		return -1;
	}

	if ( povmode )
	{
		return ( Pad[ padnum ].pov_mode = 1 );
	}

	return ( Pad[ padnum ].pov_mode = 0 );
}

#ifdef __USEFORCEFEEDBACK
int DirectXInput::CreateForceFeedback( unsigned int ffnum, int left, int right )
{

	if ( ffnum >= PAD_EFFECT_MAX )
	{
		//不正な値
		return 1;
	}

	if ( PadEffect[ ffnum ].eff == NULL )
	{
		PadEffect[ ffnum ].eff = ( struct DIEFFECT * )calloc( 1, sizeof( struct DIEFFECT ) );
		if ( PadEffect[ ffnum ].eff == NULL )
		{
			//領域確保失敗
			return 2;
		}
	} else
	{
		ZeroMemory( PadEffect[ ffnum ].eff, sizeof( struct DIEFFECT ) );
	}

	if ( PadEffect[ ffnum ].cf == NULL )
	{
		PadEffect[ ffnum ].cf = ( struct DICONSTANTFORCE * )calloc( 1, sizeof( struct DICONSTANTFORCE ) );
		if ( PadEffect[ ffnum ].cf == NULL )
		{
			//領域確保失敗
			return 3;
		}
		PadEffect[ ffnum ].envelope.dwSize = sizeof( DIENVELOPE );
		PadEffect[ ffnum ].envelope.dwAttackLevel = 0;
		PadEffect[ ffnum ].envelope.dwAttackTime = (DWORD)( 0.5 * DI_SECONDS );
		PadEffect[ ffnum ].envelope.dwFadeLevel = 0;
		PadEffect[ ffnum ].envelope.dwFadeTime = (DWORD)( 1.0 * DI_SECONDS );

		PadEffect[ ffnum ].periodic.dwMagnitude = DI_FFNOMINALMAX;
		PadEffect[ ffnum ].periodic.lOffset = 0;
		PadEffect[ ffnum ].periodic.dwPhase = 0;
		PadEffect[ ffnum ].periodic.dwPeriod = (DWORD)( 0.05 * DI_SECONDS );
	}

	PadEffect[ ffnum ].eff->dwSize = sizeof( struct DIEFFECT );
	PadEffect[ ffnum ].eff->dwFlags = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;//DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	PadEffect[ ffnum ].eff->dwDuration = (DWORD)( 2 * DI_SECONDS );//INFINITE;
	PadEffect[ ffnum ].eff->dwSamplePeriod = 0;
	PadEffect[ ffnum ].eff->dwGain = DI_FFNOMINALMAX;
	PadEffect[ ffnum ].eff->dwTriggerButton = DIJOFS_BUTTON0;//DIEB_NOTRIGGER;
	PadEffect[ ffnum ].eff->dwTriggerRepeatInterval = 0;
	PadEffect[ ffnum ].eff->cAxes = 2;//ForceFeedbackAxis;
	PadEffect[ ffnum ].eff->rgdwAxes = PadEffectAxes;
	PadEffect[ ffnum ].eff->rglDirection = PadEffectDirection[ ( left ? 2 : 0 ) + ( right ? 1 : 0 ) ];
	PadEffect[ ffnum ].eff->lpEnvelope = &( PadEffect[ ffnum ].envelope );
	PadEffect[ ffnum ].eff->cbTypeSpecificParams = sizeof( DIPERIODIC );//sizeof( struct DICONSTANTFORCE );
	PadEffect[ ffnum ].eff->lpvTypeSpecificParams = &( PadEffect[ ffnum ].periodic );//PadEffect[ ffnum ].cf;
	PadEffect[ ffnum ].eff->dwStartDelay = 0;

	return 0;
}

int DirectXInput::BeginForceFeedback( unsigned int padnum, unsigned int ffnum )
{
	if ( padnum >= PAD_MAX )
	{
		//番号が不正
		return 1;
	}
	if ( ffnum >= PAD_EFFECT_MAX )
	{
		//番号が不正
		return 2;
	}
	if ( PadEffect[ ffnum ].eff == NULL || PadEffect[ ffnum ].cf == NULL )
	{
		//何か作ってないエフェクトを使おうとしているので、デフォルトの0番を割り当てる。
		ffnum = 0;
	}

	DX_RELEASE( PadEffect[ ffnum ].die[ padnum ] );

	hr = DIDevPad[ padnum ]->CreateEffect( GUID_ConstantForce, PadEffect[ ffnum ].eff, &( PadEffect[ ffnum ].die[ padnum ] ), NULL );

	if ( PadEffect[ ffnum ].die[ padnum ] == NULL )
	{
		hr = 0;
		//エラー
		return 1;
	}

	//無限回でエフェクトは合成モード
	PadEffect[ ffnum ].die[ padnum ]->Start( 1, 0 );

	return 0;
}

int DirectXInput::EndForceFeedback( unsigned int padnum, unsigned int ffnum )
{
	if ( padnum >= PAD_MAX )
	{
		//番号が不正
		return 1;
	}
	if ( ffnum >= PAD_EFFECT_MAX )
	{
		//番号が不正
		return 2;
	}
	if ( PadEffect[ ffnum ].die[ padnum ] == NULL )
	{
		//何か作ってないエフェクトを使おうとしているので、デフォルトの0番を割り当てる。
		ffnum = 0;
	}

	PadEffect[ ffnum ].die[ padnum ]->Stop();

	return 0;
}
#endif

/*!
@brief マウスのX座標を取得する
@par   関数説明
今現在のマウスの座標を、描画範囲内を基準に返す。左上が0となり、右に行くほど値が大きくなる。
@return 座標を返す。描画領域を超えると負の数やウィンドウサイズを越える。
*/
int DirectXInput::GetMousePosX( void )
{
	POINT point;
	GetCursorPos( &point );
	ScreenToClient( GetActiveWindow(), &point );
	return point.x;
}

/*!
@brief マウスのY座標を取得する
@par   関数説明
今現在のマウスの座標を、描画範囲内を基準に返す。左上が0となり下に行くほど値が大きくなる。
@return 座標を返す。描画領域を超えると負の数やウィンドウサイズを越える。
*/
int DirectXInput::GetMousePosY( void )
{
	POINT point;
	GetCursorPos( &point );
	ScreenToClient( GetActiveWindow(), &point );
	return point.y;
}

/*!
@brief マウスのボタン入力を取得する
@par   関数説明
マウスのボタンがどれだけ押されているかを返す。
@param num 1=左クリック、2=右クリック,3=ホイールクリック, ...
@return -1 = ボタンが離れた瞬間。
@return 0 = 押されていないかマウスのボタン番号が不正。
@return 1以上 = ボタンが押されているフレーム数。
*/
int DirectXInput::GetMouseNum( unsigned int num )
{
	if ( num < MOUSE_BUTTON_MAX )
	{
		return Mouse[ num ];
	}
	return 0;
}

/*!
@brief マウスカーソルの座標を設定する
@par   関数説明
マウスカーソルの場所を変更する。
@return 0を返す。
*/
int DirectXInput::SetMousePos( int x, int y )
{
	POINT point;
	point.x = x;
	point.y = y;
	ClientToScreen( GetActiveWindow(), &point );
	SetCursorPos( point.x, point.y );
	MouseX = point.x;
	MouseY = point.y;
	return 0;
}

/*!
@brief マウスのホイール回転量を調べる
@par   関数説明
マウスのホイールの回転量を返す。
@return ホイールの回転量。
*/
int DirectXInput::GetMouseWheel( void )
{
	return MouseWheel;
}

/*!
@brief 関数実行時に押されているマウスの状態を調べる
@par   関数説明
関数を実行したときのキーボードの状態を取得し、押されている場合はバッファに状態を保存する。
このままでは入力の更新は行われない。
@return 0=成功。
@return 1=キー状態の取得が出来なかったので、入力制御を再度開始した。
*/
int DirectXInput::UpdateMouseStatus( void )
{
	int m;
	DIMOUSESTATE2 dims;
	//struct DIDEVICEOBJECTDATA od;
	unsigned long dw = 1;

	hr = DIDevMouse->GetDeviceState( sizeof( DIMOUSESTATE2 ), &dims );

	if ( SUCCEEDED( hr ) )
	{
		//MouseX = dims.lX;
		//MouseY = dims.lY;
		MouseZ = (int)dims.lZ;

		for ( m = 0; m < MOUSE_BUTTON_MAX; ++m )
		{
			if ( dims.rgbButtons[ m ] && 0x80 )
			{
				MouseBuf[ m ] = 1;
			}
		}

	} else if ( hr == DIERR_INPUTLOST )
	{
		DIDevMouse->Acquire();
		return 1;
	} else
	{
		return 2;
	}

	/*hr = DIDevMouse->GetDeviceData( sizeof( od ), &od, &dw, 0);

	if( SUCCEEDED( hr ) )
	{
	if( dw )
	{
	switch ( od.dwOfs )
	{
	case DIMOFS_Z:
	//ホイールの移動量
	MouseBuf[ MOUSE_BUTTON_MAX ] = int( od.dwData );
	break;
	}
	}
	}else
	{
	DIDevMouse->Acquire();
	return 2;
	}*/

	return 0;
}

/*!
@brief マウス入力の値を更新する
@par   関数説明
マウスの入力の値を更新する。
@return 0 = 成功。
@return 1 = UpdateMouseStatus()が失敗。
*/
int DirectXInput::UpdateMouseInput( void )
{
	int m;

	if ( UpdateMouseStatus() == 0 )
	{
		for ( m = 0; m < MOUSE_BUTTON_MAX; ++m )
		{
			if ( MouseBuf[ m ] > 0 )
			{
				if ( Mouse[ m ] < MOUSE_MAXINPUT )
				{
					++Mouse[ m ];
				}
			} else if ( Mouse[ m ] > 0 )
			{
				Mouse[ m ] = -1;
			} else
			{
				Mouse[ m ] = 0;
			}
			MouseBuf[ m ] = 0;
		}
		MouseWheel = MouseZ;
		MouseZ = 0;

		return 0;
	}

	return 1;
}

// 低レベル入力の初期化。マウスを複数取得するときなどに使う。
int DirectXInput::InitRawInput( void )
{
	unsigned long errcode;
	unsigned int i;
	// デバイス情報用構造体
	RAWINPUTDEVICELIST* deviceList;
	// 現在接続されている入力デバイスの数を取得
	unsigned int rawinputdevices = 0;


	GetRawInputDeviceList( NULL, &rawinputdevices, sizeof( RAWINPUTDEVICELIST ) );

	deviceList = new RAWINPUTDEVICELIST[ rawinputdevices ];

	// デバイス情報の取得
	if ( GetRawInputDeviceList( deviceList, &rawinputdevices, sizeof( RAWINPUTDEVICELIST ) ) != rawinputdevices )
	{
		// エラー
		errcode = GetLastError();
		errcode = 0;
		return 1;
	}

	mousenum = 0;
	DX_FREE( rawmouse );

	for ( i = 0; i < rawinputdevices; ++i )
	{
		if ( deviceList[ i ].dwType == RIM_TYPEMOUSE )
		{
			// マウスを発見。
			++mousenum;
			rawmouse = ( struct RAW_MOUSE * )realloc( rawmouse, mousenum * sizeof( struct RAW_MOUSE ) );
			rawmouse[ mousenum - 1 ].handle = deviceList[ i ].hDevice;
			rawmouse[ mousenum - 1 ].x = 0;
			rawmouse[ mousenum - 1 ].y = 0;
			rawmouse[ mousenum - 1 ].bx = 0;
			rawmouse[ mousenum - 1 ].by = 0;
			memset( (void *)rawmouse[ mousenum - 1 ].buf, 0, sizeof( int ) * 5 );
			memset( (void *)rawmouse[ mousenum - 1 ].value, 0, sizeof( int ) * 5 );
			rawmouse[ mousenum - 1 ].wheel = 0;
		}
	}

	delete[] deviceList;

	RegisterRawInputMouse();

	return 0;
}

// 低レベル入力の内マウスを有効化。
int DirectXInput::RegisterRawInputMouse( void )
{
	unsigned long errcode;

	if ( WindowHandle == NULL )
	{
		return 1;
	}

	// WM_INPUTで受け取れるように登録
	RAWINPUTDEVICE device[ 1 ];
	device[ 0 ].usUsagePage = 0x01; // マウス用の定数
	device[ 0 ].usUsage = 0x02; // マウス用の定数
	device[ 0 ].dwFlags = RIDEV_EXINPUTSINK | RIDEV_INPUTSINK;//RIDEV_NOLEGACY;//RIDEV_INPUTSINK;//0;
	device[ 0 ].hwndTarget = WindowHandle;

	if ( RegisterRawInputDevices( device, 1, sizeof( RAWINPUTDEVICE ) ) == 0 )
	{
		// エラー
		errcode = GetLastError();
		errcode = 0;
		return 2;
	}
	return 0;
}

int DirectXInput::UnregisterRawInputMouse( void )
{
	// WM_INPUTで受け取れるように登録
	RAWINPUTDEVICE device[ 1 ];
	device[ 0 ].usUsagePage = 0x01; // マウス用の定数
	device[ 0 ].usUsage = 0x02; // マウス用の定数
	device[ 0 ].dwFlags = RIDEV_REMOVE;
	device[ 0 ].hwndTarget = WindowHandle;

	if ( RegisterRawInputDevices( device, 1, sizeof( RAWINPUTDEVICE ) ) == 0 )
	{
		// エラー
		return 2;
	}
	return 0;
}

// 低レベル入力用の処理
// ウィンドウプロシージャでWM_INPUTを受け取り、そこで実行。
int DirectXInput::UpdateRawInputStatus( LPARAM lparam )
{
	unsigned int dwSize;
	RAWINPUT* raw;
	unsigned int i;

	GetRawInputData( (HRAWINPUT)lparam, RID_INPUT, NULL, &dwSize, sizeof( RAWINPUTHEADER ) );

	if ( rawsize < dwSize )
	{
		rawdata = (unsigned char *)realloc( rawdata, dwSize * sizeof( unsigned char ) );
		rawsize = dwSize;
	}

	/*  if ( rawdata == NULL)
	  {
	  // エラー
	  return 1;
	  }*/

	if ( GetRawInputData( (HRAWINPUT)lparam, RID_INPUT, rawdata, &dwSize, sizeof( RAWINPUTHEADER ) ) != dwSize )
	{
		// エラー
		return 2;
	}

	raw = (RAWINPUT*)rawdata;

	if ( raw->header.dwType == RIM_TYPEMOUSE )
	{
		for ( i = 0; i < mousenum; ++i )
		{
			if ( raw->header.hDevice == rawmouse[ i ].handle )
			{
				if ( raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE )
				{
					// マウス座標が絶対値らしい。多分ペンタブとか。
					rawmouse[ i ].x = raw->data.mouse.lLastX;
					rawmouse[ i ].y = raw->data.mouse.lLastY;
				} else
				{
					// 相対座標なので、ベースの座標に対して加算する。
					rawmouse[ i ].bx += raw->data.mouse.lLastX;
					rawmouse[ i ].by += raw->data.mouse.lLastY;
				}
				if ( raw->data.mouse.usButtonFlags > 0 )
				{
					// ボタン入力
					if ( raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN )
					{ // 1
						rawmouse[ i ].buf[ 0 ] = 1;
					}
					if ( raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP )
					{
						rawmouse[ i ].buf[ 0 ] = 0;
					}
					if ( raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN )
					{ // 2
						rawmouse[ i ].buf[ 1 ] = 1;
					}
					if ( raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP )
					{
						rawmouse[ i ].buf[ 1 ] = 0;
					}
					if ( raw->data.mouse.usButtonFlags &  RI_MOUSE_MIDDLE_BUTTON_DOWN )
					{ // 3
						rawmouse[ i ].buf[ 2 ] = 1;
					}
					if ( raw->data.mouse.usButtonFlags &  RI_MOUSE_MIDDLE_BUTTON_UP )
					{
						rawmouse[ i ].buf[ 2 ] = 0;
					}
					if ( raw->data.mouse.usButtonFlags &  RI_MOUSE_BUTTON_4_DOWN )
					{ // 4
						rawmouse[ i ].buf[ 3 ] = 1;
					}
					if ( raw->data.mouse.usButtonFlags &  RI_MOUSE_BUTTON_4_UP )
					{
						rawmouse[ i ].buf[ 3 ] = 0;
					}
					if ( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN )
					{ // 5
						rawmouse[ i ].buf[ 4 ] = 1;
					}
					if ( raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP )
					{
						rawmouse[ i ].buf[ 4 ] = 0;
					}
					if ( raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL )
					{
						rawmouse[ i ].wheel = raw->data.mouse.usButtonData;
					}
				}
				break;
			}
		}
	}

	return 0;
}

// 低レベル入力のバッファ処理。
int DirectXInput::UpdateRawInput( void )
{
	unsigned int i, j;

	for ( i = 0; i < mousenum; ++i )
	{
		for ( j = 0; j < 5; ++j )
		{
			if ( rawmouse[ i ].buf[ j ] == 0 )
			{
				// 離れている。
				if ( rawmouse[ i ].value[ j ] > 0 )
				{
					rawmouse[ i ].value[ j ] = -1;
				} else
				{
					rawmouse[ i ].value[ j ] = 0;
				}
			} else
			{
				// 押されている。
				++rawmouse[ i ].value[ j ];
			}
		}
		SetMousePos( i, rawmouse[ i ].x + rawmouse[ i ].bx, rawmouse[ i ].y + rawmouse[ i ].by );
		rawmouse[ i ].bx = 0;
		rawmouse[ i ].by = 0;
	}

	return 0;
}
// マウスの範囲。
int DirectXInput::SetMouseArea( int x, int y, int width, int height )
{
	rawmousearea_x[ 0 ] = x;
	rawmousearea_y[ 0 ] = y;
	rawmousearea_x[ 1 ] = x + width;
	rawmousearea_y[ 1 ] = y + height;
	return 0;
}

int DirectXInput::GetMouseMax( void )
{
	return mousenum;
}

int DirectXInput::GetMousePosX( unsigned int mnum )
{
	if ( mnum >= mousenum )
	{
		return 0;
	}
	return rawmouse[ mnum ].x;
}

int DirectXInput::GetMousePosY( unsigned int mnum )
{
	if ( mnum >= mousenum )
	{
		return 0;
	}
	return rawmouse[ mnum ].y;
}

int DirectXInput::SetMousePos( unsigned int mnum, int x, int y )
{
	if ( mnum >= mousenum )
	{
		return 0;
	}
	if ( x < rawmousearea_x[ 0 ] )
	{
		x = rawmousearea_x[ 0 ];
	}
	if ( y < rawmousearea_y[ 0 ] )
	{
		y = rawmousearea_y[ 0 ];
	}
	if ( rawmousearea_x[ 1 ] < x )
	{
		x = rawmousearea_x[ 1 ];
	}
	if ( rawmousearea_y[ 1 ] < y )
	{
		y = rawmousearea_y[ 1 ];
	}
	rawmouse[ mnum ].x = x;
	rawmouse[ mnum ].y = y;
	return 0;
}

int DirectXInput::GetMouseNum( unsigned int mnum, unsigned int num )
{
	if ( mnum >= mousenum || num >= 5 )
	{
		return 0;
	}
	return rawmouse[ mnum ].value[ num ];

}

int DirectXInput::SetPad2KeyInput( int enable )
{
	return ( pad2key = enable );
}

int DirectXInput::InitPad2Key( unsigned int padnum )
{
	int button;

	if ( padnum >= PAD_MAX )
	{
		return 0;
	}

	for ( button = 0; button < PAD_L + 2; ++button )
	{
		// PADとキーの対応付け。
		Pad[ padnum ].keyset[ button ] = -1;
		// バッファ初期化。
		Pad[ padnum ].keybuf[ button ] = 0;
		// ステータス初期化。
		Pad[ padnum ].keystatus[ button ] = 0;
	}

	return 0;
}

int DirectXInput::SetPad2KeyCode( unsigned int padnum, int padbutton, int keynum )
{
	if ( padnum >= PAD_MAX || Pad[ padnum ].use == 0 || padbutton > PAD_L + 1 )
	{
		return 1;
	}
	Pad[ padnum ].keyset[ padbutton ] = keynum;
	return 0;
}

int DirectXInput::SendKey( int keynum, int press )
{
	struct tagINPUT input;

	memset( &input, 0, sizeof( struct tagINPUT ) );
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = 0;//'Y';//vkey;//vkey;
	input.ki.wScan = keynum; //MapVirtualKey( input.ki.wVk, 0 );//90;//VK_LSHIFT;//;
	input.ki.dwFlags = KEYEVENTF_SCANCODE; //KEYEVENTF_EXTENDEDKEY;
	input.ki.time = 0;
	input.ki.dwExtraInfo = GetMessageExtraInfo();

	if ( press )
	{
		// 押す。
		input.ki.dwFlags = KEYEVENTF_SCANCODE;
		SendInput( 1, &input, sizeof( struct tagINPUT ) );
	} else
	{
		// 離す。
		input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;//KEYEVENTF_EXTENDEDKEY | 
		SendInput( 1, &input, sizeof( struct tagINPUT ) );
	}

	return 0;
}

int DirectXInput::SendKey( int pnum, struct tagINPUT *presskey )
{
	if ( pnum > 0 )
	{
		// 一斉に押す
		SendInput( pnum, presskey, sizeof( struct tagINPUT ) );
	}
	return 0;
}

int DirectXInput::UpdatePad2KeyStatus( unsigned int padnum )
{
	int button;

	if ( pad2key )
	{
		for ( button = 0; button < PAD_L + 2; ++button )
		{
			if ( Pad[ padnum ].value[ button ] > 0 )
			{
				++Pad[ padnum ].keybuf[ button ];
			}
		}
	}
	return 0;
}

int DirectXInput::UpdatePad2KeyInput( void )
{
	unsigned int pad, button, num = 0;
	struct tagINPUT input[ PAD_KEY_MAX ];

	if ( pad2key )
	{
		for ( pad = 0; pad < PadNum; ++pad )
		{
			for ( button = 0; button < PAD_L + 2; ++button )
			{
				if ( Pad[ pad ].keyset[ button ] >= 0 )
				{
					if ( Pad[ pad ].keybuf[ button ] > 0 )//&& Pad[ pad ].keystatus[ button ] == 0 )
					{
						// キーを押す。
						input[ num ].type = INPUT_KEYBOARD;
						input[ num ].ki.wVk = 0;
						input[ num ].ki.wScan = Pad[ pad ].keyset[ button ];
						input[ num ].ki.dwFlags = KEYEVENTF_SCANCODE; //KEYEVENTF_EXTENDEDKEY;
						if ( Pad[ pad ].keyset[ button ] >= K_KANA )
						{
							input[ num ].ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
						}
						input[ num ].ki.time = 1;
						input[ num ].ki.dwExtraInfo = GetMessageExtraInfo();
						++num;

						Pad[ pad ].keystatus[ button ] = -1;
						if ( attached == 0 )
						{
							// アタッチしてない場合はSendKeyを使わない。
							// 連打すると、ウィンドウプロシージャに処理が集中しているのか、処理に遅れが生じる。
							++Key[ Pad[ pad ].keyset[ button ] ].buf;
						}
					} else if ( Pad[ pad ].keystatus[ button ] == -1 )
					{
						// キーを離す。
						input[ num ].type = INPUT_KEYBOARD;
						input[ num ].ki.wVk = 0;
						input[ num ].ki.wScan = Pad[ pad ].keyset[ button ];
						input[ num ].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP; //KEYEVENTF_EXTENDEDKEY;
						if ( Pad[ pad ].keyset[ button ] >= K_KANA )
						{
							input[ num ].ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
						}
						input[ num ].ki.time = 1;
						input[ num ].ki.dwExtraInfo = GetMessageExtraInfo();
						++num;

						Pad[ pad ].keystatus[ button ] = 0;
					}
				}

				Pad[ pad ].keybuf[ button ] = 0;
			}
		}
	}

	if ( attached )
	{
		// アタッチしているのでSendKeyを使う。
		SendKey( num, input );
	}

	return 0;
}

int DirectXInput::AttachPad2Key( HWND hw, int attach )
{
	if ( attach )
	{
		// スレッドに対して入力のアタッチを行い、MikanのPad2Keyを使えるようにする。
		if ( AttachThreadInput( GetWindowThreadProcessId( WindowHandle, NULL ), GetWindowThreadProcessId( hw, NULL ), TRUE ) == 0 )
		{
			return -1;
		}
		SetForegroundWindow( hw );
		if ( AttachThreadInput( GetWindowThreadProcessId( WindowHandle, NULL ), GetWindowThreadProcessId( hw, NULL ), FALSE ) == 0 )
		{
			return -2;
		}
		if ( hw )
		{
			attached = 1;
		}
	} else
	{
		if ( AttachThreadInput( GetWindowThreadProcessId( WindowHandle, NULL ), GetWindowThreadProcessId( hw, NULL ), FALSE ) == 0 )
		{
			return -2;
		}
		if ( hw )
		{
			attached = 0;
			ReleaseAllPad2Key();
		}
	}
	return 0;
}

int DirectXInput::ReleaseAllPad2Key( void )
{
	unsigned int pad, button, num = 0;
	struct tagINPUT input[ PAD_KEY_MAX ];

	if ( pad2key )
	{
		for ( pad = 0; pad < PadNum; ++pad )
		{
			for ( button = 0; button < PAD_L + 2; ++button )
			{
				if ( Pad[ pad ].keyset[ button ] >= 0 )
				{
					if ( Pad[ pad ].keybuf[ button ] > 0 )//&& Pad[ pad ].keystatus[ button ] == 0 )
					{
						// キーを押しているので離す。
						input[ num ].type = INPUT_KEYBOARD;
						input[ num ].ki.wVk = 0;
						input[ num ].ki.wScan = Pad[ pad ].keyset[ button ];
						input[ num ].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP; //KEYEVENTF_EXTENDEDKEY;
						if ( Pad[ pad ].keyset[ button ] >= K_KANA )
						{
							input[ num ].ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
						}
						input[ num ].ki.time = 1;
						input[ num ].ki.dwExtraInfo = GetMessageExtraInfo();
						++num;

						Pad[ pad ].keystatus[ button ] = 0;
					}
				}

				Pad[ pad ].keybuf[ button ] = 0;
			}
		}
	}

	if ( attached )
	{
		// アタッチしているのでSendKeyを使う。
		SendKey( num, input );
	}

	return 0;
}