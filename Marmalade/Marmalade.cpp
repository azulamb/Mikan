#include "Marmalade.h"

// コンストラクタ
Marmalade::Marmalade( CLASSDEF_DIRECTX_DRAW *directx_draw )
{
	Init( directx_draw );
}

Marmalade::Marmalade( CLASSDEF_DIRECTX_DRAW *directx_draw, int lmax )
{
	Init( directx_draw, lmax );
}

// コンストラクタ本体
int Marmalade::Init( CLASSDEF_DIRECTX_DRAW *directx_draw, int lmax )
{
	_MikanDraw = directx_draw;
	layer_max = lmax;
	layer = ( struct FUNC_DATA_HEAD * )calloc( layer_max, sizeof( struct FUNC_DATA_HEAD ) );

	tex = ( struct TEXTURE_LIST_HEAD * )calloc( 1, sizeof( struct TEXTURE_LIST_HEAD ) );
	// 1版テクスチャを開けておく
	tex->read = 1;

	return 0;
}

// デストラクタ
Marmalade::~Marmalade( void )
{
	int c;
	c = layer_max;
	while ( ( c-- ) > 0 )
	{
		ReleaseLayer( c );
	}
	free( layer );

	c = tex->read;
	while ( c > 0 )
	{
		ReleaseTexture( c-- );
	}
	free( tex );

}

union FUNC_ARG * Marmalade::AddLayer( unsigned int lnum, int ( Marmalade::*func )( union FUNC_ARG * arg ) )
{
	struct LAYER_DATA *new_layer;
	union FUNC_ARG *ret;

	if ( lnum >= layer_max )
	{
		return NULL;
	}

	if ( layer[ lnum ].now == NULL || layer[ lnum ].now->next == NULL )
	{
		// 新規作成
		new_layer = ( struct LAYER_DATA * )calloc( 1, sizeof( struct LAYER_DATA ) );
		new_layer->next = NULL;
		new_layer->arg = ( union FUNC_ARG * )calloc( 1, sizeof( union FUNC_ARG ) );

		if ( layer[ lnum ].now )
		{
			layer[ lnum ].now->next = new_layer;
			layer[ lnum ].now = layer[ lnum ].now->next;
		} else
		{
			layer[ lnum ].first = layer[ lnum ].now = new_layer;
		}
	} else
	{
		// 再利用
		new_layer = ( struct LAYER_DATA * )( layer[ lnum ].now );
		layer[ lnum ].now = layer[ lnum ].now->next;
	}

	new_layer->func = func;
	ret = new_layer->arg;

	return ret;
}

int Marmalade::DrawLayer( unsigned int lnum )
{
	struct LAYER_DATA *tmp;
	if ( lnum >= layer_max )
	{
		return 1;
	}
	tmp = layer[ lnum ].first;
	while ( tmp )
	{
		// 描画
		if ( tmp->func )
		{
			//((int            (*)( void * vparam       ))(td->thread_func        ))( td->param );
			//((int (Marmalade::*)( union FUNC_ARG *arg ))(layer[ lnum ].now->func))( layer[ lnum ].now->arg );
			( this->*( tmp->func ) )( tmp->arg );
			tmp->func = NULL;
			//free( layer[ lnum ].first );
		} else
		{
			break;
		}
		// 次へ
		tmp = tmp->next;
	}
	layer[ lnum ].now = layer[ lnum ].first;
	return 0;
}

int Marmalade::ReleaseLayer( unsigned int lnum )
{
	if ( lnum >= layer_max )
	{
		return 1;
	}
	layer[ lnum ].now = layer[ lnum ].first;
	while ( layer[ lnum ].now )
	{
		layer[ lnum ].first = layer[ lnum ].now;
		// 次へ
		layer[ lnum ].now = layer[ lnum ].now->next;
		if ( layer[ lnum ].first->func == &Marmalade::RegPrintf )
		{
			free( layer[ lnum ].first->arg->uii2cp.a3 );
		}
		free( layer[ lnum ].first->arg );
		free( layer[ lnum ].first );
	}
	return 0;
}

// 画面を綺麗にする
int Marmalade::ClearScreen( unsigned long color ){
	return _MikanDraw->ClearScreen( color );
}

int Marmalade::CountClearScreen( void ){
	return _MikanDraw->CountClearScreen();
}

// テクスチャの合成方法の指定
int Marmalade::SetRender( unsigned int lnum, int kind )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegSetRender );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i = kind;
	return 0;
}

// 画面をレンダーターゲットにする。
int Marmalade::SetRenderTarget( unsigned int lnum )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegSetRenderTarget0 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i = 0;
	return 0;
}

