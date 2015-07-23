#include "MikanMovie.h"
#include "MikanMoviePlayer.h"
#include "CriticalLock.h"

// テクスチャ描画の終了
int MikanMoviePlayer_::EndDrawMovie( IDirect3DTexture9 *tex )
{
	HRESULT hr;
	hr = mdraw->D3DDev->SetTexture( 0, tex );

	mdraw->D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D );

	if ( FAILED( mdraw->D3DDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, mdraw->_cv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	return 0;
}

int MikanMoviePlayer_::DrawMovie( int dx, int dy, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), 0, 0, SurfaceAllocator->GetMovieWidth(), SurfaceAllocator->GetMovieHeight(), drawflag );
	mdraw->_SetColor( color );
	mdraw->_DrawTexture( (float)dx, (float)dy, (float)SurfaceAllocator->GetMovieWidth(), (float)SurfaceAllocator->GetMovieHeight() );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

int MikanMoviePlayer_::DrawMovie( int dx, int dy, int rx, int ry, int w, int h, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );
	mdraw->_DrawTexture( (float)dx, (float)dy, (float)w, (float)h );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

int MikanMoviePlayer_::DrawMovieC( int dx, int dy, int rx, int ry, int w, int h, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );

	mdraw->_DrawTextureC( (float)dx, (float)dy, (float)w, (float)h );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

int MikanMoviePlayer_::DrawMovieScaling( int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );

	mdraw->_DrawTextureScaling( (float)dx, (float)dy, (float)( scale * w ), (float)( scale * h ) );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

int MikanMoviePlayer_::DrawMovieScaling( int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );

	mdraw->_DrawTextureScaling( (float)dx, (float)dy, (float)dw, (float)dh );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

int MikanMoviePlayer_::DrawMovieScalingC( int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );

	mdraw->_DrawTextureScalingC( (float)dx, (float)dy, (float)( w * scale ), (float)( h * scale ) );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

int MikanMoviePlayer_::DrawMovieScalingC( int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );

	mdraw->_DrawTextureScalingC( (float)dx, (float)dy, (float)dw, (float)dh );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

int MikanMoviePlayer_::DrawMovieRotationC( int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );

	mdraw->_DrawTextureRotationC( (float)dx, (float)dy, (float)w, (float)h, rad );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

int MikanMoviePlayer_::DrawMovieRotationAngleC( int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );

	mdraw->_DrawTextureRotationC( (float)dx, (float)dy, (float)w, (float)h, AngleToRadian( angle ) );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

/*int MikanMoviePlayer_::DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag )
{
int ret;

mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
mdraw->_SetColor( color );

mdraw->_DrawTextureRotateScale( (float)(cx * scale), (float)(cy * scale), (float)dx, (float)dy, rad, (float)(w * scale), (float)(h * scale) );

lock->Lock();
ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
lock->Unlock();

return ret;
}

int MikanMoviePlayer_::DrawMovieRotateScale( int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag )
{
int ret;

mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
mdraw->_SetColor( color );

mdraw->_DrawTextureRotateScale( (float)cx, (float)cy, (float)dx, (float)dy, rad, (float)dw, (float)dh );

lock->Lock();
ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
lock->Unlock();

return ret;
}*/

int MikanMoviePlayer_::DrawMovieRotateScaleC( int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );

	mdraw->_DrawTextureRotateScale( (float)( w * scale * 0.5 ), (float)( h * scale * 0.5 ), (float)dx, (float)dy, rad, (float)( w * scale ), (float)( h * scale ) );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

int MikanMoviePlayer_::DrawMovieRotateScaleAngleC( int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );

	mdraw->_DrawTextureRotateScale( (float)( w * scale * 0.5 ), (float)( h * scale * 0.5 ), (float)dx, (float)dy, AngleToRadian( angle ), (float)( w * scale ), (float)( h * scale ) );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

int MikanMoviePlayer_::DrawMovieRotateScaleC( int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );

	mdraw->_DrawTextureRotateScale( (float)( sw * 0.5 ), (float)( sh * 0.5 ), (float)( dx ), (float)( dy ), rad, (float)( sw ), (float)( sh ) );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

int MikanMoviePlayer_::DrawMovieRotateScaleAngleC( int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );

	mdraw->_DrawTextureRotateScale( (float)( sw * 0.5 ), (float)( sh * 0.5 ), (float)dx, (float)dy, AngleToRadian( angle ), (float)sw, (float)sh );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}

int MikanMoviePlayer_::DrawMovieVertex( int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag )
{
	int ret;

	mdraw->_SetUV( (float)SurfaceAllocator->GetTextureWidth(), (float)SurfaceAllocator->GetTextureHeight(), rx, ry, w, h, drawflag );
	mdraw->_SetColor( color );

	mdraw->_DrawTextureVertex( (float)x0, (float)y0, (float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3 );

	Lock();
	ret = EndDrawMovie( SurfaceAllocator->GetDrawTexture() );
	Unlock();

	return ret;
}
