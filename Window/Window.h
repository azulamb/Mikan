#ifndef WT_DEFAULT

#ifndef UNUSE_RAW_INPUT
// �჌�x�����͂��g��
#ifndef WINVER
#define WINVER (0x500)        // RawInput�p
#define _WIN32_WINNT 0x0501 // RawInput�p
#endif
#endif
#include <winsock2.h>
#include <windows.h>

#ifndef _UNLINK_LIBRARY
#  pragma comment( lib, "winmm.lib" )
#  ifdef _MIKANDEV
#    ifdef _DEBUG
#      pragma comment( lib, "Dev/WindowD.lib" )
#    else
#      pragma comment( lib, "Dev/Window.lib" )
#    endif
#  else
#    ifdef _DEBUG
#      pragma comment( lib, "WindowD.lib" )
#    else
#      pragma comment( lib, "Window.lib" )
#    endif
#  endif
#endif

//�E�B���h�E�^�C�v�̐ݒ�
enum WINDOWTYPE
{
	WT_RENEWAL = 0,            // ���݂̐ݒ���ێ�
	WT_DEFAULT = 1,            // ���T�C�Y�s�̃E�B���h�E
	WT_NORMAL = 2,              // ���T�C�Y�Ȃǂ��\�ȃE�B���h�E
	WT_NORESIZEFULLSCREEN = 3, // ��ʐݒ��ύX���Ȃ��t���X�N���[��
	WT_NOFRAME = 4,            // �g�̂Ȃ��E�B���h�E
	WT_FULLSCREEN = 5,         // �t���X�N���[��
};

//�E�B���h�E�X�^�C���̒�`
//#define WS_DEFAULT WS_OVERLAPPED|WS_VISIBLE |WS_BORDER | WS_CAPTION
//#define WS_DEFAULT WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
//#define WS_DEFAULT WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
#define WS_DEFAULT            WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
#define WS_NORMAL             WS_OVERLAPPEDWINDOW | WS_VISIBLE
#define WS_NORESIZEFULLSCREEN WS_POPUP | WS_VISIBLE
#define WS_NOFRAME            WS_NORESIZEFULLSCREEN
#define WS_FULLSCREEN         WS_NORESIZEFULLSCREEN

//�E�B���h�E�̏����l
#define WF_WIDTH 640
#define WF_HEIGHT 480

//�^�����
typedef struct WINDOWDATA_ WINDOWDATA;
typedef WINDOWDATA * WindowData;

#define WBUFLEN 256
#define MIKANCLASSNAME "MIKANWINDOW"
/*!
@struct  WINDOWDATA_
@brief   �E�B���h�E�����Ȃǂ̕ϐ����W�߂��\����
@author  Hiroki
@version 1.0
*/
struct WINDOWDATA_
{
	//char          *ClassName;//[ WBUFLEN ]; //
	//char          ClassNameW[ WBUFLEN ]; //
	char          WindowName[ WBUFLEN ];    //
	unsigned long WindowStyle;    //�E�B���h�E�X�^�C��
	unsigned long WindowStyleEx;  //�g���E�B���h�E�X�^�C��
	int           PositionX;      //�E�B���h�Ex���W
	int           PositionY;      //�E�B���h�Ey���W
	unsigned int  Width;          //�`�ʗ̈�̉���
	unsigned int  Height;         //�`�ʗ̈�̍���
	//unsigned int WindowWidth;   //�E�B���h�E�S�̂̉���
	//unsigned int WindowHeight;  //�E�B���h�E�S�̂̍���
	HWND          WindowHandle;   //�E�B���h�E�n���h��
	HWND          WindowParent;   //�e�E�B���h�E�n���h��
	HDC           Screen;         //���z���(�������f�o�C�X)
	WNDCLASSEX    wcx;            //�g���E�B���h�E�N���X
	HMENU         hMenu;          //
	HINSTANCE     hInstance;      //
	LPVOID        lpParam;        //
	UINT          flags;
	HWND          winpos;
};

#define CLASSDEF_WINDOW class Window

//�E�B���h�E�N���X
class Window
{
protected:
	HDROP drophandle;
	int( *ReceiveDragFiles_ )( int files );
	int dragfilesmax;
	char dragfilepath[ MAX_PATH ];
	int fullscreen;
public:
	char CLASSNAME[ WBUFLEN ];
	//�\����
	WINDOWDATA wd, backup;

	//�R���X�g���N�^
	Window( void );
	//�f�X�g���N�^
	virtual ~Window( void );
	//
	virtual int InitWindowClassEx( HINSTANCE hInstance );
	virtual int RegistClassEx( void );
	//�E�B���h�E�̐���
	virtual unsigned long MakeWindow( int nCmdShow );

	// �E�B���h�E�ݒ�̃o�b�N�A�b�v�B
	virtual int BackUpWindow();
	// �E�B���h�E�ݒ�̕��A�B
	virtual int RepairWindow();
	//�E�B���h�E�̐ݒ�ύX
	virtual BOOL SetWindow( int type = WT_RENEWAL, int display = 0 );

