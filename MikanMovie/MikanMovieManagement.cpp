#include "MikanMovie.h"
#include "MikanMovieManagement.h"

extern int MoviePlayerEvent( class MikanMoviePlayer * mplayer, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

MikanMovieManagement_::MikanMovieManagement_( unsigned int max )
{
	playnow = 0;
	moviemax = max;
	movie = NULL;
}

MikanMovieManagement_::~MikanMovieManagement_( void )
{
	unsigned int i;
	for ( i = 0 ; i < moviemax ; ++i )
	{
		if ( movie[ 0 ] )
		{
			DeleteMikanMovie( movie[ i ] );
		}
	}
	delete( movie );
}

int MikanMovieManagement_::SecondInit( class DirectXDraw9 *mikandraw )
{
	unsigned int i;

	this->mikandraw = mikandraw;
	movie = ( class MikanMoviePlayer ** )calloc( moviemax, sizeof( class MikanMoviePlayer * ) );

	for ( i = 0 ; i < moviemax ; ++i )
	{
		movie[ i ] = CreateMikanMovie( mikandraw );
	}

	return 0;
}

int MikanMovieManagement_::Reset( void )
{
	unsigned int i;
	if ( playnow <= 0 )
	{
		return 0;
	}
	mikandraw->ResetRenderTarget();
	playnow = 0;
	for ( i = 0 ; i < moviemax ; ++i )
	{
		if ( IsPlay( i ) )
		{
			++playnow;
		}
	}
	return 0;
}

// ! ウィンドウメッセージの処理。
int MikanMovieManagement_::WinProcMovieFunc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	unsigned int i;
	for ( i = 0 ; i < moviemax ; ++i )
	{
		MoviePlayerEvent( movie[ i ], hWnd, uMsg, wParam, lParam );
	}
	return 0;
}

struct IDirect3DTexture9 * MikanMovieManagement_::GetTexture( unsigned int movnum )
{
	if ( moviemax <= movnum )
	{
		return NULL;
	}
	return movie[ movnum ]->GetTexture();
}

int MikanMovieManagement_::Lock( unsigned int movnum )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->Lock();
}

int MikanMovieManagement_::Unlock( unsigned int movnum )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->Unlock();
}

int MikanMovieManagement_::Load( unsigned int movnum, char *moviefile )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->Open( moviefile );
}

int MikanMovieManagement_::Play( unsigned int movnum, int loopplay )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	++playnow;
	return movie[ movnum ]->Play( loopplay );
}

int MikanMovieManagement_::Stop( unsigned int movnum )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->Stop();
}

int MikanMovieManagement_::Pause( unsigned int movnum )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->Pause();
}

int MikanMovieManagement_::IsPlay( unsigned int movnum )
{
	if ( moviemax <= movnum )
	{
		return 0;
	}
	return movie[ movnum ]->IsPlay();
}

long MikanMovieManagement_::Size( unsigned int movnum )
{
	if ( moviemax <= movnum )
	{
		return 0;
	}
	return movie[ movnum ]->Size();
}

long MikanMovieManagement_::Tell( unsigned int movnum )
{
	if ( moviemax <= movnum )
	{
		return 0;
	}
	return movie[ movnum ]->Tell();
}

int MikanMovieManagement_::Seek( unsigned int movnum, long seek )
{
	if ( moviemax <= movnum )
	{
		return 0;
	}
	return movie[ movnum ]->Seek( seek );
}


double MikanMovieManagement_::GetRate( unsigned int movnum )
{
	if ( moviemax <= movnum )
	{
		return 1.0;
	}
	return movie[ movnum ]->GetRate();
}

int MikanMovieManagement_::SetRate( unsigned int movnum, double rate )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->SetRate( rate );
}

int MikanMovieManagement_::GetWidth( unsigned int movnum )
{
	if ( moviemax <= movnum )
	{
		return 0;
	}
	return movie[ movnum ]->GetWidth();
}

int MikanMovieManagement_::GetHeight( unsigned int movnum )
{
	if ( moviemax <= movnum )
	{
		return 0;
	}
	return movie[ movnum ]->GetHeight();
}

int MikanMovieManagement_::SetColor( unsigned int movnum )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->SetColor();
}

int MikanMovieManagement_::SetColor( unsigned int movnum, unsigned long color )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->SetColor( color );
}

int MikanMovieManagement_::SetColor( unsigned int movnum, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->SetColor( color0, color1, color2, color3 );
}

int MikanMovieManagement_::SetAlpha( unsigned int movnum, unsigned char alpha )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->SetAlpha( alpha );
}

int MikanMovieManagement_::DrawMovie( unsigned int movnum, int dx, int dy, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovie( dx, dy, drawflag );
}

int MikanMovieManagement_::DrawMovie( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovie( dx, dy, rx, ry, w, h, drawflag );
}

int MikanMovieManagement_::DrawMovieC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovieC( dx, dy, rx, ry, w, h, drawflag );
}

int MikanMovieManagement_::DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovieScaling( dx, dy, rx, ry, w, h, scale, drawflag );
}

int MikanMovieManagement_::DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovieScaling( dx, dy, rx, ry, w, h, dw, dh, drawflag );
}

int MikanMovieManagement_::DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovieScalingC( dx, dy, rx, ry, w, h, scale, drawflag );
}

int MikanMovieManagement_::DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovieScalingC( dx, dy, rx, ry, w, h, dw, dh, drawflag );
}

int MikanMovieManagement_::DrawMovieRotationC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovieRotationC( dx, dy, rx, ry, w, h, rad, drawflag );
}

int MikanMovieManagement_::DrawMovieRotationAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovieRotationAngleC( dx, dy, rx, ry, w, h, angle, drawflag );
}

int MikanMovieManagement_::DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovieRotateScaleC( dx, dy, rx, ry, w, h, scale, rad, drawflag );
}

int MikanMovieManagement_::DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovieRotateScaleAngleC( dx, dy, rx, ry, w, h, scale, angle, drawflag );
}

int MikanMovieManagement_::DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovieRotateScaleC( dx, dy, rx, ry, w, h, sw, sh, rad, drawflag );
}

int MikanMovieManagement_::DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovieRotateScaleAngleC( dx, dy, rx, ry, w, h, sw, sh, angle, drawflag );
}

int MikanMovieManagement_::DrawMovieVertex( unsigned int movnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag )
{
	if ( moviemax <= movnum )
	{
		return -1;
	}
	return movie[ movnum ]->DrawMovieVertex( rx, ry, w, h, x0, y0, x1, y1, x2, y2, x3, y3, drawflag );
}
