
#ifndef _MIKANDEVELOPER
#ifdef _MIKANDEV
#if _DEBUG
#pragma comment(lib, "Dev/libogg_staticD.lib")
#pragma comment(lib, "Dev/libvorbis_staticD.lib")
#pragma comment(lib, "Dev/libvorbisfile_staticD.lib")
#else
#pragma comment(lib, "Dev/libogg_static.lib")
#pragma comment(lib, "Dev/libvorbis_static.lib")
#pragma comment(lib, "Dev/libvorbisfile_static.lib")
#endif
#else
#if _DEBUG
#pragma comment(lib, "libogg_staticD.lib")
#pragma comment(lib, "libvorbis_staticD.lib")
#pragma comment(lib, "libvorbisfile_staticD.lib")
#else
#pragma comment(lib, "libogg_static.lib")
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libvorbisfile_static.lib")
#endif
#endif
#else
#if _DEBUG
#pragma comment(lib, "libogg_staticD.lib")
#pragma comment(lib, "libvorbis_staticD.lib")
#pragma comment(lib, "libvorbisfile_staticD.lib")
#else
#pragma comment(lib, "libogg_static.lib")
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libvorbisfile_static.lib")
#endif
#endif

#include <windows.h>
#include <tchar.h>
#include <dsound.h>

#include "OggHandler.h"
#include "debug.h"

namespace Thaleia
{

	//--------------------------------------------------

	// ogg-vorbis���C�u�����̃G���[�R�[�h����, �G���[���e�̕����񃊃e�������擾����.
	inline const TCHAR* errorCode( int code )
	{
		switch ( code )
		{
		case OV_FALSE:
			return _T( "OV_FALSE \"Not true, or no data available.\"" );

		case OV_HOLE:
			return _T( "OV_HOLE \"Vorbisfile encoutered missing or corrupt data in the bitstream. Recovery is normally automatic and this return code is for informational purposes only.\"" );

		case OV_EREAD:
			return _T( "OV_EREAD \"Read error while fetching compressed data for decode.\"" );

		case OV_EFAULT:
			return _T( "OV_EFAULT \"Internal inconsistency in decode state. Continuing is likely not possible.\"" );

		case OV_EIMPL:
			return _T( "OV_EIMPL \"Feature not implemented.\"" );

		case OV_EINVAL:
			return _T( "OV_EINVAL \"Either an invalid argument, or incompletely initialized argument passed to libvorbisfile call\"" );

		case OV_ENOTVORBIS:
			return _T( "OV_ENOTVORBIS \"The given file/data was not recognized as Ogg Vorbis data.\"" );

		case OV_EBADHEADER:
			return _T( "OV_EBADHEADER \"The file/data is apparently an Ogg Vorbis stream, but contains a corrupted or undecipherable header.\"" );

		case OV_EVERSION:
			return _T( "OV_EVERSION \"The bitstream format revision of the given stream is not supported.\"" );

		case OV_EBADLINK:
			return _T( "OV_EBADLINK \"The given link exists in the Vorbis data stream, but is not decipherable due to garbacge or corruption.\"" );

		case OV_ENOSEEK:
			return _T( "OV_ENOSEEK \"The given stream is not seekable.\"" );
		}
		return _T( "" );
	}

	//--------------------------------------------------
	bool OggHandler::bind( std::unique_ptr<IFile>&& file )
	{
		//���Ƀt�@�C�����o�C���h���Ă���Ȃ�΃G���[
		if ( m_file != nullptr )
		{
			LOG( "This instance is already binded." );
			return false;
		}

		//������NULL�Ȃ�΃G���[
		if ( file == nullptr )
		{
			LOG( "Argument 'file' is nullptr." );
			return false;
		}

		//�t�@�C���̏��L�����擾
		//�����ōX�V���Ă����Ȃ��ƃR�[���o�b�N�֐��Q�����������삵�Ȃ�.
		m_file = std::move( file );

		//OggVorbis�R�[���o�b�N�֐��Q�̎w��
		ov_callbacks callbacks =
		{
			StaticReadProc,
			StaticSeekProc,
			StaticCloseProc,
			StaticTellProc
		};

		//OggVorbis�C���X�^���X����
		m_oggvorbis = decltype( m_oggvorbis )(
			new OggVorbis_File,
			[]( OggVorbis_File* p )
		{
			ov_clear( p );
			delete p;
		}
		);

		//OggVorbis�t�@�C���I�[�v��
		int ret = ov_open_callbacks( this, m_oggvorbis.get(), nullptr, 0, callbacks );
		if ( ret < 0 )
		{
			LOG( "OggVorbis open error: %s", errorCode( ret ) );

			//�����ł�ov_clear���Ă΂���delete����K�v�����邽��
			//���O��delete����.
			delete m_oggvorbis.get();
			m_oggvorbis.release();

			//���s�I��
			return false;
		}

		//�ǂݍ��݈ʒu�������߂�.
		//�����߂��Ȃ���pcm_offset��-1���Z�b�g����Ă���.
		ov_pcm_seek( m_oggvorbis.get(), 0 );

		//����
		return true;
	}

