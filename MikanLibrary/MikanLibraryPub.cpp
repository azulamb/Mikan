#include "MikanLibrary.h"
//����J�N���X
extern CLASSDEF_SYSTEM              *_MikanSystem;
extern CLASSDEF_WINDOW              *_MikanWindow;
extern CLASSDEF_DIRECTX_DRAW        *_MikanDraw;
extern CLASSDEF_MIKANMOVIE          *_MikanMovie;
extern CLASSDEF_DIRECTX_INPUT       *_MikanInput;
#ifndef MIKAN_NO_SOUND
extern CLASSDEF_DIRECTX_SOUND       *_MikanSound;
#endif
extern CLASSDEF_NETWORK             *_MikanNet;
extern class FileIOManagement       *_MikanFile;

#define MDRAW_X(x) ( MikanDrawPub::base_x + x )
#define MDRAW_Y(y) ( MikanDrawPub::base_y + y )

int MikanDrawPub::base_x = 0;
int MikanDrawPub::base_y = 0;

/*!
@class   MikanSystemPub
@brief   MikanSystem�N���X�B�Q�[���ɕK�v�ȏ������s���B
@author  Hiroki
@version 1.0
*/

/*!
@brief MikanSystem�̃R���X�g���N�^
@par   �֐�����
�������Ȃ��B
*/
MikanSystemPub::MikanSystemPub( void ){}
/*!
@brief MikanSystem�̃f�X�g���N�^
@par   �֐�����
�������Ȃ��B
*/
MikanSystemPub::~MikanSystemPub( void ){}
/*!
@brief FPS��ݒ肷��B
@par   �֐�����
FPS��ݒ肷��B
@param fps �ݒ肷��FPS�B1�b�Ԃɉ�����s���邩�B�ȗ������ꍇ60.0�B
@return �ݒ肵���l�B
*/
double MikanSystemPub::SetFPS( double fps ){
	return _MikanSystem->SetFPS( fps );
}
/*!
@brief FPS���擾����B
@par   �֐�����
����FPS���v�����ĕԂ��B
@return ���݂�FPS�B�v�������l���o��B�����ƒႢ�l���o���ꍇ�͏����������N�����Ă���B
*/
double MikanSystemPub::GetFPS( void ){
	return _MikanSystem->GetFPS() + 0.5;
}
/*!
@brief ���̃t���[���܂ő҂�
@par   �֐�����
���s����ƑO�̎��ԂȂǂƔ�r���A�ǂꂾ���x�߂΂悢�����v�Z���A���̎��Ԃ̕�����Sleep����B
����ő҂��Ƃɂ���āAFPS������s�Ȃ��Ă���B
@return Sleep��������(�~���b)�B
*/
int MikanSystemPub::WaitNextFrame( int draw ){
	return _MikanSystem->WaitNextFrame( draw );
}
//�҂��֐��̐ݒ�
int MikanSystemPub::SetWaitFunction( int type ){
	return _MikanSystem->SetWaitFunction( type );
}
/*!
@brief �f�o�C�X���X�g���N�����Ă��邩�ǂ������ׂ�
@par   �֐�����
�����݃f�o�C�X���X�g���N�����Ă��邩�ǂ����𒲂ׂ�B
�������A���s����MainLoop���̃v���O�����͎��s����Ȃ����߁A�ʎ�i��p���Ȃ���ΈӖ��������B
@return 0=����B
@return ���̑�=�f�o�C�X���X�g��
*/
int MikanSystemPub::IsDeviceLost( void ){
	return _MikanSystem->IsDeviceLost();
}

/*!
@brief ���������҂��̐ݒ�
@par   �֐�����
����������҂��ǂ����̐ݒ�BSystemInit���O�Ɏ��s���Ȃ��Ǝ��s����B
@param vs 0�ȊO�Ő��������҂����A0�ő҂��Ȃ��B-1�͓���ȕԂ�l�Ȃ̂Ŏg��Ȃ����Ƃ������߂�B
@return -1 ���s�B
@return ���̑� �ݒ肵���l���Ԃ��Ă���B
*/
int MikanSystemPub::SetVsyncWait( int vs )
{
	if ( _MikanSystem->GetMikanPosition() <= MIKAN_POSITION_SYSTEMINIT )
	{
		return _MikanSystem->SetVsyncWait( vs );
	}
	return -1;
}
int MikanSystemPub::SetFullScreen( int fullscreen_on ){
	return _MikanSystem->SetFullScreen( fullscreen_on );
}
/*!
@brief �E�B���h�E��A�N�e�B�u���̓���ݒ�
@par   �֐�����
�E�B���h�E����A�N�e�B�u�ɂȂ����Ƃ��̋����ɂ��Đݒ肷��B
@param flag 0�Ŕ�A�N�e�B�u��������𑱂��A����ȊO�̒l�Ŕ�A�N�e�B�u���ɓ�����~�߂�B
@return 0 ��A�N�e�B�u���ɃQ�[���𓮍��������B
@return 1 ��A�N�e�B�u���ɃQ�[���̓�����~�߂�B
*/
int MikanSystemPub::SetInactiveWindow( int flag ){
	return _MikanSystem->SetInactiveWindow( flag );
}

//�X���b�h�𗧂ĂĊ֐������s����
int MikanSystemPub::RunThread( unsigned int tnum, int( *thread_func )( void * vparam ), void *param ){
	return _MikanSystem->RunThread( tnum, thread_func, param );
}

//�X���b�h�𗧂ĂĊ֐������s����
int MikanSystemPub::RunThread( unsigned int tnum, int( *thread_func )( void ) ){
	return _MikanSystem->RunThread( tnum, thread_func );
}

//���Ă��X���b�h��j������
int MikanSystemPub::StopThread( unsigned int tnum ){
	return _MikanSystem->StopThread( tnum );
}
void * MikanSystemPub::GetThreadHandle( unsigned int tnum ){
	return _MikanSystem->GetThreadHandle( tnum );
}
int MikanSystemPub::Join( unsigned int tnum, unsigned long sleep ){
	return _MikanSystem->Join( tnum, sleep );
}
int MikanSystemPub::LockMainLoop( void ){
	return _MikanSystem->LockMainLoop();
}
int MikanSystemPub::UnlockMainLoop( void ){
	return _MikanSystem->UnlockMainLoop();
}
int MikanSystemPub::WaitClearScreen( int value ){
	return _MikanSystem->WaitClearScreen( value );
}
int MikanSystemPub::CreateLock( unsigned int lnum ){
	return _MikanSystem->CreateLock( lnum );
}
unsigned long MikanSystemPub::Lock( unsigned int lnum, unsigned long timeout ){
	return _MikanSystem->Lock( lnum, timeout );
}
int MikanSystemPub::Unlock( unsigned int lnum ){
	return _MikanSystem->Unlock( lnum );
}

/*!
@class   MikanWindowPub
@brief   MikanWindow�N���X�B�E�B���h�E����Ȃǂ��s���B
@author  Hiroki
@version 1.0
*/

/*!
@brief MikanWindow�̃R���X�g���N�^
@par   �֐�����
�������Ȃ��B
*/
MikanWindowPub::MikanWindowPub(){}
/*!
@brief MikanWindow�̃f�X�g���N�^
@par   �֐�����
�������Ȃ��B
*/
MikanWindowPub::~MikanWindowPub(){}
//BOOL MikanWindowPub::SetWindow( int type ){ return _MikanWindow->SetWindow( type ); }
/*!
@brief �E�B���h�E�̖��O�̎擾
@par   �֐�����
�E�B���h�E�̖��O��Ԃ��B
@return ���݂̃E�B���h�E��(char*�^)�B
*/
char * MikanWindowPub::GetWindowName( void ){
	return _MikanWindow->GetWindowName();
}
/*!
@brief �E�B���h�E�̖��O�̐ݒ�
@par   �֐�����
�E�B���h�E�̖��O��ݒ肷��B���ݒ�f��SetWindow()�֐����g���B
@param name TCHAR�^�����z��̃E�B���h�E�N���X��
@return 0=�����B
@return 1=�^����������ւ̃|�C���^��NULL�B
*/
int MikanWindowPub::SetWindowName( char * name )
{
	int ret = _MikanWindow->SetWindowName( name );
	//�E�B���h�E�ݒ�̗L����
	_MikanWindow->SetWindow();
	return ret;
}
//�E�B���h�E�̃A�C�R���ݒ�
int MikanWindowPub::SetWindowIcon( int iconnum ){
	return _MikanWindow->SetWindowIcon( iconnum, iconnum );
}
int MikanWindowPub::SetWindowIcon( int smalliconnum, int bigiconnum ){
	return _MikanWindow->SetWindowIcon( smalliconnum, bigiconnum );
}
int MikanWindowPub::SetWindowIcon( char *iconname ){
	return _MikanWindow->SetWindowIcon( iconname, iconname );
}
int MikanWindowPub::SetWindowIcon( char *smalliconname, char *bigiconname ){
	return _MikanWindow->SetWindowIcon( smalliconname, bigiconname );
}
/*!
@brief �E�B���h�E�̉������擾
@par   �֐�����
�E�B���h�E�̉������擾����B
@param int type �����l=0�B0�Őݒ�l��Ԃ��A1�Ō��ݒl�𒲂ׂĕԂ��B
@return �E�B���h�E�̉����B
*/
unsigned int MikanWindowPub::GetWindowWidth( int type ){
	return _MikanWindow->GetWindowWidth( type );
}
/*!
@brief �E�B���h�E�̍������擾
@par   �֐�����
�E�B���h�E�̍������擾����B
@param int type �����l=0�B0�Őݒ�l��Ԃ��A1�Ō��ݒl�𒲂ׂĕԂ��B
@return �E�B���h�E�̍���
*/
unsigned int MikanWindowPub::GetWindowHeight( int type ){
	return _MikanWindow->GetWindowHeight( type );
}
/*!
@brief �E�B���h�E�̃T�C�Y�ݒ�
@par   �֐�����
�E�B���h�E�̃T�C�Y��ݒ肷��B�����ł�SetScreenSize���g���Ă���B
@param unsigned int width ������^����B
@param unsigned int height ������^����B
@return 0=�ݒ�l�����������ƈقȂ�
@return 1=�����͐ݒ�l���قȂ邪�����̐ݒ�l�͈����ʂ�
@return 2=�����͐ݒ�l�������ʂ肾�������̐ݒ�l�͈قȂ�
@return 3=�ݒ�l�����������Ɠ���
*/
int MikanWindowPub::SetWindowSize( unsigned int width, unsigned int height )
{
	//  int ret = _MikanWindow->SetWindowSize( width, height );
	//�X�N���[���̃T�C�Y����E�B���h�E�T�C�Y�����肷��B
	int ret = _MikanWindow->SetScreenSize( width, height );
	//�E�B���h�E�̐ݒ�̗L�����B
	_MikanWindow->SetWindow();
	//UserInit�ȍ~�Ŏ��s���ꂽ�ꍇ�̓f�o�C�X���X�g���������������I�ɍs���B
	//����ɂ��DirectX�̃f�o�C�X����蒼�����B
	if ( _MikanSystem->GetMikanPosition() > MIKAN_POSITION_LIBLARYINIT )
	{
		//�f�o�C�X���X�g���������������I�ɍs���B
		_MikanDraw->RecoverFromDeviceLost( 1 );
	}
	return ret;
}

