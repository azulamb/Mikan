#include "MikanNetwork.h"

extern CLASSDEF_NETWORK             *_MikanNet;

char MikanNetwork::base64table_enc[ 64 ] =
{
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

char MikanNetwork::base64table_dec[ 256 ] =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, 62, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, 65, 62, 63, // 65 -
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 63, -1, -1, -2, -1, -1,
	-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 65, // 65 _
	-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

char MikanNetwork::urlencodetable[ 256 ] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	6, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, //  !"#$%&'()*+,-./
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 1, 1, // 0123456789;:<=>?
	1, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, // @ABCDEFGHIJKLMNO
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, // PQRSTUVWXYZ[\]^_
	1, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, // `abcdefghijklmno
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, // pqrstuvwxyz{|}~
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

//スレッド実行用関数
unsigned long WINAPI MikanNetworkThreadFunc( void * param )
{
	int ret;
	struct P2PTHREADDATA *td = ( struct P2PTHREADDATA * )param;

	if ( td == NULL )
	{
		ExitThread( -1 );
	}

	if ( _MikanNet )
	{
		ret = _MikanNet->RunP2PNetwork( td );
	}

	td->handle = NULL;

	ExitThread( ret );
}

MikanNetwork::MikanNetwork( void )
{
	Init();
}

MikanNetwork::~MikanNetwork( void )
{
	Release();
}

int MikanNetwork::Init( void )
{
	socket_max = 16;
	p2p_max = 4;

	p2pthread = NULL;

	msocket = NULL;

	// fd_setの初期化。
	FD_ZERO( &readfds );

	return 0;
}

int MikanNetwork::SecondInit( void )
{
	unsigned int i;

	if ( p2p_max > 0 )
	{
		p2pthread = ( struct P2PTHREADDATA * )calloc( p2p_max, sizeof( struct P2PTHREADDATA ) );

		for ( i = 0; i < p2p_max; ++i )
		{
			p2pthread[ i ].handle = NULL;
			p2pthread[ i ].lockhandle = NULL;
			p2pthread[ i ].clientlist = NULL;
		}

		msocket = ( class MikanSocket ** )calloc( socket_max, sizeof( class MikanSocket * ) );
		for ( i = 0; i < socket_max; ++i )
		{
			msocket[ i ] = new MikanSocket();
		}
	}

	return 0;
}

int MikanNetwork::Release( void )
{
	unsigned int i;

	for ( i = 0; i < p2p_max; ++i )
	{
		StopP2PNetwork( i );
	}

	SAFE_FREE( p2pthread );

	for ( i = 0; i < socket_max; ++i )
	{
		delete( msocket[ i ] );
	}
	SAFE_FREE( msocket );

	return 0;
}

class MikanIO * MikanNetwork::GetMikanIO( unsigned int nnum )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return NULL;
	}

	return (MikanIO *)msocket[ nnum ];
}

class MikanSocket * MikanNetwork::GetMikanSocket( unsigned int nnum )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return NULL;
	}

	return msocket[ nnum ];
}

int MikanNetwork::CheckUDPAddressArea( unsigned int address )
{
	unsigned char *addr;

	addr = (unsigned char *)address;

	if ( 224 <= addr[ 0 ] && addr[ 0 ] <= 239 )
	{
		return MIKAN_NET_MULTICAST;
	}

	if ( addr[ 3 ] == 255 )
	{
		return MIKAN_NET_BROADCAST;
	}

	return MIKAN_NET_UDP;
}

// サーバーモードにする。一度だけ実行可能。
SOCKET MikanNetwork::CreateTCPServer( unsigned int nnum, unsigned short port, int connect )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	// すでにネットワークが立ち上がっている場合は、一度開放する。
	if ( msocket[ nnum ]->GetMode() != MIKAN_SOCKET_MODE_NONE )
	{
		msocket[ nnum ]->Release();
	}

	return msocket[ nnum ]->CreateServer( port, connect, MIKAN_SOCKET_TCP );
}

SOCKET MikanNetwork::CreateUDPServer( unsigned int nnum, unsigned short port, int connect )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	// すでにネットワークが立ち上がっている場合は、一度開放する。
	if ( msocket[ nnum ]->GetMode() != MIKAN_SOCKET_MODE_NONE )
	{
		msocket[ nnum ]->Release();
	}

	return msocket[ nnum ]->CreateServer( port, connect, MIKAN_SOCKET_UDP );
}

SOCKET MikanNetwork::CreateBloadcastServer( unsigned int nnum, unsigned short port, int connect )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	// すでにネットワークが立ち上がっている場合は、一度開放する。
	if ( msocket[ nnum ]->GetMode() != MIKAN_SOCKET_MODE_NONE )
	{
		msocket[ nnum ]->Release();
	}

	return msocket[ nnum ]->CreateBloadcastServer( port, connect );
}

SOCKET MikanNetwork::CreateMulticastServer( unsigned int nnum, const char *address, unsigned short port, int connect )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	// すでにネットワークが立ち上がっている場合は、一度開放する。
	if ( msocket[ nnum ]->GetMode() != MIKAN_SOCKET_MODE_NONE )
	{
		msocket[ nnum ]->Release();
	}

	return msocket[ nnum ]->CreateMulticastServer( address, port, connect );
}

