#ifndef MikanVersion
#define MikanVersion "2.1.5"
#define MikanFirstFPS 60.0
#define LOCK_HANDLE_MAX 16
#define LOCK_NAME_MAX 16

#ifndef MIKAN_NO_SOUND
//#define MIKAN_NO_SOUND
#endif

//windows�A�v���P�[�V��������邽�߂ɕK�v
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

//���C��
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

//Mikan���C�u�����̖{�̂̌^�錾
typedef class MikanLibrary   CLASSDEF_SYSTEM;

//���p�Ȃ��̂��B�������N���X�̌^�錾
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

//�ÓI���C�u�����̃����N
//VC�̂�
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

//�X���b�h����p�f�[�^
struct ThreadData
{
	unsigned int num;
	HANDLE handle;
	void * thread_func;//int (*thread_func)( void * vparam );
	void * param;
	unsigned long threadid;
	int type;
};

//�҂��֐�
enum
{
	WAIT_BUSY = 0,
	WAIT_SLEEP,
	WAIT_NONE
};

//���݂̏����ʒu
enum
{
	MIKAN_POSITION_BEGIN = 0,   // ���s�J�n
	MIKAN_POSITION_SYSTEMINIT,  // SystemInit���s��
	MIKAN_POSITION_LIBLARYINIT, // SystemInit����UserInit�܂ł̊�
	MIKAN_POSITION_USERINIT,    // UserInit���s��
	MIKAN_POSITION_MAINLOOP,    // MainLoop���s��
	MIKAN_POSITION_CLEANUP,     // CleanUp���s��
	MIKAN_POSITION_RELEASE,     // CleanUp�I����
	MIKAN_POSITION_END,         // ���s�I��
};

//�E�B���h�E�����O�Ƀ��[�U�[�����s����
extern void SystemInit( void );
//�������I����Ƀ��[�U�[�����s����
extern void UserInit( void );
//1�b��60�񃆁[�U�[�����s����
extern int MainLoop( void );
//MainLoop�I�����1�x�������[�U�[�����s����
extern void CleanUp( void );

//�R�[���o�b�N�֐�
LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//WinMain�֐�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow );
//�X���b�h���s�֐�
unsigned long WINAPI ThreadFunc( void * param );

//MikanLibrary�N���X
class MikanLibrary
{
protected:
	int gameloop;
	int windowactive;
	int devicelost;
	double fps;
	double getfps;
	//�E�B���h�E�`��t���O
	int view;
	// �E�B���h�E�̏��
	int active;
	//�C���X�^���X
	HINSTANCE hInstance;
	//�t���[��
	unsigned int frames;
	unsigned int beforetime;
	unsigned int process;
	//���݂̏����ʒu
	int position;
	//���������̒l
	int vsync;
	//�X���b�h����p�f�[�^
	unsigned int thread_max;
	struct ThreadData *tdata;
	//�҂��֐�
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
	//�Q�[�����[�v��ݒ�
	virtual int SetGameLoop( int loop = 0 );
	//�C���X�^���X���擾����
	virtual HINSTANCE GetInstance( void );
	//FPS��ݒ肷��
	virtual double SetFPS( double fps = MikanFirstFPS );
	//FPS���擾����
	virtual double GetFPS( void );
	//���̃t���[���܂ő҂�
	virtual int WaitNextFrame( int draw = 1 );
	//�҂��֐��̐ݒ�
	virtual int SetWaitFunction( int WaitFunc( double fps, double *getfps ) );
	virtual int SetWaitFunction( int type = WAIT_SLEEP );

	//�f�o�C�X���X�g����s����֐��̐ݒ�
	virtual int SetAfterDeviceLostFunction( int( *func )( void ) );

	//Mikan���C�u������Main�֐�
	virtual int WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow );

	//�f�o�C�X���X�g�̔����𒲂ׂ�
	virtual int IsDeviceLost( void );

	//���݂̏����ʒu�𒲂ׂ�
	virtual int GetMikanPosition( void );

	//���������҂��̐ݒ�
	virtual int SetVsyncWait( int vs = 0 );

	// �E�B���h�E���[�h�̕ύX�B
	virtual int SetFullScreen( int fullscreen_on = 1 );
	//�E�B���h�E��A�N�e�B�u���̓���ݒ�
	virtual int SetInactiveWindow( int flag = 1 );

	//�E�B���h�E��A�N�e�B�u���̓�����擾
	virtual int GetInactiveWindow( void );

	//�E�B���h�E�̏�Ԃ�ݒ�
	virtual int SetWindowState( int flag = 1 );

	//�E�B���h�E���A�N�e�B�u���ǂ������ׂ�
	virtual int IsActive( void );

	// �X�N���[���Z�[�o�[�̉ғ����������ǂ���()�B
	virtual int SetPermitScreenSaver( int permit = 1 );
	virtual int GetPermitScreenSaver( void );
	//�X���b�h�𗧂ĂĊ֐������s����
	virtual int RunThread( unsigned int tnum, int( *thread_func )( void * vparam ), void *param = NULL );
	//�X���b�h�𗧂ĂĊ֐������s����
	virtual int RunThread( unsigned int tnum, int( *thread_func )( void ) );
	//�X���b�h�Ŏ��s���Ă��钆�g
	virtual int RunThread_( struct ThreadData *td );
	//���Ă��X���b�h��j������
	virtual int StopThread( unsigned int tnum );
	// �X���b�h�̃n���h�����擾����B
	virtual void * GetThreadHandle( unsigned int tnum );
	// �w��X���b�h���I������܂őҋ@����B
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

	// DLL����B

	// DLL��ǂݍ��ށB
	//virtual HINSTANCE * LoadDLL( const char *dllfile );
	// DLL���J������B
	//virtual int UnloadDLL( HINSTANCE *dllhandle );
	// ��荞��DLL��S�ĊJ������B
	//virtual int UnloadDLLAll( void );
};

//�B�������V�X�e���N���X
class MikanSystemPub
{
public:
	MikanSystemPub( void );
	virtual ~MikanSystemPub( void );
	// !FPS��ݒ肷��B
	/*!
	  \param fps �ݒ肷��FPS�̒l(�f�t�H���g60.0)�B
	  */
	virtual double SetFPS( double fps = MikanFirstFPS );
	// !FPS���擾����B
	/*!
	  \retval �l ���݂�FPS�B
	  */
	virtual double GetFPS( void );
	// !���̃t���[���܂ő҂B
	/*!
	  \param draw �`�揈�����s�Ȃ��Ă���҂�(�f�t�H���g)�B0�ő҂����B
	  */
	virtual int WaitNextFrame( int draw = 1 );
	// !�҂��֐��̐ݒ�B
	/*!
	  \param type �p�ӂ��ꂽ�҂��֐����w�肷��(WAIT_BUSY[�f�t�H���g], WAIT_SLEEP, WAIT_NONE)�B
	  */
	virtual int SetWaitFunction( int type = WAIT_BUSY );
	// !�f�o�C�X���X�g�̔����𒲂ׂ�B
	/*!
	  \retval 0 �f�o�C�X���X�g�͔������Ă��Ȃ��B
	  \retval ���̑� �f�o�C�X���X�g�������B
	  */
	virtual int IsDeviceLost( void );
	// !���������҂��̐ݒ�B
	/*!
	  \param vs 0=���������̖�����, ����ȊO=�L����
	  */
	virtual int SetVsyncWait( int vs = 0 );
	// �E�B���h�E���[�h�̕ύX�B
	/*!
	  \param flag 1=�t���X�N���[�����[�h�ɂ���(�f�t�H���g), 0=�E�B���h�E���[�h�ɂ���B
	  */
	virtual int SetFullScreen( int fullscreen_on = 1 );  // !�E�B���h�E��A�N�e�B�u���̓���ݒ�B
	// !�E�B���h�E����A�N�e�B�u���̓����ݒ肷��B
	/*!
	  \param flag 1=��A�N�e�B�u���Ɏ��s���~�߂�(�f�t�H���g), 0=��A�N�e�B�u�������s�B
	  */
	virtual int SetInactiveWindow( int flag = 1 );
	// !�X���b�h�𗧂ĂĊ֐������s����B
	/*!
	  \param tnum �X���b�h�ԍ��B
	  \param thread_func ���s����֐��Bint �֐���( void *param )�Ƃ����֐��̊֐����B
	  \param param ���s����֐��ɗ^������|�C���^�B�^����|�C���^��void *�ŃL���X�g���Ďg���B
	  */
	virtual int RunThread( unsigned int tnum, int( *thread_func )( void * vparam ), void *param = NULL );
	// !�X���b�h�𗧂ĂĊ֐������s����B
	/*!
	  \param tnum �X���b�h�ԍ��B
	  \param thread_func ���s����֐��Bint �֐���( void )�Ƃ����֐��̊֐����B
	  */
	virtual int RunThread( unsigned int tnum, int( *thread_func )( void ) );
	// !���Ă��X���b�h��j������B
	/*!
	  \param tnum ������~����X���b�h�̔ԍ��B
	  */
	virtual int StopThread( unsigned int tnum );
	// !�X���b�h�̃n���h�����擾����B
	/*!
	  \param tnum �X���b�h�n���h�����擾����X���b�h�ԍ��B
	  \retval �n���h�� �X���b�h�̃n���h���B
	  */
	virtual void * GetThreadHandle( unsigned int tnum );
	// !�w��X���b�h���I������܂őҋ@����B
	/*!
	  \param tnum �I������̂�҂X���b�h�ԍ��B
	  \param sleep 1���[�v�ӂ�̑҂�����(�~���b�B�f�t�H���g5)�B
	  */
	virtual int Join( unsigned int tnum, unsigned long sleep = 5 );
	// !MainLoop�����b�N����B
	virtual int LockMainLoop( void );
	// !MainLoop���A�����b�N����B
	virtual int UnlockMainLoop( void );
	// !ClearScreen�̎��s��҂B
	/*!
	  \param value value��ȏ�ClearScreen�����s�����܂ő҂�(�f�t�H���g1)�B
	  */
	virtual int WaitClearScreen( int value = 1 );
	// !���b�N�̍쐬�B
	/*!
	  \param lnum ���b�N�ԍ��B
	  */
	virtual int CreateLock( unsigned int lnum );
	// !�����̃��b�N�B
	/*!
	  \param lnum ���b�N�ԍ��B
	  \param timeout �^�C���A�E�g����(�f�t�H���g�͖������ɑ҂�)�B
	  */
	virtual unsigned long Lock( unsigned int lnum, unsigned long timeout = INFINITE );
	// !�����̃A�����b�N�B
	/*!
	  \param lnum ���b�N�ԍ��B
	  */
	virtual int Unlock( unsigned int lnum );
};

