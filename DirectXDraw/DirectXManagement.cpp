#include "DirectXDraw.h"

/*!
@brief �R���X�g���N�^(�񐄏�)
@par   �֐�����
�ϐ�������������B
*/
DirectXDraw9::DirectXDraw9( void )
{
	Init();
}

/*!
@brief �R���X�g���N�^(�ʏ�)
@par   �֐�����
�ϐ�������������B
*/
DirectXDraw9::DirectXDraw9( HWND wh, unsigned int tmax, unsigned int pmax, unsigned int fmax )
{
	Init( wh, tmax, pmax, fmax );
}

/*!
@brief �R���X�g���N�^�{��
@par   �֐�����
�ϐ�������������B
*/
int DirectXDraw9::Init( HWND wh, unsigned int tmax, unsigned int pmax, unsigned int fmax )
{
	unsigned int c;

	D3D = NULL;

	//�o�b�N�o�b�t�@
	backbuffer = NULL;
	backbufferz = NULL;
	line = NULL;
	//�e�N�X�`���̍ő吔
	SetTextureMax( tmax );
	//���p�`�̍ő吔
	SetPolygonMax( pmax );

	lastpvnum = 0;
	//�e�N�X�`���̃|�C���^�z���NULL�ɂ���
	dxtex = NULL;
	//���p�`�̃|�C���^�z���NULL�ɂ���B
	pv = NULL;
	rpv = NULL;
	rpvmax = 0;
	//�E�B���h�E�n���h���̐ݒ�
	SetWindowHandle( wh );

	// ��ʃT�C�Y��ݒ�B
	// 0 ���ƃE�B���h�E�T�C�Y�ɂȂ�B
	SetScreenSize( 0, 0 );

	//�����_�[�^�[�Q�b�g�͉�ʁB
	target[ 0 ] = 1;
	target[ 1 ] = 0;
	//�e�N�X�`���̃o�b�N�A�b�v���s���B
	SetAutoBackUpTexture( 0 );

	//�����̏�����
	line = NULL;

	fontmax = fmax;
	fontdata = ( struct FONTDATA * )calloc( fontmax, sizeof( struct FONTDATA ) );
	//�t�H���g�̏�����
	for ( c = 0; c < fontmax; ++c )
	{
		fontdata[ c ].D3DFont = NULL;
		fontdata[ c ].use = 0;
	}

	//���������t���O
	SetVsyncWait();

	swapchainnum = 0;
	swapchainmax = 1;
	/*  swapchain = (IDirect3DSwapChain9 **)calloc( sizeof(IDirect3DSwapChain9 *), swapchainmax );
	  for( c = 0 ; c < swapchainmax ; ++c )
	  {
	  swapchain[ c ] = NULL;
	  }*/
	SetDrawFunction();

	fullscreen = 0;

	// �`�撆���ǂ����B
	drawing = 0;
	lock = 0;
	locksleeptime = 1;
	ismultithread = 0;

	D3D = Direct3DCreate9( D3D_SDK_VERSION );

	return 0;
}

/*!
@brief �f�X�g���N�^
@par   �֐�����
�������Ă��Ȃ��B
*/
DirectXDraw9::~DirectXDraw9( void )
{
	//  CleanupD3DObject();
	//  CleanupD3D();
}

int DirectXDraw9::SetFullScreenMode( int fullscreen_on )
{
	fullscreen = fullscreen_on;
	return 0;
}

/*!
@brief �E�B���h�E�n���h���̐ݒ�
@par   �֐�����
DirectX�Ɗ֘A�t����E�B���h�E�n���h����ݒ肷��B
@param wh �E�B���h�E�n���h���B
@return �ݒ肵���E�B���h�E�n���h����Ԃ��B
*/
HWND DirectXDraw9::SetWindowHandle( HWND wh )
{
	return ( WindowHandle = wh );
}

/*!
@brief �E�B���h�E�n���h���̎擾
@par   �֐�����
DirectX�Ɗ֘A�t����ꂽ�E�B���h�E�n���h�����擾����B
@return �E�B���h�E�n���h���B
*/
HWND DirectXDraw9::GetWindowHandle( void )
{
	return WindowHandle;
}

/*!
@brief ���������҂��̐ݒ�
@par   �֐�����
����������҂��ǂ����̐ݒ�BInitD3D���O�Ɏ��s����K�v������B
@param vs 0�Ő���������҂��Ȃ��B����ȊO�Ő���������҂B
@return �ݒ肵���l��Ԃ��B
*/
int DirectXDraw9::SetVsyncWait( int vs )
{
	return ( vsync = vs );
}

//�e�N�X�`���̍ő吔�̐ݒ�
unsigned int DirectXDraw9::SetTextureMax( unsigned int tmax )
{
	//if( dxtex == NULL )
	//{
	return ( texmax = tmax );
	//}
	//�e�N�X�`���p�̔z��Ƃ����o���Ă���ꍇ�ɂ́A�������Ȃ艽�Ȃ�̍�Ƃ��K�v�B
	//�Ƃ肠�����ʓ|�Ȃ̂Ŏ����͌����邪�A�ǂ����邩�ȁB
	//return 0;
}

unsigned int DirectXDraw9::GetTextureMax( void )
{
	return texmax;
}

int DirectXDraw9::IsEmptyTexture( unsigned int txnum )
{
	if ( txnum >= texmax )//if( txnum >= texmax || dxtex[ txnum ].ext == 0 )
	{
		//�e�N�X�`���ԍ����s��
		return 2;
	}

	if ( dxtex[ txnum ].ext )
	{
		// �e�N�X�`���g�p��
		return 1;
	}
	// �e�N�X�`�����g�p
	return 0;
}

// ���p�`���_�̍ő吔�̐ݒ�
unsigned int DirectXDraw9::SetPolygonMax( unsigned int pmax )
{
	return ( pvmax = pmax );
}

// �`��T�C�Y�̐ݒ�B
int DirectXDraw9::SetScreenSize( int width, int height )
{
	swidth = width;
	sheight = height;

	if ( D3D )
	{
		//InitD3D( 0 );
		RecoverFromDeviceLost( 1 );
	}

	return 0;
}

int DirectXDraw9::GetScreenWidth( int now )
{
	return now ? D3DPP.BackBufferWidth : swidth;
}
int DirectXDraw9::GetScreenHeight( int now )
{
	return now ? D3DPP.BackBufferHeight : sheight;
}

/*!
@brief DirectX�̏�����
@par   �֐�����
DirectX�̏��������s���B
@return 0=�����B
@return 1=�E�B���h�E�n���h����������Ȃ��B
@return 2=���������s�B
@return 3=IDirect3Device9�̏������Ɏ��s�B
*/
int DirectXDraw9::InitD3D( int create )
{
	//  unsigned int i;
	D3DDEVTYPE D3DDevType;
	//  D3DDISPLAYMODE d3ddm;
	//  tagRECT rect;

	if ( create )
	{
		if ( GetWindowHandle() == NULL )
		{
			//�E�B���h�E�n���h����������Ȃ�
			return 1;
		}
	}

	//  D3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

	//D3PP�̒��g��0�Ŗ��߂�B
	ZeroMemory( &D3DPP, sizeof( D3DPP ) );
	//�E�B���h�E���[�h�̎w��
	D3DPP.Windowed = TRUE;

	if ( vsync )
	{
		D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	} else
	{
		D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	//    D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	if ( fullscreen )
	{
		// �t���X�N���[�����[�h
		D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
		D3DPP.BackBufferCount = 1;
		//D3DPP.SwapEffect = D3DSWAPEFFECT_COPY;
		//D3DPP.BackBufferFormat = d3ddm.Format;
		//D3DPP.EnableAutoDepthStencil = TRUE;
		//D3DPP.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		D3DPP.Windowed = FALSE;
		D3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;
		if ( GetScreenWidth() == 0 )
		{
			SetScreenSize( 640, 480 );
		}
	} else
	{
		// �E�B���h�E���[�h
		if ( vsync )
		{
			//�X���b�v�G�t�F�N�g
			D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
		} else
		{
			//�X���b�v�G�t�F�N�g
			D3DPP.SwapEffect = D3DSWAPEFFECT_COPY;
		}

		//�o�b�N�o�b�t�@�̃t�H�[�}�b�g
		D3DPP.BackBufferCount = 1;//D3DFMT_UNKNOWN;;//1;//2//D3DFMT_UNKNOWN;

	}


	// �����Ńo�b�N�o�b�t�@�̃T�C�Y�ݒ���s���B
	// �{���̓A�X�y�N�g��ێ��̏ꍇ�A�E�B���h�E�T�C�Y�Ɣ�r���Ă���o�b�N�o�b�t�@�T�C�Y�����߂�B
	// ���łɁA�`������炵�ĕ`�悪�^�񒆂ɗ���悤�ɂ���B

	D3DPP.BackBufferWidth = GetScreenWidth();
	D3DPP.BackBufferHeight = GetScreenHeight();
	// �}���`�T���v�����ɂڂ₯�Ȃ��悤�ɂ���B
	D3DPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	//  D3DPP.BackBufferFormat = d3ddm.Format;
	//  GetClientRect( GetWindowHandle(), &rect );
	//  D3DPP.BackBufferWidth = rect.right;
	//  D3DPP.BackBufferHeight = rect.bottom;
	//  D3DPP.Windowed = TRUE;

	//  D3DPP.EnableAutoDepthStencil = TRUE;
	//  D3DPP.AutoDepthStencilFormat = D3DFMT_D16;
	//  D3DPP.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	//D3D�f�o�C�X�����B
	//  hr = D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
	//    GetWindowHandle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING,
	//    &D3DPP, &D3DDev );
	ismultithread = 1;
	if ( create )
	{
		hr = D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetWindowHandle(), D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DPP, &D3DDev );
		// T&L, HAL
		if ( FAILED( hr ) )
		{
			hr = D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetWindowHandle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DPP, &D3DDev );
			// not T&L, HAL
			if ( FAILED( hr ) )
			{
				hr = D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, GetWindowHandle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DPP, &D3DDev );
				// REF
				if ( FAILED( hr ) )
				{
					//�f�o�C�X�������ׂĎ��s
					ismultithread = 0;
					return 3;
				} else
				{
					D3DDevType = D3DDEVTYPE_REF;
				}
			} else
			{
				D3DDevType = D3DDEVTYPE_HAL;
			}
		} else
		{
			D3DDevType = D3DDEVTYPE_HAL;
		}


		if ( FAILED( hr ) )
		{
			//IDirect3Device9�C���^�[�t�F�C�X�̎擾�Ɏ��s�B
			return 3;
		}

		// �e�N�X�`���ւ̕`�悪�ł��邩�̃`�F�b�N
		hr = D3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDevType, D3DPP.BackBufferFormat, D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8 );
		if ( FAILED( hr ) )
		{
			candrawtexture = 0;
		} else
		{
			candrawtexture = 1;
		}
		// �T�[�t�F�C�X�ւ̕`�悪�ł��邩�̃`�F�b�N
		hr = D3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDevType, D3DPP.BackBufferFormat, D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE, D3DFMT_A8R8G8B8 );
		if ( FAILED( hr ) )
		{
			candrawsurface = 0;
		} else
		{
			candrawsurface = 1;
		}
	} else
	{
		hr = D3DDev->Reset( &D3DPP );

		if ( FAILED( hr ) )
		{
			if ( hr == D3DERR_DEVICELOST )
			{
				//�f�o�C�X���X�g
				return 4;
			}
			//�����Ɏ��s
			return 5;
		}
	}


	// �X���b�v�`�F�[���̍쐬
	/*  for( i = 0 ; i < swapchainmax ; ++i )
	  {
	  D3DDev->CreateAdditionalSwapChain( &D3DPP, &(swapchain[ i ]) );
	  }*/

	InitRender();

	if ( create )
	{
		FontInit();
	}

	//IDirect3DSurface9 *surface;
	//swapchain[ swapchainnum ]->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &surface);
	//D3DDev->SetRenderTarget( 0, surface );


	return 0;
}

/*!
@brief DirectX�̉��
@par   �֐�����
DirectX�̃f�o�C�X��I�u�W�F�N�g�̉�����s���B
@return 0��Ԃ��B
*/
int DirectXDraw9::CleanupD3D( void )
{
	//Direct3D�I�u�W�F�N�g�̉��
	DX_RELEASE( D3D );

	FontCleanUp();

	//�f�o�C�X�̉��
	DX_RELEASE( D3DDev );

	return 0;
}

/*!
@brief DirectX�̃I�u�W�F�N�g�̏�����
@par   �֐�����
DirectX�̃e�N�X�`�����̏��������s���B
�f�o�C�X���X�g��Ɏ��s����ƁA�e�N�X�`���̍Đ������s���B
@return 0��Ԃ��B
*/
int DirectXDraw9::InitD3DObject( void )
{
	unsigned int t;

	//�o�b�N�o�b�t�@�̊m��
	D3DDev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer );
	// Z�o�b�t�@�̎擾
	D3DDev->GetDepthStencilSurface( &backbufferz );

	//�̈�̊m��
	if ( dxtex == NULL )
	{
		dxtex = ( struct MYTEXTURE * )calloc( texmax, sizeof( struct MYTEXTURE ) );
	}
	if ( pv == NULL )
	{
		pv = ( struct POLYGON_VERTEX * )calloc( pvmax + 1, sizeof( struct POLYGON_VERTEX ) );
		pv[ pvmax ].type = 2;
	}

	for ( t = 0; t < texmax; ++t )
	{
		//    dxtex[ t ].tex = ( IDirect3DTexture9 * )calloc( 1, sizeof( IDirect3DTexture9 ) );

		//�t�@�C����������ꍇ�ɂ̓e�N�X�`�������B
		//�o�b�N�A�b�v������ꍇ�̓X���b�h�t���O�𗧂Ă��ɍ��B
		if ( dxtex[ t ].ext )
		{
			if ( dxtex[ t ].filename )
			{
				if ( dxtex[ t ].module )
				{
					CreateTexture( t, dxtex[ t ].module, dxtex[ t ].filename, dxtex[ t ].color );//, (dxtex[ t ].backup == NULL) );
				} else
				{
					CreateTexture( t, dxtex[ t ].filename, dxtex[ t ].color );//, (dxtex[ t ].backup == NULL) );
				}
				//dxtex[ t ].ext = 1;
			} else
			{
				//��̃e�N�X�`��
				CreateTexture( t, dxtex[ t ].width, dxtex[ t ].height );//, (dxtex[ t ].backup == NULL) );
			}

			//�e�N�X�`���̃o�b�N�A�b�v�ɐ��������̂ŏ����߂��B
			if ( dxtex[ t ].backup )
			{
				//�e�N�X�`�����T�[�t�F�C�X��
				DX_RELEASE( dxtex[ t ].sur[ 0 ] );
				dxtex[ t ].tex->GetSurfaceLevel( 0, &( dxtex[ t ].sur[ 0 ] ) );
				//�e�N�X�`���ɃT�[�t�F�C�X�������߂�
				hr = D3DDev->UpdateSurface( dxtex[ t ].backup, NULL, dxtex[ t ].sur[ 0 ], NULL );
			}

		}//end texture exist check
	}//end texture loop

	return 0;
}

