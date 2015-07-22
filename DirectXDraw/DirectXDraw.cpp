/*!
@class   DirectXDraw DirectXDraw.h "DirectXDraw.h"
@brief   _MikanDrawクラス。DirectXを用いた描画を行う。
@author  Hiroki
@version 1.0
*/
#include "DirectXDraw.h"

// テクスチャ描画の終了
int DirectXDraw9::EndDrawTexture( IDirect3DTexture9 *tex )
{
	hr = D3DDev->SetTexture( 0, tex );

#ifdef _DEBUG
	if ( FAILED( hr ) )
	{
		//テクスチャセットに失敗
		return 3;
	}
#endif

	//  D3DDev->SetTextureStageState( 0, D3DTSS_COLORARG2,D3DTA_DIFFUSE );
	/*  if( FAILED( hr ) )
	  {
	  //テクスチャ設定に失敗
	  return 4;
	  }*/

	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );

#ifdef _DEBUG
	if ( FAILED( hr ) )
	{
		//頂点ストリームの設定に失敗
		return 5;
	}
#endif

	if ( FAILED( D3DDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, _cv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	return 0;
}

/*!
@brief テクスチャの描画
@par   関数説明
テクスチャを描画する。始点は左上。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int DirectXDraw9::DrawTexture( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTexture( (float)dx, (float)dy, (float)w, (float)h );

	return EndDrawTexture( dxtex[ txnum ].tex );
}

/*!
@brief テクスチャの描画
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int DirectXDraw9::DrawTextureC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureC( (float)dx, (float)dy, (float)w, (float)h );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

/*!
@brief テクスチャの拡大縮小描画
@par   関数説明
テクスチャを描画する。始点は左上。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param scale 拡大縮小率。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int DirectXDraw9::DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureScaling( (float)dx, (float)dy, (float)( scale * w ), (float)( scale * h ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

/*!
@brief テクスチャの拡大縮小描画
@par   関数説明
テクスチャを描画する。始点は左上。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param dw 描画横幅。
@param dh 描画高さ。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int DirectXDraw9::DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureScaling( (float)dx, (float)dy, (float)dw, (float)dh );
	return EndDrawTexture( dxtex[ txnum ].tex );
}


/*!
@brief テクスチャの拡大縮小描画
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param scale 拡大縮小率。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int DirectXDraw9::DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureScalingC( (float)dx, (float)dy, (float)( w * scale ), (float)( h * scale ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

/*!
@brief テクスチャの拡大縮小描画
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param dw 描画横幅。
@param dh 描画高さ。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int DirectXDraw9::DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureScalingC( (float)dx, (float)dy, (float)dw, (float)dh );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

/*!
@brief テクスチャを回転描画する(ラジアン)
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param rad ラジアン。時計回り。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int DirectXDraw9::DrawTextureRotationC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotationC( (float)dx, (float)dy, (float)w, (float)h, rad );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

/*!
@brief テクスチャを回転描画する(角度)
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param angle 角度。時計回り。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int DirectXDraw9::DrawTextureRotationAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotationC( (float)dx, (float)dy, (float)w, (float)h, AngleToRadian( angle ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

// テクスチャを回転拡大描画する
int DirectXDraw9::DrawTextureRotateScale( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag )
{
	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, drawflag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotateScale( (float)( cx * scale ), (float)( cy * scale ), (float)dx, (float)dy, rad, (float)( w * scale ), (float)( h * scale ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

// テクスチャを回転拡大描画する
int DirectXDraw9::DrawTextureRotateScale( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag )
{
	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, drawflag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotateScale( (float)cx, (float)cy, (float)dx, (float)dy, rad, (float)dw, (float)dh );
	return EndDrawTexture( dxtex[ txnum ].tex );
}


//テクスチャを回転拡大描画する
int DirectXDraw9::DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int flag )
{
	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotateScale( (float)( w * scale * 0.5 ), (float)( h * scale * 0.5 ), (float)dx, (float)dy, rad, (float)( w * scale ), (float)( h * scale ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

//テクスチャを回転拡大描画する
int DirectXDraw9::DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int flag )
{
	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotateScale( (float)( w * scale * 0.5 ), (float)( h * scale * 0.5 ), (float)dx, (float)dy, AngleToRadian( angle ), (float)( w * scale ), (float)( h * scale ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

//テクスチャを回転拡大描画する
int DirectXDraw9::DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int flag )
{
	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotateScale( (float)( sw * 0.5 ), (float)( sh * 0.5 ), (float)( dx ), (float)( dy ), rad, (float)( sw ), (float)( sh ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

//テクスチャを回転拡大描画する
int DirectXDraw9::DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int flag )
{
	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotateScale( (float)( sw * 0.5 ), (float)( sh * 0.5 ), (float)dx, (float)dy, AngleToRadian( angle ), (float)sw, (float)sh );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

//テクスチャの4点を自由に決めて描画する
int DirectXDraw9::DrawTextureVertex( unsigned int txnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int flag )
{
	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureVertex( (float)x0, (float)y0, (float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3 );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

//テクスチャの4点を相対座標で自由に決めて描画する
int DirectXDraw9::DrawTextureRelativeVertex( unsigned int txnum, int rx, int ry, int w, int h, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3, int flag )
{
	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureVertex( (float)x0, (float)y0, (float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3 );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

//テクスチャの4点を相対座標で自由に決めて描画する(テクスチャチェックなし:内部用)
int DirectXDraw9::DrawTextureRelativeVertex( unsigned int txnum, int rx, int ry, int w, int h, double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, int drawflag )
{
	// TODO: Z座標。

	if ( txnum >= texmax )
	{
		//テクスチャ番号が不正
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, drawflag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureVertex( (float)x0, (float)y0, (float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3 );
	return EndDrawTexture( dxtex[ txnum ].tex );
}
