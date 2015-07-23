#ifndef CLASSDEF_MIKANMOVIEPLAYER
#define CLASSDEF_MIKANMOVIEPLAYER

#include "MikanMovie.h"

#include <dshow.h>
#include <Vmr9.h>

#include "MikanVMRSurfaceAllocator.h"



class MikanMoviePlayer_ : public MikanMoviePlayer
{
private:
	class DirectXDraw9 *mdraw;

	IGraphBuilder *builder;
	IMediaControl *control;
	IMediaSeeking *seeking;
	IMediaEventEx *eventex;

	IBaseFilter *vmrfilter;
	IBaseFilter *sourcefilter;

	MikanVMRSurfaceAllocator *SurfaceAllocator;
	unsigned long color[ 4 ];

	long long endpoint, nowpoint;

	HRESULT hr;
	long eventcode;
	WCHAR *moviefile;

	int play, loopplay;

	static class CriticalLock *lock;
public:
	MikanMoviePlayer_( void );
	virtual ~MikanMoviePlayer_( void );

	virtual int Init( class DirectXDraw9 *mikandraw );

	virtual int Event( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// ! テクスチャを取得する(Lockしないと何が起こるか不明)。
	virtual struct IDirect3DTexture9 * GetTexture( void );
	// ! テクスチャ処理をロックする。
	virtual int Lock( void );
	// テクスチャ処理をアンロックする。
	virtual int Unlock( void );

	virtual int Open( char *moviefile );
	virtual int Play( int loop = 0 );
	virtual int Stop( void );
	virtual int Pause( void );
	virtual int IsPlay( int getstate = 0 );
	virtual long Size( void );
	virtual long Tell( void );
	virtual int Seek( long seek );
	virtual double GetRate( void );
	virtual int SetRate( double rate );

	// ! 動画の横幅を取得する。
	virtual int GetWidth( void );
	// ! 動画の高さを取得する。
	virtual int GetHeight( void );

	// テクスチャ描画の終了
	virtual int EndDrawMovie( IDirect3DTexture9 *tex );

	virtual int SetColor( void );
	virtual int SetColor( unsigned long color );
	virtual int SetColor( unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 );
	virtual int SetAlpha( unsigned char alpha );

	virtual int DrawMovie( int dx = 0, int dy = 0, int drawflag = 0 );
	virtual int DrawMovie( int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	virtual int DrawMovieC( int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );

	virtual int DrawMovieScaling( int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	virtual int DrawMovieScaling( int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	virtual int DrawMovieScalingC( int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	virtual int DrawMovieScalingC( int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );

	virtual int DrawMovieRotationC( int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 );
	virtual int DrawMovieRotationAngleC( int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 );

	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag = 0 );
	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag = 0 );

	virtual int DrawMovieRotateScaleC( int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 );
	virtual int DrawMovieRotateScaleAngleC( int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 );
	virtual int DrawMovieRotateScaleC( int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag = 0 );
	virtual int DrawMovieRotateScaleAngleC( int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag = 0 );

	virtual int DrawMovieVertex( int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 );

};

#endif