SOCKET MikanNetwork::CreateMulticastServer( unsigned int nnum, unsigned long address, unsigned short port, int connect )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	// すでにネットワークが立ち上がっている場合は、一度開放する。
	if ( msocket[ nnum ]->GetMode() != MIKAN_SOCKET_MODE_NONE )
	{
		msocket[ nnum ]->Release();
	}

	return msocket[ nnum ]->CreateMulticastServer( address, port, connect );
}


// クライアントモードにする。
SOCKET MikanNetwork::CreateTCPClient( unsigned int nnum, const char *address, unsigned short port )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	// すでにネットワークが立ち上がっている場合は、一度開放する。
	if ( msocket[ nnum ]->GetMode() != MIKAN_SOCKET_MODE_NONE )
	{
		msocket[ nnum ]->Release();
	}

	return msocket[ nnum ]->CreateClient( address, port, MIKAN_SOCKET_TCP );
}

// クライアントモードにする。
SOCKET MikanNetwork::CreateUDPClient( unsigned int nnum, const char *address, unsigned short port )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	// すでにネットワークが立ち上がっている場合は、一度開放する。
	if ( msocket[ nnum ]->GetMode() != MIKAN_SOCKET_MODE_NONE )
	{
		msocket[ nnum ]->Release();
	}

	return msocket[ nnum ]->CreateUDPClient( address, port );
}

SOCKET MikanNetwork::CreateBloadcastClient( unsigned int nnum, const char *address, unsigned short port )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	// すでにネットワークが立ち上がっている場合は、一度開放する。
	if ( msocket[ nnum ]->GetMode() != MIKAN_SOCKET_MODE_NONE )
	{
		msocket[ nnum ]->Release();
	}

	return msocket[ nnum ]->CreateBloadcastClient( address, port );
}

SOCKET MikanNetwork::CreateMulticastClient( unsigned int nnum, const char *address, unsigned short port )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	// すでにネットワークが立ち上がっている場合は、一度開放する。
	if ( msocket[ nnum ]->GetMode() != MIKAN_SOCKET_MODE_NONE )
	{
		msocket[ nnum ]->Release();
	}

	return msocket[ nnum ]->CreateMulticastClient( address, port );
}

int MikanNetwork::Close( unsigned int nnum )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	return msocket[ nnum ]->Close();;
}

int MikanNetwork::Send( unsigned int nnum, const char *str )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	// ネットワークが立ち上がってない場合は、不正。
	if ( msocket[ nnum ]->GetMode() == MIKAN_SOCKET_MODE_NONE )
	{
		return INVALID_SOCKET;
	}

	return msocket[ nnum ]->Send( str );
}

int MikanNetwork::Send( unsigned int nnum, const char *str, unsigned size )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	// ネットワークが立ち上がってない場合は、不正。
	if ( msocket[ nnum ]->GetMode() == MIKAN_SOCKET_MODE_NONE )
	{
		return INVALID_SOCKET;
	}

	return msocket[ nnum ]->Send( str, size );
}

char * MikanNetwork::Receive( unsigned int nnum, int *getsize, struct sockaddr *from )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return NULL;
	}

	// ネットワークが立ち上がってない場合は、不正。
	if ( msocket[ nnum ]->GetMode() == MIKAN_SOCKET_MODE_NONE )
	{
		return NULL;
	}

	return msocket[ nnum ]->Receive( getsize, from );
}

int MikanNetwork::Receive( unsigned int nnum, char *buf, int size, struct sockaddr *from )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return -1;
	}

	// ネットワークが立ち上がってない場合は、不正。
	if ( msocket[ nnum ]->GetMode() == MIKAN_SOCKET_MODE_NONE )
	{
		return INVALID_SOCKET;
	}

	return msocket[ nnum ]->Receive( buf, size, from );
}

SOCKET MikanNetwork::Accept( unsigned int nnum, struct sockaddr_in *client )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return INVALID_SOCKET;
	}

	// ネットワークが立ち上がってない場合は、不正。
	if ( msocket[ nnum ]->GetMode() == MIKAN_SOCKET_MODE_NONE )
	{
		return INVALID_SOCKET;
	}

	return msocket[ nnum ]->Accept( client );
}

int MikanNetwork::RegisterSelect( unsigned int nnum )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return 1;
	}

	FD_SET( msocket[ nnum ]->GetSocket(), &readfds );

	return 0;
}

int MikanNetwork::Select( long timeout_sec, long timeout_usec )
{
	struct timeval tv;

	memcpy( &fds, &readfds, sizeof( struct fd_set ) );

	if ( timeout_sec != 0 || timeout_usec != 0 )
	{
		tv.tv_sec = timeout_sec;
		tv.tv_usec = timeout_usec;
		return select( 0, &fds, NULL, NULL, &tv );
	}

	return select( 0, &fds, NULL, NULL, NULL );
}

int MikanNetwork::IsSelected( unsigned int nnum )
{
	if ( CannotUseNetnum( nnum ) )
	{
		return 0;
	}
	return FD_ISSET( msocket[ nnum ]->GetSocket(), &fds );
}