// テクスチャをレンダーターゲットにする。
int Marmalade::SetRenderTarget( unsigned int lnum, unsigned int txnum )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegSetRenderTarget1 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->ui = txnum;
	return 0;
}

unsigned int Marmalade::AddTexture( char *filename )
{
	unsigned int ret;
	struct TEXTURE_LIST *tmp;

	if ( filename )
	{
		// すでに作ったテクスチャを検索。
		tmp = tex->first;
		while ( tmp )
		{
			if ( tmp->file && strcmp( tmp->file, filename ) == 0 )
			{
				return tmp->tnum;
			}
			tmp = tmp->next;
		}
	}

	ret = tex->read;
	// 存在しないテクスチャ番号の検索。
	while ( _MikanDraw->IsEmptyTexture( ret ) == 1 )
	{
		++ret;
	}

	if ( _MikanDraw->IsEmptyTexture( ret ) != 0 )
	{
		return 0;
	}

	tex->read = ret;

	return AddTexture( filename, ret );
}

unsigned int Marmalade::AddTexture( char *filename, unsigned int tnum )
{
	struct TEXTURE_LIST *tmp;
	tmp = ( struct TEXTURE_LIST * )calloc( 1, sizeof( TEXTURE_LIST ) );

	// 重複するファイルも無理やり追加する。

	if ( filename )
	{
		tmp->file = (char *)calloc( strlen( filename ) + 1, sizeof( char ) );
		strcpy_s( tmp->file, strlen( filename ) + 1, filename );
	} else
	{
		tmp->file = filename;
	}
	tmp->tnum = tnum;

	if ( tex->now )
	{
		tex->now->next = tmp;
		tex->now = tmp;
	} else
	{
		tex->first = tex->now = tmp;
	}

	return tnum;
}

// テクスチャを生成しテクスチャ番号を返すCreateTexture
// 画像を読み込んでテクスチャを作る
unsigned int Marmalade::CreateTexture( char *filename, unsigned long tcolor, int thread )
{
	unsigned int tnum;

	tnum = AddTexture( filename );

	_MikanDraw->CreateTexture( tnum, filename, tcolor, thread );

	return tnum;
}

// リソースを読み込んでテクスチャを作る
unsigned int Marmalade::CreateTexture( HMODULE module, char *resource, unsigned long tcolor, int thread )
{
	unsigned int tnum;

	tnum = AddTexture( resource );

	_MikanDraw->CreateTexture( tnum, module, resource, tcolor, thread );

	return tnum;
}

// 空のテクスチャを作る
unsigned int Marmalade::CreateTexture( unsigned int width, unsigned int height, int thread )
{
	unsigned int tnum;

	tnum = AddTexture( NULL );

	_MikanDraw->CreateTexture( tnum, width, height, thread );

	return tnum;
}

// テクスチャ番号を指定するCreateTexture
// 画像を読み込んでテクスチャを作る
int Marmalade::CreateTexture( unsigned int txnum, char *filename, unsigned long tcolor, int thread )
{
	return _MikanDraw->CreateTexture( AddTexture( filename, txnum ), filename, tcolor, thread );
}

// リソースを読み込んでテクスチャを作る
int Marmalade::CreateTexture( unsigned int txnum, HMODULE module, char *resource, unsigned long tcolor, int thread )
{
	return _MikanDraw->CreateTexture( AddTexture( resource, txnum ), module, resource, tcolor, thread );
}

// 空のテクスチャを作る
int Marmalade::CreateTexture( unsigned int txnum, unsigned int width, unsigned int height, int thread )
{
	return _MikanDraw->CreateTexture( txnum, width, height, thread );
}

// テクスチャの解放
int Marmalade::ReleaseTexture( unsigned int txnum )
{
	struct TEXTURE_LIST *tmp, *prev = NULL;

	tmp = tex->first;
	while ( tmp )
	{
		if ( tmp->tnum == txnum )
		{
			// ポインタ書き換え。
			if ( prev )
			{
				prev->next = tmp->next;
			} else
			{
				tex->first = tmp->next;
				if ( tmp == tex->now )
				{
					tex->now = tex->first;
				}
			}

			break;
		}
		prev = tmp;
		tmp = tmp->next;
	}

	if ( tmp )
	{
		free( tmp->file );
		free( tmp );
	}
	_MikanDraw->ReleaseTexture( txnum );


	return 0;
}

// 多角形の解放
//int Marmalade::ReleasePolygon( unsigned int pvnum ){}

// 点を描画する。
int Marmalade::DrawPixel( unsigned int lnum, int x, int y, unsigned long color )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawPixel );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->iiul.a0 = x;
	arg->iiul.a1 = y;
	arg->iiul.a2 = color;
	return 0;
}

