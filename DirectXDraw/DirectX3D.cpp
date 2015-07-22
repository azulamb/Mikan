#include "DirectX3D.h"

DirectX3D::DirectX3D( CLASSDEF_DIRECTX_DRAW *directxdraw )
{
	// Ç®óFíBñ≥ÇµÇ≈ÇÕâΩÇ‡èoóàÇ»Ç¢ÇÃÇ≈ÅAÇ®óFíBïKê{ÅB
	this->directxdraw = directxdraw;

	// ê¢äEÇç∂éËånÇ…Ç∑ÇÈÅB
	SetWorldLeftHanded();

	texmax = NULL;
	dxtex = NULL;
	pvmax = 0;
	pv = NULL;
}

DirectX3D::~DirectX3D( void )
{
}

int DirectX3D::Init( unsigned int pmax )
{
	pvmax = pmax;
	texmax = directxdraw->texmax;
	dxtex = directxdraw->dxtex;
	pvmax = directxdraw->pvmax;
	pv = ( struct POLYGON_VERTEX_3D * )calloc( pmax, sizeof( struct POLYGON_VERTEX_3D ) );
	SetWorld();
	return 0;
}

int DirectX3D::Release( void )
{
	ReleasePolygonAll();
	free( pv );
	return 0;
}

int DirectX3D::CreatePolygon( unsigned int pnum, unsigned int max, int filltype )
{
	if ( pnum > pvmax )
	{
		// î‘çÜÇ™ïsê≥ÅB
		return 1;
	}

	if ( pv[ pnum ].ext )
	{
		free( pv[ pnum ].vertex );
	}

	pv[ pnum ].ext = 1;

	pv[ pnum ].max = max;

	pv[ pnum ].txnum = texmax;

	pv[ pnum ].vertex = ( struct CUSTOMVERTEX_3D * )calloc( max, sizeof( struct CUSTOMVERTEX_3D ) );

	pv[ pnum ].filltype = (D3DPRIMITIVETYPE)filltype;

	return 0;
}

int DirectX3D::ReleasePolygon( unsigned int pnum )
{
	if ( CannotUsePorygon( pnum ) )
	{
		return 1;
	}

	free( pv[ pnum ].vertex );
	pv[ pnum ].ext = 0;

	return 0;
}

int DirectX3D::ReleasePolygonAll( void )
{
	unsigned int i;

	for ( i = 0; i < pvmax; ++i )
	{
		ReleasePolygon( i );
	}

	return 0;
}

int DirectX3D::DrawPolygon( unsigned int pnum, double x, double y, double z )
{
	unsigned int i, n;
	struct CUSTOMVERTEX_LIST *next;
	struct CUSTOMVERTEX *cv;
	IDirect3DDevice9 *D3DDev = directxdraw->D3DDev;

	if ( CannotUsePorygon( pnum ) )//pnum > directxdraw->pvmax )
	{
		return 1;
	}

	n = pv[ pnum ].read;
	switch ( pv[ pnum ].filltype )
	{
	case D3DPT_TRIANGLELIST:
		n = n / 3;
		break;
	case 	D3DPT_TRIANGLESTRIP:
		n = n - 2;
		break;
	case D3DPT_TRIANGLEFAN:
		n = n - 2;
		break;
	};

	D3DDev->SetTexture( 0, pv[ pnum ].txnum >= texmax ? NULL : dxtex[ pv[ pnum ].txnum ].tex );
	D3DDev->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
	if ( FAILED( D3DDev->DrawPrimitiveUP( pv[ pnum ].filltype, n, pv[ pnum ].vertex, sizeof( CUSTOMVERTEX ) ) ) ) { return 2; }


	return 0;
}



int DirectX3D::SetWorldLeftHanded( void )
{
	leftworld = 1;
	return 0;
}

int DirectX3D::SetWorldRightHanded( void )
{
	leftworld = 0;
	return 0;
}

int DirectX3D::SetWorld( const D3DXMATRIX *world )
{
	if ( world == NULL )
	{
		// íPà çsóÒÅB
		D3DXMatrixIdentity( &( this->world ) );
	} else
	{
		memcpy( &( this->world ), world, sizeof( D3DXMATRIX ) );
	}

	// ê¢äEÇê›íËÅB
	hret = directxdraw->GetDirect3DDevice()->SetTransform( D3DTS_WORLD, &( this->world ) );

	if ( hret != D3D_OK )
	{
		return 1;
	}

	return 0;
}

int DirectX3D::SetView( void )
{
	D3DXMatrixLookAt( &view, &eye, &at, &up );
	directxdraw->GetDirect3DDevice()->SetTransform( D3DTS_VIEW, &view );
	return 0;
}

int DirectX3D::SetLookAtTarget( double x, double y, double z )
{
	at.x = (float)x;
	at.y = (float)y;
	at.z = (float)z;
	return 0;
}

int DirectX3D::SetLookAtTarget( const D3DXVECTOR3 *at )
{
	this->at.x = (float)at->x;
	this->at.y = (float)at->y;
	this->at.z = (float)at->z;
	return 0;
}

int DirectX3D::SetEye( double x, double y, double z )
{
	SetEyeX( x );
	SetEyeY( y );
	SetEyeZ( z );
	return 0;
}

