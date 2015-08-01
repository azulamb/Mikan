/*!
@class   Window Window.h "Window.h"
@brief   Window�����Ȃǂ��s���N���X
@author  Hiroki
@version 1.0
*/
#include "Window.h"

/*!
@brief �R���X�g���N�^(�ʏ�)
@par   �֐�����
�ϐ�������������B
*/
Window::Window( void )
{
	wd.wcx.cbSize = sizeof( WNDCLASSEX );
	wd.wcx.hIcon = wd.wcx.hIconSm = NULL;
	strcpy_s( CLASSNAME, WBUFLEN, MIKANCLASSNAME );
	//�N���X�l�[���̐ݒ�
	wd.wcx.lpszClassName = TEXT( MIKANCLASSNAME );//SetClassNameEx( "MikanWindow" );
	//�E�B���h�E�̖��O�̐ݒ�
	SetWindowName( "Window" );//TEXT( __FILE__ )
	//wd.wc.lpszClassName = wd.ClassName;
	//�E�B���h�E�X�^�C���̐ݒ�
	SetWindowStyle( WT_DEFAULT );
	//�g���E�B���h�E�X�^�C���̐ݒ�
	SetWindowStyleExDirect( NULL );
	//�E�B���h�E��W���̈ʒu�ɐݒ�
	SetPositionXY( 0, 0 );//CW_USEDEFAULT, CW_USEDEFAULT );
	//�E�B���h�E�T�C�Y��640*480�ɐݒ�
	//  SetWindowSize( WF_WIDTH  + GetSystemMetrics(SM_CXEDGE) + GetSystemMetrics(SM_CXBORDER) + GetSystemMetrics(SM_CXDLGFRAME),
	//                 WF_HEIGHT + GetSystemMetrics(SM_CYEDGE) + GetSystemMetrics(SM_CYBORDER) + GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION));
	SetScreenSize( WF_WIDTH, WF_HEIGHT );
	//�E�B���h�E�n���h���̐ݒ�
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
@brief �f�X�g���N�^
@par   �֐�����
�������Ă��Ȃ��B
*/
Window::~Window( void )
{
}

int Window::InitWindowClassEx( HINSTANCE hInstance )
{
	wd.wcx.style = CS_HREDRAW | CS_VREDRAW;
	//wcx->lpfnWndProc   = ::WindowProc; //�E�B���h�E�v���V�[�W��
	SetWindowProc( DefWindowProc );
	wd.wcx.cbClsExtra = 0;
	wd.wcx.cbWndExtra = 0;
	wd.wcx.hInstance = wd.hInstance = hInstance;
	wd.wcx.hIcon = LoadIcon( NULL, IDI_APPLICATION ); //�A�C�R��
	//  wc->hIcon         = LoadIcon(hInstance,TEXT("ICON_16"));
	wd.wcx.hCursor = LoadCursor( NULL, IDC_ARROW ); //�J�[�\��
	//  wc->hCursor       = LoadCursor(hInstance,TEXT("MYCURSOR"));//
	wd.wcx.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wd.wcx.lpszMenuName = NULL;
	wd.wcx.hIconSm = LoadIcon( NULL, IDI_APPLICATION ); //�A�C�R��
	return 0;
}

int Window::RegistClassEx( void )
{
	unsigned long atom;
	//�E�B���h�E�N���X�̓o�^
	atom = RegisterClassEx( &( GetWindowClassEx() ) );
	if ( atom == NULL )
	{
		//�o�^���s
		return 1;
	}
	return 0;
}

/*!
@brief �E�B���h�E�̐���
@par   �֐�����
�E�B���h�E�𐶐�����B
���܂����E�B���h�E�T�C�Y��X�^�C��������̂ł���΂��炩����Set�n�֐��Őݒ肵�Ă����B
@param nCmdShow WinMain�֐��Ŏ擾�ł����O�����B
@return 0=�����B
@return 1=�����Ɏ��s�B
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

	// ����B
	//if ( GetPositionX() < 0 ){ SetPositionX( 0 ); w = GetWindowWidth();  SetWindowWidth( 640 );  }
	//if ( GetPositionY() < 0 ){ SetPositionY( 0 ); h = GetWindowHeight(); SetWindowHeight( 480 ); }

	//�E�B���h�E�̐���
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

	//�����Ɏ��s������1��Ԃ��ďI��
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

// �E�B���h�E�ݒ�̃o�b�N�A�b�v�B
int Window::BackUpWindow()
{
	backup = wd;
	return 0;
}

// �E�B���h�E�ݒ�̕��A�B
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
@brief �E�B���h�E�̐ݒ��ύX����
@par   �֐�����
�E�B���h�E�X�^�C����ݒ肷��B
@param Type WT_�Ŏn�܂�萔�B
@return false=�ݒ蔽�f��SetWindowPos�ł̐ݒ莸�s�B
@return true=SetWindowPos�ł̐ݒ萬���B
*/
BOOL Window::SetWindow( int type, int display )
{
	HWND myhwnd = NULL;//�E�B���h�E�n���h��
	RECT myrect;//RECT�\����
	BOOL ret;

	// �f�B�X�v���C�ԍ�����̈������o���Ĉړ����Ă���T�C�Y�ύX�Ƃ����s���悤�ɏ�������

	switch ( type )
	{
	case WT_DEFAULT://�T�C�Y�Œ�̃E�B���h�E
		//�E�B���h�E�X�^�C���̐ݒ�
		SetWindowStyle( WT_DEFAULT );
		//�E�B���h�E�X�^�C���̕ύX
		SetWindowLong( GetWindowHandle(), GWL_STYLE, GetWindowStyle() );
		wd.flags = SWP_SHOWWINDOW;
		wd.winpos = HWND_TOP;
		//�E�B���h�E�̃T�C�Y��ʒu��ύX
		ret = SetWindowPos(
			GetWindowHandle(), wd.winpos,
			GetPositionX(), GetPositionY(),//CW_USEDEFAULT, CW_USEDEFAULT,
			GetWindowWidth(), GetWindowHeight(),
			wd.flags
			);
		SetPositionX( GetPositionX( 1 ) );
		SetPositionY( GetPositionY( 1 ) );
		return ret;
	case WT_NORMAL://�ʏ�̃��T�C�Y�\�E�B���h�E
		//�E�B���h�E�X�^�C���̐ݒ�
		SetWindowStyle( WT_NORMAL );
		//�E�B���h�E�X�^�C���̕ύX
		SetWindowLong( GetWindowHandle(), GWL_STYLE, GetWindowStyle() );
		wd.flags = SWP_SHOWWINDOW;
		//�E�B���h�E�̃T�C�Y��ʒu��ύX
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
		//�E�B���h�E�X�^�C���̐ݒ�
		SetWindowStyle( WT_NOFRAME );

		//�E�B���h�E�X�^�C���̕ύX
		SetWindowLong( GetWindowHandle(), GWL_STYLE, GetWindowStyle() );
		wd.flags = SWP_SHOWWINDOW;
		wd.winpos = HWND_TOP;
		//�E�B���h�E�̃T�C�Y��ʒu��ύX
		return SetWindowPos(
			GetWindowHandle(), wd.winpos,
			GetPositionX(), GetPositionY(),
			GetWindowWidth(), GetWindowHeight(),
			wd.flags
			);
	case WT_NORESIZEFULLSCREEN://��ʐݒ��ς��Ȃ��t���X�N���[��
		//�v���O�������N�������f�X�N�g�b�v�T�C�Y�̃E�B���h�E��
		//�`�ʗD��x����ԏ�̃E�B���h�E�𐶐�����

		//�E�B���h�E�X�^�C���̐ݒ�
		SetWindowStyle( WT_NORESIZEFULLSCREEN );

		// �Ώۃf�B�X�v���C�̃T�C�Y�Ȃǂ��擾�B
		if ( GetDisplayRect( display, &myrect ) == 0 )
		{
			// ���������̂ŁA���܂���ɐݒ�B
			// �ʒu�̓f�B�X�v���C�̍���B
			SetPositionXY( myrect.left, myrect.top );
			// �T�C�Y�̓f�B�X�v���C�ɍ��킹��B
			SetWindowSize( myrect.right - myrect.left, myrect.bottom - myrect.top );
		} else
		{
			// ���s�����̂ŁA�f�X�N�g�b�v����擾�B

			//�f�X�N�g�b�v�̃E�B���h�E�n���h���擾
			myhwnd = GetDesktopWindow();
			//�f�X�N�g�b�v�̃T�C�Y�Ȃǂ��擾
			if ( myhwnd && GetClientRect( myhwnd, &myrect ) )
			{
				//�ʒu�͌��_�ɐݒ�
				SetPositionXY( 0, 0 );
				//�T�C�Y�̓f�X�N�g�b�v�Ɠ����T�C�Y�ɐݒ�
				SetWindowSize( myrect.right, myrect.bottom );
			}
		}

		//�E�B���h�E�X�^�C���̕ύX
		SetWindowLong( GetWindowHandle(), GWL_STYLE, GetWindowStyle() );
		wd.flags = SWP_SHOWWINDOW;
		wd.winpos = HWND_TOPMOST;
		//�E�B���h�E�̃T�C�Y��ʒu��ύX
		return SetWindowPos(
			GetWindowHandle(), wd.winpos,
			GetPositionX(), GetPositionY(),
			GetWindowWidth(), GetWindowHeight(),
			wd.flags
			);

	default:
		//��ōl����
		//���T�C�Y���x
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
@brief �g���E�B���h�E�N���X���̎擾
@par   �֐�����
�g���E�B���h�E�N���X����Ԃ��B
@return �g���E�B���h�E�N���X��(char*�^)�B
*/
const char * Window::GetClassNameEx( void )
{
	//�E�B���h�E�̐ݒ肪�E�B���h�E�N���X�B
	//����ւ̌ʂ̖��O�����̃E�B���h�E�N���X�̖��O�ƂȂ�B
	return CLASSNAME;//wd.ClassName;//wd.ClassName;
}

/*!
@brief �E�B���h�E�N���X�̖��O�̐ݒ�
@par   �֐�����
�E�B���h�E�N���X���̐ݒ���s���B�ݒ蔽�f��SetWindow()�֐����g���B
@param LPCTSTR name TCHAR�^�����z��̃E�B���h�E�N���X��
*/
/*int Window::SetClassNameEx( char * name )
{
wd.ClassName = name;
return 0;
}*/

/*!
@brief �E�B���h�E�̖��O�̎擾
@par   �֐�����
�E�B���h�E�̖��O��Ԃ��B
@return ���݂̃E�B���h�E��(char*�^)�B
*/
char * Window::GetWindowName( void )
{
	//char�ŕԂ��̂�Mikan�ɔC����
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
@brief �E�B���h�E�̖��O�̐ݒ�
@par   �֐�����
�E�B���h�E�̖��O��ݒ肷��B���ݒ�f��SetWindow()�֐����g���B
@param name TCHAR�^�����z��̃E�B���h�E�N���X��
@return 0=�����B
@return 1=�^����������ւ̃|�C���^��NULL�B
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
@brief �E�B���h�E�X�^�C���̎擾
@par   �֐�����
�E�B���h�E�X�^�C�����擾����B
@return ���݂̃E�B���h�E�X�^�C���B
*/
unsigned long Window::GetWindowStyle( void )
{
	return wd.WindowStyle;
}

/*!
@brief �E�B���h�E�X�^�C���̐ݒ�
@par   �֐�����
�E�B���h�E�N�X�^�C���̐ݒ���s���B
�E�B���h�E�X�^�C���𒼐ݒ肵�����ꍇ��SetWindowStyleDirect()�֐����g���B
�ݒ蔽�f��SetWindow()�֐����g���B
@param wt WT_����n�܂�萔�B
@return NULL=wt���s���B��������SetWindowStyleDirect�̕Ԃ�l�B
@return ���̑�=SetWindowStyleDirect�̕Ԃ�l�B
*/
unsigned long Window::SetWindowStyle( enum WINDOWTYPE wt )
{
	fullscreen = 0;
	//WINDOWTYPE�ŕ���
	switch ( wt )
	{
	case WT_DEFAULT://���T�C�Y�s�E�B���h�E
		return SetWindowStyleDirect( WS_DEFAULT );
	case WT_NORMAL://���T�C�Y�\�E�B���h�E
		return SetWindowStyleDirect( WS_NORMAL );
	case WT_NORESIZEFULLSCREEN://��ʐݒ��ύX���Ȃ��t���X�N���[��
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
@brief �E�B���h�E�X�^�C���̒��ݒ�
@par   �֐�����
�E�B���h�E�N�X�^�C���̐ݒ���s���B
�ݒ蔽�f��SetWindow()�֐����g���B
@param ws WS_����n�܂�萔�̑g�ݍ��킹�B�ڂ�����MSDN���ŁB
@return �ݒ肵���E�B���h�E�X�^�C���B
*/
unsigned long Window::SetWindowStyleDirect( unsigned long ws )
{
	wd.WindowStyle = ws;
	return ws;
}

/*!
@brief �g���E�B���h�E�X�^�C���̎擾
@par   �֐�����
�g���E�B���h�E�N�X�^�C���̐ݒ���擾����B
@return �g���E�B���h�E�X�^�C���B
*/
unsigned long Window::GetWindowStyleEx( void )
{
	return wd.WindowStyleEx;
}

/*!
@brief �g���E�B���h�E�X�^�C���̒��ݒ�
@par   �֐�����
�g���E�B���h�E�N�X�^�C���̐ݒ���s���B
�ݒ蔽�f��SetWindow()�֐����g���B
@param unsigned long ws �g���E�B���h�E�X�^�C���̒萔�̑g�ݍ��킹�B
@return �ݒ肵���g���E�B���h�E�X�^�C���B
*/
unsigned long Window::SetWindowStyleExDirect( unsigned long wse )
{
	wd.WindowStyleEx = wse;
	return wse;
}

/*!
@brief �E�B���h�E�̉������擾
@par   �֐�����
�E�B���h�E�̉������擾����B
@param int type �����l=0�B0�Őݒ�l��Ԃ��A1�Ō��ݒl�𒲂ׂĕԂ��B
@return �E�B���h�E�̉����B
*/
unsigned int Window::GetWindowWidth( int type )
{
	//type=0:�ݒ�l,1=���ݒl
	RECT myrect;//RECT�\����

	if ( type )
	{
		GetClientRect( GetWindowHandle(), &myrect );
		return myrect.right - myrect.left;
	}

	return wd.Width;
}

/*!
@brief �E�B���h�E�̍������擾
@par   �֐�����
�E�B���h�E�̍������擾����B
@param int type �����l=0�B0�Őݒ�l��Ԃ��A1�Ō��ݒl�𒲂ׂĕԂ��B
@return �E�B���h�E�̍���
*/
unsigned int Window::GetWindowHeight( int type )
{
	//type=0:�ݒ�l,1=���ݒl
	RECT myrect;//RECT�\����

	if ( type )
	{
		GetClientRect( GetWindowHandle(), &myrect );
		return myrect.bottom - myrect.top;
	}

	return wd.Height;
}

/*!
@brief �E�B���h�E�̉����ݒ�
@par   �֐�����
�E�B���h�E�̉�����ݒ肷��B�ݒ蔽�f��SetWindow()�֐����g���B
@param unsigned int width ������^����B�ɒ[�Ȓl���ƕʂ̒l�ɂȂ�B
@return �ݒ肵�������B
*/
unsigned int Window::SetWindowWidth( unsigned int width )
{
	int w = 0;
	//unsigned�Ȃ̂ň�����0�Ȃ�l�������ς���
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
	//���ۂɑ�������l��Ԃ�
	return width;
}

/*!
@brief �E�B���h�E�̍����ݒ�
@par   �֐�����
�E�B���h�E�̍�����ݒ肷��B�ݒ蔽�f��SetWindow()�֐����g���B
@param unsigned int height ������^����B�ɒ[�Ȓl���ƕʂ̒l�ɂȂ�B
@return �ݒ肵�������B
*/
unsigned int Window::SetWindowHeight( unsigned int height )
{
	//unsigned�Ȃ̂ň�����0�Ȃ�l�������ς���
	if ( height <= 0 )
	{
		height = WF_HEIGHT;
	}
	wd.Height = height;
	return height;
}

/*!
@brief �E�B���h�E�̃T�C�Y�ݒ�
@par   �֐�����
�E�B���h�E�̃T�C�Y��ݒ肷��B�ݒ蔽�f��SetWindow()�֐����g���B
@param unsigned int width ������^����B
@param unsigned int height ������^����B
@return 0=�ݒ�l�����������ƈقȂ�
@return 1=�����͐ݒ�l���قȂ邪�����̐ݒ�l�͈����ʂ�
@return 2=�����͐ݒ�l�������ʂ肾�������̐ݒ�l�͈قȂ�
@return 3=�ݒ�l�����������Ɠ���
*/
int Window::SetWindowSize( unsigned int width, unsigned int height )
{
	return ( SetWindowWidth( width ) == width ) * 2 +
		( SetWindowHeight( height ) == height );
}

/*!
@brief �`��̈悩��E�B���h�E�̃T�C�Y�ݒ�
@par   �֐�����
�E�B���h�E�̃T�C�Y��`��̈����ɂ��Đݒ肷��B�ݒ蔽�f��SetWindow()�֐����g���B
@param unsigned int width ������^����B
@param unsigned int height ������^����B
@return 0=�ݒ�l�����������ƈقȂ�
@return 1=�����͐ݒ�l���قȂ邪�����̐ݒ�l�͈����ʂ�
@return 2=�����͐ݒ�l�������ʂ肾�������̐ݒ�l�͈قȂ�
@return 3=�ݒ�l�����������Ɠ���
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
@brief ���݂̃E�B���h�E��X���W���擾
@par   �֐�����
�E�B���h�E��X���W���擾����B
@param int type �����l=0�B0�Őݒ�l��Ԃ��A1�Ō��ݒl�𒲂ׂĕԂ��B
@return ������̈ʒu�B
*/
int Window::GetPositionX( int type )
{
	//type=0:�ݒ�l,1=���ݒl
	RECT myrect;//RECT�\����

	if ( type )
	{
		GetClientRect( GetWindowHandle(), &myrect );
		return myrect.left;
	}

	return wd.PositionX;
}

/*!
@brief ���݂̃E�B���h�E��Y���W���擾
@par   �֐�����
�E�B���h�E��Y���W���擾����B
@param int type �����l=0�B0�Őݒ�l��Ԃ��A1�Ō��ݒl�𒲂ׂĕԂ��B
@return �ォ��̈ʒu�B
*/
int Window::GetPositionY( int type )
{
	//type=0:�ݒ�l,1=���ݒl
	RECT myrect;//RECT�\����

	if ( type )
	{
		GetClientRect( GetWindowHandle(), &myrect );
		return myrect.top;
	}

	return wd.PositionY;
}

/*!
@brief �E�B���h�E��X���W�ݒ�
@par   �֐�����
�E�B���h�E��X���W��ݒ肷��B�ݒ蔽�f��SetWindow()�֐����g���B
@param unsigned int x X���W��^����B
@return �ݒ肵���l�B
*/
int Window::SetPositionX( int x )
{
	wd.PositionX = x;
	//���ۂɑ�������l��Ԃ�
	return x;
}

/*!
@brief �E�B���h�E��Y���W�ݒ�
@par   �֐�����
�E�B���h�E��Y���W��ݒ肷��B�ݒ蔽�f��SetWindow()�֐����g���B
@param unsigned int y Y���W��^����B
@return �ݒ肵���l�B
*/
int Window::SetPositionY( int y )
{
	wd.PositionY = y;
	return y;
}

/*!
@brief �ݒ�E�B���h�E�̍��W�ݒ�
@par   �֐�����
�E�B���h�E�̍��W��ݒ肷��B�ݒ蔽�f��SetWindow()�֐����g���B
@param unsigned int x X���W��^����B
@param unsigned int y Y���W��^����B
@return 0��Ԃ��B
*/
int Window::SetPositionXY( int x, int y )
{
	SetPositionX( x );
	SetPositionY( y );
	return 0;
}

//�f�B�X�v���C�̐���Ԃ�
int Window::GetDisplayMax( void )
{
	return GetSystemMetrics( SM_CMONITORS );
}

// �f�B�X�v���C�̔ԍ���Ԃ�
int Window::GetDisplayNumber( HWND window )
{
	int max, i, ret = 0, area[ 2 ] = { 0, 0 };
	RECT win, disp;

	if ( window == NULL )
	{
		return 0;
	}

	// �E�B���h�E�̈ʒu�ƃT�C�Y��RECT�Ŏ擾
	GetWindowRect( window, &win );

	// �f�B�X�v���C�̐�
	max = GetDisplayMax();

	for ( i = 0; i < max; ++i )
	{
		GetDisplayRect( i, &disp );
		// ���������������ĂȂ��B
		if ( win.right < disp.left ||
			disp.right < win.left ||
			win.bottom < disp.top ||
			disp.bottom < win.top )
		{
			continue;
		}

		// �E�B���h�E�̓f�B�X�v���C�̒� or �ꕔ�����d�Ȃ��Ă���B
		// �f�B�X�v���C��RECT���E�B���h�E�Əd�Ȃ��Ă��镔���̑傫���ɒu���B
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

		// RECT�̖ʐς��v�Z�B
		area[ 1 ] = ( disp.right - disp.left ) * ( disp.bottom - disp.top );
		if ( area[ 0 ] < area[ 1 ] )
		{
			area[ 0 ] = area[ 1 ];
			ret = i;
		}
	}

	return ret;
}

// �f�B�X�v���C�̃T�C�Y��Ԃ�
int Window::GetDisplaySize( long displaynum, int *width, int *height )
{
	return GetDisplayRect( displaynum, NULL, NULL, width, height );
}

// �f�B�X�v���C�̈ʒu�ƃT�C�Y��Ԃ�
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
		// �s���ȃf�B�X�v���C�ԍ�
		return 1;
	}

	memset( &devmode, 0, sizeof( DEVMODE ) );
	devmode.dmSize = sizeof( DEVMODE );
	if ( EnumDisplaySettingsEx( disdev.DeviceName, /*ENUM_CURRENT_SETTINGS*/ENUM_REGISTRY_SETTINGS, &devmode, EDS_RAWMODE ) == 0 )
	{
		// �ݒ���擾�ł��Ȃ�����
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
	HWND myhwnd; //�E�B���h�E�n���h��
	RECT myrect; //RECT�\����

	//�f�X�N�g�b�v�̃E�B���h�E�n���h���擾
	myhwnd = GetDesktopWindow();

	//�f�X�N�g�b�v�̃T�C�Y�Ȃǂ��擾
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
@brief �E�B���h�E�n���h����Ԃ�
@par   �֐�����
�E�B���h�E�n���h�����擾����B
@return ���݂̃E�B���h�E�n���h���B
*/
HWND Window::GetWindowHandle( void )
{
	return wd.WindowHandle;
}

/*!
@brief �E�B���h�E�n���h���̐ݒ�
@par   �֐�����
�E�B���h�E�n���h����ݒ肷��B
@param HWND wh �E�B���h�E�n���h��
@return �ݒ肵���E�B���h�E�n���h���B
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
@brief �E�B���h�E�n���h���̐ݒ�
@par   �֐�����
�E�B���h�E�n���h����ݒ肷��B
@param HWND wh �E�B���h�E�n���h��
@return �ݒ肵���E�B���h�E�n���h���B
*/
HWND Window::SetParentWindowHandle( HWND wh )
{
	wd.WindowParent = wh;
	return wd.WindowParent;
}

/*!
@brief �E�B���h�E�N���X��Ԃ�
@par   �֐�����
�E�B���h�E�N���X���擾����B
@return �E�B���h�E�N���X�B
*/
WNDCLASSEX Window::GetWindowClassEx( void )
{
	return wd.wcx;
}

/*!
@brief �E�B���h�E�N���X�̐ݒ�
@par   �֐�����
�E�B���h�E�N���X��ݒ肷��B
@param WNDCLASS wc �E�B���h�E�N���X
@return �ݒ肵���E�B���h�E�N���X�B
*/
WNDCLASSEX Window::SetWindowClassEx( WNDCLASSEX wcx )
{
	wd.wcx = wcx;
	return wd.wcx;
}

/*!
@brief ���j���[�n���h���̎擾
@par   �֐�����
���j���[�n���h�����擾����B
@return ���j���[�n���h���B
*/
HMENU Window::GetMenuHandle( void )
{
	return wd.hMenu;
}

/*!
@brief �C���X�^���X�n���h���̎擾
@par   �֐�����
�C���X�^���X�n���h�����擾����B
@return �C���X�^���X�n���h���B
*/
HINSTANCE Window::GetInstanceHandle( void )
{
	return wd.hInstance;
}

// �E�B���h�E�v���V�[�W���̐ݒ�
int Window::SetWindowProc( WNDPROC winproc )
{
	wd.wcx.lpfnWndProc = winproc;
	return 0;
}

/*!
@brief �p�����[�^�[�̎擾
@par   �֐�����
�p�����[�^�[���擾����B
@return �p�����[�^�[�B
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