// 直線を描画する。
int Marmalade::DrawLine( unsigned int lnum, int sx, int sy, int ex, int ey, unsigned long color, double width, int antialias )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawLine );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i4uldi.a0 = sx;
	arg->i4uldi.a1 = sy;
	arg->i4uldi.a2 = ex;
	arg->i4uldi.a3 = ey;
	arg->i4uldi.a4 = color;
	arg->i4uldi.a5 = width;
	arg->i4uldi.a6 = antialias;
	return 0;
}

// 四角形を描画する
int Marmalade::DrawBox( unsigned int lnum, int dx, int dy, int w, int h, unsigned long color )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawBox0 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i4ul.a0 = dx;
	arg->i4ul.a1 = dy;
	arg->i4ul.a2 = w;
	arg->i4ul.a3 = h;
	arg->i4ul.a4 = color;
	return 0;
}

// 四角形を描画する
int Marmalade::DrawBox( unsigned int lnum, int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawBox1 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i4ul4.a0 = dx;
	arg->i4ul4.a1 = dy;
	arg->i4ul4.a2 = w;
	arg->i4ul4.a3 = h;
	arg->i4ul4.a4 = color1;
	arg->i4ul4.a5 = color2;
	arg->i4ul4.a6 = color3;
	arg->i4ul4.a7 = color4;
	return 0;
}

// 四角形を描画する
int Marmalade::DrawBox( unsigned int lnum, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawBox2 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i8ul.a0 = x0;
	arg->i8ul.a1 = y0;
	arg->i8ul.a2 = x1;
	arg->i8ul.a3 = y1;
	arg->i8ul.a4 = x2;
	arg->i8ul.a5 = y2;
	arg->i8ul.a6 = x3;
	arg->i8ul.a7 = y3;
	arg->i8ul.a8 = color;
	return 0;
}

// 四角形を描画する
int Marmalade::DrawBox( unsigned int lnum, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawBox3 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i8ul4.a0 = x0;
	arg->i8ul4.a1 = y0;
	arg->i8ul4.a2 = x1;
	arg->i8ul4.a3 = y1;
	arg->i8ul4.a4 = x2;
	arg->i8ul4.a5 = y2;
	arg->i8ul4.a6 = x3;
	arg->i8ul4.a7 = y3;
	arg->i8ul4.a8 = color1;
	arg->i8ul4.a9 = color2;
	arg->i8ul4.a10 = color3;
	arg->i8ul4.a11 = color4;
	return 0;
}

// 四角形を描画する
int Marmalade::DrawBoxC( unsigned int lnum, int dx, int dy, int w, int h, unsigned long color )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawBoxC0 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i4ul.a0 = dx;
	arg->i4ul.a1 = dy;
	arg->i4ul.a2 = w;
	arg->i4ul.a3 = h;
	arg->i4ul.a4 = color;
	return 0;
}

// 四角形を描画する
int Marmalade::DrawBoxC( unsigned int lnum, int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawBoxC1 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i4ul4.a0 = dx;
	arg->i4ul4.a1 = dy;
	arg->i4ul4.a2 = w;
	arg->i4ul4.a3 = h;
	arg->i4ul4.a4 = color1;
	arg->i4ul4.a5 = color2;
	arg->i4ul4.a6 = color3;
	arg->i4ul4.a7 = color4;
	return 0;
}

// 円の線描画(円のみ)。
int Marmalade::DrawCircleLineC( unsigned int lnum, int x, int y, int r, unsigned long color, double width, int antialias )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawCircleLineC );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i3uldi.a0 = x;
	arg->i3uldi.a1 = y;
	arg->i3uldi.a2 = r;
	arg->i3uldi.a3 = color;
	arg->i3uldi.a4 = width;
	arg->i3uldi.a5 = antialias;
	return 0;
}

// 円の線描画(箱内に収まる楕円)。
int Marmalade::DrawCircleLine( unsigned int lnum, int x, int y, int w, int h, unsigned long color, double width, int antialias )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawCircleLine );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i4uldi.a0 = x;
	arg->i4uldi.a1 = y;
	arg->i4uldi.a2 = w;
	arg->i4uldi.a3 = h;
	arg->i4uldi.a4 = color;
	arg->i4uldi.a5 = width;
	arg->i4uldi.a6 = antialias;
	return 0;
}

// 円描画(円のみ)。
int Marmalade::DrawCircleC( unsigned int lnum, int x, int y, int r, unsigned long color )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawCircleC );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i3ul.a0 = x;
	arg->i3ul.a1 = y;
	arg->i3ul.a2 = r;
	arg->i3ul.a3 = color;
	return 0;
}

// 円描画(箱内に収まる楕円)。
int Marmalade::DrawCircle( unsigned int lnum, int x, int y, int w, int h, unsigned long color )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawCircle );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->i4ul.a0 = x;
	arg->i4ul.a1 = y;
	arg->i4ul.a2 = w;
	arg->i4ul.a2 = h;
	arg->i4ul.a4 = color;
	return 0;
}

