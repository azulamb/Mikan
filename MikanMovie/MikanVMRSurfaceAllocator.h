#ifndef CLASS_MIKANVMRSURFACEALLOCATOR
#define CLASS_MIKANVMRSURFACEALLOCATOR


#include "MikanMovie.h"

#include <dshow.h>
#include <Vmr9.h>

#include "WinSDKSample/Streams.h"

#ifndef _UNLINK_LIBRARY
#  pragma comment( lib, "Strmiids.lib" )
#endif

class SurfaceList
{
private:

	unsigned long size;
public:
	IDirect3DSurface9 **list;
	SurfaceList( void );
	virtual ~SurfaceList( void );
	virtual unsigned long Size( void );
	virtual int Resize( unsigned long newsize );
	virtual IDirect3DSurface9 * Get( unsigned long index );
	virtual IDirect3DSurface9 ** GetPointer( unsigned long index );
	virtual int Release( unsigned long index );
	virtual unsigned long ReleaseAll( void );
};

#define VMRSALLOC_USE_TEXTURE_MAX 2

class VMRSurfaceAllocator : public CUnknown, public IVMRSurfaceAllocator9, public IVMRImagePresenter9, public MikanVMRSurfaceAllocator
{
private:
	class DirectXDraw9 *mdraw;
	IDirect3DDevice9 *D3DDev; // DirectXDrawからもらおう。
	IDirect3D9 *D3D;
	IVMRSurfaceAllocatorNotify9 *SurfaceAllocNotify;
	D3DFORMAT m_fmtTexture;

	IDirect3DTexture9 *m_alpDirect3DTexture[ VMRSALLOC_USE_TEXTURE_MAX ];
	SurfaceList Surfaces;
	// 動画サイズ。
	unsigned long mwidth, mheight;
	// テクスチャサイズ。
	unsigned long twidth, theight;
	// 書き込みテクスチャ番号, 利用可能テクスチャ番号
	int m_nFilpTexNum, texnum;

	LONG count;

	class CriticalLock *clock;


	virtual HRESULT VMRSurfaceAllocator::PresentToTexture( VMR9PresentationInfo *lpPresInfo );
	virtual BOOL VMRSurfaceAllocator::DeleteSurface( void );

public:
	VMRSurfaceAllocator( class DirectXDraw9 *mikandraw, class CriticalLock *clock );
	virtual ~VMRSurfaceAllocator( void );

	// IUnknownに必要なマクロ
	//DECLARE_IUNKNOWN;
	virtual STDMETHODIMP QueryInterface( REFIID riid, __deref_out void **ppv );// { return GetOwner()->QueryInterface(riid,ppv); };
	virtual STDMETHODIMP_( ULONG ) AddRef();// { return GetOwner()->AddRef(); };
	virtual STDMETHODIMP_( ULONG ) Release();// { return GetOwner()->Release(); };
	//LPUNKNOWN GetOwner() const { return m_pUnknown; };

	//QueryInterface
	virtual STDMETHODIMP NonDelegatingQueryInterface( REFIID refiid, void **lplpInterface );

	//IVMRSurfaceAllocator9
	virtual STDMETHODIMP InitializeDevice( DWORD_PTR dwUserID, VMR9AllocationInfo *lpAllocInfo, DWORD *lpNumBuffers );
	virtual STDMETHODIMP TerminateDevice( DWORD_PTR dwUserID );
	virtual STDMETHODIMP GetSurface( DWORD_PTR dwUserID, DWORD dwSurfaceIndex, DWORD dwSurfaceFlags, IDirect3DSurface9 far **lplpSurface );
	virtual STDMETHODIMP AdviseNotify( IVMRSurfaceAllocatorNotify9 *lpVMRSurfaceAllocNotify );

	//IVMRImagePresenter9
	virtual STDMETHODIMP StartPresenting( DWORD_PTR dwUserID );
	virtual STDMETHODIMP StopPresenting( DWORD_PTR dwUserID );
	virtual STDMETHODIMP PresentImage( DWORD_PTR dwUserID, VMR9PresentationInfo *lpPresInfo );

	//デバイスのリセット処理
	virtual HRESULT OnLostDevice( void );
	virtual HRESULT OnResetDevice( void );

	virtual int GetMovieWidth( void );
	virtual int GetMovieHeight( void );

	virtual int GetTextureWidth( void );
	virtual int GetTextureHeight( void );

	virtual IDirect3DTexture9 * GetDrawTexture( void );

};

#endif