//�B������Window�N���X
class MikanWindowPub
{
public:
	MikanWindowPub();
	virtual ~MikanWindowPub();
	//�E�B���h�E�̐ݒ�ύX
	//  virtual BOOL SetWindow( int type = WT_RENEWAL );

	// !�E�B���h�E�̖��O�̎擾�B
	/*!
	  \retval �E�B���h�E�����i�[����char�z��̃|�C���^�B
	  */
	virtual char * GetWindowName( void );
	// !�E�B���h�E�̖��O�̐ݒ�B
	/*!
	  \param name �ݒ肷��E�B���h�E���B
	  */
	virtual int SetWindowName( char * name );
	// !�E�B���h�E�̃A�C�R���ݒ�B
	/*!
	  \param iconnum �A�C�R�����\�[�X�ɕt����ꂽ�ԍ�(���\�[�X�^�C�vICON)�B
	  */
	virtual int SetWindowIcon( int smalliconnum, int bigiconnum );
	// !�E�B���h�E�̃A�C�R���ݒ�B
	/*!
	  \param iconnum �A�C�R�����\�[�X�ɕt����ꂽ�ԍ�(���\�[�X�^�C�vICON)�B
	  */
	virtual int SetWindowIcon( int iconnum );
	// !�E�B���h�E�̃A�C�R���ݒ�B
	/*!
	  \param iconnum �A�C�R�����\�[�X�ɕt����ꂽ���O(���\�[�X�^�C�vICON)�B
	  */
	virtual int SetWindowIcon( char *smalliconname, char *bigiconname );
	// !�E�B���h�E�̃A�C�R���ݒ�B
	/*!
	  \param iconnum �A�C�R�����\�[�X�ɕt����ꂽ�ԍ�(���\�[�X�^�C�vICON)�B
	  */
	virtual int SetWindowIcon( char *iconname );

	// !���݂̃E�B���h�E�̉������擾�B
	/*!
	  \param type 0=�ݒ�l��Ԃ�(�f�t�H���g), 1=���݂̃T�C�Y�𒲂ׂĕԂ��B
	  \retval �l �E�B���h�E�̉����B
	  */
	virtual unsigned int GetWindowWidth( int type = 0 );
	// !���݂̃E�B���h�E�̍������擾�B
	/*!
	  \param type 0=�ݒ�l��Ԃ�(�f�t�H���g), 1=���݂̃T�C�Y�𒲂ׂĕԂ��B
	  \retval �l �E�B���h�E�̍����B
	  */
	virtual unsigned int GetWindowHeight( int type = 0 );
	// !�E�B���h�E�̃T�C�Y�ݒ�BMikanWindow�o�R���ƃN���C�A���g�̈悩��v�Z�����B
	/*!
	  \param width �ݒ肷��E�B���h�E�̉����B
	  \param height �ݒ肷��E�B���h�E�̍����B
	  \retval 0 ����
	  */
	virtual int SetWindowSize( unsigned int width, unsigned int height );

	// !���݂̃E�B���h�E��X���W���擾�B
	/*!
	  \param type 0=�ݒ�l��Ԃ�(�f�t�H���g), 1=���݂̃T�C�Y�𒲂ׂĕԂ��B
	  \retval �l �E�B���h�E��X���W�B
	  */
	virtual int GetPositionX( int type = 0 );
	// !���݂̃E�B���h�E��Y���W���擾�B
	/*!
	  \param type 0=�ݒ�l��Ԃ�(�f�t�H���g), 1=���݂̃T�C�Y�𒲂ׂĕԂ��B
	  \retval �l �E�B���h�E��Y���W�B
	  */
	virtual int GetPositionY( int type = 0 );
	// !�E�B���h�E�̍��W�ݒ�B
	/*!
	  \param x �ݒ肷��E�B���h�E��X���W�B
	  \param y �ݒ肷��E�B���h�E��Y���W�B
	  */
	virtual int SetPositionXY( int x, int y );

	// !�t���X�N���[���ɂ���(SystemInit�ł̎��s�𐄏�)�B
	virtual int SetFullScreen( void );

	/*  //�f�B�X�v���C�̐���Ԃ�
	virtual int Window::GetMonitorNumber(void);*/

	// �f�X�N�g�b�v�̉������擾����B
	/*!
	  \retval �f�X�N�g�b�v�̉����B
	  */
	virtual int GetDesktopWidth( void );
	// �f�X�N�g�b�v�̍������擾����B
	/*!
	  \retval �f�X�N�g�b�v�̍����B
	  */
	virtual int GetDesktopHeight( void );
	// �f�X�N�g�b�v�̑傫�����擾����B
	/*!
	  \param width �������i�[����int�^�ϐ��ւ̃|�C���^�B
	  \param height �������i�[����int�^�ϐ��ւ̃|�C���^�B
	  */
	virtual int GetDesktopSize( int *width, int *height );

	//�E�B���h�E�n���h����Ԃ�
	/*!
	  \retval �E�B���h�E�n���h�� ���݂̃E�B���h�E�n���h���B
	  */
	virtual HWND GetWindowHandle( void );

	// !�h���b�O&�h���b�v���ꂽ���Ɏ󂯎��֐���ݒ肷��B
	/*!
	  \param func int Function( int files )�Ƃ����֐��̖��O��^����B�����͎󂯎�����t�@�C�����B
	  */
	virtual int SetReceiveDragFileFunction( int( *func )( int files ) );
	// !�h���b�O&�h���b�v���ꂽ�t�@�C�������擾
	/*!
	  \param num �h���b�O&�h���b�v���ꂽ�t�@�C���ԍ�
	  \retval ������̃|�C���^�B�����R�s�[���邱�ƁB
	  */
	virtual char * GetReceiveDragFilePath( int num );
};

//�B������Draw�N���X
class MikanDrawPub
{
	friend class MikanMovieManagementPub;
protected:
	static int base_x, base_y;
public:

	MikanDrawPub();
	virtual ~MikanDrawPub();
	// !Mikan���C�u���������߂���ێ����Ă���F(���F�厞�_http://www.colordic.org/���)�B
	struct WEBCOLOR ColorList;

	// !�e�N�X�`���̍ő吔�̐ݒ�B
	/*!
	  \param tmax �g�p����e�N�X�`���̍ő吔(�f�t�H���gDX_TEXTURE_MAX)�B
	  */
	virtual unsigned int SetTextureMax( unsigned int tmax = DX_TEXTURE_MAX );

	// !�`��T�C�Y�̐ݒ�
	/*!
	  \param color ��ʂ�h��ׂ��F(�f�t�H���g0xFF000000[��])�B
	  */
	virtual int SetScreenSize( int width, int height );

	// !�`�扡�����擾����B
	/*
	\param now 0�Őݒ�l���擾(�f�t�H���g)�B1�Ō��݂̗L���l���擾�B
	*/
	virtual int GetScreenWidth( int now = 0 );
	// !�`�捂�����擾����B
	/*
	\param now 0�Őݒ�l���擾(�f�t�H���g)�B1�Ō��݂̗L���l���擾�B
	*/
	virtual int GetScreenHeight( int now = 0 );