int DirectX3D::SetEye( const D3DXVECTOR3 *eye )
{
	SetEyeX( eye->x );
	SetEyeY( eye->y );
	SetEyeZ( eye->z );
	return 0;
}

int DirectX3D::SetEyeX( double x )
{
	eye.x = (float)x;
	return 0;
}

int DirectX3D::SetEyeY( double y )
{
	eye.y = (float)y;
	return 0;
}

int DirectX3D::SetEyeZ( double z )
{
	eye.z = (float)z;
	return 0;
}

int DirectX3D::MoveEye( double x, double y, double z )
{
	MoveEyeX( x );
	MoveEyeY( y );
	MoveEyeZ( z );
	return 0;
}

int DirectX3D::MoveEyeX( double x )
{
	eye.x += (float)x;
	return 0;
}

int DirectX3D::MoveEyeY( double y )
{
	eye.y += (float)y;
	return 0;
}

int DirectX3D::MoveEyeZ( double z )
{
	eye.z += (float)z;
	return 0;
}

int DirectX3D::SetCameraUp( double x, double y, double z )
{
	SetCameraUpX( x );
	SetCameraUpY( y );
	SetCameraUpZ( z );
	return 0;
}

int DirectX3D::SetCameraUp( const D3DXVECTOR3 *up )
{
	SetCameraUpX( up->x );
	SetCameraUpY( up->y );
	SetCameraUpZ( up->z );
	return 0;
}

int DirectX3D::SetCameraUpX( double x )
{
	up.x = (float)x;
	return 0;
}

int DirectX3D::SetCameraUpY( double y )
{
	up.y = (float)y;
	return 0;
}

int DirectX3D::SetCameraUpZ( double z )
{
	up.z = (float)z;
	return 0;
}

int DirectX3D::SetPerspectiveFieldOfView( void )
{
	directxdraw->GetDirect3DDevice()->SetTransform( D3DTS_PROJECTION, &proj );
	return 0;
}

int DirectX3D::SetPerspectiveFieldOfView( double fovy, double aspect, double front, double back )
{
	SetPerspectiveFieldOfViewY( fovy );
	SetPerspectiveAspect( aspect );
	SetPerspectiveZFront( front );
	SetPerspectiveZBack( back );

	D3DXMatrixPerspectiveFov( &proj, (float)fovy, (float)aspect, (float)front, (float)back );

	// ê›íËÇîΩâfÅB
	SetPerspectiveFieldOfView();

	return 0;
}

int DirectX3D::SetPerspectiveFieldOfView( const D3DXMATRIX *proj )
{
	memcpy( &( this->proj ), proj, sizeof( D3DXMATRIX ) );

	// ê›íËÇîΩâfÅB
	SetPerspectiveFieldOfView();

	return 0;
}

int DirectX3D::SetPerspectiveFieldOfViewY( double fovy )
{
	this->fovy = fovy;
	return 0;
}

int DirectX3D::SetPerspectiveAspect( double aspect )
{
	this->aspect = aspect;
	return 0;
}

int DirectX3D::SetPerspectiveZFront( double front )
{
	this->front = front;
	return 0;
}

int DirectX3D::SetPerspectiveZBack( double back )
{
	this->back = back;
	return 0;
}

D3DXMATRIX * DirectX3D::D3DXMatrixLookAt( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp )
{
	if ( leftworld )
	{
		return D3DXMatrixLookAtLH( pOut, pEye, pAt, pUp );
	}
	return D3DXMatrixLookAtRH( pOut, pEye, pAt, pUp );
}

D3DXMATRIX * DirectX3D::D3DXMatrixOrtho( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf )
{
	if ( leftworld )
	{
		return D3DXMatrixOrthoLH( pOut, w, h, zn, zf );
	}
	return D3DXMatrixOrthoRH( pOut, w, h, zn, zf );
}

D3DXMATRIX * DirectX3D::D3DXMatrixOrthoOffCenter( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn, FLOAT zf )
{
	if ( leftworld )
	{
		return D3DXMatrixOrthoOffCenterLH( pOut, l, r, b, t, zn, zf );
	}
	return D3DXMatrixOrthoOffCenterRH( pOut, l, r, b, t, zn, zf );
}

D3DXMATRIX * DirectX3D::D3DXMatrixPerspectiveFov( D3DXMATRIX *pOut, FLOAT fovY, FLOAT Aspect, FLOAT zn, FLOAT zf )
{
	if ( leftworld )
	{
		return D3DXMatrixPerspectiveFovLH( pOut, fovY, Aspect, zn, zf );
	}
	return D3DXMatrixPerspectiveFovRH( pOut, fovY, Aspect, zn, zf );
}

D3DXMATRIX * DirectX3D::D3DXMatrixPerspective( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf )
{
	if ( leftworld )
	{
		return D3DXMatrixPerspectiveLH( pOut, w, h, zn, zf );
	}
	return D3DXMatrixPerspectiveRH( pOut, w, h, zn, zf );
}

D3DXMATRIX * DirectX3D::D3DXMatrixPerspectiveOffCenter( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn, FLOAT zf )
{
	if ( leftworld )
	{
		return D3DXMatrixPerspectiveOffCenterLH( pOut, l, r, b, t, zn, zf );
	}
	return D3DXMatrixPerspectiveOffCenterRH( pOut, l, r, b, t, zn, zf );
}

