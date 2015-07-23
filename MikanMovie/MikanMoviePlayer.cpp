#include "MikanMoviePlayer.h"
#include "CriticalLock.h"

// TODO:
// 動画の再生回数を指定する命令欲しい。

class CriticalLock * MikanMoviePlayer_::lock = NULL;

MikanMoviePlayer_::MikanMoviePlayer_( void )
{
	builder = NULL;
	control = NULL;
	seeking = NULL;
	eventex = NULL;

	vmrfilter = NULL;
	sourcefilter = NULL;

	SurfaceAllocator = NULL;
	moviefile = NULL;

	// COMの初期化
	hr = CoInitialize( NULL );

	hr = CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&builder );

	//hr = builder->QueryInterface( IID_IMediaControl, (void **)&control );
	//hr = builder->QueryInterface( IID_IMediaSeeking, (void **)&seeking );
	//hr = builder->QueryInterface( IID_IMediaEventEx, (void **)&eventex );

	hr = CoCreateInstance( CLSID_VideoMixingRenderer9, 0, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&vmrfilter );

	play = false;

	SetColor();

	endpoint = nowpoint = 0;

	if ( lock == NULL )
	{
		// 本当に初回だけ。
		lock = new CriticalLock();
	}
}

MikanMoviePlayer_::~MikanMoviePlayer_( void )
{
	if ( IsPlay() )
	{
		Stop();
		while ( IsPlay() )
		{
			Sleep( 1 );
		}
	}

	if ( lock )
	{
		lock->Unlock();
		delete( lock );
		lock = NULL;
	}
	if ( free )
	{
		free( moviefile );
	}

	if ( control )
	{
		control->Release();
	}
	if ( seeking )
	{
		seeking->Release();
	}
	if ( eventex )
	{
		eventex->Release();
	}
	SurfaceAllocator->Release();
	builder->Release();
	vmrfilter->Release();

	CoUninitialize();
}

int MikanMoviePlayer_::Init( class DirectXDraw9 *mikandraw )
{
	mdraw = mikandraw;

	// Renderlessモードへ。
	IVMRFilterConfig9 *FilterConfig;
	hr = vmrfilter->QueryInterface( IID_IVMRFilterConfig9, (void **)&FilterConfig );

	hr = FilterConfig->SetNumberOfStreams( 1 );
	hr = FilterConfig->SetRenderingMode( VMR9Mode_Renderless );
	hr = FilterConfig->Release();

	IVMRSurfaceAllocator9 *SurfaceAllocator9;
	IVMRSurfaceAllocatorNotify9 *SurfaceAllocNotify;
	IVMRImagePresenter9 *ImagePresenter9;

	SurfaceAllocator = new VMRSurfaceAllocator( mikandraw, lock );

	hr = vmrfilter->QueryInterface( IID_IVMRSurfaceAllocatorNotify9, (void **)&SurfaceAllocNotify );
	hr = SurfaceAllocator->QueryInterface( IID_IVMRSurfaceAllocator9, (void **)&SurfaceAllocator9 );
	hr = SurfaceAllocator->QueryInterface( IID_IVMRImagePresenter9, (void **)&ImagePresenter9 );

	hr = SurfaceAllocNotify->AdviseSurfaceAllocator( NULL, SurfaceAllocator9 );
	hr = SurfaceAllocator9->AdviseNotify( SurfaceAllocNotify );

	ImagePresenter9->Release();
	SurfaceAllocator9->Release();
	SurfaceAllocNotify->Release();

	return 0;
}

int MikanMoviePlayer_::Event( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	long evCode;
	LONG param1, param2;

	if ( endpoint == 0 )
	{
		return -1;
	}

	// イベントを全て取得
	while ( SUCCEEDED( eventex->GetEvent( &evCode, &param1, &param2, 0 ) ) )
	{
		eventex->FreeEventParams( evCode, param1, param2 );
		switch ( evCode )
		{
		case EC_COMPLETE:
			// 再生終了
			Stop();
			if ( loopplay )
			{
				Play();
			}
			break;
		}
	}
	return 0;
}


struct IDirect3DTexture9 * MikanMoviePlayer_::GetTexture( void )
{
	return SurfaceAllocator->GetDrawTexture();
}

int MikanMoviePlayer_::Lock( void )
{
	return lock->Lock();
}

int MikanMoviePlayer_::Unlock( void )
{
	return lock->Unlock();
}

