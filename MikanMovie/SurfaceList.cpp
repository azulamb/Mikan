
#include "MikanVMRSurfaceAllocator.h"

SurfaceList::SurfaceList( void )
{
	list = NULL;
	size = 0;
}

SurfaceList::~SurfaceList( void )
{
	if ( list )
	{
		ReleaseAll();
	}
}

unsigned long SurfaceList::Size( void )
{
	return size;
}

int SurfaceList::Resize( unsigned long newsize )
{
	if ( size < newsize )
	{
		list = (IDirect3DSurface9 **)realloc( list, newsize * sizeof( IDirect3DSurface9 * ) );
		for ( ; size < newsize ; ++size )
		{
			list[ size ] = NULL;
		}
		return 1;
	} else if ( newsize < size )
	{
		for ( --size ; newsize <= size ; --size )
		{
			Release( size );
		}
		list = (IDirect3DSurface9 **)realloc( list, newsize * sizeof( IDirect3DSurface9 * ) );
		return -1;
	}

	return 0;
}

IDirect3DSurface9 * SurfaceList::Get( unsigned long index )
{
	if ( size <= index )
	{
		return NULL;
	}
	return list[ index ];
}

IDirect3DSurface9 ** SurfaceList::GetPointer( unsigned long index )
{
	if ( size <= index )
	{
		return NULL;
	}
	return &( list[ index ] );
}

int SurfaceList::Release( unsigned long index )
{
	if ( size <= index )
	{
		return -1;
	}

	if ( list[ index ] )
	{
		list[ index ]->Release();
	}
	list[ index ] = NULL;

	return 0;
}

unsigned long SurfaceList::ReleaseAll( void )
{
	unsigned long i;

	if ( list == NULL )
	{
		return 0;
	}

	for ( i = 0 ; i < size ; ++i )
	{
		list[ i ]->Release();
	}

	free( list );
	list = NULL;

	return i;
}
