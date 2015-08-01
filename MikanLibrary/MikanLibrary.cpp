/*//���C��
#define _CRTDBG_MAP_ALLOC
//#define _CRTDBG_MAPALLOC
#include <stdlib.h>
#include <crtdbg.h>*/

#include "MikanLibrary.h"
#include <Winuser.h>

#include "../MikanMovie/MikanMovieManagement.h"
//#include <mmsystem.h>

//���J�N���X
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
//����J�N���X
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
	case WM_INPUT: // �}�E�X�Ȃǂ̒჌�x�����́B
		_MikanInput->UpdateRawInputStatus( lParam );
		break;
		//case WM_TOUCH: // �^�b�`�̒჌�x�����́B
		//break;
	case WM_ACTIVATE:
		if ( LOWORD( wParam ) == WA_INACTIVE )
		{
			//��A�N�e�B�u��
			_MikanSystem->SetWindowState( 0 );
		} else
		{
			//�A�N�e�B�u��
			_MikanSystem->SetWindowState( 1 );
		}
		break;
	case WM_SYSCOMMAND:
		if ( wParam == SC_SCREENSAVE && _MikanSystem->GetPermitScreenSaver() == 0 )
		{
			// �X�N���[���Z�[�o�[�̋N����j�~�B
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
	case WM_CLOSE://�E�B���h�E��������
	case WM_DESTROY://�E�B���h�E���j�����ꂽ��
		//PostQuitMessage( 0 );
		_MikanSystem->SetGameLoop();
		break;
		//  case WM_PAINT://�E�B���h�E���ĕ`�悳��鎞
		//    return 0;
	case WM_SIZE://�E�B���h�E�̃T�C�Y���ς������
		switch ( wParam )
		{
		case SIZE_RESTORED://�T�C�Y�ύX
			break;
		case SIZE_MAXIMIZED://�E�B���h�E�ő剻
			break;
		}
		break;
	case WM_CREATE://�E�B���h�E�����ꂽ��
		//������ӂŃX�N���[���I����ʂ��o���̂�����B
		//�f�o�C�X�R���e�L�X�g�̎擾
		_MikanWindow->wd.Screen = CreateCompatibleDC( GetDC( hWnd ) );
		//_MikanWindow->wd.WindowParent = hWnd;
		_MikanWindow->wd.WindowHandle = hWnd;
		DragAcceptFiles( hWnd, TRUE );
		break;
	case WM_DROPFILES:
		_MikanWindow->ReceiveDragFiles( (HDROP)wParam );
		break;
	}
	//���ꂪ�Ȃ��ƃE�B���h�E�������ł��Ȃ��B
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//�X���b�h���s�p�֐�
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

