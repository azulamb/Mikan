#ifndef CLASSDEF_DIRECTX_3D
#define CLASSDEF_DIRECTX_3D class DirectX3D

#include "DirectXDraw.h"

//�|���S���ԍ����L���ȏꍇ�ɐ^
#define CanUsePorygon(t)    ( (t) < pvmax && pv[(t)].ext )
//�|���S���ԍ����s���ȏꍇ�ɐ^
#define CannotUsePorygon(t) ( (t) >= pvmax || pv[(t)].ext == 0 )

struct CUSTOMVERTEX_3D
{
	float         x, y, z; // ���_���W
	unsigned long color;   // ���_�̐F
	float         u, v;    // �e�N�X�`�����W
};

struct POLYGON_VERTEX_3D
{
	//���݃t���O
	int ext;
	unsigned int max;
	unsigned int read;
	struct CUSTOMVERTEX_3D *vertex;
	D3DPRIMITIVETYPE filltype;
	unsigned int txnum;
	double space[ 6 ]; // (min & max) * (x, y, z)
};

class DirectX3D
{
private:
	long hret;
	int leftworld;
	CLASSDEF_DIRECTX_DRAW *directxdraw;
	D3DXVECTOR3 eye, at, up;
	D3DXMATRIX world, view, proj;
	double fovy, aspect, front, back;
	// �e�N�X�`���ő吔
	unsigned int texmax;
	// �e�N�X�`��
	struct MYTEXTURE *dxtex;
	// ���p�`�ő吔
	unsigned int pvmax;
	// ���p�`
	struct POLYGON_VERTEX_3D *pv;
public:
	DirectX3D( CLASSDEF_DIRECTX_DRAW *directxdraw );
	virtual ~DirectX3D( void );

	virtual int Init( unsigned int pmax = DX_POKYGON_MAX );

	virtual int Release( void );

	// ���p�`�̓o�^
	virtual int CreatePolygon( unsigned int pnum, unsigned int max, int filltype = P3D_TRIANGLE );

	virtual int ReleasePolygon( unsigned int pnum );

	virtual int ReleasePolygonAll( void );

	// �|���S���̕`���쐬�p�̊֐������B
	virtual int DrawPolygon( unsigned int pnum, double x, double y, double z );

	// ������W�n�̐��E�ɂ���
	virtual int SetWorldLeftHanded( void );
	// �E����W�n�̐��E�ɂ���
	virtual int SetWorldRightHanded( void );

	// ���[���h��ݒ肷��
	virtual int SetWorld( const D3DXMATRIX *world = NULL );

	// �r���[��ݒ肷��
	virtual int SetView( void );

	// �����_��ݒ肷��
	virtual int SetLookAtTarget( double x, double y, double z );
	virtual int SetLookAtTarget( const D3DXVECTOR3 *at );

	// �J�����̈ʒu��ݒ肷��
	virtual int SetEye( double x, double y, double z );
	virtual int SetEye( const D3DXVECTOR3 *eye );
	virtual int SetEyeX( double x );
	virtual int SetEyeY( double y );
	virtual int SetEyeZ( double z );

	// �J���������ݒn���瓮����
	virtual int MoveEye( double x, double y, double z );
	virtual int MoveEyeX( double x );
	virtual int MoveEyeY( double y );
	virtual int MoveEyeZ( double z );

	// �J�����̏������ݒ肷��
	virtual int SetCameraUp( double x, double y, double z );
	virtual int SetCameraUp( const D3DXVECTOR3 *up );
	virtual int SetCameraUpX( double x );
	virtual int SetCameraUpY( double y );
	virtual int SetCameraUpZ( double z );

	// ���E�Ƃ���ݒ肷��
	virtual int SetPerspectiveFieldOfView( void );
	virtual int SetPerspectiveFieldOfView( double fovy, double aspect, double front, double back );
	virtual int SetPerspectiveFieldOfView( const D3DXMATRIX *proj );
	virtual int SetPerspectiveFieldOfViewY( double fovy );
	virtual int SetPerspectiveAspect( double aspect );
	virtual int SetPerspectiveZFront( double front );
	virtual int SetPerspectiveZBack( double back );

	// ����E�E����W�n�̕������֐��̃��b�p�[�B
	virtual D3DXMATRIX *D3DXMatrixLookAt( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp );
	virtual D3DXMATRIX *D3DXMatrixOrtho( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );
	virtual D3DXMATRIX *D3DXMatrixOrthoOffCenter( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn, FLOAT zf );
	virtual D3DXMATRIX *D3DXMatrixPerspectiveFov( D3DXMATRIX *pOut, FLOAT fovY, FLOAT Aspect, FLOAT zn, FLOAT zf );
	virtual D3DXMATRIX *D3DXMatrixPerspective( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );
	virtual D3DXMATRIX *D3DXMatrixPerspectiveOffCenter( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn, FLOAT zf );

};

#endif
