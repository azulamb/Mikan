#include "MikanMovie.h"
#include "MikanMoviePlayer.h"

class MikanMoviePlayer_ *MikanMovie;

int MoviePlayerEvent( class MikanMoviePlayer * mplayer, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return ( ( class MikanMoviePlayer_ * )mplayer )->Event( hWnd, uMsg, wParam, lParam );
}

class MikanMoviePlayer * CreateMikanMovie( class DirectXDraw9 *mikandraw )
{
	MikanMovie = new MikanMoviePlayer_();
	MikanMovie->Init( mikandraw );

	return MikanMovie;
}

int DeleteMikanMovie( class MikanMoviePlayer *mikanmovie )
{
	delete( ( class MikanMoviePlayer_ * )mikanmovie );
	return 0;
}

