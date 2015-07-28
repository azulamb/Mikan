#include <string>
#include <windows.h>
#include <dsound.h>

#include "debug.h"

#include "IFile.h"
#include "IAudioHandler.h"
#include "WaveHandler.h"

namespace Thaleia
{
	//--------------------------------------------------
	bool WaveHandler::bind( std::unique_ptr<IFile>&& file )
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

		//�����o�ϐ����X�V
		if ( !openWave( file ) )
			return false;

		//�t�@�C���̏��L�����擾
		m_file = std::move( file );

		//����
		return true;
	}

	std::unique_ptr<IFile> WaveHandler::unbind()
	{
		//�����o�ϐ�������
		m_datachunk_offset = 0;
		m_datachunk_size = 0;
		ZeroMemory( &m_format, WAVEFORMATEX_SIZE );

		//�t�@�C���̏��L����ԋp
		return std::move( m_file );
	}

	unsigned long WaveHandler::read( void* buffer, unsigned long length )
	{
		//�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_file == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return 0;
		}

		//�f�[�^�`�����N�̑O�ɂ���Ȃ�f�[�^�`�����N�擪�ɃV�[�N
		if ( m_file->tell() < m_datachunk_offset )
			m_file->seek( m_datachunk_offset );

		//�f�[�^�`�����N�̌�ɂ���Ȃ�f�[�^�`�����N�����ɃV�[�N
		if ( m_file->tell() > m_datachunk_offset + m_datachunk_size )
			m_file->seek( m_datachunk_offset + m_datachunk_size );

		//PCM�ǂݍ��݈ʒu�擾
		unsigned long offset = m_file->tell() - m_datachunk_offset;

		//�f�[�^�`�����N�����ɒB����Ƃ��̓ǂݍ��ݗʒ���
		if ( offset + length > m_datachunk_size )
			length = m_datachunk_size - offset;

		//�ǂݍ��ݗʂ̒[�����ۂ߂�
		//length -= length % m_waveformat.nBlockAlign;

		//�ǂݍ���
		return m_file->read( buffer, length );
	}

	unsigned long WaveHandler::tell() const
	{
		//�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_file == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return 0;
		}

		//�f�[�^�`�����N�̑O�ɂ���
		if ( m_file->tell() < m_datachunk_offset )
			return 0;

		//�f�[�^�`�����N�̌�ɂ���
		if ( m_file->tell() > m_datachunk_offset + m_datachunk_size )
			return m_datachunk_size;

		//PCM�f�[�^�J�n�n�_����̈ʒu��Ԃ�
		unsigned long offset = m_file->tell() - m_datachunk_offset;
		return offset;
	}

	void WaveHandler::seek( int offset, bool relatively )
	{
		//�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_file == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return;
		}

		//��Έʒu���擾
		unsigned long absolute_offset = relatively ? tell() + offset : offset;

		//�u���b�N�T�C�Y�ɃA���C��
		absolute_offset -= absolute_offset % m_format.nBlockAlign;

		//PCM�f�[�^�J�n�n�_����̈ʒu
		unsigned long raw_offset = m_datachunk_offset + absolute_offset;

		//�͈͊O�`�F�b�N
		if ( raw_offset > m_datachunk_offset + m_datachunk_size )
			raw_offset = m_datachunk_offset + m_datachunk_size;
		else if ( raw_offset < m_datachunk_offset )
			raw_offset = m_datachunk_offset;

		//�ړ������s
		m_file->seek( raw_offset );
	}

	unsigned long WaveHandler::size() const
	{
		//�t�@�C���ƃo�C���h����Ă��Ȃ���΃G���[
		if ( m_file == nullptr )
		{
			LOG( "This instance is not binded yet." );
			return 0;
		}

		//�f�[�^�`�����N�̑傫����Ԃ�
		return m_datachunk_size;
	}

	size_t WaveHandler::getWaveFormatEx( void* format, size_t size ) const
	{
		//�g���T�C�Y���܂߂��R�s�[����T�C�Y�𓾂�
		size_t copy_size = sizeof( m_format ) + m_format.cbSize;

		//�R�s�[����T�C�Y�̒���
		if ( copy_size > size )
			copy_size = size;
		if ( copy_size > WAVEFORMATEX_SIZE )
			copy_size = WAVEFORMATEX_SIZE;

		//�R�s�[�̎��s
		memcpy( format, &m_format, copy_size );

		//�R�s�[�����T�C�Y��Ԃ�
		return copy_size;
	}

	int WaveHandler::getTagLength() const
	{
		LOG( "Not implemented." );
		return 0;
	}

	std::string WaveHandler::getTag( int index ) const
	{
		LOG( "Not implemented." );
		return std::string();
	}

	//--------------------------------------------------
	bool WaveHandler::openWave( const std::unique_ptr<IFile>& file )
	{
		file->seek( 0 );

		//four-cc��������.
		char fourcc[ 5 ];
		fourcc[ 4 ] = 0; //�[���I�[������Ƃ���.

		//four-cc��RIFF�łȂ���΃G���[
		file->read( (void*)fourcc, 4 );
		if ( strcmp( fourcc, "RIFF" ) != 0 )
		{
			LOG( "File is not a RIFF format." );
			return false;
		}

		//RIFF�T�C�Y���擾
		DWORD riff_size;
		file->read( (void*)&riff_size, 4 );

		//four-cc��WAVE�łȂ���΃G���[
		file->read( (void*)fourcc, 4 );
		if ( strcmp( fourcc, "WAVE" ) != 0 )
		{
			LOG( "File is not a WAVE format." );
			return false;
		}

		//�t�H�[�}�b�g�`�����N����
		DWORD fmt_size; //�t�H�[�}�b�g�`�����N�̑傫��
		{
			DWORD chunk_size;
			while ( true )
			{
				//four-cc���擾�擾
				if ( !file->read( (void*)fourcc, 4 ) )
				{
					LOG( "File structure seems to be broken." );
					return false;
				}

				//�`�����N�T�C�Y���擾
				if ( !file->read( (void*)&chunk_size, 4 ) )
				{
					LOG( "File structure seems to be broken." );
					return false;
				}

				//�t�H�[�}�b�g�`�����N�Ȃ��break
				if ( strcmp( fourcc, "fmt " ) == 0 )
					break;

				//�t�H�[�}�b�g�`�����N�łȂ���Γǂݔ�΂�
				if ( !file->seek( chunk_size, true ) )
					return false;
			}

			//�t�H�[�}�b�g�`�����N�̑傫�����擾
			fmt_size = chunk_size;
		}

		//�t�H�[�}�b�g�擾
		if ( fmt_size > WAVEFORMATEX_SIZE )
		{
			LOG( "Format data is too large." );
			return false;
		}
		file->read( (void*)&m_format, fmt_size );

		//WAVE�`�����N�̐擪�Ɉړ�
		if ( !file->seek( 12 ) )
		{
			LOG( "File structure seems to be broken." );
			return false;
		}

		//�f�[�^�`�����N����
		{
			DWORD chunk_size;
			while ( true )
			{
				//four-cc���擾
				if ( !file->read( (void*)fourcc, 4 ) )
				{
					LOG( "File structure seems to be broken." );
					return false;
				}

				//�`�����N�T�C�Y���擾
				if ( !file->read( (void*)&chunk_size, 4 ) )
				{
					LOG( "File structure seems to be broken." );
					return false;
				}

				//�f�[�^�`�����N�Ȃ��break
				if ( strcmp( fourcc, "data" ) == 0 )
					break;

				//�f�[�^�`�����N�łȂ���Γǂݔ�΂�
				if ( !file->seek( chunk_size, true ) )
				{
					LOG( "File structure seems to be broken." );
					return false;
				}
			}

			//�f�[�^�`�����N�̑傫�����擾
			m_datachunk_size = chunk_size;
		}

		//�f�[�^�I�t�Z�b�g���擾
		m_datachunk_offset = file->tell();

		return true;
	}


};