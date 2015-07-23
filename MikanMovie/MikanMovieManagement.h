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

	// ! テクスチャを取得する(Lockしないと何が起こるか不明。2枚のテクスチャでダブルバッファリングしてる可能性が高い)。
	virtual struct IDirect3DTexture9 * GetTexture( unsigned int movnum );
	// ! テクスチャ処理をロックする。
	virtual int Lock( unsigned int movnum );
	// ! テクスチャ処理をアンロックする。
	virtual int Unlock( unsigned int movnum );

	// ! プレイからの復帰作業。
	virtual int Reset( void );

	// ! ウィンドウメッセージの処理。
	virtual int WinProcMovieFunc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// ! 動画を開く。
	virtual int Load( unsigned int movnum, char *moviefile );
	// ! 動画の再生を開始する。
	virtual int Play( unsigned int movnum, int loopplay = false );
	// ! 動画の再生を止める。
	virtual int Stop( unsigned int movnum );
	// ! 動画を中断する。
	virtual int Pause( unsigned int movnum );
	// ! 動画が再生しているか調べる。
	virtual int IsPlay( unsigned int movnum );
	// ! 動画の最後の場所を取得する。
	virtual long Size( unsigned int movnum );
	// ! 動画の現在の再生場所を取得する。
	virtual long Tell( unsigned int movnum );
	// ! 動画の再生位置を移動する(ミリ秒)。
	virtual int Seek( unsigned int movnum, long seek );
	// ! 動画の再生速度の取得。通常は1.0;
	virtual double GetRate( unsigned int movnum );
	// ! 動画の再生速度の設定。通常は1.0。
	virtual int SetRate( unsigned int movnum, double rate );

	// ! 動画の横幅を取得する。
	virtual int GetWidth( unsigned int movnum );
	// ! 動画の高さを取得する。
	virtual int GetHeight( unsigned int movnum );

	// ! 動画の色合いを元に戻す。
	virtual int SetColor( unsigned int movnum );
	// ! 動画全体の色合いを決める。
	virtual int SetColor( unsigned int movnum, unsigned long color );
	// ! 動画の各頂点に色を設定する。
	virtual int SetColor( unsigned int movnum, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 );
	// ! 動画のアルファ値のみ変更する。
	virtual int SetAlpha( unsigned int movnum, unsigned char alpha );

	// ! 動画を元サイズで指定位置に描画する。
	virtual int DrawMovie( unsigned int movnum, int dx = 0, int dy = 0, int drawflag = 0 );
	// ! 動画を切り取って描画する。
	virtual int DrawMovie( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	// ! 動画を切り取って中央描画する。
	virtual int DrawMovieC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );

	// ! 動画を切り取って拡大縮小描画する。
	virtual int DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// ! 動画を切り取って拡大縮小描画する。
	virtual int DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	// ! 動画を切り取って中央に拡大縮小描画する。
	virtual int DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// ! 動画を切り取って中央に拡大縮小描画する。
	virtual int DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );

	// ! 動画を切り取って中央を基準に回転する(ラジアン角)。
	virtual int DrawMovieRotationC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 );
	// ! 動画を切り取って中央を基準に回転する(角度)。
	virtual int DrawMovieRotationAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 );

	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag = 0 ) = 0;
	//virtual int DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag = 0 ) = 0;

	// ! 動画を切り取って中央に回転拡大縮小描画する(ラジアン角)。
	virtual int DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 );
	// ! 動画を切り取って中央に回転拡大縮小描画する(角度)。
	virtual int DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 );
	// ! 動画を切り取って中央に回転拡大縮小描画する(ラジアン角)。
	virtual int DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag = 0 );
	// ! 動画を切り取って中央に回転拡大縮小描画する(角度)。
	virtual int DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag = 0 );

	// ! 動画を切り取って4点指定して描画する。
	virtual int DrawMovieVertex( unsigned int movnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 );

};

#endif
