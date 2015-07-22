#ifndef _UNLINK_LIBRARY
#  ifdef _MIKANDEV
#    ifdef _DEBUG
#      pragma comment(lib,"Dev/MikanIOD.lib")
#    else
#      pragma comment(lib,"Dev/MikanIO.lib")
#    endif
#  else
#    ifdef _DEBUG
#      pragma comment(lib,"MikanIOD.lib")
#    else
#      pragma comment(lib,"MikanIO.lib")
#    endif
#  endif
#endif

#ifndef CLASSDEF_MIKANIO
#define CLASSDEF_MIKANIO class MikanIO

#include <stdio.h>
#include <stdlib.h>

#define MIKANIO_MAX 16
#define MIKANIO_DEFAULT_BUFFER 1024

class MikanIO
{
public:
	// 開く。-1で失敗。
	virtual int Open( const char *io ) = 0;
	// 読み込む。NULLで失敗。
	virtual void * Read( unsigned int read_size = 0, int *readed_size = NULL ) = 0;
	// 読み込む。領域は確保しない。
	virtual int Read( void *data, unsigned int read_size, int *readed_size = NULL ) = 0;
	// シークする。-1で失敗。
	virtual int Seek( int offset, int seekmode = SEEK_CUR ) const = 0;
	// 書き込む。-1で失敗。書き込んだサイズを返す。
	virtual int Write( const void *write_data, int write_size = -1 ) = 0;
	// 今まで読んだバイト数を返す。-1で失敗。
	virtual int Tell( void ) const = 0;
	// サイズを返す。-1で失敗。もしくはTell。
	virtual int Size( void ) const = 0;
	// 閉じる。-1で失敗。
	virtual int Close( void ) = 0;
};

class MikanIOManagementBase
{
protected:
	unsigned int iomax;
	class MikanIO **mio;
public:
	MikanIOManagementBase( unsigned int max = MIKANIO_MAX );
	virtual ~MikanIOManagementBase( void );
	// 開く。-1で失敗。
	virtual int Open( unsigned int ionum, const char *io );
	// 読み込む。NULLで失敗。
	virtual void * Read( unsigned int ionum, unsigned int read_size = 0, int *readed_size = NULL );
	// 読み込む。領域は確保しない。
	virtual int Read( unsigned int ionum, void *data, unsigned int read_size, int *readed_size = NULL );
	// シークする。-1で失敗。
	virtual int Seek( unsigned int ionum, int offset, int seekmode = SEEK_CUR ) const;
	// 書き込む。-1で失敗。書き込んだサイズを返す。
	virtual int Write( unsigned int ionum, const void *write_data, int write_size = -1 );
	// 今まで読んだバイト数を返す。-1で失敗。
	virtual int Tell( unsigned int ionum ) const;
	// サイズを返す。-1で失敗。もしくはTell。
	virtual int Size( unsigned int ionum ) const;
	// 閉じる。-1で失敗。
	virtual int Close( unsigned int ionum );

	// IOを破棄する。
	virtual int Destroy( unsigned int ionum );
	// すべて閉じる。閉じることに成功したIOの数を返す。
	virtual int CloseAll( void );
};

#endif
