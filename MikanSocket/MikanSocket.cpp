/*!
@class   Socket Socket.h "Socket.h"
@brief   Socket�ʐM���s���N���X
@author  Hiroki
@version 0.8
*/

#include "MikanSocket.h"

MikanSocket::MikanSocket( int major_version, int minor_version )
{
	// ���ʐݒ�B

	fileio = NULL;

	// �\�P�b�g�^�C�v�𖢐ݒ�ɂ���B
	mode = MIKAN_SOCKET_MODE_NONE;

	// �\�P�b�g�𖳌����B
	sock = ssock = INVALID_SOCKET;

	// �|�[�g�ԍ���K���ɐݒ�B
	port = MIKAN_SOCKET_DEFAULT_PORT;

	// �ʐM��TCP�ɐݒ肷��B
	SetType();

	// �T�[�o�[�p�ݒ�B

	// �ő�ڑ����B
	connect_max = MIKAN_SOCKET_DEFAULT_CONNECT_MAX;

	// �����J�n�B
	wsaerr = WSAStartup( MAKEWORD( major_version, minor_version ), &wsadata );

	SetNonBlockingIO( 0 );

	switch ( wsaerr )
	{
	case 0:
		// �G���[�����B
		break;
	case WSASYSNOTREADY:
		// �l�b�g���[�N�ɐڑ��ł��Ȃ��B
		break;
	case WSAVERNOTSUPPORTED:
		// WinSock�̃o�[�W���������������B
		break;
	case WSAEINPROGRESS:
		// ������Ńu���b�N����Ă�B
		break;
	case WSAEPROCLIM:
		// Winsock�̍ő又���v���Z�X���ɒB���Ă��܂����B
		break;
	case WSAEFAULT:
		// wsadata�̃|�C���^�����������B
		break;
	}

}

MikanSocket::~MikanSocket( void )
{
	Release();
	WSACleanup();
}

// ���̑������Ȃ��\�P�b�g�̍쐬�B
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
		// �r���ł̕ύX�͕s�B
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

// Receive���m���u���b�L���O�ɂ���B
int MikanSocket::SetNonBlockingIO( int nonblocking )
{
	this->nonblocking = nonblocking;

	ioctlsocket( sock, FIONBIO, &( this->nonblocking ) );

	return 0;
}

