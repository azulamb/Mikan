#ifndef CLASSDEF_DIRECTX_DRAW
#define CLASSDEF_DIRECTX_DRAW class DirectXDraw9
//#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <stdio.h>
#include <stdarg.h>
#include <varargs.h>

#ifndef _UNLINK_LIBRARY
#  ifdef _MIKANDEV
#    ifdef _DEBUG
#      pragma comment( lib, "Dev/DirectXDrawD.lib" )
#    else
#      pragma comment( lib, "Dev/DirectXDraw.lib" )
#    endif
#  else
#    ifdef _DEBUG
#      pragma comment( lib, "DirectXDrawD.lib" )
#    else
#      pragma comment( lib, "DirectXDraw.lib" )
#    endif
#  endif
#  pragma comment( lib, "d3dx9.lib" )
#  pragma comment( lib, "d3d9.lib" )
#endif

#include "DirectXDrawBase.h"

#define	WM_GRAPH_NOTIFY (WM_APP + 1)

//#define MIKAN_CUSTOMVERTEX_2D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define MIKAN_CUSTOMVERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//WinAPI��CreateFont��CreateFontA��CreateFontW�ɏ���������}�N���Ƃ��Ē�`����Ă���B
//���������Ȃ��ƁAMikan��CreateFont��A��W�����Ă��܂��B
#undef CreateFont

#define D3D_DEBUG_INFO
#define MIKAN_PI            3.14159265358979323846264338327950288
#define AngleToRadian(a)    ( MIKAN_PI * 2 * (a) / 360 )
#define DX_RELEASE(r)       { if( r ){ ( r )->Release(); } r = NULL; }
#define DX_FREE(f)          { if( f ){ free( f ); } f = NULL; }
#define COLOR_RGB(r,g,b)    ( D3DCOLOR_XRGB( r, g, b ) )
#define COLOR_ARGB(a,r,g,b) ( D3DCOLOR_ARGB( a, r, g, b ) )
#define COLOR_RGBA(r,g,b,a) ( D3DCOLOR_RGBA( r, g, b, a ) )
//�e�N�X�`���ԍ����L���ȏꍇ�ɐ^
#define CanUseTexture(t)    ( (t) < texmax && dxtex[(t)].ext )
//�e�N�X�`���ԍ����s���ȏꍇ�ɐ^
#define CannotUseTexture(t) ( (t) >= texmax || dxtex[(t)].ext == 0 )
//�e�N�X�`�����̏����l
#define DX_TEXTURE_MAX      256
//�|���S�����̏����l
#define DX_POKYGON_MAX      (1024-1)
//�t�H���g���̏����l
#define DX_FONT_MAX         256
//�o�b�t�@�����l
#define MAX_BUFFER_SIZE     1024

//�e�N�X�`���������̓��ߐF�̎w��
enum
{
	TRC_NONE = 0x00000000, //���ߐF�Ȃ�
	TRC_ZERO = 0x00FFFFFF  //����̐F�𓧉ߐF�ɂ���
};

//�����_�����O�t���O
enum
{
	DRAW_TRC,       //���ߐF��L���ɂ��ď㏑���`��
	DRAW_ADD,       //���ߐF��L���ɂ��ĉ��Z�`��
	DRAW_SUB,       //���ߐF��L���ɂ��Č��Z�`��
	DRAW_MUL,       //���ߐF�𖳌��ɂ��ď�Z�`��
	DRAW_MUL_WHITE, // ���ƍ������ď�Z�`��
	DRAW_FILL,      // ���ߐF��L���ɂ��ēh��Ԃ��B
	DRAW_HALFADD,   // �����Z����
	DRAW_CADD_ASUB, // ���ߐF�͌��Z�A����ȊO�͉��Z
	DRAW_BLUR, // �u���[�H
	DRAW_CUT, // �J�b�g�p
	DRAW_NON, //���ߐF�𖳌��ɂ��ď㏑���`��
};

