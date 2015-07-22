#ifndef CLASSDEF_DIRECTX_3D
#define CLASSDEF_DIRECTX_3D class DirectX3D

#include "DirectXDraw.h"

//ポリゴン番号が有効な場合に真
#define CanUsePorygon(t)    ( (t) < pvmax && pv[(t)].ext )
//ポリゴン番号が不正な場合に真
#define CannotUsePorygon(t) ( (t) >= pvmax || pv[(t)].ext == 0 )

struct CUSTOMVERTEX_3D
{
	float         x, y, z; // 頂点座標
	unsigned long color;   // 頂点の色
	float         u, v;    // テクスチャ座標
};

struct POLYGON_VERTEX_3D
{
	//存在フラグ
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
	// テクスチャ最大数
	unsigned int texmax;
	// テクスチャ
	struct MYTEXTURE *dxtex;
	// 多角形最大数
	unsigned int pvmax;
	// 多角形
	struct POLYGON_VERTEX_3D *pv;
public:
	DirectX3D( CLASSDEF_DIRECTX_DRAW *directxdraw );
	virtual ~DirectX3D( void );

	virtual int Init( unsigned int pmax = DX_POKYGON_MAX );

	virtual int Release( void );

	// 多角形の登録
	virtual int CreatePolygon( unsigned int pnum, unsigned int max, int filltype = P3D_TRIANGLE );

	virtual int ReleasePolygon( unsigned int pnum );

	virtual int ReleasePolygonAll( void );

	// ポリゴンの描画や作成用の関数を作る。
	virtual int DrawPolygon( unsigned int pnum, double x, double y, double z );

	// 左手座標系の世界にする
	virtual int SetWorldLeftHanded( void );
	// 右手座標系の世界にする
	virtual int SetWorldRightHanded( void );

	// ワールドを設定する
	virtual int SetWorld( const D3DXMATRIX *world = NULL );

	// ビューを設定する
	virtual int SetView( void );

	// 注視点を設定する
	virtual int SetLookAtTarget( double x, double y, double z );
	virtual int SetLookAtTarget( const D3DXVECTOR3 *at );

	// カメラの位置を設定する
	virtual int SetEye( double x, double y, double z );
	virtual int SetEye( const D3DXVECTOR3 *eye );
	virtual int SetEyeX( double x );
	virtual int SetEyeY( double y );
	virtual int SetEyeZ( double z );

	// カメラを現在地から動かす
	virtual int MoveEye( double x, double y, double z );
	virtual int MoveEyeX( double x );
	virtual int MoveEyeY( double y );
	virtual int MoveEyeZ( double z );

	// カメラの上方向を設定する
	virtual int SetCameraUp( double x, double y, double z );
	virtual int SetCameraUp( const D3DXVECTOR3 *up );
	virtual int SetCameraUpX( double x );
	virtual int SetCameraUpY( double y );
	virtual int SetCameraUpZ( double z );

	// 視界とかを設定する
	virtual int SetPerspectiveFieldOfView( void );
	virtual int SetPerspectiveFieldOfView( double fovy, double aspect, double front, double back );
	virtual int SetPerspectiveFieldOfView( const D3DXMATRIX *proj );
	virtual int SetPerspectiveFieldOfViewY( double fovy );
	virtual int SetPerspectiveAspect( double aspect );
	virtual int SetPerspectiveZFront( double front );
	virtual int SetPerspectiveZBack( double back );

	// 左手・右手座標系の分かれる関数のラッパー。
	virtual D3DXMATRIX *D3DXMatrixLookAt( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp );
	virtual D3DXMATRIX *D3DXMatrixOrtho( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );
	virtual D3DXMATRIX *D3DXMatrixOrthoOffCenter( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn, FLOAT zf );
	virtual D3DXMATRIX *D3DXMatrixPerspectiveFov( D3DXMATRIX *pOut, FLOAT fovY, FLOAT Aspect, FLOAT zn, FLOAT zf );
	virtual D3DXMATRIX *D3DXMatrixPerspective( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );
	virtual D3DXMATRIX *D3DXMatrixPerspectiveOffCenter( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn, FLOAT zf );

};

#endif
