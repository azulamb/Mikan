/*!
@class   Socket Socket.h "Socket.h"
@brief   Socket通信を行うクラス
@author  Hiroki
@version 0.8
*/

#include "MikanSocket.h"

MikanSocket::MikanSocket( int major_version, int minor_version )
{
	// 共通設定。

	fileio = NULL;

	// ソケットタイプを未設定にする。
	mode = MIKAN_SOCKET_MODE_NONE;

	// ソケットを無効化。
	sock = ssock = INVALID_SOCKET;

	// ポート番号を適当に設定。
	port = MIKAN_SOCKET_DEFAULT_PORT;

	// 通信をTCPに設定する。
	SetType();

	// サーバー用設定。

	// 最大接続数。
	connect_max = MIKAN_SOCKET_DEFAULT_CONNECT_MAX;

	// 処理開始。
	wsaerr = WSAStartup( MAKEWORD( major_version, minor_version ), &wsadata );

	SetNonBlockingIO( 0 );

	switch ( wsaerr )
	{
	case 0:
		// エラー無し。
		break;
	case WSASYSNOTREADY:
		// ネットワークに接続できない。
		break;
	case WSAVERNOTSUPPORTED:
		// WinSockのバージョンがおかしい。
		break;
	case WSAEINPROGRESS:
		// 諸事情でブロックされてる。
		break;
	case WSAEPROCLIM:
		// Winsockの最大処理プロセス数に達してしまった。
		break;
	case WSAEFAULT:
		// wsadataのポインタがおかしい。
		break;
	}

}

MikanSocket::~MikanSocket( void )
{
	Release();
	WSACleanup();
}

// 何の属性もないソケットの作成。
SOCKET MikanSocket::CreateSocket( int socket_type, int addr_family, int protocol )
{
	Open( NULL );
	switch ( socket_type )
	{
	case MIKAN_SOCKET_BROADCAST:
	case MIKAN_SOCKET_MULTICAST:
		socket_type = MIKAN_SOCKET_UDP;
		break;
	}
	return socket( addr_family, socket_type, protocol );
}

int MikanSocket::GetLastError( void )
{
	return WSAGetLastError();
}

unsigned long MikanSocket::AddressToULong( const char *address, int sockettype )
{
	unsigned long ipad;
	struct addrinfo hints, *res;
	//struct in_addr addr;
	int err;
	WSADATA data;

	ipad = inet_addr( address );

	if ( ipad != 0xffffffff || strcmp( address, "255.255.255.255" ) == 0 )
	{
		return ipad;
	}

	WSAStartup( MAKEWORD( 2, 0 ), &data );

	memset( &hints, 0, sizeof( struct addrinfo ) );
	hints.ai_socktype = sockettype;
	hints.ai_family = AF_INET;

	if ( ( err = getaddrinfo( address, NULL, &hints, &res ) ) != 0 )
	{
		return 0xffffffff;
	}

	ipad = ( ( struct sockaddr_in * )( res->ai_addr ) )->sin_addr.S_un.S_addr;

	freeaddrinfo( res );

	WSACleanup();

	return ipad;
	/*  unsigned long ipad;
	  struct hostent *host;

	  ipad = inet_addr( address );

	  if ( ipad == 0xffffffff && strcmp( address, "255.255.255.255" ) )
	  {
	  host = gethostbyname( address );
	  if ( host == NULL )
	  {
	  return ipad;
	  }

	  ipad = *(unsigned int *)host->h_addr_list[ 0 ];
	  }

	  return ipad;*/
}