// ネットワークサポート

unsigned long * MikanNetwork::GetNicAddrList( int *size )
{
	SOCKET sd;
	INTERFACE_INFO InterfaceList[ 20 ];
	unsigned long rsize, *ret = NULL, flag;
	int isize;
	int i;
	struct sockaddr_in *addr;

	*size = 0;

	sd = WSASocket( AF_INET, SOCK_DGRAM, 0, 0, 0, 0 );
	if ( sd == SOCKET_ERROR )
	{
		return ret;
	}

	if ( WSAIoctl( sd, SIO_GET_INTERFACE_LIST, 0, 0, &InterfaceList, sizeof( InterfaceList ), &rsize, 0, 0 ) == SOCKET_ERROR )
	{
		return ret;
	}

	isize = rsize / sizeof( INTERFACE_INFO );

	ret = (unsigned long *)calloc( isize, sizeof( unsigned long ) );
	if ( ret == NULL )
	{
		return ret;
	}

	for ( i = 0; i < isize; ++i )
	{
		addr = (sockaddr_in *)& ( InterfaceList[ i ].iiAddress );

		flag = InterfaceList[ i ].iiFlags;
		if ( flag & IFF_UP && !( flag & IFF_LOOPBACK ) )
		{
			// 起動済みかつループバックアドレスではない。
			ret[ ( *size )++ ] = addr->sin_addr.S_un.S_addr;
		}
	}

	return ret;
}

// HTTPサポート

int MikanNetwork::EncodeBase64( const char *src, char *dist, int size, const char *newline )
{
	int i = 0, w = 0, null = 0;
	char buf[ 3 ];
	int rsize, nsize;

	if ( size < 0 )
	{
		size = strlen( src );
	}

	nsize = strlen( newline );
	rsize = ( size / 3 + ( size % 3 != 0 ) ) * 4;
	if ( dist == NULL )
	{
		return rsize + ( rsize / 76 ) * nsize + 1;
	}

	--size;

	while ( w < size )
	{
		// buf埋め。
		if ( src[ i ] == '\0' )
		{
			break;
		} else if ( src[ i + 1 ] == '\0' )
		{
			buf[ 0 ] = src[ i ];
			buf[ 1 ] = buf[ 2 ] = 0;
			dist[ w++ ] = base64table_enc[ ( buf[ 0 ] & 0xFC ) >> 2 ];
			dist[ w++ ] = base64table_enc[ ( ( buf[ 0 ] & 0x3 ) << 4 ) + ( ( buf[ 1 ] & 0xF0 ) >> 4 ) ];
			dist[ w++ ] = '=';
			dist[ w++ ] = '=';
		} else if ( src[ i + 2 ] == '\0' )
		{
			buf[ 0 ] = src[ i ];
			buf[ 1 ] = src[ i + 1 ];
			buf[ 2 ] = 0;
			dist[ w++ ] = base64table_enc[ ( buf[ 0 ] & 0xFC ) >> 2 ];
			dist[ w++ ] = base64table_enc[ ( ( buf[ 0 ] & 0x3 ) << 4 ) + ( ( buf[ 1 ] & 0xF0 ) >> 4 ) ];
			dist[ w++ ] = base64table_enc[ ( ( buf[ 1 ] & 0xF ) << 2 ) + ( ( buf[ 2 ] & 0xC0 ) >> 6 ) ];
			dist[ w++ ] = '=';
		} else
		{
			buf[ 0 ] = src[ i ];
			buf[ 1 ] = src[ i + 1 ];
			buf[ 2 ] = src[ i + 2 ];
			dist[ w++ ] = base64table_enc[ ( buf[ 0 ] & 0xFC ) >> 2 ];
			dist[ w++ ] = base64table_enc[ ( ( buf[ 0 ] & 0x3 ) << 4 ) + ( ( buf[ 1 ] & 0xF0 ) >> 4 ) ];
			dist[ w++ ] = base64table_enc[ ( ( buf[ 1 ] & 0xF ) << 2 ) + ( ( buf[ 2 ] & 0xC0 ) >> 6 ) ];
			dist[ w++ ] = base64table_enc[ ( buf[ 2 ] & 0x3F ) ];
			if ( src[ i + 2 ] == '\0' )
			{
				break;
			}
		}
		i += 3;
	}
	return w;
}

int MikanNetwork::DecodeBase64( const char *src, char *dist, int size )
{
	int r = 0, bit, sz;
	//  char *ret;

	if ( src )
	{
		return NULL;
	}
	sz = strlen( src );
	//  ret = (char *)calloc( sz * 4 / 3 + 1, sizeof(char) );

	while ( src[ r ] != '\0' )
	{
		++r;
	}

	return 0;
}

