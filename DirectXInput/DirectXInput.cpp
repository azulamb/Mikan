#include "DirectXInput.h"

// �U���@�\���ڂ��������A�������s����B

/*!
@class   DirectXInput DirectXInput.h "DirectXInput.h"
@brief   _MikanInput�N���X�BDirectX��p�������͂��s���B
@author  Hiroki
@version 1.0
*/

int CALLBACK JoystickCallback( const DIDEVICEINSTANCE* lpdi, void *obj )
{
	CLASSDEF_DIRECTX_INPUT *dic = (CLASSDEF_DIRECTX_INPUT*)obj;
	HRESULT hr;

	//�W���C�X�e�B�b�N�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
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
@brief �R���X�g���N�^
@par   �֐�����
�ϐ��̏��������s��
*/
DirectXInput::DirectXInput( void )
{
	int p, i;
	// Pad2Key�������B
	SetPad2KeyInput();

	attached = 0;
	for ( p = 0; p < PAD_MAX; ++p )
	{
		//�S�Ẵp�b�h������������
		//g_pDIDevPad[ i ] = NULL;
		Pad[ p ].use = 0;
		//�ǂ̃Q�[���p�b�h�������ꂽ���̃t���O������������
		Pad[ p ].push = 0;
		// Pad2Key�̏�����
		InitPad2Key( p );
		//PadCFF[ p ] = 0;
		for ( i = 0; i < PAD_KEYMOUNT; ++i )
		{
			Pad[ p ].value[ i ] = Pad[ p ].buf[ i ] = 0;
			Pad[ p ].pov_mode = 0;
		}
	}
	//�o�b�t�@�̏�����
	for ( i = 0; i < 256; ++i )
	{
		Key[ i ].value = Key[ i ].buf = 0;
	}
	//�}�E�X�{�^���̏�����
	for ( i = 0; i < MOUSE_BUTTON_MAX; ++i )
	{
		Mouse[ i ] = MouseBuf[ i ] = 0;
	}

	//�p�b�h�̐��̏�����
	CountPadNum = PadNum = 0;
	//�}�E�X�̕ϐ��̏�����
	MouseX = MouseY = MouseZ = MouseWheel = 0;
	//�E�B���h�E�n���h���̏�����
	WindowHandle = NULL;

	// �}�E�X�̐�(RawInput)
	mousenum = 0;
	rawmouse = NULL;
	SetMouseArea( 0, 0, 3000, 3000 );
	rawsize = 0;
	rawdata = NULL;

	//���̑�DirectXInput���݂̏�����
	DIKey = NULL;
	DIPad = NULL;
	DIMouse = NULL;

	init[ 0 ] = init[ 1 ] = init[ 2 ] = 0;
	SetForceFeedBackMode( 0 );
}

/*!
@brief �f�X�g���N�^
@par   �֐�����
�I�u�W�F�N�g�̉���ȂǁB
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
			//�ꕔ�Q�[���p�b�h�̊J�����ɖ��ȃG���[���o�邽�ߕs�{�ӂȂ���R�����g�A�E�g�B
			//���ۂɂ͉�����ׂ��B
			//�G���[���ʂ̃T���v���B
			//MikanSample2010.exe �� 0x009c3635 �Ńn���h������Ă��Ȃ���O���������܂���: 0xC0000005: �ꏊ 0xfeeeff6a ��ǂݍ��ݒ��ɃA�N�Z�X�ᔽ���������܂����B
			//RFVibraPS02.dll�̂���ꏊ�ŗ����Ă�炵���B
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
	// �Q�[���p�b�h�̏������O�B
	// �������ゾ�����������񏉊�������H
	if ( init[ 1 ] == 0 )
	{
		forcefeedback = enable;
	}

	return 0;
}

/*!
@brief �L�[�{�[�h���͂̏�����
@par   �֐�����
�L�[�{�[�h���͂̂��߂̃f�o�C�X�����Ȃǂ��s���B
@param wh �E�B���h�E�n���h���B
@param ih �C���X�^���X�n���h���B
@return 0=�����B
@return 1=DirectInput�̍쐬�Ɏ��s�B
@return 2=�f�o�C�X�I�u�W�F�N�g�̍쐬�Ɏ��s�B
@return 3=�f�[�^�E�t�H�[�}�b�g�̐ݒ�Ɏ��s�B
@return 4=�������[�h�̐ݒ�Ɏ��s�B
*/
int DirectXInput::InitKeyboard( HWND wh, HINSTANCE ih )
{
	// DirectInput�̍쐬
	hr = DirectInput8Create( ih, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DIKey, NULL );
	if ( FAILED( hr ) )
	{
		return 1;
	}

	// �f�o�C�X�E�I�u�W�F�N�g���쐬
	hr = DIKey->CreateDevice( GUID_SysKeyboard, &DIDevKey, NULL );
	if ( FAILED( hr ) )
	{
		return 2;
	}

	// �f�[�^�E�t�H�[�}�b�g��ݒ�
	hr = DIDevKey->SetDataFormat( &c_dfDIKeyboard );
	if ( FAILED( hr ) )
	{
		return 3;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = DIDevKey->SetCooperativeLevel( wh, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	if ( FAILED( hr ) )
	{
		return 4;
	}
	// ���͐���J�n
	DIDevKey->Acquire();
	init[ 0 ] = 1;
	return 0;
}

/*!
@brief �p�b�h�̏�����
@par   �֐�����
�p�b�h�̏��������s���B
@param wh �E�B���h�E�n���h���B
@param ih �C���X�^���X�n���h���B
@return 0=�����B
@return 1=DirectInput8�̐����Ɏ��s�B
@return 2=�p�b�h��T�����߂�Callback�֐��̐ݒ�Ɏ��s�B
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
		//�t�H�[�X�t�B�[�h�o�b�N�֌W�̏�����
		//�G�t�F�N�g�̎�
		PadEffectAxes[ 0 ] = DIJOFS_X;
		PadEffectAxes[ 1 ] = DIJOFS_Y;
		//�G�t�F�N�g�̑Ώۏ�����
		//���E�h��Ȃ��B
		PadEffectDirection[ 0 ][ 0 ] = 0;
		PadEffectDirection[ 0 ][ 1 ] = 0;
		//�E�����h���B
		PadEffectDirection[ 1 ][ 0 ] = 0;
		PadEffectDirection[ 1 ][ 1 ] = 1;
		//�������h���B
		PadEffectDirection[ 2 ][ 0 ] = 1;
		PadEffectDirection[ 2 ][ 1 ] = 0;
		//���E���h���B
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

	//�W���C�X�e�B�b�N��T��
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
	//�Q�[���p�b�h�̔F��
	RecognitionGamepad( 0 );

	//RecognitionGamepad();

	init[ 1 ] = 1;
	return 0;
}

/*!
@brief �}�E�X�̏�����
@par   �֐�����
�}�E�X�̏��������s���B
@param wh �E�B���h�E�n���h���B
@param ih �C���X�^���X�n���h���B
@return 0=�����B
@return 1=DirectInput8�̐����Ɏ��s�B
@return 2=�p�b�h��T�����߂�Callback�֐��̐ݒ�Ɏ��s�B
*/
int DirectXInput::InitMouse( HWND wh, HINSTANCE ih )
{
	DIPROPDWORD dipdw;

	//DIrectInput�I�u�W�F�N�g�̍쐬
	hr = DirectInput8Create( ih, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DIMouse, NULL );
	if ( FAILED( hr ) )
	{
		return 1;
	}

	//�f�o�C�X�E�I�u�W�F�N�g���쐬
	hr = DIMouse->CreateDevice( GUID_SysMouse, &DIDevMouse, NULL );
	if ( FAILED( hr ) )
	{
		return 2;
	}

	//�f�[�^�`���̐ݒ�
	hr = DIDevMouse->SetDataFormat( &c_dfDIMouse2 );
	if ( FAILED( hr ) )
	{
		return 3;
	}

	//�}�E�X�̓���̐ݒ�
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

	// �f�[�^
	//dipdw.dwData            = DIPROPAXISMODE_REL; //���Βl�����[�h
	//dipdw.dwData            = DIPROPAXISMODE_ABS; //��Βl�����[�h
	//���
	//hr = DIDevMouse->SetProperty( DIPROP_AXISMODE, &dipdw.diph );

	//�o�b�t�@�T�C�Y
	dipdw.dwData = 1000;
	//�ψي
	DIDevMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
	if ( FAILED( hr ) )
	{
		return 4;
	}

	//�L����
	DIDevMouse->Acquire();
	init[ 2 ] = 1;
	return 0;
}

/*!
@brief �L�[�{�[�h�ƃp�b�h�̏�����
@par   �֐�����
������InitKeyboard��InitGamepad���Ăяo���Ă���B
@param wh �E�B���h�E�n���h���B
@param ih �C���X�^���X�B
@return 0��Ԃ��B
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

	//�A�i���O(�X�e�B�b�N��g���K�[)�̒l���-1000�`1000�ɂ��邽�߂̑O�����B
	//���������Ă����̏����͂ǂ����邩�����B
	//�����ݍ��X�e�B�b�N�̏����l��0�Œl�悪-1000�`1000�̃R���g���[���[��
	//�����l��32767�Œl�悪0�`65535�̃R���g���[���[(XBOX)���m�F����Ă���B

	//���A�i���O�X�e�B�b�N�̏㉺�����̏����l��^�񒆂Ɖ���B
	//�����������@�Ȃ��H
	center = js.lY;

	if ( center == 0 )
	{
		//������0�̎��͍ő�l��1000�̃R���g���[���[�������̂ł���ɐݒ�B
		max = 1000;
	} else
	{
		max = center * 2;
		if ( max + 2 > 0xffff )
		{
			//������65535/2���炢�̒l�������炾�������ő�l��65535�Ȃ̂ł���ɂ���B
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
@brief �p�b�h�̔F��
@par   �֐�����
�p�b�h�̍ĔF�����s���B
@param resume 0�ŏ��߂���F�����������B�ȗ������ꍇ��1�Ɠ����Œǉ��F�����s���B
@return 0=�����B
@return 1=�f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s�B
@return 2=���[�h�ݒ�Ɏ��s�B
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
		//�f�[�^�t�H�[�}�b�g��ݒ�
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

		//���[�h��ݒ�
		//ret = pDInputDevice->SetCooperativeLevel(hwnd,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		//    hr = g_pDIDevPad[ i ]->SetCooperativeLevel( WindowHandle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND );

		if ( GetForceFeedBackMode() )
		{
			// �t�H�[�X�t�B�[�h�o�b�N�ɕK�v�Ȕr���������[�h�������H�̐ݒ�B
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
		//    Pad[ i ].devcaps.dwFlags;//DIDC_FORCEFEEDBACK=�t�H�[�X�t�B�[�h�o�b�N�\���ǂ���

		// ���̒l�͈̔͂�ݒ�
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
@brief ����̃L�[���͂̏�Ԃ��擾����B
@par   �֐�����
����̃L�[�ɑ΂�����͂��ǂ��Ȃ��Ă��邩�𒲂ׂĕԂ��B
@param keynum ���ׂ����L�[�̒l�B�ڂ�����keymap.h�ɋL���ꂽ�L�[�}�b�v�Q�ƁB
@return -1=���ꂽ�B
@return 0=�����Ă��Ȃ��B
@return 0�ȏ�=�����Ă���B
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
@brief �֐����s���ɉ�����Ă���L�[�̏�Ԃ𒲂ׂ�B
@par   �֐�����
�֐������s�����Ƃ��̃L�[�{�[�h�̏�Ԃ��擾���A������Ă���ꍇ�̓o�b�t�@�ɏ�Ԃ�ۑ�����B
���̂܂܂ł͓��͂̍X�V�͍s���Ȃ��B
@return 0=�����B
@return 1=�L�[��Ԃ̎擾���o���Ȃ������̂ŁA���͐�����ēx�J�n�����B
*/
int DirectXInput::UpdateKeyStatus( void )
{
	int i;
	char KeyState[ 256 ] = {};

	//�L�[�̏�Ԃ𒲂ׂ�B
	hr = DIDevKey->GetDeviceState( sizeof( KeyState ), KeyState );
	if ( SUCCEEDED( hr ) )
	{
		for ( i = 0; i < KB_KEYMOUNT; ++i )
		{
			if ( KeyState[ i ] & 0x80 )
			{
				//�L�[��������Ă���B
				Key[ i ].buf = 1;
			}
		}
		return 0;
	} else
	{
		// ���͐���J�n
		if ( DIDevKey )
		{
			DIDevKey->Acquire();
		}
	}
	return 1;
}

/*!
@brief ���s���܂łɉ����ꂽ�L�[�ɑ΂��A�l���X�V����B
@par   �֐�����
UpdateKeyStatus�ɂ���Ď擾���ꂽ�o�b�t�@�����ɁA�L�[���͂��X�V����B
���̊֐��͂����ԓ�(�t���[���Ȃ�)�ň�x�������s����̂��D�܂����B
���̊֐��͒P�̂�UpdateKeyStatus���s���̂ŁA���O�Ɏg���K�v�͂Ȃ��B
@return 0=�����B
@return 1=UpdateKeyState�����s�B
*/
int DirectXInput::UpdateKeyInput( void )
{
	int i;
	//�L�[�̏�Ԃ𒲂ׂ�B
	//UpdateKeyStatus();
	//if ( SUCCEEDED( hr ) )
	if ( UpdateKeyStatus() == 0 )
	{
		for ( i = 0; i < KB_KEYMOUNT; ++i )
		{
			if ( Key[ i ].buf > 0 )
			{
				//�L�[��������Ă���B
				if ( Key[ i ].value < KB_MAXINPUT )
				{
					++Key[ i ].value;
				}
			} else if ( Key[ i ].value > 0 )
			{
				//�L�[�������ꂽ�B
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
@brief �w�肳�ꂽ�p�b�h�̎w��{�^���̓��͏�Ԃ��擾����B
@par   �֐�����
�F�����ꂽ����p�b�h�̎w��{�^���̓��͏�Ԃ𒲂ׂĕԂ��B
@param padnum �p�b�h�ԍ��B
@param button �㉺���E��PAD_UP,PAD_DOWN,PAD_LEFT,PAD_RIGHT�ƂȂ�A���̑��{�^����1�Ԃ���PAD_A,PAD_B��
���t�����APAD_L�܂ő��݂���B
@return -1=���ꂽ�B
@return 0=�����Ă��Ȃ��B
@return 0�ȏ�=�����Ă���B
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
@brief �w�肳�ꂽ�p�b�h�̃X�e�B�b�N���͂��擾����B
@par   �֐�����
�F�����ꂽ����p�b�h�̃A�i���O�X�e�B�b�N�̓��͏�Ԃ𒲂ׂĕԂ��B
�l�̓A�i���O�̐��̒l�ŕ�����DirectX�̍��W�n�Ɠ����ŁAPAD_UD�͉����������A����������BPAD_LR�͉E���������A�����������B
@param padnum �p�b�h�ԍ��B
@param button �A�i���O�X�e�B�b�N�̏㉺������PAD_UD�ŁA���E������PAD_LR�Ŏ擾�ł���B
@return 0=�����Ă��Ȃ��B�������͖����B
@return ����ȊO=�X�e�B�b�N�̓��́B�X����B
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
@brief �p�b�h�̔F����
@par   �֐�����
���ݔF������Ă���p�b�h�̐���Ԃ��B
@return ���݂ɔF�����Ă���p�b�h�̐��B
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
@brief �w��p�b�h�̎w��{�^���̃o�b�t�@���X�V����B
@par   �֐�����
UpdatePadState�ɂă{�^���������Ă��邱�Ƃ����m�������ɌĂяo�����B
�o�b�t�@��1������d���Ƃǂ̃p�b�h��������Ă��邩�̃t���O�𗧂Ă�d�����s���B
���z���͂Ȃǂɂ͎g���邪�A������ނ�݂Ɏg�����Ƃ��֎~����B
@param padnum �p�b�h�ԍ��B
@param button GetPadNum�Q�ƁB
@return �o�b�t�@�ɓ��ꂽ�l(1)��Ԃ��B
*/
int DirectXInput::PadPush( unsigned int padnum, int button )
{
	Pad[ padnum ].push = 1;
	return ( Pad[ padnum ].buf[ button ] = 1 );
}

/*!
@brief ���ݎw��p�b�h�̏�Ԃ��擾����B
@par   �֐�����
���݂ǂ̃p�b�h�̂ǂ̃{�^����������Ă��邩�𔻒f���A�o�b�t�@�Ƀt���O�𗧂Ă�B
@param padnum �p�b�h�ԍ�
@return 0=�����B
@return 1=�p�b�h�ԍ����s�����A�p�b�h���g���Ă��Ȃ��B
@return 2=�p�b�h�̓��͏������o���Ă��Ȃ��B
@return 3=�X�e�[�^�X�̎擾�Ɏ��s�B
*/
int DirectXInput::UpdatePadStatus( unsigned int padnum )
{
	//���̊֐��́A���̓o�b�t�@�ɒl������B
	//���̓o�b�t�@����͒l�ɓ����̂�UpdatePadInput�B
	HRESULT hr;
	struct DIJOYSTATE js;

	if ( padnum >= PAD_MAX || Pad[ padnum ].use == 0 )
	{
		return 1;
	}

	// �擾����
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
	// �擾
	hr = DIDevPad[ padnum ]->GetDeviceState( sizeof( DIJOYSTATE ), &js );
	if ( FAILED( hr ) )
	{
		return 3;
	}

	// �擾���

	//�����̓A�i���O�l�Ƃ��ăo�b�t�@�ɂ��߂�B
	//���X�e�B�b�N
	Pad[ padnum ].buf[ PAD_LS_UD ] = PAD_STICK_VALUE( js.lY, Pad[ padnum ].stickcenter[ 0 ], Pad[ padnum ].stickcenter[ 1 ] );
	Pad[ padnum ].buf[ PAD_LS_LR ] = PAD_STICK_VALUE( js.lX, Pad[ padnum ].stickcenter[ 0 ], Pad[ padnum ].stickcenter[ 1 ] );
	//�E�X�e�B�b�N�BXY��
	//�R���g���[���[�ɂ���ĐF�X�B
	Pad[ padnum ].buf[ PAD_X_AXIS ] = PAD_STICK_VALUE( js.lRx, Pad[ padnum ].stickcenter[ 0 ], Pad[ padnum ].stickcenter[ 1 ] );
	Pad[ padnum ].buf[ PAD_Y_AXIS ] = PAD_STICK_VALUE( js.lRy, Pad[ padnum ].stickcenter[ 0 ], Pad[ padnum ].stickcenter[ 1 ] );
	//Z���B
	//�R���g���[���[�ɂ���ĐF�X�B
	//XBOX�R������L�g���K�[(��)��R�g���K�[(��)�̍��v�l�B�܂藼�����������ŉ�����0�B
	Pad[ padnum ].buf[ PAD_Z_AXIS ] = PAD_STICK_VALUE( js.lZ, Pad[ padnum ].stickcenter[ 0 ], Pad[ padnum ].stickcenter[ 1 ] );
	Pad[ padnum ].buf[ PAD_RZ_AXIS ] = PAD_STICK_VALUE( js.lRz, Pad[ padnum ].stickcenter[ 0 ], Pad[ padnum ].stickcenter[ 1 ] );

	if ( Pad[ padnum ].pov_mode )
	{
		//POV�L���ƍl���āA�\���L�[�̓��͂̂ݎ󂯕t����B
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
		//�\���L�[�̓��͂��󂯕t����B
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
		//�A�i���O�X�e�B�b�N�̓��͂��㉺���E�Ƃ��ĔF�����Ď󂯕t����B
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

	//�Q�[���p�b�h�̃{�^��
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

	//�A�i���O�X�e�B�b�N�̃t���[���P�ʂŉ��Z����l�B
	//�X�e�B�b�N�̃A�i���O�l��GetPadStick�Ŏ擾�ł��邪�AGetPadNum�Ŏ擾�ł��Ȃ��̂�
	//�����Ŏ擾�ł���悤�ɂ���B
	//�܂�X�e�B�b�N�̃A�i���O�l�ł͂Ȃ��A�X�e�B�b�N�����t���[���ǂ̕����ɌX���Ă��邩�������B
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
@brief �S�p�b�h�̃L�[���͂̒l���X�V����B
@par   �֐�����
�S�Ẵp�b�h�̑S�Ẵ{�^���̃L�[���͂̒l���X�V����B
@return ���͂��ꂽ�p�b�h�̐��B
*/
int DirectXInput::UpdatePadInput( void )
{
	//���s���܂łɉ����ꂽ�L�[�ɑ΂��A�l���X�V����B
	unsigned int i, p;
	int f = 0;
	//�L�[�̏�Ԃ𒲂ׂ�B
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
					//�L�[��������Ă���B
					if ( Pad[ p ].value[ i ] < PAD_MAXINPUT )
					{
						++Pad[ p ].value[ i ];
					}
				} else if ( Pad[ p ].value[ i ] > 0 )
				{
					//�L�[�������ꂽ�B
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
		// Pad2Key�̏�ԍX�V�B���͍X�V�͕ʁB
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
		//�s���Ȓl
		return 1;
	}

	if ( PadEffect[ ffnum ].eff == NULL )
	{
		PadEffect[ ffnum ].eff = ( struct DIEFFECT * )calloc( 1, sizeof( struct DIEFFECT ) );
		if ( PadEffect[ ffnum ].eff == NULL )
		{
			//�̈�m�ێ��s
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
			//�̈�m�ێ��s
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
		//�ԍ����s��
		return 1;
	}
	if ( ffnum >= PAD_EFFECT_MAX )
	{
		//�ԍ����s��
		return 2;
	}
	if ( PadEffect[ ffnum ].eff == NULL || PadEffect[ ffnum ].cf == NULL )
	{
		//��������ĂȂ��G�t�F�N�g���g�����Ƃ��Ă���̂ŁA�f�t�H���g��0�Ԃ����蓖�Ă�B
		ffnum = 0;
	}

	DX_RELEASE( PadEffect[ ffnum ].die[ padnum ] );

	hr = DIDevPad[ padnum ]->CreateEffect( GUID_ConstantForce, PadEffect[ ffnum ].eff, &( PadEffect[ ffnum ].die[ padnum ] ), NULL );

	if ( PadEffect[ ffnum ].die[ padnum ] == NULL )
	{
		hr = 0;
		//�G���[
		return 1;
	}

	//������ŃG�t�F�N�g�͍������[�h
	PadEffect[ ffnum ].die[ padnum ]->Start( 1, 0 );

	return 0;
}

int DirectXInput::EndForceFeedback( unsigned int padnum, unsigned int ffnum )
{
	if ( padnum >= PAD_MAX )
	{
		//�ԍ����s��
		return 1;
	}
	if ( ffnum >= PAD_EFFECT_MAX )
	{
		//�ԍ����s��
		return 2;
	}
	if ( PadEffect[ ffnum ].die[ padnum ] == NULL )
	{
		//��������ĂȂ��G�t�F�N�g���g�����Ƃ��Ă���̂ŁA�f�t�H���g��0�Ԃ����蓖�Ă�B
		ffnum = 0;
	}

	PadEffect[ ffnum ].die[ padnum ]->Stop();

	return 0;
}
#endif

/*!
@brief �}�E�X��X���W���擾����
@par   �֐�����
�����݂̃}�E�X�̍��W���A�`��͈͓�����ɕԂ��B���オ0�ƂȂ�A�E�ɍs���قǒl���傫���Ȃ�B
@return ���W��Ԃ��B�`��̈�𒴂���ƕ��̐���E�B���h�E�T�C�Y���z����B
*/
int DirectXInput::GetMousePosX( void )
{
	POINT point;
	GetCursorPos( &point );
	ScreenToClient( GetActiveWindow(), &point );
	return point.x;
}

/*!
@brief �}�E�X��Y���W���擾����
@par   �֐�����
�����݂̃}�E�X�̍��W���A�`��͈͓�����ɕԂ��B���オ0�ƂȂ艺�ɍs���قǒl���傫���Ȃ�B
@return ���W��Ԃ��B�`��̈�𒴂���ƕ��̐���E�B���h�E�T�C�Y���z����B
*/
int DirectXInput::GetMousePosY( void )
{
	POINT point;
	GetCursorPos( &point );
	ScreenToClient( GetActiveWindow(), &point );
	return point.y;
}

/*!
@brief �}�E�X�̃{�^�����͂��擾����
@par   �֐�����
�}�E�X�̃{�^�����ǂꂾ��������Ă��邩��Ԃ��B
@param num 1=���N���b�N�A2=�E�N���b�N,3=�z�C�[���N���b�N, ...
@return -1 = �{�^�������ꂽ�u�ԁB
@return 0 = ������Ă��Ȃ����}�E�X�̃{�^���ԍ����s���B
@return 1�ȏ� = �{�^����������Ă���t���[�����B
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
@brief �}�E�X�J�[�\���̍��W��ݒ肷��
@par   �֐�����
�}�E�X�J�[�\���̏ꏊ��ύX����B
@return 0��Ԃ��B
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
@brief �}�E�X�̃z�C�[����]�ʂ𒲂ׂ�
@par   �֐�����
�}�E�X�̃z�C�[���̉�]�ʂ�Ԃ��B
@return �z�C�[���̉�]�ʁB
*/
int DirectXInput::GetMouseWheel( void )
{
	return MouseWheel;
}

/*!
@brief �֐����s���ɉ�����Ă���}�E�X�̏�Ԃ𒲂ׂ�
@par   �֐�����
�֐������s�����Ƃ��̃L�[�{�[�h�̏�Ԃ��擾���A������Ă���ꍇ�̓o�b�t�@�ɏ�Ԃ�ۑ�����B
���̂܂܂ł͓��͂̍X�V�͍s���Ȃ��B
@return 0=�����B
@return 1=�L�[��Ԃ̎擾���o���Ȃ������̂ŁA���͐�����ēx�J�n�����B
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
	//�z�C�[���̈ړ���
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
@brief �}�E�X���͂̒l���X�V����
@par   �֐�����
�}�E�X�̓��͂̒l���X�V����B
@return 0 = �����B
@return 1 = UpdateMouseStatus()�����s�B
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

// �჌�x�����͂̏������B�}�E�X�𕡐��擾����Ƃ��ȂǂɎg���B
int DirectXInput::InitRawInput( void )
{
	unsigned long errcode;
	unsigned int i;
	// �f�o�C�X���p�\����
	RAWINPUTDEVICELIST* deviceList;
	// ���ݐڑ�����Ă�����̓f�o�C�X�̐����擾
	unsigned int rawinputdevices = 0;


	GetRawInputDeviceList( NULL, &rawinputdevices, sizeof( RAWINPUTDEVICELIST ) );

	deviceList = new RAWINPUTDEVICELIST[ rawinputdevices ];

	// �f�o�C�X���̎擾
	if ( GetRawInputDeviceList( deviceList, &rawinputdevices, sizeof( RAWINPUTDEVICELIST ) ) != rawinputdevices )
	{
		// �G���[
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
			// �}�E�X�𔭌��B
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

// �჌�x�����͂̓��}�E�X��L�����B
int DirectXInput::RegisterRawInputMouse( void )
{
	unsigned long errcode;

	if ( WindowHandle == NULL )
	{
		return 1;
	}

	// WM_INPUT�Ŏ󂯎���悤�ɓo�^
	RAWINPUTDEVICE device[ 1 ];
	device[ 0 ].usUsagePage = 0x01; // �}�E�X�p�̒萔
	device[ 0 ].usUsage = 0x02; // �}�E�X�p�̒萔
	device[ 0 ].dwFlags = RIDEV_EXINPUTSINK | RIDEV_INPUTSINK;//RIDEV_NOLEGACY;//RIDEV_INPUTSINK;//0;
	device[ 0 ].hwndTarget = WindowHandle;

	if ( RegisterRawInputDevices( device, 1, sizeof( RAWINPUTDEVICE ) ) == 0 )
	{
		// �G���[
		errcode = GetLastError();
		errcode = 0;
		return 2;
	}
	return 0;
}

int DirectXInput::UnregisterRawInputMouse( void )
{
	// WM_INPUT�Ŏ󂯎���悤�ɓo�^
	RAWINPUTDEVICE device[ 1 ];
	device[ 0 ].usUsagePage = 0x01; // �}�E�X�p�̒萔
	device[ 0 ].usUsage = 0x02; // �}�E�X�p�̒萔
	device[ 0 ].dwFlags = RIDEV_REMOVE;
	device[ 0 ].hwndTarget = WindowHandle;

	if ( RegisterRawInputDevices( device, 1, sizeof( RAWINPUTDEVICE ) ) == 0 )
	{
		// �G���[
		return 2;
	}
	return 0;
}

// �჌�x�����͗p�̏���
// �E�B���h�E�v���V�[�W����WM_INPUT���󂯎��A�����Ŏ��s�B
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
	  // �G���[
	  return 1;
	  }*/

	if ( GetRawInputData( (HRAWINPUT)lparam, RID_INPUT, rawdata, &dwSize, sizeof( RAWINPUTHEADER ) ) != dwSize )
	{
		// �G���[
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
					// �}�E�X���W����Βl�炵���B�����y���^�u�Ƃ��B
					rawmouse[ i ].x = raw->data.mouse.lLastX;
					rawmouse[ i ].y = raw->data.mouse.lLastY;
				} else
				{
					// ���΍��W�Ȃ̂ŁA�x�[�X�̍��W�ɑ΂��ĉ��Z����B
					rawmouse[ i ].bx += raw->data.mouse.lLastX;
					rawmouse[ i ].by += raw->data.mouse.lLastY;
				}
				if ( raw->data.mouse.usButtonFlags > 0 )
				{
					// �{�^������
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

// �჌�x�����͂̃o�b�t�@�����B
int DirectXInput::UpdateRawInput( void )
{
	unsigned int i, j;

	for ( i = 0; i < mousenum; ++i )
	{
		for ( j = 0; j < 5; ++j )
		{
			if ( rawmouse[ i ].buf[ j ] == 0 )
			{
				// ����Ă���B
				if ( rawmouse[ i ].value[ j ] > 0 )
				{
					rawmouse[ i ].value[ j ] = -1;
				} else
				{
					rawmouse[ i ].value[ j ] = 0;
				}
			} else
			{
				// ������Ă���B
				++rawmouse[ i ].value[ j ];
			}
		}
		SetMousePos( i, rawmouse[ i ].x + rawmouse[ i ].bx, rawmouse[ i ].y + rawmouse[ i ].by );
		rawmouse[ i ].bx = 0;
		rawmouse[ i ].by = 0;
	}

	return 0;
}
// �}�E�X�͈̔́B
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
		// PAD�ƃL�[�̑Ή��t���B
		Pad[ padnum ].keyset[ button ] = -1;
		// �o�b�t�@�������B
		Pad[ padnum ].keybuf[ button ] = 0;
		// �X�e�[�^�X�������B
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
		// �����B
		input.ki.dwFlags = KEYEVENTF_SCANCODE;
		SendInput( 1, &input, sizeof( struct tagINPUT ) );
	} else
	{
		// �����B
		input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;//KEYEVENTF_EXTENDEDKEY | 
		SendInput( 1, &input, sizeof( struct tagINPUT ) );
	}

	return 0;
}

int DirectXInput::SendKey( int pnum, struct tagINPUT *presskey )
{
	if ( pnum > 0 )
	{
		// ��Ăɉ���
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
						// �L�[�������B
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
							// �A�^�b�`���ĂȂ��ꍇ��SendKey���g��Ȃ��B
							// �A�ł���ƁA�E�B���h�E�v���V�[�W���ɏ������W�����Ă���̂��A�����ɒx�ꂪ������B
							++Key[ Pad[ pad ].keyset[ button ] ].buf;
						}
					} else if ( Pad[ pad ].keystatus[ button ] == -1 )
					{
						// �L�[�𗣂��B
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
		// �A�^�b�`���Ă���̂�SendKey���g���B
		SendKey( num, input );
	}

	return 0;
}

int DirectXInput::AttachPad2Key( HWND hw, int attach )
{
	if ( attach )
	{
		// �X���b�h�ɑ΂��ē��͂̃A�^�b�`���s���AMikan��Pad2Key���g����悤�ɂ���B
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
						// �L�[�������Ă���̂ŗ����B
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
		// �A�^�b�`���Ă���̂�SendKey���g���B
		SendKey( num, input );
	}

	return 0;
}