unsigned long * MikanSocket::AddressToULong( const char *address, int *size, int sockettype )
{
	unsigned long *ret = NULL, ipad;
	struct addrinfo hints, *res, *next;
	//struct in_addr addr;
	int err, count;
	WSADATA data;

	ipad = inet_addr( address );

	if ( ipad != 0xffffffff || strcmp( address, "255.255.255.255" ) == 0 )
	{
		ret = (unsigned long *)malloc( sizeof( unsigned long ) );
		ret[ 0 ] = ipad;
		if ( size )
		{
			*size = 1;
		}
		return ret;
	}

	WSAStartup( MAKEWORD( 2, 0 ), &data );

	memset( &hints, 0, sizeof( struct addrinfo ) );
	hints.ai_socktype = sockettype;
	hints.ai_family = AF_INET;

	if ( ( err = getaddrinfo( address, NULL, &hints, &res ) ) != 0 )
	{
		if ( size )
		{
			*size = 0;
		}
		return NULL;
	}

	count = 0;
	ret = (unsigned long *)malloc( sizeof( unsigned long ) );
	next = res;
	while ( next )
	{
		ret[ count++ ] = ( ( struct sockaddr_in * )( next->ai_addr ) )->sin_addr.S_un.S_addr;

		next = next->ai_next;
		if ( next )
		{
			ret = (unsigned long *)realloc( (void *)ret, ( count + 1 ) * sizeof( unsigned long ) );
		}
	}

	freeaddrinfo( res );

	if ( size )
	{
		*size = count;
	}

	WSACleanup();

	return ret;
	/*  unsigned long ipad, *ret = NULL;
	  unsigned int **addrptr;
	  int count;
	  struct hostent *host;

	  ipad = inet_addr( address );

	  if ( ipad == 0xffffffff && strcmp( address, "255.255.255.255" ) )
	  {
	  host = gethostbyname( address );
	  if ( host == NULL )
	  {
	  if ( size ){ *size = 0; }
	  return NULL;
	  }

	  addrptr = (unsigned int **)host->h_addr_list;
	  count = 1;
	  ret = (unsigned long *)calloc( count, sizeof(unsigned long) );

	  while ( *addrptr )
	  {
	  ret[ count ] = *(*addrptr);
	  addrptr++;
	  if ( *addrptr )
	  {
	  ++count;
	  ret = (unsigned long *)realloc( (void *)ret, count * sizeof(unsigned long) );
	  }
	  }

	  if( size ){ *size = count; }

	  } else
	  {
	  if ( size ){ *size = 1; }
	  ret = (unsigned long *)calloc( 1, sizeof(unsigned long) );
	  ret[ 0 ] = ipad;
	  }

	  return ret;*/
}

int MikanSocket::SetType( int type )
{
	if ( mode != MIKAN_SOCKET_MODE_NONE )
	{
		// 途中での変更は不可。
		return 1;
	}

	this->type = type;
	return 0;
}

int MikanSocket::GetSocketType( void )
{
	switch ( type )
	{
	case MIKAN_SOCKET_BROADCAST:
	case MIKAN_SOCKET_MULTICAST:
		return MIKAN_SOCKET_UDP;
	}

	return type;
}

int MikanSocket::GetMode( void ){
	return mode;
}

// Receiveをノンブロッキングにする。
int MikanSocket::SetNonBlockingIO( int nonblocking )
{
	this->nonblocking = nonblocking;

	ioctlsocket( sock, FIONBIO, &( this->nonblocking ) );

	return 0;
}

// サーバーモードにする。一度だけ実行可能。
SOCKET MikanSocket::CreateServer( unsigned short port, int connect, int type )
{
	struct sockaddr_in server;
	int yes;

	if ( mode != MIKAN_SOCKET_MODE_NONE )
	{
		return INVALID_SOCKET;
	}

	if ( type != MIKAN_SOCKET_NONE )
	{
		SetType( type );
	}

	mode = MIKAN_SOCKET_MODE_SERVER;

	connect_max = connect;

	// ソケットの設定。
	server.sin_port = this->port = htons( port );
	server.sin_family = AF_INET;//AF_INET6
	server.sin_addr.S_un.S_addr = INADDR_ANY;

	sock = ssock = CreateSocket( GetSocketType(), server.sin_family, 0 );

	if ( sock == INVALID_SOCKET )
	{
		return INVALID_SOCKET;
	}

	// bind時のtime_waitによるエラー回避。
	setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof( int ) );

	if ( bind( sock, ( struct sockaddr * )&server, sizeof( struct sockaddr_in ) ) != 0 )
	{
		// bindエラー。
		return INVALID_SOCKET;
	}

	// TCPサーバーの場合はlistenする。
	if ( GetSocketType() == MIKAN_SOCKET_TCP && listen( sock, connect_max ) != 0 )
	{
		// listenエラー。
		return INVALID_SOCKET;
	}

	return sock;
}