int MikanNetwork::EncodeURL( const char *src, char *dist )
{
	int size, i, w;

	// とりあえず必要なバイト数を計算する。
	size = 1;
	//for( i = 0 ; i < size ; ++i )
	for ( i = 0; src[ i ] != '\0'; ++i )
	{
		switch ( urlencodetable[ src[ i ] ] )
		{
		case 1:
		case 3:
			// 変換対象のためバイト数は3。
			size += 3;
			break;
		default:
			// 変換無しのため追加バイト数は1。
			size += 1;
		}
	}

	if ( !dist )
	{
		return size;
	}

	w = 0;
	for ( i = 0; src[ i ] != '\0'; ++i )
	{
		if ( urlencodetable[ src[ i ] ] == 6 )
		{
			// スペースは+に変換。
			dist[ w++ ] = '+';
		} else if ( urlencodetable[ src[ i ] ] % 2 )
		{
			// 変換文字列の先頭は%。
			dist[ w++ ] = '%';
			// 続く2文字はバイトの16進表記2桁。
			sprintf_s( dist + w, size - w, "%02x", (unsigned char)src[ i ] );
			w += 2;
		} else
		{
			// 変換対象以外の文字列はそのまま書きこむ。
			dist[ w++ ] = src[ i ];
		}
	}

	dist[ w ] = '\0';

	// 領域確保したままなので、ちゃんと解放すること。
	return size;
}
int MikanNetwork::SplitHost( const char *src, char *dist, unsigned short *port )
{
	unsigned short portnum = 80;
	int r, l, w = 0;

	if ( src == NULL )
	{
		return NULL;
	}

	// http:// とか除去。
	for ( r = 0; src[ r ] != '\0'; ++r )
	{
		if ( src[ r ] == ':' )
		{
			if ( src[ r + 1 ] == '/' && src[ r + 2 ] == '/' )
			{
				r += 2;
			} else
			{
				r = 0;
			}
			break;
		}
		if ( src[ r ] == '/' || src[ r ] == '.' )
		{
			r = 0;
			break;
		}
	}

	l = r + 1;

	// http://～/までのドメインをコピー。
	// ポート番号が出てきたらそこでストップ。
	for ( ++r; src[ r ] != '\0'; ++r )
	{
		if ( src[ r ] == ':' || src[ r ] == '/' )
		{
			break;
		}
		++w;
		//addr[ w++ ] = src[ r ];
	}

	if ( dist )
	{
		strncpy_s( dist, w + 1, src + l, w );
		dist[ w ] = '\0';
	}

	if ( src[ r ] == ':' )
	{
		portnum = 0;
		while ( src[ ++r ] != '\0' )
		{
			if ( src[ r ] < '0' || '9' < src[ r ] )
			{
				break;
			}
			portnum = portnum * 10 + ( src[ r ] - '0' );
		}
	}

	if ( port )
	{
		*port = portnum;
	}

	return w + 1;
}

int MikanNetwork::SplitPath( const char *src, char *dist )
{
	int r, w = 0, tmp, getsplit;
	//char *addr;

	if ( src == NULL )
	{
		return NULL;
	}


	// http://～/ の/一歩手前まで進む。
	for ( r = 0; src[ r ] != '\0'; ++r )
	{
		if ( src[ r ] == '/' && src[ r + 1 ] == '/' )
		{
			r += 2;
		} else if ( src[ r ] == '/' )
		{
			break;
		}
	}

	// URLチェック。
	for ( tmp = r; src[ tmp ] != '\0'; ++tmp )
	{
		if ( src[ tmp ] == '?' )
		{
			break;
		}
	}
	if ( src[ tmp ] == '?' )
	{
		getsplit = tmp;
		while ( src[ ++tmp ] != '\0' )
		{
			if ( urlencodetable[ src[ tmp ] ] % 2 )
			{
				if ( !( urlencodetable[ src[ tmp ] ] == 3 &&
					urlencodetable[ src[ tmp + 1 ] ] == 2 &&
					urlencodetable[ src[ tmp + 2 ] ] == 2 )
					)
				{
					// URLエンコード済みじゃないらしい。
					tmp = -1;
					break;
				}
			}
		}
	}

	if ( tmp < 0 )
	{
		// URLエンコードが必要。

		// エンコード後のサイズを取得。
		w = EncodeURL( src + getsplit + 1, NULL );

		// データ部をエンコード後のサイズで確保。
		//addr = (char *)calloc( getsplit + w + 2, sizeof(char) );
		//if ( addr == NULL ){ return NULL; }
		if ( dist == NULL )
		{
			return getsplit + w + 2;
		}

		strncpy_s( dist, getsplit, src + r, getsplit );
		/*for ( w = 0 ; w <= getsplit ; ++w )
		{
		addr[ w ] = src[ r++ ];
		}*/

		// 確保した領域の後半部分にエンコード済みデータを入れる。
		EncodeURL( src + getsplit + 1, dist + getsplit );//addr + w );

		return getsplit + w + 2;
	}

	// URLエンコードが不要。

	//addr = (char *)calloc( strlen( src ) + 1, sizeof(char) );
	//if ( addr == NULL ){ return NULL; }
	if ( dist == NULL )
	{
		return strlen( src ) + 1;
	}

	/*while ( src[ r ] != '\0' )
	{
	addr[ w++ ] = src[ r++ ];
	}

	addr[ w ] = '\0';*/

	strncpy_s( dist, strlen( src + r ) + 1, src + r, strlen( src + r ) + 1 );

	return strlen( src + r ) + 1;
}

