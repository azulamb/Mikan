#include "DirectXDraw.h"

// 点を描画する。
int DirectXDraw9::DrawPixel( int x, int y, unsigned long color )
{
	float fx, fy, fw, fh;
	struct CUSTOMVERTEX dv[ 3 ];

	fx = (float)x - 0.5f;
	fy = (float)y - 0.5f;
	fw = (float)1;
	fh = (float)1;

#ifdef USERHW
	dv[ 0 ].rhw = dv[ 1 ].rhw = dv[ 2 ].rhw = /*dv[3].rhw =*/ 1.0f;
#endif
	dv[ 0 ].x = fx;        dv[ 0 ].y = fy;        dv[ 0 ].z = 0.5f; dv[ 0 ].color = color;// dv[0].u = 0.0; dv[0].v = 0.0;
	dv[ 1 ].x = fx + 1.5f; dv[ 1 ].y = fy;        dv[ 1 ].z = 0.5f; dv[ 1 ].color = color;// dv[1].u = 0.0; dv[1].v = 0.0;
	dv[ 2 ].x = fx;        dv[ 2 ].y = fy + 1.5f; dv[ 2 ].z = 0.5f; dv[ 2 ].color = color;// dv[2].u = 0.0; dv[2].v = 0.0;
	//  dv[3].x = fx + 1.0f; dv[3].y = fy + 1.0f; dv[3].z = 0.5f; dv[3].color = color;// dv[3].u = 0.0; dv[3].v = 0.0;

	D3DDev->SetTexture( 0, NULL );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );

	if ( FAILED( D3DDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 1, dv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	return 0;
}

/*!
@brief 線の描画
@par   関数説明
線を描画する。
@param sx 描画開始x座標。
@param sy 描画開始y座標。
@param ex 描画終了x座標。
@param ey 描画終了y座標。
@param color 線の色。省略すると白い線になる。
@param width 線の太さ。諸略すると1.0。
@param antialias アンチエイリアスをかけるならば1。省略すると0でアンチエイリアスはかからない。
@return 0を返す。
*/
int DirectXDraw9::DrawLine( int sx, int sy, int ex, int ey, unsigned long color, double width, int antialias )
{
	DX_RELEASE( line );
	D3DXCreateLine( D3DDev, &line );
	if ( line )
	{
		linevec[ 0 ].x = (float)sx;
		linevec[ 0 ].y = (float)sy;
		linevec[ 1 ].x = (float)ex;
		linevec[ 1 ].y = (float)ey;

		line->SetWidth( (float)width );
		line->SetAntialias( ( antialias ? true : false ) );
		line->Draw( linevec, 2, color );
	}

	return 0;
}

int DirectXDraw9::DrawBoxLine( int dx, int dy, int w, int h, unsigned long color, double width, int antialias )
{
	// ライン
	struct D3DXVECTOR2 linevec[ 5 ];
	struct ID3DXLine *line;

	D3DXCreateLine( D3DDev, &line );
	if ( line )
	{
		linevec[ 0 ].x = linevec[ 4 ].x = (float)dx;
		linevec[ 0 ].y = linevec[ 4 ].y = (float)dy;
		linevec[ 1 ].x = (float)( dx + w );
		linevec[ 1 ].y = (float)dy;
		linevec[ 2 ].x = (float)( dx + w );
		linevec[ 2 ].y = (float)( dy + h );
		linevec[ 3 ].x = (float)dx;
		linevec[ 3 ].y = (float)( dy + h );

		line->SetWidth( (float)width );
		line->SetAntialias( ( antialias ? true : false ) );
		line->Draw( linevec, 5, color );
	}

	DX_RELEASE( line );

	return 0;
}
int DirectXDraw9::DrawBoxLineC( int dx, int dy, int w, int h, unsigned long color, double width, int antialias )
{
	// ライン
	struct D3DXVECTOR2 linevec[ 5 ];
	struct ID3DXLine *line;
	double dw, dh;

	D3DXCreateLine( D3DDev, &line );
	if ( line )
	{
		dw = (double)w / 2.0;
		dh = (double)h / 2.0;
		linevec[ 0 ].x = linevec[ 4 ].x = (float)( dx - dw );
		linevec[ 0 ].y = linevec[ 4 ].y = (float)( dy - dh );
		linevec[ 1 ].x = (float)( dx + dw );
		linevec[ 1 ].y = (float)( dy - dh );
		linevec[ 2 ].x = (float)( dx + dw );
		linevec[ 2 ].y = (float)( dy + dh );
		linevec[ 3 ].x = (float)( dx - dw );
		linevec[ 3 ].y = (float)( dy + dh );

		line->SetWidth( (float)width );
		line->SetAntialias( ( antialias ? true : false ) );
		line->Draw( linevec, 5, color );
	}

	DX_RELEASE( line );

	return 0;
}

/*!
@brief 長方形の描画
@par   関数説明
長方形を描画する。
@param dx 描画x座標。
@param dy 描画y座標。
@param w 横幅。
@param h 高さ。
@param color 長方形の色。
@return 0を返す。
*/
int DirectXDraw9::DrawBox( int dx, int dy, int dw, int dh, unsigned long color )
{
	return DrawBox( dx, dy, dw, dh, color, color, color, color );
}

/*!
@brief 長方形の描画
@par   関数説明
長方形を描画する。各頂点に色を指定することでグラデーションをかける事ができる。
@param dx 描画x座標。
@param dy 描画y座標。
@param w 横幅。
@param h 高さ。
@param color1 左上の色。
@param color2 右上の色。
@param color3 左下の色。
@param color4 右下の色。
@return 0を返す。
*/
int DirectXDraw9::DrawBox( int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 )
{
	float fx, fy, fw, fh;
	struct CUSTOMVERTEX dv[ 4 ];

	fx = (float)dx - 0.5f;
	fy = (float)dy - 0.5f;
	fw = (float)w;
	fh = (float)h;

#ifdef USERHW
	dv[ 0 ].rhw = dv[ 1 ].rhw = dv[ 2 ].rhw = dv[ 3 ].rhw = 1.0f;
#endif
	dv[ 0 ].x = fx;      dv[ 0 ].y = fy;      dv[ 0 ].z = 0.5f; dv[ 0 ].color = color1; dv[ 0 ].u = 0.0; dv[ 0 ].v = 0.0;
	dv[ 1 ].x = fx + fw; dv[ 1 ].y = fy;      dv[ 1 ].z = 0.5f; dv[ 1 ].color = color2; dv[ 1 ].u = 0.0; dv[ 1 ].v = 0.0;
	dv[ 2 ].x = fx;      dv[ 2 ].y = fy + fh; dv[ 2 ].z = 0.5f; dv[ 2 ].color = color3; dv[ 2 ].u = 0.0; dv[ 2 ].v = 0.0;
	dv[ 3 ].x = fx + fw; dv[ 3 ].y = fy + fh; dv[ 3 ].z = 0.5f; dv[ 3 ].color = color4; dv[ 3 ].u = 0.0; dv[ 3 ].v = 0.0;

	D3DDev->SetTexture( 0, NULL );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );

	if ( FAILED( D3DDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, dv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	return 0;
}

int DirectXDraw9::DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color )
{
	return DrawBox( x0, y0, x1, y1, x2, y2, x3, y3, color, color, color, color );
}

int DirectXDraw9::DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 )
{
	struct CUSTOMVERTEX dv[ 4 ];

#ifdef USERHW
	dv[ 0 ].rhw = dv[ 1 ].rhw = dv[ 2 ].rhw = dv[ 3 ].rhw = 1.0f;
#endif
	dv[ 0 ].x = (float)x0; dv[ 0 ].y = (float)y0; dv[ 0 ].z = 0.5f; dv[ 0 ].color = color1; dv[ 0 ].u = 0.0; dv[ 0 ].v = 0.0;
	dv[ 1 ].x = (float)x1; dv[ 1 ].y = (float)y1; dv[ 1 ].z = 0.5f; dv[ 1 ].color = color2; dv[ 1 ].u = 0.0; dv[ 1 ].v = 0.0;
	dv[ 2 ].x = (float)x2; dv[ 2 ].y = (float)y2; dv[ 2 ].z = 0.5f; dv[ 2 ].color = color3; dv[ 2 ].u = 0.0; dv[ 2 ].v = 0.0;
	dv[ 3 ].x = (float)x3; dv[ 3 ].y = (float)y3; dv[ 3 ].z = 0.5f; dv[ 3 ].color = color4; dv[ 3 ].u = 0.0; dv[ 3 ].v = 0.0;

	D3DDev->SetTexture( 0, NULL );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );
	if ( FAILED( D3DDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, dv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	return 0;
}

/*!
@brief 長方形の描画
@par   関数説明
長方形を中心を基準に描画する。
@param dx 描画x座標。
@param dy 描画y座標。
@param w 横幅。
@param h 高さ。
@param color 長方形の色。
@return 0を返す。
*/
int DirectXDraw9::DrawBoxC( int dx, int dy, int dw, int dh, unsigned long color )
{
	return DrawBoxC( dx, dy, dw, dh, color, color, color, color );
}

/*!
@brief 長方形の描画
@par   関数説明
長方形を中心を基準に描画する。各頂点に色を指定することでグラデーションをかける事ができる。
@param dx 描画x座標。
@param dy 描画y座標。
@param w 横幅。
@param h 高さ。
@param color1 左上の色。
@param color2 右上の色。
@param color3 左下の色。
@param color4 右下の色。
@return 0を返す。
*/
int DirectXDraw9::DrawBoxC( int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 )
{
	float fx, fy, fw, fh;
	struct CUSTOMVERTEX dv[ 4 ];

	fx = (float)dx - 0.5f;
	fy = (float)dy - 0.5f;
	fw = (float)w;
	fh = (float)h;

#ifdef USERHW
	dv[ 0 ].rhw = dv[ 1 ].rhw = dv[ 2 ].rhw = dv[ 3 ].rhw = 1.0f;
#endif
	dv[ 0 ].x = fx - fw / 2.0f; dv[ 0 ].y = fy - fh / 2.0f; dv[ 0 ].z = 0.5f; dv[ 0 ].color = color1; dv[ 0 ].u = 0.0; dv[ 0 ].v = 0.0;
	dv[ 1 ].x = fx + fw / 2.0f; dv[ 1 ].y = fy - fh / 2.0f; dv[ 1 ].z = 0.5f; dv[ 1 ].color = color2; dv[ 1 ].u = 0.0; dv[ 1 ].v = 0.0;
	dv[ 2 ].x = fx - fw / 2.0f; dv[ 2 ].y = fy + fh / 2.0f; dv[ 2 ].z = 0.5f; dv[ 2 ].color = color3; dv[ 2 ].u = 0.0; dv[ 2 ].v = 0.0;
	dv[ 3 ].x = fx + fw / 2.0f; dv[ 3 ].y = fy + fh / 2.0f; dv[ 3 ].z = 0.5f; dv[ 3 ].color = color4; dv[ 3 ].u = 0.0; dv[ 3 ].v = 0.0;

	D3DDev->SetTexture( 0, NULL );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );

	if ( FAILED( D3DDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, dv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	return 0;
}

int DirectXDraw9::DrawCircleLineC( int x, int y, int r, unsigned long color, double width, int antialias )
{
	int i;
	const unsigned int n = 96;
	double arad;
	// ライン
	struct D3DXVECTOR2 linevec[ n + 1 ];
	struct ID3DXLine *line;

	D3DXCreateLine( D3DDev, &line );
	if ( line )
	{
		arad = AngleToRadian( 360.0 / (double)n );
		for ( i = 0; i <= n; ++i )
		{
			linevec[ i ].x = (float)( (double)x + cos( arad * i ) * (double)r );
			linevec[ i ].y = (float)( (double)y + sin( arad * i ) * (double)r );
		}
		line->SetWidth( (float)width );
		line->SetAntialias( ( antialias ? true : false ) );
		line->Draw( linevec, n + 1, color );
	}

	DX_RELEASE( line );

	return 0;
}


// 円の描画
int DirectXDraw9::DrawCircleC( int x, int y, int r, unsigned long color )
{
	double arad;
	struct CUSTOMVERTEX *dv;
	unsigned int i;
	const unsigned int n = 96;

	dv = ( struct CUSTOMVERTEX * )calloc( n + 2, sizeof( struct CUSTOMVERTEX ) );

	// 中央座標
	dv[ 0 ].x = (float)x;
	dv[ 0 ].y = (float)y;
	dv[ 0 ].z = 0.5f;
	dv[ 0 ].color = color;
#ifdef USERHW
	dv[ 0 ].rhw = 1.0f;
#endif
	dv[ 0 ].u = 0.0f;
	dv[ 0 ].v = 0.0f;

	arad = AngleToRadian( 360.0 / (double)n );

	for ( i = 0; i <= n; ++i )
	{
		dv[ i + 1 ].x = (float)( x + cos( arad * i ) * r );
		dv[ i + 1 ].y = (float)( y + sin( arad * i ) * r );
		dv[ i + 1 ].z = 0.5f;
		dv[ i + 1 ].color = color;
#ifdef USERHW
		dv[ i + 1 ].rhw = 1.0f;
#endif
		dv[ i + 1 ].u = 0.0f;
		dv[ i + 1 ].v = 0.0f;
	}

	D3DDev->SetTexture( 0, NULL );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );
	if ( FAILED( D3DDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, n, dv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	DX_FREE( dv );
	return 0;
}

int DirectXDraw9::DrawCircleLine( int x, int y, int w, int h, unsigned long color, double width, int antialias )
{
	int i;
	const unsigned int n = 96;
	double arad, dw, dh, cx, cy;
	// ライン
	struct D3DXVECTOR2 linevec[ n + 1 ];
	struct ID3DXLine *line;

	D3DXCreateLine( D3DDev, &line );
	if ( line )
	{
		arad = AngleToRadian( 360.0 / (double)n );
		dw = (double)w / 2.0;
		dh = (double)h / 2.0;
		cx = (double)x + dw;
		cy = (double)y + dh;
		for ( i = 0; i <= n; ++i )
		{
			linevec[ i ].x = (float)( cx + cos( arad * i ) * dw );
			linevec[ i ].y = (float)( cy + sin( arad * i ) * dh );
		}
		line->SetWidth( (float)width );
		line->SetAntialias( ( antialias ? true : false ) );
		line->Draw( linevec, n + 1, color );
	}

	DX_RELEASE( line );

	return 0;
}


// 楕円かもしれない描画
int DirectXDraw9::DrawCircle( int x, int y, int w, int h, unsigned long color )
{
	double arad, cx, cy, dw, dh;
	struct CUSTOMVERTEX *dv;
	unsigned int i;
	unsigned int n = 96;

	dw = (double)w / 2.0;
	dh = (double)h / 2.0;
	cx = (double)x + dw;
	cy = (double)y + dh;

	dv = ( struct CUSTOMVERTEX * )calloc( n + 2, sizeof( struct CUSTOMVERTEX ) );

	// 中央座標
	dv[ 0 ].x = (float)cx;
	dv[ 0 ].y = (float)cy;
	dv[ 0 ].z = 0.5f;
	dv[ 0 ].color = color;
#ifdef USERHW
	dv[ 0 ].rhw = 1.0f;
#endif
	dv[ 0 ].u = 0.0f;
	dv[ 0 ].v = 0.0f;

	arad = AngleToRadian( 360.0 / (double)n );

	for ( i = 0; i <= n; ++i )
	{
		dv[ i + 1 ].x = (float)( cx + cos( arad * i ) * dw );
		dv[ i + 1 ].y = (float)( cy + sin( arad * i ) * dh );
		dv[ i + 1 ].z = 0.5f;
		dv[ i + 1 ].color = color;
#ifdef USERHW
		dv[ i + 1 ].rhw = 1.0f;
#endif
		dv[ i + 1 ].u = 0.0f;
		dv[ i + 1 ].v = 0.0f;
	}

	D3DDev->SetTexture( 0, NULL );
	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );
	if ( FAILED( D3DDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, n, dv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	DX_FREE( dv );
	return 0;
}

