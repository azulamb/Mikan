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

//WinAPIのCreateFontはCreateFontAかCreateFontWに書き換えるマクロとして定義されている。
//無効化しないと、MikanのCreateFontにAかWがついてしまう。
#undef CreateFont

#define D3D_DEBUG_INFO
#define MIKAN_PI            3.14159265358979323846264338327950288
#define AngleToRadian(a)    ( MIKAN_PI * 2 * (a) / 360 )
#define DX_RELEASE(r)       { if( r ){ ( r )->Release(); } r = NULL; }
#define DX_FREE(f)          { if( f ){ free( f ); } f = NULL; }
#define COLOR_RGB(r,g,b)    ( D3DCOLOR_XRGB( r, g, b ) )
#define COLOR_ARGB(a,r,g,b) ( D3DCOLOR_ARGB( a, r, g, b ) )
#define COLOR_RGBA(r,g,b,a) ( D3DCOLOR_RGBA( r, g, b, a ) )
//テクスチャ番号が有効な場合に真
#define CanUseTexture(t)    ( (t) < texmax && dxtex[(t)].ext )
//テクスチャ番号が不正な場合に真
#define CannotUseTexture(t) ( (t) >= texmax || dxtex[(t)].ext == 0 )
//テクスチャ数の初期値
#define DX_TEXTURE_MAX      256
//ポリゴン数の初期値
#define DX_POKYGON_MAX      (1024-1)
//フォント数の初期値
#define DX_FONT_MAX         256
//バッファ初期値
#define MAX_BUFFER_SIZE     1024

//テクスチャ生成時の透過色の指定
enum
{
	TRC_NONE = 0x00000000, //透過色なし
	TRC_ZERO = 0x00FFFFFF  //左上の色を透過色にする
};

//レンダリングフラグ
enum
{
	DRAW_TRC,       //透過色を有効にして上書き描画
	DRAW_ADD,       //透過色を有効にして加算描画
	DRAW_SUB,       //透過色を有効にして減算描画
	DRAW_MUL,       //透過色を無効にして乗算描画
	DRAW_MUL_WHITE, // 白と合成して乗算描画
	DRAW_FILL,      // 透過色を有効にして塗りつぶし。
	DRAW_HALFADD,   // 半加算合成
	DRAW_CADD_ASUB, // 透過色は減算、それ以外は加算
	DRAW_BLUR, // ブラー？
	DRAW_CUT, // カット用
	DRAW_NON, //透過色を無効にして上書き描画
};

// 頂点編集フラグ
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
// ファイルフラグ
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
@brief   テクスチャ関連のデータを格納した構造体。
@author  Hiroki
@version 1.0
*/
struct MYTEXTURE
{
	//テクスチャ
	IDirect3DTexture9 *tex;
	//サーフェイス
	IDirect3DSurface9 *sur[ 2 ];
	//ステンシルサーフェイス
	IDirect3DSurface9 *zsur;
	//存在フラグ
	int ext;
	// ロックフラグ
	int lock;
	//ロックする矩形領域
	D3DLOCKED_RECT lockrect;
	// カットする領域
	RECT cutrect;
	//リソースフラグ
	int resource;
	//ファイル名
	char *filename;
	//モジュール
	HMODULE module;
	//サイズ
	unsigned int width;
	unsigned int height;
	//その他テクスチャ生成時に指定できる値
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
	//生の色データ
	IDirect3DSurface9 *backup;//unsigned long *rawdata;
	//書き込みフラグ
	int writed;
};

