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
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"Strmiids.lib") 
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
	// ! テクスチャを取得する(Lockしないと何が起こるか不明。2枚のテクスチャでダブルバッファリングしてる可能性が高い)。
	virtual struct IDirect3DTexture9 * GetTexture( void ) = 0;
	// ! テクスチャ処理をロックする。
	virtual int Lock( void ) = 0;
	// テクスチャ処理をアンロックする。
	virtual int Unlock( void ) = 0;

	// ! 動画を開く。
	virtual int Open( char *moviefile ) = 0;
	// ! 動画の再生を開始する。
	virtual int Play( int loopplay = 0 ) = 0;
	// ! 動画の再生を止める。
	virtual int Stop( void ) = 0;
	// ! 動画を中断する。
	virtual int Pause( void ) = 0;
	// ! 動画が再生しているか調べる。
	virtual int IsPlay( int getstate = 0 ) = 0;
	// ! 動画の最後の場所を取得する。
	virtual long Size( void ) = 0;
	// ! 動画の現在の再生場所を取得する。
	virtual long Tell( void ) = 0;
	// ! 動画の再生位置をシークする。
	virtual int Seek( long seek ) = 0;
	// ! 動画の再生速度の取得。通常は1.0;
	virtual double GetRate( void ) = 0;
	// ! 動画の再生速度の設定。通常は1.0。
	virtual int SetRate( double rate ) = 0;

	// ! 動画の横幅を取得する。
	virtual int GetWidth( void ) = 0;
	// ! 動画の高さを取得する。
	virtual int GetHeight( void ) = 0;

	// ! 動画の色合いを元に戻す。
	virtual int SetColor( void ) = 0;
	// ! 動画全体の色合いを決める。
	virtual int SetColor( unsigned long color ) = 0;
	// ! 動画の各頂点に色を設定する。
	virtual int SetColor( unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 ) = 0;
	// ! 動画のアルファ値のみ変更する。
	virtual int SetAlpha( unsigned char alpha ) = 0;

	// ! 動画を元サイズで指定位置に描画する。
	virtual int DrawMovie( int dx = 0, int dy = 0, int drawflag = 0 ) = 0;
	// ! 動画を切り取って描画する。
	virtual int DrawMovie( int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央描画する。
	virtual int DrawMovieC( int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 ) = 0;

	// ! 動画を切り取って拡大縮小描画する。
	virtual int DrawMovieScaling( int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 ) = 0;
	// ! 動画を切り取って拡大縮小描画する。
	virtual int DrawMovieScaling( int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央に拡大縮小描画する。
	virtual int DrawMovieScalingC( int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央に拡大縮小描画する。
	virtual int DrawMovieScalingC( int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 ) = 0;

	// ! 動画を切り取って中央を基準に回転する(ラジアン角)。
	virtual int DrawMovieRotationC( int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央を基準に回転する(角度)。
	virtual int DrawMovieRotationAngleC( int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 ) = 0;

	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag = 0 ) = 0;
	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag = 0 ) = 0;

	// ! 動画を切り取って中央に回転拡大縮小描画する(ラジアン角)。
	virtual int DrawMovieRotateScaleC( int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央に回転拡大縮小描画する(角度)。
	virtual int DrawMovieRotateScaleAngleC( int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央に回転拡大縮小描画する(ラジアン角)。
	virtual int DrawMovieRotateScaleC( int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央に回転拡大縮小描画する(角度)。
	virtual int DrawMovieRotateScaleAngleC( int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag = 0 ) = 0;

	// ! 動画を切り取って4点指定して描画する。
	virtual int DrawMovieVertex( int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 ) = 0;

};

class MikanMovieManagementInterface
{
public:
	virtual ~MikanMovieManagementInterface( void ){}
	// ! テクスチャを取得する(Lockしないと何が起こるか不明。2枚のテクスチャでダブルバッファリングしてる可能性が高い)。
	virtual struct IDirect3DTexture9 * GetTexture( unsigned int movnum ) = 0;
	// ! テクスチャ処理をロックする。
	virtual int Lock( unsigned int movnum ) = 0;
	// テクスチャ処理をアンロックする。
	virtual int Unlock( unsigned int movnum ) = 0;

	// ! プレイからの復帰作業。
	virtual int Reset( void ) = 0;

	// ! ウィンドウメッセージの処理。
	virtual int WinProcMovieFunc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;

	// ! 動画を開く。
	virtual int Load( unsigned int movnum, char *moviefile ) = 0;
	// ! 動画の再生を開始する。
	virtual int Play( unsigned int movnum, int loopplay = 0 ) = 0;
	// ! 動画の再生を止める。
	virtual int Stop( unsigned int movnum ) = 0;
	// ! 動画を中断する。
	virtual int Pause( unsigned int movnum ) = 0;
	// ! 動画が再生しているか調べる。
	virtual int IsPlay( unsigned int movnum ) = 0;
	// ! 動画の最後の場所を取得する。
	virtual long Size( unsigned int movnum ) = 0;
	// ! 動画の現在の再生場所を取得する。
	virtual long Tell( unsigned int movnum ) = 0;
	// ! 動画の再生位置をシークする。
	virtual int Seek( unsigned int movnum, long seek ) = 0;
	// ! 動画の再生速度の取得。通常は1.0;
	virtual double GetRate( unsigned int movnum ) = 0;
	// ! 動画の再生速度の設定。通常は1.0。
	virtual int SetRate( unsigned int movnum, double rate ) = 0;
	// ! 動画の横幅を取得する。
	virtual int GetWidth( unsigned int movnum ) = 0;
	// ! 動画の高さを取得する。
	virtual int GetHeight( unsigned int movnum ) = 0;

	// ! 動画の色合いを元に戻す。
	virtual int SetColor( unsigned int movnum ) = 0;
	// ! 動画全体の色合いを決める。
	virtual int SetColor( unsigned int movnum, unsigned long color ) = 0;
	// ! 動画の各頂点に色を設定する。
	virtual int SetColor( unsigned int movnum, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 ) = 0;
	// ! 動画のアルファ値のみ変更する。
	virtual int SetAlpha( unsigned int movnum, unsigned char alpha ) = 0;

	// ! 動画を元サイズで指定位置に描画する。
	virtual int DrawMovie( unsigned int movnum, int dx = 0, int dy = 0, int drawflag = 0 ) = 0;
	// ! 動画を切り取って描画する。
	virtual int DrawMovie( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央描画する。
	virtual int DrawMovieC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 ) = 0;

	// ! 動画を切り取って拡大縮小描画する。
	virtual int DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 ) = 0;
	// ! 動画を切り取って拡大縮小描画する。
	virtual int DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央に拡大縮小描画する。
	virtual int DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央に拡大縮小描画する。
	virtual int DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 ) = 0;

	// ! 動画を切り取って中央を基準に回転する(ラジアン角)。
	virtual int DrawMovieRotationC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央を基準に回転する(角度)。
	virtual int DrawMovieRotationAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 ) = 0;

	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag = 0 ) = 0;
	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag = 0 ) = 0;

	// ! 動画を切り取って中央に回転拡大縮小描画する(ラジアン角)。
	virtual int DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央に回転拡大縮小描画する(角度)。
	virtual int DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央に回転拡大縮小描画する(ラジアン角)。
	virtual int DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag = 0 ) = 0;
	// ! 動画を切り取って中央に回転拡大縮小描画する(角度)。
	virtual int DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag = 0 ) = 0;

	// ! 動画を切り取って4点指定して描画する。
	virtual int DrawMovieVertex( unsigned int movnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 ) = 0;

};

#endif
