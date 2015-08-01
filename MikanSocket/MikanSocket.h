#ifndef CLASSDEF_NETWORK_SOCKET
#define CLASSDEF_NETWORK_SOCKET class MikanSocket

#define DEFAULT_SERVER_MAX 5

// MikanSocket
// ��{�I�Ƀ\�P�b�g���ȈՓI�Ɉ������߂̃N���X�ŁA�\�P�b�g�̎g���񂵂Ȃǂ͑S���l�����Ă��Ȃ��B
// ��p�ɍ�����\�P�b�g���g��������݌v���j�B

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USEMIKANLIBRARY
#ifdef _MIKANDEV
#include "Dev/MikanIO.h"
#include "Dev/FileIO.h"
#else
#include "MikanIO.h"
#include "FileIO.h"
#endif
#else
#include "../MikanIO/MikanIO.h"
#include "../FileIO/FileIO.h"
#endif

#ifndef _UNLOADLIBRARY
#  pragma comment ( lib, "ws2_32.lib" )
#  ifdef _MIKANDEV
#    ifdef _DEBUG
#      pragma comment( lib, "Dev/MikanSocketD.lib" )
#    else
#      pragma comment( lib, "Dev/MikanSocket.lib" )
#    endif
#  else
#    ifdef _DEBUG
#      pragma comment( lib, "MikanSocketD.lib" )
#    else
#      pragma comment( lib, "MikanSocket.lib" )
#    endif
#  endif
#endif

#define MIKAN_SOCKET_DEFAULT_PORT 61000
#define MIKAN_SOCKET_DEFAULT_CONNECT_MAX 5

enum
{
	MIKAN_SOCKET_TCP = SOCK_STREAM,
	MIKAN_SOCKET_UDP = SOCK_DGRAM,
	MIKAN_SOCKET_BROADCAST = -3,
	MIKAN_SOCKET_MULTICAST = -2,
	MIKAN_SOCKET_NONE = -1,
};

enum
{
	MIKAN_SOCKET_MODE_SERVER = 0,
	MIKAN_SOCKET_MODE_CLIENT,
	MIKAN_SOCKET_MODE_NONE
};

struct SOCKETDATA
{
	unsigned int Sock;
	struct sockaddr_in Sockaddr;
};

class MikanSocket : public MikanIO
{
protected:
	struct WSAData wsadata;
	int mode;
	int wsaerr;
	SOCKET sock, ssock;
	int port;
	int type;
	int connect_max;
	struct sockaddr_in addr;
	unsigned long nonblocking;
	unsigned int loadsize;
	class FileIO *fileio;
public:
	MikanSocket( int major_version = 2, int minor_version = 0 );
	virtual ~MikanSocket( void );

	// �ėp�����B

	// ���̑������Ȃ��\�P�b�g�̍쐬�B
	virtual SOCKET CreateSocket( int socket_type = MIKAN_SOCKET_TCP, int addr_family = AF_INET, int protocol = 0 );

	// �Ō�̃G���[�擾�B
	virtual int GetLastError( void );

	// �A�h���X��IP�A�h���X�̋l�܂���unsigned long�ɕϊ�����B
	// �������A�h���C�������w�肵���ꍇ�́A�����Ԃ���邤����0�Ԗڂ������I�Ɏg�p����B
	static unsigned long AddressToULong( const char *address = "127.0.0.1", int sockettype = MIKAN_SOCKET_TCP );
	// ��̕����ŁB
	static unsigned long * AddressToULong( const char *address, int *size = NULL, int sockettype = MIKAN_SOCKET_TCP );

	// ���[�h���̑��ݒ�B

	// 
	virtual int SetType( int type = MIKAN_SOCKET_TCP );
	virtual int GetSocketType( void );

	//
	virtual int GetMode( void );

	// Receive���m���u���b�L���O�ɂ���B
	virtual int SetNonBlockingIO( int nonblocking = 1 );

	// �T�[�o�[���[�h�ɂ���B��x�������s�\�B
	virtual SOCKET CreateServer( unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX, int type = MIKAN_SOCKET_NONE );
	virtual SOCKET CreateBloadcastServer( unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	virtual SOCKET CreateMulticastServer( const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	virtual SOCKET CreateMulticastServer( unsigned long address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );

	// �N���C�A���g���[�h�ɂ���BTCP/UDP���ʁB��x�������s�\�B
	virtual SOCKET CreateClient( const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int type = MIKAN_SOCKET_NONE );
	// �N���C�A���g���[�h�ɂ���BUDP��p(�ƌ�����̃��b�p�[)�B��x�������s�\�B
	virtual SOCKET CreateUDPClient( const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	virtual SOCKET CreateBloadcastClient( const char *address = "255.255.255.255", unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	virtual SOCKET CreateMulticastClient( const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );

	// MikanIO

	// �t�@�C�����w�肷�邱�ƂŁA�����ɕۑ����s���B
	virtual int Open( const char *io );
	// �ǂݍ��ށBNULL�Ŏ��s�B
	virtual void * Read( unsigned int read_size = 0, int *readed_size = NULL );
	// �ǂݍ��ށB�̈�͊m�ۂ��Ȃ��B
	virtual int Read( void *data, unsigned int read_size, int *readed_size = NULL );
	// �V�[�N����B���ʂȂ��B
	virtual int Seek( int offset, int seekmode = SEEK_CUR ) const;
	// �������ށB-1�Ŏ��s�B�������񂾃T�C�Y��Ԃ��B
	virtual int Write( const void *write_data, int write_size = -1 );
	// ���܂œǂ񂾃o�C�g����Ԃ��B-1�Ŏ��s�B
	virtual int Tell( void ) const;
	// �T�C�Y��Ԃ��B�����ł�Tell�B
	virtual int Size( void ) const;
	// �\�P�b�g����邾���B
	virtual int Close( void );

	// ���ʏ����B

	virtual int Send( const char *str );
	virtual int Send( const char *data, int size );

	virtual char * Receive( int *getsize, struct sockaddr *from = NULL );
	virtual int Receive( char *buf, int size, struct sockaddr *from = NULL );

	virtual SOCKET GetSocket( void );
	// ���[�h���܂߂ĉ���B
	// ���[�h�̍Đݒ�Ȃǂ��\�ƂȂ�B
	virtual int Release( void );

	// �T�[�o�[����B
	virtual SOCKET Accept( struct sockaddr_in *client = NULL );
	virtual struct sockaddr_in * GetAddr( struct sockaddr_in *client = NULL );
	virtual int ResetSelect( void );
	virtual int Select( void );

};

#endif
