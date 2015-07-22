#include "FileIO.h"

FileIO::FileIO( void )
{
	size = -1;
	SetBufferSize();
	file = NULL;
	filepath = NULL;
}

FileIO::~FileIO( void )
{
	Close();
}

int FileIO::FileExist( const char *filepath )
{
	struct stat st;

	if ( stat( filepath, &st ) != 0 )
	{
		return 0;
	}

	return 1;
}

int FileIO::Open( const char *io )
{
	return Open( io, "r" );
}

int FileIO::Open( const char *io, const char *mode )
{
	char *cmode;
	char modecode[] = { 0, 0, 0 }; // 読み, 書き, 追加 その他は適当に後付けていいよもう。
	unsigned int i;
	// 何かfopen_s使うとモード無効とかファイルがない時プログラムが落ちるんだけどなぜ？
	// しかしVS2012以降ではfopen使えないしなぁ。何なんだよぉ。

	// やけくそモードチェック。
	cmode = (char *)calloc( strlen( mode ) + 1, sizeof( char ) );
	if ( strncmp( mode, "ccs=", 4 ) )
	{
		for ( i = 0; i < strlen( mode ); ++i )
		{
			if ( mode[ i ] == 'r' )
			{
				if ( modecode[ 0 ] == 0 )
				{
					modecode[ 0 ] = 1;
				} else
				{
					modecode[ 0 ] = 2;
				}
			} else if ( mode[ i ] == 'w' )
			{
				if ( modecode[ 1 ] == 0 )
				{
					modecode[ 1 ] = 1;
				} else
				{
					modecode[ 1 ] = 2;
				}
			} else if ( mode[ i ] == 'a' )
			{
				modecode[ 2 ] = 1;
			} else if ( mode[ i ] == '+' )
			{
				if ( modecode[ 0 ] == 1 )
				{
					modecode[ 1 ] = 2;
				} else if ( modecode[ 1 ] == 1 )
				{
					modecode[ 0 ] = 2;
				} else if ( modecode[ 2 ] == 1 )
				{
					modecode[ 2 ] = 2;
				}
			}
		}

		if ( modecode[ 2 ] )
		{
			cmode[ 0 ] = 'a';
			cmode[ 1 ] = '+';
		} else
		{
			if ( modecode[ 0 ] == 1 )
			{
				cmode[ 0 ] = 'r';
			} else if ( modecode[ 1 ] == 1 )
			{
				cmode[ 0 ] = 'w';
			}
			if ( modecode[ 0 ] == 2 )
			{
				cmode[ 1 ] = '+';
			} else if ( modecode[ 1 ] == 2 )
			{
				cmode[ 1 ] = '+';
			}
		}
		for ( i = 0; i < strlen( mode ); ++i )
		{
			switch ( mode[ i ] )
			{
			case 'r':
			case 'w':
			case 'a':
			case '+':
				break;
			default:
				cmode[ strlen( cmode ) ] = mode[ i ];
			}
		}
	} else
	{
		strcpy_s( cmode, strlen( mode ) + 1, mode );
	}

	// ファイルへの読み書き権限しかないのにファイルを読み込もうとしてたらそこで終了する。
	if ( modecode[ 1 ] == 0 && modecode[ 2 ] == 0 && FileExist( io ) == 0 )
	{
		free( cmode );
		return -1;
	}

	try
	{
		if ( fopen_s( &file, io, cmode ) != 0 )
		{
			free( cmode );
			cmode = NULL;
			return -1;
		}
	} catch ( ... )
	{
		if ( cmode )
		{
			free( cmode ); cmode = NULL;
		}
		return -1;
	}
	free( cmode );

	filepath = (char *)calloc( strlen( io ) + 1, sizeof( char ) );
	strcpy_s( filepath, strlen( io ) + 1, io );

	size = 0;
	Seek( 0, SEEK_END );
	if ( ( size = Tell() ) < 0 )
	{
		size = 0;
	}
	Seek( 0, SEEK_SET );

	return size;
}

void * FileIO::Read( unsigned int read_size, int *readed_size )
{
	int rsize = 0;
	void *buf;

	if ( size < 0 )
	{
		return NULL;
	}

	if ( read_size == 0 )
	{
		read_size = bufsize;
	}

	buf = calloc( read_size, sizeof( char ) );

	rsize = fread( buf, sizeof( char ), read_size, file );

	if ( readed_size )
	{
		*readed_size = rsize;
	}

	return buf;
}

// 読み込む(領域は確保しない)。
int FileIO::Read( void *data, unsigned int read_size, int *readed_size )
{
	int rsize = 0;

	rsize = fread( data, sizeof( char ), read_size, file );

	if ( readed_size )
	{
		*readed_size = rsize;
	}

	return rsize;
}

char * FileIO::ReadLine( unsigned int read_size )
{
	int rsize = 0;
	int mode = 0;
	int readed = 0;
	char *buf;

	if ( size < 0 )
	{
		return NULL;
	}

	if ( read_size == 0 )
	{
		read_size = bufsize; mode = 1;
	}
	buf = (char *)calloc( read_size, sizeof( char ) );

	while ( fgets( buf + readed, read_size, file ) != NULL && mode )
	{
		readed = strlen( buf );
		if ( readed > 0 && buf[ readed - 1 ] == '\n' )
		{
			break;
		}
		buf = (char *)realloc( buf, readed + read_size );
	}

	return buf;
}

