#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <DxErr.h>

//#define __USEFORCEFEEDBACK
#ifndef _UNLINK_LIBRARY
//リンクするライブラリの指定。VC限定。
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#endif
//#ifndef __CANNOT_USE_RAWINPUT
// 低レベル入力を使う
//#define WINVER (0x500)        // RawInput用
//#define _WIN32_WINNT (0x0501) // RawInput用
//#endif


#ifdef _MIKANDEV
#  ifdef USEMIKANLIBRARY
#    include "Dev/keymap.h"
#  else
#    include "keymap.h"
#  endif
#  ifndef _UNLINK_LIBRARY
#    ifdef _DEBUG
#      pragma comment(lib,"Dev/DirectXInputD.lib")
#    else
#      pragma comment(lib,"Dev/DirectXInput.lib")
#    endif
#  endif
#else
#  include "keymap.h"
#  ifndef _UNLINK_LIBRARY
#    ifdef _DEBUG
#      pragma comment(lib,"DirectXInputD.lib")
#    else
#      pragma comment(lib,"DirectXInput.lib")
#    endif
#  endif
#endif


#ifndef CLASSDEF_DIRECTX_INPUT
#define CLASSDEF_DIRECTX_INPUT class DirectXInput

// メモ
// RegisterTouchWindow

//キーボードのキー数
#define KB_KEYMOUNT 0xEE
//ゲームパッドのキーの数
//#define PAD_KEYMOUNT  16
//入力の最大値
#define KB_MAXINPUT 0x7FFFFFFF
//入力の最大値
#define PAD_MAXINPUT 0x7FFFFFFF
//入力の最大値
#define MOUSE_MAXINPUT 0x7FFFFFFF

#define DX_RELEASE(r) { if( r ){ ( r )->Release(); } r = NULL; }
#define DX_FREE(f)    { if( f ){ free( f ); } f = NULL; }

#define PAD_MAX 16

#define MOUSE_BUTTON_MAX 8

#define PAD_EFFECT_MAX 128

#define PAD_STICK_VALUE(v,c,m) ((int)( (c) == 0 ? ((double)(v) / (m)) * 1000 : ((double)((v)-(c)) / (c)) * 1000 ))

//定数命名規則
//PAD_ボタン名
//  ボタン名はUPやAなど。
//  基本的にボタンに対しての命名。
//PAD_*S_スティック名
//  *はLeftやRightなどわかるように。
//  スティック名は上下左右など。
//PAD_*_AXIS
//  *は軸名。XYZなど。
//  スティックなどが軸に対応している。
//  必ずしもスティックとは限らないので軸単位の名前にする。
//PAD_KEYMOUNT
//  一番最後で定義。ボタンがいくつ使えるかの判断に使う。
enum
{
	PAD_UP = 0,
	PAD_DOWN,
	PAD_LEFT,
	PAD_RIGHT,
	PAD_A,
	PAD_B,
	PAD_C,
	PAD_D,
	PAD_E,
	PAD_F,
	PAD_G,
	PAD_H,
	PAD_I,
	PAD_J,
	PAD_K,
	PAD_L,
	PAD_LS_LEFT,  //左スティックの左方向。GetPadNum用。
	PAD_LS_RIGHT, //左スティックの右方向。GetPadNum用。
	PAD_LS_UP,    //左スティックの上方向。GetPadNum用。
	PAD_LS_DOWN,  //左スティックの下方向。GetPadNum用。
	PAD_RS_LEFT,  //右スティックの左方向。GetPadNum用。
	PAD_RS_RIGHT, //右スティックの右方向。GetPadNum用。
	PAD_RS_UP,    //右スティックの上方向。GetPadNum用。
	PAD_RS_DOWN,  //右スティックの下方向。GetPadNum用。
	//ここまでが何フレーム押しているかの判別用定数。
	//ここ以降はその時のアナログ値を取得するための定数。
	PAD_LS_LR,    //左スティック左右方向。
	PAD_LS_UD,    //左スティック上下方向。
	PAD_X_AXIS,   //右スティック左右方向。XBOXコンだとX軸対応。他のだと何か1ボタンとかと対応してるようなので絶望中。
	PAD_Y_AXIS,   //右スティック上下方向。XBOXコンだとY軸対応。何か1ボタンとかと対応してるようなので絶望中。
	PAD_Z_AXIS,   //Z軸。XBOXコンだとLRトリガーが対応。
	PAD_RZ_AXIS,  //Z軸らしい。
	PAD_KEYMOUNT  //キーの数
};