/*!
@struct  THREADIN_TEX
@brief   テクスチャを生成するスレッドに渡す構造体。
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
	float         x, y, z; // 頂点座標
#ifdef USERHW
	float         rhw;     // 除算数
#endif
	unsigned long color;   // 頂点の色
	float         u, v;    // テクスチャ座標
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
//#define DEFAULT_FONT "ＭＳ ゴシック"
#define DEFAULT_FONT "MS ゴシック"

/*!
@struct  FONTDATA
@brief   フォント情報を格納した構造体。
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
	// テクスチャ最大数
	unsigned int texmax;
	// テクスチャ
	struct MYTEXTURE *dxtex;
	// 多角形最大数
	unsigned int pvmax;
	// 最後に使った多角形番号
	unsigned int lastpvnum;
	// 多角形
	struct POLYGON_VERTEX *pv;
	struct CUSTOMVERTEX *rpv;
	unsigned int rpvmax;
	// ライン
	struct D3DXVECTOR2 linevec[ 2 ];
	struct ID3DXLine *line;
	// フォント最大数
	unsigned int fontmax;
	// フォント
	struct FONTDATA *fontdata;
	// 画面使用フラグ,テクスチャ番号
	unsigned int target[ 2 ];
	// 垂直同期フラグ
	unsigned int vsync;
	// テクスチャ書き込みフラグ
	unsigned int candrawtexture;
	// サーフェイス書き込みフラグ
	unsigned int candrawsurface;
	int countcs;
	// テクスチャバックアップを行うかどうかの設定
	int texturebackup;
	unsigned int swapchainmax;
	// 描画サイズ
	int swidth, sheight;
	// 描画中かどうか。
	int drawing, drawing_;
	// /BeginEndSceneのロック。
	int lock;
	unsigned long locksleeptime;

	// マルチスレッドかどうか。
	int ismultithread;

	int fullscreen;
	int ( DirectXDraw9::*drawfunc )( int num );
public:
	// Microsoftが用意した関数エラーの返り値
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

	// コンストラクタ
	DirectXDraw9( void );
	// コンストラクタ
	DirectXDraw9( HWND wh, unsigned int tmax = DX_TEXTURE_MAX, unsigned int pmax = DX_POKYGON_MAX, unsigned int fmax = DX_FONT_MAX );
	// コンストラクタ本体
	virtual int Init( HWND wh = NULL, unsigned int tmax = DX_TEXTURE_MAX, unsigned int pmax = DX_POKYGON_MAX, unsigned int fmax = DX_FONT_MAX );
	// デストラクタ
	virtual ~DirectXDraw9( void );
	// ウィンドウハンドルの設定
	virtual HWND SetWindowHandle( HWND wh );
	// ウィンドウハンドルの設定
	virtual HWND GetWindowHandle( void );
	virtual int SetFullScreenMode( int fullscreen_on = 1 );
	// 垂直同期待ちの設定
	virtual int SetVsyncWait( int vs = 0 );
	// テクスチャの最大数の設定
	virtual unsigned int SetTextureMax( unsigned int tmax = DX_TEXTURE_MAX );
	virtual unsigned int GetTextureMax( void );
	virtual int IsEmptyTexture( unsigned int txnum );
	// 多角形頂点の最大数の設定
	virtual unsigned int SetPolygonMax( unsigned int pmax = DX_POKYGON_MAX );
	// 描画サイズの設定
	virtual int SetScreenSize( int width, int height );
	// 描画サイズの取得
	virtual int GetScreenWidth( int now = 0 );
	virtual int GetScreenHeight( int now = 0 );
	// DirectXの初期化
	virtual int InitD3D( int create = 1 );
	// DirectXの解放
	virtual int CleanupD3D( void );
	// DirectXのオブジェクトの初期化
	virtual int InitD3DObject( void );
	// DirectXのオブジェクトの解放
	virtual int CleanupD3DObject( void );
	// テクスチャのバックアップを自動で行うかどうかの設定
	virtual int SetAutoBackUpTexture( int enable = 1 );
	// Texture(Surface)のバックアップ
	virtual int BackUpTexture( unsigned int txnum, int force = 0 );

	// バックアップ
	virtual int BackUp( void );
	// 回復作業
	virtual int Recover( void );
	// デバイスロストからの復帰
	virtual int RecoverFromDeviceLost( int force = 0 );

	virtual int SetDrawFunction( int mode = DRAW_SWAPCHAIN );
	virtual int Draw( int num );
	virtual int DrawNull( int num )
	{
		return num;
	}

	// 画面を綺麗にする
	//  virtual int ClearScreen( void );
	// 画面を綺麗にする
	virtual int ClearScreen( unsigned long color = 0xFF000000 );
	virtual int CountClearScreen( void );

	// テクスチャの準備
	virtual int InitRender( void );
	// テクスチャの合成方法の指定
	virtual int SetRender( int kind = DRAW_TRC );
	// 画面をレンダーターゲットにする。
	virtual int SetRenderTarget( void );
	// テクスチャをレンダーターゲットにする。
	virtual int SetRenderTarget( unsigned int txnum );
	// レンダーターゲットを元に戻す(直前に画面だったら画面、テクスチャだったらテクスチャ)。
	virtual int ResetRenderTarget( void );
	// 画像を読み込んでテクスチャを作る
	virtual int CreateTexture( unsigned int txnum, const char *filename, unsigned long tcolor = 0x00000000, int thread = 0 );
	// リソースを読み込んでテクスチャを作る
	virtual int CreateTexture( unsigned int txnum, HMODULE module, const char *resource, unsigned long tcolor = 0x00000000, int thread = 0 );
	// 空のテクスチャを作る
	virtual int CreateTexture( unsigned int txnum, unsigned int width, unsigned int height, int thread = 0 );
	// 裏でテクスチャを作る
	static unsigned long WINAPI ThreadCreateTexture( void * threadin );
	// 裏でリソースからテクスチャを作る
	static unsigned long WINAPI ThreadCreateTextureFromResorce( void * threadin );
	// 裏で空のテクスチャを作る
	static unsigned long WINAPI ThreadCreateTextureEmpty( void * threadin );
	// テクスチャの解放
	virtual int ReleaseTexture( unsigned int txnum );
	// 全テクスチャの解放。
	virtual int ReleaseTexture();
	// 多角形の解放
	virtual int ReleasePolygon( unsigned int pvnum );

	// 点を描画する。
	virtual int DrawPixel( int x, int y, unsigned long color = 0xffffffff );

	// 直線を描画する。
	virtual int DrawLine( int sx, int sy, int ex, int ey, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );
	virtual int DrawBoxLine( int dx, int dy, int w, int h, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );
	virtual int DrawBoxLineC( int dx, int dy, int w, int h, unsigned long color = 0xffffffff, double width = 1.0, int antialias = 0 );
	// 四角形を描画する
	virtual int DrawBox( int dx, int dy, int w, int h, unsigned long color );
	// 四角形を描画する
	virtual int DrawBox( int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 );
	// 四角形を描画する
	virtual int DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color );
	// 四角形を描画する
	virtual int DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 );
	// 四角形を描画する
	virtual int DrawBoxC( int dx, int dy, int w, int h, unsigned long color );
	// 四角形を描画する
	virtual int DrawBoxC( int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 );
	virtual int DrawCircleLineC( int x, int y, int r, unsigned long color = 0xFFFFFFFF, double width = 1.0, int antialias = 0 );
	// 円の描画
	virtual int DrawCircleC( int x, int y, int r, unsigned long color = 0xFFFFFFFF );
	virtual int DrawCircleLine( int x, int y, int w, int h, unsigned long color = 0xFFFFFFFF, double width = 1.0, int antialias = 0 );
	// 楕円かもしれない描画
	virtual int DrawCircle( int x, int y, int w, int h, unsigned long color = 0xFFFFFFFF );

	// テクスチャ描画の終了
	virtual int EndDrawTexture( IDirect3DTexture9 *tex );
	// テクスチャを描画する
	virtual int DrawTexture( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );
	// テクスチャを描画する
	virtual int DrawTextureC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag = 0 );

	// テクスチャを拡大縮小描画する
	virtual int DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// テクスチャを拡大縮小描画する
	virtual int DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );
	// テクスチャを拡大縮小描画する
	virtual int DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag = 0 );
	// テクスチャを拡大縮小描画する
	virtual int DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag = 0 );

	// テクスチャを回転描画する
	virtual int DrawTextureRotationC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag = 0 );
	// テクスチャを回転描画する
	virtual int DrawTextureRotationAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag = 0 );

	// テクスチャを回転拡大描画する
	virtual int DrawTextureRotateScale( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag = 0 );
	// テクスチャを回転拡大描画する
	virtual int DrawTextureRotateScale( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag = 0 );

	// テクスチャを回転拡大描画する
	virtual int DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag = 0 );
	// テクスチャを回転拡大描画する
	virtual int DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag = 0 );
	// テクスチャを回転拡大描画する
	virtual int DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag = 0 );
	// テクスチャを回転拡大描画する
	virtual int DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag = 0 );

	// テクスチャの4点を自由に決めて描画する
	virtual int DrawTextureVertex( unsigned int txnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag = 0 );
	// テクスチャの4点を相対座標で自由に決めて描画する
	virtual int DrawTextureRelativeVertex( unsigned int txnum, int rx, int ry, int w, int h, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3, int drawflag = 0 );
	// テクスチャの4点を相対座標で自由に決めて描画する(テクスチャチェックなし:内部用)
	// すべてのテクスチャ描画は最終的にこれの関節関数となっている
	virtual int DrawTextureRelativeVertex( unsigned int txnum, int rx, int ry, int w, int h, double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, int drawflag = 0 );
	// 多角形の登録(リスト)
	virtual int CreatePolygon( unsigned int pnum, D3DPRIMITIVETYPE filltype = D3DPT_TRIANGLESTRIP );
	// 多角形の登録(配列)
	virtual int CreatePolygon( unsigned int pnum, unsigned int max, D3DPRIMITIVETYPE filltype = D3DPT_TRIANGLESTRIP );
	// 正多角形の登録
	virtual int CreateRegularPolygon( unsigned int pnum, double cx, double cy, unsigned int n, double r, double rad = 0.0, unsigned long color = 0xffffffff );
	// テクスチャの登録
	virtual int SetTextureOnPolygon( unsigned int pnum, unsigned int tnum );
	// テクスチャの解除
	virtual int SetTextureOnPolygon( unsigned int pnum );
	// テクスチャ番号の取得。
	virtual unsigned int GetTextureNumOnPolygon( unsigned int pnum );
	// UV座標の登録。
	virtual int SetPolygonUV( unsigned int pnum, unsigned int vnum, int u, int v );
	//頂点の登録(配列限定)
	virtual int SetPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color );
	//頂点の登録
	virtual int SetPolygonVertex( unsigned int pnum, double x, double y, unsigned long color );
	//  virtual int SetPolygonVertex( double x, double y, unsigned long color );
	//頂点情報の取得
	virtual struct CUSTOMVERTEX * GetPolygonVertex( unsigned int pnum, unsigned int vnum );
	// 頂点要素数の取得
	virtual unsigned int GetPolygonVertexLength( unsigned int pnum );
	// 頂点情報の編集
	virtual int EditPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color = 0xffffffff, double u = 0.0, double v = 0.0, int edit = PVEDIT_DEFAULT );
	// 多角形の描画範囲を取得。xyそれぞれ。
	// 多角形の変形
	// 移動
	virtual int TransformPolygonMoving( unsigned int pnum, double mx, double my );
	// 拡大
	virtual int TransformPolygonScaling( unsigned int pnum, double scale, unsigned int basenum = 0 );
	// 多角形の描画
	//  virtual int DrawPolygonVertex( unsigned int pnum );
	//  virtual int DrawPolygonVertex( unsigned int pnum, unsigned int draw );
	//  virtual int DrawPolygonVertex( unsigned int pnum, unsigned int begin, unsigned int end );
	//  virtual int DrawPolygon( unsigned int pnum, double basex, double basey, int cx, int cy );
	virtual int DrawPolygon( unsigned int pnum, int cx, int cy, unsigned int basenum, unsigned int begin, unsigned int end );
	virtual int DrawPolygon( unsigned int pnum, int cx, int cy, unsigned int begin, unsigned int end );
	virtual int DrawPolygon( unsigned int pnum, int cx, int cy );
	virtual int DrawPolygon( unsigned int pnum );
	// 多角形の拡大描画
	/*  virtual int DrawPolygonScaling( unsigned int pnum, double basex, double basey, int cx, int cy, double scale );*/
	virtual int DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale, unsigned int basenum, unsigned int start, unsigned int end );
	virtual int DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale, unsigned int start, unsigned int end );
	virtual int DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale );
	// scale x,y,z指定可能なものとか用意。
	// 正多角形の描画
	virtual int DrawRegularPolygon( double cx, double cy, unsigned int n, int drawnum, double r, double rad = 0.0, unsigned long color = 0xffffffff );
	virtual int DrawRegularPolygon( double cx, double cy, unsigned int n, double r, double rad = 0.0, unsigned long color = 0xffffffff );

	// 色の作成
	virtual unsigned long Color( unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue );
	virtual unsigned long ColorHSV( unsigned char alpha, unsigned int hue, unsigned char sat, unsigned char vob );
	// 色の設定
	virtual int SetColor( unsigned int txnum, unsigned long color = 0xFFFFFFFF );
	// アルファ値の設定
	virtual int SetAlpha( unsigned int txnum, unsigned char alpha );
	// 頂点色の設定
	virtual int SetColorVertex( unsigned int txnum, unsigned long color );

	// テクスチャのデータを読めるようにする。
	virtual int OpenTexture( unsigned int txnum, int x = 0, int y = 0, int w = 0, int h = 0 );
	// テクスチャのデータを読める状態から戻す。
	virtual int CloseTexture( unsigned int txnum );
	// テクスチャの指定座標の色を取得する。
	virtual unsigned long GetColorTexturePixel( unsigned int txnum, int x, int y );
	// テクスチャの範囲内の色を置換する。
	virtual unsigned int TextureColorReplacement( unsigned int txnum, unsigned long scolor, unsigned long rcolor, int x = 0, int y = 0, int w = 0, int h = 0 );
	// テクスチャの範囲内の色を検索する。
	virtual unsigned int TextureColorSearch( unsigned int txnum, unsigned long scolor, int x = 0, int y = 0, int w = 0, int h = 0 );
	// テクスチャの範囲内の色を検索し数を数える。
	virtual unsigned int TextureColorCount( unsigned int txnum, unsigned long scolor, int x = 0, int y = 0, int w = 0, int h = 0 );

	// テクスチャの横幅取得
	virtual unsigned int GetTextureWidth( unsigned int txnum = 0 );
	// テクスチャの高さ取得
	virtual unsigned int GetTextureHeight( unsigned int txnum = 0 );

	// スクリーンショット
	virtual int ScreenShot( const char *filename, unsigned int imagetype = IMAGE_BMP );

	// テクスチャを画像へ出力
	virtual int TextureToImage( const char *filename, unsigned int txnum, unsigned int imagetype = IMAGE_BMP );

	// テクスチャの描画開始
	virtual int Begin( void );
	// テクスチャの描画終了
	virtual int End( void );
	// Endロック。
	virtual int LockDrawEnd( unsigned long sleeptime = 1 );
	// Endアンロック。
	virtual int UnlockDarawEnd( void );
	// ! Beginを待つ。
	/* !
	\param sleeptime Begin実行時まで待つ単位時間。
	\param autounlock 1=デッドロック解消のためにロックされている場合は自動的に解除する(デフォルト),0=解除しない
	*/
	virtual int WaitDrawBegin( unsigned long sleeptime = 1, unsigned int timeout = INFINITE, int autounlock = 1 );
	virtual int WaitDrawEnd( unsigned long sleeptime = 1, unsigned int timeout = INFINITE );

	virtual MYTEXTURE * GetTextureData( unsigned int txnum );

	// ! DirectXへのアクセス
	/* !
	\param force 1=DirectX3DDeviceをすぐに取得する[デフォルト],0=描画中に取得する
	\param sleeptime 描画中かどうか調べる間隔(ミリ秒)
	*/
	virtual IDirect3DDevice9 * GetDirect3DDevice( int force = 1, unsigned long sleeptime = 1 );
	virtual HRESULT SetRenderState( D3DRENDERSTATETYPE State, DWORD Value );
	virtual HRESULT SetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value );

	// フォント

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