class MikanSocket * MikanNetwork::HttpConnect( const char *address, const char *proxy, const char *user, const char *password )
{
	char *addr, *path, *tmp;
	char buf[ 1024 ];
	int size, r;
	unsigned short port = 80;
	class MikanSocket *msock = NULL;

	// 単体で動かす。
	// http://x68000.q-e-d.net/~68user/net/http-1.html
	// プロキシサーバー、BASIC認証突破できたら嬉しい。
	// ダウンロードしたファイルサイズを返し、ファイルとして出力。
	if ( address == NULL )
	{
		return NULL;
	}

	// MikanSocketクラス生成。
	msock = new MikanSocket();
	if ( msock == NULL )
	{
		return NULL;
	}

	if ( proxy )
	{
		size = SplitHost( proxy, NULL, &port );
		addr = (char *)calloc( size, sizeof( char ) );
		SplitHost( proxy, addr, &port );
	} else
	{
		size = SplitHost( address, NULL, &port );
		addr = (char *)calloc( size, sizeof( char ) );
		SplitHost( address, addr, &port );
	}

	if ( addr == NULL )
	{
		return NULL;
	}

	// Web or Proxyサーバーへ接続。
	if ( msock->CreateClient( addr, port, MIKAN_SOCKET_TCP ) == INVALID_SOCKET )
	{
		free( addr );
		return NULL;
	}

	if ( proxy )
	{
		for ( r = 0; address[ r ] != '\0'; ++r )
		{
			if ( address[ r ] == '/' && address[ r + 1 ] != '/' )
			{
				break;
			}
		}
		size = SplitPath( address, NULL );
		path = (char *)calloc( size + r + 2, sizeof( char ) );
		if ( path == NULL )
		{
			free( addr );
			return NULL;
		}
		//GET http://127.0.0.1/index.html HTTP/1.0\n

		//Authorization: Basic aG9nZTpmdWdh
		//sprintf_s( buf, 1024, "%s:%s", user, password );
		//size = MikanNetwork::EncodeBase64( buf );
		//tmp = (char *)calloc( size + 22, sizeof( char ) );
		//strcpy_s( tmp, 22, "Authorization: Basic " );
		//MikanNetwork::EncodeBase64( buf, tmp + 21, size );
		//tmp[ size + 20 ] = '\n';
		//tmp[ size + 21 ] = '\0';

		//return msock;
	} else
	{
		// GET処理開始。
		size = SplitPath( address, NULL );
		path = (char *)calloc( size, sizeof( char ) );
		if ( path == NULL )
		{
			free( addr );
			return NULL;
		}
		SplitPath( address, path );

		if ( path == NULL )
		{
			return NULL;
		}
		// [GET ]+path+[ HTTP/1.0][\n]
		size = strlen( path );
		tmp = (char *)calloc( size + 15, sizeof( char ) );
		if ( tmp == NULL )
		{
			free( addr );
			return NULL;
		}
		sprintf_s( tmp, size + 15, "GET %s HTTP/1.0\n", path );

		// GET
		msock->Send( tmp, size + 14 );

		// Host処理開始。
		r = size + 15;
		size = strlen( addr ) + 14;
		if ( r < size )
		{
			free( tmp );
			tmp = (char *)calloc( size, sizeof( char ) );
		}

		// [Host: ]+addr+[:xxxxx][\n]
		if ( tmp )
		{
			sprintf_s( tmp, size, "Host: %s:%d\n", addr, port );

			// Host:
			msock->Send( tmp, strlen( tmp ) );

			free( tmp );
		}

	}

	if ( user && password )
	{
		//Authorization: Basic aG9nZTpmdWdh
		sprintf_s( buf, 1024, "%s:%s", user, password );
		size = MikanNetwork::EncodeBase64( buf );
		tmp = (char *)calloc( size + 22, sizeof( char ) );
		strcpy_s( tmp, 22, "Authorization: Basic " );
		MikanNetwork::EncodeBase64( buf, tmp + 21, size );
		tmp[ size + 20 ] = '\n';
		tmp[ size + 21 ] = '\0';

		msock->Send( tmp, size + 21 );
		free( tmp );
	}

	// User-Agent処理開始。
	size = 30;
	tmp = (char *)calloc( size, sizeof( char ) );
	if ( tmp )
	{
		sprintf_s( tmp, size, "User-Agent: MikanNetwork/1.1\n" );
		//User-Agent:
		msock->Send( tmp, size - 1 );

		free( tmp );
	}

	free( addr );
	free( path );
	return msock;
}

int MikanNetwork::HttpGetReturnCode( class MikanSocket *msock )
{
	char buf[ 2 ];
	int retcode = 0;

	// ステータスコードを読み取る。
	while ( msock->Receive( buf, 1 ) > 0 )
	{
		if ( buf[ 0 ] == ' ' )
		{
			break;
		}
	}
	while ( msock->Receive( buf, 1 ) > 0 )
	{
		if ( buf[ 0 ] < '0' || '9' < buf[ 0 ] )
		{
			break;
		}
		retcode += ( buf[ 0 ] - '0' );
	}
	return retcode;
}