enum
{
	PAD_1 = PAD_A,
	PAD_2 = PAD_B,
	PAD_3 = PAD_C,
	PAD_4 = PAD_D,
	PAD_5 = PAD_E,
	PAD_6 = PAD_F,
	PAD_7 = PAD_G,
	PAD_8 = PAD_H,
	PAD_9 = PAD_I,
	PAD_10 = PAD_J,
	PAD_11 = PAD_K,
	PAD_12 = PAD_L,
};

//XY軸は右のアナログスティックであることが多いようなので、同じ定数をそれらしい名前で宣言。
#define PAD_RS_LR PAD_X_AXIS
#define PAD_RS_UD PAD_Y_AXIS

//通常のパッドキーの最大数
#define PAD_KEY_MAX (PAD_L + 1)
// キーの状態
/*enum
{
PAD_NONE  = 0x00,
PAD_TRIG  = 0x01,
PAD_PUSH  = 0x02,
PAD_PULL  = 0x04,
};*/
#ifdef __USEFORCEFEEDBACK
struct PAD_EFFECT
{
	struct IDirectInputEffect *die[ PAD_MAX ];
	struct DICONSTANTFORCE    *cf;
	DIPERIODIC                periodic;
	DIENVELOPE                envelope;

	struct DIEFFECT           *eff;
};
#endif

BOOL CALLBACK JoystickCallback( const DIDEVICEINSTANCE* lpdi, void *obj );

struct KEY_STATUS
{
	int buf;
	int value;
};

struct PAD_STATUS
{
	int buf[ PAD_KEYMOUNT ];
	int value[ PAD_KEYMOUNT ];
	int stickcenter[ 2 ];
	int keyset[ PAD_L + 2 ];
	int keybuf[ PAD_L + 2 ];
	int keystatus[ PAD_L + 2 ];
	int use;
	int push;
	struct DIDEVCAPS devcaps;
	int pov_mode;
};

struct RAW_MOUSE
{
	HANDLE handle;
	long x, y, bx, by;
	int buf[ 5 ];
	int value[ 5 ];
	int wheel;
};

class DirectXInput
{
protected:
	struct KEY_STATUS     Key[ 256 ];
	struct PAD_STATUS     Pad[ PAD_MAX ];
	int                   attached;
	unsigned int          PadNum;
	char                  MouseBuf[ MOUSE_BUTTON_MAX ];
	int                   Mouse[ MOUSE_BUTTON_MAX ];
	int                   MouseX, MouseY, MouseZ, MouseWheel;
	int                   pad2key;

#ifdef __USEFORCEFEEDBACK
	// フォースフィードバック絡み
	unsigned long         PadEffectAxes[ 2 ];
	long                  PadEffectDirection[ 4 ][ 2 ];
	struct PAD_EFFECT     PadEffect[ PAD_EFFECT_MAX ];
#endif

	HWND                  WindowHandle;
	unsigned int          mousenum;
	struct RAW_MOUSE      *rawmouse;
	int                   rawmousearea_x[ 2 ], rawmousearea_y[ 2 ];
	unsigned int          rawsize;
	unsigned char         *rawdata;
	int                   forcefeedback;
	int                   init[ 3 ];
public:
	unsigned int          CountPadNum;
	IDirectInput8         *DIKey, *DIPad, *DIMouse;
	IDirectInputDevice8   *DIDevKey, *DIDevPad[ PAD_MAX ], *DIDevMouse;
	//  int                   PadCFF[ PAD_MAX ];

