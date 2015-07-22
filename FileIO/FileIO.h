#ifdef USEMIKANLIBRARY
#ifdef _MIKANDEV
#include "Dev/MikanIO.h"
#else
#include "MikanIO.h"
#endif
#else
#include "../MikanIO/MikanIO.h"
#endif

#ifndef _UNLINK_LIBRARY
#  ifdef _MIKANDEV
#    ifdef _DEBUG
#      pragma comment(lib,"Dev/FileIOD.lib")
#    else
#      pragma comment(lib,"Dev/FileIO.lib")
#    endif
#  else
#    ifdef _DEBUG
#      pragma comment(lib,"FileIOD.lib")
#    else
#      pragma comment(lib,"FileIO.lib")
#    endif
#  endif
#endif

#ifndef CLASSDEF_FILEIO
#define CLASSDEF_FILEIO class FileIO
#define CLASSDEF_FILEIO_MANAGEMENT class FileIOManagement
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

class FileIO : public MikanIO
{
protected:
	int size;
	FILE *file;
	int bufsize;
	char *filepath;
public:
	FileIO( void );
	virtual ~FileIO( void );
	static int FileExist( const char *filepath );
	// 読み込みモードで開く。-1で失敗。
	virtual int Open( const char *io );
	// 開く。-1で失敗。
	virtual int Open( const char *io, const char *mode );
	// 読み込む。NULLで失敗。
	virtual void * Read( unsigned int read_size = 0, int *readed_size = NULL );
	// 読み込む(領域は確保しない)。
	virtual int Read( void *data, unsigned int read_size, int *readed_size = NULL );
	// 1行読み込む。
	virtual char * ReadLine( unsigned int read_size = 0 );
	// 1行読み込む。
	virtual int ReadLine( char *buf, unsigned int max_size, unsigned int *read_size = NULL );
	// シークする。-1で失敗。
	virtual int Seek( int offset, int seekmode = SEEK_CUR ) const;
	// 書き込む。-1で失敗。書き込んだサイズを返す。
	virtual int Write( const void *write_data, int write_size = -1 );
	virtual int Write( const char *write_data, int write_size = -1 );
	// 今まで読んだバイト数を返す。-1で失敗。
	virtual int Tell( void ) const;
	// サイズを返す。-1で失敗。もしくはTell。
	virtual int Size( void ) const;
	// 閉じる。-1で失敗。
	virtual int Close( void );

	virtual char *GetFilePath( void );

	// バッファサイズの変更。
	virtual int SetBufferSize( int bufsize = MIKANIO_DEFAULT_BUFFER );

};

class FileIOManagement : public MikanIOManagementBase
{
public:
	FileIOManagement( unsigned int max = MIKANIO_MAX );
	// 開く。-1で失敗。
	virtual int Open( unsigned int ionum, const char *io, const char *mode );
	// 1行読み込む。
	virtual char * ReadLine( unsigned int ionum, unsigned int read_size = 0 );
	virtual int ReadLine( unsigned int ionum, char *buf, unsigned int max_size, unsigned int *read_size = NULL );

	virtual int Write( unsigned int ionum, const char *write_data, int write_size = -1 );
	virtual int Printf( unsigned int ionum, int bufsize, const char *format, ... );

	static int Remove( const char *file );
	virtual int Remove( unsigned int ionum );

	static int Move( const char *oldfile, const char *newfile );
	virtual int Move( unsigned int ionum, const char *newfile );
	virtual int Move( unsigned int oldio, unsigned int newio );

	// FileIOの管理数変更。
	virtual int InitIO( unsigned int max );
	// FileIOの取得。
	virtual class FileIO * GetIO( unsigned int ionum );
	// MikanIOの取得。
	virtual class MikanIO * GetMikanIO( unsigned int ionum );
};

#endif
