#include "DirectXDrawBase.h"
#include <d3dx9.h>

// テクスチャ情報を登録する。
int DirectXDrawBase::_SetUV( float width, float height, int rx, int ry, int w, int h, int drawflag )
{
	int lr, ud;
	float u[ 2 ], v[ 2 ];

	//lr=1の時左右反転
	lr = ( drawflag & DRAW_LR ) ? 1 : 0;
	//ud=1の時上下反転
	ud = ( drawflag & DRAW_UD ) ? 1 : 0;

	u[ 0 ] = (float)( ( rx + w * lr ) / width );
	u[ 1 ] = (float)( ( rx + w * ( 1 - lr ) ) / width );
	v[ 0 ] = (float)( ( ry + h * ud ) / height );
	v[ 1 ] = (float)( ( ry + h * ( 1 - ud ) ) / height );

	_cv[ 0 ].u = u[ 0 ]; _cv[ 0 ].v = v[ 0 ];
	_cv[ 1 ].u = u[ 1 ]; _cv[ 1 ].v = v[ 0 ];
	_cv[ 2 ].u = u[ 0 ]; _cv[ 2 ].v = v[ 1 ];
	_cv[ 3 ].u = u[ 1 ]; _cv[ 3 ].v = v[ 1 ];

#ifdef USERHW
	_cv[ 0 ].rhw = _cv[ 1 ].rhw = _cv[ 2 ].rhw = _cv[ 3 ].rhw = 1.0f;
#endif
	_cv[ 0 ].z = 0.5f;
	_cv[ 1 ].z = 0.5f;
	_cv[ 2 ].z = 0.5f;
	_cv[ 3 ].z = 0.5f;
	return 0;
}

// 頂点色。
int DirectXDrawBase::_SetColor( unsigned long *colors )
{
	_cv[ 0 ].color = colors[ 0 ];
	_cv[ 1 ].color = colors[ 1 ];
	_cv[ 2 ].color = colors[ 2 ];
	_cv[ 3 ].color = colors[ 3 ];
	return 0;
}

int DirectXDrawBase::_SetColor( unsigned long lt_color, unsigned long rt_color, unsigned long ld_color, unsigned long rd_color )
{
	_cv[ 0 ].color = lt_color;
	_cv[ 1 ].color = rt_color;
	_cv[ 2 ].color = ld_color;
	_cv[ 3 ].color = rd_color;
	return 0;
}


// テクスチャを描画する
int DirectXDrawBase::_DrawTexture( float dx, float dy, float w, float h )
{
	float fx, fy, fw, fh;

	fx = dx - 0.5f;
	fy = dy - 0.5f;
	fw = w;
	fh = h;

	_cv[ 0 ].x = fx;      _cv[ 0 ].y = fy;
	_cv[ 1 ].x = fx + fw; _cv[ 1 ].y = fy;
	_cv[ 2 ].x = fx;      _cv[ 2 ].y = fy + fh;
	_cv[ 3 ].x = fx + fw; _cv[ 3 ].y = fy + fh;

	return 0;
}

// テクスチャを描画する
int DirectXDrawBase::_DrawTextureC( float dx, float dy, float w, float h )
{
	float fx, fy, fw, fh;

	fx = dx - 0.5f;
	fy = dy - 0.5f;
	fw = w;
	fh = h;

	_cv[ 0 ].x = (float)( fx - fw / 2.0 ); _cv[ 0 ].y = (float)( fy - fh / 2.0 );
	_cv[ 1 ].x = (float)( fx + fw / 2.0 ); _cv[ 1 ].y = (float)( fy - fh / 2.0 );
	_cv[ 2 ].x = (float)( fx - fw / 2.0 ); _cv[ 2 ].y = (float)( fy + fh / 2.0 );
	_cv[ 3 ].x = (float)( fx + fw / 2.0 ); _cv[ 3 ].y = (float)( fy + fh / 2.0 );

	return 0;
}

// テクスチャを拡大縮小描画する
int DirectXDrawBase::_DrawTextureScaling( float dx, float dy, float dw, float dh )
{
	float fx, fy, fw, fh;

	fx = dx - 0.5f;
	fy = dy - 0.5f;
	fw = dw;
	fh = dh;

	_cv[ 0 ].x = fx;      _cv[ 0 ].y = fy;
	_cv[ 1 ].x = fx + fw; _cv[ 1 ].y = fy;
	_cv[ 2 ].x = fx;      _cv[ 2 ].y = fy + fh;
	_cv[ 3 ].x = fx + fw; _cv[ 3 ].y = fy + fh;

	return 0;
}

// テクスチャを拡大縮小描画する
int DirectXDrawBase::_DrawTextureScalingC( float dx, float dy, float dw, float dh )
{
	float fx, fy, fw, fh;

	fx = dx - 0.5f;
	fy = dy - 0.5f;
	fw = dw;
	fh = dh;

	_cv[ 0 ].x = (float)( fx - fw / 2.0 ); _cv[ 0 ].y = (float)( fy - fh / 2.0 );
	_cv[ 1 ].x = (float)( fx + fw / 2.0 ); _cv[ 1 ].y = (float)( fy - fh / 2.0 );
	_cv[ 2 ].x = (float)( fx - fw / 2.0 ); _cv[ 2 ].y = (float)( fy + fh / 2.0 );
	_cv[ 3 ].x = (float)( fx + fw / 2.0 ); _cv[ 3 ].y = (float)( fy + fh / 2.0 );

	return 0;
}

