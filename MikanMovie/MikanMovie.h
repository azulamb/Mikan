#ifndef CLASSDEF_MIKANMOVIE
#define CLASSDEF_MIKANMOVIE class MikanMovieManagementInterface

#ifdef USEMIKANLIBRARY
#ifdef _MIKANDEV
#include "Dev/DirectXDraw.h"
#else
#include "DirectXDraw.h"
#endif
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
#endif

#ifndef _UNLINK_LIBRARY
#  ifdef _DEBUG
#    pragma comment( lib, "MikanMovieD.lib" )
#  else
#    pragma comment( lib, "MikanMovie.lib" )
#  endif
#endif

class MikanVMRSurfaceAllocator
{
private:
public:

	virtual STDMETHODIMP QueryInterface( REFIID riid, __deref_out void **ppv ) = 0;
	virtual STDMETHODIMP_( ULONG ) Release() = 0;

	virtual int GetMovieWidth( void ) = 0;
	virtual int GetMovieHeight( void ) = 0;

	virtual int GetTextureWidth( void ) = 0;
	virtual int GetTextureHeight( void ) = 0;

	virtual IDirect3DTexture9 * GetDrawTexture( void ) = 0;
};

extern class MikanMoviePlayer * CreateMikanMovie( class DirectXDraw9 *mikandraw );
extern int DeleteMikanMovie( class MikanMoviePlayer *mikanmovie );

class MikanMoviePlayer
{
public:
	// ! �e�N�X�`�����擾����(Lock���Ȃ��Ɖ����N���邩�s���B2���̃e�N�X�`���Ń_�u���o�b�t�@�����O���Ă�\��������)�B
	virtual struct IDirect3DTexture9 * GetTexture( void ) = 0;
	// ! �e�N�X�`�����������b�N����B
	virtual int Lock( void ) = 0;
	// �e�N�X�`���������A�����b�N����B
	virtual int Unlock( void ) = 0;

	// ! ������J���B
	virtual int Open( char *moviefile ) = 0;
	// ! ����̍Đ����J�n����B
	virtual int Play( int loopplay = 0 ) = 0;
	// ! ����̍Đ����~�߂�B
	virtual int Stop( void ) = 0;
	// ! ����𒆒f����B
	virtual int Pause( void ) = 0;
	// ! ���悪�Đ����Ă��邩���ׂ�B
	virtual int IsPlay( int getstate = 0 ) = 0;
	// ! ����̍Ō�̏ꏊ���擾����B
	virtual long Size( void ) = 0;
	// ! ����̌��݂̍Đ��ꏊ���擾����B
	virtual long Tell( void ) = 0;
	// ! ����̍Đ��ʒu���V�[�N����B
	virtual int Seek( long seek ) = 0;
	// ! ����̍Đ����x�̎擾�B�ʏ��1.0;
	virtual double GetRate( void ) = 0;
	// ! ����̍Đ����x�̐ݒ�B�ʏ��1.0�B
	virtual int SetRate( double rate ) = 0;

	// ! ����̉������擾����B
	virtual int GetWidth( void ) = 0;
	// ! ����̍������擾����B
	virtual int GetHeight( void ) = 0;

	// ! ����̐F���������ɖ߂��B
	virtual int SetColor( void ) = 0;
	// ! ����S�̂̐F���������߂�B
	virtual int SetColor( unsigned long color ) = 0;
	// ! ����̊e���_�ɐF��ݒ肷��B
	virtual int SetColor( unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 ) = 0;
	// ! ����̃A���t�@�l�̂ݕύX����B
	virtual int SetAlpha( unsigned char alpha ) = 0;

	// ! ��������T�C�Y�Ŏw��ʒu�ɕ`�悷��B
	virtual int DrawMovie( int dx = 0, int dy = 0, int drawflag = 0 ) = 0;
	// ! �����؂����ĕ`�悷��B
	virtual int DrawMovie( int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 ) = 0;
	// ! �����؂����Ē����`�悷��B
	virtual int DrawMovieC( int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 ) = 0;

	// ! �����؂����Ċg��k���`�悷��B
	virtual int DrawMovieScaling( int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 ) = 0;
	// ! �����؂����Ċg��k���`�悷��B
	virtual int DrawMovieScaling( int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 ) = 0;
	// ! �����؂����Ē����Ɋg��k���`�悷��B
	virtual int DrawMovieScalingC( int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 ) = 0;
	// ! �����؂����Ē����Ɋg��k���`�悷��B
	virtual int DrawMovieScalingC( int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 ) = 0;