	std::unique_ptr<IFile> OggHandler::unbind()
	{
		//OggVorbis���
		m_oggvorbis.reset();

		//�t�@�C���̏��L����ԋp
		return std::move( m_file );
	}

	unsigned long OggHandler::read( void* buffer, unsigned long length )
	{
		//OGG�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_oggvorbis == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return 0;
		}

		//�ǂݍ��݃T�C�Y�[���̓���P�[�X�͈ȍ~�l�����Ȃ�.
		if ( length == 0 )
		{
			return 0;
		}

		int bitstream = 0;
		long read_size = 0;
		unsigned long total_read_size = 0; //�ǂݍ��ݍς݃T�C�Y
		do
		{
			//ov_read�œǂݎ��ʂ�����
			int request_size;
			if ( length > total_read_size + INT_MAX ) //����length�΍�
				request_size = INT_MAX;
			else
				request_size = length - total_read_size;

			//ov_read�� ��5����word * 2 �̔{�����Ƃɂ����ǂݍ��߂Ȃ����Ƃɒ���
			read_size = ov_read( m_oggvorbis.get(), (char*)buffer + total_read_size, request_size, 0, 2, 1, &bitstream );

			//�G���[�����ƃG���[���b�Z�[�W�\��
			if ( read_size < 0 )
			{
				switch ( read_size )
				{
				case OV_HOLE:
					LOG( "OV_HOLE \"indicates there was an interruption in the data. \"" );
					break;

				case OV_EBADLINK:
					LOG( "OV_EBADLINK \"indicates that an invalid stream section was supplied to libvorbisfile, or the requested link is corrupt.\"" );
					break;

				case OV_EINVAL:
					LOG( "OV_EINVAL \"indicates the initial file headers couldn't be read or are corrupt, or that the initial open call for vf failed.\"" );
					break;
				}
				read_size = 0;
			}

			total_read_size += read_size;
		} while ( total_read_size < length && read_size > 0 );

