#include <math.h>
#include <windows.h>
#include <dsound.h>

#include "debug.h"

#include "IFile.h"
#include "IAudioHandler.h"
#include "IVoice.h"

#include "DirectSoundBufferWrapper.h"
#include "DirectSoundVoice.h"

namespace Thaleia
{
	//----------------------------------------------------------
	bool DirectSoundVoice::bind( std::unique_ptr<IAudioHandler>&& audio )
	{
		//���Ƀo�C���h�ς݂Ȃ�΃G���[
		if ( m_audio != nullptr )
		{
			LOG( "This instance is already binded with PCM." );
			return false;
		}

		//DirectSound��NULL�Ȃ�΃G���[
		if ( m_directsound == nullptr )
		{
			LOG( "Internal DirectSound pointer is nullptr." );
			return false;
		}

		//������NULL�Ȃ�΃G���[
		if ( audio == nullptr )
		{
			LOG( "Argument 'audio' is nullptr." );
			return false;
		}

		//�o�b�t�@�T�C�Y�擾
		unsigned long buffer_size = audio->size();
		if ( buffer_size == 0 )
		{
			LOG( "PCM size is zero." );
			return false;
		}

		//�t�H�[�}�b�g�擾
		WAVEFORMATEX format;
		audio->getWaveFormatEx( &format, sizeof( format ) );

		//�o�b�t�@������
		{
			bool result = m_dsbuffer.initialize(
				m_directsound,
				audio->size(),
				&format
				);
			if ( !result )
			{
				LOG( "DirectSoundBuffer initializing failed." );
				return false;
			}
		}

		//�o�b�t�@�ɃI�[�f�B�I��W�J
		{
			//�I�[�f�B�I���̓ǂݍ��݈ʒu��������.
			audio->seek( 0 );

			//�������݊֘A�̕ϐ���������.
			void* write_cursor = nullptr;  ///�������݃A�h���X
			unsigned long locked_size = 0; ///�������݃��b�N���Ă���T�C�Y(�o�C�g��)

			//�������݃��b�N�𓾂�.
			{
				HRESULT result = m_dsbuffer.Lock( 0, buffer_size, &write_cursor, &locked_size, nullptr, nullptr, 0 );

				//Lock�̌��ʂ�DS_OK�łȂ���΃G���[.
				if ( result != DS_OK )
				{
					LOG( "DirectSoundBuffer cannot be locked." );
					m_dsbuffer.release();
					return false;
				}

				//���b�N�T�C�Y���v���T�C�Y��菬������΃G���[.
				if ( locked_size < buffer_size )
				{
					LOG( "locked size is too small." );
					m_dsbuffer.Unlock( write_cursor, locked_size, nullptr, 0 );
					m_dsbuffer.release();
				}
			}

			//DirectSound�o�b�t�@�ɃI�[�f�B�I���f�[�^���R�s�[����.
			unsigned long copied_size = audio->read( write_cursor, locked_size );

			//�������݃��b�N���������.
			m_dsbuffer.Unlock( write_cursor, locked_size, nullptr, 0 );

			//�R�s�[�ɐ��������T�C�Y���v���T�C�Y��菬�����Ȃ�΃G���[.
			if ( copied_size != buffer_size )
			{
				LOG( "PCM reading failed." );
				m_dsbuffer.release();
				return false;
			}
		}

		//�I�[�f�B�I�̏��L�����擾
		m_audio = std::move( audio );

		//����
		return true;
	}

	std::unique_ptr<IAudioHandler> DirectSoundVoice::unbind()
	{
		//�o�b�t�@�J��
		m_dsbuffer.Stop();
		m_dsbuffer.release();

		//�ݒ菉����
		m_loop = false;

		//PCM�̏��L����ԋp
		return std::move( m_audio );
	}

	void DirectSoundVoice::play()
	{
		if ( isPlaying() )
			seek( 0 );
		else
			m_dsbuffer.Play( 0, 0, m_loop ? DSBPLAY_LOOPING : 0 );
	}

	void DirectSoundVoice::pause()
	{
		m_dsbuffer.Stop();
	}

	void DirectSoundVoice::stop()
	{
		m_dsbuffer.Stop();
		m_dsbuffer.SetCurrentPosition( 0 );
	}

	unsigned long DirectSoundVoice::tell() const
	{
		unsigned long play_cursor = 0;
		m_dsbuffer.GetCurrentPosition( &play_cursor, nullptr );

		return toMillisecond( play_cursor );
	}

	void DirectSoundVoice::seek( unsigned long millisecond )
	{
		unsigned long moveto = toByte( millisecond );
		if ( moveto >= m_audio->size() )
			moveto = m_audio->size() - 1; // �I�[�s�b�^���ɂ͈ړ��ł��Ȃ�. Align��DirectSound���������Ă����.

		m_dsbuffer.SetCurrentPosition( moveto );
	}

	double DirectSoundVoice::getVolume() const
	{
		return m_dsbuffer.getVolume();
	}

	double DirectSoundVoice::getPan() const
	{
		return m_dsbuffer.getPan();
	}

	unsigned int DirectSoundVoice::getFrequency() const
	{
		return m_dsbuffer.getFrequency();
	}

	bool DirectSoundVoice::isLoopEnabled() const
	{
		return m_loop;
	}

	bool DirectSoundVoice::getLoopTime( unsigned long* start, unsigned long* end ) const
	{
		return false;
	}

	void DirectSoundVoice::setVolume( double volume )
	{
		m_dsbuffer.setVolume( volume );
	}

	void DirectSoundVoice::setPan( double pan )
	{
		m_dsbuffer.setPan( pan );
	}

	void DirectSoundVoice::setFrequency( unsigned int frequency )
	{
		m_dsbuffer.setFrequency( frequency );
	}

	bool DirectSoundVoice::enableLoop( bool enable )
	{
		m_loop = enable;
		return true;
	}

	bool DirectSoundVoice::setLoopTime( unsigned long start, unsigned long end )
	{
		return false;
	}

	bool DirectSoundVoice::isPlaying() const
	{
		unsigned long status;
		if ( m_dsbuffer.GetStatus( &status ) != DS_OK )
			return false;

		return status & DSBSTATUS_PLAYING;
	}

};