int FileIO::ReadLine( char *buf, unsigned int max_size, unsigned int *read_size )
{
	char *ret;
	if ( max_size < 0 || buf == NULL )
	{
		return 0;
	}

	ret = fgets( buf, max_size, file );

	if ( read_size )
	{
		*read_size = strlen( buf );
	}

	return ( ret != NULL );
}

int FileIO::Seek( int offset, int seekmode ) const
{
	if ( size < 0 )
	{
		return -1;
	}
	return fseek( file, offset, seekmode );
}

int FileIO::Write( const void *write_data, int write_size )
{
	if ( size < 0 || write_size <= 0 )
	{
		return -1;
	}

	fwrite( write_data, sizeof( char ), write_size, file );

	return -1;
}

int FileIO::Write( const char *write_data, int write_size )
{
	if ( size < 0 )
	{
		return -1;
	}

	if ( write_size <= 0 )
	{
		write_size = strlen( write_data );
	}

	fwrite( write_data, sizeof( char ), write_size, file );

	return -1;
}

int FileIO::Tell( void ) const
{
	if ( size < 0 )
	{
		return -1;
	}
	return ftell( file );
}

int FileIO::Size( void ) const
{
	return size;
}

int FileIO::Close( void )
{
	if ( size < 0 )
	{
		return -1;
	}

	fclose( file );

	free( filepath );
	filepath = NULL;

	size = -1;

	return 0;
}

char * FileIO::GetFilePath( void )
{
	return filepath;
}

int FileIO::SetBufferSize( int bufsize )
{
	if ( bufsize < 0 )
	{
		return -1;
	}
	this->bufsize = bufsize;
	return 0;
}

FileIOManagement::FileIOManagement( unsigned int max )
{
	iomax = max;
	//mio = (class MikanIOBase **)calloc( iomax, sizeof(class FileIO *) );
}

int FileIOManagement::InitIO( unsigned int max )
{
	unsigned int i;

	if ( max < iomax )
	{
		for ( i = max; i < iomax; ++max )
		{
			if ( mio[ i ] )
			{
				( ( class FileIO * )mio[ i ] )->Close();
				delete( mio[ i ] );
			}
		}
	}

	mio = ( class MikanIO ** )realloc( mio, max * sizeof( class FileIO * ) );

	for ( i = iomax; i < max; ++i )
	{
		mio[ i ] = NULL;
	}

	iomax = max;

	return 0;
}

// 開く。-1で失敗。
int FileIOManagement::Open( unsigned int ionum, const char *io, const char *mode )
{
	if ( ionum >= iomax )
	{
		return -1;
	}

	if ( mio[ ionum ] )
	{
		mio[ ionum ]->Close();
	} else
	{
		mio[ ionum ] = new FileIO();
	}

	if ( mio[ ionum ] == NULL )
	{
		return -1;
	}

	return ( ( class FileIO * )mio[ ionum ] )->Open( io, mode );
}

char * FileIOManagement::ReadLine( unsigned int ionum, unsigned int read_size )
{
	if ( ionum >= iomax || mio[ ionum ] == NULL )
	{
		return NULL;
	}

	return ( ( class FileIO * )mio[ ionum ] )->ReadLine( read_size );
}

int FileIOManagement::ReadLine( unsigned int ionum, char *buf, unsigned int max_size, unsigned int *read_size )
{
	if ( ionum >= iomax || mio[ ionum ] == NULL )
	{
		return NULL;
	}

	return ( ( class FileIO * )mio[ ionum ] )->ReadLine( buf, max_size, read_size );
}

int FileIOManagement::Write( unsigned int ionum, const char *write_data, int write_size )
{
	if ( ionum >= iomax || mio[ ionum ] == NULL )
	{
		return -1;
	}

	return ( ( class FileIO * )mio[ ionum ] )->Write( write_data, write_size );
}

int FileIOManagement::Printf( unsigned int ionum, int bufsize, const char *format, ... )
{
	int ret;
	char *str;
	//va_listを使う方法もあるけど、別にこれで問題ないよね。
	str = (char *)calloc( bufsize, sizeof( char ) );
	vsnprintf_s( str, bufsize, bufsize, format, (char*)( &format + 1 ) );

	ret = Write( ionum, str );

	free( str );

	return ret;
}

int FileIOManagement::Remove( unsigned int ionum )
{
	if ( ionum >= iomax )
	{
		return -1;
	}

	mio[ ionum ]->Close();

	return Remove( ( ( class FileIO * )mio[ ionum ] )->GetFilePath() );
}

int FileIOManagement::Remove( const char *file )
{
	return remove( file );
}

int FileIOManagement::Move( const char *oldfile, const char *newfile )
{
	Remove( newfile );
	return rename( oldfile, newfile );
}

int FileIOManagement::Move( unsigned int ionum, const char *newfile )
{
	Move( ( ( class FileIO * )mio[ ionum ] )->GetFilePath(), newfile );

	return 0;
}

int FileIOManagement::Move( unsigned int oldio, unsigned int newio )
{
	Move( ( ( class FileIO * )mio[ oldio ] )->GetFilePath(), ( ( class FileIO * )mio[ newio ] )->GetFilePath() );

	return 0;
}

class FileIO * FileIOManagement::GetIO( unsigned int ionum )
{
	if ( ionum >= iomax )
	{
		return NULL;
	}

	return ( class FileIO * )mio[ ionum ];
}

class MikanIO * FileIOManagement::GetMikanIO( unsigned int ionum )
{
	return ( class MikanIO * )GetIO( ionum );
}