// ���_�ҏW�t���O
enum
{
	PVEDIT_X = 1,
	PVEDIT_Y = 2,
	PVEDIT_XY = 3,
	PVEDIT_Z = 4,
	PVEDIT_POS = 7,
	PVEDIT_COLOR = 8,
	PVEDIT_DEFAULT = 15,
	PVEDIT_U = 16,
	PVEDIT_V = 32,
	PVEDIT_UV = 48,
	PVEDIT_ALL = 0xFFFFFFFF,
};
// �t�@�C���t���O
enum
{
	IMAGE_BMP = D3DXIFF_BMP, //0
	IMAGE_JPG = D3DXIFF_JPG, //1
	IMAGE_TGA = D3DXIFF_TGA, //2
	IMAGE_PNG = D3DXIFF_PNG, //3
	IMAGE_DDS = D3DXIFF_DDS, //4
	IMAGE_PPM = D3DXIFF_PPM, //5
	IMAGE_DIB = D3DXIFF_DIB, //6
	IMAGE_MAX,
};

/*!
@struct  MYTEXTURE
@brief   �e�N�X�`���֘A�̃f�[�^���i�[�����\���́B
@author  Hiroki
@version 1.0
*/
struct MYTEXTURE
{
	//�e�N�X�`��
	IDirect3DTexture9 *tex;
	//�T�[�t�F�C�X
	IDirect3DSurface9 *sur[ 2 ];
	//�X�e���V���T�[�t�F�C�X
	IDirect3DSurface9 *zsur;
	//���݃t���O
	int ext;
	// ���b�N�t���O
	int lock;
	//���b�N�����`�̈�
	D3DLOCKED_RECT lockrect;
	// �J�b�g����̈�
	RECT cutrect;
	//���\�[�X�t���O
	int resource;
	//�t�@�C����
	char *filename;
	//���W���[��
	HMODULE module;
	//�T�C�Y
	unsigned int width;
	unsigned int height;
	//���̑��e�N�X�`���������Ɏw��ł���l
	unsigned int miplevel;
	unsigned long usage;
	D3DFORMAT format;
	D3DPOOL pool;
	unsigned int filter;
	unsigned int mipfilter;
	unsigned long color;
	unsigned long vcolor[ 4 ];
	D3DXIMAGE_INFO * imgdata;
	PALETTEENTRY *pallete;
	//���̐F�f�[�^
	IDirect3DSurface9 *backup;//unsigned long *rawdata;
	//�������݃t���O
	int writed;
};

/*!
@struct  THREADIN_TEX
@brief   �e�N�X�`���𐶐�����X���b�h�ɓn���\���́B
@author  Hiroki
@version 1.0
*/
struct THREADIN_TEX
{
	CLASSDEF_DIRECTX_DRAW *dxd;
	int num;
	int mode;
	void *resouce;
	unsigned int resoucesize;
};

struct CUSTOMVERTEX_LIST
{
	float         x, y, z; // ���_���W
#ifdef USERHW
	float         rhw;     // ���Z��
#endif
	unsigned long color;   // ���_�̐F
	float         u, v;    // �e�N�X�`�����W
	struct CUSTOMVERTEX_LIST *next;
};

union MYVERTEX
{
	struct CUSTOMVERTEX *cv;
	struct CUSTOMVERTEX_LIST *cvl;
};

struct POLYGON_VERTEX
{
	unsigned int max;
	union
	{
		unsigned int num;  struct CUSTOMVERTEX_LIST * pointer;
	} read;
	int type;
	union MYVERTEX vertex;
	D3DPRIMITIVETYPE filltype;
	unsigned int txnum;
	double space[ 6 ]; // (min & max) * (x, y, z)
	struct CUSTOMVERTEX *cv;
	unsigned int cvmax;
};

//Font
//#define MAX_FONT     256
//#define DEFAULT_FONT "�l�r �S�V�b�N"
#define DEFAULT_FONT "MS �S�V�b�N"