	// !��ʂ��Y��ɂ���B
	/*!
	  \param color ��ʂ�h��ׂ��F(�f�t�H���g0xFF000000[��])�B
	  */
	virtual int ClearScreen( unsigned long color = 0xFF000000 );

	// !�`��x���W���Z�b�g�B
	/*!
	  \param x �`����X���W�B
	  */
	virtual int SetBaseX( int x = 0 );
	// !�`��y���W���Z�b�g�B
	/*!
	  \param y �`����Y���W�B
	  */
	virtual int SetBaseY( int y = 0 );

	// !�e�N�X�`���̍������@�̎w��B
	/*!
	  \param kind �`����@(DRAW_TRC=���ߐF�L��[�f�t�H���g], DRAW_ADD=���Z����, DRAW_SUB=���Z����, DRAW_MUL=��Z����, DRAW_NON=���ߐF����)�B
	  */
	virtual int SetRender( int kind = DRAW_TRC );
	// !��ʂ������_�[�^�[�Q�b�g�ɂ���B
	virtual int SetRenderTarget( void );
	// !�e�N�X�`���������_�[�^�[�Q�b�g�ɂ���B
	/*!
	  \param txnum �`��Ώۂɂ���e�N�X�`���ԍ��B
	  \retval 0 ����
	  */
	virtual int SetRenderTarget( unsigned int txnum );
	// !���\�[�X��ǂݍ���Ńe�N�X�`�������B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param module ���\�[�X�������W���[���̃n���h���Bexe�t�@�C���̏ꍇ��NULL���w�肷��B
	  \param resource ���\�[�X��(���\�[�X�� IMAGE "�t�@�C���p�X")�B
	  */
	virtual int CreateTexture( unsigned int txnum, HMODULE module, const char *resource, unsigned long tcolor = 0x00000000, int thread = 0 );
	// !��̃e�N�X�`�������B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param width �e�N�X�`���̉����B
	  \param height �e�N�X�`���̍����B
	  */
	virtual int CreateTexture( unsigned int txnum, unsigned int width, unsigned int height, int thread = 0 );
	// !�摜��ǂݍ���Ńe�N�X�`�������B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param filename �t�@�C���p�X�B
	  \param tcolor ���ߐF�̎w��(TRC_NONE=���ߐF���w�肵�Ȃ�[�f�t�H���g], TRC_ZERO=����̐F�𓧉ߐF�ɂ���, ���̑��F=���ߐF�ɂ���F)�B
	  */
	virtual int CreateTexture( unsigned int txnum, const char *filename, unsigned long tcolor = 0x00000000, int thread = 0 );
	// !�e�N�X�`���̉���B
	/*!
	  \param txnum �J������e�N�X�`���̔ԍ��B
	  */
	virtual int ReleaseTexture( unsigned int txnum );
	// !�_��`�悷��B
	/*!
	  \param x �`��X���W�B
	  \param y �`��Y���W�B
	  \param color ���̐F(�f�t�H���g0xFFFFFFFF[��])
	  */
	virtual int DrawPixel( int x, int y, unsigned long color = 0xffffffff );
	// !������`�悷��B
	/*!
	  \param sx �`��J�nX���W�B
	  \param sy �`��J�nY���W�B
	  \param ex �`��I��X���W�B
	  \param ey �`��I��Y���W�B
	  \param color ���̐F(�f�t�H���g0xFFFFFFFF[��])
	  \param width ���̑���(�f�t�H���g1.0)�B
	  \param antialias �A���`�G�C���A�X�̎w��(0=����[�f�t�H���g], 1�L��)�B
	  */
	virtual int DrawLine( int sx, int sy, int ex, int ey, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );
	// !�l�p�`�̐���`�悷��B
	/*!
	  \param dx �`��J�nX���W�B
	  \param dy �`��J�nY���W�B
	  \param w ���̉����B
	  \param h ���̍����B
	  \param color ���̐F�B
	  \param width ���̑����B
	  \param antialias �A���`�G�C���A�X�̎w��(0=����[�f�t�H���g], 1�L��)�B
	  */
	virtual int DrawBoxLine( int dx, int dy, int w, int h, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );
	// !�l�p�`�̐���`�悷��B
	/*!
	  \param dx �`�撆��X���W�B
	  \param dy �`�撆��Y���W�B
	  \param w ���̉����B
	  \param h ���̍����B
	  \param color ���̐F�B
	  \param width ���̑����B
	  \param antialias �A���`�G�C���A�X�̎w��(0=����[�f�t�H���g], 1�L��)�B
	  */
	virtual int DrawBoxLineC( int dx, int dy, int w, int h, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );

	// !�l�p�`��`�悷��B
	/*!
	  \param dx �`��J�nX���W�B
	  \param dy �`��J�nY���W�B
	  \param w ���̉����B
	  \param h ���̍����B
	  \param color ���̐F�B
	  */
	virtual int DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color );
	// !�l�p�`��`�悷��B
	/*!
	  \param x0 �����X���W�B
	  \param y0 �����Y���W�B
	  \param x1 �E���X���W�B
	  \param y1 �E���Y���W�B
	  \param x2 ������X���W�B
	  \param y2 ������Y���W�B
	  \param x3 �E���X���W�B
	  \param y3 �E���Y���W�B
	  \param color0 ����̐F�B
	  \param color1 �E��̐F�B
	  \param color2 �����̐F�B
	  \param color3 �E��̐F�B
	  */
	virtual int DrawBox( int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 );
	// !�l�p�`��`�悷��B
	/*!
	  \param x0 �����X���W�B
	  \param y0 �����Y���W�B
	  \param x1 �E���X���W�B
	  \param y1 �E���Y���W�B
	  \param x2 ������X���W�B
	  \param y2 ������Y���W�B
	  \param x3 �E���X���W�B
	  \param y3 �E���Y���W�B
	  \param color ���̐F�B
	  */
	virtual int DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 );
	// !�l�p�`��`�悷��B
	/*!
	  \param dx �`��J�nX���W�B
	  \param dy �`��J�nY���W�B
	  \param w ���̉����B
	  \param h ���̍����B
	  \param color ���̐F�B
	  */
	virtual int DrawBox( int dx, int dy, int w, int h, unsigned long color );
	// !�l�p�`�𒆐S����ɕ`�悷��B
	/*!
	  \param dx �`�撆��X���W�B
	  \param dy �`�撆��Y���W�B
	  \param w ���̉����B
	  \param h ���̍����B
	  \param color0 ����̐F�B
	  \param color1 �E��̐F�B
	  \param color2 �����̐F�B
	  \param color3 �E��̐F�B
	  */
	virtual int DrawBoxC( int dx, int dy, int w, int h, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 );
	// !�l�p�`�𒆐S����ɕ`�悷��B
	/*!
	  \param dx �`�撆��X���W�B
	  \param dy �`�撆��Y���W�B
	  \param w ���̉����B
	  \param h ���̍����B
	  \param color ���̐F�B
	  */
	virtual int DrawBoxC( int dx, int dy, int w, int h, unsigned long color );

	// !�~�`��(�~�̐��̂�)�B
	/*!
	  \param x �~�̒��SX���W�B
	  \param y �~�̒��SY���W�B
	  \param r �~�̔��a�B
	  \param color �~�̐F(�f�t�H���g0xFFFFFFFF[��])�B
	  */
	virtual int DrawCircleLineC( int x, int y, int r, unsigned long color = 0xFFFFFFFF, double width = 1.0, int antialias = 0 );
	// !�~�`��(�����Ɏ��܂�ȉ~�̐�)�B
	/*!
	  \param x �~�����߂锠�̕`��J�nX���W�B
	  \param y �~�����߂锠�̕`��J�nY���W�B
	  \param w �~�����߂锠�̉����B
	  \param h �~�����߂锠�̍����B
	  \param color �~�̐F(�f�t�H���g0xFFFFFFFF[��])�B
	  */
	virtual int DrawCircleLine( int x, int y, int w, int h, unsigned long color = 0xFFFFFFFF, double width = 1.0, int antialias = 0 );

	// !�~�`��(�~�̂�)�B
	/*!
	  \param x �~�̒��SX���W�B
	  \param y �~�̒��SY���W�B
	  \param r �~�̔��a�B
	  \param color �~�̐F(�f�t�H���g0xFFFFFFFF[��])�B
	  */
	virtual int DrawCircleC( int x, int y, int r, unsigned long color = 0xFFFFFFFF );
	// !�~�`��(�����Ɏ��܂�ȉ~)�B
	/*!
	  \param x �~�����߂锠�̕`��J�nX���W�B
	  \param y �~�����߂锠�̕`��J�nY���W�B
	  \param w �~�����߂锠�̉����B
	  \param h �~�����߂锠�̍����B
	  \param color �~�̐F(�f�t�H���g0xFFFFFFFF[��])�B
	  */
	virtual int DrawCircle( int x, int y, int w, int h, unsigned long color = 0xFFFFFFFF );