/*!
@brief ���݂̃E�B���h�E��X���W���擾
@par   �֐�����
�E�B���h�E��X���W���擾����B
@param int type �����l=0�B0�Őݒ�l��Ԃ��A1�Ō��ݒl�𒲂ׂĕԂ��B
@return ������̈ʒu�B
*/
int MikanWindowPub::GetPositionX( int type ){
	return _MikanWindow->GetPositionX( type );
}
/*!
@brief ���݂̃E�B���h�E��Y���W���擾
@par   �֐�����
�E�B���h�E��Y���W���擾����B
@param int type �����l=0�B0�Őݒ�l��Ԃ��A1�Ō��ݒl�𒲂ׂĕԂ��B
@return �ォ��̈ʒu�B
*/
int MikanWindowPub::GetPositionY( int type ){
	return _MikanWindow->GetPositionY( type );
}
/*!
@brief �ݒ�E�B���h�E�̍��W�ݒ�
@par   �֐�����
�E�B���h�E�̍��W��ݒ肷��B�ݒ蔽�f��SetWindow()�֐����g���B
@param unsigned int x X���W��^����B
@param unsigned int y Y���W��^����B
@return 0��Ԃ��B
*/
int MikanWindowPub::SetPositionXY( int x, int y )
{
	int ret = _MikanWindow->SetPositionXY( x, y );
	//�E�B���h�E�̐ݒ�̗L�����B
	_MikanWindow->SetWindow();
	return ret;
}

// !�t���X�N���[���ɂ���(SystemInit�ł̎��s�𐄏�)�B
int MikanWindowPub::SetFullScreen( void )
{
	return _MikanWindow->SetWindow( WT_FULLSCREEN );
}

int MikanWindowPub::GetDesktopWidth( void ){
	return Window::GetDesktopWidth();
}
int MikanWindowPub::GetDesktopHeight( void ){
	return Window::GetDesktopHeight();
}
int MikanWindowPub::GetDesktopSize( int *width, int *height ){
	return Window::GetDesktopSize( width, height );
}
/*!
@brief �E�B���h�E�n���h����Ԃ�
@par   �֐�����
�E�B���h�E�n���h�����擾����B
@return ���݂̃E�B���h�E�n���h���B
*/
HWND MikanWindowPub::GetWindowHandle( void ){
	return _MikanWindow->GetWindowHandle();
}

// !�h���b�O&�h���b�v���ꂽ���Ɏ󂯎��֐���ݒ肷��B
/*!
  \param func int Function( int files )�Ƃ����֐��̖��O��^����B�����͎󂯎�����t�@�C�����B
  */
int MikanWindowPub::SetReceiveDragFileFunction( int( *func )( int files ) ){
	return _MikanWindow->SetReceiveDragFileFunction( func );
}

// !�h���b�O&�h���b�v���ꂽ�t�@�C�������擾
/*!
  \param num �h���b�O&�h���b�v���ꂽ�t�@�C���ԍ�
  \retval ������̃|�C���^�B�����R�s�[���邱�ƁB
  */
char * MikanWindowPub::GetReceiveDragFilePath( int num ){
	return _MikanWindow->GetReceiveDragFilePath( num );
}

/*!
@class   MikanDrawPub
@brief   MikanDraw�N���X�BDirectX��p�����`����s���B
@author  Hiroki
@version 1.0
*/

