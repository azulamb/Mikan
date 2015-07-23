#include "MikanMovie.h"
#include "MikanVMRSurfaceAllocator.h"
#include "CriticalLock.h"

#include <locale.h>



////////////////////////////////////////////////////////////////////////////////
// VMRSurfaceAllocator
////////////////////////////////////////////////////////////////////////////////

VMRSurfaceAllocator::VMRSurfaceAllocator( class DirectXDraw9 *mikandraw, class CriticalLock *clock )
	: CUnknown( NAME( "VMRSurfaceAllocator" ), reinterpret_cast<LPUNKNOWN>( static_cast<PNDUNKNOWN>( this ) ) )
{
	int i;
	mdraw = mikandraw;
	D3DDev = mdraw->GetDirect3DDevice();
	D3D = mdraw->D3D;
	//D3DDev->GetDirect3D( &D3D );

	//D3DDev->AddRef();
	count = 1;

	this->clock = clock;

	mwidth = mheight = twidth = theight = 0;

	for ( i = 0 ; i < VMRSALLOC_USE_TEXTURE_MAX ; ++i )
	{
		m_alpDirect3DTexture[ i ] = NULL;
	}

	texnum = 0;
}

VMRSurfaceAllocator::~VMRSurfaceAllocator( void )
{
	// TODO? D3DDev->Release();
	//D3D->Release();
	Release();
}

STDMETHODIMP VMRSurfaceAllocator::NonDelegatingQueryInterface( REFIID refiid, void **lplpInterface )
{
	if ( IsEqualIID( refiid, IID_IVMRSurfaceAllocator9 ) )
	{
		return GetInterface( static_cast<IVMRSurfaceAllocator9 *>( this ), lplpInterface );
	} else if ( IsEqualIID( refiid, IID_IVMRImagePresenter9 ) )
	{
		return GetInterface( static_cast<IVMRImagePresenter9 *>( this ), lplpInterface );
	}

	return CUnknown::NonDelegatingQueryInterface( refiid, lplpInterface );
}

STDMETHODIMP VMRSurfaceAllocator::AdviseNotify( IVMRSurfaceAllocatorNotify9 *lpVMRSurfaceAllocNotify )
{
	HMONITOR hMonitor;
	D3DCAPS9 d3dCaps;
	SurfaceAllocNotify = lpVMRSurfaceAllocNotify;
	D3DDev->GetDeviceCaps( &d3dCaps );
	hMonitor = D3D->GetAdapterMonitor( d3dCaps.AdapterOrdinal );
	return SurfaceAllocNotify->SetD3DDevice( D3DDev, hMonitor );
}