	// ! �����؂����Ē�������ɉ�]����(���W�A���p)�B
	virtual int DrawMovieRotationC( int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 ) = 0;
	// ! �����؂����Ē�������ɉ�]����(�p�x)�B
	virtual int DrawMovieRotationAngleC( int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 ) = 0;

	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag = 0 ) = 0;
	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag = 0 ) = 0;

	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(���W�A���p)�B
	virtual int DrawMovieRotateScaleC( int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 ) = 0;
	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(�p�x)�B
	virtual int DrawMovieRotateScaleAngleC( int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 ) = 0;
	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(���W�A���p)�B
	virtual int DrawMovieRotateScaleC( int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag = 0 ) = 0;
	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(�p�x)�B
	virtual int DrawMovieRotateScaleAngleC( int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag = 0 ) = 0;

	// ! �����؂�����4�_�w�肵�ĕ`�悷��B
	virtual int DrawMovieVertex( int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 ) = 0;

};

class MikanMovieManagementInterface
{
public:
	virtual ~MikanMovieManagementInterface( void ){}
	// ! �e�N�X�`�����擾����(Lock���Ȃ��Ɖ����N���邩�s���B2���̃e�N�X�`���Ń_�u���o�b�t�@�����O���Ă�\��������)�B
	virtual struct IDirect3DTexture9 * GetTexture( unsigned int movnum ) = 0;
	// ! �e�N�X�`�����������b�N����B
	virtual int Lock( unsigned int movnum ) = 0;
	// �e�N�X�`���������A�����b�N����B
	virtual int Unlock( unsigned int movnum ) = 0;

	// ! �v���C����̕��A��ƁB
	virtual int Reset( void ) = 0;

	// ! �E�B���h�E���b�Z�[�W�̏����B
	virtual int WinProcMovieFunc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;

	// ! ������J���B
	virtual int Load( unsigned int movnum, char *moviefile ) = 0;
	// ! ����̍Đ����J�n����B
	virtual int Play( unsigned int movnum, int loopplay = 0 ) = 0;
	// ! ����̍Đ����~�߂�B
	virtual int Stop( unsigned int movnum ) = 0;
	// ! ����𒆒f����B
	virtual int Pause( unsigned int movnum ) = 0;
	// ! ���悪�Đ����Ă��邩���ׂ�B
	virtual int IsPlay( unsigned int movnum ) = 0;
	// ! ����̍Ō�̏ꏊ���擾����B
	virtual long Size( unsigned int movnum ) = 0;
	// ! ����̌��݂̍Đ��ꏊ���擾����B
	virtual long Tell( unsigned int movnum ) = 0;
	// ! ����̍Đ��ʒu���V�[�N����B
	virtual int Seek( unsigned int movnum, long seek ) = 0;
	// ! ����̍Đ����x�̎擾�B�ʏ��1.0;
	virtual double GetRate( unsigned int movnum ) = 0;
	// ! ����̍Đ����x�̐ݒ�B�ʏ��1.0�B
	virtual int SetRate( unsigned int movnum, double rate ) = 0;
	// ! ����̉������擾����B
	virtual int GetWidth( unsigned int movnum ) = 0;
	// ! ����̍������擾����B
	virtual int GetHeight( unsigned int movnum ) = 0;

	// ! ����̐F���������ɖ߂��B
	virtual int SetColor( unsigned int movnum ) = 0;
	// ! ����S�̂̐F���������߂�B
	virtual int SetColor( unsigned int movnum, unsigned long color ) = 0;
	// ! ����̊e���_�ɐF��ݒ肷��B
	virtual int SetColor( unsigned int movnum, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 ) = 0;
	// ! ����̃A���t�@�l�̂ݕύX����B
	virtual int SetAlpha( unsigned int movnum, unsigned char alpha ) = 0;

	// ! ��������T�C�Y�Ŏw��ʒu�ɕ`�悷��B
	virtual int DrawMovie( unsigned int movnum, int dx = 0, int dy = 0, int drawflag = 0 ) = 0;
	// ! �����؂����ĕ`�悷��B
	virtual int DrawMovie( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 ) = 0;
	// ! �����؂����Ē����`�悷��B
	virtual int DrawMovieC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 ) = 0;

	// ! �����؂����Ċg��k���`�悷��B
	virtual int DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 ) = 0;
	// ! �����؂����Ċg��k���`�悷��B
	virtual int DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 ) = 0;
	// ! �����؂����Ē����Ɋg��k���`�悷��B
	virtual int DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 ) = 0;
	// ! �����؂����Ē����Ɋg��k���`�悷��B
	virtual int DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 ) = 0;

	// ! �����؂����Ē�������ɉ�]����(���W�A���p)�B
	virtual int DrawMovieRotationC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 ) = 0;
	// ! �����؂����Ē�������ɉ�]����(�p�x)�B
	virtual int DrawMovieRotationAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 ) = 0;

	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag = 0 ) = 0;
	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag = 0 ) = 0;

	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(���W�A���p)�B
	virtual int DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 ) = 0;
	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(�p�x)�B
	virtual int DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 ) = 0;
	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(���W�A���p)�B
	virtual int DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag = 0 ) = 0;
	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(�p�x)�B
	virtual int DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag = 0 ) = 0;

	// ! �����؂�����4�_�w�肵�ĕ`�悷��B
	virtual int DrawMovieVertex( unsigned int movnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 ) = 0;

};

#endif