// テクスチャを描画する
int Marmalade::DrawTexture( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawTexture );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii7.a0 = txnum;
	arg->uii7.a1 = dx;
	arg->uii7.a2 = dy;
	arg->uii7.a3 = rx;
	arg->uii7.a4 = ry;
	arg->uii7.a5 = w;
	arg->uii7.a6 = h;
	arg->uii7.a7 = drawflag;
	return 0;
}

// テクスチャを描画する
int Marmalade::DrawTextureC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawTextureC );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii7.a0 = txnum;
	arg->uii7.a1 = dx;
	arg->uii7.a2 = dy;
	arg->uii7.a3 = rx;
	arg->uii7.a4 = ry;
	arg->uii7.a5 = w;
	arg->uii7.a6 = h;
	arg->uii7.a7 = drawflag;
	return 0;
}

// テクスチャを拡大縮小描画する
int Marmalade::DrawTextureScaling( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawTextureScaling0 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii6di.a0 = txnum;
	arg->uii6di.a1 = dx;
	arg->uii6di.a2 = dy;
	arg->uii6di.a3 = rx;
	arg->uii6di.a4 = ry;
	arg->uii6di.a5 = w;
	arg->uii6di.a6 = h;
	arg->uii6di.a7 = scale;
	arg->uii6di.a8 = drawflag;
	return 0;
}

// テクスチャを拡大縮小描画する
int Marmalade::DrawTextureScaling( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawTextureScaling1 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii9.a0 = txnum;
	arg->uii9.a1 = dx;
	arg->uii9.a2 = dy;
	arg->uii9.a3 = rx;
	arg->uii9.a4 = ry;
	arg->uii9.a5 = w;
	arg->uii9.a6 = h;
	arg->uii9.a7 = dw;
	arg->uii9.a8 = dh;
	arg->uii9.a9 = drawflag;
	return 0;
}

// テクスチャを拡大縮小描画する
int Marmalade::DrawTextureScalingC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawTextureScalingC0 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii6di.a0 = txnum;
	arg->uii6di.a1 = dx;
	arg->uii6di.a2 = dy;
	arg->uii6di.a3 = rx;
	arg->uii6di.a4 = ry;
	arg->uii6di.a5 = w;
	arg->uii6di.a6 = h;
	arg->uii6di.a7 = scale;
	arg->uii6di.a8 = drawflag;
	return 0;
}

// テクスチャを拡大縮小描画する
int Marmalade::DrawTextureScalingC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawTextureScalingC1 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii9.a0 = txnum;
	arg->uii9.a1 = dx;
	arg->uii9.a2 = dy;
	arg->uii9.a3 = rx;
	arg->uii9.a4 = ry;
	arg->uii9.a5 = w;
	arg->uii9.a6 = h;
	arg->uii9.a7 = dw;
	arg->uii9.a8 = dh;
	arg->uii9.a9 = drawflag;
	return 0;
}

// テクスチャを回転描画する
int Marmalade::DrawTextureRotationC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawTextureRotationC );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii6di.a0 = txnum;
	arg->uii6di.a1 = dx;
	arg->uii6di.a2 = dy;
	arg->uii6di.a3 = rx;
	arg->uii6di.a4 = ry;
	arg->uii6di.a5 = w;
	arg->uii6di.a6 = h;
	arg->uii6di.a7 = rad;
	arg->uii6di.a8 = drawflag;
	return 0;
}

// テクスチャを回転描画する
int Marmalade::DrawTextureRotationAngleC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawTextureRotationAngleC );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii6di.a0 = txnum;
	arg->uii6di.a1 = dx;
	arg->uii6di.a2 = dy;
	arg->uii6di.a3 = rx;
	arg->uii6di.a4 = ry;
	arg->uii6di.a5 = w;
	arg->uii6di.a6 = h;
	arg->uii6di.a7 = angle;
	arg->uii6di.a8 = drawflag;
	return 0;
}

// テクスチャを回転拡大描画する
int Marmalade::DrawTextureRotateScaleC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawTextureRotateScaleC0 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii6d2i.a0 = txnum;
	arg->uii6d2i.a1 = dx;
	arg->uii6d2i.a2 = dy;
	arg->uii6d2i.a3 = rx;
	arg->uii6d2i.a4 = ry;
	arg->uii6d2i.a5 = w;
	arg->uii6d2i.a6 = h;
	arg->uii6d2i.a7 = scale;
	arg->uii6d2i.a8 = rad;
	arg->uii6d2i.a9 = drawflag;
	return 0;
}