int MikanMoviePlayer_::Open( char *moviefile )
{
	errno_t errno;
	size_t size, buflen;

	buflen = strlen( moviefile ) + 1;
	this->moviefile = (WCHAR *)calloc( buflen, sizeof( WCHAR ) );

	mbstowcs_s( &size, this->moviefile, buflen, moviefile, _TRUNCATE );

	builder->AddSourceFilter( this->moviefile, L"Source", &sourcefilter );

	builder->AddFilter( vmrfilter, L"Video Mixing Renderer9" );
	hr = builder->RenderFile( this->moviefile, NULL );

	hr = builder->QueryInterface( IID_IMediaControl, (void **)&control );
	hr = builder->QueryInterface( IID_IMediaSeeking, (void **)&seeking );
	hr = builder->QueryInterface( IID_IMediaEventEx, (void **)&eventex );

	eventex->SetNotifyWindow( (OAHWND)mdraw->GetWindowHandle(), WM_GRAPH_NOTIFY, NULL );

	seeking->SetTimeFormat( &TIME_FORMAT_MEDIA_TIME );
	hr = seeking->GetStopPosition( &endpoint );//GetDuration( &endpoint );
	if ( hr != S_OK )
	{
		endpoint = 0;
		return 1;
	}

	return 0;
}

int MikanMoviePlayer_::Play( int loop )
{
	//long eventcode;
	play = 1;
	loopplay = loop;

	hr = control->Run();

	//eventex->WaitForCompletion( INFINITE, &eventcode );

	return 0;
}

int MikanMoviePlayer_::Stop( void )
{

	Pause();

	Seek( 0 );
	return 0;
}

int MikanMoviePlayer_::Pause( void )
{
	if ( endpoint == 0 )
	{
		return -1;
	}
	control->Stop();
	play = 0;

	return 0;
}

int MikanMoviePlayer_::IsPlay( int getstate )
{
	OAFilterState state;
	if ( endpoint == 0 )
	{
		return 0;
	}
	if ( getstate )
	{
		control->GetState( 2, &state );
		return ( state == State_Running );
	}
	return play;
}

long MikanMoviePlayer_::Size( void )
{
	return (long)( endpoint / 10000 );
}

long MikanMoviePlayer_::Tell( void )
{
	if ( endpoint == 0 )
	{
		return 0;
	}
	hr = seeking->GetCurrentPosition( &nowpoint );
	if ( hr != S_OK )
	{
		return -1;
	}
	return (long)( nowpoint / 10000 );
}

int MikanMoviePlayer_::Seek( long seek )
{
	long long newnow;

	if ( endpoint == 0 )
	{
		return -1;
	}

	newnow = seek * 10000;
	hr = seeking->SetPositions( &newnow, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning );
	if ( hr != D3D_OK )
	{
		return -1;
	}

	return 0;
}

double MikanMoviePlayer_::GetRate( void )
{
	double rate = 1.0;
	if ( endpoint == 0 )
	{
		return 1.0;
	}
	seeking->GetRate( &rate );
	return rate;
}

int MikanMoviePlayer_::SetRate( double rate )
{
	if ( endpoint == 0 )
	{
		return -1;
	}
	return seeking->SetRate( rate );
}

int MikanMoviePlayer_::GetWidth( void )
{
	return SurfaceAllocator ? SurfaceAllocator->GetTextureWidth() : 0;
}

int MikanMoviePlayer_::GetHeight( void )
{
	return SurfaceAllocator ? SurfaceAllocator->GetTextureHeight() : 0;
}

int MikanMoviePlayer_::SetColor( void )
{
	color[ 0 ] = color[ 1 ] = color[ 2 ] = color[ 3 ] = 0xFFFFFFFF;
	return 0;
}

int MikanMoviePlayer_::SetColor( unsigned long scolor )
{
	color[ 0 ] = color[ 1 ] = color[ 2 ] = color[ 3 ] = scolor;
	return 0;
}

int MikanMoviePlayer_::SetColor( unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 )
{
	color[ 0 ] = color0;
	color[ 1 ] = color1;
	color[ 2 ] = color2;
	color[ 3 ] = color3;
	return 0;
}

int MikanMoviePlayer_::SetAlpha( unsigned char alpha )
{
	color[ 0 ] = ( color[ 0 ] & 0x00ffffff ) | alpha << 24;
	color[ 1 ] = ( color[ 1 ] & 0x00ffffff ) | alpha << 24;
	color[ 2 ] = ( color[ 2 ] & 0x00ffffff ) | alpha << 24;
	color[ 3 ] = ( color[ 3 ] & 0x00ffffff ) | alpha << 24;
	return 0;
}
