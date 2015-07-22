#include "DirectXDraw.h"

// 多角形描画
/*int DirectXDraw9::DrawPolygonVertex( unsigned int pnum )
{
return DrawPolygonVertex( pnum, pv[ pnum ].type == 1 ? pv[ pnum ].read.num : pv[ pnum ].max );
}*/

/*int DirectXDraw9::DrawPolygonVertex( unsigned int pnum, unsigned int draw )
{
return DrawPolygonVertex( pnum, 0, draw );
}*/

/*int DirectXDraw9::DrawPolygonVertex( unsigned int pnum, unsigned int begin, unsigned int end )
{
unsigned int i, n;//, num;
struct CUSTOMVERTEX_LIST *next;
struct CUSTOMVERTEX *cv;

if( pnum > pvmax )
{
// 番号が不正なので最後の番号を使う。
pnum = pvmax;
}

if( pv[ pnum ].type == 1 )
{
// 配列
cv = &(pv[ pnum ].vertex.cv[ begin ]);
//num = pv[ pnum ].read.num;
n = end - begin;
}else// if( pv[ pnum ].type == 2 )
{
// リスト
next = pv[ pnum ].vertex.cvl;
for( i = 0 ; next && i < begin ; ++i )
{
next = next->next;
}
n = end - begin;
cv = (struct CUSTOMVERTEX *)calloc( n, sizeof(struct CUSTOMVERTEX) );
next = pv[ pnum ].vertex.cvl;
for( i = 0 ; i < n ; ++i )
{
cv[ i ].x = next->x;
cv[ i ].y = next->y;
cv[ i ].z = next->z;
#ifdef USERHW
cv[ i ].rhw = next->rhw;
#endif
cv[ i ].u = next->u;
cv[ i ].v = next->v;
cv[ i ].color = next->color;
next = next->next;
}
}

switch( pv[ pnum ].filltype )
{
case D3DPT_TRIANGLELIST	:
n = n / 3;
break;
case 	D3DPT_TRIANGLESTRIP:
n = n - 2;
break;
case D3DPT_TRIANGLEFAN:
n = n - 2;
break;
};

// デバッグ表示！！！！！！！！！
//  DrawBox( pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 2 ], pv[ pnum ].space[ 1 ] - pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 3 ] - pv[ pnum ].space[ 2 ], 0x7f00ffff );

D3DDev->SetTexture( 0, pv[ pnum ].txnum >= texmax ? NULL : dxtex[ pv[ pnum ].txnum ].tex );
D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
if( FAILED( D3DDev->DrawPrimitiveUP( pv[ pnum ].filltype, n, cv, sizeof( CUSTOMVERTEX ) ) ) ){ return -1; }

if( pv[ pnum ].type == 2 )
{
DX_FREE( cv );
}

return 0;
}
*/
int DirectXDraw9::DrawPolygon( unsigned int pnum )
{
	unsigned int i, n;
	struct CUSTOMVERTEX_LIST *next;

	if ( pnum > pvmax )
	{
		return 1;
		// 番号が不正なので最後の番号を使う。
		//pnum = pvmax;
	}

	if ( pv[ pnum ].type == 1 )
	{
		// 配列。

		// 新配列の確保。
		pv[ pnum ].cv = pv[ pnum ].vertex.cv;

		// 頂点数を保存。
		n = pv[ pnum ].read.num + 1;

	} else// if( pv[ pnum ].type == 2 )
	{
		// リスト。

		// 新配列の確保。
		if ( pv[ pnum ].cv && pv[ pnum ].cvmax < pv[ pnum ].max )
		{
			free( pv[ pnum ].cv );
			pv[ pnum ].cv = NULL;
		}
		if ( pv[ pnum ].cv == NULL )
		{
			pv[ pnum ].cvmax = pv[ pnum ].max;
			pv[ pnum ].cv = ( struct CUSTOMVERTEX * )calloc( pv[ pnum ].cvmax, sizeof( struct CUSTOMVERTEX ) );
		}

		// 頂点数を保存。
		n = pv[ pnum ].max;

		// コピーしつつ座標移動。
		next = pv[ pnum ].vertex.cvl;
		for ( i = 0; i < pv[ pnum ].max; ++i )
		{
			pv[ pnum ].cv[ i ].x = next->x;
			pv[ pnum ].cv[ i ].y = next->y;
			pv[ pnum ].cv[ i ].z = next->z;
#ifdef USERHW
			pv[ pnum ].cv[ i ].rhw = next->rhw;
#endif
			pv[ pnum ].cv[ i ].u = next->u;
			pv[ pnum ].cv[ i ].v = next->v;
			pv[ pnum ].cv[ i ].color = next->color;
			next = next->next;
		}
	}

	// 三角形の数を計算。
	switch ( pv[ pnum ].filltype )
	{
	case D3DPT_TRIANGLELIST:
		n = n / 3;
		break;
	case 	D3DPT_TRIANGLESTRIP:
		n = n - 2;
		break;
	case D3DPT_TRIANGLEFAN:
		n = n - 2;
		break;
	};

	D3DDev->SetTexture( 0, pv[ pnum ].txnum >= texmax ? NULL : dxtex[ pv[ pnum ].txnum ].tex );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D );//D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1
	if ( FAILED( D3DDev->DrawPrimitiveUP( pv[ pnum ].filltype, n, pv[ pnum ].cv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	//  if( pv[ pnum ].type == 2 )
	//  {
	//    DX_FREE( cv );
	//  }
	return 0;
}

int DirectXDraw9::DrawPolygon( unsigned int pnum, int cx, int cy )
{
	if ( pnum > pvmax )
	{
		// 番号が不正なので最後の番号を使う。
		pnum = pvmax;
	}
	return DrawPolygon( pnum, cx, cy, 0, 0, pv[ pnum ].max );
}

/*int DirectXDraw9::DrawPolygon( unsigned int pnum, double basex, double basey, int cx, int cy )
{
unsigned int i, n;
struct CUSTOMVERTEX_LIST *next;
struct CUSTOMVERTEX *cv;

if( pnum > pvmax )
{
// 番号が不正なので最後の番号を使う。
pnum = pvmax;
}

if( pv[ pnum ].type == 1 )
{
// 配列。

// 描画先座標との差分をとっておく。
cx -= (int)basex;
cy -= (int)basey;

// 新配列の確保。
cv = (struct CUSTOMVERTEX *)calloc( pv[ pnum ].read.num, sizeof(struct CUSTOMVERTEX) );

// 頂点数を保存。
n = pv[ pnum ].read.num;

// コピーしつつ座標移動。
for( i = 0 ; i < pv[ pnum ].read.num ; ++i )
{
cv[ i ].x = pv[ pnum ].vertex.cv[ i ].x + cx;
cv[ i ].y = pv[ pnum ].vertex.cv[ i ].y + cy;
cv[ i ].z = pv[ pnum ].vertex.cv[ i ].z;
#ifdef USERHW
cv[ i ].rhw = pv[ pnum ].vertex.cv[ i ].rhw;
#endif
cv[ i ].u = pv[ pnum ].vertex.cv[ i ].u;
cv[ i ].v = pv[ pnum ].vertex.cv[ i ].v;
cv[ i ].color = pv[ pnum ].vertex.cv[ i ].color;
}
}else// if( pv[ pnum ].type == 2 )
{
// リスト。

// 描画先座標との差分をとっておく。
cx -= (int)basex;
cy -= (int)basey;

// 新配列の確保。
cv = (struct CUSTOMVERTEX *)calloc( pv[ pnum ].max, sizeof(struct CUSTOMVERTEX) );

// 頂点数を保存。
n = pv[ pnum ].max;

// コピーしつつ座標移動。
next = pv[ pnum ].vertex.cvl;
for( i = 0 ; i < pv[ pnum ].max ; ++i )
{
cv[ i ].x = next->x + cx;
cv[ i ].y = next->y + cy;
cv[ i ].z = next->z;
#ifdef USERHW
cv[ i ].rhw = next->rhw;
#endif
cv[ i ].u = next->u;
cv[ i ].v = next->v;
cv[ i ].color = next->color;
next = next->next;
}
}

// 三角形の数を計算。
switch( pv[ pnum ].filltype )
{
case D3DPT_TRIANGLELIST	:
n = n / 3;
break;
case 	D3DPT_TRIANGLESTRIP:
n = n - 2;
break;
case D3DPT_TRIANGLEFAN:
n = n - 2;
break;
};

// デバッグ表示！！！！！！！！！
//  DrawBox( pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 2 ], pv[ pnum ].space[ 1 ] - pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 3 ] - pv[ pnum ].space[ 2 ], 0x7f00ffff );

D3DDev->SetTexture( 0, pv[ pnum ].txnum >= texmax ? NULL : dxtex[ pv[ pnum ].txnum ].tex );
D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1/ );
if( FAILED( D3DDev->DrawPrimitiveUP( pv[ pnum ].filltype, n, cv, sizeof( CUSTOMVERTEX ) ) ) ){ return -1; }

DX_FREE( cv );
return 0;
}*/

int DirectXDraw9::DrawPolygon( unsigned int pnum, int cx, int cy, unsigned int basenum, unsigned int begin, unsigned int end )
{
	unsigned int i, n;
	struct CUSTOMVERTEX_LIST *next;

	if ( pnum > pvmax )
	{
		// 番号が不正なので最後の番号を使う。
		pnum = pvmax;
	}

	if ( pv[ pnum ].type == 1 )
	{
		// 配列

		// 描画先座標との差分をとっておく。
		cx -= (int)pv[ pnum ].vertex.cv[ basenum ].x;
		cy -= (int)pv[ pnum ].vertex.cv[ basenum ].y;

		// 新配列の確保。
		if ( pv[ pnum ].cv == NULL )
		{
			pv[ pnum ].max = pv[ pnum ].max;
			pv[ pnum ].cv = ( struct CUSTOMVERTEX * )calloc( pv[ pnum ].max, sizeof( struct CUSTOMVERTEX ) );
		}
		// 頂点数を保存。
		n = end - begin;//pv[ pnum ].read.num;

		// コピーしつつ座標移動。
		for ( i = 0; i <= pv[ pnum ].read.num; ++i )
		{
			pv[ pnum ].cv[ i ].x = pv[ pnum ].vertex.cv[ i ].x + cx;
			pv[ pnum ].cv[ i ].y = pv[ pnum ].vertex.cv[ i ].y + cy;
			pv[ pnum ].cv[ i ].z = pv[ pnum ].vertex.cv[ i ].z;
#ifdef USERHW
			pv[ pnum ].cv[ i ].rhw = pv[ pnum ].vertex.cv[ i ].rhw;
#endif
			pv[ pnum ].cv[ i ].u = pv[ pnum ].vertex.cv[ i ].u;
			pv[ pnum ].cv[ i ].v = pv[ pnum ].vertex.cv[ i ].v;
			pv[ pnum ].cv[ i ].color = pv[ pnum ].vertex.cv[ i ].color;
		}
	} else// if( pv[ pnum ].type == 2 )
	{
		// リスト

		// 描画先座標との差分をとっておく。
		next = pv[ pnum ].vertex.cvl;
		for ( i = 0; i < basenum; ++i )
		{
			next = next->next;
		}
		cx -= (int)next->x;
		cy -= (int)next->y;

		// 新配列の確保。
		if ( pv[ pnum ].cv && pv[ pnum ].cvmax < pv[ pnum ].max )
		{
			free( pv[ pnum ].cv );
			pv[ pnum ].cv = NULL;
		}
		if ( pv[ pnum ].cv == NULL )
		{
			pv[ pnum ].cvmax = pv[ pnum ].max;
			pv[ pnum ].cv = ( struct CUSTOMVERTEX * )calloc( pv[ pnum ].cvmax, sizeof( struct CUSTOMVERTEX ) );
		}
		// 頂点数を保存。
		n = end - begin;//pv[ pnum ].max;

		// コピーしつつ座標移動。
		next = pv[ pnum ].vertex.cvl;
		for ( i = 0; i < begin; ++i )
		{
			next = next->next;
		}
		for ( ; i < pv[ pnum ].max; ++i )
		{
			pv[ pnum ].cv[ i ].x = next->x + cx;
			pv[ pnum ].cv[ i ].y = next->y + cy;
			pv[ pnum ].cv[ i ].z = next->z;
#ifdef USERHW
			pv[ pnum ].cv[ i ].rhw = next->rhw;
#endif
			pv[ pnum ].cv[ i ].u = next->u;
			pv[ pnum ].cv[ i ].v = next->v;
			pv[ pnum ].cv[ i ].color = next->color;
			next = next->next;
		}
	}

	switch ( pv[ pnum ].filltype )
	{
	case D3DPT_TRIANGLELIST:
		n = n / 3;
		break;
	case 	D3DPT_TRIANGLESTRIP:
		n = n - 2;
		break;
	case D3DPT_TRIANGLEFAN:
		n = n - 2;
		break;
	};

	// デバッグ表示！！！！！！！！！
	//  DrawBox( pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 2 ], pv[ pnum ].space[ 1 ] - pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 3 ] - pv[ pnum ].space[ 2 ], 0x7f00ffff );

	D3DDev->SetTexture( 0, pv[ pnum ].txnum >= texmax ? NULL : dxtex[ pv[ pnum ].txnum ].tex );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );
	if ( FAILED( D3DDev->DrawPrimitiveUP( pv[ pnum ].filltype, n, pv[ pnum ].cv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	//DX_FREE( cv );

	return 0;
}
int DirectXDraw9::DrawPolygon( unsigned int pnum, int cx, int cy, unsigned int begin, unsigned int end )
{
	return DrawPolygon( pnum, cx, cy, 0, begin, end );
	/*unsigned int i, n;
	struct CUSTOMVERTEX_LIST *next;
	struct CUSTOMVERTEX *cv;

	if( pnum > pvmax )
	{
	// 番号が不正なので最後の番号を使う。
	pnum = pvmax;
	}

	if( pv[ pnum ].type == 1 )
	{
	// 配列

	// 描画先座標との差分をとっておく。
	cx -= (int)pv[ pnum ].vertex.cv[ 0 ].x;
	cy -= (int)pv[ pnum ].vertex.cv[ 0 ].y;

	// 新配列の確保。
	cv = (struct CUSTOMVERTEX *)calloc( pv[ pnum ].read.num, sizeof(struct CUSTOMVERTEX) );

	// 頂点数を保存。
	n = end - begin;//pv[ pnum ].read.num;

	// コピーしつつ座標移動。
	for( i = 0 ; i < pv[ pnum ].read.num ; ++i )
	{
	cv[ i ].x = pv[ pnum ].vertex.cv[ i ].x + cx;
	cv[ i ].y = pv[ pnum ].vertex.cv[ i ].y + cy;
	cv[ i ].z = pv[ pnum ].vertex.cv[ i ].z;
	#ifdef USERHW
	cv[ i ].rhw = pv[ pnum ].vertex.cv[ i ].rhw;
	#endif
	cv[ i ].u = pv[ pnum ].vertex.cv[ i ].u;
	cv[ i ].v = pv[ pnum ].vertex.cv[ i ].v;
	cv[ i ].color = pv[ pnum ].vertex.cv[ i ].color;
	}
	}else// if( pv[ pnum ].type == 2 )
	{
	// リスト

	// 描画先座標との差分をとっておく。
	next = pv[ pnum ].vertex.cvl;
	cx -= (int)next->x;
	cy -= (int)next->y;

	// 新配列の確保。
	cv = (struct CUSTOMVERTEX *)calloc( pv[ pnum ].max, sizeof(struct CUSTOMVERTEX) );

	// 頂点数を保存。
	n = end - begin;//pv[ pnum ].max;

	// コピーしつつ座標移動。
	next = pv[ pnum ].vertex.cvl;
	for( i = 0 ; i < begin ; ++i )
	{
	next = next->next;
	}
	for( i = 0 ; i < pv[ pnum ].max ; ++i )
	{
	cv[ i ].x = next->x + cx;
	cv[ i ].y = next->y + cy;
	cv[ i ].z = next->z;
	#ifdef USERHW
	cv[ i ].rhw = next->rhw;
	#endif
	cv[ i ].u = next->u;
	cv[ i ].v = next->v;
	cv[ i ].color = next->color;
	next = next->next;
	}
	}

	switch( pv[ pnum ].filltype )
	{
	case D3DPT_TRIANGLELIST	:
	n = n / 3;
	break;
	case 	D3DPT_TRIANGLESTRIP:
	n = n - 2;
	break;
	case D3DPT_TRIANGLEFAN:
	n = n - 2;
	break;
	};

	// デバッグ表示！！！！！！！！！
	//  DrawBox( pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 2 ], pv[ pnum ].space[ 1 ] - pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 3 ] - pv[ pnum ].space[ 2 ], 0x7f00ffff );

	D3DDev->SetTexture( 0, pv[ pnum ].txnum >= texmax ? NULL : dxtex[ pv[ pnum ].txnum ].tex );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D );//D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1
	if( FAILED( D3DDev->DrawPrimitiveUP( pv[ pnum ].filltype, n, cv, sizeof( CUSTOMVERTEX ) ) ) ){ return -1; }

	DX_FREE( cv );*/
	return 0;
}
/*int DirectXDraw9::DrawPolygonScaling( unsigned int pnum, double basex, double basey, int cx, int cy, double scale )
{
unsigned int i, n;
double mx, my, cx_ = cx, cy_ = cy;
struct CUSTOMVERTEX_LIST *next;
struct CUSTOMVERTEX *cv;
D3DXMATRIX posmatrix,zoommatrix;

// 行列の初期化。
D3DXMatrixIdentity( &posmatrix );
D3DXMatrixIdentity( &zoommatrix );

// 拡大率の値をzoommatrix行列に代入する
D3DXMatrixScaling( &zoommatrix, (float)scale, (float)scale, 0.0 );

if( pnum > pvmax )
{
// 番号が不正なので最後の番号を使う。
pnum = pvmax;
}

if( pv[ pnum ].type == 1 )
{
// 配列

// 基準座標への移動距離。
mx = basex;
my = basey;

// 描画先座標との差分をとっておく。
cx_ -= mx;
cy_ -= my;

// 新配列の確保。
cv = (struct CUSTOMVERTEX *)calloc( pv[ pnum ].read.num, sizeof(struct CUSTOMVERTEX) );

// 頂点数を保存。
n = pv[ pnum ].read.num;

// 新配列に座標を拡大・移動しつつコピー。
for( i = 0 ; i < pv[ pnum ].read.num ; ++i )
{
// 基準点を原点とした時の頂点の位置を算出。
D3DXMatrixTranslation( &posmatrix, (float)(pv[ pnum ].vertex.cv[ i ].x - mx), (float)(pv[ pnum ].vertex.cv[ i ].y - my), (float)(pv[ pnum ].vertex.cv[ i ].z) );
// 行列演算。
posmatrix *= zoommatrix;

cv[ i ].x = posmatrix._41 + (float)cx_;//pv[ pnum ].vertex.cv[ i ].x + cx;
cv[ i ].y = posmatrix._42 + (float)cy_;//pv[ pnum ].vertex.cv[ i ].y + cy;
cv[ i ].z = posmatrix._43;//pv[ pnum ].vertex.cv[ i ].z;
#ifdefUSERHW
cv[ i ].rhw = pv[ pnum ].vertex.cv[ i ].rhw;
#endif
cv[ i ].u = pv[ pnum ].vertex.cv[ i ].u;
cv[ i ].v = pv[ pnum ].vertex.cv[ i ].v;
cv[ i ].color = pv[ pnum ].vertex.cv[ i ].color;
}
}else if( pv[ pnum ].type == 2 )
{
// リスト

// 基準座標への移動距離。
mx = basex;
my = basey;
// 描画先座標との差分をとっておく。
cx_ -= mx;
cy_ -= my;

// 新配列の確保。
cv = (struct CUSTOMVERTEX *)calloc( pv[ pnum ].max, sizeof(struct CUSTOMVERTEX) );

// 頂点数の保存。
n = pv[ pnum ].max;

// 新配列に座標を拡大・移動しつつコピー。
next = pv[ pnum ].vertex.cvl;
for( i = 0 ; i < pv[ pnum ].max ; ++i )
{
// 基準点を原点とした時の頂点の位置を算出。
D3DXMatrixTranslation( &posmatrix, (float)(pv[ pnum ].vertex.cv[ i ].x - mx), (float)(pv[ pnum ].vertex.cv[ i ].y - my), (float)(pv[ pnum ].vertex.cv[ i ].z) );
// 行列演算。
posmatrix *= zoommatrix;

cv[ i ].x = posmatrix._41 + (float)cx_;//next->x + cx;
cv[ i ].y = posmatrix._42 + (float)cy_;//next->y + cy;
cv[ i ].z = posmatrix._43;//next->z;
#ifdef USERHW
cv[ i ].rhw = next->rhw;
#endif
cv[ i ].u = next->u;
cv[ i ].v = next->v;
cv[ i ].color = next->color;
next = next->next;
}
}

switch( pv[ pnum ].filltype )
{
case D3DPT_TRIANGLELIST	:
n = n / 3;
break;
case 	D3DPT_TRIANGLESTRIP:
n = n - 2;
break;
case D3DPT_TRIANGLEFAN:
n = n - 2;
break;
};

// デバッグ表示！！！！！！！！！
//  DrawBox( pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 2 ], pv[ pnum ].space[ 1 ] - pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 3 ] - pv[ pnum ].space[ 2 ], 0x7f00ffff );

D3DDev->SetTexture( 0, pv[ pnum ].txnum >= texmax ? NULL : dxtex[ pv[ pnum ].txnum ].tex );
D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D );//D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1
if( FAILED( D3DDev->DrawPrimitiveUP( pv[ pnum ].filltype, n, cv, sizeof( CUSTOMVERTEX ) ) ) ){ return -1; }

DX_FREE( cv );

return 0;
}*/

int DirectXDraw9::DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale )
{
	unsigned int i, n;
	double mx, my, cx_ = cx, cy_ = cy;
	struct CUSTOMVERTEX_LIST *next;
	D3DXMATRIX posmatrix, zoommatrix;

	// 行列の初期化。
	D3DXMatrixIdentity( &posmatrix );
	D3DXMatrixIdentity( &zoommatrix );

	// 拡大率の値をzoommatrix行列に代入する。
	D3DXMatrixScaling( &zoommatrix, (float)scale, (float)scale, 0.0 );

	if ( pnum > pvmax )
	{
		// 番号が不正なので最後の番号を使う。
		pnum = pvmax;
	}

	if ( pv[ pnum ].type == 1 )
	{
		// 配列

		// 基準座標への移動距離。
		mx = pv[ pnum ].vertex.cv[ 0 ].x;
		my = pv[ pnum ].vertex.cv[ 0 ].y;

		// 描画先座標との差分をとっておく。
		cx_ -= mx;
		cy_ -= my;

		// 新配列の確保。
		if ( pv[ pnum ].cv == NULL )
		{
			pv[ pnum ].cvmax = pv[ pnum ].max;
			pv[ pnum ].cv = ( struct CUSTOMVERTEX * )calloc( pv[ pnum ].cvmax, sizeof( struct CUSTOMVERTEX ) );
		}
		// 頂点数を保存。
		n = pv[ pnum ].read.num;

		// 新配列に座標を拡大・移動しつつコピー。
		for ( i = 0; i <= pv[ pnum ].read.num; ++i )
		{
			// 基準点を原点とした時の頂点の位置を算出。
			D3DXMatrixTranslation( &posmatrix, (float)( pv[ pnum ].vertex.cv[ i ].x - mx ), (float)( pv[ pnum ].vertex.cv[ i ].y - my ), (float)( pv[ pnum ].vertex.cv[ i ].z ) );
			// 行列演算。
			posmatrix *= zoommatrix;

			pv[ pnum ].cv[ i ].x = posmatrix._41 + (float)cx_;//pv[ pnum ].vertex.cv[ i ].x + cx;
			pv[ pnum ].cv[ i ].y = posmatrix._42 + (float)cy_;//pv[ pnum ].vertex.cv[ i ].y + cy;
			pv[ pnum ].cv[ i ].z = posmatrix._43;//pv[ pnum ].vertex.cv[ i ].z;
#ifdef USERHW
			pv[ pnum ].cv[ i ].rhw = pv[ pnum ].vertex.cv[ i ].rhw;
#endif
			pv[ pnum ].cv[ i ].u = pv[ pnum ].vertex.cv[ i ].u;
			pv[ pnum ].cv[ i ].v = pv[ pnum ].vertex.cv[ i ].v;
			pv[ pnum ].cv[ i ].color = pv[ pnum ].vertex.cv[ i ].color;
		}
	} else// if( pv[ pnum ].type == 2 )
	{
		// リスト

		// 基準座標を調べる。
		next = pv[ pnum ].vertex.cvl;
		// 基準座標への移動距離。
		mx = next->x;
		my = next->y;
		// 描画先座標との差分をとっておく。
		cx_ -= mx;
		cy_ -= my;

		// 新配列の確保。
		if ( pv[ pnum ].cv && pv[ pnum ].cvmax < pv[ pnum ].max )
		{
			free( pv[ pnum ].cv );
			pv[ pnum ].cv = NULL;
		}
		if ( pv[ pnum ].cv == NULL )
		{
			pv[ pnum ].cv = ( struct CUSTOMVERTEX * )calloc( pv[ pnum ].max, sizeof( struct CUSTOMVERTEX ) );
		}
		// 頂点数の保存。
		n = pv[ pnum ].max;

		// 新配列に座標を拡大・移動しつつコピー。
		next = pv[ pnum ].vertex.cvl;
		for ( i = 0; i < pv[ pnum ].max; ++i )
		{
			// 基準点を原点とした時の頂点の位置を算出。
			D3DXMatrixTranslation( &posmatrix, (float)( pv[ pnum ].vertex.cv[ i ].x - mx ), (float)( pv[ pnum ].vertex.cv[ i ].y - my ), (float)( pv[ pnum ].vertex.cv[ i ].z ) );
			// 行列演算。
			posmatrix *= zoommatrix;

			pv[ pnum ].cv[ i ].x = posmatrix._41 + (float)cx_;//next->x + cx;
			pv[ pnum ].cv[ i ].y = posmatrix._42 + (float)cy_;//next->y + cy;
			pv[ pnum ].cv[ i ].z = posmatrix._43;//next->z;
#ifdef USERHW
			pv[ pnum ].cv[ i ].rhw = next->rhw;
#endif
			pv[ pnum ].cv[ i ].u = next->u;
			pv[ pnum ].cv[ i ].v = next->v;
			pv[ pnum ].cv[ i ].color = next->color;
			next = next->next;
		}
	}

	switch ( pv[ pnum ].filltype )
	{
	case D3DPT_TRIANGLELIST:
		n = n / 3;
		break;
	case 	D3DPT_TRIANGLESTRIP:
		n = n - 2;
		break;
	case D3DPT_TRIANGLEFAN:
		n = n - 2;
		break;
	};

	// デバッグ表示！！！！！！！！！
	//  DrawBox( pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 2 ], pv[ pnum ].space[ 1 ] - pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 3 ] - pv[ pnum ].space[ 2 ], 0x7f00ffff );

	D3DDev->SetTexture( 0, pv[ pnum ].txnum >= texmax ? NULL : dxtex[ pv[ pnum ].txnum ].tex );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );
	if ( FAILED( D3DDev->DrawPrimitiveUP( pv[ pnum ].filltype, n, pv[ pnum ].cv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	//DX_FREE( cv );

	return 0;
}

int DirectXDraw9::DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale, unsigned int basenum, unsigned int start, unsigned int end )
{
	unsigned int i, n;
	double mx, my, cx_ = cx, cy_ = cy;
	struct CUSTOMVERTEX_LIST *next;
	D3DXMATRIX posmatrix, zoommatrix;

	// 行列の初期化。
	D3DXMatrixIdentity( &posmatrix );
	D3DXMatrixIdentity( &zoommatrix );

	// 拡大率の値をzoommatrix行列に代入する。
	D3DXMatrixScaling( &zoommatrix, (float)scale, (float)scale, 0.0 );

	if ( pnum > pvmax )
	{
		// 番号が不正なので最後の番号を使う。
		pnum = pvmax;
	}

	if ( pv[ pnum ].type == 1 )
	{
		// 配列

		// 基準座標への移動距離。
		mx = pv[ pnum ].vertex.cv[ basenum ].x;
		my = pv[ pnum ].vertex.cv[ basenum ].y;

		// 描画先座標との差分をとっておく。
		cx_ -= mx;
		cy_ -= my;

		// 新配列の確保。
		if ( pv[ pnum ].cv == NULL )
		{
			pv[ pnum ].cvmax = pv[ pnum ].max;
			pv[ pnum ].cv = ( struct CUSTOMVERTEX * )calloc( pv[ pnum ].cvmax, sizeof( struct CUSTOMVERTEX ) );
		}

		// 頂点数を保存。
		n = end - start;//pv[ pnum ].read.num;

		// 新配列に座標を拡大・移動しつつコピー。
		for ( i = start; i < end/*pv[ pnum ].read.num*/; ++i )
		{
			// 基準点を原点とした時の頂点の位置を算出。
			D3DXMatrixTranslation( &posmatrix, (float)( pv[ pnum ].vertex.cv[ i ].x - mx ), (float)( pv[ pnum ].vertex.cv[ i ].y - my ), (float)( pv[ pnum ].vertex.cv[ i ].z ) );
			// 行列演算。
			posmatrix *= zoommatrix;

			pv[ pnum ].cv[ i ].x = posmatrix._41 + (float)cx_;//pv[ pnum ].vertex.cv[ i ].x + cx;
			pv[ pnum ].cv[ i ].y = posmatrix._42 + (float)cy_;//pv[ pnum ].vertex.cv[ i ].y + cy;
			pv[ pnum ].cv[ i ].z = posmatrix._43;//pv[ pnum ].vertex.cv[ i ].z;
#ifdef USERHW
			pv[ pnum ].cv[ i ].rhw = pv[ pnum ].vertex.cv[ i ].rhw;
#endif
			pv[ pnum ].cv[ i ].u = pv[ pnum ].vertex.cv[ i ].u;
			pv[ pnum ].cv[ i ].v = pv[ pnum ].vertex.cv[ i ].v;
			pv[ pnum ].cv[ i ].color = pv[ pnum ].vertex.cv[ i ].color;
		}
	} else// if( pv[ pnum ].type == 2 )
	{
		// リスト

		// 基準座標を調べる。
		next = pv[ pnum ].vertex.cvl;
		for ( i = start; i < basenum; ++i )
		{
			next = next->next;
		}
		// 基準座標への移動距離。
		mx = next->x;
		my = next->y;
		// 描画先座標との差分をとっておく。
		cx_ -= mx;
		cy_ -= my;

		// 新配列の確保。
		if ( pv[ pnum ].cv && pv[ pnum ].cvmax < pv[ pnum ].max )
		{
			free( pv[ pnum ].cv );
			pv[ pnum ].cv = NULL;
		}
		if ( pv[ pnum ].cv == NULL )
		{
			pv[ pnum ].cvmax = pv[ pnum ].max;
			pv[ pnum ].cv = ( struct CUSTOMVERTEX * )calloc( pv[ pnum ].cvmax, sizeof( struct CUSTOMVERTEX ) );
		}
		// 頂点数の保存。
		n = end - start;//pv[ pnum ].max;

		// 新配列に座標を拡大・移動しつつコピー。
		next = pv[ pnum ].vertex.cvl;
		for ( i = 0; i < pv[ pnum ].max; ++i )
		{
			// 基準点を原点とした時の頂点の位置を算出。
			D3DXMatrixTranslation( &posmatrix, (float)( pv[ pnum ].vertex.cv[ i ].x - mx ), (float)( pv[ pnum ].vertex.cv[ i ].y - my ), (float)( pv[ pnum ].vertex.cv[ i ].z ) );
			// 行列演算。
			posmatrix *= zoommatrix;

			pv[ pnum ].cv[ i ].x = posmatrix._41 + (float)cx_;//next->x + cx;
			pv[ pnum ].cv[ i ].y = posmatrix._42 + (float)cy_;//next->y + cy;
			pv[ pnum ].cv[ i ].z = posmatrix._43;//next->z;
#ifdef USERHW
			pv[ pnum ].cv[ i ].rhw = next->rhw;
#endif
			pv[ pnum ].cv[ i ].u = next->u;
			pv[ pnum ].cv[ i ].v = next->v;
			pv[ pnum ].cv[ i ].color = next->color;
			next = next->next;
		}
	}

	switch ( pv[ pnum ].filltype )
	{
	case D3DPT_TRIANGLELIST:
		n = n / 3;
		break;
	case 	D3DPT_TRIANGLESTRIP:
		n = n - 2;
		break;
	case D3DPT_TRIANGLEFAN:
		n = n - 2;
		break;
	};

	// デバッグ表示！！！！！！！！！
	//  DrawBox( pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 2 ], pv[ pnum ].space[ 1 ] - pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 3 ] - pv[ pnum ].space[ 2 ], 0x7f00ffff );

	D3DDev->SetTexture( 0, pv[ pnum ].txnum >= texmax ? NULL : dxtex[ pv[ pnum ].txnum ].tex );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );
	if ( FAILED( D3DDev->DrawPrimitiveUP( pv[ pnum ].filltype, n, pv[ pnum ].cv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	//DX_FREE( cv );

	return 0;
}

int DirectXDraw9::DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale, unsigned int start, unsigned int end )
{
	unsigned int i, n;
	double mx, my, cx_ = cx, cy_ = cy;
	struct CUSTOMVERTEX_LIST *next;
	D3DXMATRIX posmatrix, zoommatrix;

	// 行列の初期化。
	D3DXMatrixIdentity( &posmatrix );
	D3DXMatrixIdentity( &zoommatrix );

	// 拡大率の値をzoommatrix行列に代入する。
	D3DXMatrixScaling( &zoommatrix, (float)scale, (float)scale, 0.0 );

	if ( pnum > pvmax )
	{
		// 番号が不正なので最後の番号を使う。
		pnum = pvmax;
	}

	if ( pv[ pnum ].type == 1 )
	{
		// 配列

		// 基準座標への移動距離。
		mx = pv[ pnum ].vertex.cv[ start ].x;
		my = pv[ pnum ].vertex.cv[ start ].y;

		// 描画先座標との差分をとっておく。
		cx_ -= mx;
		cy_ -= my;

		// 新配列の確保。
		if ( pv[ pnum ].cv == NULL )
		{
			pv[ pnum ].cvmax = pv[ pnum ].max;
			pv[ pnum ].cv = ( struct CUSTOMVERTEX * )calloc( pv[ pnum ].cvmax, sizeof( struct CUSTOMVERTEX ) );
		}

		// 頂点数を保存。
		n = end - start;//pv[ pnum ].read.num;

		// 新配列に座標を拡大・移動しつつコピー。
		for ( i = start; i < end/*pv[ pnum ].read.num*/; ++i )
		{
			// 基準点を原点とした時の頂点の位置を算出。
			D3DXMatrixTranslation( &posmatrix, (float)( pv[ pnum ].vertex.cv[ i ].x - mx ), (float)( pv[ pnum ].vertex.cv[ i ].y - my ), (float)( pv[ pnum ].vertex.cv[ i ].z ) );
			// 行列演算。
			posmatrix *= zoommatrix;

			pv[ pnum ].cv[ i ].x = posmatrix._41 + (float)cx_;//pv[ pnum ].vertex.cv[ i ].x + cx;
			pv[ pnum ].cv[ i ].y = posmatrix._42 + (float)cy_;//pv[ pnum ].vertex.cv[ i ].y + cy;
			pv[ pnum ].cv[ i ].z = posmatrix._43;//pv[ pnum ].vertex.cv[ i ].z;
#ifdef USERHW
			pv[ pnum ].cv[ i ].rhw = pv[ pnum ].vertex.cv[ i ].rhw;
#endif
			pv[ pnum ].cv[ i ].u = pv[ pnum ].vertex.cv[ i ].u;
			pv[ pnum ].cv[ i ].v = pv[ pnum ].vertex.cv[ i ].v;
			pv[ pnum ].cv[ i ].color = pv[ pnum ].vertex.cv[ i ].color;
		}
	} else// if( pv[ pnum ].type == 2 )
	{
		// リスト

		// 基準座標を調べる。
		next = pv[ pnum ].vertex.cvl;
		for ( i = start; i < end; ++i )
		{
			next = next->next;
		}
		// 基準座標への移動距離。
		mx = next->x;
		my = next->y;
		// 描画先座標との差分をとっておく。
		cx_ -= mx;
		cy_ -= my;

		// 新配列の確保。
		if ( pv[ pnum ].cv && pv[ pnum ].cvmax < pv[ pnum ].max )
		{
			free( pv[ pnum ].cv );
			pv[ pnum ].cv = NULL;
		}
		if ( pv[ pnum ].cv == NULL )
		{
			pv[ pnum ].cvmax = pv[ pnum ].max;
			pv[ pnum ].cv = ( struct CUSTOMVERTEX * )calloc( pv[ pnum ].cvmax, sizeof( struct CUSTOMVERTEX ) );
		}
		// 頂点数の保存。
		n = end - start;//pv[ pnum ].max;

		// 新配列に座標を拡大・移動しつつコピー。
		next = pv[ pnum ].vertex.cvl;
		for ( i = 0; i < pv[ pnum ].max; ++i )
		{
			// 基準点を原点とした時の頂点の位置を算出。
			D3DXMatrixTranslation( &posmatrix, (float)( pv[ pnum ].vertex.cv[ i ].x - mx ), (float)( pv[ pnum ].vertex.cv[ i ].y - my ), (float)( pv[ pnum ].vertex.cv[ i ].z ) );
			// 行列演算。
			posmatrix *= zoommatrix;

			pv[ pnum ].cv[ i ].x = posmatrix._41 + (float)cx_;//next->x + cx;
			pv[ pnum ].cv[ i ].y = posmatrix._42 + (float)cy_;//next->y + cy;
			pv[ pnum ].cv[ i ].z = posmatrix._43;//next->z;
#ifdef USERHW
			pv[ pnum ].cv[ i ].rhw = next->rhw;
#endif
			pv[ pnum ].cv[ i ].u = next->u;
			pv[ pnum ].cv[ i ].v = next->v;
			pv[ pnum ].cv[ i ].color = next->color;
			next = next->next;
		}
	}

	switch ( pv[ pnum ].filltype )
	{
	case D3DPT_TRIANGLELIST:
		n = n / 3;
		break;
	case 	D3DPT_TRIANGLESTRIP:
		n = n - 2;
		break;
	case D3DPT_TRIANGLEFAN:
		n = n - 2;
		break;
	};

	// デバッグ表示！！！！！！！！！
	//  DrawBox( pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 2 ], pv[ pnum ].space[ 1 ] - pv[ pnum ].space[ 0 ], pv[ pnum ].space[ 3 ] - pv[ pnum ].space[ 2 ], 0x7f00ffff );

	D3DDev->SetTexture( 0, pv[ pnum ].txnum >= texmax ? NULL : dxtex[ pv[ pnum ].txnum ].tex );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );
	if ( FAILED( D3DDev->DrawPrimitiveUP( pv[ pnum ].filltype, n, pv[ pnum ].cv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	//DX_FREE( cv );

	return 0;
}

// 正多角形の描画
int DirectXDraw9::DrawRegularPolygon( double cx, double cy, unsigned int n, double r, double rad, unsigned long color )
{
	return DrawRegularPolygon( cx, cy, n, n, r, rad, color );
}

int DirectXDraw9::DrawRegularPolygon( double cx, double cy, unsigned int n, int drawnum, double r, double rad, unsigned long color )
{
	double arad;
	//  CreatePolygon( pvmax-3, n + 1, D3DPT_TRIANGLEFAN );
	//  CreatePolygon( pvmax, n, D3DPT_TRIANGLEFAN );
	unsigned int i;
	//  drawnum;

	if ( rpv && rpvmax < n + 2 )
	{
		free( rpv );
		rpv = NULL;
	}
	if ( rpv == NULL )
	{
		rpvmax = n + 2;
		rpv = ( struct CUSTOMVERTEX * )calloc( rpvmax, sizeof( struct CUSTOMVERTEX ) );
	}
	// 中央座標
	//  SetPolygonVertex( pvmax-3, cx, cy, 0.5, color );
	rpv[ 0 ].x = (float)cx;
	rpv[ 0 ].y = (float)cy;
	rpv[ 0 ].z = 0.5f;
	rpv[ 0 ].color = color;
#ifdef USERHW
	rpv[ 0 ].rhw = 1.0f;
#endif
	rpv[ 0 ].u = 0.0f;
	rpv[ 0 ].v = 0.0f;

	arad = AngleToRadian( 360.0 / (double)n );

	for ( i = 0; i <= n; ++i )
	{
		//SetPolygonVertex( pvmax, cx + cos( arad * n ) * r, cy + sin( arad * n ) * r, 0.5, color );
		rpv[ i + 1 ].x = (float)( cx + cos( rad + arad * i ) * r );
		rpv[ i + 1 ].y = (float)( cy + sin( rad + arad * i ) * r );
		rpv[ i + 1 ].z = 0.5f;
		rpv[ i + 1 ].color = color;
#ifdef USERHW
		rpv[ i + 1 ].rhw = 1.0f;
#endif
		rpv[ i + 1 ].u = 0.0f;
		rpv[ i + 1 ].v = 0.0f;
	}

	//  return DrawPolygonVertex( pvmax );
	D3DDev->SetTexture( 0, NULL );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );
	if ( FAILED( D3DDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, drawnum, rpv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	//DX_FREE( cv );
	return 0;
}
