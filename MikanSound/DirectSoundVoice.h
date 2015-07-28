#pragma once

#include "DirectSoundBufferWrapper.h"

namespace Thaleia
{

	/// 線形バッファを利用するDirectSoundプレイヤー.
	/// ループはPCM全体のみで可能で, ループ区間を設定することはできない.
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
		IDirectSound* m_directsound;			/// DirectSoundのインスタンス. コンストラクタで受け取る.
		DirectSoundBufferWrapper m_dsbuffer;	/// DirectSoundバッファのインスタンス.

		std::unique_ptr<IAudioHandler> m_audio;	/// オーディオデータ.
		bool m_loop;							/// ループ再生を行うか否か.

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
			//PCMハンドラを解放
			unbind();
		}

		// private method
	private:
		/// バイト数から時間長を計算する.
		inline unsigned long toMillisecond( unsigned long byte ) const
		{
			if ( m_audio == nullptr )
				return 0;

			WaveFormatEx format;
			m_audio->getWaveFormatEx( &format, sizeof( format ) );
			return IVoice::toMillisecond( byte, format.channels, format.bits_per_sample, format.samples_per_second );
		}

		/// 時間長からバイト数を計算する.
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