int MikanNetwork::HttpAnalysisHeader( class MikanSocket *msock, int *contentlength, char **location )
{
	char buf[ 1024 ], *tmp;
	int r, size;

	if ( contentlength )
	{
		*contentlength = -1;
	}

	buf[ 1 ] = buf[ 2 ] = buf[ 3 ] = buf[ 4 ] = '\0';
	r = 4;
	while ( msock->Receive( buf, 1 ) > 0 )
	{
		if ( buf[ 0 ] == '\n' && buf[ 1 ] == '\n' )
		{
			break;
		}
		if ( buf[ 0 ] == '\n' && buf[ 1 ] == '\r' && buf[ 2 ] == '\n' && buf[ 3 ] == '\r' )
		{
			break;
		}
		if ( buf[ 0 ] == ':' )
		{
			tmp = buf + 4;
			if ( strncmp( tmp, "Content-length", 14 ) == 0 ||
				strncmp( tmp, "Content-Length", 14 ) == 0 )
			{
				// ファイルサイズゲット
				if ( contentlength )
				{
					*contentlength = 0;
					while ( msock->Receive( buf, 1 ) > 0 )
					{
						if ( buf[ 0 ] == '\r' || buf[ 0 ] == '\n' )
						{
							break;
						}
						if ( '0' <= buf[ 0 ] && buf[ 0 ] <= '9' )
						{
							*contentlength = ( *contentlength ) * 10 + ( buf[ 0 ] - '0' );
						}
					}
				}
			} else if ( strncmp( tmp, "Location", 8 ) == 0 ||
				strncmp( tmp, "location", 8 ) == 0 )
			{
				// 正式なアドレスゲット。
				if ( location == NULL )
				{
					while ( msock->Receive( buf, 1 ) > 0 )
					{
						if ( buf[ 0 ] == '\r' || buf[ 0 ] == '\n' )
						{
							break;
						}
					}
				} else
				{
					size = 256;
					*location = (char *)calloc( size, sizeof( char ) );
					r = 0;
					while ( msock->Receive( buf, 1 ) > 0 )
					{
						if ( buf[ 0 ] == '\r' || buf[ 0 ] == '\n' )
						{
							break;
						}
						if ( 'a' <= buf[ 0 ] && buf[ 0 ] <= 'z' )
						{
							// begin http://...
							*location[ 0 ] = buf[ 0 ];
							r = 1;
							break;
						}
					}
				}
				if ( r )
				{
					while ( msock->Receive( buf, 1 ) > 0 )
					{
						if ( buf[ 0 ] == '\r' || buf[ 0 ] == '\n' )
						{
							break;
						}
						if ( size <= r + 1 )
						{
							size += 256;
							*location = (char *)realloc( *location, size );
						}
						*location[ r++ ] = buf[ 0 ];
					}
					*location[ r ] = '\0';
				}


			} else
			{
				while ( msock->Receive( buf, 1 ) > 0 )
				{
					if ( buf[ 0 ] == '\r' || buf[ 0 ] == '\n' )
					{
						break;
					}
				}
			}
			r = 4;
			buf[ 4 ] = '\0';
		} else if ( 33 <= buf[ 0 ] && buf[ 0 ] <= 126 )
		{
			buf[ r++ ] = buf[ 0 ];
			buf[ r ] = '\0';
		}
		buf[ 3 ] = buf[ 2 ];
		buf[ 2 ] = buf[ 1 ];
		buf[ 1 ] = buf[ 0 ];

	}
	return 0;
}

int MikanNetwork::HttpReceiveBody( class MikanSocket *msock, const char *filepath, const char *address, int *loadsize )
{
	FILE *fp;
	char *tmp, buf[ 1024 ];
	int size, r, sp;

	// ファイル名用意
	if ( filepath == NULL )
	{
		sp = 0;
		size = strlen( address );
		for ( r = 0; r < size; ++r )
		{
			if ( address[ r ] == '/' )
			{
				sp = r;
			}
		}
		++sp;
		for ( r = size - 1; sp < r; --r )
		{
			if ( address[ r ] == '?' )
			{
				size = r;
			}
		}
		if ( size > sp )
		{
			tmp = (char *)calloc( size - sp + 1, sizeof( char ) );
			if ( tmp == NULL )
			{
				return -1;
			}
			strncpy_s( tmp, size - sp + 1, address + sp, size - sp );
		} else
		{
			tmp = (char *)calloc( strlen( address ) - sp + 1, sizeof( char ) );
			if ( tmp == NULL )
			{
				return -1;
			}
			strcpy_s( tmp, strlen( address ) - sp + 1, address + sp );
		}
	} else
	{
		tmp = (char *)calloc( strlen( filepath ) + 1, sizeof( char ) );
		if ( tmp == NULL )
		{
			return -1;
		}
		strcpy_s( tmp, strlen( filepath ) + 1, filepath );
	}

	// ボディ受け取り
	fopen_s( &fp, tmp, "wb" );
	fclose( fp );

	while ( ( size = msock->Receive( buf, 1024 ) ) > 0 )
	{
		fopen_s( &fp, tmp, "ab" );
		fwrite( buf, size, 1, fp );
		fclose( fp );
		if ( loadsize )
		{
			*loadsize += size;
		}
	}
	free( tmp );
	return 0;
}