/*!
@brief MikanDraw�̃R���X�g���N�^
@par   �֐�����
�F�Ɗ���W�����������Ă��邾���B
*/
MikanDrawPub::MikanDrawPub()
{
	//�F�̒�`
	//�����������@�Ȃ����ˁH
	ColorList.Black = COLOR_BLACK;
	ColorList.Aliceblue = COLOR_ALICEBLUE;
	ColorList.Darkcyan = COLOR_DARKCYAN;
	ColorList.Lightyellow = COLOR_LIGHTYELLOW;
	ColorList.Coral = COLOR_CORAL;
	ColorList.Dimgray = COLOR_DIMGRAY;
	ColorList.Lavender = COLOR_LAVENDER;
	ColorList.Teal = COLOR_TEAL;
	ColorList.Lightgoldenrodyellow = COLOR_LIGHTGOLDENRODYELLOW;
	ColorList.Tomato = COLOR_TOMATO;
	ColorList.Gray = COLOR_GRAY;
	ColorList.Lightsteelblue = COLOR_LIGHTSTEELBLUE;
	ColorList.Darkslategray = COLOR_DARKSLATEGRAY;
	ColorList.Lemonchiffon = COLOR_LEMONCHIFFON;
	ColorList.Orangered = COLOR_ORANGERED;
	ColorList.Darkgray = COLOR_DARKGRAY;
	ColorList.Lightslategray = COLOR_LIGHTSLATEGRAY;
	ColorList.Darkgreen = COLOR_DARKGREEN;
	ColorList.Wheat = COLOR_WHEAT;
	ColorList.Red = COLOR_RED;
	ColorList.Silver = COLOR_SILVER;
	ColorList.Slategray = COLOR_SLATEGRAY;
	ColorList.Green = COLOR_GREEN;
	ColorList.Burlywood = COLOR_BURLYWOOD;
	ColorList.Crimson = COLOR_CRIMSON;
	ColorList.Lightgrey = COLOR_LIGHTGREY;
	ColorList.Steelblue = COLOR_STEELBLUE;
	ColorList.Forestgreen = COLOR_FORESTGREEN;
	ColorList.Tan = COLOR_TAN;
	ColorList.Mediumvioletred = COLOR_MEDIUMVIOLETRED;
	ColorList.Gainsboro = COLOR_GAINSBORO;
	ColorList.Royalblue = COLOR_ROYALBLUE;
	ColorList.Seagreen = COLOR_SEAGREEN;
	ColorList.Khaki = COLOR_KHAKI;
	ColorList.Deeppink = COLOR_DEEPPINK;
	ColorList.Whitesmoke = COLOR_WHITESMOKE;
	ColorList.Midnightblue = COLOR_MIDNIGHTBLUE;
	ColorList.Mediumseagreen = COLOR_MEDIUMSEAGREEN;
	ColorList.Yellow = COLOR_YELLOW;
	ColorList.Hotpink = COLOR_HOTPINK;
	ColorList.White = COLOR_WHITE;
	ColorList.Navy = COLOR_NAVY;
	ColorList.Mediumaquamarine = COLOR_MEDIUMAQUAMARINE;
	ColorList.Gold = COLOR_GOLD;
	ColorList.Palevioletred = COLOR_PALEVIOLETRED;
	ColorList.Snow = COLOR_SNOW;
	ColorList.Darkblue = COLOR_DARKBLUE;
	ColorList.Darkseagreen = COLOR_DARKSEAGREEN;
	ColorList.Orange = COLOR_ORANGE;
	ColorList.Pink = COLOR_PINK;
	ColorList.Ghostwhite = COLOR_GHOSTWHITE;
	ColorList.Mediumblue = COLOR_MEDIUMBLUE;
	ColorList.Aquamarine = COLOR_AQUAMARINE;
	ColorList.Sandybrown = COLOR_SANDYBROWN;
	ColorList.Lightpink = COLOR_LIGHTPINK;
	ColorList.Floralwhite = COLOR_FLORALWHITE;
	ColorList.Blue = COLOR_BLUE;
	ColorList.Palegreen = COLOR_PALEGREEN;
	ColorList.Darkorange = COLOR_DARKORANGE;
	ColorList.Thistle = COLOR_THISTLE;
	ColorList.Linen = COLOR_LINEN;
	ColorList.Dodgerblue = COLOR_DODGERBLUE;
	ColorList.Lightgreen = COLOR_LIGHTGREEN;
	ColorList.Goldenrod = COLOR_GOLDENROD;
	ColorList.Magenta = COLOR_MAGENTA;
	ColorList.Antiquewhite = COLOR_ANTIQUEWHITE;
	ColorList.Cornflowerblue = COLOR_CORNFLOWERBLUE;
	ColorList.Springgreen = COLOR_SPRINGGREEN;
	ColorList.Peru = COLOR_PERU;
	ColorList.Fuchsia = COLOR_FUCHSIA;
	ColorList.Papayawhip = COLOR_PAPAYAWHIP;
	ColorList.Deepskyblue = COLOR_DEEPSKYBLUE;
	ColorList.Mediumspringgreen = COLOR_MEDIUMSPRINGGREEN;
	ColorList.Darkgoldenrod = COLOR_DARKGOLDENROD;
	ColorList.Violet = COLOR_VIOLET;
	ColorList.Blanchedalmond = COLOR_BLANCHEDALMOND;
	ColorList.Lightskyblue = COLOR_LIGHTSKYBLUE;
	ColorList.Lawngreen = COLOR_LAWNGREEN;
	ColorList.Chocolate = COLOR_CHOCOLATE;
	ColorList.Plum = COLOR_PLUM;
	ColorList.Bisque = COLOR_BISQUE;
	ColorList.Skyblue = COLOR_SKYBLUE;
	ColorList.Chartreuse = COLOR_CHARTREUSE;
	ColorList.Sienna = COLOR_SIENNA;
	ColorList.Orchid = COLOR_ORCHID;
	ColorList.Moccasin = COLOR_MOCCASIN;
	ColorList.Lightblue = COLOR_LIGHTBLUE;
	ColorList.Greenyellow = COLOR_GREENYELLOW;
	ColorList.Saddlebrown = COLOR_SADDLEBROWN;
	ColorList.Mediumorchid = COLOR_MEDIUMORCHID;
	ColorList.Navajowhite = COLOR_NAVAJOWHITE;
	ColorList.Powderblue = COLOR_POWDERBLUE;
	ColorList.Lime = COLOR_LIME;
	ColorList.Maroon = COLOR_MAROON;
	ColorList.Darkorchid = COLOR_DARKORCHID;
	ColorList.Peachpuff = COLOR_PEACHPUFF;
	ColorList.Paleturquoise = COLOR_PALETURQUOISE;
	ColorList.Limegreen = COLOR_LIMEGREEN;
	ColorList.Darkred = COLOR_DARKRED;
	ColorList.Darkviolet = COLOR_DARKVIOLET;
	ColorList.Mistyrose = COLOR_MISTYROSE;
	ColorList.Lightcyan = COLOR_LIGHTCYAN;
	ColorList.Yellowgreen = COLOR_YELLOWGREEN;
	ColorList.Brown = COLOR_BROWN;
	ColorList.Darkmagenta = COLOR_DARKMAGENTA;
	ColorList.Lavenderblush = COLOR_LAVENDERBLUSH;
	ColorList.Cyan = COLOR_CYAN;
	ColorList.Darkolivegreen = COLOR_DARKOLIVEGREEN;
	ColorList.Firebrick = COLOR_FIREBRICK;
	ColorList.Purple = COLOR_PURPLE;
	ColorList.Seashell = COLOR_SEASHELL;
	ColorList.Aqua = COLOR_AQUA;
	ColorList.Olivedrab = COLOR_OLIVEDRAB;
	ColorList.Indianred = COLOR_INDIANRED;
	ColorList.Indigo = COLOR_INDIGO;
	ColorList.Oldlace = COLOR_OLDLACE;
	ColorList.Turquoise = COLOR_TURQUOISE;
	ColorList.Olive = COLOR_OLIVE;
	ColorList.Rosybrown = COLOR_ROSYBROWN;
	ColorList.Darkslateblue = COLOR_DARKSLATEBLUE;
	ColorList.Ivory = COLOR_IVORY;
	ColorList.Mediumturquoise = COLOR_MEDIUMTURQUOISE;
	ColorList.Darkkhaki = COLOR_DARKKHAKI;
	ColorList.Darksalmon = COLOR_DARKSALMON;
	ColorList.Blueviolet = COLOR_BLUEVIOLET;
	ColorList.Honeydew = COLOR_HONEYDEW;
	ColorList.Darkturquoise = COLOR_DARKTURQUOISE;
	ColorList.Palegoldenrod = COLOR_PALEGOLDENROD;
	ColorList.Lightcoral = COLOR_LIGHTCORAL;
	ColorList.Mediumpurple = COLOR_MEDIUMPURPLE;
	ColorList.Mintcream = COLOR_MINTCREAM;
	ColorList.Lightseagreen = COLOR_LIGHTSEAGREEN;
	ColorList.Cornsilk = COLOR_CORNSILK;
	ColorList.Salmon = COLOR_SALMON;
	ColorList.Slateblue = COLOR_SLATEBLUE;
	ColorList.Azure = COLOR_AZURE;
	ColorList.Cadetblue = COLOR_CADETBLUE;
	ColorList.Beige = COLOR_BEIGE;
	ColorList.Lightsalmon = COLOR_LIGHTSALMON;
	ColorList.Mediumslateblue = COLOR_MEDIUMSLATEBLUE;
}
/*!
@brief MikanDraw�̃f�X�g���N�^
@par   �֐�����
�������Ă��Ȃ��B
*/
MikanDrawPub::~MikanDrawPub(){}

//�e�N�X�`���̍ő吔�̐ݒ�
unsigned int MikanDrawPub::SetTextureMax( unsigned int tmax )
{
	if ( _MikanSystem->GetMikanPosition() == MIKAN_POSITION_SYSTEMINIT )
	{
		return _MikanDraw->SetTextureMax( tmax );
	}
	return 0;
}

// !�`��T�C�Y�̐ݒ�
/*!
  \param color ��ʂ�h��ׂ��F(�f�t�H���g0xFF000000[��])�B
  */
int MikanDrawPub::SetScreenSize( int width, int height ){
	return _MikanDraw->SetScreenSize( width, height );
}

int MikanDrawPub::GetScreenWidth( int now ){
	return _MikanDraw->GetScreenWidth( now );
}

int MikanDrawPub::GetScreenHeight( int now ){
	return _MikanDraw->GetScreenHeight( now );
}

/*!
@brief ��ʂ����ꂢ�ɂ���B
@par   �֐�����
��ʂ�P��F�œh��Ԃ��B
@param color �h��Ԃ��F�B�ȗ���0xFF000000(��)�B
@return 0��Ԃ��B
*/
int MikanDrawPub::ClearScreen( unsigned long color ){
	return _MikanDraw->ClearScreen( color );
}

//�`��x���W���Z�b�g
int MikanDrawPub::SetBaseX( int x )
{
	int bx = base_x;
	base_x = x;
	return bx;
}

//�`��y���W���Z�b�g
int MikanDrawPub::SetBaseY( int y )
{
	int by = base_y;
	base_y = y;
	return by;
}

/*!
@brief �e�N�X�`���̍������@�̎w��
@par   �֐�����
�e�N�X�`���̍������@���w�肷��B
@param kind �ȉ��̒l�̂ǂꂩ���w�肷��B
DRAW_TRC=���ߐF��L���ɂ���B
DRAW_ADD=���Z�����B
DRAW_SUB=���Z�����B
DRAW_MUL=��Z�����B
DRAW_NON=�������Ȃ��B���ߐF�������B
@return 0��Ԃ��B
*/
int MikanDrawPub::SetRender( int kind ){
	return _MikanDraw->SetRender( kind );
}
/*!
@brief ��ʂ������_�[�^�[�Q�b�g�ɂ���
@par   �֐�����
�`��Ώۂ���ʂɂ���B
@return 0��Ԃ��B
*/
int MikanDrawPub::SetRenderTarget( void ){
	return _MikanDraw->SetRenderTarget();
}
/*!
@brief �e�N�X�`���������_�[�^�[�Q�b�g�ɂ���
@par   �֐�����
�`��Ώۂ��e�N�X�`���ɂ���B
@param txnum �`��Ώۂɂ���e�N�X�`���ԍ��B
@return 0��Ԃ��B
*/
int MikanDrawPub::SetRenderTarget( unsigned int txnum ){
	return _MikanDraw->SetRenderTarget( txnum );
}
/*!
@brief �摜��ǂݍ���Ńe�N�X�`�������
@par   �֐�����
�摜�t�@�C�����w�肵�ăe�N�X�`�������B
@param txnum �e�N�X�`���ԍ��B
@param filename �t�@�C�����B
@param tcolor ���ߐF�BTRC_NONE�ŉ������Ȃ��BTRC_ZERO�ō���̐F�𓧉ߐF�Ɏw�肷��B�ȗ������ꍇTRC_NONE(�������Ȃ�)�B
@return 0=�e�N�X�`�������X���b�h�̐����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�t�@�C�������Ȃ��B
@return 3=�t�@�C����񂪓ǂݎ��Ȃ������B
@return 4=�̈�m�ۂɎ��s�B
*/
int MikanDrawPub::CreateTexture( unsigned int txnum, const char *filename, unsigned long tcolor, int thread ){
	return _MikanDraw->CreateTexture( txnum, filename, tcolor, thread );
}
int MikanDrawPub::CreateTexture( unsigned int txnum, HMODULE module, const char *resource, unsigned long tcolor, int thread )
{
	if ( module == NULL )
	{
		module = _MikanSystem->GetInstance();
	}
	return _MikanDraw->CreateTexture( txnum, module, resource, tcolor, thread );
}