/*!
@struct  FONTDATA
@brief   �t�H���g�����i�[�����\���́B
@author  Hiroki
@version 1.0
*/
struct FONTDATA
{
	char use;
	int height;
	unsigned int width;
	unsigned int weight;
	unsigned int miplevel;
	bool italic;
	unsigned long charset;
	unsigned long outputprecision;
	unsigned long quality;
	unsigned long pitchandfamily;
	char fontname[ 64 ];
	ID3DXFont *D3DFont;
	RECT rect;
	unsigned long color;
};

enum
{
	DRAW_NONE,
	DRAW_SWAPCHAIN,
};

class DirectXDraw9 : public DirectXDrawBase
{
protected:
	// �e�N�X�`���ő吔
	unsigned int texmax;
	// �e�N�X�`��
	struct MYTEXTURE *dxtex;
	// ���p�`�ő吔
	unsigned int pvmax;
	// �Ō�Ɏg�������p�`�ԍ�
	unsigned int lastpvnum;
	// ���p�`
	struct POLYGON_VERTEX *pv;
	struct CUSTOMVERTEX *rpv;
	unsigned int rpvmax;
	// ���C��
	struct D3DXVECTOR2 linevec[ 2 ];
	struct ID3DXLine *line;
	// �t�H���g�ő吔
	unsigned int fontmax;
	// �t�H���g
	struct FONTDATA *fontdata;
	// ��ʎg�p�t���O,�e�N�X�`���ԍ�
	unsigned int target[ 2 ];
	// ���������t���O
	unsigned int vsync;
	// �e�N�X�`���������݃t���O
	unsigned int candrawtexture;
	// �T�[�t�F�C�X�������݃t���O
	unsigned int candrawsurface;
	int countcs;
	// �e�N�X�`���o�b�N�A�b�v���s�����ǂ����̐ݒ�
	int texturebackup;
	unsigned int swapchainmax;
	// �`��T�C�Y
	int swidth, sheight;
	// �`�撆���ǂ����B
	int drawing, drawing_;
	// /BeginEndScene�̃��b�N�B
	int lock;
	unsigned long locksleeptime;

	// �}���`�X���b�h���ǂ����B
	int ismultithread;

	int fullscreen;
	int ( DirectXDraw9::*drawfunc )( int num );
public:
	// Microsoft���p�ӂ����֐��G���[�̕Ԃ�l
	long hr;
	IDirect3D9 *D3D;
	D3DPRESENT_PARAMETERS D3DPP;
	IDirect3DDevice9 *D3DDev;
	IDirect3DSurface9 *backbuffer;
	IDirect3DSurface9 *backbufferz;
	IDirect3DSwapChain9 **swapchain;
	HWND WindowHandle;
	//struct CUSTOMVERTEX dv[4];
	int swapchainnum;

	// �R���X�g���N�^
	DirectXDraw9( void );
	// �R���X�g���N�^
	DirectXDraw9( HWND wh, unsigned int tmax = DX_TEXTURE_MAX, unsigned int pmax = DX_POKYGON_MAX, unsigned int fmax = DX_FONT_MAX );
	// �R���X�g���N�^�{��
	virtual int Init( HWND wh = NULL, unsigned int tmax = DX_TEXTURE_MAX, unsigned int pmax = DX_POKYGON_MAX, unsigned int fmax = DX_FONT_MAX );
	// �f�X�g���N�^
	virtual ~DirectXDraw9( void );
	// �E�B���h�E�n���h���̐ݒ�
	virtual HWND SetWindowHandle( HWND wh );
	// �E�B���h�E�n���h���̐ݒ�
	virtual HWND GetWindowHandle( void );
	virtual int SetFullScreenMode( int fullscreen_on = 1 );
	// ���������҂��̐ݒ�
	virtual int SetVsyncWait( int vs = 0 );
	// �e�N�X�`���̍ő吔�̐ݒ�
	virtual unsigned int SetTextureMax( unsigned int tmax = DX_TEXTURE_MAX );
	virtual unsigned int GetTextureMax( void );
	virtual int IsEmptyTexture( unsigned int txnum );
	// ���p�`���_�̍ő吔�̐ݒ�
	virtual unsigned int SetPolygonMax( unsigned int pmax = DX_POKYGON_MAX );
	// �`��T�C�Y�̐ݒ�
	virtual int SetScreenSize( int width, int height );
	// �`��T�C�Y�̎擾
	virtual int GetScreenWidth( int now = 0 );
	virtual int GetScreenHeight( int now = 0 );
	// DirectX�̏�����
	virtual int InitD3D( int create = 1 );
	// DirectX�̉��
	virtual int CleanupD3D( void );
	// DirectX�̃I�u�W�F�N�g�̏�����
	virtual int InitD3DObject( void );
	// DirectX�̃I�u�W�F�N�g�̉��
	virtual int CleanupD3DObject( void );
	// �e�N�X�`���̃o�b�N�A�b�v�������ōs�����ǂ����̐ݒ�
	virtual int SetAutoBackUpTexture( int enable = 1 );
	// Texture(Surface)�̃o�b�N�A�b�v
	virtual int BackUpTexture( unsigned int txnum, int force = 0 );