		return total_read_size;
	}

	unsigned long OggHandler::tell() const
	{
		//OGG�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_oggvorbis == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return 0;
		}

		//OGG�t�@�C�������擾
		vorbis_info* info = ov_info( m_oggvorbis.get(), -1 );
		if ( info == nullptr )
		{
			LOG( "Getting infomation in OggVorbis failed." );
			return 0;
		}

		//�ǂݎ��ʒu���T���v���ԍ��Ƃ��Ď擾
		long long position = ov_pcm_tell( m_oggvorbis.get() );
		if ( position < 0 )
		{
			LOG( "Getting PCM cursor in OggVorbis failed." );
			return 0;
		}

		//
		return (unsigned long)position * ( info->channels * 2 );
	}

	void OggHandler::seek( int offset, bool relatively )
	{
		//OGG�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_oggvorbis == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return;
		}

		//OGG�t�@�C�������擾
		vorbis_info* info = ov_info( m_oggvorbis.get(), -1 );
		if ( info == nullptr )
		{
			LOG( "Getting infomation in OggVorbis failed." );
			return;
		}

		long long position;
		if ( relatively )
			position = tell() + offset;
		else
			position = offset;

		if ( position < 0 )
			position = 0;

		unsigned long byte_per_sample = info->channels * 2;

		ov_pcm_seek( m_oggvorbis.get(), (unsigned long)( position / byte_per_sample ) );
	}

	unsigned long OggHandler::size() const
	{
		//OGG�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_oggvorbis == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return 0;
		}

		//OGG�t�@�C�������擾
		vorbis_info* info = ov_info( m_oggvorbis.get(), -1 );
		if ( info == nullptr )
		{
			LOG( "Getting infomation in OggVorbis failed." );
			return 0;
		}

		long long size = ov_pcm_total( m_oggvorbis.get(), -1 ) * ( info->channels * 2 );
		return (unsigned long)size;
	}

	size_t OggHandler::getWaveFormatEx( void* format, size_t size ) const
	{
		//OGG�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_oggvorbis == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return 0;
		}

		//OGG�t�@�C�������擾
		vorbis_info* info = ov_info( m_oggvorbis.get(), -1 );
		if ( info == nullptr )
		{
			LOG( "Getting infomation in OggVorbis failed." );
			return 0;
		}

		//�������݃T�C�Y����
		size_t copy_size = sizeof( WAVEFORMATEX );
		if ( copy_size > size )
			copy_size = size;

		//�t�H�[�}�b�g�f�[�^�쐬
		WAVEFORMATEX waveformatex;
		waveformatex.wFormatTag = WAVE_FORMAT_PCM;
		waveformatex.nChannels = info->channels;
		waveformatex.nSamplesPerSec = info->rate;
		waveformatex.wBitsPerSample = 16;
		waveformatex.cbSize = 0;
		waveformatex.nBlockAlign = waveformatex.nChannels * 16 / 8;
		waveformatex.nAvgBytesPerSec = waveformatex.nSamplesPerSec * waveformatex.nBlockAlign;

		//�R�s�[
		memcpy( format, &waveformatex, copy_size );
		return copy_size;
	}

	int OggHandler::getTagLength() const
	{
		//OGG�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_oggvorbis == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return 0;
		}

		//OGG�t�@�C�������擾
		vorbis_comment* comment = ov_comment( m_oggvorbis.get(), -1 );
		if ( comment == nullptr )
		{
			LOG( "Getting comment in OggVorbis failed." );
			return 0;
		}

		return comment->comments;
	}

	std::string OggHandler::getTag( int index ) const
	{
		//OGG�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_oggvorbis == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return std::string();
		}

		//OGG�t�@�C�������擾
		vorbis_comment* comment = ov_comment( m_oggvorbis.get(), -1 );
		if ( comment == nullptr )
		{
			LOG( "Getting comment in OggVorbis failed." );
			return nullptr;
		}

		int length = comment->comments;
		if ( index < 0 || length <= index )
		{
			LOG( "Argument 'index' is out of range." );
			return nullptr;
		}

		return comment->user_comments[ index ];
	}

	//--------------------------------------------------
	size_t OggHandler::StaticReadProc( void* buffer, size_t size, size_t n, void* self )
	{
		//self��NULL���n����Ă���Ȃ�΃G���[
		if ( self == nullptr )
		{
			LOG( "Self pointer is NULL." );
			return 0;
		}

		size_t read_size = ( (OggHandler*)self )->ReadProc( buffer, size, n );
		return read_size;
	}

	int OggHandler::StaticSeekProc( void* self, ogg_int64_t offset, int flag )
	{
		//self��NULL���n����Ă���Ȃ�΃G���[
		if ( self == nullptr )
		{
			LOG( "Self pointer is NULL." );
			return OV_FALSE;
		}

		int result = ( (OggHandler*)self )->SeekProc( offset, flag );
		return result;
	}

	int OggHandler::StaticCloseProc( void* self )
	{
		return 0;
	}

	long OggHandler::StaticTellProc( void* self )
	{
		//self��NULL���n����Ă���Ȃ�΃G���[
		if ( self == nullptr )
		{
			LOG( "Self pointer is NULL." );
			return OV_FALSE;
		}

		long position = ( (OggHandler*)self )->TellProc();
		return position;
	}

	//--------------------------------------------------
	size_t OggHandler::ReadProc( void* buffer, size_t size, size_t n )
	{
		//OGG�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_file == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return 0;
		}

		//�ǂݍ��݈ʒu����t�@�C���I�[�܂ł̃T�C�Y���v�Z
		int remnant = m_file->size() - m_file->tell();

		//�ǂݍ��݃u���b�N�����v�Z
		size_t count = remnant / size;
		if ( count > n )
			count = n;

		//�ǂݍ���
		m_file->read( buffer, size * count );
		//m_file->read(buffer, size * n); /* 2013.01.05 : n�������������̂Œ���. ����������count���ƍŌ�܂œǂݍ��߂Ȃ��Ȃ�P�[�X�����݂���H */

		return count;
	}

	int OggHandler::SeekProc( ogg_int64_t offset, int flag )
	{
		//OGG�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_file == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return OV_FALSE;
		}

		switch ( flag )
		{
		case SEEK_CUR:
			m_file->seek( offset, true );
			break;

		case SEEK_END:
			m_file->seek( m_file->size() + offset );
			break;

		case SEEK_SET:
			m_file->seek( offset );
			break;

		default:
			return -1;
		}

		return 0;
	}

	long OggHandler::TellProc()
	{
		//OGG�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_file == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return OV_FALSE;
		}

		return m_file->tell();
	}

};