	// !�e�N�X�`����`�悷��B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param dx �`��J�nX���W�B
	  \param dy �`��J�nY���W�B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTexture( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	// !�e�N�X�`����`�悷��B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param dx �`�撆��X���W�B
	  \param dy �`�撆��Y���W�B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTextureC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	// !�e�N�X�`�����g��k���`�悷��B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param dx �`��J�nX���W�B
	  \param dy �`��J�nY���W�B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param dw �`�扡���B
	  \param dh �`�捂���B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	// !�e�N�X�`�����g��k���`�悷��B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param dx �`��J�nX���W�B
	  \param dy �`��J�nY���W�B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param scale �g�嗦�B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// !�e�N�X�`�����g��k���`�悷��B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param dx �`�撆��X���W�B
	  \param dy �`�撆��Y���W�B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param dw �`�扡���B
	  \param dh �`�捂���B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	// !�e�N�X�`�����g��k���`�悷��B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param dx �`�撆��X���W�B
	  \param dy �`�撆��Y���W�B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param scale �g�嗦�B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// !�e�N�X�`������]�`�悷��(���W�A���p)�B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param dx �`�撆��X���W�B
	  \param dy �`�撆��Y���W�B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param rad ��]�p�x(���W�A���p�x)�B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTextureRotationC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 );
	// !�e�N�X�`������]�`�悷��(���W�A���p�ł͂Ȃ�)�B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param dx �`�撆��X���W�B
	  \param dy �`�撆��Y���W�B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param angle ��]�p�x(�x�B���W�A���p�x�ł͂Ȃ��̂Œ��p��90.0)�B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTextureRotationAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 );
	// !�e�N�X�`������]�g��`�悷��(���W�A���p)�B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param dx �`�撆��X���W�B
	  \param dy �`�撆��Y���W�B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param sw �`�扡���B
	  \param sh �`�捂���B
	  \param rad ��]�p�x(���W�A���p�x)�B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag = 0 );
	// !�e�N�X�`������]�g��`�悷��(���W�A���p�ł͂Ȃ�)�B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param dx �`�撆��X���W�B
	  \param dy �`�撆��Y���W�B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param sw �`�扡���B
	  \param sh �`�捂���B
	  \param angle ��]�p�x(�x�B���W�A���p�x�ł͂Ȃ��̂Œ��p��90.0)�B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag = 0 );
	// !�e�N�X�`������]�g��`�悷��B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param dx �`�撆��X���W�B
	  \param dy �`�撆��Y���W�B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param scale �g�嗦�B
	  \param rad ��]�p�x(���W�A���p�x)�B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 );
	// !�e�N�X�`������]�g��`�悷��B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param dx �`�撆��X���W�B
	  \param dy �`�撆��Y���W�B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param scale �g�嗦�B
	  \param angle ��]�p�x(�x�B���W�A���p�x�ł͂Ȃ��̂Œ��p��90.0)�B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 );
	// !�e�N�X�`����4�_�����R�Ɍ��߂ĕ`�悷��B
	/*!
	  \param txnum �e�N�X�`���ԍ��B
	  \param rx �ǂݍ��݊J�nX���W�B
	  \param ry �ǂݍ��݊J�nY���W�B
	  \param w �؂��鉡���B
	  \param h �؂��鍂���B
	  \param x0 �e�N�X�`���̍���̕`���X���W�B
	  \param y0 �e�N�X�`���̍���̕`���Y���W�B
	  \param x1 �e�N�X�`���̉E��̕`���X���W�B
	  \param y1 �e�N�X�`���̉E��̕`���Y���W�B
	  \param x2 �e�N�X�`���̍����̕`���X���W�B
	  \param y2 �e�N�X�`���̍����̕`���Y���W�B
	  \param x3 �e�N�X�`���̉E���̕`���X���W�B
	  \param y3 �e�N�X�`���̉E���̕`���Y���W�B
	  \param drawflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	  */
	virtual int DrawTextureVertex( unsigned int txnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 );

	// !�|���S���̍쐬�B
	/*!
	  \param pnum �|���S���ԍ��B
	  \param max  �|���S���̒��_���B
	  \param filltype �|���S���̓h���(D3DPT_TRIANGLESTRIP=�A�Ȃ����O�p�`�Ƃ���(�f�t�H���g),D3DPT_TRIANGLEFAN=�ŏ��̒��_�Ƃ��̑��̒��_�ŎO�p�`�Ƃ���,�ȂǂȂ�)�B
	  */
	virtual int CreatePolygon( unsigned int pnum, unsigned int max, D3DPRIMITIVETYPE filltype = D3DPT_TRIANGLESTRIP );

	// !�����p�`�̍쐬(D3DPT_TRIANGLEFAN�ō쐬�����|���S��)�B
	/*!
	  \param pnum �|���S���ԍ��B
	  \param cx ����X���W�B
	  \param cy ����Y���W�B
	  \param n �����p�`�̊p���B
	  \param r ���a�B
	  \param rad ��]�p�x�B
	  \param color �F�B
	  */
	virtual int CreateRegularPolygon( unsigned int pnum, double cx, double cy, unsigned int n, double r, double rad = 0.0, unsigned long color = 0xffffffff );

	// !�e�N�X�`���̓o�^
	/*!
	  \param pnum �|���S���ԍ��B
	  \param tnum �|���S���ɐݒ肷��e�N�X�`���ԍ��B
	  */
	virtual int SetTextureOnPolygon( unsigned int pnum, unsigned int tnum );

	// !�e�N�X�`���̓o�^����
	/*!
	  \param pnum �|���S���ԍ��B
	  */
	virtual int SetTextureOnPolygon( unsigned int pnum );

	// !UV���W�̓o�^�B
	/*!
	  \param pnum �|���S���ԍ��B
	  \param vnum ���_�ԍ��B
	  \param u UV�W�J��X���W�B
	  \param v UV�W�J��Y���W�B
	  */
	virtual int SetPolygonUV( unsigned int pnum, unsigned int vnum, int u, int v );

	// !���_�̓o�^
	/*!
	  \param pnum �|���S���ԍ��B
	  \param vnum ���_�ԍ��B
	  \param x �`��X���W�B
	  \param y �`��Y���W�B
	  \param color ���_�F�B
	  */
	virtual int SetPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color = 0xFFFFFFFF );

	// !���_���̕ҏW
	/*!
	  \param pnum �|���S���ԍ��B
	  \param vnum ���_�ԍ��B
	  \param x �`��X���W�B
	  \param y �`��Y���W�B
	  \param color ���_�F�B
	  \param u UV�W�J��X���W�B
	  \param v UV�W�J��Y���W�B
	  \param edit �ǂ̕ҏW��L�������邩(PVEDIT_X=X���W,PVEDIT_Y=Y���W,PVEDIT_XY=���W,PVEDIT_DEFAULT=���W(�f�t�H���g),PVEDIT_COLOR=�F,PVEDIT_U=U���W,PVEDIT_V=V���W,PVEDIT_UV=UV���W,PVEDIT_ALL=�S��)�B
	  */
	virtual int EditPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color = 0xFFFFFFFF, int u = 0, int v = 0, int edit = PVEDIT_DEFAULT );
	// !�|���S���̈ړ�
	/*!
	  \param pnum �|���S���ԍ��B
	  \param mx �ړ�X���W�B
	  \param my �ړ�Y���W�B
	  */
	virtual int TransformPolygonMoving( unsigned int pnum, double mx, double my );
	// !�|���S���̊g��
	/*!
	  \param pnum �|���S���ԍ��B
	  \param scale �g�嗦�B
	  \param basenum ��Ƃ��钸�_�ԍ�(�f�t�H���g=0)�B
	  */
	virtual int TransformPolygonScaling( unsigned int pnum, double scale, unsigned int basenum = 0 );
	// !�|���S���̕`��
	/*!
	  \param pnum �|���S���ԍ��B
	  \param cx �`��X���W�B
	  \param cy �`��Y���W�B
	  \param begin �`��J�n���_�ԍ��B
	  \param end �`��I�����_�ԍ��B
	  */
	virtual int DrawPolygon( unsigned int pnum, int cx, int cy, unsigned int begin, unsigned int end );
	// !�|���S���̕`��
	/*!
	  \param pnum �|���S���ԍ��B
	  \param cx �`��X���W�B
	  \param cy �`��Y���W�B
	  */
	virtual int DrawPolygon( unsigned int pnum, int cx, int cy );
	// !�|���S�����ێ�����ʒu�ɑ��p�`�̕`��
	/*!
	  \param pnum �|���S���ԍ��B
	  */
	virtual int DrawPolygon( unsigned int pnum );
	// !�|���S���̊g��`��
	/*!
	  \param pnum �|���S���ԍ��B
	  \param cx �`��X���W�B
	  \param cy �`��Y���W�B
	  \param scale �g�嗦
	  \param start �`��J�n���_�ԍ��B
	  \param end �`��I�����_�ԍ��B
	  */
	virtual int DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale, unsigned int start, unsigned int end );
	// !�|���S���̊g��`��
	/*!
	  \param pnum �|���S���ԍ��B
	  \param cx �`��X���W�B
	  \param cy �`��Y���W�B
	  \param scale �g�嗦
	  */
	virtual int DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale );
	// !�����p�`�̕`��
	/*!
	  \param cx �`����W�B
	  \param cy �`����W�B
	  \param n ��N�p�`��N�B
	  \param r ���a�B
	  \param rad ��]�p�x�B
	  \param color ���_�F�B
	  */
	virtual int DrawRegularPolygon( double cx, double cy, unsigned int n, double r, double rad = 0.0, unsigned long color = 0xffffffff );

	// !�F�̍쐬�B
	/*!
	  \param alpha �A���t�@�l(�s�����x�B0�`255�B255�ŕs����)�B
	  \param red ��(0�`255)�B
	  \param green ��(0�`255)�B
	  \param blue ��(0�`255)�B
	  */
	virtual unsigned long Color( unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue );
	// !�J���[�̐ݒ�B
	/*!
	  \param txnum �F��ύX����e�N�X�`���ԍ��B
	  \param color �e�N�X�`���ɐݒ肷��F(���Œʏ�)�B
	  */
	virtual int SetColor( unsigned int txnum, unsigned long color = 0xFFFFFFFF );
	// �A���t�@�l�̐ݒ�B
	/*!
	  \param txnum �A���t�@�l��ύX����e�N�X�`���ԍ��B
	  \param alpha �A���t�@�l(�s�����x�B0�`255�B255�ŕs����)�B�A���t�@�l�ȊO�̐F�͕ێ������B
	  */
	virtual int SetAlpha( unsigned int txnum, unsigned char alpha );
	// !���_�J���[�̐ݒ�
	/*!
	  \param txnum ���_�̐F��ύX����e�N�X�`���ԍ��B
	  \param color ����ɐݒ肷��F�B�ߋ��̐F��1�Â���ĉE���̐F���ǂ��o�����B
	  */
	virtual int SetColorVertex( unsigned int txnum, unsigned long color );

	// !�e�N�X�`���̃f�[�^��ǂ߂�悤�ɂ���(�F�̌�����u����������������ꍇ�Ɍy�ʉ��\)�B
	/*!
	  \param �ǂݏ������b�N���s���e�N�X�`���ԍ��B
	  \param x �ǂݏ����J�nX���W(�f�t�H���g0)�B
	  \param y �ǂݏ����J�nY���W(�f�t�H���g0)�B
	  \param w ����(�f�t�H���g0[�ő�l�܂�])�B
	  \param h ����(�f�t�H���g0[�ő�l�܂�])�B
	  */
	virtual int OpenTexture( unsigned int txnum, int x = 0, int y = 0, int w = 0, int h = 0 );
	// !�e�N�X�`���̃f�[�^��ǂ߂��Ԃ���߂��B
	/*!
	  \param txnum �ǂݏ����A�����b�N���s���e�N�X�`���ԍ��B
	  */
	virtual int CloseTexture( unsigned int txnum );
	// !�e�N�X�`���̎w����W�̐F���擾����B
	/*!
	  \param txnum �F�̌������s���e�N�X�`���ԍ��B
	  \param x �ǂݍ��ޓ_��X���W�B
	  \param y �ǂݍ��ޓ_��Y���W�B
	  */
	virtual unsigned long GetColorTexturePixel( unsigned int txnum, int x, int y );
	// !�e�N�X�`���͈͓̔��̐F��u������B
	/*!
	  \param txnum �F��u������e�N�X�`���ԍ��B
	  \param scolor �����F�B
	  \param rcolor �u���F�B
	  \param x �ǂݍ��݊J�nX���W(�f�t�H���g0)�B
	  \param y �ǂݍ��݊J�nY���W(�f�t�H���g0)�B
	  \param w ����(�f�t�H���g0[�ő�l�܂�])�B
	  \param h ����(�f�t�H���g0[�ő�l�܂�])�B
	  */
	virtual unsigned int TextureColorReplacement( unsigned int txnum, unsigned long scolor, unsigned long rcolor, int x = 0, int y = 0, int w = 0, int h = 0 );
	// !�e�N�X�`���͈͓̔��̐F����������B
	/*!
	  \param txnum �F����������e�N�X�`���ԍ��B
	  \param scolor �����F�B
	  \param x �ǂݍ��݊J�nX���W(�f�t�H���g0)�B
	  \param y �ǂݍ��݊J�nY���W(�f�t�H���g0)�B
	  \param w ����(�f�t�H���g0[�ő�l�܂�])�B
	  \param h ����(�f�t�H���g0[�ő�l�܂�])�B
	  \retval 0 ������Ȃ������B
	  \retval ����ȊO ������܂łɒ��ׂ��s�N�Z����(���ォ��1)�B
	  */
	virtual unsigned int TextureColorSearch( unsigned int txnum, unsigned long scolor, int x = 0, int y = 0, int w = 0, int h = 0 );
	// !�e�N�X�`���͈͓̔��̐F�����������𐔂���B
	/*!
	  \param txnum �F����������e�N�X�`���ԍ��B
	  \param scolor �����F�B
	  \param x �ǂݍ��݊J�nX���W(�f�t�H���g0)�B
	  \param y �ǂݍ��݊J�nY���W(�f�t�H���g0)�B
	  \param w ����(�f�t�H���g0[�ő�l�܂�])�B
	  \param h ����(�f�t�H���g0[�ő�l�܂�])�B
	  \retval �l ���������F�̐��B
	  */

	virtual unsigned int TextureColorCount( unsigned int txnum, unsigned long scolor, int x = 0, int y = 0, int w = 0, int h = 0 );
	// !�e�N�X�`���̉����擾�B
	/*!
	  \param txnum �������擾����e�N�X�`���ԍ��B
	  */
	virtual unsigned int GetTextureWidth( unsigned int txnum = 0 );
	// !�e�N�X�`���̍����擾�B
	/*!
	  \param txnum �������擾����e�N�X�`���ԍ��B
	  */
	virtual unsigned int GetTextureHeight( unsigned int txnum = 0 );
	// !�X�N���[���V���b�g�B
	/*!
	  \param filename �o�̓t�@�C�����B
	  \param imagetype �o�͉摜�`��(IMAGE_BMP=Bitmap[�f�t�H���g], IMAGE_JPG=JPEG, IMAGE_PNG=PNG, ... etc)
	  */
	virtual int ScreenShot( const char *filename, unsigned int imagetype = IMAGE_BMP );

	//�t�H���g
	// !�t�H���g��ǂݍ���(�t�@�C������B�J����ƕK�v)�B
	/*!
	  \param fontfile �t�H���g�t�@�C���̃p�X�B
	  */
	virtual int LoadFontFile( const char *fontfile );
	// !�t�H���g��ǂݍ���(���\�[�X����B�J����ƕs�v)�B
	/*!
	  \param module ���\�[�X�������W���[���̃n���h���Bexe�t�@�C���̏ꍇ��NULL���w�肷��B
	  \param resource ���\�[�X��(���\�[�X�� FONT_FILE "�t�@�C���p�X")�B
	  */
	virtual int LoadFontFile( HMODULE module, const char *resource );
	// !�t�H���g�����B
	/*!
	  \param fontnum �쐬����t�H���g�ԍ��B
	  \param fontname �t�H���g��(Windows�ɓo�^����Ă�����̂�LoadFontFile�œǂݍ��񂾂���)�B
	  \param weight ����(���̏ꍇ0���w�肵�Ă���)�B
	  \param italic �C�^���b�N�̂̐ݒ�(1=�C�^���b�N�̂ɂ���,0=�C�^���b�N�̂ɂ��Ȃ�[�f�t�H���g])
	  \param size �t�H���g�T�C�Y(�f�t�H���g10)�B
	  \param color �t�H���g�̐F(�f�t�H���g0xFFFFFFFF[��])�B
	  */
	virtual int CreateFont( unsigned int fontnum, const char *fontname, int size, unsigned long color, int weight, int italic = 0 );
	// !�t�H���g�����B
	/*!
	  \param fontnum �쐬����t�H���g�ԍ��B
	  \param fontname �t�H���g��(Windows�ɓo�^����Ă�����̂�LoadFontFile�œǂݍ��񂾂���)�B
	  \param size �t�H���g�T�C�Y(�f�t�H���g10)�B
	  \param color �t�H���g�̐F(�f�t�H���g0xFFFFFFFF[��])�B
	  */
	virtual int CreateFont( unsigned int fontnum, const char *fontname, int size = 10, unsigned long color = 0xFFFFFFFF );
	// !�t�H���g�����B
	/*!
	  \param fontnum �쐬����t�H���g�ԍ��B
	  \param size �t�H���g�T�C�Y(�f�t�H���g10)�B
	  \param color �t�H���g�̐F(�f�t�H���g0xFFFFFFFF[��])�B
	  */
	virtual int CreateFont( unsigned int fontnum, int size = 10, unsigned long color = 0xFFFFFFFF );
	//�t�H���g���Đݒ肷��

	// !������̕`��B
	/*!
	  \param fontnum �t�H���g�ԍ�(�������̏ꍇ0���g����)�B
	  \param x �`��J�nX���W�B
	  \param y �`��J�nY���W�B
	  \param color �����̐F�B
	  \param format �ϊ��w��q���܂ޕ�����(C�����printf�Ɠ���)�B
	  \param ����ȍ~ �ϊ��w��q���g���l�B
	  */
	virtual int Printf( unsigned int fontnum, int x, int y, unsigned long color, const char *format, ... );
	// !������̕`��B
	/*!
	  \param fontnum �t�H���g�ԍ�(�������̏ꍇ0���g����)�B
	  \param x �`��J�nX���W�B
	  \param y �`��J�nY���W�B
	  \param format �ϊ��w��q���܂ޕ�����(C�����printf�Ɠ���)�B
	  \param ����ȍ~ �ϊ��w��q���g���l�B
	  */
	virtual int Printf( unsigned int fontnum, int x, int y, const char *format, ... );
};