STDMETHODIMP VMRSurfaceAllocator::StartPresenting( DWORD_PTR dwUserID )
{
	if ( D3DDev != NULL )
	{
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP VMRSurfaceAllocator::StopPresenting( DWORD_PTR dwUserID )
{
	return S_OK;
}

STDMETHODIMP VMRSurfaceAllocator::GetSurface( DWORD_PTR dwUserID, DWORD dwSurfaceIndex, DWORD dwSurfaceFlags, IDirect3DSurface9 far **lplpSurface )
{
	// 恐らくこの後RenderTargetが変更される。
	// それを避けてMainLoopの終わりまで待つことで画面のちらつきを抑えることにした。
	// ちょっと前の実装で無限待ちだとデッドロックしたので次第5ms待つ。
	mdraw->WaitDrawEnd( 1, 5 );

	CheckPointer( lplpSurface, E_POINTER );

	if ( dwSurfaceIndex >= Surfaces.Size() )
	{
		return E_FAIL;
	}

	*lplpSurface = Surfaces.Get( dwSurfaceIndex );
	if ( *lplpSurface == NULL )
	{
		return E_POINTER;
	}

	( *lplpSurface )->AddRef();

	return S_OK;
}

STDMETHODIMP VMRSurfaceAllocator::PresentImage( DWORD_PTR dwUserID, VMR9PresentationInfo *lpPresInfo )
{
	HRESULT hr = 0;
	//IDirect3DTexture9 *lpTexture;
	CheckPointer( lpPresInfo, E_POINTER );
	CheckPointer( lpPresInfo->lpSurf, E_POINTER );

	//while( lock )
	//{
	//  Sleep( 1 );
	//}

	mdraw->WaitDrawEnd( 1, 16 );

	hr = PresentToTexture( lpPresInfo );

	//if(hr == D3D_OK)
	//{
	//  if ( texnum < 0 )
	//  {
	//    hr = lpPresInfo->lpSurf->GetContainer(IID_IDirect3DTexture9,(void **)&lpTexture);
	//  } else
	//  {
	//    lpTexture = m_alpDirect3DTexture[ texnum ];
	//    lpTexture->AddRef();
	//    hr = D3D_OK;
	//  }

	//  if ( hr == D3D_OK )
	//  {
	//    //...ここで必要であればテクスチャを使っての描画処理を行う
	//    lpTexture->Release();
	//  }
	//}

	if ( hr == D3DERR_DEVICELOST )
	{
		OnLostDevice();
		hr = D3DDev->TestCooperativeLevel();
		if ( hr == D3DERR_DEVICENOTRESET )
		{
			OnResetDevice();
		}
	}

	//mdraw->UnlockDarawEnd();

	return hr;
}

HRESULT VMRSurfaceAllocator::PresentToTexture( VMR9PresentationInfo *lpPresInfo )
{
	HRESULT hr;
	IDirect3DTexture9 *lpTexture;
	IDirect3DSurface9 *lpSurface;

	if ( m_alpDirect3DTexture[ 0 ] != NULL )
	{
		clock->Lock();
		lpTexture = m_alpDirect3DTexture[ m_nFilpTexNum ];
		hr = lpTexture->GetSurfaceLevel( 0, &lpSurface );
		if ( hr != S_OK )
		{
			return hr;
		}

		hr = D3DDev->StretchRect( lpPresInfo->lpSurf, NULL, lpSurface, NULL, D3DTEXF_NONE );
		if ( hr != S_OK )
		{
			lpSurface->Release();
			return hr;
		}
		lpSurface->Release();
		texnum = m_nFilpTexNum;
		m_nFilpTexNum = m_nFilpTexNum ^ 0x01;
		clock->Unlock();
	} else
	{
		m_nFilpTexNum = -1;
	}

	return S_OK;
}

STDMETHODIMP VMRSurfaceAllocator::InitializeDevice( DWORD_PTR dwUserID, VMR9AllocationInfo *lpAllocInfo, DWORD *lpNumBuffers )
{
	IDirect3DTexture9 *lpTexture;
	D3DCAPS9 d3dcaps;
	D3DDISPLAYMODE d3ddm;
	HRESULT hr;
	unsigned long ulWidth, ulHeight;//, ul;
	unsigned int i;

	CheckPointer( lpAllocInfo, E_POINTER );
	CheckPointer( lpNumBuffers, E_POINTER );
	CheckPointer( SurfaceAllocNotify, E_FAIL );

	mwidth = ulWidth = lpAllocInfo->dwWidth;
	mheight = ulHeight = lpAllocInfo->dwHeight;
	m_fmtTexture = lpAllocInfo->Format;

	if ( m_fmtTexture == D3DFMT_UNKNOWN )
	{
		hr = D3DDev->GetDisplayMode( NULL, &d3ddm );
		if ( hr != S_OK )
		{
			return hr;
		}
		lpAllocInfo->Format = m_fmtTexture = d3ddm.Format;
	}

	hr = D3DDev->GetDeviceCaps( &d3dcaps );
	if ( d3dcaps.TextureCaps & D3DPTEXTURECAPS_POW2 )
	{
		for ( ulWidth = 1 ; ulWidth < lpAllocInfo->dwWidth ; ulWidth <<= 1 );
		for ( ulHeight = 1 ; ulHeight < lpAllocInfo->dwHeight ; ulHeight <<= 1 );
		lpAllocInfo->dwWidth = ulWidth;
		lpAllocInfo->dwHeight = ulHeight;
	}

	lpAllocInfo->dwFlags |= VMR9AllocFlag_TextureSurface;
	DeleteSurface();
	Surfaces.Resize( *lpNumBuffers );

	hr = SurfaceAllocNotify->AllocateSurfaceHelper( lpAllocInfo, lpNumBuffers, Surfaces.GetPointer( 0 ) );

	if ( hr != S_OK && !( lpAllocInfo->dwFlags & VMR9AllocFlag_3DRenderTarget ) )
	{
		DeleteSurface();

		if ( lpAllocInfo->Format > '0000' )
		{
			hr = D3DDev->GetDisplayMode( NULL, &d3ddm );
			if ( hr != S_OK )
			{
				return hr;
			}

			for ( i = 0 ; i < VMRSALLOC_USE_TEXTURE_MAX ; ++i )
			{
				hr = D3DDev->CreateTexture( lpAllocInfo->dwWidth, lpAllocInfo->dwHeight, 1, D3DUSAGE_RENDERTARGET, d3ddm.Format, D3DPOOL_DEFAULT, &lpTexture, NULL );
				if ( hr != S_OK )
				{
					return hr;
				}
				m_alpDirect3DTexture[ i ] = lpTexture;
			}
			m_fmtTexture = d3ddm.Format;
		}

		lpAllocInfo->dwFlags &= ~VMR9AllocFlag_TextureSurface;
		lpAllocInfo->dwFlags |= VMR9AllocFlag_OffscreenSurface;

		hr = SurfaceAllocNotify->AllocateSurfaceHelper( lpAllocInfo, lpNumBuffers, Surfaces.GetPointer( 0 ) );
	} else
	{
		for ( i = 0 ; i < VMRSALLOC_USE_TEXTURE_MAX ; ++i )
		{
			hr = D3DDev->CreateTexture( lpAllocInfo->dwWidth, lpAllocInfo->dwHeight, 1, D3DUSAGE_RENDERTARGET, d3ddm.Format, D3DPOOL_DEFAULT, &lpTexture, NULL );
			if ( hr != S_OK )
			{
				return hr;
			}
			m_alpDirect3DTexture[ i ] = lpTexture;
		}
	}

	m_nFilpTexNum = texnum = -1;
	if ( m_alpDirect3DTexture[ 0 ] )
	{
		m_nFilpTexNum = 0; texnum = 1;
	}
	twidth = lpAllocInfo->dwWidth;
	theight = lpAllocInfo->dwHeight;
	return hr;
}

STDMETHODIMP VMRSurfaceAllocator::TerminateDevice( DWORD_PTR dwUserID )
{
	DeleteSurface();
	return S_OK;
}

BOOL VMRSurfaceAllocator::DeleteSurface( void )
{
	int i;

	for ( i = 0 ; i < VMRSALLOC_USE_TEXTURE_MAX ; ++i )
	{
		if ( m_alpDirect3DTexture[ i ] != NULL )
		{
			m_alpDirect3DTexture[ i ]->Release();
			m_alpDirect3DTexture[ i ] = NULL;
		}
	}

	Surfaces.ReleaseAll();

	return TRUE;
}

HRESULT VMRSurfaceAllocator::OnLostDevice( void )
{
	return 0;
}

HRESULT VMRSurfaceAllocator::OnResetDevice( void )
{
	return 0;
}










HRESULT VMRSurfaceAllocator::QueryInterface( REFIID iid, PVOID *pvInterface )
{
	return NonDelegatingQueryInterface( iid, pvInterface );
}

DWORD STDMETHODCALLTYPE VMRSurfaceAllocator::AddRef()
{
	InterlockedIncrement( &count );
	return static_cast<DWORD>( count );
}

DWORD STDMETHODCALLTYPE VMRSurfaceAllocator::Release()
{
	LONG n = InterlockedDecrement( &count );
	if ( n == 0 )
	{
		delete this;
	}
	return static_cast<DWORD>( n );
}

int VMRSurfaceAllocator::GetMovieWidth( void )
{
	return mwidth;
}

int VMRSurfaceAllocator::GetMovieHeight( void )
{
	return mheight;
}

int VMRSurfaceAllocator::GetTextureWidth( void )
{
	return twidth;
}

int VMRSurfaceAllocator::GetTextureHeight( void )
{
	return theight;
}

IDirect3DTexture9 * VMRSurfaceAllocator::GetDrawTexture( void )
{
	return m_alpDirect3DTexture[ texnum ];
}