	// �o�b�N�A�b�v
	virtual int BackUp( void );
	// �񕜍��
	virtual int Recover( void );
	// �f�o�C�X���X�g����̕��A
	virtual int RecoverFromDeviceLost( int force = 0 );

	virtual int SetDrawFunction( int mode = DRAW_SWAPCHAIN );
	virtual int Draw( int num );
	virtual int DrawNull( int num )
	{
		return num;
	}

	// ��ʂ��Y��ɂ���
	//  virtual int ClearScreen( void );
	// ��ʂ��Y��ɂ���
	virtual int ClearScreen( unsigned long color = 0xFF000000 );
	virtual int CountClearScreen( void );

	// �e�N�X�`���̏���
	virtual int InitRender( void );
	// �e�N�X�`���̍������@�̎w��
	virtual int SetRender( int kind = DRAW_TRC );
	// ��ʂ������_�[�^�[�Q�b�g�ɂ���B
	virtual int SetRenderTarget( void );
	// �e�N�X�`���������_�[�^�[�Q�b�g�ɂ���B
	virtual int SetRenderTarget( unsigned int txnum );
	// �����_�[�^�[�Q�b�g�����ɖ߂�(���O�ɉ�ʂ��������ʁA�e�N�X�`����������e�N�X�`��)�B
	virtual int ResetRenderTarget( void );
	// �摜��ǂݍ���Ńe�N�X�`�������
	virtual int CreateTexture( unsigned int txnum, const char *filename, unsigned long tcolor = 0x00000000, int thread = 0 );
	// ���\�[�X��ǂݍ���Ńe�N�X�`�������
	virtual int CreateTexture( unsigned int txnum, HMODULE module, const char *resource, unsigned long tcolor = 0x00000000, int thread = 0 );
	// ��̃e�N�X�`�������
	virtual int CreateTexture( unsigned int txnum, unsigned int width, unsigned int height, int thread = 0 );
	// ���Ńe�N�X�`�������
	static unsigned long WINAPI ThreadCreateTexture( void * threadin );
	// ���Ń��\�[�X����e�N�X�`�������
	static unsigned long WINAPI ThreadCreateTextureFromResorce( void * threadin );
	// ���ŋ�̃e�N�X�`�������
	static unsigned long WINAPI ThreadCreateTextureEmpty( void * threadin );
	// �e�N�X�`���̉��
	virtual int ReleaseTexture( unsigned int txnum );
	// �S�e�N�X�`���̉���B
	virtual int ReleaseTexture();
	// ���p�`�̉��
	virtual int ReleasePolygon( unsigned int pvnum );

	// �_��`�悷��B
	virtual int DrawPixel( int x, int y, unsigned long color = 0xffffffff );