SOCKET MikanSocket::CreateBloadcastServer( unsigned short port, int connect )
{
	return CreateServer( port, connect, MIKAN_SOCKET_BROADCAST );
}

SOCKET MikanSocket::CreateMulticastServer( const char *address, unsigned short port, int connect )
{
	return CreateMulticastServer( AddressToULong( address, MIKAN_SOCKET_UDP ), port, connect );
}

SOCKET MikanSocket::CreateMulticastServer( unsigned long address, unsigned short port, int connect )
{
	Open( NULL );
	return 0;
}

// クライアントモードにする。一度だけ実行可能。
SOCKET MikanSocket::CreateClient( const char *address, unsigned short port, int type )
{
	//int yes;
	struct sockaddr_in target_server;

	//int errcode;

	if ( mode != MIKAN_SOCKET_MODE_NONE )
	{
		return INVALID_SOCKET;
	}

	if ( type != MIKAN_SOCKET_NONE )
	{
		SetType( type );
	}

	mode = MIKAN_SOCKET_MODE_CLIENT;

	this->port = port;
	type = this->type;


	switch ( type )
	{
	case MIKAN_SOCKET_TCP:
		// 接続先の設定。
		target_server.sin_port = htons( port );
		target_server.sin_family = AF_INET;
		target_server.sin_addr.S_un.S_addr = AddressToULong( address, MIKAN_SOCKET_TCP );


		sock = ssock = CreateSocket( type, target_server.sin_family, 0 );

		if ( sock == INVALID_SOCKET )
		{
			return INVALID_SOCKET;
		}

		// サーバに接続。
		if ( connect( sock, ( struct sockaddr * )&target_server, sizeof( struct sockaddr_in ) ) != 0 )
		{
			return INVALID_SOCKET;
		}
		break;
	case MIKAN_SOCKET_UDP:
		// 接続先の設定。
		addr.sin_port = htons( port );
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = AddressToULong( address, MIKAN_SOCKET_UDP );

		sock = ssock = CreateSocket( MIKAN_SOCKET_UDP, addr.sin_family, 0 );
		break;
	case MIKAN_SOCKET_BROADCAST:
		// ブロードキャスト用の設定。

		// 接続先の設定。
		sock = ssock = CreateSocket( MIKAN_SOCKET_UDP, AF_INET, 0 );

		/*    if ( (errcode = setsockopt( sock, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof( int ) ) ) != 0 )
			{
			return errcode;//INVALID_SOCKET;
			}*/

		memset( &addr, 0, sizeof( struct sockaddr_in ) );
		addr.sin_port = htons( port );
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = 0xffffffff;//AddressToULong( address );

		break;
	case MIKAN_SOCKET_MULTICAST:
		// マルチキャスト用の設定。
		addr.sin_port = htons( port );
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = AddressToULong( address, MIKAN_SOCKET_UDP );
		sock = ssock = CreateSocket( MIKAN_SOCKET_UDP, addr.sin_family, 0 );
		if ( setsockopt( sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&( addr.sin_addr.S_un.S_addr ), sizeof( unsigned long ) ) != 0 )
		{
			return INVALID_SOCKET;
		}
		break;
	}

	return sock;
}

SOCKET MikanSocket::CreateUDPClient( const char *address, unsigned short port )
{
	return CreateClient( address, port, MIKAN_SOCKET_UDP );
}

