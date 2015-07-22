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
	// �ǂݍ��݃��[�h�ŊJ���B-1�Ŏ��s�B
	virtual int Open( const char *io );
	// �J���B-1�Ŏ��s�B
	virtual int Open( const char *io, const char *mode );
	// �ǂݍ��ށBNULL�Ŏ��s�B
	virtual void * Read( unsigned int read_size = 0, int *readed_size = NULL );
	// �ǂݍ���(�̈�͊m�ۂ��Ȃ�)�B
	virtual int Read( void *data, unsigned int read_size, int *readed_size = NULL );
	// 1�s�ǂݍ��ށB
	virtual char * ReadLine( unsigned int read_size = 0 );
	// 1�s�ǂݍ��ށB
	virtual int ReadLine( char *buf, unsigned int max_size, unsigned int *read_size = NULL );
	// �V�[�N����B-1�Ŏ��s�B
	virtual int Seek( int offset, int seekmode = SEEK_CUR ) const;
	// �������ށB-1�Ŏ��s�B�������񂾃T�C�Y��Ԃ��B
	virtual int Write( const void *write_data, int write_size = -1 );
	virtual int Write( const char *write_data, int write_size = -1 );
	// ���܂œǂ񂾃o�C�g����Ԃ��B-1�Ŏ��s�B
	virtual int Tell( void ) const;
	// �T�C�Y��Ԃ��B-1�Ŏ��s�B��������Tell�B
	virtual int Size( void ) const;
	// ����B-1�Ŏ��s�B
	virtual int Close( void );

	virtual char *GetFilePath( void );

	// �o�b�t�@�T�C�Y�̕ύX�B
	virtual int SetBufferSize( int bufsize = MIKANIO_DEFAULT_BUFFER );

};

class FileIOManagement : public MikanIOManagementBase
{
public:
	FileIOManagement( unsigned int max = MIKANIO_MAX );
	// �J���B-1�Ŏ��s�B
	virtual int Open( unsigned int ionum, const char *io, const char *mode );
	// 1�s�ǂݍ��ށB
	virtual char * ReadLine( unsigned int ionum, unsigned int read_size = 0 );
	virtual int ReadLine( unsigned int ionum, char *buf, unsigned int max_size, unsigned int *read_size = NULL );

	virtual int Write( unsigned int ionum, const char *write_data, int write_size = -1 );
	virtual int Printf( unsigned int ionum, int bufsize, const char *format, ... );

	static int Remove( const char *file );
	virtual int Remove( unsigned int ionum );

	static int Move( const char *oldfile, const char *newfile );
	virtual int Move( unsigned int ionum, const char *newfile );
	virtual int Move( unsigned int oldio, unsigned int newio );

	// FileIO�̊Ǘ����ύX�B
	virtual int InitIO( unsigned int max );
	// FileIO�̎擾�B
	virtual class FileIO * GetIO( unsigned int ionum );
	// MikanIO�̎擾�B
	virtual class MikanIO * GetMikanIO( unsigned int ionum );
};

#endif
