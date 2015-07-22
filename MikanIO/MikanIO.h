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
	// �J���B-1�Ŏ��s�B
	virtual int Open( const char *io ) = 0;
	// �ǂݍ��ށBNULL�Ŏ��s�B
	virtual void * Read( unsigned int read_size = 0, int *readed_size = NULL ) = 0;
	// �ǂݍ��ށB�̈�͊m�ۂ��Ȃ��B
	virtual int Read( void *data, unsigned int read_size, int *readed_size = NULL ) = 0;
	// �V�[�N����B-1�Ŏ��s�B
	virtual int Seek( int offset, int seekmode = SEEK_CUR ) const = 0;
	// �������ށB-1�Ŏ��s�B�������񂾃T�C�Y��Ԃ��B
	virtual int Write( const void *write_data, int write_size = -1 ) = 0;
	// ���܂œǂ񂾃o�C�g����Ԃ��B-1�Ŏ��s�B
	virtual int Tell( void ) const = 0;
	// �T�C�Y��Ԃ��B-1�Ŏ��s�B��������Tell�B
	virtual int Size( void ) const = 0;
	// ����B-1�Ŏ��s�B
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
	// �J���B-1�Ŏ��s�B
	virtual int Open( unsigned int ionum, const char *io );
	// �ǂݍ��ށBNULL�Ŏ��s�B
	virtual void * Read( unsigned int ionum, unsigned int read_size = 0, int *readed_size = NULL );
	// �ǂݍ��ށB�̈�͊m�ۂ��Ȃ��B
	virtual int Read( unsigned int ionum, void *data, unsigned int read_size, int *readed_size = NULL );
	// �V�[�N����B-1�Ŏ��s�B
	virtual int Seek( unsigned int ionum, int offset, int seekmode = SEEK_CUR ) const;
	// �������ށB-1�Ŏ��s�B�������񂾃T�C�Y��Ԃ��B
	virtual int Write( unsigned int ionum, const void *write_data, int write_size = -1 );
	// ���܂œǂ񂾃o�C�g����Ԃ��B-1�Ŏ��s�B
	virtual int Tell( unsigned int ionum ) const;
	// �T�C�Y��Ԃ��B-1�Ŏ��s�B��������Tell�B
	virtual int Size( unsigned int ionum ) const;
	// ����B-1�Ŏ��s�B
	virtual int Close( unsigned int ionum );

	// IO��j������B
	virtual int Destroy( unsigned int ionum );
	// ���ׂĕ���B���邱�Ƃɐ�������IO�̐���Ԃ��B
	virtual int CloseAll( void );
};

#endif