SOCKET MikanSocket::CreateBloadcastClient( const char *address, unsigned short port )
{
	return CreateClient( address, port, MIKAN_SOCKET_BROADCAST );
}

SOCKET MikanSocket::CreateMulticastClient( const char *address, unsigned short port )
{
	return CreateClient( address, port, MIKAN_SOCKET_MULTICAST );
}

// MikanIO

// 開く。効果なし。読み込みサイズの初期化とかくらい。
int MikanSocket::Open( const char *io )
{
	loadsize = 0;
	if ( io == NULL )
	{
		return -1;
	}
	// FileIOでファイルを開く。
	if ( fileio )
	{
		fileio->Close();
	} else
	{
		fileio = new FileIO();
	}
	if ( fileio == NULL )
	{
		return -1;
	}
	return fileio->Open( io, "a+" );
}

// 読み込む。NULLで失敗。
void * MikanSocket::Read( unsigned int read_size, int *readed_size )
{
	int loaded = 0;
	// 強制的にReceive。
	void *ret = (void *)Receive( &loaded, NULL );
	if ( fileio )
	{
		fileio->Write( ret, loaded );
	}
	if ( readed_size )
	{
		*readed_size = loaded;
	}
	loadsize += loaded;
	return ret;
}

// 読み込む。領域は確保しない。
int MikanSocket::Read( void *data, unsigned int read_size, int *readed_size )
{
	int load;
	// ファイルの現在地から適切に算出。
	if ( fileio && fileio->Tell() < Tell() )
	{
		// 現在地からならファイルから読む。
		fileio->Read( data, read_size, &load );
		// ここはある種のデバッグのために、最新情報に保つ。消さないこと。
		if ( readed_size )
		{
			*readed_size = load;
		}

		// 足りない場合はReceiveしてみる？
		if ( (unsigned int)load < read_size )
		{
			loadsize += load;
			load = Receive( (char *)data + load, read_size - load, NULL );
			if ( fileio )
			{
				fileio->Write( data, load );
			};
		}

	} else
	{
		// 末尾ならReceive。
		load = Receive( (char *)data, read_size, NULL );
		if ( fileio )
		{
			fileio->Write( data, load );
		};
	}

	if ( readed_size )
	{
		*readed_size = load;
	}
	loadsize += load;

	return load;
}

// シークする。
int MikanSocket::Seek( int offset, int seekmode ) const
{
	// ファイルシーク。
	if ( fileio )
	{
		fileio->Seek( offset, seekmode );
	}
	return 0;
}

// 書き込む。-1で失敗。書き込んだサイズを返す。
int MikanSocket::Write( const void *write_data, int write_size )
{
	if ( write_size < 0 )
	{
		return Send( (char *)write_data );
	}
	return Send( (char *)write_data, write_size );
}

// 今まで読んだバイト数を返す。-1で失敗。
int MikanSocket::Tell( void ) const
{
	return loadsize;
}

// サイズを返す。ここではTell。
int MikanSocket::Size( void ) const
{
	return Tell();
}

int MikanSocket::Close( void )
{
	closesocket( sock );
	closesocket( ssock );

	if ( fileio )
	{
		fileio->Close();
		delete( fileio );
		fileio = NULL;
	}

	// ソケットを無効化。
	sock = ssock = INVALID_SOCKET;

	return 0;
}

int MikanSocket::Send( const char *str )
{
	int len;

	len = strlen( str );

	return Send( str, len );
}

int MikanSocket::Send( const char *data, int size )
{
	struct sockaddr_in addrin;
	int loadedsize;
	int yes;

	if ( sock == INVALID_SOCKET )
	{
		return -1;
	}

	switch ( GetSocketType() )
	{
	case MIKAN_SOCKET_TCP:
		return send( sock, data, size, 0 );
	case MIKAN_SOCKET_UDP:
		// すでに設定済みの相手に送る。
		addrin.sin_family = addr.sin_family;
		addrin.sin_port = addr.sin_port;
		addrin.sin_addr.S_un.S_addr = addr.sin_addr.S_un.S_addr;

		if ( type == MIKAN_SOCKET_BROADCAST )
		{
			// ソケットにブロードキャストの属性を付ける。
			setsockopt( sock, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof( yes ) );
		}

		loadedsize = sendto( sock, data, size, 0, ( struct sockaddr * )&addrin, sizeof( struct sockaddr_in ) );//sizeof(addr) );
		loadsize += loadedsize;

		return loadedsize;
	}

	return 0;
}