/*!
@brief ��̃e�N�X�`�������
@par   �֐�����
��̃e�N�X�`�������B
@param txnum �e�N�X�`���ԍ��B
@param width �e�N�X�`���̉����B
@param height �e�N�X�`���̍����B
@param thread �X���b�h�����t���O�B0�Ńe�N�X�`���������ɃX���b�h�����Ȃ��B�ȗ������1�ŁA�X���b�h�������ɃX���b�h�����B
@return 0=�e�N�X�`�������X���b�h�̐����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�ǂ��炩�̑傫����0�B
@return 3=�̈�m�ۂɎ��s�B
*/
int MikanDrawPub::CreateTexture( unsigned int txnum, unsigned int width, unsigned int height, int thread ){
	return _MikanDraw->CreateTexture( txnum, width, height, thread );
}
/*!
@brief �e�N�X�`���̉��
@par   �֐�����
�e�N�X�`�����������B
@param txnum �e�N�X�`���ԍ��B
@return 0��Ԃ��B
*/
int MikanDrawPub::ReleaseTexture( unsigned int txnum ){
	return _MikanDraw->ReleaseTexture( txnum );
}

/*!
@brief �_�̕`��
@par   �֐�����
�_��`�悷��B
@param sx �`��x���W�B
@param sy �`��y���W�B
@param color �F�B�ȗ�����Ɣ��ɂȂ�B
@return 0��Ԃ��B
*/
int MikanDrawPub::DrawPixel( int x, int y, unsigned long color ){
	return _MikanDraw->DrawPixel( MDRAW_X( x ), MDRAW_Y( y ), color );
}

/*!
@brief ���̕`��
@par   �֐�����
����`�悷��B
@param sx �`��J�nx���W�B
@param sy �`��J�ny���W�B
@param ex �`��I��x���W�B
@param ey �`��I��y���W�B
@param color ���̐F�B�ȗ�����Ɣ������ɂȂ�B
@param width ���̑����B���������1.0�B
@param antialias �A���`�G�C���A�X��������Ȃ��1�B�ȗ������0�ŃA���`�G�C���A�X�͂�����Ȃ��B
@return 0��Ԃ��B
*/
int MikanDrawPub::DrawLine( int sx, int sy, int ex, int ey, unsigned long color, double width, int antialias ){
	return _MikanDraw->DrawLine( MDRAW_X( sx ), MDRAW_Y( sy ), MDRAW_X( ex ), MDRAW_Y( ey ), color, width, antialias );
}
int MikanDrawPub::DrawBoxLine( int dx, int dy, int w, int h, unsigned long color, double width, int antialias ){
	return _MikanDraw->DrawBoxLine( MDRAW_X( dx ), MDRAW_Y( dy ), w, h, color, width, antialias );
}
int MikanDrawPub::DrawBoxLineC( int dx, int dy, int w, int h, unsigned long color, double width, int antialias ){
	return _MikanDraw->DrawBoxLineC( MDRAW_X( dx ), MDRAW_Y( dy ), w, h, color, width, antialias );
}

/*!
@brief �����`�̕`��
@par   �֐�����
�����`��`�悷��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param color �����`�̐F�B
@return 0��Ԃ��B
*/
int MikanDrawPub::DrawBox( int dx, int dy, int w, int h, unsigned long color ){
	return _MikanDraw->DrawBox( MDRAW_X( dx ), MDRAW_Y( dy ), w, h, color );
}
/*!
@brief �����`�̕`��
@par   �֐�����
�����`��`�悷��B�e���_�ɐF���w�肷�邱�ƂŃO���f�[�V�����������鎖���ł���B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param color1 ����̐F�B
@param color2 �E��̐F�B
@param color3 �����̐F�B
@param color4 �E���̐F�B
@return 0��Ԃ��B
*/
int MikanDrawPub::DrawBox( int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 ){
	return _MikanDraw->DrawBox( MDRAW_X( dx ), MDRAW_Y( dy ), w, h, color1, color2, color3, color4 );
}

int MikanDrawPub::DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color ){
	return _MikanDraw->DrawBox( MDRAW_X( x0 ), MDRAW_Y( y0 ), MDRAW_X( x1 ), MDRAW_Y( y1 ), MDRAW_X( x2 ), MDRAW_Y( y2 ), MDRAW_X( x3 ), MDRAW_Y( y3 ), color );
}
int MikanDrawPub::DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 ){
	return _MikanDraw->DrawBox( MDRAW_X( x0 ), MDRAW_Y( y0 ), MDRAW_X( x1 ), MDRAW_Y( y1 ), MDRAW_X( x2 ), MDRAW_Y( y2 ), MDRAW_X( x3 ), MDRAW_Y( y3 ), color1, color2, color3, color4 );
}


/*!
@brief �����`�̕`��
@par   �֐�����
�����`�𒆉�����ɂ��ĕ`�悷��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param color �����`�̐F�B
@return 0��Ԃ��B
*/
int MikanDrawPub::DrawBoxC( int dx, int dy, int w, int h, unsigned long color ){
	return _MikanDraw->DrawBoxC( MDRAW_X( dx ), MDRAW_Y( dy ), w, h, color );
}
/*!
@brief �����`�̕`��
@par   �֐�����
�����`�𒆉�����ɂ��ĕ`�悷��B�e���_�ɐF���w�肷�邱�ƂŃO���f�[�V�����������鎖���ł���B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param color1 ����̐F�B
@param color2 �E��̐F�B
@param color3 �����̐F�B
@param color4 �E���̐F�B
@return 0��Ԃ��B
*/
int MikanDrawPub::DrawBoxC( int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 ){
	return _MikanDraw->DrawBoxC( MDRAW_X( dx ), MDRAW_Y( dy ), w, h, color1, color2, color3, color4 );
}
/*!
@brief �e�N�X�`���̕`��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͍���B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/

int MikanDrawPub::DrawCircleLineC( int x, int y, int r, unsigned long color, double width, int antialias ){
	return _MikanDraw->DrawCircleLineC( MDRAW_X( x ), MDRAW_Y( y ), r, color, width, antialias );
}

int MikanDrawPub::DrawCircleLine( int x, int y, int w, int h, unsigned long color, double width, int antialias ){
	return _MikanDraw->DrawCircleLine( MDRAW_X( x ), MDRAW_Y( y ), w, h, color, width, antialias );
}

int MikanDrawPub::DrawCircleC( int x, int y, int r, unsigned long color ){
	return _MikanDraw->DrawCircleC( MDRAW_X( x ), MDRAW_Y( y ), r, color );
}

int MikanDrawPub::DrawCircle( int x, int y, int w, int h, unsigned long color ){
	return _MikanDraw->DrawCircle( MDRAW_X( x ), MDRAW_Y( y ), w, h, color );
}

int MikanDrawPub::DrawTexture( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag ){
	return _MikanDraw->DrawTexture( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, drawflag );
}
/*!
@brief �e�N�X�`���̕`��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int MikanDrawPub::DrawTextureC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag ){
	return _MikanDraw->DrawTextureC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, drawflag );
}
/*!
@brief �e�N�X�`���̊g��k���`��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͍���B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param scale �g��k�����B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int MikanDrawPub::DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag ){
	return _MikanDraw->DrawTextureScaling( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, drawflag );
}
/*!
@brief �e�N�X�`���̊g��k���`��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͍���B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param dw �`�扡���B
@param dh �`�捂���B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int MikanDrawPub::DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag ){
	return _MikanDraw->DrawTextureScaling( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, dw, dh, drawflag );
}
/*!
@brief �e�N�X�`���̊g��k���`��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param scale �g��k�����B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int MikanDrawPub::DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag ){
	return _MikanDraw->DrawTextureScalingC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, drawflag );
}
/*!
@brief �e�N�X�`���̊g��k���`��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param dw �`�扡���B
@param dh �`�捂���B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int MikanDrawPub::DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag ){
	return _MikanDraw->DrawTextureScalingC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, dw, dh, drawflag );
}
/*!
@brief �e�N�X�`������]�`�悷��(���W�A��)
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param rad ���W�A���B���v���B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int MikanDrawPub::DrawTextureRotationC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag ){
	return _MikanDraw->DrawTextureRotationC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, rad, drawflag );
}
/*!
@brief �e�N�X�`������]�`�悷��(�p�x)
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param angle �p�x�B���v���B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int MikanDrawPub::DrawTextureRotationAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag ){
	return _MikanDraw->DrawTextureRotationAngleC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, angle, drawflag );
}
/*!
@brief �e�N�X�`������]�g��`�悷��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param scale �g��k�����B
@param rad ���W�A���B���v���B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int MikanDrawPub::DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag ){
	return _MikanDraw->DrawTextureRotateScaleC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, rad, drawflag );
}
/*!
@brief �e�N�X�`������]�g��`�悷��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param scale �g��k�����B
@param angle ���W�A���B���v���B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int MikanDrawPub::DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag ){
	return _MikanDraw->DrawTextureRotateScaleAngleC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, angle, drawflag );
}
/*!
@brief �e�N�X�`������]�g��`�悷��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param dw �`�扡���B
@param dh �`�捂���B
@param rad ���W�A���B���v���B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int MikanDrawPub::DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, double rad, int drawflag ){
	return _MikanDraw->DrawTextureRotateScaleC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, dw, dh, rad, drawflag );
}
/*!
@brief �e�N�X�`������]�g��`�悷��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param dw �`�扡���B
@param dh �`�捂���B
@param angle ���W�A���B���v���B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int MikanDrawPub::DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, double angle, int drawflag ){
	return _MikanDraw->DrawTextureRotateScaleAngleC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, dw, dh, angle, drawflag );
}
/*!
@brief �e�N�X�`����4�_�����R�Ɍ��߂ĕ`�悷��
@par   �֐�����
�e�N�X�`����`�悷��B���_�w��͓ǂݍ��񂾃e�N�X�`���̍���A�E��A�����A�E����(Z��`���悤��)���ԂŎw�肷��B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param x0 �����x���W
@param y0 �����y���W
@param x1 �E���x���W
@param y1 �E���y���W
@param x2 ������x���W
@param y2 ������y���W
@param x3 �E����x���W
@param y3 �E����y���W
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int MikanDrawPub::DrawTextureVertex( unsigned int txnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag ){
	return _MikanDraw->DrawTextureVertex( txnum, rx, ry, w, h, MDRAW_X( x0 ), MDRAW_Y( y0 ), MDRAW_X( x1 ), MDRAW_Y( y1 ), MDRAW_X( x2 ), MDRAW_Y( y2 ), MDRAW_X( x3 ), MDRAW_Y( y3 ), drawflag );
}

int MikanDrawPub::CreatePolygon( unsigned int pnum, unsigned int max, D3DPRIMITIVETYPE filltype ){
	return _MikanDraw->CreatePolygon( pnum, max, filltype );
}

int MikanDrawPub::CreateRegularPolygon( unsigned int pnum, double cx, double cy, unsigned int n, double r, double rad, unsigned long color ){
	return _MikanDraw->CreateRegularPolygon( pnum, MDRAW_X( cx ), MDRAW_Y( cy ), n, r, rad, color );
}

int MikanDrawPub::SetTextureOnPolygon( unsigned int pnum, unsigned int tnum ){
	return _MikanDraw->SetTextureOnPolygon( pnum, tnum );
}

int MikanDrawPub::SetTextureOnPolygon( unsigned int pnum ){
	return _MikanDraw->SetTextureOnPolygon( pnum );
}

int MikanDrawPub::SetPolygonUV( unsigned int pnum, unsigned int vnum, int u, int v ){
	return _MikanDraw->SetPolygonUV( pnum, vnum, u, v );
}

int MikanDrawPub::SetPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color ){
	return _MikanDraw->SetPolygonVertex( pnum, vnum, MDRAW_X( x ), MDRAW_Y( y ), color );
}

int MikanDrawPub::EditPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color, int u, int v, int edit ){
	return _MikanDraw->EditPolygonVertex( pnum, vnum, MDRAW_X( x ), MDRAW_Y( y ), color, (double)u / (double)_MikanDraw->GetTextureWidth( _MikanDraw->GetTextureNumOnPolygon( pnum ) ), (double)v / (double)_MikanDraw->GetTextureHeight( _MikanDraw->GetTextureNumOnPolygon( pnum ) ), edit );
}

int MikanDrawPub::TransformPolygonMoving( unsigned int pnum, double mx, double my ){
	return _MikanDraw->TransformPolygonMoving( pnum, mx, my );
}

int MikanDrawPub::TransformPolygonScaling( unsigned int pnum, double scale, unsigned int basenum ){
	return _MikanDraw->TransformPolygonScaling( pnum, scale, basenum );
}

int MikanDrawPub::DrawPolygon( unsigned int pnum, int cx, int cy, unsigned int begin, unsigned int end ){
	return _MikanDraw->DrawPolygon( pnum, MDRAW_X( cx ), MDRAW_Y( cy ), begin, end );
}

int MikanDrawPub::DrawPolygon( unsigned int pnum, int cx, int cy ){
	return _MikanDraw->DrawPolygon( pnum, MDRAW_X( cx ), MDRAW_Y( cy ) );
}

int MikanDrawPub::DrawPolygon( unsigned int pnum ){
	return _MikanDraw->DrawPolygon( pnum );
}

int MikanDrawPub::DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale, unsigned int start, unsigned int end ){
	return _MikanDraw->DrawPolygonScaling( pnum, MDRAW_X( cx ), MDRAW_Y( cy ), scale, start, end );
}

int MikanDrawPub::DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale ){
	return _MikanDraw->DrawPolygonScaling( pnum, MDRAW_X( cx ), MDRAW_Y( cy ), scale );
}

int MikanDrawPub::DrawRegularPolygon( double cx, double cy, unsigned int n, double r, double rad, unsigned long color ){
	return _MikanDraw->DrawRegularPolygon( MDRAW_X( cx ), MDRAW_Y( cy ), n, r, rad, color );
}

/*!
@brief �F�̍쐬
@par   �֐�����
Mikan(DirectX)�Ŏg����F�����B
@param alpha �A���t�@�l�B
@param red �ԁB
@param green �΁B
@param blue �B
@return �F�B0xAARRGGBB�B
*/
unsigned long MikanDrawPub::Color( unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue ){
	return _MikanDraw->Color( alpha, red, green, blue );
}
/*!
@brief �J���[�̐ݒ�
@par   �֐�����
�e�N�X�`������߂�F��ݒ肷��B4�F�����ݒ�B
@param txnum �e�N�X�`���ԍ��B
@param color �F�B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
*/
int MikanDrawPub::SetColor( unsigned int txnum, unsigned long color ){
	return _MikanDraw->SetColor( txnum, color );
}