// テクスチャを回転拡大描画する
int Marmalade::DrawTextureRotateScaleAngleC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawTextureRotateScaleAngleC0 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii6d2i.a0 = txnum;
	arg->uii6d2i.a1 = dx;
	arg->uii6d2i.a2 = dy;
	arg->uii6d2i.a3 = rx;
	arg->uii6d2i.a4 = ry;
	arg->uii6d2i.a5 = w;
	arg->uii6d2i.a6 = h;
	arg->uii6d2i.a7 = scale;
	arg->uii6d2i.a8 = angle;
	arg->uii6d2i.a9 = drawflag;
	return 0;
}

// テクスチャを回転拡大描画する
int Marmalade::DrawTextureRotateScaleC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawTextureRotateScaleC1 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii8di.a0 = txnum;
	arg->uii8di.a1 = dx;
	arg->uii8di.a2 = dy;
	arg->uii8di.a3 = rx;
	arg->uii8di.a4 = ry;
	arg->uii8di.a5 = w;
	arg->uii8di.a6 = h;
	arg->uii8di.a7 = sw;
	arg->uii8di.a8 = sh;
	arg->uii8di.a9 = rad;
	arg->uii8di.a10 = drawflag;
	return 0;
}

// テクスチャを回転拡大描画する
int Marmalade::DrawTextureRotateScaleAngleC( unsigned int lnum, unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegDrawTextureRotateScaleAngleC1 );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii8di.a0 = txnum;
	arg->uii8di.a1 = dx;
	arg->uii8di.a2 = dy;
	arg->uii8di.a3 = rx;
	arg->uii8di.a4 = ry;
	arg->uii8di.a5 = w;
	arg->uii8di.a6 = h;
	arg->uii8di.a7 = sw;
	arg->uii8di.a8 = sh;
	arg->uii8di.a9 = angle;
	arg->uii8di.a10 = drawflag;
	return 0;
}

// テクスチャの4点を自由に決めて描画する
//int Marmalade::DrawTextureVertex( unsigned int lnum, unsigned int txnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag ){}

// テクスチャの4点を相対座標で自由に決めて描画する
//int Marmalade::DrawTextureRelativeVertex( unsigned int lnum, unsigned int txnum, int rx, int ry, int w, int h, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3, int drawflag ){}

// テクスチャの4点を相対座標で自由に決めて描画する(テクスチャチェックなし:内部用)
// すべてのテクスチャ描画は最終的にこれの関節関数となっている
//int Marmalade::DrawTextureRelativeVertex( unsigned int lnum, unsigned int txnum, int rx, int ry, int w, int h, double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, int drawflag ){}

// 多角形の登録(リスト)
//int Marmalade::CreatePolygon( unsigned int pnum, D3DPRIMITIVETYPE filltype ){}

// 多角形の登録(配列)
//int Marmalade::CreatePolygon( unsigned int pnum, unsigned int max, D3DPRIMITIVETYPE filltype ){}

// 正多角形の登録
//int Marmalade::CreateRegularPolygon( unsigned int pnum, double cx, double cy, unsigned int n, double r, double rad, unsigned long color ){}

// テクスチャの登録
//int Marmalade::SetTextureOnPolygon( unsigned int pnum, unsigned int tnum ){}

//頂点の登録
//int Marmalade::SetPolygonVertex( unsigned int pnum, double x, double y, double z, unsigned long color ){}
//int Marmalade::SetPolygonVertex( double x, double y, double z, unsigned long color ){}


// 頂点要素数の取得
//unsigned int Marmalade::GetPolygonVertexLength( unsigned int pnum ){}

// 頂点情報の編集
//int Marmalade::EditPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, double z, unsigned long color, double u, double v, int edit ){}

// 多角形の描画範囲を取得。xyzそれぞれ。
// 多角形の変形
// 移動
//int Marmalade::TransformPolygonMoving( unsigned int pnum, double mx, double my, double mz ){}

// 拡大
//int Marmalade::TransformPolygonScaling( unsigned int pnum, double scale, unsigned int basenum ){}

// 多角形の描画
//int Marmalade::DrawPolygonVertex( unsigned int pnum ){}
//int Marmalade::DrawPolygonVertex( unsigned int pnum, unsigned int draw ){}
//int Marmalade::DrawPolygonVertex( unsigned int pnum, unsigned int begin, unsigned int end ){}
//int Marmalade::DrawPolygon( unsigned int pnum, double basex, double basey, int cx, int cy ){}
//int Marmalade::DrawPolygon( unsigned int pnum, int cx, int cy, unsigned int basenum ){}

// 多角形の拡大描画
//int Marmalade::DrawPolygonScaling( unsigned int pnum, double basex, double basey, int cx, int cy, double scale ){}
//int Marmalade::DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale, unsigned int basenum ){}

