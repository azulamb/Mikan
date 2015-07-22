#include "MikanIO.h"

MikanIOManagementBase::MikanIOManagementBase( unsigned int max )
{
	iomax = max;
	mio = ( class MikanIO ** )calloc( iomax, sizeof( MikanIO ) );
}

MikanIOManagementBase::~MikanIOManagementBase( void )
{
	unsigned int i;
	CloseAll();

	for ( i = 0 ; i < iomax ; ++i )
	{
		if ( mio[ i ] )
		{
			Destroy( i );
		}
	}

	free( mio );

}

int MikanIOManagementBase::Open( unsigned int ionum, const char *io )
{
	if ( ionum >= iomax || mio[ ionum ] == NULL )
	{
		return -1;
	}

	return mio[ ionum ]->Open( io );
}

void * MikanIOManagementBase::Read( unsigned int ionum, unsigned int read_size, int *readed_size )
{
	if ( ionum >= iomax || mio[ ionum ] == NULL )
	{
		return NULL;
	}

	return mio[ ionum ]->Read( read_size, readed_size );
}

int MikanIOManagementBase::Read( unsigned int ionum, void *data, unsigned int read_size, int *readed_size )
{
	if ( ionum >= iomax || mio[ ionum ] == NULL )
	{
		return -1;
	}

	return mio[ ionum ]->Read( data, read_size, readed_size );
}

int MikanIOManagementBase::Seek( unsigned int ionum, int offset, int seekmode ) const
{
	if ( ionum >= iomax || mio[ ionum ] == NULL )
	{
		return -1;
	}

	return mio[ ionum ]->Seek( offset, seekmode );
}

int MikanIOManagementBase::Write( unsigned int ionum, const void *write_data, int write_size )
{
	if ( ionum >= iomax || mio[ ionum ] == NULL )
	{
		return -1;
	}

	return mio[ ionum ]->Write( write_data, write_size );
}

int MikanIOManagementBase::Tell( unsigned int ionum ) const
{
	if ( ionum >= iomax || mio[ ionum ] == NULL )
	{
		return -1;
	}

	return mio[ ionum ]->Tell();
}

int MikanIOManagementBase::Size( unsigned int ionum ) const
{
	if ( ionum >= iomax || mio[ ionum ] == NULL )
	{
		return -1;
	}

	return mio[ ionum ]->Size();
}

int MikanIOManagementBase::Close( unsigned int ionum )
{
	if ( ionum >= iomax || mio[ ionum ] == NULL )
	{
		return -1;
	}

	return mio[ ionum ]->Close();
}

int MikanIOManagementBase::Destroy( unsigned int ionum )
{
	if ( ionum >= iomax || mio[ ionum ] == NULL )
	{
		return -1;
	}

	delete( mio[ ionum ] );
	mio[ ionum ] = NULL;

	return 0;
}

int MikanIOManagementBase::CloseAll( void )
{
	unsigned int i, ct = 0;

	for ( i = 0 ; i < iomax ; ++i )
	{
		if ( Close( i ) == 0 )
		{
			++ct;
		}
	}

	return ct;
}