/*!
@brief DirectX�̃I�u�W�F�N�g�̉��
@par   �֐�����
�e�N�X�`���̉�����s���B����������ŉ������ƃe�N�X�`���̍Đ����Ȃǂ͂ł��Ȃ��B
@return 0��Ԃ��B
*/
int DirectXDraw9::CleanupD3DObject( void )
{
	unsigned int i;
	//�o�b�N�o�b�t�@�̉��
	DX_RELEASE( backbuffer );
	DX_RELEASE( backbufferz );
	DX_RELEASE( line );

	//�e�N�X�`���̉��
	if ( dxtex )
	{
		for ( i = 0; i < texmax; ++i )
		{
			if ( dxtex[ i ].ext )
			{
				ReleaseTexture( i );
			}
		}
		DX_FREE( dxtex );
	}

	//���_�̉��
	if ( pv )
	{
		for ( i = 0; i <= pvmax; ++i )
		{
			//      if( pv[ i ] )
			//      {
			ReleasePolygon( i );
			//      }
		}
		DX_FREE( pv );
	}

	if ( rpv )
	{
		free( rpv );
		rpv = NULL;
		rpvmax = 0;
	}

	/*  for( i = 0 ; i < swapchainmax ; ++i )
	  {
	  DX_RELEASE( swapchain[ i ] );
	  }
	  DX_FREE( swapchain );*/

	return 0;
}

int DirectXDraw9::SetAutoBackUpTexture( int enable )
{
	return ( texturebackup = enable );
}

/*!
@brief �e�N�X�`���̃o�b�N�A�b�v
@par   �֐�����
�e�N�X�`�����f�o�C�X���X�g���ɂ��������A����T�[�t�F�C�X�Ƃ��ĕۑ�����B
�T�[�t�F�C�X��]�������Ƃ������ăe�N�X�`���𕜌����邱�Ƃ��\�ɂȂ�B
@param txnum �e�N�X�`���ԍ�
@return 0 = �o�b�N�A�b�v�ɐ����B
@return -1 = �o�b�N�A�b�v�����Ȃ��B
@return 1 = �e�N�X�`���ԍ����s���B
@return 2 = �o�b�N�A�b�v�Ɏ��s�B
*/
int DirectXDraw9::BackUpTexture( unsigned int txnum, int force )
{
	//�T�[�t�F�C�X
	IDirect3DSurface9 *bsur;
	//�T�[�t�F�C�X�̃t�H�[�}�b�g
	D3DSURFACE_DESC desc;

	if ( texturebackup == 0 && force == 0 )
	{
		return -1;
	}

	if ( CannotUseTexture( txnum ) )//if( txnum >= texmax || dxtex[ txnum ].ext == 0 )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	//�����_�[�e�N�X�`���̃t�H�[�}�b�g���擾
	hr = dxtex[ txnum ].tex->GetLevelDesc( 0, &desc );

	//�o�b�N�A�b�v�T�[�t�F�C�X
	bsur = dxtex[ txnum ].backup;
	hr = D3DDev->CreateOffscreenPlainSurface(
		desc.Width, desc.Height, desc.Format,
		D3DPOOL_SYSTEMMEM, &( dxtex[ txnum ].backup ), NULL );

	if ( dxtex[ txnum ].sur == NULL )
	{
		dxtex[ txnum ].tex->GetSurfaceLevel( 0, &( dxtex[ txnum ].sur[ 0 ] ) );
	}

	if ( SUCCEEDED( hr ) )
	{
		hr = D3DDev->GetRenderTargetData( dxtex[ txnum ].sur[ 0 ], dxtex[ txnum ].backup );
		if ( SUCCEEDED( hr ) )
		{
			DX_RELEASE( bsur );
		}
		return 0;
	}

	dxtex[ txnum ].backup = bsur;

	return 2;
}

/*!
@brief �f�o�C�X���X�g����̕��A����
@par   �֐�����
DirectX�̃I�u�W�F�N�g�̉���Ȃǂ��s���AReset���邽�߂̏������s���B
�����ŉ�����ꂽ�e�N�X�`���͌�ɍĐ����\�B
@return 0��Ԃ��B
*/
int DirectXDraw9::BackUp( void )
{
	unsigned int i;
	IDirect3DTexture9 *release;

	//DirectX���ŊǗ����Ă��Ȃ����\�[�X�̉��
	//�o�b�N�o�b�t�@�̉��
	DX_RELEASE( backbuffer );
	DX_RELEASE( backbufferz );
	DX_RELEASE( line );
	if ( dxtex )
	{
		for ( i = 0; i < texmax; ++i )
		{
			if ( dxtex[ i ].ext && dxtex[ i ].pool != D3DPOOL_MANAGED )
			{
				release = dxtex[ i ].tex;
				DX_RELEASE( release );
				DX_RELEASE( dxtex[ i ].sur[ 0 ] );
				DX_RELEASE( dxtex[ i ].sur[ 1 ] );
				DX_RELEASE( dxtex[ i ].zsur );
				dxtex[ i ].tex = NULL;
			}
		}
	}
	/*  for( i = 0 ; i < swapchainmax ; ++i )
	  {
	  DX_RELEASE( swapchain[ i ] );
	  }*/

	FontCleanUp();
	return 0;
}

/*!
@brief �f�o�C�X���X�g����̕��A���
@par   �֐�����
�e�N�X�`����t�H���g�Ȃǂ��Đ�������B
@return 0��Ԃ��B
*/
int DirectXDraw9::Recover( void )
{
	InitD3DObject();
	FontRecover();
	return 0;
}

/*!
@brief �f�o�C�X���X�g����̕��A
@par   �֐�����
�f�o�C�X���X�g�ɂȂ����Ƃ��A�e�N�X�`���̉����Reset�A���A�Ȃǂ̍�Ƃ��܂Ƃ߂čs���B
@return 0=�����B
@return 1=�f�o�C�X���X�g���Ȃ̂ł܂����A��Ƃ��o���Ȃ��B
@return 2=�܂�Reset�ł��Ȃ��B
@return 3=Reset�������f�o�C�X���X�g�������B
@return 4=Reset�Ɏ��s�B
*/
int DirectXDraw9::RecoverFromDeviceLost( int force )
{
	if ( GetWindowHandle() == NULL )
	{
		return 0;
	}
	if ( force == 0 )
	{
		hr = D3DDev->TestCooperativeLevel();
		if ( hr == D3DERR_DEVICELOST )
		{
			//�f�o�C�X���X�g���Ȃ̂ł��������҂B
			return 1;
		} else if ( hr != D3DERR_DEVICENOTRESET )
		{
			//�\�����ʃG���[
			return 2;
		}
	}

	//�f�o�C�X���X�g�I���B���A��ƊJ�n�B

	//Direct3D�ŊǗ����ĂȂ����\�[�X�̉��
	//Reset�O�Ƀ��\�[�X�̉�������Ă����Ȃ��Ƃ����Ȃ��B
	BackUp();

	/*
	//D3PP�̒��g��0�Ŗ��߂�B
	ZeroMemory( &D3DPP, sizeof( D3DPP ) );
	//�E�B���h�E���[�h�̎w��
	D3DPP.Windowed = TRUE;
	//�X���b�v�G�t�F�N�g
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//�o�b�N�o�b�t�@�̃t�H�[�}�b�g
	D3DPP.BackBufferCount = D3DFMT_UNKNOWN;*/

	//�������s��

	if ( InitD3D( 0 ) == 5 )
	{
		InitD3D( 1 );
	}

	//���ߐF�Ȃǂ̍Đݒ�
	//InitRender();

	//�������Direct3D�ŊǗ����Ă��Ȃ����\�[�X���Ď擾
	Recover();

	//�f�o�C�X���X�g����̕��A
	return 0;
}

int DirectXDraw9::SetDrawFunction( int mode )
{
	switch ( mode )
	{
	case DRAW_SWAPCHAIN:
		drawfunc = &DirectXDraw9::Draw;
		break;
	default:
		drawfunc = &DirectXDraw9::DrawNull;
		break;
	}
	return 0;
}

int DirectXDraw9::Draw( int num )
{
	//swapchain[ num ]->Present();

	return 0;
}

/*!
@brief ��ʂ����ꂢ�ɂ���B
@par   �֐�����
��ʂ�P��F�œh��Ԃ��B
@param color �h��Ԃ��F�B�ȗ���0xFF000000(��)�B
@return 0��Ԃ��B
*/
int DirectXDraw9::ClearScreen( unsigned long color )
{
	D3DDev->Clear( 0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0 );
	++countcs;
	return 0;
}

int DirectXDraw9::CountClearScreen( void )
{
	return countcs;
}

/*!
@brief �`��J�n
@par   �֐�����
DirectX��p�����e�N�X�`����t�H���g�̕`��̊J�n�B
@return 0=�����B
@return 1=���s�B
*/
int DirectXDraw9::Begin( void )
{
	drawing_ = 0;
	//�`��J�n
	if ( SUCCEEDED( D3DDev->BeginScene() ) )
	{
		//����
		countcs = 0;
		drawing = 1;
		return 0;
	}
	//���s
	return 1;
}

/*!
@brief �`��I��
@par   �֐�����
DirectX�𗎂������e�N�X�`����t�H���g�̕`��̏I���B
@return 0=�����B
@return ���̑�=�`����Present�̕Ԃ�l�B
*/
int DirectXDraw9::End( void )
{
	drawing_ = 1;
	while ( lock )
	{
		Sleep( locksleeptime );
	}
	D3DDev->EndScene();//�`��I��
	drawing = 0;
	//�`��X�V
	//  if( swapchainnum >= 0 && FAILED( (hr = swapchain[ swapchainnum ]->Present( NULL, NULL, NULL, NULL, 0 )) ) )
	//  {
	if ( FAILED( ( hr = D3DDev->Present( NULL, NULL, NULL, NULL ) ) ) )
	{
		return hr;
	}
	//  }

	return 0;
}

int DirectXDraw9::LockDrawEnd( unsigned long sleeptime )
{
	locksleeptime = sleeptime;
	lock = 1;
	return 0;
}

int DirectXDraw9::UnlockDarawEnd( void )
{
	lock = 0;
	return 0;
}

int DirectXDraw9::WaitDrawBegin( unsigned long sleeptime, unsigned int timeout, int autounlock )
{
	unsigned int before;

	before = timeGetTime();

	while ( drawing == 0 )
	{
		Sleep( sleeptime );
		if ( autounlock && lock )
		{
			lock = 0;
		}
		if ( timeGetTime() - before > timeout )
		{
			return 1;
		}
	}
	return 0;
}

int DirectXDraw9::WaitDrawEnd( unsigned long sleeptime, unsigned int timeout )
{
	unsigned int before;

	before = timeGetTime();

	while ( drawing_ == 0 )
	{
		Sleep( sleeptime );
		if ( timeGetTime() - before > timeout )
		{
			return 1;
		}
	}
	return 0;
}

MYTEXTURE * DirectXDraw9::GetTextureData( unsigned int txnum )
{
	if ( CannotUseTexture( txnum ) )//if( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return NULL;
	}
	return dxtex + txnum;
}

/*!
@brief �e�N�X�`���̏���
@par   �֐�����
�e�N�X�`���̍����Ȃǂ̏����ݒ���s���B
@return SetRender�̕Ԃ�l�B
*/
int DirectXDraw9::InitRender( void )
{

	//�`��I�v�V������
	//�e�N�X�`���u�����h�ŐF����Z����
	D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	//�F�̃p�����[�^2�̓f�B�t���[�Y�l
	D3DDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	//�F�̃p�����[�^1�̓e�N�Z��
	D3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	//�e�N�X�`���u�����h�ŃA���t�@����Z����
	D3DDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	//�A���t�@�̃p�����[�^2�̓f�B�t���[�Y�l
	D3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	//�A���t�@�̃p�����[�^1�̓e�N�Z��
	D3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	//Z�o�b�t�@�֘A�̐ݒ�
	D3DDev->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS );
	D3DDev->SetRenderState( D3DRS_ZENABLE, TRUE );
	D3DDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

	//�J�����O���[�h�Ȃ�
	D3DDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	//���C�e�B���O�Ȃ�
	D3DDev->SetRenderState( D3DRS_LIGHTING, FALSE );

	// �h�b�g���ڂ₯�Ȃ��悤�ɂ���B
	D3DDev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	D3DDev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	D3DDev->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT );
	return SetRender( DRAW_TRC );
}