int MikanNetwork::HttpGet( const char *address, const char *filepath, const char *proxy, const char *user, const char *password, int *getsize, int *loadsize )
{
	char *location;
	class MikanSocket *msock = NULL;
	int retcode = 0;

	msock = HttpConnect( address, proxy, user, password );

	if ( msock == NULL )
	{
		return -1;
	}

	//\n
	msock->Send( "\n", 1 );

	// ヘッダ受け取り

	// ステータスコードを読み取る。
	retcode = HttpGetReturnCode( msock );
	// 301なら本当はここで繋ぎ直したい。

	HttpAnalysisHeader( msock, getsize, &location );

	// データ受け取り。
	HttpReceiveBody( msock, filepath, address, loadsize );

	msock->Close();
	delete( msock );

	return 0;
}

int MikanNetwork::HttpPost( const char *address, const char *data, int datasize, const char *filepath, const char *proxy, const char *user, const char *password, int *getsize, int *loadsize )
{
	char *location;
	char buf[ 1024 ];
	class MikanSocket *msock = NULL;
	int retcode = 0;

	msock = HttpConnect( address, proxy, user, password );

	if ( msock == NULL )
	{
		return -1;
	}

	// データサイズ
	if ( datasize < 0 )
	{
		datasize = strlen( data );
	}
	sprintf_s( buf, 1024, "Content-Length: %d\n", datasize );
	msock->Send( buf, strlen( buf ) );

	//\n\n
	msock->Send( "\n", 1 );

	// データ送信。
	msock->Send( data );

	// ヘッダ受け取り

	// ステータスコードを読み取る。
	retcode = HttpGetReturnCode( msock );
	// 301なら本当はここで繋ぎ直したい。

	HttpAnalysisHeader( msock, getsize, &location );

	// データ受け取り。
	HttpReceiveBody( msock, filepath, address, loadsize );

	msock->Close();
	delete( msock );

	return 0;
}

int MikanNetwork::CreateP2PNetwork( unsigned int p2pnum, const char *addr, unsigned short port, int type, int *func( char *msg, int size, struct sockaddr_in *addr ), unsigned long timeout )
{
	if ( p2pnum >= p2p_max || p2pthread == NULL )
	{
		return 1;
	}

	if ( p2pthread[ p2pnum ].handle != NULL )
	{
		StopP2PNetwork( p2pnum );
	}

	p2pthread[ p2pnum ].connect = 5;
	p2pthread[ p2pnum ].port = htons( port );
	p2pthread[ p2pnum ].addr = MikanSocket::AddressToULong( addr, MIKAN_SOCKET_UDP );
	p2pthread[ p2pnum ].func = func;
	p2pthread[ p2pnum ].p2ptimeout = timeout;

	if ( type == MIKAN_NET_AUTO )
	{
		switch ( CheckUDPAddressArea( p2pthread[ p2pnum ].addr ) )
		{
		case MIKAN_NET_MULTICAST:
			p2pthread[ p2pnum ].type = MIKAN_NET_MULTICAST;
			break;
		default:
			p2pthread[ p2pnum ].type = MIKAN_NET_BROADCAST;
			break;
		}

	}

	p2pthread[ p2pnum ].client_max = 0;

	// ロックの作成。
	// クライアントリストを作る時に使う。

	if ( p2pthread[ p2pnum ].lockhandle )
	{
		CloseHandle( p2pthread[ p2pnum ].lockhandle );
		p2pthread[ p2pnum ].lockhandle = NULL;
	} else
	{
		sprintf_s( p2pthread[ p2pnum ].locknames, "MIKANMUTEX_N%d", p2pnum );
	}
	p2pthread[ p2pnum ].lockhandle = CreateMutex( 0, FALSE, p2pthread[ p2pnum ].locknames );

	p2pthread[ p2pnum ].handle = CreateThread( NULL, 0, MikanNetworkThreadFunc, (void *)&( p2pthread[ p2pnum ] ), 0, &( p2pthread[ p2pnum ].threadid ) );

	return 0;
}

int MikanNetwork::StopP2PNetwork( unsigned int p2pnum )
{
	unsigned int i, max;
	if ( p2pnum >= p2p_max || p2pthread == NULL || p2pthread[ p2pnum ].handle == NULL )
	{
		return 1;
	}

	GetP2PClientList( p2pnum, &max );

	for ( i = 0; i < max; ++i )
	{
		ClearP2PClientList( i );
	}

	CloseHandle( p2pthread[ p2pnum ].lockhandle );
	p2pthread[ p2pnum ].lockhandle = NULL;

	TerminateThread( p2pthread[ p2pnum ].handle, 1 );
	p2pthread[ p2pnum ].handle = NULL;

	return 0;
}