char * MikanSocket::Receive( int *getsize, struct sockaddr *from )
{
	int len = 0, head = 0, fromsize;
	char * buf;

	if ( sock == INVALID_SOCKET )
	{
		return NULL;
	}

	buf = (char *)calloc( 1024 + 1, sizeof( char ) );

	if ( buf )
	{
		if ( GetSocketType() == MIKAN_SOCKET_TCP || from == NULL )
		{
			// TCP
			while ( ( len = recv( sock, buf + head, 1024, 0 ) ) && nonblocking )
			{
				// ノンブロッキングなら、データを全て受け取るまでデータを回せる。
				head += len;
				buf = (char *)realloc( buf, 1024 + head + 1 );
			}
		} else if ( GetSocketType() == MIKAN_SOCKET_UDP || from != NULL )
		{
			// UDP
			while ( ( len = recvfrom( sock, buf + head, 1024, 0, from, &fromsize ) ) && nonblocking )
			{
				// ノンブロッキングなら、データを全て受け取るまでデータを回せる。
				head += len;
				buf = (char *)realloc( buf, 1024 + head + 1 );
			}
		} else
		{
			// エラー
		}

		if ( len < 1 )
		{
			if ( nonblocking && GetLastError() == WSAEWOULDBLOCK )
			{
				// まだ来ない。
			} else
			{
				//エラー
			}
			len = 0;
		}
		len += head;
	}

	//サイズを入れるint型ポインタが指定されているならサイズを代入する。
	if ( getsize )
	{
		*getsize = len;
	}
	loadsize += len;

	buf[ len ] = '\0';

	//受け取ったデータの文字列を返す。
	return buf;
}

int MikanSocket::Receive( char *buf, int size, struct sockaddr *from )
{
	int len = 0;
	int fromsize;

	if ( sock == INVALID_SOCKET )
	{
		return -1;
	}

	if ( GetSocketType() == MIKAN_SOCKET_TCP || from == NULL )
	{
		len = recv( sock, buf, size, 0 );
	} else if ( GetSocketType() == MIKAN_SOCKET_UDP )
	{
		fromsize = sizeof( sockaddr );
		len = recvfrom( sock, buf, size, 0, from, &fromsize );
	}

	loadsize += len;

	return len;
}

SOCKET MikanSocket::GetSocket( void )
{
	return ssock;
}

int MikanSocket::Release( void )
{
	Close();

	// モードを未設定にする。
	mode = MIKAN_SOCKET_MODE_NONE;

	return 0;
}

SOCKET MikanSocket::Accept( struct sockaddr_in *client )
{
	int len;

	if ( mode != MIKAN_SOCKET_MODE_SERVER )
	{
		// サーバーモードではないので不正。
		return INVALID_SOCKET;
	}

	if ( client == NULL )
	{
		client = &addr;
	}

	len = sizeof( struct sockaddr_in );

	sock = accept( ssock, ( struct sockaddr * )client, &len );

	return sock;
}

struct sockaddr_in * MikanSocket::GetAddr( struct sockaddr_in *client )
{
	if ( mode != MIKAN_SOCKET_MODE_SERVER )
	{
		// サーバーモードではないので不正。
		return NULL;
	}

	if ( client != NULL )
	{
		// コピー。
		memcpy( client, &addr, sizeof( sockaddr_in ) );
	}

	return &addr;
}

int MikanSocket::ResetSelect( void )
{
	return 0;
}

int MikanSocket::Select( void )
{
	return 0;
}