class MikanMovieManagementPub
{
public:
	// ! �e�N�X�`�����擾����(Lock���Ȃ��Ɖ����N���邩�s���B2���̃e�N�X�`���Ń_�u���o�b�t�@�����O���Ă�\��������)�B
	virtual struct IDirect3DTexture9 * GetTexture( unsigned int movnum );
	// ! �e�N�X�`�����������b�N����B
	virtual int Lock( unsigned int movnum );
	// �e�N�X�`���������A�����b�N����B
	virtual int Unlock( unsigned int movnum );

	// ! ������J���B
	/* !
	\param movnum ����ԍ��B
	\param moviefile ����t�@�C���B
	*/
	virtual int Load( unsigned int movnum, char *moviefile );
	// ! ����̍Đ����J�n����B
	/* !
	\param movnum ����ԍ��B
	\param loopplay true�Ń��[�v�Đ��B�ȗ���������false��1�x�����Đ��B
	*/
	virtual int Play( unsigned int movnum, int loopplay = false );
	// ! ����̍Đ����~�߂�B
	/* !
	\param movnum ����ԍ��B
	*/
	virtual int Stop( unsigned int movnum );
	// ! ����𒆒f����B
	/* !
	\param movnum ����ԍ��B
	*/
	virtual int Pause( unsigned int movnum );
	// ! ������Đ����Ă��邩���ׂ�B
	/* !
	\param movnum ����ԍ��B
	*/
	virtual int IsPlay( unsigned int movnum );
	// ! ����̍Ō�̏ꏊ���擾����(�~���b)�B
	/* !
	\param movnum ����ԍ��B
	*/
	virtual long Size( unsigned int movnum );
	// ! ����̌��݂̍Đ��ꏊ���擾����(�~���b)�B
	/* !
	\param movnum ����ԍ��B
	*/
	virtual long Tell( unsigned int movnum );
	// ! ����̍Đ��ʒu���V�[�N����(�~���b)�B
	/* !
	\param movnum ����ԍ��B
	\param seek �Đ��ʒu�B�~���b�B
	*/
	virtual int Seek( unsigned int movnum, long seek );
	// ! ����̍Đ����x�̎擾�B�ʏ��1.0�B
	virtual double GetRate( unsigned int movnum );
	// ! ����̍Đ����x�̐ݒ�B�ʏ��1.0�B
	virtual int SetRate( unsigned int movnum, double rate );