int MikanDrawPub::SetAlpha( unsigned int txnum, unsigned char alpha ){
	return _MikanDraw->SetAlpha( txnum, alpha );
}


/*!
@brief ���_�J���[�̐ݒ�
@par   �֐�����
�e�N�X�`������߂�F��ݒ肷��B1�F�����̐ݒ�Ȃ̂ŁA4��g����4���_�ɕʁX�̐F��ݒ肷�鎖���ł���B
@param txnum �e�N�X�`���ԍ��B
@param color �F�B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
*/
int MikanDrawPub::SetColorVertex( unsigned int txnum, unsigned long color ){
	return _MikanDraw->SetColorVertex( txnum, color );
}
int MikanDrawPub::OpenTexture( unsigned int txnum, int x, int y, int w, int h ){
	return _MikanDraw->OpenTexture( txnum, x, y, w, h );
}
int MikanDrawPub::CloseTexture( unsigned int txnum ){
	return _MikanDraw->CloseTexture( txnum );
}

/*!
@brief �e�N�X�`���̎w����W�̐F���擾����B
@par   �֐�����
�e�N�X�`���̎w����W�̐F���擾����B
@param txnum �e�N�X�`���ԍ��B
@param x x���W�B
@param y y���W�B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
*/
unsigned long MikanDrawPub::GetColorTexturePixel( unsigned int txnum, int x, int y ){
	return _MikanDraw->GetColorTexturePixel( txnum, x, y );
}
/*!
@brief �e�N�X�`���̐F��ʂ̐F�ɒu������
@par   �֐�����
�e�N�X�`���͈͓̔��̐F��u������B
@param txnum �e�N�X�`���ԍ��B
@param scolor ��������F�B
@param rcolor �u������F�B
@param x �����J�nx���W�B�����l��0�B
@param y �����J�ny���W�B�����l��0�B
@param w �����B�����l��0�ŁA�e�N�X�`���̉����ɂȂ�B
@param h �����B�����l��0�ŁA�e�N�X�`���̍����ɂȂ�B
@return �u�������񐔂�Ԃ��B��x���u�����Ȃ����0�B
*/
unsigned int MikanDrawPub::TextureColorReplacement( unsigned int txnum, unsigned long scolor, unsigned long rcolor, int x, int y, int w, int h ){
	return _MikanDraw->TextureColorReplacement( txnum, scolor, rcolor, x, y, w, h );
}
/*!
@brief �e�N�X�`���̐F����������
@par   �֐�����
�e�N�X�`���͈͓̔��̐F����������B
@param txnum �e�N�X�`���ԍ��B
@param scolor ��������F�B
@param x �����J�nx���W�B�����l��0�B
@param y �����J�ny���W�B�����l��0�B
@param w �����B�����l��0�ŁA�e�N�X�`���̉����ɂȂ�B
@param h �����B�����l��0�ŁA�e�N�X�`���̍����ɂȂ�B
@return 0 ������Ȃ������B
@return ���߂ăq�b�g����܂ł̌����ς݃s�N�Z�����B���ォ��E���Ɍ��������ԁB
*/
unsigned int MikanDrawPub::TextureColorSearch( unsigned int txnum, unsigned long scolor, int x, int y, int w, int h ){
	return _MikanDraw->TextureColorSearch( txnum, scolor, x, y, w, h );
}
//�e�N�X�`���͈͓̔��̐F�����������𐔂���B
unsigned int MikanDrawPub::TextureColorCount( unsigned int txnum, unsigned long scolor, int x, int y, int w, int h ){
	return _MikanDraw->TextureColorCount( txnum, scolor, x, y, w, h );
}
/*!
@brief �e�N�X�`���̉����擾
@par   �֐�����
�e�N�X�`���̉������擾�B
@param txnum �e�N�X�`���ԍ��B
@return 0=���s�B
@return ���̑�=�e�N�X�`���̉����B
*/
unsigned int MikanDrawPub::GetTextureWidth( unsigned int txnum ){
	return _MikanDraw->GetTextureWidth( txnum );
}
/*!
@brief �e�N�X�`���̍����擾
@par   �֐�����
�e�N�X�`���̍������擾�B
@param txnum �e�N�X�`���ԍ��B
@return 0=���s�B
@return ���̑�=�e�N�X�`���̍����B
*/
unsigned int MikanDrawPub::GetTextureHeight( unsigned int txnum ){
	return _MikanDraw->GetTextureHeight( txnum );
}
/*!
@brief �X�N���[���V���b�g���B��
@par   �֐�����
���݂̃X�N���[���V���b�g
@param filename �t�@�C�����B
@param imagetype �摜�̃^�C�v�B
@return ���0��Ԃ��B
*/
int MikanDrawPub::ScreenShot( const char *filename, unsigned int imagetype ){
	return _MikanDraw->ScreenShot( filename, imagetype );
}