// 正多角形の描画
//int Marmalade::DrawRegularPolygon( double cx, double cy, unsigned int n, double r, double rad, unsigned long color ){}
//int Marmalade::DrawRegularPolygon( double cx, double cy, unsigned int n, int drawnum, double r, double rad, unsigned long color ){}

// 色の作成
unsigned long Marmalade::Color( unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue ){
	return _MikanDraw->Color( alpha, red, green, blue );
}
unsigned long Marmalade::ColorHSV( unsigned char alpha, unsigned int hue, unsigned char sat, unsigned char vob ){
	return _MikanDraw->ColorHSV( alpha, hue, sat, vob );
}

// 色の設定
int Marmalade::SetColor( unsigned int lnum, unsigned int txnum, unsigned long color )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegSetColor );
	if ( arg == NULL )
	{
		return 1;
	}
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uiul.a0 = txnum;
	arg->uiul.a1 = color;
	return 0;
}

// 頂点色の設定
//int Marmalade::SetColorVertex( unsigned int lnum, unsigned int txnum, unsigned long color ){}

// テクスチャのデータを読めるようにする。
//int Marmalade::OpenTexture( unsigned int txnum, int x, int y, int w, int h ){}

// テクスチャのデータを読める状態から戻す。
//int Marmalade::CloseTexture( unsigned int txnum ){}

// テクスチャの指定座標の色を取得する。
//unsigned long Marmalade::GetColorTexturePixel( unsigned int txnum, int x, int y ){}

// テクスチャの範囲内の色を置換する。
//unsigned int Marmalade::TextureColorReplacement( unsigned int txnum, unsigned long scolor, unsigned long rcolor, int x, int y, int w, int h ){}

// テクスチャの範囲内の色を検索する。
//unsigned int Marmalade::TextureColorSearch( unsigned int txnum, unsigned long scolor, int x, int y, int w, int h ){}

// テクスチャの範囲内の色を検索し数を数える。
//unsigned int Marmalade::TextureColorCount( unsigned int txnum, unsigned long scolor, int x, int y, int w, int h ){}

// テクスチャの横幅取得
//unsigned int Marmalade::GetTextureWidth( unsigned int txnum ){}

// テクスチャの高さ取得
//unsigned int Marmalade::GetTextureHeight( unsigned int txnum ){}

// フォント

int Marmalade::LoadFontFile( char *fontfile ){
	return _MikanDraw->LoadFontFile( fontfile );
}
int Marmalade::LoadFontFile( HMODULE module, char *resource ){
	return _MikanDraw->LoadFontFile( module, resource );
}
int Marmalade::CreateFont( unsigned int fontnum, char *fontname, int size, unsigned long color ){
	return _MikanDraw->CreateFont( fontnum, fontname, size, color );
}
int Marmalade::CreateFont( unsigned int fontnum, int size, unsigned long color ){
	return _MikanDraw->CreateFont( fontnum, size, color );
}
int Marmalade::ResetFont( unsigned int fontnum ){
	return _MikanDraw->ResetFont( fontnum );
}

int Marmalade::Printf( unsigned int lnum, unsigned int fontnum, int x, int y, char *format, ... )
{
	union FUNC_ARG *arg = AddLayer( lnum, &Marmalade::RegPrintf );
	if ( arg == NULL )
	{
		return 1;
	}
	char *str;//[ MAX_BUFFER_SIZE ];
	//va_listを使う方法もあるけど、別にこれで問題ないよね。
	str = (char *)malloc( MAX_BUFFER_SIZE );
	vsnprintf_s( str, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, format, (char*)( &(format)+1 ) );
	//  arg = (union FUNC_ARG *)calloc( 1, sizeof( union FUNC_ARG ) );
	arg->uii2cp.a0 = fontnum;
	arg->uii2cp.a1 = x;
	arg->uii2cp.a2 = y;
	arg->uii2cp.a3 = str;
	return 0;
}

// レイヤー登録用関数。

int Marmalade::RegSetRender( union FUNC_ARG * arg )
{
	return _MikanDraw->SetRender( arg->i );
}

int Marmalade::RegSetRenderTarget0( union FUNC_ARG * arg )
{
	return _MikanDraw->SetRenderTarget();
}

int Marmalade::RegSetRenderTarget1( union FUNC_ARG * arg )
{
	return _MikanDraw->SetRenderTarget( arg->ui );
}

int Marmalade::RegDrawPixel( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawPixel( arg->iiul.a0, arg->iiul.a1, arg->iiul.a2 );
}

int Marmalade::RegDrawLine( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawLine( arg->i4uldi.a0, arg->i4uldi.a1, arg->i4uldi.a2, arg->i4uldi.a3, arg->i4uldi.a4, arg->i4uldi.a5, arg->i4uldi.a6 );
}