	struct DIDEVCAPS      DevCaps;
	unsigned long         ForceFeedbackAxis;
	HRESULT               hr;
	DirectXInput( void );
	virtual ~DirectXInput( void );
	virtual HWND GetWindowHandle( void );
	virtual int SetForceFeedBackMode( int enable = 1 );
	virtual int GetForceFeedBackMode( void );
	virtual int InitKeyboard( HWND wh, HINSTANCE ih );
	virtual int InitGamepad( HWND wh, HINSTANCE ih );
	virtual int InitMouse( HWND wh, HINSTANCE ih );
	virtual int SetUp( HWND wh, HINSTANCE ih );
	virtual int SetStickCenter( unsigned int padnum );
	virtual int SetStickCenter( unsigned int padnum, int center, int max );
	virtual int RecognitionGamepad( int resume = 1 );
	virtual int GetKeyNum( int keynum );
	virtual int GetKeyNumRapid( int keynum, int threshold, unsigned int interval = 0 );
	virtual int GetKeyNumRapidF( int keynum, int threshold, unsigned int interval = 0 );
	virtual int GetKeyWhichButton( void );
	virtual int UpdateKeyStatus( void );
	virtual int UpdateKeyInput( void );

	virtual int GetPadNum( unsigned int padnum, int button );
	virtual int GetPadStick( unsigned int padnum, int button );
	virtual int GetPadMount( void );
	virtual int GetPadWhichButton( unsigned int padnum );
	virtual int PadPush( unsigned int padnum, int button );
	virtual int UpdatePadStatus( unsigned int padnum );
	virtual int UpdatePadInput( void );
	virtual int CanForceFeedback( unsigned int padnum );
	virtual int SetPOVMode( unsigned int padnum, int povmode );//POV tte Point of View Hat rasiiyo.

#ifdef __USEFORCEFEEDBACK
	virtual int CreateForceFeedback( unsigned int ffnum, int left = 1, int right = 1 );
	virtual int BeginForceFeedback( unsigned int padnum, unsigned int ffnum );
	virtual int EndForceFeedback( unsigned int padnum, unsigned int ffnum );
#endif

	//  virtual int GetMousePos( int *x=NULL, int *y=NULL, int *z=NULL );
	virtual int GetMousePosX( void );
	virtual int GetMousePosY( void );
	virtual int GetMouseNum( unsigned int num );
	//  virtual int GetMouseState( int n );
	virtual int SetMousePos( int x, int y );
	virtual int GetMouseWheel( void );
	virtual int UpdateMouseStatus( void );
	virtual int UpdateMouseInput( void );

	// 低レベル入力の初期化。マウスを複数取得するときなどに使う。
	virtual int InitRawInput( void );
	// 低レベル入力の内マウスを有効化。
	virtual int RegisterRawInputMouse( void );
	virtual int UnregisterRawInputMouse( void );
	// 低レベル入力用の処理。ウィンドウプロシージャ用。
	virtual int UpdateRawInputStatus( LPARAM lparam );
	// 低レベル入力のバッファ処理。
	virtual int UpdateRawInput( void );
	// マウスの範囲。
	virtual int SetMouseArea( int x, int y, int width, int height );
	virtual int GetMouseMax( void );
	virtual int GetMousePosX( unsigned int mnum );
	virtual int GetMousePosY( unsigned int mnum );
	virtual int SetMousePos( unsigned int mnum, int x, int y );
	virtual int GetMouseNum( unsigned int mnum, unsigned int num );

	virtual int SetPad2KeyInput( int enable = 0 );
	virtual int InitPad2Key( unsigned int padnum );
	virtual int SetPad2KeyCode( unsigned int padnum, int padbutton, int keynum = -1 );
	virtual int SendKey( int keynum, int press );
	virtual int SendKey( int pnum, struct tagINPUT *presskey );
	virtual int UpdatePad2KeyStatus( unsigned int padnum );
	virtual int UpdatePad2KeyInput( void );
	virtual int AttachPad2Key( HWND hw, int attach = TRUE );
	virtual int ReleaseAllPad2Key( void );
};

#endif