/*!
@brief �e�N�X�`���̍������@�̎w��
@par   �֐�����
�e�N�X�`���̍������@���w�肷��B
@param kind �ȉ��̒l�̂ǂꂩ���w�肷��B
DRAW_TRC=���ߐF��L���ɂ���B
DRAW_ADD=���Z�����B
DRAW_SUB=���Z�����B
DRAW_MUL=��Z�����B
DRAW_NON=�������Ȃ��B���ߐF�������B
@return 0��Ԃ��B
*/
int DirectXDraw9::SetRender( int kind )
{
	switch ( kind )
	{
	case DRAW_TRC://���ߐF�L���`��
		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		//�A���t�@�E�u�����f�B���O���s��
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		//�u�����f�B���O�W��( 1 - As, 1 - As, 1 - As, 1 - As )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		//�u�����f�B���O�W��( As, As, As, As )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		break;
	case DRAW_ADD://���Z�`��
		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		//�A���t�@�E�u�����f�B���O���s��
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		//�u�����f�B���O�W��( 1, 1, 1, 1 )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		//�u�����f�B���O�W��( As, As, As, As )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		break;
	case DRAW_SUB://���Z�`��
		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		//�A���t�@�E�u�����f�B���O���s��
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		//�u�����f�B���O�W��( 1 - Rs, 1 - Gs, 1 - Bs, 1 - As )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		//�u�����f�B���O�W��( 0, 0, 0, 0 )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
		break;
	case DRAW_MUL://��Z�`��
		//    D3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//    D3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		//    D3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR); 
		//D3DRS_BLENDOP�ł̉��Z��L���ɂ���炵��
		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		//�u�����f�B���O�W��( Rs, Gs, Bs, As )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
		//�u�����f�B���O�W��( 0, 0, 0, 0 )��( As, As, As, As )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );// | D3DBLEND_SRCALPHA );
		//�]���� + �]����
		D3DDev->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
		break;
	case DRAW_MUL_WHITE: // ���w�i������Z�`��
		D3DDev->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF );

		D3DDev->SetTexture( 1, NULL );

		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		D3DDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		D3DDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		D3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

		// �e�N�X�`���X�e�[�W1�Ŕ��e�N�X�`���ƍ���
		D3DDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TFACTOR ); // �萔�e�N�X�`�����g�p
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_BLENDCURRENTALPHA );
		D3DDev->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT );
		D3DDev->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );

		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

		// ��Z�u�����h
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
		D3DDev->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
		break;
	case DRAW_FILL:

		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		//�A���t�@�E�u�����f�B���O���s��
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		//�u�����f�B���O�W��( 1 - As, 1 - As, 1 - As, 1 - As )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		//�u�����f�B���O�W��( As, As, As, As )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		break;
	case DRAW_HALFADD:
		//a = 0;
		break;
	case DRAW_CADD_ASUB:
		// �F
		/*    //�u�����f�B���O�W��( 1, 1, 1, 1 )
			D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			//�u�����f�B���O�W��( As, As, As, As )
			D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );

			// �A���t�@�l
			D3DDev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
			//�A���t�@�E�u�����f�B���O���s��
			//    D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			//�u�����f�B���O�W��( 1 - Rs, 1 - Gs, 1 - Bs, 1 - As )
			D3DDev->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCCOLOR );
			//�u�����f�B���O�W��( 0, 0, 0, 0 )
			D3DDev->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO );*/

		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
		D3DDev->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVSRCCOLOR );
		D3DDev->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA );
		//    D3DDev->SetRenderState(D3DRS_BLENDOPALPHA,D3DBLENDOP_ADD);

		break;
	case DRAW_BLUR:

		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
		//�A���t�@�E�u�����f�B���O���s��
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		//�u�����f�B���O�W��( 1 - Rs, 1 - Gs, 1 - Bs, 1 - As )
		D3DDev->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCCOLOR );
		//�u�����f�B���O�W��( 0, 0, 0, 0 )
		D3DDev->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO );

		D3DDev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
		//�u�����f�B���O�W��( 1, 1, 1, 1 )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		//�u�����f�B���O�W��( As, As, As, As )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );

		break;
	case DRAW_CUT:
		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );//D3DBLEND_SRCCOLOR );
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA );
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA );
		break;
	default://�`��
		D3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		D3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		//�A���t�@�E�u�����f�B���O���s��Ȃ�
		D3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		//�u�����f�B���O�W��( 0, 0, 0, 0 )
		D3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
		//�u�����f�B���O�W��( 1, 1, 1, 1 )
		D3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
		break;
	}
	return 0;
}

/*!
@brief ��ʂ������_�[�^�[�Q�b�g�ɂ���
@par   �֐�����
�`��Ώۂ���ʂɂ���B
@return 0��Ԃ��B
*/
int DirectXDraw9::SetRenderTarget( void )
{
	D3DDev->SetRenderTarget( 0, backbuffer );
	D3DDev->SetDepthStencilSurface( backbuffer );
	if ( target[ 0 ] == 0 )
	{
		//�O��̓e�N�X�`���ɏ�������ł����B
		BackUpTexture( target[ 1 ] );
	}
	target[ 0 ] = 1;
	return 0;
}

/*!
@brief �e�N�X�`���������_�[�^�[�Q�b�g�ɂ���
@par   �֐�����
�`��Ώۂ��e�N�X�`���ɂ���B
@param txnum �`��Ώۂɂ���e�N�X�`���ԍ��B
@return 0��Ԃ��B
*/
int DirectXDraw9::SetRenderTarget( unsigned int txnum )
{

	if ( CannotUseTexture( txnum ) )//if( txnum >= texmax || dxtex[ txnum ].ext == 0 )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	//�T�[�t�F�C�X�̉��
	DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
	//�T�[�t�F�C�X�̎擾
	hr = dxtex[ txnum ].tex->GetSurfaceLevel( 0, &( dxtex[ txnum ].sur[ 0 ] ) );

	//Depth�p�T�[�t�F�C�X�̉��
	DX_RELEASE( dxtex[ txnum ].zsur );
	//Depth�p�T�[�t�F�C�X�̍쐬
	hr = D3DDev->CreateDepthStencilSurface(
		dxtex[ txnum ].width, dxtex[ txnum ].height,
		D3DPP.AutoDepthStencilFormat,
		D3DPP.MultiSampleType,
		D3DPP.MultiSampleQuality,
		FALSE, &( dxtex[ txnum ].zsur ), NULL );

	//�^�[�Q�b�g���e�N�X�`���ɂ���
	hr = D3DDev->SetRenderTarget( 0, dxtex[ txnum ].sur[ 0 ] );
	//Depth�p�T�[�t�F�C�X���w�肷��B
	hr = D3DDev->SetDepthStencilSurface( dxtex[ txnum ].zsur );

	if ( target[ 0 ] == 0 )
	{
		//�O��̓e�N�X�`���ɕ`�悵���B
		BackUpTexture( target[ 1 ] );
	}

	target[ 0 ] = 0;
	target[ 1 ] = txnum;

	return 0;
}

int DirectXDraw9::ResetRenderTarget( void )
{
	if ( target[ 0 ] )
	{
		// ��ʁB
		return SetRenderTarget();
	}
	// �e�N�X�`���B
	return SetRenderTarget( target[ 1 ] );
}

/*!
@brief �摜��ǂݍ���Ńe�N�X�`�������
@par   �֐�����
�摜�t�@�C�����w�肵�ăe�N�X�`�������B
@param txnum �e�N�X�`���ԍ��B
@param filename �t�@�C�����B
@param tcolor ���ߐF�BTRC_NONE�ŉ������Ȃ��BTRC_ZERO�ō���̐F�𓧉ߐF�Ɏw�肷��B�ȗ������ꍇTRC_NONE(�������Ȃ�)�B
@param thread �X���b�h�����t���O�B0�Ńe�N�X�`���������ɃX���b�h�����Ȃ��B�ȗ������1�ŁA�X���b�h�������ɃX���b�h�����B
@return 0=�e�N�X�`�������X���b�h�̐����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�t�@�C�������Ȃ��B
@return 3=�t�@�C����񂪓ǂݎ��Ȃ������B
@return 4=�̈�m�ۂɎ��s�B
*/
int DirectXDraw9::CreateTexture( unsigned int txnum, const char *filename, unsigned long tcolor, int thread )
{
	HRESULT hr;
	D3DXIMAGE_INFO image_info;
	struct THREADIN_TEX * in;
	unsigned long tid;

	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	if ( filename == NULL )
	{
		//�t�@�C�������Ȃ�
		return 2;
	}

	hr = D3DXGetImageInfoFromFile( filename, &image_info );
	if ( FAILED( hr ) )
	{
		//�t�@�C����񂪓ǂݎ��Ȃ�����
		return 3;
	}
	//�e�N�X�`�������̂ŁA�g�p�s�ɂ���
	dxtex[ txnum ].ext = 0;
	//�������݃t���O���Ȃ��ɂ���B
	dxtex[ txnum ].writed = 0;

	//�e�N�X�`���T�C�Y�̎擾
	dxtex[ txnum ].width = image_info.Width;
	dxtex[ txnum ].height = image_info.Height;
	//���\�[�X�̐ݒ�
	dxtex[ txnum ].module = NULL;
	dxtex[ txnum ].resource = 0;

	//�X���b�h�ɓn���\���̗̂̈�m��
	in = ( struct THREADIN_TEX * )calloc( 1, sizeof( struct THREADIN_TEX ) );

	if ( in == NULL )
	{
		//�̈�m�ۂɎ��s
		return 4;
	}

	if ( dxtex[ txnum ].filename == NULL )
	{
		//�t�@�C���������镶����̈���m�ۂ���
		dxtex[ txnum ].filename = (char *)calloc( strlen( filename ) + 1, sizeof( char ) );
		//�t�@�C�������R�s�[����
		memcpy( dxtex[ txnum ].filename, filename, strlen( filename ) );
	}

	//�e�N�X�`���̐ݒ���s���B

	//�~�b�v���x���̐�
	dxtex[ txnum ].miplevel = 1;//D3DX_DEFAULT;
	//�e�N�X�`���̎g�p�ړI
	dxtex[ txnum ].usage = D3DUSAGE_RENDERTARGET;//0;
	//�e�N�X�`���̃t�H�[�}�b�g
	dxtex[ txnum ].format = D3DFMT_A8R8G8B8;//D3DFMT_UNKNOWN;
	//�������Ǘ����@
	dxtex[ txnum ].pool = D3DPOOL_DEFAULT;//D3DPOOL_MANAGED;
	//�t�B���^�����O�̕��@
	dxtex[ txnum ].filter = D3DX_FILTER_NONE;//D3DX_DEFAULT;
	//�~�b�v�t�B���^�����O�̕��@
	dxtex[ txnum ].mipfilter = D3DX_FILTER_LINEAR;//D3DX_DEFAULT;
	//���ߐF
	dxtex[ txnum ].color = tcolor;
	//D3DXIMAGE_INFO�\���̂ւ̃|�C���^
	dxtex[ txnum ].imgdata = NULL;
	//PALETTEENTRY�\���̂ւ̃|�C���^
	dxtex[ txnum ].pallete = NULL;
	//���_�̐F
	dxtex[ txnum ].vcolor[ 0 ] = dxtex[ txnum ].vcolor[ 1 ] = dxtex[ txnum ].vcolor[ 2 ] = dxtex[ txnum ].vcolor[ 3 ] = 0xFFFFFFFF;

	//DirectXDraw�N���X�ւ̃|�C���^
	in->dxd = this;
	//�e�N�X�`���ԍ�
	in->num = txnum;

	if ( thread )
	{
		in->mode = 1;
		//�e�N�X�`�������X���b�h�Ɏc���Ƃ�C����
		CreateThread( NULL, 0, ThreadCreateTexture, (void *)in, 0, &tid );
	} else
	{
		in->mode = 0;
		//�X���b�h�͍�炸�����ď������s���B
		return ThreadCreateTexture( (void *)in );
	}
	return 0;
}

/*!
@brief ���\�[�X��ǂݍ���Ńe�N�X�`�������
@par   �֐�����
�摜�t�@�C�����w�肵�ăe�N�X�`�������B
@param txnum �e�N�X�`���ԍ��B
@param module ���\�[�X�̂��郂�W���[���B�����t�@�C�����Ȃ��NULL�B
@param filename ���\�[�X���B
@param tcolor ���ߐF�BTRC_NONE�ŉ������Ȃ��BTRC_ZERO�ō���̐F�𓧉ߐF�Ɏw�肷��B�ȗ������ꍇTRC_NONE(�������Ȃ�)�B
@param thread �X���b�h�����t���O�B0�Ńe�N�X�`���������ɃX���b�h�����Ȃ��B�ȗ������1�ŁA�X���b�h�������ɃX���b�h�����B
@return 0=�e�N�X�`�������X���b�h�̐����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=���\�[�X�����Ȃ��B
@return 3=�t�@�C����񂪓ǂݎ��Ȃ������B
@return 4=�̈�m�ۂɎ��s�B
*/
int DirectXDraw9::CreateTexture( unsigned int txnum, HMODULE module, const char *resource, unsigned long tcolor, int thread )
{
	HRSRC hres;
	void *ptr;

	HRESULT hr;
	D3DXIMAGE_INFO image_info;
	struct THREADIN_TEX * in;
	unsigned long tid;

	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	if ( resource == NULL )
	{
		//���\�[�X�����Ȃ�
		return 2;
	}

	hres = FindResource( module/*NULL*/, resource, "IMAGE" );
	ptr = LockResource( LoadResource( module/*NULL*/, hres ) );

	//  hr = D3DXGetImageInfoFromResource( module, resource, &image_info );
	hr = D3DXGetImageInfoFromFileInMemory( ptr, SizeofResource( module/*NULL*/, hres ), &image_info );
	if ( FAILED( hr ) )
	{
		//�t�@�C����񂪓ǂݎ��Ȃ�����
		return 3;
	}
	//�e�N�X�`�������̂ŁA�g�p�s�ɂ���
	dxtex[ txnum ].ext = 0;

	//�e�N�X�`���T�C�Y�̎擾
	dxtex[ txnum ].width = image_info.Width;
	dxtex[ txnum ].height = image_info.Height;
	//���\�[�X�̐ݒ�
	dxtex[ txnum ].module = module;
	dxtex[ txnum ].resource = 1;

	//�X���b�h�ɓn���\���̗̂̈�m��
	in = ( struct THREADIN_TEX * )calloc( 1, sizeof( struct THREADIN_TEX ) );
	in->resouce = ptr;
	in->resoucesize = SizeofResource( module/*NULL*/, hres );

	if ( in == NULL )
	{
		//�̈�m�ۂɎ��s
		return 4;
	}

	if ( dxtex[ txnum ].filename == NULL )
	{
		//�t�@�C���������镶����̈���m�ۂ���
		dxtex[ txnum ].filename = (char *)calloc( strlen( resource ) + 1, sizeof( char ) );
		//�t�@�C�������R�s�[����
		memcpy( dxtex[ txnum ].filename, resource, strlen( resource ) );
	}

	//�e�N�X�`���̐ݒ���s���B

	//�~�b�v���x���̐�
	dxtex[ txnum ].miplevel = 1;//D3DX_DEFAULT;
	//�e�N�X�`���̎g�p�ړI
	dxtex[ txnum ].usage = D3DUSAGE_RENDERTARGET;//0;
	//�e�N�X�`���̃t�H�[�}�b�g
	dxtex[ txnum ].format = D3DFMT_A8R8G8B8;//D3DFMT_UNKNOWN;
	//�������Ǘ����@
	dxtex[ txnum ].pool = D3DPOOL_DEFAULT;//D3DPOOL_MANAGED;
	//�t�B���^�����O�̕��@
	dxtex[ txnum ].filter = D3DX_FILTER_NONE;//D3DX_DEFAULT;
	//�~�b�v�t�B���^�����O�̕��@
	dxtex[ txnum ].mipfilter = D3DX_FILTER_LINEAR;//D3DX_DEFAULT;
	//���ߐF
	dxtex[ txnum ].color = tcolor;
	//D3DXIMAGE_INFO�\���̂ւ̃|�C���^
	dxtex[ txnum ].imgdata = NULL;
	//PALETTEENTRY�\���̂ւ̃|�C���^
	dxtex[ txnum ].pallete = NULL;
	//���_�̐F
	dxtex[ txnum ].vcolor[ 0 ] = dxtex[ txnum ].vcolor[ 1 ] = dxtex[ txnum ].vcolor[ 2 ] = dxtex[ txnum ].vcolor[ 3 ] = 0xFFFFFFFF;

	//DirectXDraw�N���X�ւ̃|�C���^
	in->dxd = this;
	//�e�N�X�`���ԍ�
	in->num = txnum;

	if ( thread )
	{
		in->mode = 1;
		//�e�N�X�`�������X���b�h�Ɏc���Ƃ�C����
		CreateThread( NULL, 0, ThreadCreateTextureFromResorce, (void *)in, 0, &tid );
	} else
	{
		in->mode = 0;
		//�X���b�h�͍�炸�����ď������s���B
		return ThreadCreateTextureFromResorce( (void *)in );
	}
	return 0;
}

