#ifndef CLASSDEF_MARMALADE
#define CLASSDEF_MARMALADE class Marmalade
// �ˑ����C�u����
#include <windows.h>
#ifdef USEMIKANLIBRARY
#ifdef _MIKANDEV
#include "Dev/DirectXDraw.h"
#else
#include "DirectXDraw.h"
#endif
// ���ł�_MikanDraw������B
//extern CLASSDEF_DIRECTX_DRAW *_MikanDraw;
#else
#ifdef _UNLOADLIBRARY
#include "../DirectXDraw/DirectXDraw.h"
#else
#ifdef _MIKANDEV
#include "Dev/DirectXDraw.h"
#else
#include "DirectXDraw.h"
#endif
#endif
// _MikanDraw���Ȃ��B
//CLASSDEF_DIRECTX_DRAW *_MikanDraw;
#endif

#ifndef _UNLINK_LIBRARY
#  ifdef _MIKANDEV
#    ifdef _DEBUG
#      pragma comment( lib, "Dev/MarmaladeD.lib" )
#    else
#      pragma comment( lib, "Dev/Marmalade.lib" )
#    endif
#  else
#    ifdef _DEBUG
#      pragma comment( lib, "MarmaladeD.lib" )
#    else
#      pragma comment( lib, "Marmalade.lib" )
#    endif
#  endif
#endif

// �ΏۂƂȂ�֐��̈��������p�̂ɂ���B
// ���p�̖̂w�ǂ͖����\���̂ƂȂ��Ă��āA���̖��O�͈ȉ��̃��[���Ɋ�Â��������K���ō���Ă���B
// �֐��ɗ^��������̌^�����ׂė񋓂��A���������Ȃ���B
// �������Aunsigned int�Ȃǂ�ui�Ƃ���2�����o���A�|�C���^�͍Ō��p������B
// �����^�̈������������݂���ꍇ�͐��l���g���Ă܂Ƃ߂邱�Ƃ��o����B
union FUNC_ARG
{
	int i;
	unsigned int ui;
	struct {
		int a0; int a1; unsigned long a2;
	} iiul;
	struct {
		int a0; int a1; int a2; unsigned long a3;
	} i3ul;
	struct {
		int a0; int a1; int a2; int a3; unsigned long a4;
	} i4ul;
	struct {
		int a0; int a1; int a2; unsigned long a3; double a4; int a5;
	} i3uldi;
	struct {
		int a0; int a1; int a2; int a3; unsigned long a4; double a5; int a6;
	} i4uldi;
	struct {
		int a0; int a1; int a2; int a3; unsigned long a4; unsigned long a5; unsigned long a6; unsigned long a7;
	} i4ul4;
	struct {
		int a0; int a1; int a2; int a3; int a4; int a5; int a6; int a7; unsigned long a8;
	} i8ul;
	struct {
		int a0; int a1; int a2; int a3; int a4; int a5; int a6; int a7; unsigned long a8; unsigned long a9; unsigned long a10; unsigned long a11;
	} i8ul4;
	struct {
		unsigned int a0; int a1; int a2; char *a3;
	} uii2cp;
	struct {
		unsigned int a0; int a1; int a2; int a3; int a4; int a5; int a6; int a7;
	} uii7;
	struct {
		unsigned int a0; int a1; int a2; int a3; int a4; int a5; int a6; int a7; int a8; int a9;
	} uii9;
	struct {
		unsigned int a0; int a1; int a2; int a3; int a4; int a5; int a6; double a7; int a8;
	} uii6di;
	struct {
		unsigned int a0; int a1; int a2; int a3; int a4; int a5; int a6; double a7; double a8; int a9;
	} uii6d2i;
	struct {
		unsigned int a0; int a1; int a2; int a3; int a4; int a5; int a6; int a7; int a8; double a9; int a10;
	}uii8di;
	struct {
		unsigned int a0; unsigned long a1;
	} uiul;
};

#define LAYER_MAX 32

class Marmalade
{
protected:
	unsigned int layer_max;
	struct FUNC_DATA_HEAD *layer;
	struct TEXTURE_LIST_HEAD *tex;
	CLASSDEF_DIRECTX_DRAW *_MikanDraw;
public:

