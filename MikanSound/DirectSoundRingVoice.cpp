#include <process.h>
#include <windows.h>
#include <dsound.h>

#include "debug.h"

#include "IFile.h"
#include "IAudioHandler.h"
#include "IVoice.h"

#include "DirectSoundRingVoice.h"

namespace Thaleia
{
	//--------------------------------------------------
	bool DirectSoundRingVoice::bind( std::unique_ptr<IAudioHandler>&& pcm )
	{
		auto cs = threadLock();

		//�G���[�`�F�b�N
		if ( m_directsound == nullptr )
		{
			LOG( "Internal DirectSound pointer is nullptr." );
			return false;
		}
		if ( m_audio != nullptr )
		{
			LOG( "Already binded." );
			return false;
		}
		if ( pcm == nullptr )
		{
			LOG( "Argument 'pcm' is nullptr." );
			return false;
		}

		//WAVE�t�H�[�}�b�g�擾
		WAVEFORMATEX format;
		if ( pcm->getWaveFormatEx( &format, sizeof( format ) ) != sizeof( format ) )
		{
			LOG( "Cannot get WaveFormat." );
			return false;
		}

		//�o�b�t�@�T�C�Y����
		m_dsbuffer_size = DEFAULT_BUFFER_SIZE;
		if ( m_division < 1 )
			m_division = 2;
		m_dsbuffer_size -= m_dsbuffer_size % ( m_division * format.nBlockAlign );

		//�t�B���C���f�b�N�X�쐬
		m_fill_position = new unsigned long[ m_division ];
		for ( unsigned short i = 0; i < m_division; i++ )
			m_fill_position[ i ] = 0;

		//�o�b�t�@������
		{
			bool result = m_dsbuffer.initialize(
				m_directsound,
				m_dsbuffer_size,
				&format
				);
			if ( !result )
			{
				LOG( "DirectSoundBuffer initializing failed." );
				return false;
			}
		}

		//PCM�̏��L�����擾
		m_audio = std::move( pcm );

		return true;
	}

	std::unique_ptr<IAudioHandler> DirectSoundRingVoice::unbind()
	{
		auto cs = threadLock();

		//�X���b�h���
		m_dsbuffer.Stop();
		terminateThread( std::move( cs ) );

		//DirectSound�o�b�t�@���
		m_dsbuffer.release();

		//�t�B���C���f�b�N�X�폜
		if ( m_fill_position != nullptr )
			delete[] m_fill_position;
		m_fill_position = nullptr;

		//PCM�̏��L����ԋp
		return std::move( m_audio );
	}

	void DirectSoundRingVoice::play()
	{
		auto cs = threadLock();

		if ( m_thread_terminate )
		{
			LOG( "Waiting for termination of streaming thread." );
			return;
		}

		//play���Ȃ��~��, �ŏ��ɖ߂�
		if ( isPlaying() )
		{
			m_dsbuffer.Stop();
			terminateThread( std::move( cs ) );
			m_audio->seek( 0 );
		}

		if ( !threadAlive() ) //stop��
		{
			//�X���b�h�쐬
			if ( !threadRun() )
				LOG( "Creating streaming thread failed." );
		} else //pause��
			m_dsbuffer.Play( 0, 0, DSBPLAY_LOOPING );
	}

	void DirectSoundRingVoice::pause()
	{
		m_dsbuffer.Stop();
	}

	void DirectSoundRingVoice::stop()
	{
		auto cs = threadLock();

		//DirectSound�o�b�t�@�̍Đ���~
		m_dsbuffer.Stop();

		//�X���b�h��~�v���𔭍s����
		if ( threadAlive() )
			m_thread_terminate = true;

		//�����߂�
		if ( m_audio != nullptr )
			m_audio->seek( 0 );
	}

	unsigned long DirectSoundRingVoice::tell() const
	{
		return toMillisecond( tellByte() );
	}

	void DirectSoundRingVoice::seek( unsigned long millisecond )
	{
		auto cs = threadLock();

		if ( m_audio == nullptr )
		{
			LOG( "Not initialized." );
			return;
		}

		//�Đ����Ȃ�΋�����~
		bool playing = isPlaying();
		if ( threadAlive() ) //�X�g���[���X���b�h�����݂���ꍇ
		{
			if ( playing ) //play��
			{
				m_dsbuffer.Stop();
				terminateThread( std::move( cs ) );
			} else //pause��
			{
				//�X���b�h��~�v���𔭍s����
				m_thread_terminate = true;
			}
		}

		//�ړ���̌���
		unsigned long moveto = toByte( millisecond );
		if ( moveto >= m_audio->size() )
			moveto = m_audio->size();

		//�ړ�
		m_audio->seek( moveto );

		//���̍Đ���Ԃɍ��킹��
		if ( playing )
			play();
	}

	double DirectSoundRingVoice::getVolume() const
	{
		return m_dsbuffer.getVolume();
	}