//�t���[���҂��֐��̎���
int WaitSleep( double fps, double *getfps )
{
	static unsigned int beforetime = timeGetTime(), frames = 0;
	unsigned int nowtime, progress, idealtime;
	Sleep( 0 );
	//windows���N�����Ă���̃~���b���擾
	nowtime = timeGetTime();
	//�i�s����
	progress = nowtime - beforetime;
	//FPS�̌v�Z
	*getfps = (double)( frames ) / (double)(progress)* 1000.0;
	//�O�̎��Ԃ���1�b�ȏ�o����
	if ( progress >= 1000 )
	{
		//1�b����
		progress = 0;//-= 1000;
		//�O�̎��Ԃ̍X�V
		beforetime = nowtime;
		//�t���[�����̏�����
		frames = 0;
	}

	++frames;

	//�t���[��������O�̎��Ԃ���ǂꂾ���҂����v�Z����
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
	//windows���N�����Ă���̃~���b���擾
	nowtime = timeGetTime();
	//�i�s����
	progress = nowtime - beforetime;
	//FPS�̌v�Z
	*getfps = (double)( frames ) / (double)(progress)* 1000.0;
	//�O�̎��Ԃ���1�b�ȏ�o����
	if ( progress >= 1000 )
	{
		//1�b����
		progress = 0;//-= 1000;
		//�O�̎��Ԃ̍X�V
		beforetime = nowtime;
		//�t���[�����̏�����
		frames = 0;
	}

	++frames;

	//�t���[��������O�̎��Ԃ���ǂꂾ���҂����v�Z����
	idealtime = (int)( frames * ( 1000.0 / fps ) );
	nowtime += idealtime - progress - 1;
	//�i�s���Ԃ��O�̎��Ԃ���҂��Ԃ��傫���ꍇ�X���[�v����
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
@brief   _MikanSystem�N���X�B�Q�[���ɕK�v�ȏ������s���B
@author  Hiroki
@version 1.0
*/

/*!
@brief _MikanSystem(Mikan���C�u����)�̃R���X�g���N�^
@par   �֐�����
�e���J�E����J�N���X�̐�����ϐ��������Ȃǂ��s���B
*/
MikanLibrary::MikanLibrary( void )
{
	//�|�W�V�����̐ݒ�
	position = MIKAN_POSITION_BEGIN;
	//����J�N���X
	_MikanWindow = new Window();
	_MikanDraw = new DirectXDraw9();
	_MikanMovie = new MikanMovieManagement_( 4 );
	_MikanInput = new DirectXInput();
#ifndef MIKAN_NO_SOUND
	_MikanSound = new ThaleiaSound();
#endif
	_MikanNet = new MikanNetwork();
	_MikanFile = new FileIOManagement();

	//���J�N���X
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

	// �Q�[�����[�v�J�n�B
	SetGameLoop( 1 );
}

/*!
@brief _MikanSystem(Mikan���C�u����)�̃f�X�g���N�^
@par   �֐�����
���������e�N���X��j������B
*/
MikanLibrary::~MikanLibrary( void )
{
	//�|�W�V�����̐ݒ�
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
@brief �������֐�
@par   �֐�����
�E�B���h�E�̐ݒ�Ȃǂ̏��������s���B
@param hInstance WinMain�֐��ɓn������1�����B
@return 0��Ԃ��B
*/
int MikanLibrary::Init( HINSTANCE hInstance )
{
	int i;
	WNDCLASSEX *wcx;

	//���������҂��̐ݒ�
	vsync = 0;

	//�҂��֐��̐ݒ�
	SetWaitFunction();

	SetAfterDeviceLostFunction( NULL );

	wcx = &( _MikanWindow->wd.wcx );

	this->hInstance = hInstance;

	_MikanWindow->InitWindowClassEx( hInstance );
	_MikanWindow->SetWindowProc( ::WindowProc );

	_MikanWindow->SetScreenSize( 640, 480 );

	//fps�����l
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
	//�X���b�h
	tdata = ( struct ThreadData * )calloc( thread_max, sizeof( struct ThreadData ) );

	return 0;
}

//�Q�[�����[�v��ݒ�
int MikanLibrary::SetGameLoop( int loop )
{
	return ( gameloop = loop );
}

/*!
@brief �������֐�
@par   �֐�����
�E�B���h�E�̐ݒ�Ȃǂ̏��������s���B
@param hInstance WinMain�֐��ɓn������1�����B
@return 0��Ԃ��B
*/
HINSTANCE MikanLibrary::GetInstance( void )
{
	return hInstance;
}

/*!
@brief Mikan���C�u�����̎�ȏ���
@par   �֐�����
Mikan���C�u�����ł̏������s����BWinMain�֐����炱�ꂪ�Ă΂�A�������ꂪWinMain�֐��ƂȂ��ē��삵�Ă���B
@param hInstance WinMain�֐��ɓn������1�����B
@param hPrevInstance WinMain�֐��ɓn������2�����B
@param lpCmdLine WinMain�֐��ɓn������3�����B
@param nCmdShow WinMain�֐��ɓn������4�����B
@return 1=�E�B���h�E�N���X�̓o�^���s
@return 2=�E�B���h�E�����Ɏ��s�B
@return 3=DirectX�̏��������o�����Ɏ��s�B
@return ���̑�=���b�Z�[�W�̃p�����[�^�[��Ԃ��B
*/
int MikanLibrary::WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow )
{
	unsigned int i;
	MSG msg = {};
	//�E�B���h�E�n���h��
	HWND myhwnd = NULL;
	//RECT�\����
	RECT myrect;
	//  unsigned long atom;

	if ( Init( hInstance ) )
	{//�������Ɏ��s�����̂ŏI��
		return 1;
	}

	//�|�W�V�����̐ݒ�
	position = MIKAN_POSITION_SYSTEMINIT;
	//���[�U�[
	SystemInit();
	//�|�W�V�����̐ݒ�
	position = MIKAN_POSITION_LIBLARYINIT;

	SecondInit();

	//���������҂��̐ݒ�
	_MikanDraw->SetVsyncWait( vsync );

	// �l�b�g���[�N�̏�����
	_MikanNet->SecondInit();

	//�E�B���h�E�N���X�̓o�^
	//  atom = RegisterClassEx( &( _MikanWindow->GetWindowClassEx() ) );
	//  if( atom == NULL )
	if ( _MikanWindow->RegistClassEx() )
	{
		//�o�^���s
		return 1;
	}

	if ( _MikanWindow->MakeWindow( nCmdShow ) || _MikanWindow->wd.Screen == NULL )
	{
		//�E�B���h�E�����Ȃ������̂ŏI��
		return 2;
	}

	//�f�X�N�g�b�v�̃E�B���h�E�n���h���擾
	myhwnd = GetDesktopWindow();
	if ( myhwnd && GetClientRect( myhwnd, &myrect ) )
	{
		//�E�B���h�E�|�W�V�����̕ύX
		MikanWindow->SetPositionXY( ( myrect.right - MikanWindow->GetWindowWidth() ) / 2,
			( myrect.bottom - MikanWindow->GetWindowHeight() ) / 2 );
	}

	//Direct3D
	//�E�B���h�E�n���h���̃Z�b�g
	_MikanDraw->SetWindowHandle( _MikanWindow->GetWindowHandle() );
	//DirectX�̏�����
	if ( _MikanDraw->InitD3D() )
	{
		//DirectX�̏��������ł��Ȃ������̂ŏI��
		return 3;
	}
	_MikanDraw->InitD3DObject();

	( ( class MikanMovieManagement_ * )_MikanMovie )->SecondInit( _MikanDraw );
	//DirectInput
	//���͂̃Z�b�g�A�b�v
	_MikanInput->SetUp( _MikanWindow->GetWindowHandle(), hInstance );

#ifndef MIKAN_NO_SOUND
	//DirectSound
	//������
	_MikanSound->Initialize( _MikanWindow->GetWindowHandle() );
#endif
	//�|�W�V�����̐ݒ�
	position = MIKAN_POSITION_USERINIT;

	waitsleep = 0;
	DrawBegin();

	//���[�U�[
	UserInit();

	WaitNextFrame( 1 );

	//windows���N�����Ă���̃~���b���擾
	beforetime = timeGetTime();
	//�t���[�����̏�����
	frames = 0;

	//�|�W�V�����̐ݒ�
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

		//�Q�[������
		if ( devicelost || ( _MikanSystem->IsActive() == 0 && GetInactiveWindow() == 1 ) )
		{
			//���炩�̌����œ��삪�ł��Ȃ��B

			if ( devicelost )
			{
				//�f�o�C�X���X�g
				devicelost = _MikanDraw->RecoverFromDeviceLost();
				if ( devicelost == 0 && AfterDeviceLost )
				{
					( *AfterDeviceLost )( );
				}
			} else// if( GetInactiveWindow() == 0 )
			{
				//�E�B���h�E����A�N�e�B�u
				//��������A�N�e�B�u���ɓ�����~�߂Ȃ��ݒ肾�ƁA�����ɂ͂��Ȃ��B
				Sleep( 10 );
			}
		} else
		{
			//����ɓ��삵�Ă���B

			DEGUB_TIME( 2 );
			//�Q�[���p�b�h���͂̍X�V
			_MikanInput->UpdatePadInput();
			// �Q�[���p�b�h��Pad2Key�̍X�V�B
			_MikanInput->UpdatePad2KeyInput();
			//�L�[���͂̍X�V
			_MikanInput->UpdateKeyInput();
			//�}�E�X���͂̍X�V
			_MikanInput->UpdateMouseInput();
			// �჌�x�����͂̍X�V�B
			_MikanInput->UpdateRawInput();
			DEGUB_TIME( 3 );
			//�`��J�n
			DrawBegin();
			// MikanMovie����DirectX��D���Ԃ��B
			_MikanMovie->Reset();
			//���[�U�[
			if ( MainLoop() )
			{
				break;
			}
			DEGUB_TIME( 4 );
			//�`��I��
			//���̃t���[���܂őҋ@
			WaitNextFrame( 1 );
			DEGUB_TIME( 6 );
#ifdef _DEBUG
			tm[ 10 ] = tm[ 4 ] - tm[ 3 ];
			tm[ 11 ] = tm[ 5 ] - tm[ 4 ];
			tm[ 12 ] = tm[ 6 ] - tm[ 5 ];
#endif
		}
	}

	//�|�W�V�����̐ݒ�
	position = MIKAN_POSITION_CLEANUP;
	//���[�U�[
	CleanUp();

	position = MIKAN_POSITION_RELEASE;

	// �X���b�h�S��~
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
	// ���b�N�S��~
	for ( i = 0 ; i < LOCK_HANDLE_MAX ; ++i )
	{
		ReleaseLock( i );
	}
	if ( tdata )
	{
		free( tdata ); tdata = NULL;
	}

	//DirectX�̑|��
	_MikanDraw->CleanupD3DObject();
	_MikanDraw->CleanupD3D();
	_MikanDraw->CleanupFont();
#ifndef MIKAN_NO_SOUND
	MikanSound->UnloadAll();
#endif

	return msg.wParam;
}

/*!
@brief FPS��ݒ肷��B
@par   �֐�����
FPS��ݒ肷��B
@param fps �ݒ肷��FPS�B1�b�Ԃɉ�����s���邩�B�ȗ������ꍇ60.0�B
@return �ݒ肵���l�B
*/
double MikanLibrary::SetFPS( double fps )
{
	return ( this->fps = fps );
}

/*!
@brief FPS���擾����B
@par   �֐�����
����FPS���v�����ĕԂ��B
@return ���݂�FPS�B�v�������l���o��B�����ƒႢ�l���o���ꍇ�͏����������N�����Ă���B
*/
double MikanLibrary::GetFPS( void )
{
	return getfps;//frames > 0 ? (double)(start-beforetime) / frames : 0.0;//( process < (1000.0 / fps) ) ? 60 : process / ( 1000.0 / fps );
}

/*!
@brief ���̃t���[���܂ő҂�
@par   �֐�����
���s����ƑO�̎��ԂȂǂƔ�r���A�ǂꂾ���x�߂΂悢�����v�Z���A���̎��Ԃ̕�����Sleep����B
����ő҂��Ƃɂ���āAFPS������s�Ȃ��Ă���B
@return Sleep��������(�~���b)�B
*/
int MikanLibrary::WaitNextFrame( int draw )
{
	// ��ʂɕ`��
	if ( draw )
	{
		DrawEnd();
	}
	DEGUB_TIME( 5 );
	// �҂��֐��ɏ����𓊂���
	( *Wait )( fps, &getfps );

	return 0;
}

//�҂��֐��̐ݒ�
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
@brief �f�o�C�X���X�g���N�����Ă��邩�ǂ������ׂ�
@par   �֐�����
�����݃f�o�C�X���X�g���N�����Ă��邩�ǂ����𒲂ׂ�B
�������A���s����MainLoop���̃v���O�����͎��s����Ȃ����߁A�ʎ�i��p���Ȃ���ΈӖ��������B
@return 0=����B
@return ���̑�=�f�o�C�X���X�g��
*/
int MikanLibrary::IsDeviceLost( void )
{
	return devicelost;
}

/*!
@brief ���݂̏����ʒu�𒲂ׂ�
@par   �֐�����
���݂̏����ʒu�𒲂ׂ�B�����ʒu��MIKAN_POSITION_�Ŏn�܂�B
@return MIKAN_POSITION_BEGIN=�J�n
@return MIKAN_POSITION_SYSTEMINIT=SystemInit���s��
@return MIKAN_POSITION_LIBLARYINIT=���C�u�����̏�������
@return MIKAN_POSITION_USERINIT=UserInit���s��
@return MIKAN_POSITION_MAINLOOP=MainLoop���s��
@return MIKAN_POSITION_CLEANUP=CleanUp���s��
@return MIKAN_POSITION_RELEASE=���C�u�����̃��\�[�X�����ƂȂ�
@return MIKAN_POSITION_END=�Ō�̃N���X�J����
*/
int MikanLibrary::GetMikanPosition( void )
{
	return position;
}

/*!
@brief ���������҂��̐ݒ�
@par   �֐�����
����������҂��ǂ����̐ݒ�B
@param vs 0�ȊO�Ő��������҂����A0�ő҂��Ȃ��B-1�͓���ȕԂ�l�Ȃ̂Ŏg��Ȃ����Ƃ������߂�B
@return -1 ���s�B
@return ���̑� �ݒ肵���l���Ԃ��Ă���B
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
@brief �E�B���h�E���[�h�̕ύX
@par   �֐�����
�E�B���h�E���t���X�N���[��or�E�B���h�E�Ő؂�ւ���B
@param fullscreen_on 1=�t���X�N���[�����[�h�A0=�E�B���h�E���[�h�B
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
		// ����ȊO�B
		if ( fullscreen_on )
		{
			// �E�B���h�E�ݒ�̃o�b�N�A�b�v�B
			_MikanWindow->BackUpWindow();
		} else
		{
			// �E�B���h�E�ݒ�̕��A�B
			_MikanWindow->RepairWindow();
		}
		// �t���X�N���[�����[�h�ɂ���B
		_MikanWindow->SetWindowStyle( WT_FULLSCREEN );
		_MikanDraw->SetFullScreenMode( fullscreen_on );

		// ���A��ƁB
		_MikanDraw->RecoverFromDeviceLost( 1 );
	}

	return 0;
}

/*!
@brief �E�B���h�E��A�N�e�B�u���̓���ݒ�
@par   �֐�����
�E�B���h�E����A�N�e�B�u�ɂȂ����Ƃ��̋����ɂ��Đݒ肷��B
@param flag 0�Ŕ�A�N�e�B�u��������𑱂��A����ȊO�̒l�Ŕ�A�N�e�B�u���ɓ�����~�߂�B
@return 0 ��A�N�e�B�u���ɃQ�[���𓮍��������B
@return 1 ��A�N�e�B�u���ɃQ�[���̓�����~�߂�B
*/
int MikanLibrary::SetInactiveWindow( int flag )
{
	return ( view = flag );
}

/*!
@brief �E�B���h�E��A�N�e�B�u���̓�����擾
@par   �֐�����
�E�B���h�E����A�N�e�B�u�ɂȂ����Ƃ��̋����ɂ��ĕԂ��B
@return 0 ��A�N�e�B�u���ɃQ�[���̓����������B
@return 1 ��A�N�e�B�u���ɃQ�[���̓�����~�߂�B
*/
int MikanLibrary::GetInactiveWindow( void )
{
	return view;
}

/*!
@brief �E�B���h�E�̏�Ԃ�ݒ�
@par   �֐�����
�E�B���h�E���A�N�e�B�u����A�N�e�B�u���̐ݒ���s���B
@param flag 0�Ŕ�A�N�e�B�u���B����ȊO�ŃA�N�e�B�u���B
@return 0 ��A�N�e�B�u��
@return 1 �A�N�e�B�u��
*/
int MikanLibrary::SetWindowState( int flag )
{
	return ( active = flag );
}

/*!
@brief �E�B���h�E���A�N�e�B�u���ǂ������ׂ�
@par   �֐�����
�E�B���h�E�����݃A�N�e�B�u�������łȂ����𒲂ׂ�B
@return 0 ��A�N�e�B�u
@return 1 �A�N�e�B�u
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

//�X���b�h�𗧂ĂĊ֐������s����
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
	//�p�����[�^�[�̗L���Ŏ��s�֐���ω�������B
	//���Ƃ������A�L���X�g�Ƃ����낢��s��B

	if ( td )
	{
		if ( td->type )
		{
			//int �֐���( void * vparam ); �Ƃ����֐����󂯎���Ď��s
			ret = ( ( int( *)( void * vparam ) )( td->thread_func ) )( td->param );
		} else
		{
			//int �֐���( void ); �Ƃ����֐����󂯎���Ď��s
			ret = ( ( int( *)( void ) )( td->thread_func ) )( );
		}
	}

	return ret;
}

//���Ă��X���b�h��j������
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

// �X���b�h�̃n���h�����擾����B
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
	// �O��MainLoop�I���ԍۂɃ��b�N����A���̂܂܂����ă��b�N���ꂽ�ꍇ��MainLoop�ł̏������x���B���\��������B
	// ����̉���̂��߁A�`����I���ۂɃ��b�N����Ă����drawable��2�ȏ�ɂȂ�B
	// �ςݏd�Ȃ����I���������Ƃ����C����(drawable)��2�ȏ�̏ꍇ�͂��̋C�����𑸏d���I��������B
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
	// ClearScreen�͕`��J�n����I���܂ł̊Ԏ��s�񐔂𑫂�������B
	// ClearScreen�̎��s�ꏊ�Ɖ񐔂͑�̓����ӏ��ɂȂ�̂ŁA�{�l�͉�����s�������Ń^�C�~���O��m�邱�Ƃ��ł���B��ˁH
	// ����̊ȈՔŁB
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
	// �`�惍�b�N�҂�
	// �I���������C������`���邽�߁Adrawable�𑫂�������B
	while ( waitsleep && drawable++ )
	{
		Sleep( 1 );
	}
	drawable = 0;
	devicelost = _MikanDraw->End();

	return 0;
}




