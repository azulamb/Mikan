#pragma once

#include "DirectSoundBufferWrapper.h"
#include "Thread.h"

namespace Thaleia
{

	/// 循環バッファを利用するDirectSoundプレイヤー.
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
		IDirectSound* m_directsound;			/// DirectSoundのインスタンス. コンストラクタで受け取る.
		std::unique_ptr<IAudioHandler> m_audio;	/// PCMデータ

		//DirectSound関連
		DirectSoundBufferWrapper m_dsbuffer;	/// DirectSoundバッファのインスタンス.
		unsigned long m_dsbuffer_size;			/// セカンダリバッファの大きさ
		unsigned int m_division;				/// セカンダリバッファの分割ブロック数
		int m_last_index;						/// 最後にバッファを充填したブロックの位置
		unsigned long* m_fill_position;			/// バッファを充填したブロックへのリソースの書き込み開始位置

		//ループ関連
		bool m_loop;							/// ループ再生を行うか否か.
		unsigned long m_loop_start;				/// ループ開始ミリ秒
		unsigned long m_loop_end;				/// ループ終了ミリ秒

		//スレッド関連
		volatile bool m_thread_terminate;		/// スレッドに終了要求を出しているかどうか

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
			//PCMハンドラを切り離し, 解放する.
			unbind();
		}

		// private method
	private:
		/// ループを考慮したpcm読み取り関数
		unsigned long read( void* buffer, unsigned long length );

		/// セカンダリバッファをm_division個に分割したときの
		/// index番目のブロックにバッファを充填する. 
		/// indexは0からm_division-1番目までとする. 
		bool fillBuffer( unsigned int index );

		/// ストリーム充填スレッドに停止要求出し, 停止するまで待つ.
		/// クリティカルセクション内から呼び出したい場合,
		/// デッドロックを回避するためにクリティカルセクションオブジェクトを渡すこと.
		void terminateThread( Thread::CriticalSection&& cs = nullptr );

		inline unsigned long tellByte() const;

		inline int getCurrentBlockIndex() const;

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