/*!
@brief ��̃e�N�X�`�������
@par   �֐�����
��̃e�N�X�`�������B
@param txnum �e�N�X�`���ԍ��B
@param width �e�N�X�`���̉����B
@param height �e�N�X�`���̍����B
@param thread �X���b�h�����t���O�B0�Ńe�N�X�`���������ɃX���b�h�����Ȃ��B�ȗ������1�ŁA�X���b�h�������ɃX���b�h�����B
@return 0=�e�N�X�`�������X���b�h�̐����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�ǂ��炩�̑傫����0�B
@return 3=�̈�m�ۂɎ��s�B
*/
int DirectXDraw9::CreateTexture( unsigned int txnum, unsigned int width, unsigned int height, int thread )
{
	struct THREADIN_TEX * in;
	unsigned long tid;

	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	if ( width == 0 || height == 0 )
	{
		//�T�C�Y���s��
		return 2;
	}

	//�e�N�X�`�������̂ŁA�g�p�s�ɂ���
	dxtex[ txnum ].ext = 0;

	//�e�N�X�`���T�C�Y�̎擾
	dxtex[ txnum ].width = width;
	dxtex[ txnum ].height = height;
	//���\�[�X�̐ݒ�
	dxtex[ txnum ].module = NULL;
	dxtex[ txnum ].resource = 0;

	//�X���b�h�ɓn���\���̗̂̈�m��
	in = ( struct THREADIN_TEX * )calloc( 1, sizeof( struct THREADIN_TEX ) );

	if ( in == NULL )
	{
		//�̈�m�ۂɎ��s
		return 3;
	}

	//�e�N�X�`���̐ݒ���s���B

	//�~�b�v���x���̐�
	dxtex[ txnum ].miplevel = 1;//D3DX_DEFAULT;
	//�e�N�X�`���̎g�p�ړI
	dxtex[ txnum ].usage = D3DUSAGE_RENDERTARGET;//0;
	//�e�N�X�`���̃t�H�[�}�b�g
	dxtex[ txnum ].format = D3DFMT_A8R8G8B8;//D3DFMT_UNKNOWN;
	//�������Ǘ����@
	dxtex[ txnum ].pool = D3DPOOL_DEFAULT;
	//���_�̐F
	dxtex[ txnum ].vcolor[ 0 ] = dxtex[ txnum ].vcolor[ 1 ] = dxtex[ txnum ].vcolor[ 2 ] = dxtex[ txnum ].vcolor[ 3 ] = 0xffffffff;

	//�ȉ�4�̒l�͎g��Ȃ��B
	//�t�B���^�����O�̕��@
	//����l���v�]�B�e�N�X�`���͈̔͊O��`�悵���Ƃ��ɍ��͌J��Ԃ������A�Ⴆ��X�����������J��Ԃ��Ƃ��A�ʂ̐F�œh��Ԃ��I�v�V�������~�����炵���B
	//�����̃t�B���^�����O��ς���΁A���̊�]�����������B
	dxtex[ txnum ].filter = D3DX_FILTER_NONE;//NULL;
	//�~�b�v�t�B���^�����O�̕��@
	dxtex[ txnum ].mipfilter = D3DX_FILTER_LINEAR;//NULL;
	//D3DXIMAGE_INFO�\���̂ւ̃|�C���^
	dxtex[ txnum ].imgdata = NULL;
	//PALETTEENTRY�\���̂ւ̃|�C���^
	dxtex[ txnum ].pallete = NULL;

	//DirectXDraw�N���X�ւ̃|�C���^
	in->dxd = this;
	//�e�N�X�`���ԍ�
	in->num = txnum;

	if ( thread )
	{
		in->mode = 1;
		//�e�N�X�`�������X���b�h�Ɏc���Ƃ�C����
		CreateThread( NULL, 0, ThreadCreateTextureEmpty, (void *)in, 0, &tid );
	} else
	{
		in->mode = 0;
		return ThreadCreateTextureEmpty( (void *)in );
	}

	return 0;
}

/*!
@brief ���Ńe�N�X�`�������B
@par   �֐�����
�e�N�X�`�������X���b�h�B
@param threadin �X���b�h�ɓn���\���̂ւ̃|�C���^�B
@return 0=�e�N�X�`���̐����ɐ����B
@return 1=�����ǂݍ��݂Ɏ��s�B
@return 2=�T�[�t�F�C�X�̍쐬�Ɏ��s�B
@return 3=�t�@�C���̃T�[�t�F�C�X�ւ̓ǂݍ��݂Ɏ��s�B
@return 4=�T�[�t�F�C�X�̃��b�N�Ɏ��s�B
@return 5=�e�N�X�`�������Ɏ��s�B
*/
unsigned long WINAPI DirectXDraw9::ThreadCreateTexture( void * threadin )
{
	HRESULT hr;
	//�_�擾�̂��߂̃T�[�t�F�C�X
	IDirect3DSurface9 *surface;
	//��`�̈�
	RECT cutrect;
	//���b�N�����`�̈�
	D3DLOCKED_RECT lockrect;
	//DirectXDraw�N���X�ւ̃|�C���^
	CLASSDEF_DIRECTX_DRAW * dxd = NULL;
	//�e�N�X�`���ԍ�
	int num = 0;
	int ret = 0;
	int mode = 0;

	if ( threadin == NULL )
	{
		//�����ǂݍ��݂Ɏ��s
		return 1;
	}

	dxd = ( ( struct THREADIN_TEX * )( threadin ) )->dxd;
	num = ( ( struct THREADIN_TEX * )( threadin ) )->num;
	mode = ( ( struct THREADIN_TEX * )( threadin ) )->mode;

	if ( dxd->dxtex[ num ].color == TRC_ZERO )
	{

		//�摜�擾�̈�̍쐬
		cutrect.left = cutrect.top = 0;
		cutrect.right = cutrect.bottom = 1;
		//��̃T�[�t�F�C�X�̍쐬
		if ( SUCCEEDED( dxd->D3DDev->CreateOffscreenPlainSurface( 1, 1, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL ) ) )
		{
			//�摜�̍���1px���T�[�t�F�C�X�ɓǂݍ���
			if ( SUCCEEDED( D3DXLoadSurfaceFromFile( surface, NULL, NULL, dxd->dxtex[ num ].filename, &cutrect, D3DX_FILTER_NONE, 0, NULL ) ) )
			{
				//�T�[�t�F�C�X�̃��b�N
				if ( SUCCEEDED( surface->LockRect( &lockrect, &cutrect, D3DLOCK_READONLY ) ) )
				{
					//����̐F���擾
					dxd->dxtex[ num ].color = *( (unsigned long *)lockrect.pBits );
					//�T�[�t�F�C�X�̃A�����b�N
					surface->UnlockRect();
				} else
				{
					//�T�[�t�F�C�X�̃��b�N�Ɏ��s
					ret = 4;
				}
				//�T�[�t�F�C�X�̉��
				DX_RELEASE( surface );
			} else
			{
				//�t�@�C���̃T�[�t�F�C�X�ւ̓ǂݍ��݂Ɏ��s
				ret = 3;
			}
		} else
		{
			//�T�[�t�F�C�X�̍쐬�Ɏ��s
			ret = 2;
		}
	}

	hr = D3DXCreateTextureFromFileEx(
		dxd->D3DDev,                 //D3D�f�o�C�X
		dxd->dxtex[ num ].filename,  //�t�@�C����
		dxd->dxtex[ num ].width,     //�e�N�X�`���T�C�Y
		dxd->dxtex[ num ].height,    //�e�N�X�`���T�C�Y
		dxd->dxtex[ num ].miplevel,  //�~�b�v���x���̐�
		dxd->dxtex[ num ].usage,     //�e�N�X�`���̎g�p�ړI
		dxd->dxtex[ num ].format,    //�e�N�X�`���̃t�H�[�}�b�g
		dxd->dxtex[ num ].pool,      //�������Ǘ����@
		dxd->dxtex[ num ].filter,    //�t�B���^�����O�̕��@
		dxd->dxtex[ num ].mipfilter, //�~�b�v�t�B���^�����O�̕��@
		dxd->dxtex[ num ].color,     //���ߐF
		dxd->dxtex[ num ].imgdata,   //D3DXIMAGE_INFO�\���̂ւ̃|�C���^
		dxd->dxtex[ num ].pallete,   //PALETTEENTRY�\���̂ւ̃|�C���^
		&( dxd->dxtex[ num ].tex )     //�e�N�X�`�����󂯎��IDirect3DTexture9�ւ̃|�C���^
		);

	//�\���̂̉��
	DX_FREE( threadin );

	if ( FAILED( hr ) )
	{
		//�e�N�X�`�������Ɏ��s
		return 5;
	}

	dxd->dxtex[ num ].ext = 1;
	dxd->dxtex[ num ].lock = 0;


	if ( mode )
	{
		ExitThread( 0 );
	}
	return ret;
}

/*!
@brief ���Ńe�N�X�`�������B
@par   �֐�����
�e�N�X�`�������X���b�h�B
@param threadin �X���b�h�ɓn���\���̂ւ̃|�C���^�B
@return 0=�e�N�X�`���̐����ɐ����B
@return 1=�����ǂݍ��݂Ɏ��s�B
@return 2=�T�[�t�F�C�X�̍쐬�Ɏ��s�B
@return 3=�t�@�C���̃T�[�t�F�C�X�ւ̓ǂݍ��݂Ɏ��s�B
@return 4=�T�[�t�F�C�X�̃��b�N�Ɏ��s�B
@return 5=�e�N�X�`�������Ɏ��s�B
*/
unsigned long WINAPI DirectXDraw9::ThreadCreateTextureFromResorce( void * threadin )
{
	HRESULT hr;
	//�_�擾�̂��߂̃T�[�t�F�C�X
	IDirect3DSurface9 *surface;
	//��`�̈�
	RECT cutrect;
	//���b�N�����`�̈�
	D3DLOCKED_RECT lockrect;
	//DirectXDraw�N���X�ւ̃|�C���^
	CLASSDEF_DIRECTX_DRAW * dxd = NULL;
	void *ptr;
	unsigned int size;
	//�e�N�X�`���ԍ�
	int num = 0;
	int ret = 0;
	int mode = 0;

	if ( threadin == NULL )
	{
		//�����ǂݍ��݂Ɏ��s
		return 1;
	}

	dxd = ( ( struct THREADIN_TEX * )( threadin ) )->dxd;
	num = ( ( struct THREADIN_TEX * )( threadin ) )->num;
	mode = ( ( struct THREADIN_TEX * )( threadin ) )->mode;
	ptr = ( ( struct THREADIN_TEX * )( threadin ) )->resouce;
	size = ( ( struct THREADIN_TEX * )( threadin ) )->resoucesize;

	if ( dxd->dxtex[ num ].color == TRC_ZERO )
	{

		//�摜�擾�̈�̍쐬
		cutrect.left = cutrect.top = 0;
		cutrect.right = cutrect.bottom = 1;
		//��̃T�[�t�F�C�X�̍쐬
		if ( SUCCEEDED( dxd->D3DDev->CreateOffscreenPlainSurface( 1, 1, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL ) ) )
		{
			//�摜�̍���1px���T�[�t�F�C�X�ɓǂݍ���
			//      if( SUCCEEDED( D3DXLoadSurfaceFromFileInMemory( surface, NULL, NULL, dxd->dxtex[ num ].module, dxd->dxtex[ num ].filename, &cutrect, D3DX_FILTER_NONE, 0, NULL ) ) )
			if ( SUCCEEDED( D3DXLoadSurfaceFromFileInMemory( surface, NULL, NULL, ptr, size, &cutrect, D3DX_FILTER_NONE, 0, NULL ) ) )
			{
				//�T�[�t�F�C�X�̃��b�N
				if ( SUCCEEDED( surface->LockRect( &lockrect, &cutrect, D3DLOCK_READONLY ) ) )
				{
					//����̐F���擾
					dxd->dxtex[ num ].color = *( (unsigned long *)lockrect.pBits );
					//�T�[�t�F�C�X�̃A�����b�N
					surface->UnlockRect();
				} else
				{
					//�T�[�t�F�C�X�̃��b�N�Ɏ��s
					ret = 4;
				}
				//�T�[�t�F�C�X�̉��
				DX_RELEASE( surface );
			} else
			{
				//�t�@�C���̃T�[�t�F�C�X�ւ̓ǂݍ��݂Ɏ��s
				ret = 3;
			}
		} else
		{
			//�T�[�t�F�C�X�̍쐬�Ɏ��s
			ret = 2;
		}
	}

	//  hr = D3DXCreateTextureFromResourceEx(
	//    dxd->D3DDev,                 //D3D�f�o�C�X
	//    dxd->dxtex[ num ].module,    //���W���[��
	//    dxd->dxtex[ num ].filename,  //���\�[�X��
	//    dxd->dxtex[ num ].width,     //�e�N�X�`���T�C�Y
	//    dxd->dxtex[ num ].height,    //�e�N�X�`���T�C�Y
	//    dxd->dxtex[ num ].miplevel,  //�~�b�v���x���̐�
	//    dxd->dxtex[ num ].usage,     //�e�N�X�`���̎g�p�ړI
	//    dxd->dxtex[ num ].format,    //�e�N�X�`���̃t�H�[�}�b�g
	//    dxd->dxtex[ num ].pool,      //�������Ǘ����@
	//    dxd->dxtex[ num ].filter,    //�t�B���^�����O�̕��@
	//    dxd->dxtex[ num ].mipfilter, //�~�b�v�t�B���^�����O�̕��@
	//    dxd->dxtex[ num ].color,     //���ߐF
	//    dxd->dxtex[ num ].imgdata,   //D3DXIMAGE_INFO�\���̂ւ̃|�C���^
	//    dxd->dxtex[ num ].pallete,   //PALETTEENTRY�\���̂ւ̃|�C���^
	//    &(dxd->dxtex[ num ].tex)     //�e�N�X�`�����󂯎��IDirect3DTexture9�ւ̃|�C���^
	//    );
	hr = D3DXCreateTextureFromFileInMemoryEx(
		dxd->D3DDev,                 //D3D�f�o�C�X
		ptr,                         //���\�[�X�ւ̃|�C���^
		size,                        //���\�[�X�̃T�C�Y
		dxd->dxtex[ num ].width,     //�e�N�X�`���T�C�Y
		dxd->dxtex[ num ].height,    //�e�N�X�`���T�C�Y
		dxd->dxtex[ num ].miplevel,  //�~�b�v���x���̐�
		dxd->dxtex[ num ].usage,     //�e�N�X�`���̎g�p�ړI
		dxd->dxtex[ num ].format,    //�e�N�X�`���̃t�H�[�}�b�g
		dxd->dxtex[ num ].pool,      //�������Ǘ����@
		dxd->dxtex[ num ].filter,    //�t�B���^�����O�̕��@
		dxd->dxtex[ num ].mipfilter, //�~�b�v�t�B���^�����O�̕��@
		dxd->dxtex[ num ].color,     //���ߐF
		dxd->dxtex[ num ].imgdata,   //D3DXIMAGE_INFO�\���̂ւ̃|�C���^
		dxd->dxtex[ num ].pallete,   //PALETTEENTRY�\���̂ւ̃|�C���^
		&( dxd->dxtex[ num ].tex )     //�e�N�X�`�����󂯎��IDirect3DTexture9�ւ̃|�C���^
		);
	//�\���̂̉��
	DX_FREE( threadin );

	if ( FAILED( hr ) )
	{
		//�e�N�X�`�������Ɏ��s
		return 5;
	}

	dxd->dxtex[ num ].ext = 1;
	dxd->dxtex[ num ].lock = 0;

	if ( mode )
	{
		ExitThread( 0 );
	}
	return ret;
}