	double DirectSoundRingVoice::getPan() const
	{
		return m_dsbuffer.getPan();
	}

	unsigned int DirectSoundRingVoice::getFrequency() const
	{
		return m_dsbuffer.getFrequency();
	}

	bool DirectSoundRingVoice::isLoopEnabled() const
	{
		return m_loop;
	}

	bool DirectSoundRingVoice::getLoopTime( unsigned long* start, unsigned long* end ) const
	{
		if ( start != nullptr )
			*start = m_loop_start;
		if ( end != nullptr )
			*end = m_loop_end;
		return true;
	}

	void DirectSoundRingVoice::setVolume( double volume )
	{
		m_dsbuffer.setVolume( volume );
	}

	void DirectSoundRingVoice::setPan( double pan )
	{
		m_dsbuffer.setPan( pan );
	}

	void DirectSoundRingVoice::setFrequency( unsigned int frequency )
	{
		m_dsbuffer.setFrequency( frequency );
	}

	bool DirectSoundRingVoice::enableLoop( bool enable )
	{
		m_loop = enable;
		return true;
	}

	bool DirectSoundRingVoice::setLoopTime( unsigned long start, unsigned long end )
	{
		m_loop_start = start;
		m_loop_end = end;
		return true;
	}

	bool DirectSoundRingVoice::isPlaying() const
	{
		unsigned long status;
		if ( m_dsbuffer.GetStatus( &status ) != DS_OK )
			return false;

		return status & DSBSTATUS_PLAYING;
	}

	//--------------------------------------------------
	void DirectSoundRingVoice::threadProcedure()
	{
		{
			auto cs = threadLock();

			//�G���[�`�F�b�N
			if ( m_audio == nullptr )
			{
				LOG( "Not initialized." );
				m_dsbuffer.Stop();
				m_thread_terminate = false;
				return;
			}

			unsigned long position = m_audio->tell();

			//�ŏ��̓ǂݍ���
			m_dsbuffer.SetCurrentPosition( 0 );
			fillBuffer( 0 );
			m_last_index = 0;

			//�Đ��J�n
			HRESULT result = m_dsbuffer.Play( 0, 0, DSBPLAY_LOOPING );
			if ( result != S_OK )
			{
				LOG( "DirectSound Play failed. HRESULT=%Xh", result );
				m_thread_terminate = false;
			}
		}

		//�o�b�t�@�Ď����[�v
		while ( true )
		{
			auto cs = threadLock();

			if ( m_thread_terminate ) //���[�v���������while�������ł͂Ȃ�lock�ɐ�����������ɍs��
				break;

			//�Đ������`�F�b�N
			if ( tellByte() > m_audio->size() )
			{
				m_dsbuffer.Stop();
				m_audio->seek( 0 );
				break;
			}

			//�u���b�N�ʒu���擾
			DWORD play_cursor, write_cursor;
			m_dsbuffer.GetCurrentPosition( &play_cursor, &write_cursor );
			unsigned long current_index = play_cursor / ( m_dsbuffer_size / m_division );
			unsigned long next_index = ( current_index + 1 ) % m_division;

			//���̃u���b�N�����ǂȂ�ǂݍ���
			if ( m_last_index != next_index )
			{
				fillBuffer( next_index );
				m_last_index = next_index;
			} else
			{
				cs.reset();
				Sleep( 100 );
			}
		}

		//�I������
		auto cs = threadLock();
		m_thread_terminate = false;
	}

	//--------------------------------------------------

	unsigned long DirectSoundRingVoice::read( void* buffer, unsigned long length )
	{
		//PcmHandler�m�F
		if ( m_audio == nullptr )
			return 0;

		//���[�v�ʒu�擾
		bool loop = false;
		unsigned long loop_start, loop_end;
		if ( m_loop )
		{
			if ( m_loop_start < m_loop_end )
			{
				loop_start = toByte( m_loop_start );
				loop_end = toByte( m_loop_end );
				loop = true;
			} else if ( m_loop_start <= m_loop_end )
			{
				loop_start = 0;
				loop_end = m_audio->size();
				loop = true;
			}
		}

		//���[�v�s�v�܂��̓��[�v�͈͂𔲂��Ă���Ȃ畁�ʂɓǂݍ���
		if ( !loop || loop_end < m_audio->tell() )
			return m_audio->read( buffer, length );

		//���[�v���l�������ǂݍ���
		{
			//�t�H�[�}�b�g�擾
			WAVEFORMATEX waveformat;
			m_audio->getWaveFormatEx( &waveformat, sizeof( waveformat ) );

			unsigned long remained = length; //�ǂݍ��ނׂ��c��T�C�Y
			while ( remained > 0 )
			{
				//���݈ʒu�擾
				unsigned long current = m_audio->tell();

				//�ǂݍ��ݗv���T�C�Y�v�Z
				bool loop; //���[�v�t���O
				unsigned long request; //�v���T�C�Y
				if ( loop_end < current + remained ) //���[�v�I���n�_���ׂ��Ƃ�
				{
					loop = true;
					request = loop_end - current; //���[�v�I�����O�܂œǂݍ���
				} else //���[�v�I���n�_���ׂ��Ȃ��Ƃ�
				{
					loop = false;
					request = remained; //�c��T�C�Y��S���ǂݍ���
				}

				//�u���b�N���E�̃`�F�b�N
				if ( request % waveformat.nBlockAlign != 0 )
				{
					LOG( "block align error." );
					request -= request % waveformat.nBlockAlign;
					if ( request == 0 )
						break;
				}

				//�v���T�C�Y����ǂݍ���
				unsigned long read_size = m_audio->read( buffer, request );
				buffer = (unsigned char*)buffer + read_size; //�������݃|�C���^�O�i
				remained -= read_size; //�c��T�C�Y�����炷

				//�v���T�C�Y�Ǝ��ۂɓǂݍ��߂��T�C�Y���Ⴄ�ꍇ
				if ( read_size != request )
				{
					if ( request >= waveformat.nBlockAlign )
					{
						LOG( "read size[%u] differs form requested size[%u] at %u.", read_size, request, m_audio->tell() );
						return length - remained;
					}
				}

				//�K�v�Ȃ烋�[�v�J�n�n�_�ɖ߂�
				if ( loop )
					m_audio->seek( loop_start );
			}
		}
		return length;
	}

