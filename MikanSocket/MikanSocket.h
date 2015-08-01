#ifndef CLASSDEF_NETWORK_SOCKET
#define CLASSDEF_NETWORK_SOCKET class MikanSocket

#define DEFAULT_SERVER_MAX 5

// MikanSocket
// 基本的にソケットを簡易的に扱うためのクラスで、ソケットの使い回しなどは全く考慮していない。
// 専用に作ったソケットを使い続ける設計方針。

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

	// 汎用処理。

	// 何の属性もないソケットの作成。
	virtual SOCKET CreateSocket( int socket_type = MIKAN_SOCKET_TCP, int addr_family = AF_INET, int protocol = 0 );

	// 最後のエラー取得。
	virtual int GetLastError( void );

	// アドレスをIPアドレスの詰まったunsigned longに変換する。
	// ただし、ドメイン等を指定した場合は、複数返されるうちの0番目を強制的に使用する。
	static unsigned long AddressToULong( const char *address = "127.0.0.1", int sockettype = MIKAN_SOCKET_TCP );
	// 上の複数版。
	static unsigned long * AddressToULong( const char *address, int *size = NULL, int sockettype = MIKAN_SOCKET_TCP );

	// モードその他設定。

	// 
	virtual int SetType( int type = MIKAN_SOCKET_TCP );
	virtual int GetSocketType( void );

	//
	virtual int GetMode( void );

	// Receiveをノンブロッキングにする。
	virtual int SetNonBlockingIO( int nonblocking = 1 );

	// サーバーモードにする。一度だけ実行可能。
	virtual SOCKET CreateServer( unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX, int type = MIKAN_SOCKET_NONE );
	virtual SOCKET CreateBloadcastServer( unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	virtual SOCKET CreateMulticastServer( const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	virtual SOCKET CreateMulticastServer( unsigned long address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );

	// クライアントモードにする。TCP/UDP共通。一度だけ実行可能。
	virtual SOCKET CreateClient( const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int type = MIKAN_SOCKET_NONE );
	// クライアントモードにする。UDP専用(と言いつつ上のラッパー)。一度だけ実行可能。
	virtual SOCKET CreateUDPClient( const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	virtual SOCKET CreateBloadcastClient( const char *address = "255.255.255.255", unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	virtual SOCKET CreateMulticastClient( const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );

	// MikanIO

	// ファイルを指定することで、そこに保存しつつ行う。
	virtual int Open( const char *io );
	// 読み込む。NULLで失敗。
	virtual void * Read( unsigned int read_size = 0, int *readed_size = NULL );
	// 読み込む。領域は確保しない。
	virtual int Read( void *data, unsigned int read_size, int *readed_size = NULL );
	// シークする。効果なし。
	virtual int Seek( int offset, int seekmode = SEEK_CUR ) const;
	// 書き込む。-1で失敗。書き込んだサイズを返す。
	virtual int Write( const void *write_data, int write_size = -1 );
	// 今まで読んだバイト数を返す。-1で失敗。
	virtual int Tell( void ) const;
	// サイズを返す。ここではTell。
	virtual int Size( void ) const;
	// ソケットを閉じるだけ。
	virtual int Close( void );

	// 共通処理。

	virtual int Send( const char *str );
	virtual int Send( const char *data, int size );

	virtual char * Receive( int *getsize, struct sockaddr *from = NULL );
	virtual int Receive( char *buf, int size, struct sockaddr *from = NULL );

	virtual SOCKET GetSocket( void );
	// モードも含めて解放。
	// モードの再設定なども可能となる。
	virtual int Release( void );

	// サーバー限定。
	virtual SOCKET Accept( struct sockaddr_in *client = NULL );
	virtual struct sockaddr_in * GetAddr( struct sockaddr_in *client = NULL );
	virtual int ResetSelect( void );
	virtual int Select( void );

};

#endif