/*!
@brief ���ŋ�̃e�N�X�`�������B
@par   �֐�����
�e�N�X�`�������X���b�h�B
@param threadin �X���b�h�ɓn���\���̂ւ̃|�C���^�B
@return 0=�e�N�X�`���̐����ɐ����B
@return 1=�����ǂݍ��݂Ɏ��s�B
@return 5=�e�N�X�`�������Ɏ��s�B
*/
unsigned long WINAPI DirectXDraw9::ThreadCreateTextureEmpty( void * threadin )
{
	HRESULT hr;
	//DirectXDraw�N���X�ւ̃|�C���^
	CLASSDEF_DIRECTX_DRAW * dxd = NULL;
	//�e�N�X�`���ԍ�
	int num = 0;
	int ret = 0;
	int mode = 0;

	if ( threadin == NULL )
	{
		//�����ǂݍ��݂Ɏ��s
		return 1;
	}

	dxd = ( ( struct THREADIN_TEX * )( threadin ) )->dxd;
	num = ( ( struct THREADIN_TEX * )( threadin ) )->num;
	mode = ( ( struct THREADIN_TEX * )( threadin ) )->mode;

	hr = D3DXCreateTexture(
		dxd->D3DDev,                 //D3D�f�o�C�X
		dxd->dxtex[ num ].width,     //�e�N�X�`���T�C�Y
		dxd->dxtex[ num ].height,    //�e�N�X�`���T�C�Y
		dxd->dxtex[ num ].miplevel,  //�~�b�v���x���̐�
		dxd->dxtex[ num ].usage,     //�e�N�X�`���̎g�p�ړI
		dxd->dxtex[ num ].format,    //�e�N�X�`���̃t�H�[�}�b�g
		dxd->dxtex[ num ].pool,      //�������Ǘ����@
		&( dxd->dxtex[ num ].tex )     //�e�N�X�`�����󂯎��IDirect3DTexture9�ւ̃|�C���^
		);

	//�\���̂̉��
	DX_FREE( threadin );

	if ( FAILED( hr ) )
	{
		//�e�N�X�`�������Ɏ��s
		return 5;
	}

	dxd->dxtex[ num ].ext = 1;
	dxd->dxtex[ num ].lock = 0;

	if ( mode )
	{
		ExitThread( 0 );
	}
	return ret;
}

/*!
@brief �e�N�X�`���̉��
@par   �֐�����
�e�N�X�`�����������B
@param txnum �e�N�X�`���ԍ��B
@return 0��Ԃ��B
*/
int DirectXDraw9::ReleaseTexture( unsigned int txnum )
{
	if ( CannotUseTexture( txnum ) )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	if ( txnum < texmax )
	{
		dxtex[ txnum ].ext = 0;
		dxtex[ txnum ].lock = 0;
		DX_FREE( dxtex[ txnum ].filename );
		DX_FREE( dxtex[ txnum ].pallete );
		DX_FREE( dxtex[ txnum ].imgdata );
		DX_RELEASE( dxtex[ txnum ].tex );
		DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
		DX_RELEASE( dxtex[ txnum ].sur[ 1 ] );
		DX_RELEASE( dxtex[ txnum ].zsur );
		DX_RELEASE( dxtex[ txnum ].backup );
	}
	return 0;
}

/*!
@brief �e�N�X�`���̉��
@par   �֐�����
�S�e�N�X�`�����������B
@return �J�������e�N�X�`������Ԃ��B
*/
int DirectXDraw9::ReleaseTexture()
{
	int ret = 0;
	unsigned int i;

	for ( i = 0; i < texmax; ++i )
	{
		if ( ReleaseTexture( i ) == 0 )
		{
			++ret;
		}
	}

	return ret;
}

//���p�`�̉��
int DirectXDraw9::ReleasePolygon( unsigned int pvnum )
{
	struct CUSTOMVERTEX_LIST *tmp, *del;
	if ( pvnum <= pvmax )
	{
		if ( pv[ pvnum ].type == 1 )
		{
			// �z��
			DX_FREE( pv[ pvnum ].vertex.cv );
		} else if ( pv[ pvnum ].type == 2 )
		{
			// ���X�g
			tmp = pv[ pvnum ].vertex.cvl;
			while ( tmp )
			{
				del = tmp;
				tmp = tmp->next;
				DX_FREE( del );
			}
		}
		DX_FREE( pv[ pvnum ].cv );
		pv[ pvnum ].cvmax = 0;
		pv[ pvnum ].max = 0;
		pv[ pvnum ].read.num = 0;
		pv[ pvnum ].read.pointer = NULL;
		pv[ pvnum ].type = 0;

	}
	return 0;
}


//���p�`�̓o�^(���X�g)
int DirectXDraw9::CreatePolygon( unsigned int pnum, D3DPRIMITIVETYPE filltype )
{
	if ( pnum > pvmax )
	{
		// �ԍ����s���B
		return 1;
	}

	lastpvnum = pnum;

	// ���łɑO�̂����肻���Ȃ̂ŊJ����ƁB
	ReleasePolygon( pnum );

	// ���X�g�Ƃ��đ��p�`��o�^���Ă����B
	pv[ pnum ].type = 2;

	pv[ pnum ].max = 0;

	pv[ pnum ].vertex.cvl = NULL;

	pv[ pnum ].txnum = texmax;

	switch ( filltype )
	{
	case D3DPT_POINTLIST:
		pv[ pnum ].filltype = D3DPT_POINTLIST;
		break;
	case D3DPT_LINELIST:
		pv[ pnum ].filltype = D3DPT_LINELIST;
		break;
	case D3DPT_LINESTRIP:
		pv[ pnum ].filltype = D3DPT_LINESTRIP;
		break;
	case D3DPT_TRIANGLELIST:
		pv[ pnum ].filltype = D3DPT_TRIANGLELIST;
		break;
	case D3DPT_TRIANGLESTRIP:
		pv[ pnum ].filltype = D3DPT_TRIANGLESTRIP;
		break;
	case D3DPT_TRIANGLEFAN:
		pv[ pnum ].filltype = D3DPT_TRIANGLEFAN;
		break;
	}
	return 0;
}

//���p�`�̓o�^(�z��)
int DirectXDraw9::CreatePolygon( unsigned int pnum, unsigned int max, D3DPRIMITIVETYPE filltype )
{
	if ( pnum > pvmax + 1 )
	{
		// �ԍ����s���B
		return 1;
	}

	lastpvnum = pnum;

	// �z��Ƃ��đ��p�`��o�^���Ă����B
	pv[ pnum ].type = 1;

	pv[ pnum ].max = max;

	pv[ pnum ].txnum = texmax;

	// �̈���������Ɗm�ہB
	pv[ pnum ].vertex.cv = ( struct CUSTOMVERTEX * )calloc( max, sizeof( struct CUSTOMVERTEX ) );

	switch ( filltype )
	{
	case D3DPT_POINTLIST:
		pv[ pnum ].filltype = D3DPT_POINTLIST;
		break;
	case D3DPT_LINELIST:
		pv[ pnum ].filltype = D3DPT_LINELIST;
		break;
	case D3DPT_LINESTRIP:
		pv[ pnum ].filltype = D3DPT_LINESTRIP;
		break;
	case D3DPT_TRIANGLELIST:
		pv[ pnum ].filltype = D3DPT_TRIANGLELIST;
		break;
	case D3DPT_TRIANGLESTRIP:
		pv[ pnum ].filltype = D3DPT_TRIANGLESTRIP;
		break;
	case D3DPT_TRIANGLEFAN:
		pv[ pnum ].filltype = D3DPT_TRIANGLEFAN;
		break;
	}

	return 0;
}

//�����p�`�̓o�^
int DirectXDraw9::CreateRegularPolygon( unsigned int pnum, double cx, double cy, unsigned int n, double r, double rad, unsigned long color )
{
	double arad;
	unsigned i;
	CreatePolygon( pnum, n + 2, D3DPT_TRIANGLEFAN );

	// �������W
	SetPolygonVertex( pnum, cx, cy, color );

	arad = AngleToRadian( 360.0 / (double)n );

	for ( i = 0; i <= n; ++i )
	{
		SetPolygonVertex( pnum,
			cx + cos( rad + arad * i ) * r - 0.5,
			cy + sin( rad + arad * i ) * r - 0.5,
			color );
	}


	return 0;
}

// �e�N�X�`���̓o�^
int DirectXDraw9::SetTextureOnPolygon( unsigned int pnum, unsigned int tnum )
{
	if ( pnum > pvmax )
	{
		// �|���S���ԍ����s��
		return 1;
	}

	if ( CannotUseTexture( tnum ) )//if( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 2;
	}

	pv[ pnum ].txnum = tnum;

	return 0;
}

// �e�N�X�`���̓o�^����
int DirectXDraw9::SetTextureOnPolygon( unsigned int pnum )
{
	if ( pnum > pvmax )
	{
		// �|���S���ԍ����s��
		return 1;
	}

	pv[ pnum ].txnum = texmax;

	return 0;
}

// �e�N�X�`���ԍ��̎擾�B
unsigned int DirectXDraw9::GetTextureNumOnPolygon( unsigned int pnum )
{
	if ( pnum > pvmax )
	{
		// �|���S���ԍ����s��
		return texmax;
	}
	return pv[ pnum ].txnum;
}

// UV���W�̓o�^�B
int DirectXDraw9::SetPolygonUV( unsigned int pnum, unsigned int vnum, int u, int v )
{
	struct CUSTOMVERTEX_LIST *next;
	double w, h;

	if ( pnum > pvmax )
	{
		// �ԍ����s���Ȃ̂ōŌ�̗̈���g���B
		pnum = pvmax;
	}

	if ( vnum >= pv[ pnum ].max )
	{
		// ���_�����s���B
		return 1;
	}

	if ( pv[ pnum ].txnum >= texmax )
	{
		return 2;
	}
	w = (double)GetTextureWidth( pv[ pnum ].txnum );
	h = (double)GetTextureHeight( pv[ pnum ].txnum );

	if ( pv[ pnum ].type == 1 )
	{
		// �z��
		pv[ pnum ].vertex.cv[ vnum ].u = (float)( (double)u / w );
		pv[ pnum ].vertex.cv[ vnum ].v = (float)( (double)v / h );

	} else if ( pv[ pnum ].type == 2 )
	{
		// ���X�g
		next = pv[ pnum ].vertex.cvl;
		while ( vnum-- > 0 )
		{
			next = next->next;
		}

		next->u = (float)( (double)u / w );
		next->v = (float)( (double)v / h );
	}

	return 0;
}

//���_�̓o�^(�z�����)
int DirectXDraw9::SetPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color )
{
	if ( pnum > pvmax )
	{
		// �ԍ����s���Ȃ̂ōŌ�̗̈���g���B
		return -1;
	}

	if ( pv[ pnum ].type != 1 )
	{
		// �z�񂶂�Ȃ��̂ŕs���B
		return -2;
	}

	if ( pv[ pnum ].max <= vnum )
	{
		// ���_�ԍ����z��T�C�Y�𒴂��Ă�B
		return -3;
	}

	lastpvnum = vnum;

	// �z��B
	if ( pv[ pnum ].read.num == 0 )
	{
		pv[ pnum ].space[ 0 ] = x;
		pv[ pnum ].space[ 1 ] = x;
		pv[ pnum ].space[ 2 ] = y;
		pv[ pnum ].space[ 3 ] = y;
		pv[ pnum ].space[ 4 ] = 0.5;
		pv[ pnum ].space[ 5 ] = 0.5;
	} else
	{
		if ( x < pv[ pnum ].space[ 0 ] )
		{
			pv[ pnum ].space[ 0 ] = x;
		}
		if ( pv[ pnum ].space[ 1 ] < x )
		{
			pv[ pnum ].space[ 1 ] = x;
		}
		if ( y < pv[ pnum ].space[ 2 ] )
		{
			pv[ pnum ].space[ 2 ] = y;
		}
		if ( pv[ pnum ].space[ 3 ] < y )
		{
			pv[ pnum ].space[ 3 ] = y;
		}
	}

	if ( pv[ pnum ].read.num < pv[ pnum ].max )
	{
		//n = pv[ pnum ].read.num;
		pv[ pnum ].vertex.cv[ vnum ].x = (float)x - 0.5f;
		pv[ pnum ].vertex.cv[ vnum ].y = (float)y - 0.5f;
		pv[ pnum ].vertex.cv[ vnum ].z = ( float )0.5;
		pv[ pnum ].vertex.cv[ vnum ].color = color;
#ifdef USERHW
		pv[ pnum ].vertex.cv[ vnum ].rhw = 1.0f;
#endif
		pv[ pnum ].vertex.cv[ vnum ].u = 0.0f;
		pv[ pnum ].vertex.cv[ vnum ].v = 0.0f;
		//++pv[ pnum ].read.num;
		if ( pv[ pnum ].read.num < vnum )
		{
			pv[ pnum ].read.num = vnum;
		}
	}

	return 0;
}

