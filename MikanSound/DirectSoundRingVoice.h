#pragma once

#include "DirectSoundBufferWrapper.h"
#include "Thread.h"

namespace Thaleia
{

	/// �z�o�b�t�@�𗘗p����DirectSound�v���C���[.
	class DirectSoundRingVoice :
		public IVoice,
		private Thread
	{
		//----------------------------------------------------------
		// IVoice

		// public method (override)
	public:
		bool bind( std::unique_ptr<IAudioHandler>&& pcm );
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
		// Thread

		// private method (override)
	private:
		void threadProcedure();

		//----------------------------------------------------------
		// DirectSoundRingVoice

		// private constance
	private:
		static const unsigned long DEFAULT_BUFFER_SIZE = 1 * 1024 * 1024;	/// 
		static const unsigned long DEFAULT_DIVISION_LENGTH = 4;				/// 

		// private variable
	private:
		//	Thaleia* m_thaleia;
		IDirectSound* m_directsound;			/// DirectSound�̃C���X�^���X. �R���X�g���N�^�Ŏ󂯎��.
		std::unique_ptr<IAudioHandler> m_audio;	/// PCM�f�[�^

		//DirectSound�֘A
		DirectSoundBufferWrapper m_dsbuffer;	/// DirectSound�o�b�t�@�̃C���X�^���X.
		unsigned long m_dsbuffer_size;			/// �Z�J���_���o�b�t�@�̑傫��
		unsigned int m_division;				/// �Z�J���_���o�b�t�@�̕����u���b�N��
		int m_last_index;						/// �Ō�Ƀo�b�t�@���[�U�����u���b�N�̈ʒu
		unsigned long* m_fill_position;			/// �o�b�t�@���[�U�����u���b�N�ւ̃��\�[�X�̏������݊J�n�ʒu

		//���[�v�֘A
		bool m_loop;							/// ���[�v�Đ����s�����ۂ�.
		unsigned long m_loop_start;				/// ���[�v�J�n�~���b
		unsigned long m_loop_end;				/// ���[�v�I���~���b

		//�X���b�h�֘A
		volatile bool m_thread_terminate;		/// �X���b�h�ɏI���v�����o���Ă��邩�ǂ���

		// public constructor
	public:
		DirectSoundRingVoice( IDirectSound* directsound ) :
			m_directsound( directsound ),
			m_audio( nullptr ),
			m_dsbuffer_size( 0 ),
			m_division( DEFAULT_DIVISION_LENGTH ),
			m_last_index( -1 ),
			m_fill_position( nullptr ),
			m_loop( false ),
			m_loop_start( 0 ),
			m_loop_end( 0 ),
			m_thread_terminate( false )
		{

		}

		virtual ~DirectSoundRingVoice()
		{
			//PCM�n���h����؂藣��, �������.
			unbind();
		}

		// private method
	private:
		/// ���[�v���l������pcm�ǂݎ��֐�
		unsigned long read( void* buffer, unsigned long length );

		/// �Z�J���_���o�b�t�@��m_division�ɕ��������Ƃ���
		/// index�Ԗڂ̃u���b�N�Ƀo�b�t�@���[�U����. 
		/// index��0����m_division-1�Ԗڂ܂łƂ���. 
		bool fillBuffer( unsigned int index );

		/// �X�g���[���[�U�X���b�h�ɒ�~�v���o��, ��~����܂ő҂�.
		/// �N���e�B�J���Z�N�V����������Ăяo�������ꍇ,
		/// �f�b�h���b�N��������邽�߂ɃN���e�B�J���Z�N�V�����I�u�W�F�N�g��n������.
		void terminateThread( Thread::CriticalSection&& cs = nullptr );

		inline unsigned long tellByte() const;

		inline int getCurrentBlockIndex() const;

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