	// ������`�悷��B
	virtual int DrawLine( int sx, int sy, int ex, int ey, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );
	virtual int DrawBoxLine( int dx, int dy, int w, int h, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );
	virtual int DrawBoxLineC( int dx, int dy, int w, int h, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );
	// �l�p�`��`�悷��
	virtual int DrawBox( int dx, int dy, int w, int h, unsigned long color );
	// �l�p�`��`�悷��
	virtual int DrawBox( int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 );
	// �l�p�`��`�悷��
	virtual int DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color );
	// �l�p�`��`�悷��
	virtual int DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 );
	// �l�p�`��`�悷��
	virtual int DrawBoxC( int dx, int dy, int w, int h, unsigned long color );
	// �l�p�`��`�悷��
	virtual int DrawBoxC( int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 );
	virtual int DrawCircleLineC( int x, int y, int r, unsigned long color = 0xFFFFFFFF, double width = 1.0, int antialias = 0 );
	// �~�̕`��
	virtual int DrawCircleC( int x, int y, int r, unsigned long color = 0xFFFFFFFF );
	virtual int DrawCircleLine( int x, int y, int w, int h, unsigned long color = 0xFFFFFFFF, double width = 1.0, int antialias = 0 );
	// �ȉ~��������Ȃ��`��
	virtual int DrawCircle( int x, int y, int w, int h, unsigned long color = 0xFFFFFFFF );

	// �e�N�X�`���`��̏I��
	virtual int EndDrawTexture( IDirect3DTexture9 *tex );
	// �e�N�X�`����`�悷��
	virtual int DrawTexture( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	// �e�N�X�`����`�悷��
	virtual int DrawTextureC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );

	// �e�N�X�`�����g��k���`�悷��
	virtual int DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// �e�N�X�`�����g��k���`�悷��
	virtual int DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	// �e�N�X�`�����g��k���`�悷��
	virtual int DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// �e�N�X�`�����g��k���`�悷��
	virtual int DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );

	// �e�N�X�`������]�`�悷��
	virtual int DrawTextureRotationC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 );
	// �e�N�X�`������]�`�悷��
	virtual int DrawTextureRotationAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 );

	// �e�N�X�`������]�g��`�悷��
	virtual int DrawTextureRotateScale( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag = 0 );
	// �e�N�X�`������]�g��`�悷��
	virtual int DrawTextureRotateScale( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag = 0 );

	// �e�N�X�`������]�g��`�悷��
	virtual int DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 );
	// �e�N�X�`������]�g��`�悷��
	virtual int DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 );
	// �e�N�X�`������]�g��`�悷��
	virtual int DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag = 0 );
	// �e�N�X�`������]�g��`�悷��
	virtual int DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag = 0 );

	// �e�N�X�`����4�_�����R�Ɍ��߂ĕ`�悷��
	virtual int DrawTextureVertex( unsigned int txnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 );
	// �e�N�X�`����4�_�𑊑΍��W�Ŏ��R�Ɍ��߂ĕ`�悷��
	virtual int DrawTextureRelativeVertex( unsigned int txnum, int rx, int ry, int w, int h, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3, int drawflag = 0 );
	// �e�N�X�`����4�_�𑊑΍��W�Ŏ��R�Ɍ��߂ĕ`�悷��(�e�N�X�`���`�F�b�N�Ȃ�:�����p)
	// ���ׂẴe�N�X�`���`��͍ŏI�I�ɂ���̊֐ߊ֐��ƂȂ��Ă���
	virtual int DrawTextureRelativeVertex( unsigned int txnum, int rx, int ry, int w, int h, double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, int drawflag = 0 );
	// ���p�`�̓o�^(���X�g)
	virtual int CreatePolygon( unsigned int pnum, D3DPRIMITIVETYPE filltype = D3DPT_TRIANGLESTRIP );
	// ���p�`�̓o�^(�z��)
	virtual int CreatePolygon( unsigned int pnum, unsigned int max, D3DPRIMITIVETYPE filltype = D3DPT_TRIANGLESTRIP );
	// �����p�`�̓o�^
	virtual int CreateRegularPolygon( unsigned int pnum, double cx, double cy, unsigned int n, double r, double rad = 0.0, unsigned long color = 0xffffffff );
	// �e�N�X�`���̓o�^
	virtual int SetTextureOnPolygon( unsigned int pnum, unsigned int tnum );
	// �e�N�X�`���̉���
	virtual int SetTextureOnPolygon( unsigned int pnum );
	// �e�N�X�`���ԍ��̎擾�B
	virtual unsigned int GetTextureNumOnPolygon( unsigned int pnum );
	// UV���W�̓o�^�B
	virtual int SetPolygonUV( unsigned int pnum, unsigned int vnum, int u, int v );
	//���_�̓o�^(�z�����)
	virtual int SetPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color );
	//���_�̓o�^
	virtual int SetPolygonVertex( unsigned int pnum, double x, double y, unsigned long color );
	//  virtual int SetPolygonVertex( double x, double y, unsigned long color );
	//���_���̎擾
	virtual struct CUSTOMVERTEX * GetPolygonVertex( unsigned int pnum, unsigned int vnum );
	// ���_�v�f���̎擾
	virtual unsigned int GetPolygonVertexLength( unsigned int pnum );
	// ���_���̕ҏW
	virtual int EditPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color = 0xffffffff, double u = 0.0, double v = 0.0, int edit = PVEDIT_DEFAULT );
	// ���p�`�̕`��͈͂��擾�Bxy���ꂼ��B
	// ���p�`�̕ό`
	// �ړ�
	virtual int TransformPolygonMoving( unsigned int pnum, double mx, double my );
	// �g��
	virtual int TransformPolygonScaling( unsigned int pnum, double scale, unsigned int basenum = 0 );
	// ���p�`�̕`��
	//  virtual int DrawPolygonVertex( unsigned int pnum );
	//  virtual int DrawPolygonVertex( unsigned int pnum, unsigned int draw );
	//  virtual int DrawPolygonVertex( unsigned int pnum, unsigned int begin, unsigned int end );
	//  virtual int DrawPolygon( unsigned int pnum, double basex, double basey, int cx, int cy );
	virtual int DrawPolygon( unsigned int pnum, int cx, int cy, unsigned int basenum, unsigned int begin, unsigned int end );
	virtual int DrawPolygon( unsigned int pnum, int cx, int cy, unsigned int begin, unsigned int end );
	virtual int DrawPolygon( unsigned int pnum, int cx, int cy );
	virtual int DrawPolygon( unsigned int pnum );
	// ���p�`�̊g��`��
	/*  virtual int DrawPolygonScaling( unsigned int pnum, double basex, double basey, int cx, int cy, double scale );*/
	virtual int DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale, unsigned int basenum, unsigned int start, unsigned int end );
	virtual int DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale, unsigned int start, unsigned int end );
	virtual int DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale );
	// scale x,y,z�w��\�Ȃ��̂Ƃ��p�ӁB
	// �����p�`�̕`��
	virtual int DrawRegularPolygon( double cx, double cy, unsigned int n, int drawnum, double r, double rad = 0.0, unsigned long color = 0xffffffff );
	virtual int DrawRegularPolygon( double cx, double cy, unsigned int n, double r, double rad = 0.0, unsigned long color = 0xffffffff );

	// �F�̍쐬
	virtual unsigned long Color( unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue );
	virtual unsigned long ColorHSV( unsigned char alpha, unsigned int hue, unsigned char sat, unsigned char vob );
	// �F�̐ݒ�
	virtual int SetColor( unsigned int txnum, unsigned long color = 0xFFFFFFFF );
	// �A���t�@�l�̐ݒ�
	virtual int SetAlpha( unsigned int txnum, unsigned char alpha );
	// ���_�F�̐ݒ�
	virtual int SetColorVertex( unsigned int txnum, unsigned long color );

	// �e�N�X�`���̃f�[�^��ǂ߂�悤�ɂ���B
	virtual int OpenTexture( unsigned int txnum, int x = 0, int y = 0, int w = 0, int h = 0 );
	// �e�N�X�`���̃f�[�^��ǂ߂��Ԃ���߂��B
	virtual int CloseTexture( unsigned int txnum );
	// �e�N�X�`���̎w����W�̐F���擾����B
	virtual unsigned long GetColorTexturePixel( unsigned int txnum, int x, int y );
	// �e�N�X�`���͈͓̔��̐F��u������B
	virtual unsigned int TextureColorReplacement( unsigned int txnum, unsigned long scolor, unsigned long rcolor, int x = 0, int y = 0, int w = 0, int h = 0 );
	// �e�N�X�`���͈͓̔��̐F����������B
	virtual unsigned int TextureColorSearch( unsigned int txnum, unsigned long scolor, int x = 0, int y = 0, int w = 0, int h = 0 );
	// �e�N�X�`���͈͓̔��̐F�����������𐔂���B
	virtual unsigned int TextureColorCount( unsigned int txnum, unsigned long scolor, int x = 0, int y = 0, int w = 0, int h = 0 );

	// �e�N�X�`���̉����擾
	virtual unsigned int GetTextureWidth( unsigned int txnum = 0 );
	// �e�N�X�`���̍����擾
	virtual unsigned int GetTextureHeight( unsigned int txnum = 0 );

	// �X�N���[���V���b�g
	virtual int ScreenShot( const char *filename, unsigned int imagetype = IMAGE_BMP );

	// �e�N�X�`�����摜�֏o��
	virtual int TextureToImage( const char *filename, unsigned int txnum, unsigned int imagetype = IMAGE_BMP );

	// �e�N�X�`���̕`��J�n
	virtual int Begin( void );
	// �e�N�X�`���̕`��I��
	virtual int End( void );
	// End���b�N�B
	virtual int LockDrawEnd( unsigned long sleeptime = 1 );
	// End�A�����b�N�B
	virtual int UnlockDarawEnd( void );
	// ! Begin��҂B
	/* !
	\param sleeptime Begin���s���܂ő҂P�ʎ��ԁB
	\param autounlock 1=�f�b�h���b�N�����̂��߂Ƀ��b�N����Ă���ꍇ�͎����I�ɉ�������(�f�t�H���g),0=�������Ȃ�
	*/
	virtual int WaitDrawBegin( unsigned long sleeptime = 1, unsigned int timeout = INFINITE, int autounlock = 1 );
	virtual int WaitDrawEnd( unsigned long sleeptime = 1, unsigned int timeout = INFINITE );

	virtual MYTEXTURE * GetTextureData( unsigned int txnum );

	// ! DirectX�ւ̃A�N�Z�X
	/* !
	\param force 1=DirectX3DDevice�������Ɏ擾����[�f�t�H���g],0=�`�撆�Ɏ擾����
	\param sleeptime �`�撆���ǂ������ׂ�Ԋu(�~���b)
	*/
	virtual IDirect3DDevice9 * GetDirect3DDevice( int force = 1, unsigned long sleeptime = 1 );
	virtual HRESULT SetRenderState( D3DRENDERSTATETYPE State, DWORD Value );
	virtual HRESULT SetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value );

	// �t�H���g

	virtual int FontCleanUp( void );
	virtual int FontRecover( void );
	virtual int FontInit( void );
	virtual int LoadFontFile( const char *fontfile );
	virtual int LoadFontFile( HMODULE module, const char *resource );
	virtual int CreateFont( unsigned int fontnum, const char *fontname, int size, unsigned long color, int weight, int italic = 0 );
	virtual int CreateFont( unsigned int fontnum, const char *fontname, int size = 10, unsigned long color = 0xFFFFFFFF );
	virtual int CreateFont( unsigned int fontnum, int size, unsigned long color = 0xFFFFFFFF );
	virtual int ResetFont( unsigned int fontnum );
	virtual int CleanupFont( void );

	virtual int Print( unsigned int fontnum, int x, int y, const char *str );
	virtual int Print( unsigned int fontnum, int x, int y, unsigned long color, const char *str );
	virtual int Printf( unsigned int fontnum, int x, int y, const char *format, ... );
	virtual int Printf( unsigned int fontnum, int x, int y, unsigned long color, const char *format, ... );

	virtual int ControlSize( int size );
};

#endif