//���_�̓o�^
int DirectXDraw9::SetPolygonVertex( unsigned int pnum, double x, double y, unsigned long color )
{
	int n;
	if ( pnum > pvmax )
	{
		// �ԍ����s���Ȃ̂ōŌ�̗̈���g���B
		pnum = pvmax;
	}

	lastpvnum = pnum;

	if ( pv[ pnum ].type == 1 )
	{
		// �z��B
		if ( pv[ pnum ].read.num == 0 )
		{
			pv[ pnum ].space[ 0 ] = x;
			pv[ pnum ].space[ 1 ] = x;
			pv[ pnum ].space[ 2 ] = y;
			pv[ pnum ].space[ 3 ] = y;
			pv[ pnum ].space[ 4 ] = 0.5;
			pv[ pnum ].space[ 5 ] = 0.5;
		} else
		{
			if ( x < pv[ pnum ].space[ 0 ] )
			{
				pv[ pnum ].space[ 0 ] = x;
			}
			if ( pv[ pnum ].space[ 1 ] < x )
			{
				pv[ pnum ].space[ 1 ] = x;
			}
			if ( y < pv[ pnum ].space[ 2 ] )
			{
				pv[ pnum ].space[ 2 ] = y;
			}
			if ( pv[ pnum ].space[ 3 ] < y )
			{
				pv[ pnum ].space[ 3 ] = y;
			}
		}

		if ( pv[ pnum ].read.num < pv[ pnum ].max )
		{
			n = pv[ pnum ].read.num;
			pv[ pnum ].vertex.cv[ n ].x = (float)x - 0.5f;
			pv[ pnum ].vertex.cv[ n ].y = (float)y - 0.5f;
			pv[ pnum ].vertex.cv[ n ].z = ( float )0.5;
			pv[ pnum ].vertex.cv[ n ].color = color;
#ifdef USERHW
			pv[ pnum ].vertex.cv[ n ].rhw = 1.0f;
#endif
			pv[ pnum ].vertex.cv[ n ].u = 0.0f;
			pv[ pnum ].vertex.cv[ n ].v = 0.0f;
			++pv[ pnum ].read.num;
		}
	} else if ( pv[ pnum ].type == 2 )
	{
		// ���X�g�B
		if ( pv[ pnum ].read.pointer )
		{
			if ( x < pv[ pnum ].space[ 0 ] )
			{
				pv[ pnum ].space[ 0 ] = x;
			}
			if ( pv[ pnum ].space[ 1 ] < x )
			{
				pv[ pnum ].space[ 1 ] = x;
			}
			if ( y < pv[ pnum ].space[ 2 ] )
			{
				pv[ pnum ].space[ 2 ] = y;
			}
			if ( pv[ pnum ].space[ 3 ] < y )
			{
				pv[ pnum ].space[ 3 ] = y;
			}

			pv[ pnum ].read.pointer->next = ( struct CUSTOMVERTEX_LIST * )calloc( 1, sizeof( struct CUSTOMVERTEX_LIST ) );
			pv[ pnum ].read.pointer = pv[ pnum ].read.pointer->next;
		} else
		{
			pv[ pnum ].space[ 0 ] = x;
			pv[ pnum ].space[ 1 ] = x;
			pv[ pnum ].space[ 2 ] = y;
			pv[ pnum ].space[ 3 ] = y;

			pv[ pnum ].vertex.cvl = ( struct CUSTOMVERTEX_LIST * )calloc( 1, sizeof( struct CUSTOMVERTEX_LIST ) );
			pv[ pnum ].read.pointer = pv[ pnum ].vertex.cvl;

		}
		pv[ pnum ].read.pointer->x = (float)x - 0.5f;
		pv[ pnum ].read.pointer->y = (float)y - 0.5f;
		pv[ pnum ].read.pointer->z = ( float )0.5;
		pv[ pnum ].read.pointer->color = color;
#ifdef USERHW
		pv[ pnum ].read.pointer->rhw = 1.0f;
#endif
		pv[ pnum ].read.pointer->u = 0.0f;
		pv[ pnum ].read.pointer->v = 0.0f;
		++pv[ pnum ].max;
	}

	return 0;
}

//���_���̎擾
struct CUSTOMVERTEX * DirectXDraw9::GetPolygonVertex( unsigned int pnum, unsigned int vnum )
{
	struct CUSTOMVERTEX_LIST *next;
	if ( pnum > pvmax )
	{
		// �ԍ����s���Ȃ̂ōŌ�̗̈���g���B
		pnum = pvmax;
	}

	if ( vnum >= pv[ pnum ].max )
	{
		// ���_�����s���Ȃ̂�NULL��Ԃ��B
		return NULL;
	}

	if ( pv[ pnum ].type == 1 )
	{
		return &( pv[ pnum ].vertex.cv[ vnum ] );
	} else if ( pv[ pnum ].type == 2 )
	{
		next = pv[ pnum ].vertex.cvl;
		while ( vnum-- > 0 )
		{
			next = next->next;
		}
		return ( struct CUSTOMVERTEX * )next;
	}

	return 0;
}

// ���_�v�f����Ԃ�
unsigned int DirectXDraw9::GetPolygonVertexLength( unsigned int pnum )
{
	if ( pnum > pvmax )
	{
		// �ԍ����s��
		return 0;
	}
	// �z����g���Ă���ꍇ�͍ő吔�ł͂Ȃ��g�����v�f����Ԃ��B
	return pv[ pnum ].type == 1 ? pv[ pnum ].read.num : pv[ pnum ].max;
}

//���_���̕ҏW
int DirectXDraw9::EditPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color, double u, double v, int edit )
{
	struct CUSTOMVERTEX_LIST *next;
	if ( pnum > pvmax )
	{
		// �ԍ����s���Ȃ̂ōŌ�̗̈���g���B
		pnum = pvmax;
	}

	if ( vnum >= pv[ pnum ].max )
	{
		// ���_�����s���Ȃ̂�NULL��Ԃ��B
		return NULL;
	}

	if ( pv[ pnum ].type == 1 )
	{
		// �z��
		if ( edit & PVEDIT_X )
		{
			pv[ pnum ].vertex.cv[ vnum ].x = (float)x - 0.5f;
		}
		if ( edit & PVEDIT_Y )
		{
			pv[ pnum ].vertex.cv[ vnum ].y = (float)y - 0.5f;
		}
		if ( edit & PVEDIT_Z )
		{
			pv[ pnum ].vertex.cv[ vnum ].z = ( float )0.5;
		}
		if ( edit & PVEDIT_COLOR )
		{
			pv[ pnum ].vertex.cv[ vnum ].color = color;
		}
		if ( edit & PVEDIT_U )
		{
			if ( pv[ pnum ].txnum >= texmax )
			{
				pv[ pnum ].vertex.cv[ vnum ].u = (float)u / (float)GetTextureWidth( pv[ pnum ].txnum );
			} else
			{
				pv[ pnum ].vertex.cv[ vnum ].u = (float)u;
			}
		}
		if ( edit & PVEDIT_V )
		{
			if ( pv[ pnum ].txnum >= texmax )
			{
				pv[ pnum ].vertex.cv[ vnum ].v = (float)v / (float)GetTextureHeight( pv[ pnum ].txnum );
			} else
			{
				pv[ pnum ].vertex.cv[ vnum ].v = (float)v;
			}
		}

	} else if ( pv[ pnum ].type == 2 )
	{
		// ���X�g
		next = pv[ pnum ].vertex.cvl;
		while ( vnum-- > 0 )
		{
			next = next->next;
		}

		if ( edit & PVEDIT_X )
		{
			next->x = (float)x - 0.5f;
		}
		if ( edit & PVEDIT_Y )
		{
			next->y = (float)y - 0.5f;
		}
		if ( edit & PVEDIT_Z )
		{
			next->z = ( float )0.5;
		}
		if ( edit & PVEDIT_COLOR )
		{
			next->color = color;
		}
		if ( edit & PVEDIT_U )
		{
			next->u = (float)u;
		}
		if ( edit & PVEDIT_V )
		{
			next->v = (float)v;
		}
	}

	return 0;
}

/*int DirectXDraw9::SetPolygonVertex( double x, double y, unsigned long color )
{
return SetPolygonVertex( lastpvnum, x, y, color );
}*/

// ���p�`�̕ό`

// �ړ�
int DirectXDraw9::TransformPolygonMoving( unsigned int pnum, double mx, double my )
{
	unsigned int i;
	struct CUSTOMVERTEX_LIST *next;

	if ( pnum > pvmax )
	{
		// �ԍ����s���Ȃ̂ōŌ�̔ԍ����g���B
		pnum = pvmax;
	}

	if ( pv[ pnum ].type == 1 )
	{
		// �z��

		// ���W�ړ��B
		for ( i = 0; i < pv[ pnum ].read.num; ++i )
		{
			pv[ pnum ].vertex.cv[ i ].x += (float)mx;
			pv[ pnum ].vertex.cv[ i ].y += (float)my;
			//pv[ pnum ].vertex.cv[ i ].z += (float)mz;
		}
	} else if ( pv[ pnum ].type == 2 )
	{
		// ���X�g

		// �R�s�[�����W�ړ��B
		next = pv[ pnum ].vertex.cvl;
		for ( i = 0; i < pv[ pnum ].max; ++i )
		{
			next->x += (float)mx;
			next->y += (float)my;
			//next->z += (float)mz;
			next = next->next;
		}
	}

	return 0;
}

// �g��
int DirectXDraw9::TransformPolygonScaling( unsigned int pnum, double scale, unsigned int basenum )
{
	unsigned int i;
	double mx, my;
	struct CUSTOMVERTEX_LIST *next;
	D3DXMATRIX posmatrix, zoommatrix;

	// �s��̏������B
	D3DXMatrixIdentity( &posmatrix );
	D3DXMatrixIdentity( &zoommatrix );

	// �g�嗦�̒l��zoommatrix�s��ɑ������B
	D3DXMatrixScaling( &zoommatrix, (float)scale, (float)scale, 0.0 );

	if ( pnum > pvmax )
	{
		// �ԍ����s���Ȃ̂ōŌ�̔ԍ����g���B
		pnum = pvmax;
	}

	if ( pv[ pnum ].type == 1 )
	{
		// �z��

		// ����W�ւ̈ړ������B
		mx = pv[ pnum ].vertex.cv[ basenum ].x;
		my = pv[ pnum ].vertex.cv[ basenum ].y;

		// ���W���g��E�ړ�
		for ( i = 0; i < pv[ pnum ].read.num; ++i )
		{
			// ��_�����_�Ƃ������̒��_�̈ʒu���Z�o�B
			D3DXMatrixTranslation( &posmatrix, (float)( pv[ pnum ].vertex.cv[ i ].x - mx ), (float)( pv[ pnum ].vertex.cv[ i ].y - my ), (float)( pv[ pnum ].vertex.cv[ i ].z ) );
			// �s�񉉎Z�B
			posmatrix *= zoommatrix;

			pv[ pnum ].vertex.cv[ i ].x = posmatrix._41 - (float)mx;
			pv[ pnum ].vertex.cv[ i ].y = posmatrix._42 - (float)my;
			pv[ pnum ].vertex.cv[ i ].z = posmatrix._43;
		}
	} else if ( pv[ pnum ].type == 2 )
	{
		// ���X�g

		// ����W�𒲂ׂ�B
		next = pv[ pnum ].vertex.cvl;
		for ( i = 0; i < basenum; ++i )
		{
			next = next->next;
		}
		// ����W�ւ̈ړ������B
		mx = next->x;
		my = next->y;

		// �V�z��ɍ��W���g��E�ړ����R�s�[�B
		next = pv[ pnum ].vertex.cvl;
		for ( i = 0; i < pv[ pnum ].max; ++i )
		{
			// ��_�����_�Ƃ������̒��_�̈ʒu���Z�o�B
			D3DXMatrixTranslation( &posmatrix, (float)( pv[ pnum ].vertex.cv[ i ].x - mx ), (float)( pv[ pnum ].vertex.cv[ i ].y - my ), (float)( pv[ pnum ].vertex.cv[ i ].z ) );
			// �s�񉉎Z�B
			posmatrix *= zoommatrix;

			next->x = posmatrix._41 - (float)mx;
			next->y = posmatrix._42 - (float)my;
			next->z = posmatrix._43;
			next = next->next;
		}
	}

	return 0;
}

/*!
@brief �F�̍쐬
@par   �֐�����
Mikan(DirectX)�Ŏg����F�����B
@param alpha �A���t�@�l�B
@param red �ԁB
@param green �΁B
@param blue �B
@return �F�B0xAARRGGBB�B
*/
unsigned long DirectXDraw9::Color( unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue )
{
	return D3DCOLOR_ARGB( alpha, red, green, blue );
}

unsigned long DirectXDraw9::ColorHSV( unsigned char alpha, unsigned int hue, unsigned char sat, unsigned char vob )
{
	// TODO
	// ������
	return 0;
}

/*!
@brief �J���[�̐ݒ�
@par   �֐�����
�e�N�X�`������߂�F��ݒ肷��B4�F�����ݒ�B
@param txnum �e�N�X�`���ԍ��B
@param color �F�B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
*/
int DirectXDraw9::SetColor( unsigned int txnum, unsigned long color )
{
	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	dxtex[ txnum ].vcolor[ 3 ] = dxtex[ txnum ].vcolor[ 2 ] = dxtex[ txnum ].vcolor[ 1 ] = dxtex[ txnum ].vcolor[ 0 ] = color;
	return 0;
}

int DirectXDraw9::SetAlpha( unsigned int txnum, unsigned char alpha )
{
	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	dxtex[ txnum ].vcolor[ 0 ] = ( dxtex[ txnum ].vcolor[ 0 ] & 0x00ffffff ) | alpha << 24;
	dxtex[ txnum ].vcolor[ 1 ] = ( dxtex[ txnum ].vcolor[ 1 ] & 0x00ffffff ) | alpha << 24;
	dxtex[ txnum ].vcolor[ 2 ] = ( dxtex[ txnum ].vcolor[ 2 ] & 0x00ffffff ) | alpha << 24;
	dxtex[ txnum ].vcolor[ 3 ] = ( dxtex[ txnum ].vcolor[ 3 ] & 0x00ffffff ) | alpha << 24;
	return 0;
}

