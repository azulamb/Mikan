#ifndef CLASDEF_MIKANMOVIEMANAGEMENT
#define CLASDEF_MIKANMOVIEMANAGEMENT
#include "MikanMovie.h"

class MikanMovieManagement_ : public MikanMovieManagementInterface
{
protected:
	unsigned int moviemax;
	class MikanMoviePlayer **movie;
	class DirectXDraw9 *mikandraw;
	int playnow;
public:
	MikanMovieManagement_( unsigned int max = 1 );
	virtual ~MikanMovieManagement_( void );

	virtual int SecondInit( class DirectXDraw9 *mikandraw );

	// ! �e�N�X�`�����擾����(Lock���Ȃ��Ɖ����N���邩�s���B2���̃e�N�X�`���Ń_�u���o�b�t�@�����O���Ă�\��������)�B
	virtual struct IDirect3DTexture9 * GetTexture( unsigned int movnum );
	// ! �e�N�X�`�����������b�N����B
	virtual int Lock( unsigned int movnum );
	// ! �e�N�X�`���������A�����b�N����B
	virtual int Unlock( unsigned int movnum );

	// ! �v���C����̕��A��ƁB
	virtual int Reset( void );

	// ! �E�B���h�E���b�Z�[�W�̏����B
	virtual int WinProcMovieFunc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// ! ������J���B
	virtual int Load( unsigned int movnum, char *moviefile );
	// ! ����̍Đ����J�n����B
	virtual int Play( unsigned int movnum, int loopplay = false );
	// ! ����̍Đ����~�߂�B
	virtual int Stop( unsigned int movnum );
	// ! ����𒆒f����B
	virtual int Pause( unsigned int movnum );
	// ! ���悪�Đ����Ă��邩���ׂ�B
	virtual int IsPlay( unsigned int movnum );
	// ! ����̍Ō�̏ꏊ���擾����B
	virtual long Size( unsigned int movnum );
	// ! ����̌��݂̍Đ��ꏊ���擾����B
	virtual long Tell( unsigned int movnum );
	// ! ����̍Đ��ʒu���ړ�����(�~���b)�B
	virtual int Seek( unsigned int movnum, long seek );
	// ! ����̍Đ����x�̎擾�B�ʏ��1.0;
	virtual double GetRate( unsigned int movnum );
	// ! ����̍Đ����x�̐ݒ�B�ʏ��1.0�B
	virtual int SetRate( unsigned int movnum, double rate );

	// ! ����̉������擾����B
	virtual int GetWidth( unsigned int movnum );
	// ! ����̍������擾����B
	virtual int GetHeight( unsigned int movnum );

	// ! ����̐F���������ɖ߂��B
	virtual int SetColor( unsigned int movnum );
	// ! ����S�̂̐F���������߂�B
	virtual int SetColor( unsigned int movnum, unsigned long color );
	// ! ����̊e���_�ɐF��ݒ肷��B
	virtual int SetColor( unsigned int movnum, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 );
	// ! ����̃A���t�@�l�̂ݕύX����B
	virtual int SetAlpha( unsigned int movnum, unsigned char alpha );

	// ! ��������T�C�Y�Ŏw��ʒu�ɕ`�悷��B
	virtual int DrawMovie( unsigned int movnum, int dx = 0, int dy = 0, int drawflag = 0 );
	// ! �����؂����ĕ`�悷��B
	virtual int DrawMovie( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	// ! �����؂����Ē����`�悷��B
	virtual int DrawMovieC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );

	// ! �����؂����Ċg��k���`�悷��B
	virtual int DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// ! �����؂����Ċg��k���`�悷��B
	virtual int DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	// ! �����؂����Ē����Ɋg��k���`�悷��B
	virtual int DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// ! �����؂����Ē����Ɋg��k���`�悷��B
	virtual int DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );

	// ! �����؂����Ē�������ɉ�]����(���W�A���p)�B
	virtual int DrawMovieRotationC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 );
	// ! �����؂����Ē�������ɉ�]����(�p�x)�B
	virtual int DrawMovieRotationAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 );

	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag = 0 ) = 0;
	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag = 0 ) = 0;

	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(���W�A���p)�B
	virtual int DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 );
	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(�p�x)�B
	virtual int DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 );
	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(���W�A���p)�B
	virtual int DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag = 0 );
	// ! �����؂����Ē����ɉ�]�g��k���`�悷��(�p�x)�B
	virtual int DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag = 0 );

	// ! �����؂�����4�_�w�肵�ĕ`�悷��B
	virtual int DrawMovieVertex( unsigned int movnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 );

};

#endif