int Marmalade::RegDrawBox0( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawBox( arg->i4ul.a0, arg->i4ul.a1, arg->i4ul.a2, arg->i4ul.a3, arg->i4ul.a4 );
}

int Marmalade::RegDrawBox1( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawBox( arg->i4ul4.a0, arg->i4ul4.a1, arg->i4ul4.a2, arg->i4ul4.a3, arg->i4ul4.a4, arg->i4ul4.a5, arg->i4ul4.a6, arg->i4ul4.a7 );
}

int Marmalade::RegDrawBox2( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawBox( arg->i8ul.a0, arg->i8ul.a1, arg->i8ul.a2, arg->i8ul.a3, arg->i8ul.a4, arg->i8ul.a5, arg->i8ul.a6, arg->i8ul.a7, arg->i8ul.a8 );
}

int Marmalade::RegDrawBox3( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawBox( arg->i8ul4.a0, arg->i8ul4.a1, arg->i8ul4.a2, arg->i8ul4.a3, arg->i8ul4.a4, arg->i8ul4.a5, arg->i8ul4.a6, arg->i8ul4.a7, arg->i8ul4.a8, arg->i8ul4.a9, arg->i8ul4.a10, arg->i8ul4.a11 );
}

int Marmalade::RegDrawBoxC0( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawBoxC( arg->i4ul.a0, arg->i4ul.a1, arg->i4ul.a2, arg->i4ul.a3, arg->i4ul.a4 );
}

int Marmalade::RegDrawBoxC1( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawBoxC( arg->i4ul4.a0, arg->i4ul4.a1, arg->i4ul4.a2, arg->i4ul4.a3, arg->i4ul4.a4, arg->i4ul4.a5, arg->i4ul4.a6, arg->i4ul4.a7 );
}

int Marmalade::RegDrawCircleLineC( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawCircleLineC( arg->i3uldi.a0, arg->i3uldi.a1, arg->i3uldi.a2, arg->i3uldi.a3, arg->i3uldi.a4, arg->i3uldi.a5 );
}

int Marmalade::RegDrawCircleLine( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawCircleLine( arg->i4uldi.a0, arg->i4uldi.a1, arg->i4uldi.a2, arg->i4uldi.a3, arg->i4uldi.a4, arg->i4uldi.a5, arg->i4uldi.a6 );
}

int Marmalade::RegDrawCircleC( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawCircleC( arg->i3ul.a0, arg->i3ul.a1, arg->i3ul.a2, arg->i3ul.a3 );
}
int Marmalade::RegDrawCircle( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawCircle( arg->i4ul.a0, arg->i4ul.a1, arg->i4ul.a2, arg->i4ul.a3, arg->i4ul.a4 );
}

int Marmalade::RegDrawTexture( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawTexture( arg->uii7.a0, arg->uii7.a1, arg->uii7.a2, arg->uii7.a3, arg->uii7.a4, arg->uii7.a5, arg->uii7.a6, arg->uii7.a7 );
}

int Marmalade::RegDrawTextureC( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawTextureC( arg->uii7.a0, arg->uii7.a1, arg->uii7.a2, arg->uii7.a3, arg->uii7.a4, arg->uii7.a5, arg->uii7.a6, arg->uii7.a7 );
}

int Marmalade::RegDrawTextureScaling0( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawTextureScaling( arg->uii6di.a0, arg->uii6di.a1, arg->uii6di.a2, arg->uii6di.a3, arg->uii6di.a4, arg->uii6di.a5, arg->uii6di.a6, arg->uii6di.a7, arg->uii6di.a8 );
}

int Marmalade::RegDrawTextureScaling1( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawTextureScaling( arg->uii9.a0, arg->uii9.a1, arg->uii9.a2, arg->uii9.a3, arg->uii9.a4, arg->uii9.a5, arg->uii9.a6, arg->uii9.a7, arg->uii9.a8, arg->uii9.a9 );
}

int Marmalade::RegDrawTextureScalingC0( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawTextureScalingC( arg->uii6di.a0, arg->uii6di.a1, arg->uii6di.a2, arg->uii6di.a3, arg->uii6di.a4, arg->uii6di.a5, arg->uii6di.a6, arg->uii6di.a7, arg->uii6di.a8 );
}

int Marmalade::RegDrawTextureScalingC1( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawTextureScalingC( arg->uii9.a0, arg->uii9.a1, arg->uii9.a2, arg->uii9.a3, arg->uii9.a4, arg->uii9.a5, arg->uii9.a6, arg->uii9.a7, arg->uii9.a8, arg->uii9.a9 );
}

