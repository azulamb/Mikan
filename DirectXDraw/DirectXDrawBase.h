/*
  UV展開や描画座標の計算を行うテクスチャ描画の基本クラス。
  */

#ifndef CLASSDEF_DIRECTX_DRAW_BASE
#define CLASSDEF_DIRECTX_DRAW_BASE

#define USERHW

//描画フラグ
enum
{
	DRAW_LR = 0x1, //左右反転
	DRAW_UD = 0x2  //上下反転
};

/*!
@struct  CUSTOMVERTEX
@brief   頂点の構造体。
@author  Hiroki
@version 1.0
*/
struct CUSTOMVERTEX
{
	float         x, y, z; // 頂点座標
#ifdef USERHW
	float         rhw;     // 除算数
#endif
	unsigned long color;   // 頂点の色
	float         u, v;    // テクスチャ座標
};

class DirectXDrawBase
{
public:
	struct CUSTOMVERTEX _cv[ 4 ];
	// テクスチャ情報を登録する。
	virtual int _SetUV( float width, float height, int rx, int ry, int w, int h, int drawflag );
	// 頂点色。
	virtual int _SetColor( unsigned long *colors );
	virtual int _SetColor( unsigned long lt_color, unsigned long rt_color, unsigned long ld_color, unsigned long rd_color );

	// テクスチャを描画する
	virtual int _DrawTexture( float dx, float dy, float w, float h );
	// テクスチャを描画する
	virtual int _DrawTextureC( float dx, float dy, float w, float h );

	// テクスチャを拡大縮小描画する
	virtual int _DrawTextureScaling( float dx, float dy, float dw, float dh );
	// テクスチャを拡大縮小描画する
	virtual int _DrawTextureScalingC( float dx, float dy, float dw, float dh );

	// テクスチャを回転描画する
	virtual int _DrawTextureRotationC( float dx, float dy, float dw, float dh, double rad );

	// テクスチャを回転拡大描画する
	virtual int _DrawTextureRotateScale( float cx, float cy, float dx, float dy, double rad, float dw, float dh );

	// テクスチャの4点を自由に決めて描画する
	virtual int _DrawTextureVertex( float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3 );

};

#endif
