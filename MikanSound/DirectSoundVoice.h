#pragma once

#include "DirectSoundBufferWrapper.h"

namespace Thaleia
{

	/// ���`�o�b�t�@�𗘗p����DirectSound�v���C���[.
	/// ���[�v��PCM�S�݂̂̂ŉ\��, ���[�v��Ԃ�ݒ肷�邱�Ƃ͂ł��Ȃ�.
	class DirectSoundVoice : public IVoice
	{
		//----------------------------------------------------------
		// IVoice

		// public method (override)
	public:
		bool bind( std::unique_ptr<IAudioHandler>&& audio );
		std::unique_ptr<IAudioHandler> unbind();
		void play();
		void pause();
		void stop();
		unsigned long size() const
		{
			return 0;
		}///////////////////////////////////////////TODO
		unsigned long tell() const;
		void seek( unsigned long millisecond );
		double getVolume() const;
		double getPan() const;
		unsigned int getFrequency() const;
		bool isLoopEnabled() const;
		bool getLoopTime( unsigned long* start, unsigned long* end ) const;
		void setVolume( double volume );
		void setPan( double pan );
		void setFrequency( unsigned int frequency );
		bool enableLoop( bool enable );
		bool setLoopTime( unsigned long start, unsigned long end );
		bool isPlaying() const;

		//----------------------------------------------------------
		// DirectSoundVoice

		// private variable
	private:
		IDirectSound* m_directsound;			/// DirectSound�̃C���X�^���X. �R���X�g���N�^�Ŏ󂯎��.
		DirectSoundBufferWrapper m_dsbuffer;	/// DirectSound�o�b�t�@�̃C���X�^���X.

		std::unique_ptr<IAudioHandler> m_audio;	/// �I�[�f�B�I�f�[�^.
		bool m_loop;							/// ���[�v�Đ����s�����ۂ�.

		// public constructor
	public:
		DirectSoundVoice( IDirectSound* directsound ) :
			m_directsound( directsound ),
			m_loop( false ),
			m_audio( nullptr )
		{

		}

		virtual ~DirectSoundVoice()
		{
			//PCM�n���h�������
			unbind();
		}

		// private method
	private:
		/// �o�C�g�����玞�Ԓ����v�Z����.
		inline unsigned long toMillisecond( unsigned long byte ) const
		{
			if ( m_audio == nullptr )
				return 0;

			WaveFormatEx format;
			m_audio->getWaveFormatEx( &format, sizeof( format ) );
			return IVoice::toMillisecond( byte, format.channels, format.bits_per_sample, format.samples_per_second );
		}

		/// ���Ԓ�����o�C�g�����v�Z����.
		inline unsigned long toByte( unsigned long millisecond ) const
		{
			if ( m_audio == nullptr )
				return 0;

			WaveFormatEx format;
			m_audio->getWaveFormatEx( &format, sizeof( format ) );
			return IVoice::toByte( millisecond, format.channels, format.bits_per_sample, format.samples_per_second );
		}
	};

};