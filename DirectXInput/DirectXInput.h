#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <DxErr.h>

//#define __USEFORCEFEEDBACK
#ifndef _UNLINK_LIBRARY
//�����N���郉�C�u�����̎w��BVC����B
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#endif
//#ifndef __CANNOT_USE_RAWINPUT
// �჌�x�����͂��g��
//#define WINVER (0x500)        // RawInput�p
//#define _WIN32_WINNT (0x0501) // RawInput�p
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

// ����
// RegisterTouchWindow

//�L�[�{�[�h�̃L�[��
#define KB_KEYMOUNT 0xEE
//�Q�[���p�b�h�̃L�[�̐�
//#define PAD_KEYMOUNT  16
//���͂̍ő�l
#define KB_MAXINPUT 0x7FFFFFFF
//���͂̍ő�l
#define PAD_MAXINPUT 0x7FFFFFFF
//���͂̍ő�l
#define MOUSE_MAXINPUT 0x7FFFFFFF

#define DX_RELEASE(r) { if( r ){ ( r )->Release(); } r = NULL; }
#define DX_FREE(f)    { if( f ){ free( f ); } f = NULL; }

#define PAD_MAX 16

#define MOUSE_BUTTON_MAX 8

#define PAD_EFFECT_MAX 128

#define PAD_STICK_VALUE(v,c,m) ((int)( (c) == 0 ? ((double)(v) / (m)) * 1000 : ((double)((v)-(c)) / (c)) * 1000 ))

//�萔�����K��
//PAD_�{�^����
//  �{�^������UP��A�ȂǁB
//  ��{�I�Ƀ{�^���ɑ΂��Ă̖����B
//PAD_*S_�X�e�B�b�N��
//  *��Left��Right�Ȃǂ킩��悤�ɁB
//  �X�e�B�b�N���͏㉺���E�ȂǁB
//PAD_*_AXIS
//  *�͎����BXYZ�ȂǁB
//  �X�e�B�b�N�Ȃǂ����ɑΉ����Ă���B
//  �K�������X�e�B�b�N�Ƃ͌���Ȃ��̂Ŏ��P�ʂ̖��O�ɂ���B
//PAD_KEYMOUNT
//  ��ԍŌ�Œ�`�B�{�^���������g���邩�̔��f�Ɏg���B
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
	PAD_LS_LEFT,  //���X�e�B�b�N�̍������BGetPadNum�p�B
	PAD_LS_RIGHT, //���X�e�B�b�N�̉E�����BGetPadNum�p�B
	PAD_LS_UP,    //���X�e�B�b�N�̏�����BGetPadNum�p�B
	PAD_LS_DOWN,  //���X�e�B�b�N�̉������BGetPadNum�p�B
	PAD_RS_LEFT,  //�E�X�e�B�b�N�̍������BGetPadNum�p�B
	PAD_RS_RIGHT, //�E�X�e�B�b�N�̉E�����BGetPadNum�p�B
	PAD_RS_UP,    //�E�X�e�B�b�N�̏�����BGetPadNum�p�B
	PAD_RS_DOWN,  //�E�X�e�B�b�N�̉������BGetPadNum�p�B
	//�����܂ł����t���[�������Ă��邩�̔��ʗp�萔�B
	//�����ȍ~�͂��̎��̃A�i���O�l���擾���邽�߂̒萔�B
	PAD_LS_LR,    //���X�e�B�b�N���E�����B
	PAD_LS_UD,    //���X�e�B�b�N�㉺�����B
	PAD_X_AXIS,   //�E�X�e�B�b�N���E�����BXBOX�R������X���Ή��B���̂��Ɖ���1�{�^���Ƃ��ƑΉ����Ă�悤�Ȃ̂Ő�]���B
	PAD_Y_AXIS,   //�E�X�e�B�b�N�㉺�����BXBOX�R������Y���Ή��B����1�{�^���Ƃ��ƑΉ����Ă�悤�Ȃ̂Ő�]���B
	PAD_Z_AXIS,   //Z���BXBOX�R������LR�g���K�[���Ή��B
	PAD_RZ_AXIS,  //Z���炵���B
	PAD_KEYMOUNT  //�L�[�̐�
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

//XY���͉E�̃A�i���O�X�e�B�b�N�ł��邱�Ƃ������悤�Ȃ̂ŁA�����萔������炵�����O�Ő錾�B
#define PAD_RS_LR PAD_X_AXIS
#define PAD_RS_UD PAD_Y_AXIS

//�ʏ�̃p�b�h�L�[�̍ő吔
#define PAD_KEY_MAX (PAD_L + 1)
// �L�[�̏��
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
	// �t�H�[�X�t�B�[�h�o�b�N����
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

	// �჌�x�����͂̏������B�}�E�X�𕡐��擾����Ƃ��ȂǂɎg���B
	virtual int InitRawInput( void );
	// �჌�x�����͂̓��}�E�X��L�����B
	virtual int RegisterRawInputMouse( void );
	virtual int UnregisterRawInputMouse( void );
	// �჌�x�����͗p�̏����B�E�B���h�E�v���V�[�W���p�B
	virtual int UpdateRawInputStatus( LPARAM lparam );
	// �჌�x�����͂̃o�b�t�@�����B
	virtual int UpdateRawInput( void );
	// �}�E�X�͈̔́B
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
