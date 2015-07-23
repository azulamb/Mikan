#include "MikanNetwork.h"

MikanNetworkIO::MikanNetworkIO( MikanSocket *ms )
{
  msocket = ms;
  Open( NULL );
}

// �J���B�����ł͈Ӗ��Ȃ��B
int MikanNetworkIO::Open( const char *io )
{
  loadsize = 0;
  return 0;
}

// �ǂݍ��ށBNULL�Ŏ��s�B
void * MikanNetworkIO::Read( unsigned int read_size = 0, int *readed_size = NULL )
{
  void *ret = (void *)msocket->Receive( read_size, NULL );
  loadsize += strlen( (char *)ret );
  return ret;
}

// �ǂݍ��ށB�̈�͊m�ۂ��Ȃ��B
int MikanNetworkIO::Read( void *data, unsigned int read_size, int *readed_size = NULL )
{
  int size = (void *)msocket->Receive( data, read_size, NULL );
  loadsize += size;
  return size;
}

// �V�[�N����B�����ł͈Ӗ��Ȃ��B
int MikanNetworkIO::Seek( int offset, int seekmode = SEEK_CUR )
{
  return 0;
}

// �������ށB-1�Ŏ��s�B�������񂾃T�C�Y��Ԃ��B
int MikanNetworkIO::Write( const void *write_data, int write_size = -1 )
{
  return msocket->Send( write_data, write_size );
}

// ���܂œǂ񂾃o�C�g����Ԃ��B-1�Ŏ��s�B
int MikanNetworkIO::Tell( void )
{
  return loadsize;
}

// �T�C�Y��Ԃ��B�����ł�Tell�B
int MikanNetworkIO::Size( void )
{
  return Tell();
}

// ����B
int MikanNetworkIO::Close( void )
{
  loadsize = 0;
  return msocket->Close();
}