	// ! ����̉������擾����B
	virtual int GetWidth( unsigned int movnum );
	// ! ����̍������擾����B
	virtual int GetHeight( unsigned int movnum );

	// ! ����̐F���������ɖ߂��B
	/* !
	\param movnum ����ԍ��B
	*/
	virtual int SetColor( unsigned int movnum );
	// ! ����S�̂̐F���������߂�B
	/* !
	\param movnum ����ԍ��B
	\param color �F�B
	*/
	virtual int SetColor( unsigned int movnum, unsigned long color );
	// ! ����̊e���_�ɐF��ݒ肷��B
	/* !
	\param movnum ����ԍ��B
	\param color0 ����̐F�B
	\param color1 �E��̐F�B
	\param color2 �����̐F�B
	\param color3 �E���̐F�B
	*/
	virtual int SetColor( unsigned int movnum, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 );
	// ! ����̃A���t�@�l�̂ݕύX����B
	/* !
	\param movnum ����ԍ��B
	\param alpha �A���t�@�l�B
	*/
	virtual int SetAlpha( unsigned int movnum, unsigned char alpha );

	// ! ��������T�C�Y�Ŏw��ʒu�ɕ`�悷��B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovie( unsigned int movnum, int dx = 0, int dy = 0, int drawflag = 0 );
	// ! �����؂����ĕ`�悷��B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovie( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	// ! �����؂����Ē����`�悷��B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovieC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );

	// ! �����؂����Ċg��k���`�悷��B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param scale �g�嗦�B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// ! �����؂����Ċg��k���`�悷��B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param dw �`�扡���B
	\param dh �`�捂���B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	// ! �����؂����Ē����Ɋg��k���`�悷��B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param scale �g�嗦�B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// ! �����؂����Ē����Ɋg��k���`�悷��B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param dw �`�扡���B
	\param dh �`�捂���B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );

	// ! �����؂����Ē�������ɉ�]����(���W�A���p)�B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param rad ��]�p(���W�A���p)�B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovieRotationC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 );
	// ! �����؂����Ē�������ɉ�]����(�p�x)�B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param angle ��]�p(�p�x)�B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovieRotationAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 );

	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag = 0 ) = 0;
	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag = 0 ) = 0;

	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(���W�A���p)�B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param scale �g�嗦�B
	\param rad ��]�p(���W�A���p)�B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 );
	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(�p�x)�B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param scale �g�嗦�B
	\param angle ��]�p(�p�x)�B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 );
	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(���W�A���p)�B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param dw �`�扡���B
	\param dh �`�捂���B
	\param rad ��]�p(���W�A���p)�B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, double rad, int drawflag = 0 );
	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(�p�x)�B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param dw �`�扡���B
	\param dh �`�捂���B
	\param angle �p�x�B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, double angle, int drawflag = 0 );

	// ! �����؂�����4�_�w�肵�ĕ`�悷��B
	/* !
	\param movnum ����ԍ��B
	\param dx �`��J�nX���W�B
	\param dy �`��J�nY���W�B
	\param rx �ǂݎ��J�nX���W�B
	\param ry �ǂݎ��J�nY���W�B
	\param w �����B
	\param h �����B
	\param x0 �`�捶��X���W�B
	\param y0 �`�捶��Y���W�B
	\param x1 �`��E��X���W�B
	\param y1 �`��E��Y���W�B
	\param x2 �`�捶��X���W�B
	\param y2 �`�捶��Y���W�B
	\param x3 �`��E��X���W�B
	\param y3 �`��E��Y���W�B
	\param drafflag �`�攽�]( 0=���]�Ȃ�(�f�t�H���g), DRAW_LR=���E����, DRAW_UD=�㉺���], |���Z�q�ŗ����w����\)�B
	*/
	virtual int DrawMovieVertex( unsigned int movnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 );

};