// テクスチャを拡大縮小描画する
int DirectXDrawBase::_DrawTextureRotationC( float dx, float dy, float dw, float dh, double rad )
{
	D3DXMATRIX matRotate;
	D3DXMATRIX matTranslation;
	D3DXMATRIX matComposition;
	D3DXVECTOR4 vp[ 4 ];

	D3DXMatrixRotationZ( &matRotate, (float)rad );
	D3DXMatrixTranslation( &matTranslation, (float)dx - 0.5f, (float)dy - 0.5f, 0.0 );
	D3DXMatrixMultiply( &matComposition, &matRotate, &matTranslation );

	vp[ 0 ].x = (float)( -dw / 2.0 );
	vp[ 0 ].y = (float)( -dh / 2.0 );
	vp[ 1 ].x = (float)( dw / 2.0 );
	vp[ 1 ].y = (float)( -dh / 2.0 );
	vp[ 2 ].x = (float)( -dw / 2.0 );
	vp[ 2 ].y = (float)( dh / 2.0 );
	vp[ 3 ].x = (float)( dw / 2.0 );
	vp[ 3 ].y = (float)( dh / 2.0 );
	vp[ 0 ].z = vp[ 1 ].z = vp[ 2 ].z = vp[ 3 ].z = 0.5;
	vp[ 0 ].w = vp[ 1 ].w = vp[ 2 ].w = vp[ 3 ].w = 1.0;

	D3DXVec4Transform( &vp[ 0 ], &vp[ 0 ], &matComposition );
	D3DXVec4Transform( &vp[ 1 ], &vp[ 1 ], &matComposition );
	D3DXVec4Transform( &vp[ 2 ], &vp[ 2 ], &matComposition );
	D3DXVec4Transform( &vp[ 3 ], &vp[ 3 ], &matComposition );

	_cv[ 0 ].x = vp[ 0 ].x; _cv[ 0 ].y = vp[ 0 ].y;
	_cv[ 1 ].x = vp[ 1 ].x; _cv[ 1 ].y = vp[ 1 ].y;
	_cv[ 2 ].x = vp[ 2 ].x; _cv[ 2 ].y = vp[ 2 ].y;
	_cv[ 3 ].x = vp[ 3 ].x; _cv[ 3 ].y = vp[ 3 ].y;

	return 0;
}

// テクスチャを回転拡大描画する
int DirectXDrawBase::_DrawTextureRotateScale( float cx, float cy, float dx, float dy, double rad, float dw, float dh )
{
	D3DXMATRIX matRotate;
	D3DXMATRIX matTranslation;
	D3DXMATRIX matComposition;
	D3DXVECTOR4 vp[ 4 ];

	D3DXMatrixRotationZ( &matRotate, (float)rad );
	D3DXMatrixTranslation( &matTranslation, (float)dx - 0.5f, (float)dy - 0.5f, 0.0 );
	D3DXMatrixMultiply( &matComposition, &matRotate, &matTranslation );


	vp[ 0 ].x = -cx;
	vp[ 0 ].y = -cy;
	vp[ 1 ].x = -cx + dw;
	vp[ 1 ].y = -cy;
	vp[ 2 ].x = -cx;
	vp[ 2 ].y = -cy + dh;
	vp[ 3 ].x = -cx + dw;
	vp[ 3 ].y = -cy + dh;
	vp[ 0 ].z = vp[ 1 ].z = vp[ 2 ].z = vp[ 3 ].z = 0.0;
	vp[ 0 ].w = vp[ 1 ].w = vp[ 2 ].w = vp[ 3 ].w = 1.0;

	D3DXVec4Transform( &vp[ 0 ], &vp[ 0 ], &matComposition );
	D3DXVec4Transform( &vp[ 1 ], &vp[ 1 ], &matComposition );
	D3DXVec4Transform( &vp[ 2 ], &vp[ 2 ], &matComposition );
	D3DXVec4Transform( &vp[ 3 ], &vp[ 3 ], &matComposition );

	_cv[ 0 ].x = vp[ 0 ].x; _cv[ 0 ].y = vp[ 0 ].y;
	_cv[ 1 ].x = vp[ 1 ].x; _cv[ 1 ].y = vp[ 1 ].y;
	_cv[ 2 ].x = vp[ 2 ].x; _cv[ 2 ].y = vp[ 2 ].y;
	_cv[ 3 ].x = vp[ 3 ].x; _cv[ 3 ].y = vp[ 3 ].y;

	return 0;
}

// テクスチャの4点を自由に決めて描画する
int DirectXDrawBase::_DrawTextureVertex( float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3 )
{
	_cv[ 0 ].x = x0 - 0.5f; _cv[ 0 ].y = y0 - 0.5f;
	_cv[ 1 ].x = x1 - 0.5f; _cv[ 1 ].y = y1 - 0.5f;
	_cv[ 2 ].x = x2 - 0.5f; _cv[ 2 ].y = y2 - 0.5f;
	_cv[ 3 ].x = x3 - 0.5f; _cv[ 3 ].y = y3 - 0.5f;

	return 0;
}