// �t�H���g��ǂݍ���
int MikanDrawPub::LoadFontFile( const char *fontfile ){
	return _MikanDraw->LoadFontFile( fontfile );
}

int MikanDrawPub::LoadFontFile( HMODULE module, const char *resource )
{
	if ( module == NULL )
	{
		module = _MikanSystem->GetInstance();
	}
	return _MikanDraw->LoadFontFile( module, resource );
}

int MikanDrawPub::CreateFont( unsigned int fontnum, const char *fontname, int size, unsigned long color, int weight, int italic ){
	return _MikanDraw->CreateFont( fontnum, fontname, size, color, weight, italic );
}

/*!
@brief �t�H���g�����B
@par   �֐�����
�t�H���g�����B
@param fontnum �t�H���g�ԍ��B
@param fontname �t�H���g���B
@param size �t�H���g�T�C�Y�B�ȗ������ꍇ10�B
@param color �t�H���g�̐F�B�ȗ������ꍇ0xFFFFFFF(��)�B
@return 0=�����B
@return 1=�t�H���g�ԍ����s���B
*/
int MikanDrawPub::CreateFont( unsigned int fontnum, const char *fontname, int size, unsigned long color ){
	return _MikanDraw->CreateFont( fontnum, fontname, size, color );
}
/*!
@brief �t�H���g�����B
@par   �֐�����
�t�H���g�����B�t�H���g�̓S�V�b�N�́B
@param fontnum �t�H���g�ԍ��B
@param size �t�H���g�T�C�Y�B�ȗ������ꍇ10�B
@param color �t�H���g�̐F�B�ȗ������ꍇ0xFFFFFFF(��)�B
@return 0=�����B
@return 1=�t�H���g�ԍ����s���B
*/
int MikanDrawPub::CreateFont( unsigned int fontnum, int size, unsigned long color ){
	return _MikanDraw->CreateFont( fontnum, size, color );
}
/*!
@brief �������`�悷��B
@par   �֐�����
�����̕������`�悷��B
@param fontnum �t�H���g�ԍ��B
@param x �`�悷��x���W�B
@param y �`�悷��y���W�B
@param str �`�悷�镶����B
@return �t�H���g�ԍ��B�s���ȏꍇ��0���g����B
*/
//int MikanDrawPub::Print( unsigned int fontnum, int x, int y, char *str ){ return _MikanDraw->Print( fontnum, x, y, str ); }
/*!
@brief ������𐮌`�`�悷��B
@par   �֐�����
������𐮌`�`�悷��B�ڂ�����C���ꓙ��printf�Ȃǂ��Q�ƁB
@param fontnum �t�H���g�ԍ��B
@param x �`�悷��x���W�B
@param y �`�悷��y���W�B
@param format ���`�`�悷�镶����B
@param ���̑������ł� ���`������Ő��`���ĕ`�悷��ϐ������B
@return �t�H���g�ԍ��B�s���ȏꍇ��0���g����B
*/
int MikanDrawPub::Printf( unsigned int fontnum, int x, int y, const char *format, ... )
{
	char str[ MAX_BUFFER_SIZE ];
	vsnprintf_s( str, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, format, (char*)( &format + 1 ) );
	return _MikanDraw->Print( fontnum, MDRAW_X( x ), MDRAW_Y( y ), str );
}
/*!
@brief ������𐮌`�`�悷��B
@par   �֐�����
������𐮌`�`�悷��B�ڂ�����C���ꓙ��printf�Ȃǂ��Q�ƁB
@param fontnum �t�H���g�ԍ��B
@param x �`�悷��x���W�B
@param y �`�悷��y���W�B
@param color �����F�B
@param format ���`�`�悷�镶����B
@param ���̑������ł� ���`������Ő��`���ĕ`�悷��ϐ������B
@return �t�H���g�ԍ��B�s���ȏꍇ��0���g����B
*/
int MikanDrawPub::Printf( unsigned int fontnum, int x, int y, unsigned long color, const char *format, ... )
{
	char str[ MAX_BUFFER_SIZE ];
	vsnprintf_s( str, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, format, (char*)( &format + 1 ) );
	return _MikanDraw->Print( fontnum, MDRAW_X( x ), MDRAW_Y( y ), color, str );
}


struct IDirect3DTexture9 * MikanMovieManagementPub::GetTexture( unsigned int movnum ){
	return _MikanMovie->GetTexture( movnum );
}

int MikanMovieManagementPub::Lock( unsigned int movnum ){
	return _MikanMovie->Lock( movnum );
}

int MikanMovieManagementPub::Unlock( unsigned int movnum ){
	return _MikanMovie->Unlock( movnum );
}

int MikanMovieManagementPub::Load( unsigned int movnum, char *moviefile ){
	return _MikanMovie->Load( movnum, moviefile );
}

int MikanMovieManagementPub::Play( unsigned int movnum, int loopplay ){
	return _MikanMovie->Play( movnum, loopplay );
}

int MikanMovieManagementPub::Stop( unsigned int movnum ){
	return _MikanMovie->Stop( movnum );
}

int MikanMovieManagementPub::Pause( unsigned int movnum ){
	return _MikanMovie->Pause( movnum );
}

int MikanMovieManagementPub::IsPlay( unsigned int movnum ){
	return _MikanMovie->IsPlay( movnum );
}

long MikanMovieManagementPub::Size( unsigned int movnum ){
	return _MikanMovie->Size( movnum );
}

long MikanMovieManagementPub::Tell( unsigned int movnum ){
	return _MikanMovie->Tell( movnum );
}

int MikanMovieManagementPub::Seek( unsigned int movnum, long seek ){
	return _MikanMovie->Seek( movnum, seek );
}

double MikanMovieManagementPub::GetRate( unsigned int movnum ){
	return _MikanMovie->GetRate( movnum );
}

int MikanMovieManagementPub::SetRate( unsigned int movnum, double rate ){
	return _MikanMovie->SetRate( movnum, rate );
}

int MikanMovieManagementPub::GetWidth( unsigned int movnum ){
	return _MikanMovie->GetWidth( movnum );
}

int MikanMovieManagementPub::GetHeight( unsigned int movnum ){
	return _MikanMovie->GetHeight( movnum );
}

int MikanMovieManagementPub::SetColor( unsigned int movnum ){
	return _MikanMovie->SetColor( movnum );
}

int MikanMovieManagementPub::SetColor( unsigned int movnum, unsigned long color ){
	return _MikanMovie->SetColor( movnum, color );
}

int MikanMovieManagementPub::SetColor( unsigned int movnum, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 ){
	return _MikanMovie->SetColor( movnum, color0, color1, color2, color3 );
}

int MikanMovieManagementPub::SetAlpha( unsigned int movnum, unsigned char alpha ){
	return _MikanMovie->SetAlpha( movnum, alpha );
}

int MikanMovieManagementPub::DrawMovie( unsigned int movnum, int dx, int dy, int drawflag ){
	return _MikanMovie->DrawMovie( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), drawflag );
}

int MikanMovieManagementPub::DrawMovie( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag ){
	return _MikanMovie->DrawMovie( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, drawflag );
}

int MikanMovieManagementPub::DrawMovieC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag ){
	return _MikanMovie->DrawMovieC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, drawflag );
}

int MikanMovieManagementPub::DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag ){
	return _MikanMovie->DrawMovieScaling( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, drawflag );
}

int MikanMovieManagementPub::DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag ){
	return _MikanMovie->DrawMovieScaling( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, dw, dh, drawflag );
}

int MikanMovieManagementPub::DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag ){
	return _MikanMovie->DrawMovieScalingC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, drawflag );
}

int MikanMovieManagementPub::DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag ){
	return _MikanMovie->DrawMovieScalingC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, dw, dh, drawflag );
}

int MikanMovieManagementPub::DrawMovieRotationC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag ){
	return _MikanMovie->DrawMovieRotationC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, rad, drawflag );
}

int MikanMovieManagementPub::DrawMovieRotationAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag ){
	return _MikanMovie->DrawMovieRotationAngleC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, angle, drawflag );
}

int MikanMovieManagementPub::DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag ){
	return _MikanMovie->DrawMovieRotateScaleC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, rad, drawflag );
}

int MikanMovieManagementPub::DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag ){
	return _MikanMovie->DrawMovieRotateScaleAngleC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, angle, drawflag );
}

int MikanMovieManagementPub::DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag ){
	return _MikanMovie->DrawMovieRotateScaleC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, sw, sh, rad, drawflag );
}

int MikanMovieManagementPub::DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag ){
	return _MikanMovie->DrawMovieRotateScaleAngleC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, sw, sh, angle, drawflag );
}

int MikanMovieManagementPub::DrawMovieVertex( unsigned int movnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag ){
	return _MikanMovie->DrawMovieVertex( movnum, rx, ry, w, h, MDRAW_X( x0 ), MDRAW_Y( y0 ), MDRAW_X( x1 ), MDRAW_Y( y1 ), MDRAW_X( x2 ), MDRAW_Y( y2 ), MDRAW_X( x3 ), MDRAW_Y( y3 ), drawflag );
}








/*!
@class   MikanInputPub
@brief   Mikaninput�N���X�BDirectX��p�������͂��s���B
@author  Hiroki
@version 1.0
*/