	bool DirectSoundRingVoice::fillBuffer( unsigned int index )
	{
		if ( m_division - 1 < index )
			return false;

		unsigned long block_size = m_dsbuffer_size / m_division;
		LPVOID lpvWrite = 0;
		DWORD dwLength = 0;
		if ( DS_OK == m_dsbuffer.Lock( index*block_size, block_size, &lpvWrite, &dwLength, nullptr, 0, 0 ) )
		{
			//�t�B���C���f�b�N�X�X�V
			m_fill_position[ index ] = m_audio->tell();

			//�o�b�t�@�R�s�[
			unsigned long read_length = read( lpvWrite, dwLength );

			//�R�s�[����Ȃ������̈���N���A
			if ( dwLength > read_length )
			{
				ZeroMemory( (unsigned char*)lpvWrite + read_length, dwLength - read_length );
				LOG( "%d bytes cleared", dwLength - read_length );
			}

			m_dsbuffer.Unlock( lpvWrite, dwLength, nullptr, 0 );
		}

		return true;
	}

	void DirectSoundRingVoice::terminateThread( Thread::CriticalSection&& cs )
	{
		if ( !threadAlive() )
			return;

		//�X���b�h�I���v�����s
		m_thread_terminate = true;

		//�N���e�B�J���Z�N�V�������n���ꂽ�Ȃ��, ���b�N����
		bool lock = false;
		if ( cs != nullptr )
		{
			lock = true;
			cs.reset();
		}

		//�X���b�h�I���҂����[�v
#ifndef _DEBUG
		while ( threadAlive( 50 ) );
#else
		for ( int i = 0; threadAlive( 50 ); i++ )
			if ( i >= 20 )
			{
				// �N���e�B�J���Z�N�V����������, �����Ȃ��ł��̃��\�b�h���Ăяo����
				// �����ɓ��B����.
				// (threadProcedure�̏I�����肪�N���e�B�J���Z�N�V������K�v�Ƃ��邽��.)
				LOG( "Seems dead lock." );
				abort();
			}
#endif

		// �ă��b�N
		if ( lock )
			cs = threadLock();
	}

	inline unsigned long DirectSoundRingVoice::tellByte() const
	{
		//���[�v�ʒu�擾
		unsigned long loop_start = toByte( m_loop_start );
		unsigned long loop_end = toByte( m_loop_end );

		//DirectSound�o�b�t�@�̍Đ��ʒu���擾
		unsigned long play_cursor, write_cursor;
		HRESULT result = m_dsbuffer.GetCurrentPosition( &play_cursor, &write_cursor );

		//
		int block_index = getCurrentBlockIndex();
		unsigned long fill_position = m_fill_position[ block_index ];
		unsigned long position = fill_position + play_cursor % ( m_dsbuffer_size / m_division );

		//���[�v�߂�v�Z
		if ( m_loop )
		{
			if ( loop_start < loop_end && fill_position < loop_end && loop_end <= position )
				position = position % loop_end + loop_start;
			else if ( loop_start == loop_end && m_audio->size() <= position )
				position = position % m_audio->size();
		}

		if ( !SUCCEEDED( result ) )
		{
			LOG( "GetCurrentPosition Failed." );
			return 0;
		}

		//����
		return position;
	}

	inline int DirectSoundRingVoice::getCurrentBlockIndex() const
	{
		DWORD play_cursor, write_cursor;
		if ( m_dsbuffer.GetCurrentPosition( &play_cursor, &write_cursor ) != DS_OK )
			return 0;

		return play_cursor / ( m_dsbuffer_size / m_division );
	}

};