//�B������Input�N���X
class MikanInputPub
{
public:
	MikanInputPub( void );
	virtual ~MikanInputPub( void );
	// !�Q�[���p�b�h�̍ĔF���B
	/*!
	  \param resume �ǉ��F�����邩�ǂ���(1=�f�t�H���g, 0=���߂���)�B
	  */
	virtual int RecognitionGamepad( int resume = 1 );
	// !�L�[�̎擾(��Ԃ̍X�V)�B
	virtual int UpdateKeyInput( void );
	// !�Q�[���p�b�h�̍X�V(��Ԃ̍X�V)�B
	virtual int UpdatePadInput( void );
	// !�L�[���͂̎擾�B
	/*!
	  \param keynum �L�[�{�[�h�萔(K_�L�[���BK_Z��K_ESC�Ȃ�)�B
	  \retval ���̒l ������Ă���t���[�����B
	  \retval 0 ������Ă��Ȃ��B
	  \retval -1 ���ꂽ����1�t���[���B
	  */
	virtual int GetKeyNum( int keynum );
	// !�L�[���͂̎擾(interval�ȏ㉟���āA����threshold�t���[�����Ƃ�1��Ԃ�)�B
	/*!
	  \param keynum �L�[�{�[�h�萔(K_�L�[���BK_Z��K_ESC�AK_UP�Ȃ�)�B
	  \param threshold 1��Ԃ��Ԋu�B
	  \param interval ���߂̏��0��Ԃ�����(�f�t�H���g0)�B
	  \retval 0 �����𖞂����Ă��Ȃ��B
	  \retval 1 �����𖞂������B
	  */
	virtual int GetKeyNumRapid( int keynum, int threshold, unsigned int interval = 0 );
	// !���̃L�[��������Ă��邩�Ԃ��B
	/*!
	  \retval �L�[�萔 ������Ă���L�[�萔��Ԃ��B����������Ă���ꍇ�ł��Ō�ɉ����ꂽ�L�[�B
	  \reval -1 �����L�[�͉�����Ă��Ȃ��B
	  */
	virtual int GetKeyWhichButton( void );

	// !�Q�[���p�b�h���͂̎擾�B
	/*!
	  \param padnum �p�b�h�ԍ��B
	  \param button �p�b�h�̃{�^���萔(PAD_UP��PAD_A�Ȃ�)�B
	  */
	virtual int GetPadNum( int padnum, int button );
	// !�Q�[���p�b�h�̃X�e�B�b�N���͂̎擾(�A�i���O�l�̓���)�B
	/*!
	  \param padnum �p�b�h�ԍ��B
	  \param button �A�i���O�X�e�B�b�N�̃{�^���萔(PAD_LS_LR��PAD_X_AXIS��)�B
	  */
	virtual int GetPadStick( unsigned int padnum, int button );
	// �Q�[���p�b�h���t�H�[�X�t�B�[�h�o�b�N�ɑΉ����Ă��邩�ǂ������ׂ�B
	virtual int CanForceFeedback( unsigned int padnum );
	// !POV���[�h�̐ؑցB
	/*!
	  \param padnum POV���[�h(�㉺���E�L�[���͂ƃX�e�B�b�N���͂̕���)��؂�ւ���p�b�h�ԍ��B
	  \param povmode (0=�㉺���E�L�[���͂ƃX�e�B�b�N���͂𓯈ꎋ, 1=�㉺���E�ƃX�e�B�b�N���͂�ʂɌ���)�B
	  */
	virtual int SetPOVMode( unsigned int padnum, int povmode );
	// !�}�����Ă���Q�[���p�b�h�̐����擾����B
	/*!
	  \retval �l ���ݔF�����Ă���Q�[���p�b�h�̐���Ԃ��B
	  */
	virtual int GetPadMount( void );
	// !�p�b�h�̂ǂ̃{�^�������������B
	/*!
	  \retval padnum �{�^�������𒲂ׂ�p�b�h�ԍ��B
	  \retval �p�b�h�̃{�^���萔 �����ɉ�����Ă���ꍇ�ł��V���������ꂽ�ԍ��̏������p�b�h�̃{�^����D�悷��B
	  */
	virtual int GetPadWhichButton( unsigned int padnum );

	// !�}�E�X��X���W���擾����B
	/*!
	  \retval X���W �������E�B���h�E���ł̍��W�B
	  */
	virtual int GetMousePosX( void );
	// !�}�E�X��Y���W���擾����B
	/*!
	  \param Y���W �������E�B���h�E���ł̍��W�B
	  */
	virtual int GetMousePosY( void );
	// !�}�E�X�̃{�^�����͂��擾����B
	/*!
	  \param num �}�E�X�̃{�^���萔(0=���N���b�N, 1=�E�N���b�N, ... etc)�B
	  \retval TODO
	  */
	virtual int GetMouseNum( unsigned int num );
	// !�}�E�X�J�[�\���̍��W��ύX����B
	/*!
	  \param x �ݒ肷��X���W(�������E�B���h�E��)�B
	  \param y �ݒ肷��Y���W(�������E�B���h�E��)�B
	  */
	virtual int SetMousePos( int x, int y );
	// !�}�E�X�J�̃z�C�[����]���擾����B
	/*!
	\retval �z�C�[������]�����u�Ԃɐ����̒l���擾�ł���B0�͉�]�Ȃ��B
	*/
	virtual int GetMouseWheel( void );
	// !�}�E�X���͂̍X�V(��Ԃ̍X�V)�B
	virtual int UpdateMouseInput( void );

	// !Pad2Key�̋@�\��L��������B
	/*!
	  \param enable (0=������[�f�t�H���g], 1=�L����)�B
	  */
	virtual int SetPad2KeyInput( int enable = 0 );
	// !Pad2Key�̃L�[�g�ݍ��킹��ݒ肷��B
	/*!
	  \param padnum Pad2Key��ݒ肷��p�b�h�ԍ��B
	  \param padbutton �ϊ��Ώۂ̃p�b�h�{�^���萔�B
	  \param keynum �ϊ���̃L�[�{�[�h�̃L�[�萔�B
	  */
	virtual int SetPad2KeyCode( unsigned int padnum, int padbutton, int keynum = -1 );
	// !Pad2Key�̏�Ԃ��X�V����B
	virtual int UpdatePad2KeyInput( void );
};

//�B������Netqork�N���X
class MikanNetworkPub
{
public:

	MikanNetworkPub( void );
	~MikanNetworkPub( void );