int MikanNetwork::RunP2PNetwork( struct P2PTHREADDATA *td )
{
	int ret;
	//  struct MIKANCLIENT *arg;

	switch ( td->type )
	{
	case MIKAN_NET_BROADCAST:
		break;
	case MIKAN_NET_MULTICAST:
		break;
	default:
		return 1;
	}

	td->server = new MikanSocket();

	if ( msocket == NULL )
	{
		return 2;
	}

	if ( td->type == MIKAN_NET_BROADCAST )
	{
		ret = td->server->CreateBloadcastServer( td->port, td->connect );
	} else
	{
		ret = td->server->CreateMulticastServer( td->addr, td->port, td->connect );
	}

	if ( ret != 0 )
	{
		goto out;
	}

	do
	{
		// クライアント取得処理。

		if ( td->func )
		{
			ret = ( ( int( *)( char *msg, int size, struct sockaddr_in *addr ) )( td->func ) )( NULL, 0, NULL );
		}
	} while ( ret == 0 );

out:
	delete( msocket );

	return ret;
}

struct MIKANCLIENT * MikanNetwork::GetP2PClientList( unsigned int p2pnum, unsigned int *max )
{
	int i;
	struct MIKANCLIENT *ret, *read;

	if ( p2pnum >= p2p_max || p2pthread == NULL )
	{
		return NULL;
	}

	if ( max )
	{
		// クライアントの数を返す。
		*max = p2pthread[ p2pnum ].client_max;
	}

	if ( p2pthread[ p2pnum ].client_max <= 0 )
	{
		return NULL;
	}

	LockP2PClientList( p2pnum );

	ret = ( struct MIKANCLIENT * )calloc( p2pthread[ p2pnum ].client_max, sizeof( struct MIKANCLIENT ) );
	read = p2pthread[ p2pnum ].clientlist;

	for ( i = 0; i < p2pthread[ p2pnum ].client_max && read; ++i )
	{
		ret[ i ].ipaddr = read->ipaddr;
		ret[ i ].lastlogin = read->lastlogin;
		ret[ i ].next = read->next;
		read = read->next;
	}

	UnLockP2PClientList( p2pnum );

	return p2pthread->clientlist;
}

struct MIKANCLIENT * MikanNetwork::AddClientList( unsigned int p2pnum, unsigned long address )
{
	struct MIKANCLIENT *ret = NULL, *next = NULL, *prev = NULL;
	unsigned long now;

	if ( p2pnum >= p2p_max || p2pthread == NULL )
	{
		return NULL;
	}


	LockP2PClientList( p2pnum );

	now = timeGetTime();

	if ( p2pthread[ p2pnum ].clientlist )
	{
		next = p2pthread[ p2pnum ].clientlist;
		while ( next )
		{
			if ( next->ipaddr == address )
			{
				ret = next;
			} else if ( now - next->lastlogin > p2pthread[ p2pnum ].p2ptimeout )
			{
				// タイムアウトにより、クライアントの削除を行う。
				if ( prev )
				{
					// 前の次は自分の次。
					prev->next = next->next;

					// 領域解放。
					free( next );

					// 次に進む。prevに変更はなし。
					next = prev->next;
				} else
				{
					// 次に進む。
					next = next->next;
					// 初めを削除する。
					free( p2pthread[ p2pnum ].clientlist );
					p2pthread[ p2pnum ].clientlist = NULL;
				}
				--p2pthread[ p2pnum ].client_max;
			} else
			{
				prev = next;
				next = next->next;
			}
		}

	}

	// 新規クライアント追加。
	if ( ret == NULL )
	{
		ret = ( struct MIKANCLIENT * )calloc( 1, sizeof( struct MIKANCLIENT ) );
		ret->ipaddr = address;

		if ( prev )
		{
			// 最後の次は新しいクライアント。
			prev->next = ret;
		}

		++p2pthread[ p2pnum ].client_max;
	}

	// 今回追加した人の時間を更新。
	ret->lastlogin = timeGetTime();

	// 初めての追加だった場合は特殊処理。
	if ( p2pthread[ p2pnum ].clientlist == NULL )
	{
		p2pthread[ p2pnum ].clientlist = ret;
	}

	UnLockP2PClientList( p2pnum );

	return ret;
}

int MikanNetwork::ClearP2PClientList( unsigned int p2pnum )
{
	struct MIKANCLIENT *next, *del;

	if ( p2pnum >= p2p_max || p2pthread == NULL )
	{
		return 1;
	}

	LockP2PClientList( p2pnum );

	next = p2pthread[ p2pnum ].clientlist;

	while ( next )
	{
		del = next;
		next = del->next;
		free( del );
	}

	UnLockP2PClientList( p2pnum );

	return 0;
}

int MikanNetwork::LockP2PClientList( unsigned int p2pnum, unsigned long timeout )
{
	if ( p2pnum >= p2p_max || p2pthread == NULL || p2pthread[ p2pnum ].lockhandle == NULL )
	{
		return 1;
	}
	return WaitForSingleObjectEx( p2pthread[ p2pnum ].lockhandle, timeout, false );
}

int MikanNetwork::UnLockP2PClientList( unsigned int p2pnum )
{
	if ( p2pnum >= p2p_max || p2pthread == NULL || p2pthread[ p2pnum ].lockhandle == NULL )
	{
		return 1;
	}
	return ReleaseMutex( p2pthread[ p2pnum ].lockhandle );
}