	//�E�B���h�E�N���X�̖��O�̎擾
	virtual const char * GetClassNameEx( void );
	//�E�B���h�E�N���X�̖��O�̐ݒ�(���f��SetWindow()�֐�)
	//virtual int SetClassNameEx( char * name );

	//�E�B���h�E�̖��O�̎擾
	virtual char * GetWindowName( void );
	//�E�B���h�E�̖��O�̐ݒ�(���f��SetWindow()�֐�)
	virtual int SetWindowName( char * name );
	//�E�B���h�E�̃A�C�R���ݒ�
	virtual int SetWindowIcon( int smalliconnum, int bigiconnum );
	virtual int SetWindowIcon( char *smalliconname, char *bigiconname );

	//�E�B���h�E�X�^�C���̎擾
	virtual unsigned long GetWindowStyle( void );
	//�E�B���h�E�X�^�C���̐ݒ�(���f��SetWindow()�֐�)
	virtual unsigned long SetWindowStyle( enum WINDOWTYPE wt );
	//�E�B���h�E�X�^�C���̒��ݒ�(���f��SetWindow()�֐�)
	virtual unsigned long SetWindowStyleDirect( unsigned long ws );

	//�g���E�B���h�E�X�^�C���̎擾
	virtual unsigned long GetWindowStyleEx( void );
	//�g���E�B���h�E�X�^�C���̒��ݒ�(���f��SetWindow()�֐�)
	virtual unsigned long SetWindowStyleExDirect( unsigned long wse );

	//���݂̃E�B���h�E�̉������擾
	virtual unsigned int GetWindowWidth( int type = 0 );
	//���݂̃E�B���h�E�̍������擾
	virtual unsigned int GetWindowHeight( int type = 0 );
	//�E�B���h�E�̉����ݒ�(���f��SetWindow()�֐�)
	virtual unsigned int SetWindowWidth( unsigned int width );
	//�E�B���h�E�̍����ݒ�(���f��SetWindow()�֐�)
	virtual unsigned int SetWindowHeight( unsigned int height );
	//�E�B���h�E�̃T�C�Y�ݒ�
	virtual int SetWindowSize( unsigned int width, unsigned int height );
	//�E�B���h�E�̕`��̈悩��E�B���h�E�̃T�C�Y�ݒ�
	virtual int SetScreenSize( unsigned int width, unsigned int height );

	//���݂̃E�B���h�E��X���W���擾
	virtual int GetPositionX( int type = 0 );
	//���݂̃E�B���h�E��Y���W���擾
	virtual int GetPositionY( int type = 0 );
	//�E�B���h�E��X���W�ݒ�(���f��SetWindow()�֐�)
	virtual int SetPositionX( int x );
	//�E�B���h�E��Y���W�ݒ�(���f��SetWindow()�֐�)
	virtual int SetPositionY( int y );
	//�E�B���h�E�̍��W�ݒ�
	virtual int SetPositionXY( int x, int y );

	//�f�B�X�v���C�̐���Ԃ�
	static int GetDisplayMax( void );
	// ���̃E�B���h�E�����݂��Ă���f�B�X�v���C�̔ԍ���Ԃ��B
	// �����f�B�X�v���C�ɂ���ꍇ�̓E�B���h�E����ԑ傫�������Ă���f�B�X�v���C�ɂȂ�B
	static int GetDisplayNumber( HWND window = NULL );
	// �f�B�X�v���C�̃T�C�Y��Ԃ�
	static int GetDisplaySize( long displaynum, int *width = NULL, int *height = NULL );
	// �f�B�X�v���C�̈ʒu�ƃT�C�Y��Ԃ�
	static int GetDisplayRect( long displaynum, int *x = NULL, int *y = NULL, int *width = NULL, int *height = NULL );
	static int GetDisplayRect( long displaynum, RECT *rect );
	static int GetDesktopWidth( void );
	static int GetDesktopHeight( void );
	static int GetDesktopSize( int *width, int *height );
	//�E�B���h�E�n���h����Ԃ�
	virtual HWND GetWindowHandle( void );
	//�E�B���h�E�n���h���̐ݒ�
	virtual HWND SetWindowHandle( HWND wh );
	//�e�E�B���h�E�n���h����Ԃ�
	virtual HWND GetParentWindowHandle( void );
	//�e�E�B���h�E�n���h���̐ݒ�
	virtual HWND SetParentWindowHandle( HWND wh );

	//�E�B���h�E�N���X��Ԃ�
	virtual WNDCLASSEX GetWindowClassEx( void );
	//�E�B���h�E�N���X�̐ݒ�
	virtual WNDCLASSEX SetWindowClassEx( WNDCLASSEX wc );

	//
	virtual HMENU GetMenuHandle( void );
	//
	virtual HINSTANCE GetInstanceHandle( void );
	// �E�B���h�E�v���V�[�W���̐ݒ�
	virtual int SetWindowProc( WNDPROC winproc );
	//
	virtual LPVOID GetParameter( void );

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
	virtual int ReceiveDragFiles( HDROP hdrop );
};

#endif
