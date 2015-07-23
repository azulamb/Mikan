#ifndef CLASSDEF_NETWORK
#define CLASSDEF_NETWORK class MikanNetwork

// MikanNetworkはMikanSocketに依存するライブラリ。
#ifndef _MIKANDEVELOPER
#ifdef _MIKANDEV
#include "Dev/MikanSocket.h"
#else
#include "MikanSocket.h"
#endif
#else
#include "../MikanSocket/MikanSocket.h"
#endif

#ifndef _UNLINK_LIBRARY
#  ifdef _MIKANDEV
#    ifdef _DEBUG
#      pragma comment( lib, "Dev/MikanNetworkD.lib" )
#    else
#      pragma comment( lib, "Dev/MikanNetwork.lib" )
#    endif
#  else
#    ifdef _DEBUG
#      pragma comment( lib, "MikanNetworkD.lib" )
#    else
#      pragma comment( lib, "MikanNetwork.lib" )
#    endif
#  endif
#endif

#define SAFE_RELEASE(r)   { if( r ){ ( r )->Release(); } r = NULL; }
#define SAFE_FREE(f)      { if( f ){ free( f ); } f = NULL; }

#define CanUseNetnum( n )    ( n < socket_max )
#define CannotUseNetnum( n ) ( socket_max <= n )
#define CanUseNet( n )       ( n < socket_max && msocket[ n ] )
#define CannotUseNet( n )    ( socket_max <= n || msocket[ n ] == NULL )

#define CONNECT_MAX 8

/*struct STACKSTRDATA
{
char *Pointer;
struct STACKSTRDATA * Next;
};*/

struct MIKANCLIENT
{
	unsigned long ipaddr;
	unsigned long lastlogin;
	struct MIKANCLIENT *next;
};

struct P2PTHREADDATA
{
	HANDLE handle;
	unsigned long threadid;
	unsigned long addr;
	unsigned short port;
	int connect;
	int type;
	void *func; // int *func( struct MIKANCLIENT *);
	struct MIKANCLIENT *clientlist;
	// クライアント数
	int client_max;
	// ロック絡み。
	HANDLE lockhandle;
	char locknames[ 16 ];
	// 時間
	unsigned long p2ptimeout;
	// MikanSocket
	class MikanSocket *server, *client;
};

//Mikan Net Type
enum
{
	MIKAN_NET_TCP = SOCK_STREAM,
	MIKAN_NET_UDP = SOCK_DGRAM,
	MIKAN_NET_BROADCAST, // UDP Only
	MIKAN_NET_MULTICAST, // UDP Only
	MIKAN_NET_AUTO,
};

class MikanNetwork
{
protected:
	static char base64table_enc[ 64 ];
	static char base64table_dec[ 256 ];
	static char urlencodetable[ 256 ];

	unsigned int socket_max, p2p_max;

	struct P2PTHREADDATA *p2pthread;

	class MikanSocket **msocket;

	fd_set fds, readfds;

	static class MikanSocket * HttpConnect( const char *address, const char *proxy, const char *user, const char *password );
	static int HttpGetReturnCode( class MikanSocket *msock );
	static int HttpAnalysisHeader( class MikanSocket *msock, int *contentlength, char **location );
	static int HttpReceiveBody( class MikanSocket *msock, const char *filepath, const char *address, int *loadsize );
public:

	MikanNetwork( void );
	virtual ~MikanNetwork( void );

	virtual int Init( void );

	virtual int SecondInit( void );

	virtual int Release( void );

	virtual class MikanIO * GetMikanIO( unsigned int nnum );
	virtual class MikanSocket * GetMikanSocket( unsigned int nnum );

	static int CheckUDPAddressArea( unsigned int address );

	// サーバーモードにする。一度だけ実行可能。
	virtual SOCKET CreateTCPServer( unsigned int nnum, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	virtual SOCKET CreateUDPServer( unsigned int nnum, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	virtual SOCKET CreateBloadcastServer( unsigned int nnum, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	virtual SOCKET CreateMulticastServer( unsigned int nnum, const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );
	virtual SOCKET CreateMulticastServer( unsigned int nnum, unsigned long address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT, int connect = MIKAN_SOCKET_DEFAULT_CONNECT_MAX );

	// クライアントモードにする。
	virtual SOCKET CreateTCPClient( unsigned int nnum, const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	// クライアントモードにする。
	virtual SOCKET CreateUDPClient( unsigned int nnum, const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	virtual SOCKET CreateBloadcastClient( unsigned int nnum, const char *address = "255.255.255.255", unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );
	virtual SOCKET CreateMulticastClient( unsigned int nnum, const char *address, unsigned short port = MIKAN_SOCKET_DEFAULT_PORT );

	virtual int Close( unsigned int nnum );

	virtual int Send( unsigned int nnum, const char *str );
	virtual int Send( unsigned int nnum, const char *str, unsigned size );

	virtual char * Receive( unsigned int nnum, int *getsize, struct sockaddr *from = NULL );
	virtual int Receive( unsigned int nnum, char *buf, int size, struct sockaddr *from = NULL );

	virtual SOCKET Accept( unsigned int nnum, struct sockaddr_in *client = NULL );

	virtual int RegisterSelect( unsigned int nnum );
	// タイムアウトなどこれ以上Selectを続けない場合は 0 を返す。
	// 使い方は次のような感じ。
	// RegisterSelect( 0 );
	// RegisterSelect( 1 );
	// while( Select() ){
	//   if ( IsSelected( 0 ) ){ 処理 }
	//   if ( IsSelected( 1 ) ){ 処理 }
	// }
	virtual int Select( long timeout_sec = 0, long timeout_usec = 0 );
	virtual int IsSelected( unsigned int nnum );

	// ネットワークサポート
	static unsigned long * GetNicAddrList( int *size );

	// HTTPサポート
	static int EncodeBase64( const char *src, char *dist = NULL, int size = -1, const char *newline = "\n" );
	static int DecodeBase64( const char *src, char *dist = NULL, int size = -1 );
	static int EncodeURL( const char *src, char *dist );
	static int SplitHost( const char *src, char *dist, unsigned short *port = NULL );
	static int SplitPath( const char *src, char *dist );
	static int HttpGet( const char *address, const char *filepath = NULL, const char *proxy = NULL, const char *user = NULL, const char *password = NULL, int *getsize = NULL, int *loadsize = NULL );
	static int HttpPost( const char *address, const char *data = NULL, int datasize = 0, const char *filepath = NULL, const char *proxy = NULL, const char *user = NULL, const char *password = NULL, int *getsize = NULL, int *loadsize = NULL );

	// P2P。自動的にスレッドを生成してしまうので、各メソッドはデッドロックを起こさないよう頑張ること。
	virtual int CreateP2PNetwork( unsigned int p2pnum, const char *addr, unsigned short port, int type = MIKAN_NET_AUTO, int *func( char *msg, int size, struct sockaddr_in *addr ) = NULL, unsigned long timeout = 10000 );
	virtual int StopP2PNetwork( unsigned int p2pnum );
	virtual int RunP2PNetwork( struct P2PTHREADDATA *td );
	virtual struct MIKANCLIENT * GetP2PClientList( unsigned int p2pnum, unsigned int *max = NULL );
	virtual struct MIKANCLIENT * AddClientList( unsigned int p2pnum, unsigned long address );
	virtual int ClearP2PClientList( unsigned int p2pnum );
protected:
	virtual int LockP2PClientList( unsigned int p2pnum, unsigned long timeout = INFINITE );
	virtual int UnLockP2PClientList( unsigned int p2pnum );
public:

};



#endif