// �T�[�o�[���[�h�ɂ���B��x�������s�\�B
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

	// �\�P�b�g�̐ݒ�B
	server.sin_port = this->port = htons( port );
	server.sin_family = AF_INET;//AF_INET6
	server.sin_addr.S_un.S_addr = INADDR_ANY;

	sock = ssock = CreateSocket( GetSocketType(), server.sin_family, 0 );

	if ( sock == INVALID_SOCKET )
	{
		return INVALID_SOCKET;
	}

	// bind����time_wait�ɂ��G���[����B
	setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof( int ) );

	if ( bind( sock, ( struct sockaddr * )&server, sizeof( struct sockaddr_in ) ) != 0 )
	{
		// bind�G���[�B
		return INVALID_SOCKET;
	}

	// TCP�T�[�o�[�̏ꍇ��listen����B
	if ( GetSocketType() == MIKAN_SOCKET_TCP && listen( sock, connect_max ) != 0 )
	{
		// listen�G���[�B
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

// �N���C�A���g���[�h�ɂ���B��x�������s�\�B
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
		// �ڑ���̐ݒ�B
		target_server.sin_port = htons( port );
		target_server.sin_family = AF_INET;
		target_server.sin_addr.S_un.S_addr = AddressToULong( address, MIKAN_SOCKET_TCP );


		sock = ssock = CreateSocket( type, target_server.sin_family, 0 );

		if ( sock == INVALID_SOCKET )
		{
			return INVALID_SOCKET;
		}

		// �T�[�o�ɐڑ��B
		if ( connect( sock, ( struct sockaddr * )&target_server, sizeof( struct sockaddr_in ) ) != 0 )
		{
			return INVALID_SOCKET;
		}
		break;
	case MIKAN_SOCKET_UDP:
		// �ڑ���̐ݒ�B
		addr.sin_port = htons( port );
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = AddressToULong( address, MIKAN_SOCKET_UDP );

		sock = ssock = CreateSocket( MIKAN_SOCKET_UDP, addr.sin_family, 0 );
		break;
	case MIKAN_SOCKET_BROADCAST:
		// �u���[�h�L���X�g�p�̐ݒ�B

		// �ڑ���̐ݒ�B
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
		// �}���`�L���X�g�p�̐ݒ�B
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

// �J���B���ʂȂ��B�ǂݍ��݃T�C�Y�̏������Ƃ����炢�B
int MikanSocket::Open( const char *io )
{
	loadsize = 0;
	if ( io == NULL )
	{
		return -1;
	}
	// FileIO�Ńt�@�C�����J���B
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

// �ǂݍ��ށBNULL�Ŏ��s�B
void * MikanSocket::Read( unsigned int read_size, int *readed_size )
{
	int loaded = 0;
	// �����I��Receive�B
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

// �ǂݍ��ށB�̈�͊m�ۂ��Ȃ��B
int MikanSocket::Read( void *data, unsigned int read_size, int *readed_size )
{
	int load;
	// �t�@�C���̌��ݒn����K�؂ɎZ�o�B
	if ( fileio && fileio->Tell() < Tell() )
	{
		// ���ݒn����Ȃ�t�@�C������ǂށB
		fileio->Read( data, read_size, &load );
		// �����͂����̃f�o�b�O�̂��߂ɁA�ŐV���ɕۂB�����Ȃ����ƁB
		if ( readed_size )
		{
			*readed_size = load;
		}

		// ����Ȃ��ꍇ��Receive���Ă݂�H
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
		// �����Ȃ�Receive�B
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

// �V�[�N����B
int MikanSocket::Seek( int offset, int seekmode ) const
{
	// �t�@�C���V�[�N�B
	if ( fileio )
	{
		fileio->Seek( offset, seekmode );
	}
	return 0;
}

// �������ށB-1�Ŏ��s�B�������񂾃T�C�Y��Ԃ��B
int MikanSocket::Write( const void *write_data, int write_size )
{
	if ( write_size < 0 )
	{
		return Send( (char *)write_data );
	}
	return Send( (char *)write_data, write_size );
}

// ���܂œǂ񂾃o�C�g����Ԃ��B-1�Ŏ��s�B
int MikanSocket::Tell( void ) const
{
	return loadsize;
}

// �T�C�Y��Ԃ��B�����ł�Tell�B
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

	// �\�P�b�g�𖳌����B
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
		// ���łɐݒ�ς݂̑���ɑ���B
		addrin.sin_family = addr.sin_family;
		addrin.sin_port = addr.sin_port;
		addrin.sin_addr.S_un.S_addr = addr.sin_addr.S_un.S_addr;

		if ( type == MIKAN_SOCKET_BROADCAST )
		{
			// �\�P�b�g�Ƀu���[�h�L���X�g�̑�����t����B
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
				// �m���u���b�L���O�Ȃ�A�f�[�^��S�Ď󂯎��܂Ńf�[�^���񂹂�B
				head += len;
				buf = (char *)realloc( buf, 1024 + head + 1 );
			}
		} else if ( GetSocketType() == MIKAN_SOCKET_UDP || from != NULL )
		{
			// UDP
			while ( ( len = recvfrom( sock, buf + head, 1024, 0, from, &fromsize ) ) && nonblocking )
			{
				// �m���u���b�L���O�Ȃ�A�f�[�^��S�Ď󂯎��܂Ńf�[�^���񂹂�B
				head += len;
				buf = (char *)realloc( buf, 1024 + head + 1 );
			}
		} else
		{
			// �G���[
		}

		if ( len < 1 )
		{
			if ( nonblocking && GetLastError() == WSAEWOULDBLOCK )
			{
				// �܂����Ȃ��B
			} else
			{
				//�G���[
			}
			len = 0;
		}
		len += head;
	}

	//�T�C�Y������int�^�|�C���^���w�肳��Ă���Ȃ�T�C�Y��������B
	if ( getsize )
	{
		*getsize = len;
	}
	loadsize += len;

	buf[ len ] = '\0';

	//�󂯎�����f�[�^�̕������Ԃ��B
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

	// ���[�h�𖢐ݒ�ɂ���B
	mode = MIKAN_SOCKET_MODE_NONE;

	return 0;
}

SOCKET MikanSocket::Accept( struct sockaddr_in *client )
{
	int len;

	if ( mode != MIKAN_SOCKET_MODE_SERVER )
	{
		// �T�[�o�[���[�h�ł͂Ȃ��̂ŕs���B
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
		// �T�[�o�[���[�h�ł͂Ȃ��̂ŕs���B
		return NULL;
	}

	if ( client != NULL )
	{
		// �R�s�[�B
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