/*!
@brief ���_�J���[�̐ݒ�
@par   �֐�����
�e�N�X�`������߂�F��ݒ肷��B1�F�����̐ݒ�Ȃ̂ŁA4��g����4���_�ɕʁX�̐F��ݒ肷�鎖���ł���B
@param txnum �e�N�X�`���ԍ��B
@param color �F�B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
*/
int DirectXDraw9::SetColorVertex( unsigned int txnum, unsigned long color )
{
	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	dxtex[ txnum ].vcolor[ 3 ] = dxtex[ txnum ].vcolor[ 2 ];
	dxtex[ txnum ].vcolor[ 2 ] = dxtex[ txnum ].vcolor[ 1 ];
	dxtex[ txnum ].vcolor[ 1 ] = dxtex[ txnum ].vcolor[ 0 ];
	dxtex[ txnum ].vcolor[ 0 ] = color;
	return 0;
}

//�e�N�X�`���̃f�[�^��ǂ߂�悤�ɂ���B
int DirectXDraw9::OpenTexture( unsigned int txnum, int x, int y, int w, int h )
{
	//�e�N�X�`���͒��ɒl��ǂݏ����o���Ȃ��̂ŁA���̂悤�Ȏ菇�œǂݏ�������B
	//1.�e�N�X�`�����T�[�t�F�C�X�ɂ���(������ׂ��T�[�t�F�C�X�̐���)�B
	//2.�T�[�t�F�C�X�ɑ΂����b�N���|����B(OpenTexture�̍�Ƃ����܂�)
	//3.�T�[�t�F�C�X�ɑ΂��č�Ƃ��s���B
	//4.�T�[�t�F�C�X���A�����b�N����B(CloseTexture�̍��)
	//5.�T�[�t�F�C�X���e�N�X�`���ɏ����߂��B(CloseTexture�̍��)
	//���ɖʓ|���O���{���Â��ƑΉ����Ă��Ȃ��ꍇ������B
	//����ɁA���b�N���Ƃ��Ƃɔ��Ɏ��Ԃ�������̂ŁA�e�N�X�`������n�̊֐��͈ȉ��̂悤�ɍ�Ƃ���B
	//�p�^�[��1
	// �\��OpenTexture�����s����Ȃ��ꍇ�ɂ́A�J�n���Ɏ��s���ďI�����ɕ���B
	//�p�^�[��2
	// �\��OpenTexture�����s���ꂽ�ꍇ�ɂ͊J�n���ƏI�����ɉ��������A
	// CloseTexture�g�p���Ƀf�[�^�����ς���Ă�����e�N�X�`���ɏ����߂��B

	//�摜�擾�̈�̍쐬
	//  RECT cutrect;
	//�T�[�t�F�C�X�̃t�H�[�}�b�g
	D3DSURFACE_DESC desc;
	unsigned int size;

	//�e�N�X�`���`�F�b�N
	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}
	if ( w <= 0 )
	{
		w = dxtex[ txnum ].width;
	}
	if ( h <= 0 )
	{
		h = dxtex[ txnum ].height;
	}

	//�u���͈͂̐ݒ�
	dxtex[ txnum ].cutrect.left = x < 0 ? ( x = 0 ) : x;
	dxtex[ txnum ].cutrect.right = ( ( size = x + w ) > dxtex[ txnum ].width ) ? dxtex[ txnum ].width : size;//w <= 0 ? dxtex[ txnum ].width : x + w;
	dxtex[ txnum ].cutrect.top = y < 0 ? ( y = 0 ) : y;
	dxtex[ txnum ].cutrect.bottom = ( ( size = y + h ) > dxtex[ txnum ].height ) ? dxtex[ txnum ].height : size;//h <= 0 ? dxtex[ txnum ].height : y + h;

	//�e�N�X�`�����T�[�t�F�C�X��
	DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
	DX_RELEASE( dxtex[ txnum ].sur[ 1 ] );
	dxtex[ txnum ].tex->GetSurfaceLevel( 0, &( dxtex[ txnum ].sur[ 0 ] ) );

	//�����_�[�e�N�X�`���̃t�H�[�}�b�g���擾
	hr = dxtex[ txnum ].tex->GetLevelDesc( 0, &desc );
	//�����_�[�e�N�X�`���Ɠ����t�H�[�}�b�g�̃T�[�t�F�C�X���V�X�e���������Ɋm��
	hr = D3DDev->CreateOffscreenPlainSurface(
		desc.Width, desc.Height, desc.Format,
		D3DPOOL_SYSTEMMEM, &( dxtex[ txnum ].sur[ 1 ] ), NULL );

	hr = D3DDev->GetRenderTargetData( dxtex[ txnum ].sur[ 0 ], dxtex[ txnum ].sur[ 1 ] );

	if ( SUCCEEDED( hr ) && dxtex[ txnum ].sur[ 1 ] )
	{
		hr = dxtex[ txnum ].sur[ 1 ]->LockRect( &( dxtex[ txnum ].lockrect ), &( dxtex[ txnum ].cutrect ), D3DLOCK_DISCARD );
		//�e�N�X�`���̃��b�N
		if ( SUCCEEDED( hr ) )
		{
			dxtex[ txnum ].lock = 1;
			return 0;
		}
	}
	return 2;
}

//�e�N�X�`���̃f�[�^��ǂ߂��Ԃ���߂��B
int DirectXDraw9::CloseTexture( unsigned int txnum )
{
	//�e�N�X�`���`�F�b�N
	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}

	dxtex[ txnum ].sur[ 1 ]->UnlockRect();

	//�e�N�X�`���ɃT�[�t�F�C�X�������߂�
	if ( dxtex[ txnum ].writed )
	{
		hr = D3DDev->UpdateSurface( dxtex[ txnum ].sur[ 1 ], NULL, dxtex[ txnum ].sur[ 0 ], NULL );
		dxtex[ txnum ].writed = 0;
		BackUpTexture( txnum );
	}

	//�T�[�t�F�C�X�̉��(���邩�Ȃ��H)
	DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
	DX_RELEASE( dxtex[ txnum ].sur[ 1 ] );
	dxtex[ txnum ].lock = 0;
	return 0;
}

/*!
@brief �e�N�X�`���̎w����W�̐F���擾����B
@par   �֐�����
�e�N�X�`���̎w����W�̐F���擾����B
@param txnum �e�N�X�`���ԍ��B
@param x x���W�B
@param y y���W�B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
*/
unsigned long DirectXDraw9::GetColorTexturePixel( unsigned int txnum, int x, int y )
{
	//�摜�擾�̈�̍쐬
	//  RECT cutrect;
	unsigned long color = 0x00000000;
	//���b�N�����`�̈�
	//  D3DLOCKED_RECT lockrect;
	//�T�[�t�F�C�X�̃t�H�[�}�b�g
	//  D3DSURFACE_DESC desc;
	//�T�[�t�F�C�X
	//  LPDIRECT3DSURFACE9 surface[2] = {};

	//  cutrect.left   = x;
	//  cutrect.right  = x + 1;
	//  cutrect.top    = y;
	//  cutrect.bottom = y + 1;

	//�e�N�X�`���`�F�b�N
	//  if( CanUseTexture( txnum ) )//if( txnum < texmax && dxtex[ txnum ].ext )
	//  {
	//�e�N�X�`�����T�[�t�F�C�X��
	//    dxtex[ txnum ].tex->GetSurfaceLevel( 0, &(surface[0]) );

	//�����_�[�e�N�X�`���̃t�H�[�}�b�g���擾
	//    hr = dxtex[ txnum ].tex->GetLevelDesc( 0, &desc );
	//�����_�[�e�N�X�`���Ɠ����t�H�[�}�b�g�̃T�[�t�F�C�X���V�X�e���������Ɋm��
	//    hr = D3DDev->CreateOffscreenPlainSurface(
	//      desc.Width, desc.Height, desc.Format,
	//      D3DPOOL_SYSTEMMEM, &(surface[1]), NULL );

	//    hr = D3DDev->GetRenderTargetData( surface[0], surface[1] );

	//    if( SUCCEEDED( hr ) && surface[1] )
	//    {
	//�e�N�X�`���̃��b�N
	//      if( SUCCEEDED( surface[1]->LockRect( &lockrect, &cutrect, D3DLOCK_READONLY ) ) )//if( SUCCEEDED( dxtex[ txnum ].tex->LockRect( 0, &lockrect, &cutrect, D3DLOCK_READONLY ) ) )
	//      {
	//����̐F���擾
	//        color = *( (unsigned long *)lockrect.pBits );
	//�T�[�t�F�C�X�̃A�����b�N
	//        surface[1]->UnlockRect();//dxtex[ txnum ].tex->UnlockRect( 0 );
	//      }
	//    }
	//�T�[�t�F�C�X�̉��
	//    DX_RELEASE( surface[0] );
	//    DX_RELEASE( surface[1] );
	//  }

	int locked;

	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}

	locked = dxtex[ txnum ].lock;
	if ( locked == 0 )
	{
		// �e�N�X�`�����b�N���s�Ȃ��Ă��Ȃ��̂ŁA�������ƃ��b�N�B
		OpenTexture( txnum, x, y, 1, 1 );
		color = *( (unsigned long *)( dxtex[ txnum ].lockrect.pBits ) );
		CloseTexture( txnum );
		return color;
	}

	//memcpy((BYTE*)lockRect.pBits + lockRect.Pitch*y + 4*x ), &color, sizeof(DWORD));
	//  color = *((unsigned long*)dxtex[ txnum ].lockrect.pBits + dxtex[ txnum ].lockrect.Pitch * y + 4 * x );
	color = *( (unsigned long*)dxtex[ txnum ].lockrect.pBits + dxtex[ txnum ].lockrect.Pitch * y / 4 + x );

	return color;
}

/*!
@brief �e�N�X�`���̐F��ʂ̐F�ɒu������
@par   �֐�����
�e�N�X�`���͈͓̔��̐F��u������B
@param txnum �e�N�X�`���ԍ��B
@param scolor ��������F�B
@param rcolor �u������F�B
@param x �����J�nx���W�B�����l��0�B
@param y �����J�ny���W�B�����l��0�B
@param w �����B�����l��0�ŁA�e�N�X�`���̉����ɂȂ�B
@param h �����B�����l��0�ŁA�e�N�X�`���̍����ɂȂ�B
@return �u�������񐔂�Ԃ��B��x���u�����Ȃ����0�B
*/
unsigned int DirectXDraw9::TextureColorReplacement( unsigned int txnum, unsigned long scolor, unsigned long rcolor, int x, int y, int w, int h )
{
	int count = 0;
	int rx, ry, locked;
	//  unsigned int size;
	unsigned char *line;
	unsigned long *pixel;
	//�摜�擾�̈�̍쐬
	//  RECT cutrect;
	//���b�N�����`�̈�
	//  D3DLOCKED_RECT lockrect;
	//�T�[�t�F�C�X�̃t�H�[�}�b�g
	//  D3DSURFACE_DESC desc;
	//�T�[�t�F�C�X
	//  IDirect3DSurface9 *surface= NULL;

	//�e�N�X�`���`�F�b�N
	//  if( CanUseTexture( txnum ) )//if( txnum < texmax && dxtex[ txnum ].ext )
	//  {

	//    if( w <= 0 )
	//    {
	//      w = dxtex[ txnum ].width - x;
	//    }
	//    if( h <= 0 )
	//    {
	//      h = dxtex[ txnum ].height - y;
	//    }

	//�u���͈͂̐ݒ�
	//    cutrect.left   = x;
	//    cutrect.right  = ( (size = x + w) > dxtex[ txnum ].width ) ? dxtex[ txnum ].width : size;
	//    cutrect.top    = y;
	//    cutrect.bottom = ( (size = y + h) > dxtex[ txnum ].height ) ? dxtex[ txnum ].height : size;

	//�e�N�X�`�����T�[�t�F�C�X��
	//    DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
	//    dxtex[ txnum ].tex->GetSurfaceLevel( 0, &(dxtex[ txnum ].sur[ 0 ]) );

	//�����_�[�e�N�X�`���̃t�H�[�}�b�g���擾
	//    hr = dxtex[ txnum ].tex->GetLevelDesc( 0, &desc );
	//�����_�[�e�N�X�`���Ɠ����t�H�[�}�b�g�̃T�[�t�F�C�X���V�X�e���������Ɋm��
	//    hr = D3DDev->CreateOffscreenPlainSurface(
	//      desc.Width, desc.Height, desc.Format,
	//      D3DPOOL_SYSTEMMEM, &(surface), NULL );

	//    hr = D3DDev->GetRenderTargetData( dxtex[ txnum ].sur[ 0 ], surface );

	//    if( SUCCEEDED( hr ) && surface )
	//    {
	//�e�N�X�`���̃��b�N
	//      if( SUCCEEDED( surface->LockRect( &lockrect, &cutrect, D3DLOCK_DISCARD ) ) )//D3DLOCK_READONLY ) ) )//if( SUCCEEDED( dxtex[ txnum ].tex->LockRect( 0, &lockrect, &cutrect, D3DLOCK_READONLY ) ) )
	//      {
	//�F�̌���
	//        line = (unsigned char *)lockrect.pBits;
	//        for( ry = 0; ry < h ; ++ry )
	//        {
	//          pixel = (unsigned long*)line;
	//          for( rx = 0; rx < w ; rx++ )
	//          {
	//�F�̔���
	//            if( *pixel == scolor )
	//            {
	//�F�̏�������
	//              *pixel = rcolor;
	//              ++count;
	//              dxtex[ txnum ].writed = 1;
	//            }
	//���̃h�b�g��
	//            ++pixel;
	//          }
	//          line += lockrect.Pitch;
	//        }
	//�T�[�t�F�C�X�̃A�����b�N
	//        surface->UnlockRect();
	//      }
	//    }
	//�e�N�X�`���ɃT�[�t�F�C�X�������߂�
	//    hr = D3DDev->UpdateSurface( surface, NULL, dxtex[ txnum ].sur[ 0 ], NULL);

	//    BackUpTexture( txnum );

	//�T�[�t�F�C�X�̉��
	//    DX_RELEASE( surface );
	//  }

	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}

	locked = dxtex[ txnum ].lock;
	if ( locked == 0 )
	{
		// �e�N�X�`�������b�N���ĂȂ��̂Ń��b�N�J�n�B
		OpenTexture( txnum, x, y, w, h );
	}

	// �F�̌���
	line = (unsigned char *)dxtex[ txnum ].lockrect.pBits;
	for ( ry = 0; ry < h; ++ry )
	{
		pixel = (unsigned long*)line;
		for ( rx = 0; rx < w; rx++ )
		{
			//�F�̔���
			if ( *pixel == scolor )
			{
				//�F�̏�������
				*pixel = rcolor;
				++count;
				dxtex[ txnum ].writed = 1;
			}
			//���̃h�b�g��
			++pixel;
		}
		line += dxtex[ txnum ].lockrect.Pitch;
	}

	if ( locked == 0 )
	{
		// �e�N�X�`�������b�N���ĂȂ������̂Ō��ɖ߂��ׂ��A�����b�N�B
		CloseTexture( txnum );
	}

	return count;
}