	// �R���X�g���N�^
	Marmalade( CLASSDEF_DIRECTX_DRAW *directx_draw );
	Marmalade( CLASSDEF_DIRECTX_DRAW *directx_draw, int lmax );
	// �R���X�g���N�^�{��
	virtual int Init( CLASSDEF_DIRECTX_DRAW *directx_draw, int lmax = LAYER_MAX );
	// �f�X�g���N�^
	virtual ~Marmalade( void );

	virtual union FUNC_ARG * AddLayer( unsigned int lnum, int ( Marmalade::*func )( union FUNC_ARG * arg ) );
	virtual int DrawLayer( unsigned int lnum );
	virtual int ReleaseLayer( unsigned int lnum );

	// ��ʂ��Y��ɂ���
	virtual int ClearScreen( unsigned long color = 0xFF000000 );
	virtual int CountClearScreen( void );

	// �e�N�X�`���̍������@�̎w��
	virtual int SetRender( unsigned int lnum, int kind = DRAW_TRC );
	// ��ʂ������_�[�^�[�Q�b�g�ɂ���B
	virtual int SetRenderTarget( unsigned int lnum );
	// �e�N�X�`���������_�[�^�[�Q�b�g�ɂ���B
	virtual int SetRenderTarget( unsigned int lnum, unsigned int txnum );
	// �e�N�X�`������鏀��
	virtual unsigned int AddTexture( char *filename );
	virtual unsigned int AddTexture( char *filename, unsigned int tnum );
	// �摜��ǂݍ���Ńe�N�X�`�������
	virtual unsigned int CreateTexture( char *filename, unsigned long tcolor = 0x00000000, int thread = 0 );
	// ���\�[�X��ǂݍ���Ńe�N�X�`�������
	virtual unsigned int CreateTexture( HMODULE module, char *resource, unsigned long tcolor = 0x00000000, int thread = 0 );
	// ��̃e�N�X�`�������
	virtual unsigned int CreateTexture( unsigned int width, unsigned int height, int thread = 0 );
	// �摜��ǂݍ���Ńe�N�X�`�������
	virtual int CreateTexture( unsigned int txnum, char *filename, unsigned long tcolor = 0x00000000, int thread = 0 );
	// ���\�[�X��ǂݍ���Ńe�N�X�`�������
	virtual int CreateTexture( unsigned int txnum, HMODULE module, char *resource, unsigned long tcolor = 0x00000000, int thread = 0 );
	// ��̃e�N�X�`�������
	virtual int CreateTexture( unsigned int txnum, unsigned int width, unsigned int height, int thread = 0 );
	// �e�N�X�`���̉��
	virtual int ReleaseTexture( unsigned int txnum );
	// ���p�`�̉��
	//  virtual int ReleasePolygon( unsigned int pvnum );
	// �_��`�悷��B
	virtual int DrawPixel( unsigned int lnum, int x, int y, unsigned long color = 0xffffffff );
	// ������`�悷��B
	virtual int DrawLine( unsigned int lnum, int sx, int sy, int ex, int ey, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );
	// �l�p�`��`�悷��
	virtual int DrawBox( unsigned int lnum, int dx, int dy, int w, int h, unsigned long color );
	// �l�p�`��`�悷��
	virtual int DrawBox( unsigned int lnum, int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 );
	// �l�p�`��`�悷��
	virtual int DrawBox( unsigned int lnum, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color );
	// �l�p�`��`�悷��
	virtual int DrawBox( unsigned int lnum, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 );
	// �l�p�`��`�悷��
	virtual int DrawBoxC( unsigned int lnum, int dx, int dy, int w, int h, unsigned long color );
	// �l�p�`��`�悷��
	virtual int DrawBoxC( unsigned int lnum, int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 );
	// �~�̐��`��(�~�̂�)�B
	virtual int DrawCircleLineC( unsigned int lnum, int x, int y, int r, unsigned long color = 0xFFFFFFFF, double width = 1.0, int antialias = 0 );
	// �~�̐��`��(�����Ɏ��܂�ȉ~)�B
	virtual int DrawCircleLine( unsigned int lnum, int x, int y, int w, int h, unsigned long color = 0xFFFFFFFF, double width = 1.0, int antialias = 0 );
	// �~�`��(�~�̂�)�B
	virtual int DrawCircleC( unsigned int lnum, int x, int y, int r, unsigned long color = 0xFFFFFFFF );
	// �~�`��(�����Ɏ��܂�ȉ~)�B
	virtual int DrawCircle( unsigned int lnum, int x, int y, int w, int h, unsigned long color = 0xFFFFFFFF );
	// �e�N�X�`����`�悷��
	virtual int DrawTexture( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	// �e�N�X�`����`�悷��
	virtual int DrawTextureC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	// �e�N�X�`�����g��k���`�悷��
	virtual int DrawTextureScaling( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// �e�N�X�`�����g��k���`�悷��
	virtual int DrawTextureScaling( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	// �e�N�X�`�����g��k���`�悷��
	virtual int DrawTextureScalingC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// �e�N�X�`�����g��k���`�悷��
	virtual int DrawTextureScalingC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	// �e�N�X�`������]�`�悷��
	virtual int DrawTextureRotationC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 );
	// �e�N�X�`������]�`�悷��
	virtual int DrawTextureRotationAngleC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 );
	// �e�N�X�`������]�g��`�悷��
	virtual int DrawTextureRotateScaleC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 );
	// �e�N�X�`������]�g��`�悷��
	virtual int DrawTextureRotateScaleAngleC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 );
	// �e�N�X�`������]�g��`�悷��
	virtual int DrawTextureRotateScaleC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag = 0 );
	// �e�N�X�`������]�g��`�悷��
	virtual int DrawTextureRotateScaleAngleC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag = 0 );
	// �e�N�X�`����4�_�����R�Ɍ��߂ĕ`�悷��
	//  virtual int DrawTextureVertex( unsigned int lnum, unsigned int txnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 );
	// �e�N�X�`����4�_�𑊑΍��W�Ŏ��R�Ɍ��߂ĕ`�悷��
	//  virtual int DrawTextureRelativeVertex( unsigned int lnum, unsigned int txnum, int rx, int ry, int w, int h, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3, int drawflag = 0 );
	// �e�N�X�`����4�_�𑊑΍��W�Ŏ��R�Ɍ��߂ĕ`�悷��(�e�N�X�`���`�F�b�N�Ȃ�:�����p)
	// ���ׂẴe�N�X�`���`��͍ŏI�I�ɂ���̊֐ߊ֐��ƂȂ��Ă���
	//  virtual int DrawTextureRelativeVertex( unsigned int lnum, unsigned int txnum, int rx, int ry, int w, int h, double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, int drawflag = 0 );
	// ���p�`�̓o�^(���X�g)
	//  virtual int CreatePolygon( unsigned int pnum, D3DPRIMITIVETYPE filltype = D3DPT_TRIANGLESTRIP );
	// ���p�`�̓o�^(�z��)
	//  virtual int CreatePolygon( unsigned int pnum, unsigned int max, D3DPRIMITIVETYPE filltype = D3DPT_TRIANGLESTRIP );
	// �����p�`�̓o�^
	//  virtual int CreateRegularPolygon( unsigned int pnum, double cx, double cy, unsigned int n, double r, double rad = 0.0, unsigned long color = 0xffffffff );
	// �e�N�X�`���̓o�^
	//  virtual int SetTextureOnPolygon( unsigned int pnum, unsigned int tnum );
	//���_�̓o�^
	//  virtual int SetPolygonVertex( unsigned int pnum, double x, double y, double z, unsigned long color );
	//  virtual int SetPolygonVertex( double x, double y, double z, unsigned long color );
	// ���_�v�f���̎擾
	//  virtual unsigned int GetPolygonVertexLength( unsigned int pnum );
	// ���_���̕ҏW
	//  virtual int EditPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, double z, unsigned long color = 0xffffffff, double u = 0.0, double v = 0.0, int edit = PVEDIT_DEFAULT );
	// ���p�`�̕`��͈͂��擾�Bxyz���ꂼ��B
	// ���p�`�̕ό`
	// �ړ�
	//  virtual int TransformPolygonMoving( unsigned int pnum, double mx, double my, double mz = 0.0 );
	// �g��
	//  virtual int TransformPolygonScaling( unsigned int pnum, double scale, unsigned int basenum = 0 );
	// ���p�`�̕`��
	//  virtual int DrawPolygonVertex( unsigned int pnum );
	//  virtual int DrawPolygonVertex( unsigned int pnum, unsigned int draw );
	//  virtual int DrawPolygonVertex( unsigned int pnum, unsigned int begin, unsigned int end );
	//  virtual int DrawPolygon( unsigned int pnum, double basex, double basey, int cx, int cy );
	//  virtual int DrawPolygon( unsigned int pnum, int cx, int cy, unsigned int basenum = 0 );
	// ���p�`�̊g��`��
	//  virtual int DrawPolygonScaling( unsigned int pnum, double basex, double basey, int cx, int cy, double scale );
	//  virtual int DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale, unsigned int basenum = 0 );
	// scale x,y,z�w��\�Ȃ��̂Ƃ��p�ӁB

	// �����p�`�̕`��
	//  virtual int DrawRegularPolygon( double cx, double cy, unsigned int n, double r, double rad = 0.0, unsigned long color = 0xffffffff );
	//  virtual int DrawRegularPolygon( double cx, double cy, unsigned int n, int drawnum, double r, double rad = 0.0, unsigned long color = 0xffffffff );
	// �F�̍쐬
	virtual unsigned long Color( unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue );
	virtual unsigned long ColorHSV( unsigned char alpha, unsigned int hue, unsigned char sat, unsigned char vob );
	// �F�̐ݒ�
	virtual int SetColor( unsigned int lnum, unsigned int txnum, unsigned long color );
	// ���_�F�̐ݒ�
	//  virtual int SetColorVertex( unsigned int txnum, unsigned long color );
	// �e�N�X�`���̃f�[�^��ǂ߂�悤�ɂ���B
	//  virtual int OpenTexture( unsigned int txnum, int x = 0, int y = 0, int w = 0, int h = 0 );
	// �e�N�X�`���̃f�[�^��ǂ߂��Ԃ���߂��B
	//  virtual int CloseTexture( unsigned int txnum );
	// �e�N�X�`���̎w����W�̐F���擾����B
	//  virtual unsigned long GetColorTexturePixel( unsigned int txnum, int x, int y );
	// �e�N�X�`���͈͓̔��̐F��u������B
	//  virtual unsigned int TextureColorReplacement( unsigned int txnum, unsigned long scolor, unsigned long rcolor, int x = 0, int y = 0, int w = 0, int h = 0 );
	// �e�N�X�`���͈͓̔��̐F����������B
	//  virtual unsigned int TextureColorSearch( unsigned int txnum, unsigned long scolor, int x = 0, int y = 0, int w = 0, int h = 0 );
	// �e�N�X�`���͈͓̔��̐F�����������𐔂���B
	//  virtual unsigned int TextureColorCount( unsigned int txnum, unsigned long scolor, int x = 0, int y = 0, int w = 0, int h = 0 );

	// �e�N�X�`���̉����擾
	//  virtual unsigned int GetTextureWidth( unsigned int txnum = 0 );
	// �e�N�X�`���̍����擾
	//  virtual unsigned int GetTextureHeight( unsigned int txnum = 0 );

	// �t�H���g

	virtual int LoadFontFile( char *fontfile );
	virtual int LoadFontFile( HMODULE module, char *resource );
	virtual int CreateFont( unsigned int fontnum, char *fontname, int size = 10, unsigned long color = 0xFFFFFFFF );
	virtual int CreateFont( unsigned int fontnum, int size, unsigned long color = 0xFFFFFFFF );
	virtual int ResetFont( unsigned int fontnum );
	//  virtual int CleanupFont( void );

	virtual int Printf( unsigned int lnum, unsigned int fontnum, int x, int y, char *format, ... );

	// ���C���[�o�^�p�֐��B
	virtual int RegSetRender( union FUNC_ARG * arg );
	virtual int RegSetRenderTarget0( union FUNC_ARG * arg );
	virtual int RegSetRenderTarget1( union FUNC_ARG * arg );
	virtual int RegDrawPixel( union FUNC_ARG * arg );
	virtual int RegDrawLine( union FUNC_ARG * arg );
	virtual int RegDrawBox0( union FUNC_ARG * arg );
	virtual int RegDrawBox1( union FUNC_ARG * arg );
	virtual int RegDrawBox2( union FUNC_ARG * arg );
	virtual int RegDrawBox3( union FUNC_ARG * arg );
	virtual int RegDrawBoxC0( union FUNC_ARG * arg );
	virtual int RegDrawBoxC1( union FUNC_ARG * arg );
	virtual int RegDrawCircleLineC( union FUNC_ARG * arg );
	virtual int RegDrawCircleLine( union FUNC_ARG * arg );
	virtual int RegDrawCircleC( union FUNC_ARG * arg );
	virtual int RegDrawCircle( union FUNC_ARG * arg );
	virtual int RegDrawTexture( union FUNC_ARG * arg );
	virtual int RegDrawTextureC( union FUNC_ARG * arg );
	virtual int RegDrawTextureScaling0( union FUNC_ARG * arg );
	virtual int RegDrawTextureScaling1( union FUNC_ARG * arg );
	virtual int RegDrawTextureScalingC0( union FUNC_ARG * arg );
	virtual int RegDrawTextureScalingC1( union FUNC_ARG * arg );
	virtual int RegDrawTextureRotationC( union FUNC_ARG * arg );
	virtual int RegDrawTextureRotationAngleC( union FUNC_ARG * arg );
	virtual int RegDrawTextureRotateScaleC0( union FUNC_ARG * arg );
	virtual int RegDrawTextureRotateScaleAngleC0( union FUNC_ARG * arg );
	virtual int RegDrawTextureRotateScaleC1( union FUNC_ARG * arg );
	virtual int RegDrawTextureRotateScaleAngleC1( union FUNC_ARG * arg );
	//  virtual int RegDrawTextureVertex( union FUNC_ARG * arg );
	//  virtual int RegDrawTextureRelativeVertex0( union FUNC_ARG * arg );
	//  virtual int RegDrawTextureRelativeVertex1( union FUNC_ARG * arg );
	//  virtual int SetTextureOnPolygon( union FUNC_ARG * arg );
	//  virtual int SetPolygonVertex0( union FUNC_ARG * arg );
	//  virtual int SetPolygonVertex1( union FUNC_ARG * arg );
	//  virtual int EditPolygonVertex( union FUNC_ARG * arg );
	//  virtual int TransformPolygonMoving( union FUNC_ARG * arg );
	//  virtual int TransformPolygonScaling( union FUNC_ARG * arg );
	//  virtual int DrawPolygonVertex0( union FUNC_ARG * arg );
	//  virtual int DrawPolygonVertex1( union FUNC_ARG * arg );
	//  virtual int DrawPolygonVertex2( union FUNC_ARG * arg );
	//  virtual int DrawPolygon0( union FUNC_ARG * arg );
	//  virtual int DrawPolygon1( union FUNC_ARG * arg );
	//  virtual int DrawPolygonScaling0( union FUNC_ARG * arg );
	//  virtual int DrawPolygonScaling1( union FUNC_ARG * arg );
	//  virtual int DrawRegularPolygon0( union FUNC_ARG * arg );
	//  virtual int DrawRegularPolygon1( union FUNC_ARG * arg );
	virtual int RegSetColor( union FUNC_ARG * arg );
	virtual int RegPrintf( union FUNC_ARG * arg );

};

struct LAYER_DATA
{
	struct LAYER_DATA *next;
	int ( Marmalade::*func )( union FUNC_ARG *arg );//Marmalade::
	union FUNC_ARG *arg;
};

struct FUNC_DATA_HEAD
{
	struct LAYER_DATA *first;
	struct LAYER_DATA *now;
};

struct TEXTURE_LIST
{
	unsigned int tnum;
	char *file;
	struct TEXTURE_LIST *next;
};

struct TEXTURE_LIST_HEAD
{
	struct TEXTURE_LIST *first;
	struct TEXTURE_LIST *now;
	unsigned int read;
};

#ifdef USEMIKANLIBRARY
#ifndef MARMALADE_INIT_FUNC
#define MARMALADE_INIT_FUNC
static CLASSDEF_MARMALADE *Marm;
static int MarmaladeInit( void )
{
	Marm = new Marmalade( _MikanDraw );
	return 0;
}
static int MarmaladeTerm( void )
{
	delete( Marm );
	return 0;
}
#endif
#endif

#endif
