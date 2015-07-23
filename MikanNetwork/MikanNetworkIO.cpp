#include "MikanNetwork.h"

MikanNetworkIO::MikanNetworkIO( MikanSocket *ms )
{
  msocket = ms;
  Open( NULL );
}

// 開く。ここでは意味なし。
int MikanNetworkIO::Open( const char *io )
{
  loadsize = 0;
  return 0;
}

// 読み込む。NULLで失敗。
void * MikanNetworkIO::Read( unsigned int read_size = 0, int *readed_size = NULL )
{
  void *ret = (void *)msocket->Receive( read_size, NULL );
  loadsize += strlen( (char *)ret );
  return ret;
}

// 読み込む。領域は確保しない。
int MikanNetworkIO::Read( void *data, unsigned int read_size, int *readed_size = NULL )
{
  int size = (void *)msocket->Receive( data, read_size, NULL );
  loadsize += size;
  return size;
}

// シークする。ここでは意味なし。
int MikanNetworkIO::Seek( int offset, int seekmode = SEEK_CUR )
{
  return 0;
}

// 書き込む。-1で失敗。書き込んだサイズを返す。
int MikanNetworkIO::Write( const void *write_data, int write_size = -1 )
{
  return msocket->Send( write_data, write_size );
}

// 今まで読んだバイト数を返す。-1で失敗。
int MikanNetworkIO::Tell( void )
{
  return loadsize;
}

// サイズを返す。ここではTell。
int MikanNetworkIO::Size( void )
{
  return Tell();
}

// 閉じる。
int MikanNetworkIO::Close( void )
{
  loadsize = 0;
  return msocket->Close();
}