	// !TCP�T�[�o�[���쐬����B
	/*!
	  \param nnum �l�b�g���[�N�ԍ��B
	  \param port �҂��󂯃|�[�g�ԍ��B
	  \param connect �R�l�N�g���B
	  */
	virtual SOCKET CreateTCPServer( unsigned int nnum, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	// !UDP�T�[�o�[���쐬����B
	/*!
	  \param nnum �l�b�g���[�N�ԍ��B
	  \param port �҂��󂯃|�[�g�ԍ��B
	  \param connect �R�l�N�g���B
	  */
	virtual SOCKET CreateUDPServer( unsigned int nnum, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	// !�u���[�h�L���X�g�T�[�o�[���쐬����B
	/*!
	  \param nnum �l�b�g���[�N�ԍ��B
	  \param port �҂��󂯃|�[�g�ԍ��B
	  \param connect �R�l�N�g���B
	  */
	virtual SOCKET CreateBloadcastServer( unsigned int nnum, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	virtual SOCKET CreateMulticastServer( unsigned int nnum, const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	virtual SOCKET CreateMulticastServer( unsigned int nnum, unsigned long address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );

	// !TCP�N���C�A���g���쐬����B
	/*!
	  \param nnum �l�b�g���[�N�ԍ��B
	  \param address �ڑ���IP�A�h���X�܂��̓z�X�g���B
	  \param port �ڑ��|�[�g�ԍ��B
	  */
	virtual SOCKET CreateTCPClient( unsigned int nnum, const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	// !UDP�N���C�A���g���쐬����B
	/*!
	  \param nnum �l�b�g���[�N�ԍ��B
	  \param address �ڑ���IP�A�h���X�܂��̓z�X�g���B
	  \param port �ڑ��|�[�g�ԍ��B
	  */
	virtual SOCKET CreateUDPClient( unsigned int nnum, const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	// !�u���[�h�L���X�g�N���C�A���g���쐬����B
	/*!
	  \param nnum �l�b�g���[�N�ԍ��B
	  \param address �ڑ���IP�A�h���X�܂��̓z�X�g��(�f�t�H���g255.255.255)�B
	  \param port �ڑ��|�[�g�ԍ��B
	  */
	virtual SOCKET CreateBloadcastClient( unsigned int nnum, const char *address = "255.255.255.255", unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	virtual SOCKET CreateMulticastClient( unsigned int nnum, const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );

	// !�l�b�g���[�N�ڑ����I������B
	/*!
	  \param nnum �I������l�b�g���[�N�ԍ��B
	  */
	virtual int Close( unsigned int nnum );

	// !�ڑ���Ƀf�[�^�𑗂�B
	/*!
	  \param nnum �l�b�g���[�N�ԍ��B
	  \param str ���镶����B
	  */
	virtual int Send( unsigned int nnum, const char *str );
	// !�ڑ���Ƀf�[�^�𑗂�B
	/*!
	  \param nnum �l�b�g���[�N�ԍ��B
	  \param str ���镶����B
	  \param size ����f�[�^�T�C�Y�B
	  */
	virtual int Send( unsigned int nnum, const char *str, unsigned int size );

	// !�ڑ��悩��f�[�^���󂯎��B
	/*!
	  \param nnum �l�b�g���[�N�ԍ��B
	  \param getsize �󂯎�����f�[�^�T�C�Y���i�[����ϐ��ւ̃|�C���^�B
	  \param from �ڑ�����̏����i�[����ϐ��ւ̃|�C���^(UDP�n�̒ʐM�Ɍ���)�B
	  \retval ������|�C���^ �󂯎�����f�[�^(����K�{)�B
	  */
	virtual char * Receive( unsigned int nnum, int *getsize, struct sockaddr *from = NULL );
	// !�ڑ��悩��f�[�^���󂯎��B
	/*!
	  \param nnum �l�b�g���[�N�ԍ��B
	  \param buf �󂯎�����f�[�^���i�[����ϐ��ւ̃|�C���^�B
	  \param size �󂯎��̍ő�T�C�Y�B
	  \param from �ڑ�����̏����i�[����ϐ��ւ̃|�C���^(UDP�n�̒ʐM�Ɍ���)�B
	  */
	virtual int Receive( unsigned int nnum, char *buf, int size, struct sockaddr *from = NULL );

	// !TCP�ł̐ڑ����󂯓����(TCP�T�[�o�[�̂�)�B
	/*!
	  \param nnum �l�b�g���[�N�ԍ��B
	  \param client �ڑ�����̏����i�[����ϐ��ւ̃|�C���^�B
	  */
	virtual SOCKET Accept( unsigned int nnum, struct sockaddr_in *client = NULL );

	// !HTTP�o�R�Ńt�@�C�����_�E�����[�h����(GET)�B
	/*!
	  \param address �_�E�����[�h��A�h���X�B
	  \param filepath �_�E�����[�h�����f�[�^��ۑ�����t�@�C����(NULL=�_�E�����[�h��̃t�@�C����[�f�t�H���g])�B
	  \param getsize �󂯎�����f�[�^�T�C�Y���i�[����ϐ��ւ̃|�C���^(HTTP�w�b�_�𒲍�)�B
	  \param loadsize ���݃��[�h���Ă���f�[�^�T�C�Y���i�[����ϐ��ւ̃|�C���^(�}���`�X���b�h���ɗL��)�B
	  \retval �l HTTP�̃��^�[���R�[�h�B
	  */
	static int HttpGet( const char *address, const char *filepath = NULL, int *getsize = NULL, int *loadsize = NULL );
	//  static int HttpGet( const char *address, const char *filepath = NULL, const char *proxy = NULL, const char *user = NULL, const char *password = NULL, int *getsize = NULL, int *loadsize = NULL );

	// !HTTP�o�R�Ńt�@�C�����_�E�����[�h����(POST)�B
	/*!
	  \param address �_�E�����[�h��A�h���X�B
	  \param data �T�[�o�[��POST�Ƃ��đ���f�[�^�B
	  \param datasize �f�[�^�T�C�Y(�ȗ���������0�ɂ���ƕ�����̒����������I�Ɍv�Z����)�B
	  \param filepath �_�E�����[�h�����f�[�^��ۑ�����t�@�C����(NULL=�_�E�����[�h��̃t�@�C����[�f�t�H���g])�B
	  \param getsize �󂯎�����f�[�^�T�C�Y���i�[����ϐ��ւ̃|�C���^(HTTP�w�b�_�𒲍�)�B
	  \param loadsize ���݃��[�h���Ă���f�[�^�T�C�Y���i�[����ϐ��ւ̃|�C���^(�}���`�X���b�h���ɗL��)�B
	  \retval �l HTTP�̃��^�[���R�[�h�B
	  */  static int HttpPost( const char *address, const char *data = NULL, int datasize = 0, const char *filepath = NULL, int *getsize = NULL, int *loadsize = NULL );

	// MikanIO�̎擾�B
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \retval �|�C���^ MikanIO�N���X�ւ̃|�C���^�B
	  */
	virtual class MikanIO * GetMikanIO( unsigned int ionum );
};

class FileIOManagementPub
{
public:
	// !�J���B
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \param io �J���t�@�C�����B
	  \param mode �J���t�@�C�����[�h("r"=�ǂݍ���[�f�t�H���g], "w"=��������, "rw"=�ǂݏ���, ... ���BC����Ɠ���)�B
	  \retval -1 ���s�B
	  */
	virtual int Open( unsigned int ionum, const char *io, const char *mode = "r" );
	// !1�s�ǂݍ��ށB
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \param read_size �ǂݍ��ݍő�T�C�Y�B
	  \retval �|�C���^ �󂯎�����f�[�^(����K�{)�B
	  */
	virtual char * ReadLine( unsigned int ionum, unsigned int read_size = 0 );
	// !1�s�ǂݍ��ށB
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \param buf �f�[�^���i�[����|�C���^�B
	  \param max_size buf�̍ő�T�C�Y�B
	  \param read_size �ǂݍ��񂾃T�C�Y���i�[����|�C���^�B
	  */
	virtual int ReadLine( unsigned int ionum, char *buf, unsigned int max_size, unsigned int *read_size = NULL );
	// !�������ށB
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \param write_data �������ރf�[�^�B
	  \param weite_size �������݃T�C�Y(-1=NULL�����܂ł̃T�C�Y���v�Z���ď�������[�f�t�H���g])�B
	  */
	virtual int Write( unsigned int ionum, const char *write_data, int write_size = -1 );

	// !�ǂݍ��ށB
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \param read_size �ǂݍ��ݍő�T�C�Y(0=������[�f�t�H���g])�B
	  \param readed_size �ǂݍ��񂾃f�[�^�T�C�Y(�}���`�X���b�h�����ŗL��)�B
	  \retval NULL ���s�B
	  */
	virtual void * Read( unsigned int ionum, unsigned int read_size = 0, int *readed_size = NULL );
	// !�ǂݍ��ށB
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \param data �ǂݍ��݃o�b�t�@�ւ̃|�C���^�B
	  \param read_size �ǂݍ��ݍő�T�C�Y�B
	  \param readed_size �ǂݍ��񂾃f�[�^�T�C�Y(�}���`�X���b�h�����ŗL��)�B
	  \retval -1 ���s�B
	  \retval �l �ǂݍ��񂾃f�[�^�T�C�Y
	  */
	virtual int Read( unsigned int ionum, void *data, unsigned int read_size, int *readed_size = NULL );
	// �V�[�N����B
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \param offset �I�t�Z�b�g�B
	  \param seekmode �V�[�N�J�n�ꏊ(SEEK_CUR=���ݒl����[�f�t�H���g], SEEK_SET=�t�@�C���̐擪, SEEK_END=�t�@�C���̖����BC����Ɠ���)�B
	  \retval -1 ���s�B
	  */
	virtual int Seek( unsigned int ionum, int offset, int seekmode = SEEK_CUR );
	// �������ށB
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \param write_data �������ރf�[�^�B
	  \param write_size �������ރT�C�Y(-1=NULL�����܂ł̃T�C�Y���v�Z���ď�������[�f�t�H���g])�B
	  \retval -1 ���s�B
	  \retval �l �������񂾃T�C�Y�B
	  */
	virtual int Write( unsigned int ionum, const void *write_data, int write_size = -1 );
	// ���`���ď�������(1024�o�C�g�܂ŁB����Ȃ��ꍇ��_MikanFile��Printf���g������)�B
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \param format ���`������
	  \param ... ���`������ŏo�͂�������B
	  \retval -1 ���s�B
	  \retval �l �������񂾃T�C�Y�B
	  */
	virtual int Printf( unsigned int ionum, const char *format, ... );
	// !���܂œǂ񂾃o�C�g����Ԃ��B
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \retval -1 ���s
	  \retval �l ���܂œǂ񂾃o�C�g����Ԃ��B
	  */
	virtual int Tell( unsigned int ionum );
	// !�T�C�Y��Ԃ��B
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \retval -1 ���s�B
	  */
	virtual int Size( unsigned int ionum );
	// !����B
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \retval -1 ���s�B
	  */
	virtual int Close( unsigned int ionum );

	// !IO��j������B
	/*!
	  \param ionum �t�@�C���ԍ��B
	  */
	virtual int Destroy( unsigned int ionum );
	// !���ׂĕ���B
	/*!
	  \retval �l ���邱�Ƃɐ�������IO�̐���Ԃ��B
	  */
	virtual int CloseAll( void );

	static int Remove( const char *file );

	static int Move( const char *oldfile, const char *newfile );

	// FileIO�̎擾�B
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \retval �|�C���^ FileIO�N���X�ւ̃|�C���^�B
	  */
	virtual class FileIO * GetIO( unsigned int ionum );

	// MikanIO�̎擾�B
	/*!
	  \param ionum �t�@�C���ԍ��B
	  \retval �|�C���^ MikanIO�N���X�ւ̃|�C���^�B
	  */
	virtual class MikanIO * GetMikanIO( unsigned int ionum );
};

#endif