/*!
@brief �e�N�X�`���̐F����������
@par   �֐�����
�e�N�X�`���͈͓̔��̐F����������B
@param txnum �e�N�X�`���ԍ��B
@param scolor ��������F�B
@param x �����J�nx���W�B�����l��0�B
@param y �����J�ny���W�B�����l��0�B
@param w �����B�����l��0�ŁA�e�N�X�`���̉����ɂȂ�B
@param h �����B�����l��0�ŁA�e�N�X�`���̍����ɂȂ�B
@return 0 ������Ȃ������B
@return ���߂ăq�b�g����܂ł̌����ς݃s�N�Z�����B���ォ��E���Ɍ��������ԁB
*/
unsigned int DirectXDraw9::TextureColorSearch( unsigned int txnum, unsigned long scolor, int x, int y, int w, int h )
{
	int count = 0;
	int rx, ry, locked;
	//  unsigned int size;
	unsigned char *line;
	unsigned long *pixel;
	//�摜�擾�̈�̍쐬
	//  RECT cutrect;
	//���b�N�����`�̈�
	//  D3DLOCKED_RECT lockrect;
	//�T�[�t�F�C�X�̃t�H�[�}�b�g
	//  D3DSURFACE_DESC desc;
	//�T�[�t�F�C�X
	//  IDirect3DSurface9 * surface= NULL;

	//�e�N�X�`���`�F�b�N
	//  if( CanUseTexture( txnum ) )//if( txnum < texmax && dxtex[ txnum ].ext )
	//  {

	if ( w <= 0 )
	{
		w = dxtex[ txnum ].width - x;
	}
	if ( h <= 0 )
	{
		h = dxtex[ txnum ].height - y;
	}

	//�����͈�
	//    cutrect.left   = x;
	//    cutrect.right  = ( (size = x + w) > dxtex[ txnum ].width ) ? dxtex[ txnum ].width : size;
	//    cutrect.top    = y;
	//    cutrect.bottom = ( (size = y + h) > dxtex[ txnum ].height ) ? dxtex[ txnum ].height : size;

	//�e�N�X�`�����T�[�t�F�C�X��
	//    DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
	//    dxtex[ txnum ].tex->GetSurfaceLevel( 0, &(dxtex[ txnum ].sur[ 0 ]) );

	//�����_�[�e�N�X�`���̃t�H�[�}�b�g���擾
	//    hr = dxtex[ txnum ].tex->GetLevelDesc( 0, &desc );
	//�����_�[�e�N�X�`���Ɠ����t�H�[�}�b�g�̃T�[�t�F�C�X���V�X�e���������Ɋm��
	//    hr = D3DDev->CreateOffscreenPlainSurface(
	//      desc.Width, desc.Height, desc.Format,
	//      D3DPOOL_SYSTEMMEM, &(surface), NULL );

	//    hr = D3DDev->GetRenderTargetData( dxtex[ txnum ].sur[ 0 ], surface );

	//    if( SUCCEEDED( hr ) && surface )
	//    {
	//�e�N�X�`���̃��b�N
	//      if( SUCCEEDED( surface->LockRect( &lockrect, &cutrect, D3DLOCK_READONLY ) ) )
	//      {
	//�F�̌���
	//        line = (unsigned char *)lockrect.pBits;
	//        for( ry = 0; ry < h ; ++ry )
	//        {
	//          pixel = (unsigned long*)line;
	//          for( rx = 0; rx < w ; rx++ )
	//          {
	//            ++count;
	//�F�̔���
	//            if( *pixel == scolor )
	//            {
	//�T�[�t�F�C�X�̃A�����b�N
	//              surface->UnlockRect();
	//�T�[�t�F�C�X�̉��
	//              DX_RELEASE( surface );
	//              return count;
	//            }
	//���̃h�b�g��
	//            ++pixel;
	//          }
	//          line += lockrect.Pitch;
	//        }
	//�T�[�t�F�C�X�̃A�����b�N
	//        surface->UnlockRect();
	//      }
	//    }
	//�T�[�t�F�C�X�̉��
	//    DX_RELEASE( surface );
	//  }

	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}

	locked = dxtex[ txnum ].lock;
	if ( locked == 0 )
	{
		// �e�N�X�`�������b�N���ĂȂ��̂Ń��b�N�J�n�B
		OpenTexture( txnum, x, y, w, h );
	}

	// �F�̌���
	line = (unsigned char *)dxtex[ txnum ].lockrect.pBits;
	for ( ry = 0; ry < h; ++ry )
	{
		pixel = (unsigned long*)line;
		for ( rx = 0; rx < w; rx++ )
		{
			++count;
			//�F�̔���
			if ( *pixel == scolor )
			{
				ry = h;
				goto out;
			}
			//���̃h�b�g��
			++pixel;
		}
		line += dxtex[ txnum ].lockrect.Pitch;
	}

	count = 0;

out:
	if ( locked == 0 )
	{
		// �e�N�X�`�������b�N���ĂȂ������̂Ō��ɖ߂��ׂ��A�����b�N�B
		CloseTexture( txnum );
	}

	return count;
}

/*!
@brief �e�N�X�`���̐F�����������𐔂���
@par   �֐�����
�e�N�X�`���͈͓̔��̐F����������B
@param txnum �e�N�X�`���ԍ��B
@param scolor ��������F�B
@param x �����J�nx���W�B�����l��0�B
@param y �����J�ny���W�B�����l��0�B
@param w �����B�����l��0�ŁA�e�N�X�`���̉����ɂȂ�B
@param h �����B�����l��0�ŁA�e�N�X�`���̍����ɂȂ�B
@return 0 ������Ȃ������B
@return �͈͓��Ō������������F�̃s�N�Z�����B
*/
unsigned int DirectXDraw9::TextureColorCount( unsigned int txnum, unsigned long scolor, int x, int y, int w, int h )
{
	int count = 0;
	int rx, ry, locked;
	//  unsigned int size;
	unsigned char *line;
	unsigned long *pixel;
	//�摜�擾�̈�̍쐬
	//  RECT cutrect;
	//���b�N�����`�̈�
	//  D3DLOCKED_RECT lockrect;
	//�T�[�t�F�C�X�̃t�H�[�}�b�g
	//  D3DSURFACE_DESC desc;
	//�T�[�t�F�C�X
	//  IDirect3DSurface9 * surface= NULL;

	//�e�N�X�`���`�F�b�N
	//  if( CanUseTexture( txnum ) )//if( txnum < texmax && dxtex[ txnum ].ext )
	//  {

	//    if( w <= 0 )
	//    {
	//      w = dxtex[ txnum ].width - x;
	//    }
	//    if( h <= 0 )
	//    {
	//      h = dxtex[ txnum ].height - y;
	//    }

	//�����͈�
	//    cutrect.left   = x;
	//    cutrect.right  = ( (size = x + w) > dxtex[ txnum ].width ) ? dxtex[ txnum ].width : size;
	//    cutrect.top    = y;
	//    cutrect.bottom = ( (size = y + h) > dxtex[ txnum ].height ) ? dxtex[ txnum ].height : size;

	//�e�N�X�`�����T�[�t�F�C�X��
	//    DX_RELEASE( dxtex[ txnum ].sur[ 0 ] );
	//    dxtex[ txnum ].tex->GetSurfaceLevel( 0, &(dxtex[ txnum ].sur[ 0 ]) );

	//�����_�[�e�N�X�`���̃t�H�[�}�b�g���擾
	//    hr = dxtex[ txnum ].tex->GetLevelDesc( 0, &desc );
	//�����_�[�e�N�X�`���Ɠ����t�H�[�}�b�g�̃T�[�t�F�C�X���V�X�e���������Ɋm��
	//    hr = D3DDev->CreateOffscreenPlainSurface(
	//      desc.Width, desc.Height, desc.Format,
	//      D3DPOOL_SYSTEMMEM, &(surface), NULL );

	//    hr = D3DDev->GetRenderTargetData( dxtex[ txnum ].sur[ 0 ], surface );

	//    if( SUCCEEDED( hr ) && surface )
	//    {
	//�e�N�X�`���̃��b�N
	//      if( SUCCEEDED( surface->LockRect( &lockrect, &cutrect, D3DLOCK_READONLY ) ) )
	//      {
	//�F�̌���
	//        line = (unsigned char *)lockrect.pBits;
	//        for( ry = 0; ry < h ; ++ry )
	//        {
	//          pixel = (unsigned long*)line;
	//          for( rx = 0; rx < w ; rx++ )
	//          {
	//�F�̔���
	//            if( *pixel == scolor )
	//            {
	//�T�[�t�F�C�X�̃A�����b�N
	//              surface->UnlockRect();
	//�T�[�t�F�C�X�̉��
	//              DX_RELEASE( surface );
	//              ++count;
	//            }
	//���̃h�b�g��
	//            ++pixel;
	//          }
	//          line += lockrect.Pitch;
	//        }
	//�T�[�t�F�C�X�̃A�����b�N
	//        surface->UnlockRect();
	//      }
	//    }
	//�T�[�t�F�C�X�̉��
	//    DX_RELEASE( surface );
	//  }

	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}

	locked = dxtex[ txnum ].lock;
	if ( locked == 0 )
	{
		// �e�N�X�`�������b�N���ĂȂ��̂Ń��b�N�J�n�B
		OpenTexture( txnum, x, y, w, h );
	}

	// �F�̌���
	line = (unsigned char *)dxtex[ txnum ].lockrect.pBits;
	for ( ry = 0; ry < h; ++ry )
	{
		pixel = (unsigned long*)line;
		for ( rx = 0; rx < w; rx++ )
		{
			//�F�̔���
			if ( *pixel == scolor )
			{
				++count;
			}
			//���̃h�b�g��
			++pixel;
		}
		line += dxtex[ txnum ].lockrect.Pitch;
	}

	if ( locked == 0 )
	{
		// �e�N�X�`�������b�N���ĂȂ������̂Ō��ɖ߂��ׂ��A�����b�N�B
		CloseTexture( txnum );
	}

	return count;
}

/*!
@brief �e�N�X�`���̉����擾
@par   �֐�����
�e�N�X�`���̉������擾�B
@param txnum �e�N�X�`���ԍ��B
@return 0=���s�B
@return ���̑�=�e�N�X�`���̉����B
*/
unsigned int DirectXDraw9::GetTextureWidth( unsigned int txnum )
{
	if ( CanUseTexture( txnum ) )//if( txnum < texmax && dxtex[ txnum ].ext )
	{
		return dxtex[ txnum ].width;
	}
	return 0;
}

/*!
@brief �e�N�X�`���̍����擾
@par   �֐�����
�e�N�X�`���̍������擾�B
@param txnum �e�N�X�`���ԍ��B
@return 0=���s�B
@return ���̑�=�e�N�X�`���̍����B
*/
unsigned int DirectXDraw9::GetTextureHeight( unsigned int txnum )
{
	if ( CanUseTexture( txnum ) )//if( txnum < texmax && dxtex[txnum].ext )
	{
		return dxtex[ txnum ].height;
	}
	return 0;
}

/*!
@brief �X�N���[���V���b�g���B��
@par   �֐�����
���݂̃X�N���[���V���b�g
@param filename �t�@�C�����B
@param imagetype �摜�̃^�C�v�B
@return ���0��Ԃ��B
*/
int DirectXDraw9::ScreenShot( const char *filename, unsigned int imagetype )
{
	if ( imagetype >= IMAGE_MAX )
	{
		imagetype = IMAGE_BMP;
	}
	D3DXSaveSurfaceToFile( filename, (D3DXIMAGE_FILEFORMAT)imagetype, backbuffer, NULL, NULL );
	return 0;
}

// �e�N�X�`�����摜�֏o��
int DirectXDraw9::TextureToImage( const char *filename, unsigned int txnum, unsigned int imagetype )
{
	int locked;
	RECT cutrect;

	if ( CannotUseTexture( txnum ) )
	{
		return 1;
	}

	if ( imagetype >= IMAGE_MAX )
	{
		imagetype = IMAGE_BMP;
	}

	locked = dxtex[ txnum ].lock;
	if ( locked == 0 )
	{
		// �e�N�X�`�������b�N���ĂȂ��̂Ń��b�N�J�n�B
		OpenTexture( txnum );

		D3DXSaveSurfaceToFile( filename, (D3DXIMAGE_FILEFORMAT)imagetype, dxtex[ txnum ].sur[ 0 ], NULL, NULL );

		// �e�N�X�`�������b�N���ĂȂ������̂Ō��ɖ߂��ׂ��A�����b�N�B
		CloseTexture( txnum );
	} else
	{
		cutrect = dxtex[ txnum ].cutrect;
		D3DXSaveSurfaceToFile( filename, (D3DXIMAGE_FILEFORMAT)imagetype, dxtex[ txnum ].sur[ 0 ], NULL, &cutrect );
	}

	return 0;
}



/*!
@brief �l�𒲐�����B
@par   �֐�����
�l��2��n��ɒ�������B
@param size ��������l�B
@return ������̒l�B
*/
int DirectXDraw9::ControlSize( int size )
{
	//����log2���Ƃ���pow�������������C������B
	if ( size <= 2 )
	{
		return 2;
	} else if ( size <= 4 )
	{
		return 4;
	} else if ( size <= 8 )
	{
		return 8;
	} else if ( size <= 16 )
	{
		return 16;
	} else if ( size <= 32 )
	{
		return 32;
	} else if ( size <= 64 )
	{
		return 64;
	} else if ( size <= 128 )
	{
		return 128;
	} else if ( size <= 256 )
	{
		return 256;
	} else if ( size <= 512 )
	{
		return 512;
	} else if ( size <= 1024 )
	{
		return 1024;
	} else if ( size <= 2048 )
	{
		return 2048;
	} else if ( size <= 4096 )
	{
		return 4096;
	} else if ( size <= 8192 )
	{
		return 8192;
	}
	return size;
}

IDirect3DDevice9 * DirectXDraw9::GetDirect3DDevice( int force, unsigned long sleeptime )
{
	if ( force == 0 )
	{
		while ( drawing == 0 )
		{
			Sleep( sleeptime );
		}
	}
	return D3DDev;
}
HRESULT DirectXDraw9::SetRenderState( D3DRENDERSTATETYPE State, DWORD Value )
{
	return D3DDev->SetRenderState( State, Value );
}
HRESULT DirectXDraw9::SetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value )
{
	return D3DDev->SetTextureStageState( Stage, Type, Value );
}