int Marmalade::RegDrawTextureRotationC( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawTextureRotationC( arg->uii6di.a0, arg->uii6di.a1, arg->uii6di.a2, arg->uii6di.a3, arg->uii6di.a4, arg->uii6di.a5, arg->uii6di.a6, arg->uii6di.a7, arg->uii6di.a8 );
}

int Marmalade::RegDrawTextureRotationAngleC( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawTextureRotationAngleC( arg->uii6di.a0, arg->uii6di.a1, arg->uii6di.a2, arg->uii6di.a3, arg->uii6di.a4, arg->uii6di.a5, arg->uii6di.a6, arg->uii6di.a7, arg->uii6di.a8 );
}

int Marmalade::RegDrawTextureRotateScaleC0( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawTextureRotateScaleC( arg->uii6d2i.a0, arg->uii6d2i.a1, arg->uii6d2i.a2, arg->uii6d2i.a3, arg->uii6d2i.a4, arg->uii6d2i.a5, arg->uii6d2i.a6, arg->uii6d2i.a7, arg->uii6d2i.a8, arg->uii6d2i.a9 );
}

int Marmalade::RegDrawTextureRotateScaleAngleC0( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawTextureRotateScaleAngleC( arg->uii6d2i.a0, arg->uii6d2i.a1, arg->uii6d2i.a2, arg->uii6d2i.a3, arg->uii6d2i.a4, arg->uii6d2i.a5, arg->uii6d2i.a6, arg->uii6d2i.a7, arg->uii6d2i.a8, arg->uii6d2i.a9 );
}

int Marmalade::RegDrawTextureRotateScaleC1( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawTextureRotateScaleC( arg->uii8di.a0, arg->uii8di.a1, arg->uii8di.a2, arg->uii8di.a3, arg->uii8di.a4, arg->uii8di.a5, arg->uii8di.a6, arg->uii8di.a7, arg->uii8di.a8, arg->uii8di.a9, arg->uii8di.a10 );
}

int Marmalade::RegDrawTextureRotateScaleAngleC1( union FUNC_ARG * arg )
{
	return _MikanDraw->DrawTextureRotateScaleAngleC( arg->uii8di.a0, arg->uii8di.a1, arg->uii8di.a2, arg->uii8di.a3, arg->uii8di.a4, arg->uii8di.a5, arg->uii8di.a6, arg->uii8di.a7, arg->uii8di.a8, arg->uii8di.a9, arg->uii8di.a10 );
}

//int Marmalade::RegDrawTextureVertex( union FUNC_ARG * arg );
//int Marmalade::RegDrawTextureRelativeVertex0( union FUNC_ARG * arg );
//int Marmalade::RegDrawTextureRelativeVertex1( union FUNC_ARG * arg );
//int Marmalade::SetTextureOnPolygon( union FUNC_ARG * arg );
//int Marmalade::SetPolygonVertex0( union FUNC_ARG * arg );
//int Marmalade::SetPolygonVertex1( union FUNC_ARG * arg );
//int Marmalade::EditPolygonVertex( union FUNC_ARG * arg );
//int Marmalade::TransformPolygonMoving( union FUNC_ARG * arg );
//int Marmalade::TransformPolygonScaling( union FUNC_ARG * arg );
//int Marmalade::DrawPolygonVertex0( union FUNC_ARG * arg );
//int Marmalade::DrawPolygonVertex1( union FUNC_ARG * arg );
//int Marmalade::DrawPolygonVertex2( union FUNC_ARG * arg );
//int Marmalade::DrawPolygon0( union FUNC_ARG * arg );
//int Marmalade::DrawPolygon1( union FUNC_ARG * arg );
//int Marmalade::DrawPolygonScaling0( union FUNC_ARG * arg );
//int Marmalade::DrawPolygonScaling1( union FUNC_ARG * arg );
//int Marmalade::DrawRegularPolygon0( union FUNC_ARG * arg );
//int Marmalade::DrawRegularPolygon1( union FUNC_ARG * arg );

int Marmalade::RegSetColor( union FUNC_ARG * arg )
{
	return _MikanDraw->SetColor( arg->uiul.a0, arg->uiul.a1 );
}

int Marmalade::RegPrintf( union FUNC_ARG * arg )
{
	int ret;
	//  char str[ MAX_BUFFER_SIZE ];
	//va_listを使う方法もあるけど、別にこれで問題ないよね。
	//  vsnprintf_s( str, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, arg->uii2cp.a3, (char*)(&(arg->uii2cp.a3)+1) );
	ret = _MikanDraw->Print( arg->uii2cp.a0, arg->uii2cp.a1, arg->uii2cp.a2, arg->uii2cp.a3 );//, str );
	free( arg->uii2cp.a3 );
	return 0;
}