/*!
@brief �R���X�g���N�^
@par   �֐�����
�������Ă��Ȃ��B
*/
MikanInputPub::MikanInputPub( void ){}
/*!
@brief �f�X�g���N�^
@par   �֐�����
�������Ă��Ȃ��B
*/
MikanInputPub::~MikanInputPub( void ){}
/*!
@brief �p�b�h�̔F��
@par   �֐�����
�p�b�h�̍ĔF�����s���B
@param resume 0�ŏ��߂���F�����������B�ȗ������ꍇ��1�Ɠ����Œǉ��F�����s���B
@return 0=�����B
@return 1=�f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s�B
@return 2=���[�h�ݒ�Ɏ��s�B
*/
int MikanInputPub::RecognitionGamepad( int resume ){
	return _MikanInput->RecognitionGamepad( resume );
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
int MikanInputPub::GetKeyNum( int keynum ){
	return _MikanInput->GetKeyNum( keynum );
}

int MikanInputPub::GetKeyNumRapid( int keynum, int threshold, unsigned int interval ){
	return _MikanInput->GetKeyNumRapid( keynum, threshold, interval );
}

int MikanInputPub::GetKeyWhichButton( void ){
	return _MikanInput->GetKeyWhichButton();
}

/*!
@brief �֐����s���ɉ�����Ă���L�[�̏�Ԃ𒲂ׂ�B
@par   �֐�����
�֐������s�����Ƃ��̃L�[�{�[�h�̏�Ԃ��擾���A������Ă���ꍇ�̓o�b�t�@�ɏ�Ԃ�ۑ�����B
���̂܂܂ł͓��͂̍X�V�͍s���Ȃ��B
@return 0=�����B
@return 1=�L�[��Ԃ̎擾���o���Ȃ������̂ŁA���͐�����ēx�J�n�����B
*/
//int MikanInputPub::UpdateKeyStatus( void ){ return _MikanInput->UpdateKeyStatus(); }
/*!
@brief ���s���܂łɉ����ꂽ�L�[�ɑ΂��A�l���X�V����B
@par   �֐�����
UpdateKeyStatus�ɂ���Ď擾���ꂽ�o�b�t�@�����ɁA�L�[���͂��X�V����B
���̊֐��͂����ԓ�(�t���[���Ȃ�)�ň�x�������s����̂��D�܂����B
���̊֐��͒P�̂�UpdateKeyStatus���s���̂ŁA���O�Ɏg���K�v�͂Ȃ��B
@return 0=�����B
@return 1=UpdateKeyState�����s�B
*/
int MikanInputPub::UpdateKeyInput( void ){
	return _MikanInput->UpdateKeyInput();
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
int MikanInputPub::GetPadNum( int padnum, int button ){
	return _MikanInput->GetPadNum( padnum, button );
}
/*!
@brief �w�肳�ꂽ�p�b�h�̃X�e�B�b�N���͂��擾����B
@par   �֐�����
�F�����ꂽ����p�b�h�̃A�i���O�X�e�B�b�N�̓��͏�Ԃ𒲂ׂĕԂ��B
�l��-1000����1000�ŕ�����DirectX�̍��W�n�Ɠ����ŁAPAD_UD�͉����������A����������BPAD_LR�͉E���������A�����������B
@param padnum �p�b�h�ԍ��B
@param button �A�i���O�X�e�B�b�N�̏㉺������PAD_UD�ŁA���E������PAD_LR�Ŏ擾�ł���B
@return 0=�����Ă��Ȃ��B�������͖����B
@return ����ȊO=�X�e�B�b�N�̓��́B�X����B
*/
int MikanInputPub::GetPadStick( unsigned int padnum, int button ){
	return _MikanInput->GetPadStick( padnum, button );
}
/*!
@brief �p�b�h�̔F����
@par   �֐�����
���ݔF������Ă���p�b�h�̐���Ԃ��B
@return ���݂ɔF�����Ă���p�b�h�̐��B
*/
int MikanInputPub::GetPadMount( void ){
	return _MikanInput->GetPadMount();
}
int MikanInputPub::GetPadWhichButton( unsigned int padnum ){
	return _MikanInput->GetPadWhichButton( padnum );
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
//int MikanInputPub::UpdatePadStatus( int padnum ){ return _MikanInput->UpdatePadStatus( padnum ); }
/*!
@brief �S�p�b�h�̃L�[���͂̒l���X�V����B
@par   �֐�����
�S�Ẵp�b�h�̑S�Ẵ{�^���̃L�[���͂̒l���X�V����B
@return ���͂��ꂽ�p�b�h�̐��B
*/
int MikanInputPub::UpdatePadInput( void ){
	return _MikanInput->UpdatePadInput();
}
/*!
@brief �p�b�h�̃t�H�[�X�t�B�[�h�o�b�N���g���邩�ǂ������ׂ�
@par   �֐�����
�w�肵���p�b�h���t�H�[�X�t�B�[�h�o�b�N(�U��)�ɑΉ����Ă��邩�ǂ����𒲂ׂ�B
@return 0 �p�b�h���t�H�[�X�t�B�[�h�o�b�N�ɑΉ����Ă��Ȃ����A�������s���B
@return 1 �p�b�h���t�H�[�X�t�B�[�h�o�b�N�ɑΉ����Ă���B
*/
int MikanInputPub::CanForceFeedback( unsigned int padnum ){
	return _MikanInput->CanForceFeedback( padnum );
}
/*!
@brief �p�b�h��POV���[�h���w�肷��
@par   �֐�����
�A�i���O���͂ƃf�W�^�����͂̈Ⴂ�Ō����Ȃ̂��\���L�[�ƃA�i���O�X�e�B�b�N�̓���̈Ⴂ�Ȃ̂ŁA������w�肷��B
POV���[�h��^�ɂ���Ə\���L�[�ƃA�i���O�X�e�B�b�N�̓��͂�ʂƂ݂Ȃ��A�㉺���E��PAD_UP,PAD_DOWN,PAD_LEFT,PAD_RIGHT��
�\���L�[�݂̂ő�����悤�ɂȂ����A(��)�A�i���O�X�e�B�b�N��PAD_LS_UP,PAD_LS_DOWN,PAD_LS_LEFT,PAD_LS_RIGHT�Œl���擾
����悤�ɂȂ�B
POV���[�h���U�ɂ���ƁA�\���L�[�ƃA�i���O�X�e�B�b�N�̓��͂�PAD_UP,PAD_DOWN,PAD_LEFT,PAD_RIGHT�ɔ��f����B
�o�b�t�@�l���擾����GetPadStick()�Ɋւ��Ă͉��̌��ʂ��Ȃ��AGetPadNum()�̋�����ς�����̂ƍl���ėǂ��B
@param padnum �p�b�h�ԍ��B
@param povmode POV���[�h�B0�Ŗ����A����ȊO�ŗL���ɂ���B�f�t�H���g�ł͖����B
@return 0 �p�b�h���t�H�[�X�t�B�[�h�o�b�N�ɑΉ����Ă��Ȃ����A�������s���B
@return 1 �p�b�h���t�H�[�X�t�B�[�h�o�b�N�ɑΉ����Ă���B
*/
int MikanInputPub::SetPOVMode( unsigned int padnum, int povmode ){
	return _MikanInput->SetPOVMode( padnum, povmode );
}
/*!
@brief �}�E�X��X���W���擾����
@par   �֐�����
�����݂̃}�E�X�̍��W���A�`��͈͓�����ɕԂ��B���オ0�ƂȂ�A�E�ɍs���قǒl���傫���Ȃ�B
@return ���W��Ԃ��B�`��̈�𒴂���ƕ��̐���E�B���h�E�T�C�Y���z����B
*/
int MikanInputPub::GetMousePosX( void ){
	return _MikanInput->GetMousePosX();
}
/*!
@brief �}�E�X��Y���W���擾����
@par   �֐�����
�����݂̃}�E�X�̍��W���A�`��͈͓�����ɕԂ��B���オ0�ƂȂ艺�ɍs���قǒl���傫���Ȃ�B
@return ���W��Ԃ��B�`��̈�𒴂���ƕ��̐���E�B���h�E�T�C�Y���z����B
*/
int MikanInputPub::GetMousePosY( void ){
	return _MikanInput->GetMousePosY();
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
int MikanInputPub::GetMouseNum( unsigned int num ){
	return _MikanInput->GetMouseNum( num );
}
/*!
@brief �}�E�X�J�[�\���̍��W��ݒ肷��
@par   �֐�����
�}�E�X�J�[�\���̏ꏊ��ύX����B
@return 0��Ԃ��B
*/
int MikanInputPub::SetMousePos( int x, int y ){
	return _MikanInput->SetMousePos( x, y );
}
int MikanInputPub::GetMouseWheel( void ){
	return _MikanInput->GetMouseWheel();
}
/*!
@brief �}�E�X���͂̒l���X�V����
@par   �֐�����
�}�E�X�̓��͂̒l���X�V����B
@return 0 = �����B
@return 1 = UpdateMouseStatus()�����s�B
*/
int MikanInputPub::UpdateMouseInput( void ){
	return _MikanInput->UpdateMouseInput();
}

int MikanInputPub::SetPad2KeyInput( int enable ){
	return _MikanInput->SetPad2KeyInput( enable );
}

int MikanInputPub::SetPad2KeyCode( unsigned int padnum, int padbutton, int keynum ){
	return _MikanInput->SetPad2KeyCode( padnum, padbutton, keynum );
}

int MikanInputPub::UpdatePad2KeyInput( void ){
	return _MikanInput->UpdatePad2KeyInput();
}

#ifndef MIKAN_NO_SOUND
bool MikanSoundPub::Load( int sound_number, const char* file_path, bool streaming ){
	return _MikanSound->Load( sound_number, file_path, streaming );
}
#ifndef _VS2010
bool MikanSoundPub::Load( int sound_number, HMODULE module, const char* resource )
{
	if ( module == NULL )
	{
		module = _MikanSystem->GetInstance();
	}
	return _MikanSound->Load( sound_number, module, resource );
}
#endif

void MikanSoundPub::Unload( int sound_number ){
	return _MikanSound->Unload( sound_number );
}

void MikanSoundPub::UnloadAll( void ){
	return _MikanSound->UnloadAll();
}

bool MikanSoundPub::Play( int sound_number, bool loop ){
	return _MikanSound->Play( sound_number, loop );
}

#ifndef _VS2010
bool MikanSoundPub::IsPlaying( int sound_number ){
	return _MikanSound->IsPlaying( sound_number );
}
#endif

void MikanSoundPub::Pause( int sound_number ){
	return _MikanSound->Pause( sound_number );
}

void MikanSoundPub::PauseAll( void ){
	return _MikanSound->PauseAll();
}

void MikanSoundPub::Stop( int sound_number ){
	return _MikanSound->Stop( sound_number );
}

void MikanSoundPub::StopAll( void ){
	return _MikanSound->StopAll();
}

int MikanSoundPub::GetVolume( int sound_number ){
	return _MikanSound->GetVolume( sound_number );
}

void MikanSoundPub::SetVolume( int sound_number, int volume ){
	return _MikanSound->SetVolume( sound_number, volume );
}

int MikanSoundPub::GetPan( int sound_number ){
	return _MikanSound->GetPan( sound_number );
}

void MikanSoundPub::SetPan( int sound_number, int pan ){
	return _MikanSound->SetPan( sound_number, pan );
}

unsigned long MikanSoundPub::GetFrequency( int sound_number ){
	return _MikanSound->GetFrequency( sound_number );
}

void MikanSoundPub::SetFrequency( int sound_number, unsigned long frequency ){
	return _MikanSound->SetFrequency( sound_number, frequency );
}

unsigned long MikanSoundPub::Tell( int sound_number ){
	return _MikanSound->Tell( sound_number );
}

void MikanSoundPub::Seek( int sound_number, unsigned long time ){
	return _MikanSound->Seek( sound_number, time );
}

bool MikanSoundPub::GetLoop( int sound_number, unsigned long* loop_start, unsigned long* loop_end ){
	return _MikanSound->GetLoop( sound_number, loop_start, loop_end );
}

bool MikanSoundPub::SetLoop( int sound_number, unsigned long loop_start, unsigned long loop_end ){
	return _MikanSound->SetLoop( sound_number, loop_start, loop_end );
}

#endif

/*!
@class   MikanNetworkPub
@brief   MikanNetwork�N���X�B�l�b�g���[�N�ʐM���s���B
@author  Hiroki
@version 1.0
*/

MikanNetworkPub::MikanNetworkPub( void ){}
MikanNetworkPub::~MikanNetworkPub( void ){}

// �T�[�o�[���[�h�ɂ���B��x�������s�\�B
SOCKET MikanNetworkPub::CreateTCPServer( unsigned int nnum, unsigned short port, int connect ){
	return _MikanNet->CreateTCPServer( nnum, port, connect );
}
SOCKET MikanNetworkPub::CreateUDPServer( unsigned int nnum, unsigned short port, int connect ){
	return _MikanNet->CreateUDPServer( nnum, port, connect );
}
SOCKET MikanNetworkPub::CreateBloadcastServer( unsigned int nnum, unsigned short port, int connect ){
	return _MikanNet->CreateBloadcastServer( nnum, port, connect );
}
SOCKET MikanNetworkPub::CreateMulticastServer( unsigned int nnum, const char *address, unsigned short port, int connect ){
	return _MikanNet->CreateMulticastServer( nnum, address, port, connect );
}
SOCKET MikanNetworkPub::CreateMulticastServer( unsigned int nnum, unsigned long address, unsigned short port, int connect ){
	return _MikanNet->CreateMulticastServer( nnum, address, port, connect );
}

// �N���C�A���g���[�h�ɂ���B
SOCKET MikanNetworkPub::CreateTCPClient( unsigned int nnum, const char *address, unsigned short port ){
	return _MikanNet->CreateTCPClient( nnum, address, port );
}
SOCKET MikanNetworkPub::CreateUDPClient( unsigned int nnum, const char *address, unsigned short port ){
	return _MikanNet->CreateUDPClient( nnum, address, port );
}
SOCKET MikanNetworkPub::CreateBloadcastClient( unsigned int nnum, const char *address, unsigned short port ){
	return _MikanNet->CreateBloadcastClient( nnum, address, port );
}
SOCKET MikanNetworkPub::CreateMulticastClient( unsigned int nnum, const char *address, unsigned short port ){
	return _MikanNet->CreateMulticastClient( nnum, address, port );
}

int MikanNetworkPub::Close( unsigned int nnum ){
	return _MikanNet->Close( nnum );
}

int MikanNetworkPub::Send( unsigned int nnum, const char *str ){
	return _MikanNet->Send( nnum, str );
}
int MikanNetworkPub::Send( unsigned int nnum, const char *str, unsigned int size ){
	return _MikanNet->Send( nnum, str, size );
}

char * MikanNetworkPub::Receive( unsigned int nnum, int *getsize, struct sockaddr *from ){
	return _MikanNet->Receive( nnum, getsize, from );
}
int MikanNetworkPub::Receive( unsigned int nnum, char *buf, int size, struct sockaddr *from ){
	return _MikanNet->Receive( nnum, buf, size, from );
}

SOCKET MikanNetworkPub::Accept( unsigned int nnum, struct sockaddr_in *client ){
	return _MikanNet->Accept( nnum, client );
}

//int MikanNetworkPub::HttpGet( const char *address, const char *filepath, const char *proxy, const char *user, const char *password, int *getsize, int *loadsize ){ return MikanNetwork::HttpGet( address, filepath, proxy, user, password, getsize, loadsize ); }
int MikanNetworkPub::HttpGet( const char *address, const char *filepath, int *getsize, int *loadsize ){
	return MikanNetwork::HttpGet( address, filepath, NULL, NULL, NULL, getsize, loadsize );
}
int MikanNetworkPub::HttpPost( const char *address, const char *data, int datasize, const char *filepath, int *getsize, int *loadsize ){
	return MikanNetwork::HttpPost( address, data, datasize, filepath, NULL, NULL, NULL, getsize, loadsize );
}
class MikanIO * MikanNetworkPub::GetMikanIO( unsigned int ionum ){
	return _MikanNet->GetMikanIO( ionum );
}

// �J���B-1�Ŏ��s�B
int FileIOManagementPub::Open( unsigned int ionum, const char *io, const char *mode ){
	return _MikanFile->Open( ionum, io, mode );
}
// 1�s�ǂݍ��ށB
char * FileIOManagementPub::ReadLine( unsigned int ionum, unsigned int read_size ){
	return _MikanFile->ReadLine( ionum, read_size );
}
int FileIOManagementPub::ReadLine( unsigned int ionum, char *buf, unsigned int max_size, unsigned int *read_size ){
	return _MikanFile->ReadLine( ionum, buf, max_size, read_size );
}

int FileIOManagementPub::Write( unsigned int ionum, const char *write_data, int write_size ){
	return _MikanFile->Write( ionum, write_data, write_size );
}
int FileIOManagementPub::Printf( unsigned int ionum, const char *format, ... )
{
	int ret;
	char *str;
	//va_list���g�����@�����邯�ǁA�ʂɂ���Ŗ��Ȃ���ˁB
	str = (char *)calloc( 1024, sizeof( char ) );
	vsnprintf_s( str, 1024, 1024, format, (char*)( &format + 1 ) );

	ret = Write( ionum, str );

	free( str );

	return ret;
}

// �ǂݍ��ށBNULL�Ŏ��s�B
void * FileIOManagementPub::Read( unsigned int ionum, unsigned int read_size, int *readed_size ){
	return _MikanFile->Read( ionum, read_size, readed_size );
}
int FileIOManagementPub::Read( unsigned int ionum, void *data, unsigned int read_size, int *readed_size ){
	return _MikanFile->Read( ionum, data, read_size, readed_size );
}
// �V�[�N����B-1�Ŏ��s�B
int FileIOManagementPub::Seek( unsigned int ionum, int offset, int seekmode ){
	return _MikanFile->Seek( ionum, offset, seekmode );
}
// �������ށB-1�Ŏ��s�B�������񂾃T�C�Y��Ԃ��B
int FileIOManagementPub::Write( unsigned int ionum, const void *write_data, int write_size ){
	return _MikanFile->Write( ionum, (const char *)write_data, write_size );
}
// ���܂œǂ񂾃o�C�g����Ԃ��B-1�Ŏ��s�B
int FileIOManagementPub::Tell( unsigned int ionum ){
	return _MikanFile->Tell( ionum );
}
// �T�C�Y��Ԃ��B-1�Ŏ��s�B��������Tell�B
int FileIOManagementPub::Size( unsigned int ionum ){
	return _MikanFile->Size( ionum );
}
// ����B-1�Ŏ��s�B
int FileIOManagementPub::Close( unsigned int ionum ){
	return _MikanFile->Close( ionum );
}
// IO��j������B
int FileIOManagementPub::Destroy( unsigned int ionum ){
	return _MikanFile->Destroy( ionum );
}
int FileIOManagementPub::CloseAll( void ){
	return _MikanFile->CloseAll();
}

int FileIOManagementPub::Remove( const char *file ){
	return _MikanFile->Remove( file );
}

int FileIOManagementPub::Move( const char *oldfile, const char *newfile ){
	return _MikanFile->Move( oldfile, newfile );
}

// FileIO�̎擾�B
class FileIO * FileIOManagementPub::GetIO( unsigned int ionum ){
	return _MikanFile->GetIO( ionum );
}
class MikanIO * FileIOManagementPub::GetMikanIO( unsigned int ionum ){
	return _MikanFile->GetMikanIO( ionum );
}
